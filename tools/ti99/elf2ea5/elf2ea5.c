/*
This code is based on version 1.2 of the ELF specification

This program converts an ELF32 source file for a TMS9900 target into a
TI99/4A EA5 binary file.

This is done by copying the .text and .data sections to the output file.
The .text section is placed at the start of the file, and the .data section
is placed immediately after that.

Assumptions:

The entry point for the executable is at the start of the .text ssection.
There is a "_init_data" symbol followed by space for a ten byte record.
That record holds the data required to initialize the .data and.bss 
sections once the EA5 image has been loaded.
*/


/*
 ****************************************************************************
 *                                Header Files
 ****************************************************************************
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "elf.h"


/*
 ****************************************************************************
 *                                Data Types
 ****************************************************************************
 */

// Structure used to store initialization data
typedef struct
{
   short data_addr;    // Start of data section
   short data_src;     // Location of initial contents of data section
   short data_size;    // Size of data section
   short bss_addr;     // Start of BSS section
   short bss_size;     // Size of BSS section
} init_data_struct;


/*
 ****************************************************************************
 *                            Function Prototypes
 ****************************************************************************
 */

static unsigned int   reverseBytes4(unsigned int value);
static unsigned short reverseBytes2(unsigned short value);
static unsigned int   dontReverseBytes4(unsigned int value);
static unsigned short dontReverseBytes2(unsigned short value);

static void convertElfHeader(Elf32_Ehdr *ptr);
static void convertSymbolEntry(Elf32_Sym *ptr);
static void convertSectionHeader(Elf32_Shdr *ptr);


/*
 ****************************************************************************
 *                                Global Data
 ****************************************************************************
 */

// Pointers to file data translation functions, assume no translation needed
static unsigned int   (*convert4)(unsigned int value)   = dontReverseBytes4;
static unsigned short (*convert2)(unsigned short value) = dontReverseBytes2;

static Elf32_Ehdr  header;  // The ELF header for this file


/*
 ****************************************************************************
 *                                   Code
 ****************************************************************************
 */


/*===========================================================================
 *                                copy_block
 *===========================================================================
 * Description: Copy a block of data
 *
 * Parameters:  input  - Input file
 *              output - Output file
 *              size   - Number of bytes to copy
 *
 * Returns:     1 - None
 */
int copy_block(FILE* input, FILE* output, int size)
{
   char buffer[1024];
   int readsize = 1024;

   if(size < readsize) readsize = size;
   while(size >= readsize && size > 0)
   {
      if(fread(buffer, readsize, 1, input) <= 0)
      {
         return(0);               
      }
      fwrite(buffer, readsize, 1, output);
      size -= readsize;
      if(size < readsize) readsize = size;
   }
   return(1);
}


/*
 *===========================================================================
 *                                main
 *===========================================================================
 * Description: Entry point for program
 *
 * Parameters:  argc - Number of arguments
 *              argv - Argument list
 *
 * Returns:     
 */
int main(int argc, char **argv)
{
   FILE       *input;         // The currently open ELF file
   FILE       *output;        // The output file
   Elf32_Shdr *all_sections;  // Section header entry
   Elf32_Shdr *section;       // Section header entry
   int         i;             // Section entry counter
   Elf32_Sym   symbol;        // Symbol table entry
   char *strtab;              // Copy of the string table
   int init_data_addr = 0;    // Address of "_init_data" symbol

   Elf32_Shdr *text_section   = NULL;
   Elf32_Shdr *data_section   = NULL;
   Elf32_Shdr *bss_section    = NULL;
   Elf32_Shdr *symtab_section = NULL;
   Elf32_Shdr *strtab_section = NULL;

   int size;
   init_data_struct init_data;
   short ea5_header[3];
   int image_size;

   // Check arguments
   if(argc < 3)
   {
      printf("Usage:\n");
      printf("elf2cart <elf_file> <cart_file>\n");
      printf("\n");

      return(1);
   }

   // Attempt to open file
   input = fopen(argv[1], "rb");
   if(input == NULL)
   {
      // Could not open file
      printf("Cannot open \"%s\"\n", argv[1]);
      return(1);
   }

   // Skip to start of the section entries
   fseek(input, header.e_shoff, SEEK_SET);

   // Read ELF header
   if(fread(&header, sizeof(Elf32_Ehdr), 1, input) <= 0)
   {
      printf("Error: File too short, invalid ELF header\n");
      return(1);
   }

   // Validate this as an ELF file
   if(header.e_ident[0] != 0x7F ||
      header.e_ident[1] != 'E'  ||
      header.e_ident[2] != 'L'  ||
      header.e_ident[3] != 'F')
   {
      // This is not a valid ELF file
      printf("Error: Missing signature, this is not an ELF file\n");
      return(1);
   }

   // Determine endianness of the file
   if(header.e_ident[EI_DATA] == ELFDATA2MSB)
   {
      // We need to reverse all bytes read or saved from the file
      convert4 = reverseBytes4;
      convert2 = reverseBytes2;
   }
   else
   {
      // No need to reverse data, we can use it natively
      convert4 = dontReverseBytes4;
      convert2 = dontReverseBytes2;
   }

   // Convert the header data to native format
   convertElfHeader(&header);
/*
   printf("\nELF Header\n");
   printf("====================\n");
   printf("type      = 0x%X\n", header.e_type     );
   printf("machine   = 0x%X\n", header.e_machine  );
   printf("version   = 0x%X\n", header.e_version  );
   printf("entry     = 0x%X\n", header.e_entry    );
   printf("phoff     = 0x%X\n", header.e_phoff    );
   printf("shoff     = 0x%X\n", header.e_shoff    );
   printf("flags     = 0x%X\n", header.e_flags    );
   printf("ehsize    = 0x%X\n", header.e_ehsize   );
   printf("phentsize = 0x%X\n", header.e_phentsize);
   printf("phnum     = 0x%X\n", header.e_phnum    );
   printf("shentsize = 0x%X\n", header.e_shentsize);
   printf("shnum     = 0x%X\n", header.e_shnum    );
   printf("shstrndx  = 0x%X\n", header.e_shstrndx );
*/
   // Make sure this is a TMS9900 file
   if(header.e_machine != 0xABBA)
   {
      printf("Error: Not for TMS9900, cannot convert\n");
      return(1);
   } 

   // Read the section headers

   // Move file pointer to the start of the headers
   fseek(input, header.e_shoff, SEEK_SET);

   // Load all section entries
   all_sections = malloc(header.e_shnum * sizeof(Elf32_Shdr));
   for(i = 0; i < header.e_shnum; i++)
   {
      // Read in a section entry
      section = &all_sections[i];
      if(fread(section, sizeof(Elf32_Shdr), 1, input) <= 0)
      {
         printf("Error: Early end of file\n");
         return(1);               
      }

      // Convert to native format
      convertSectionHeader(section);
   }

   // Get section names  
   for(i = 0; i < header.e_shnum; i++)
   {
      char name[64];
      section = &all_sections[i];

      fseek(input,
            all_sections[header.e_shstrndx].sh_offset +
            section->sh_name, SEEK_SET);
      if(fread(name, 64, 1, input) <= 0)
      {
         printf("Error: Early end of file\n");
         return(1);               
      }      

      if(strcmp(name, ".text") == 0)     text_section   = section;
      if(strcmp(name, ".data") == 0)     data_section   = section;
      if(strcmp(name, ".bss" ) == 0)     bss_section    = section;
      if(section->sh_type == SHT_SYMTAB) symtab_section = section;
      if(section->sh_type == SHT_STRTAB) strtab_section = section;
/*            
      printf("\n\nSECTION %d\n",i);
      printf("====================\n");
      printf("name=%s\n", name);
      printf("sh_name      = %d\n", section->sh_name     );
      printf("sh_type      = %d\n", section->sh_type     );
      printf("sh_flags     = %d\n", section->sh_flags    );
      printf("sh_addr      = %d\n", section->sh_addr     );
      printf("sh_offset    = %d\n", section->sh_offset   );
      printf("sh_size      = %d\n", section->sh_size     );
      printf("sh_link      = %d\n", section->sh_link     );
      printf("sh_info      = %d\n", section->sh_info     );
      printf("sh_addralign = %d\n", section->sh_addralign);
      printf("sh_entsize   = %d\n", section->sh_entsize  );
*/
   }

   // If there's no .text section, there's no code to execute
   if(text_section == NULL)
   {
      printf("Error: No .text section found\n");
      return(1);
   }

   // Build initialization structure
   // If the .data section is located immediately after the .text 
   // section n the loaded image, no need to fill out the .data
   // initializer fields as the TI loader will take care of that
   // for us when it loads the EA5 file.
   memset(&init_data, 0, sizeof(init_data_struct));
   if(data_section != NULL &&
      data_section->sh_addr != text_section->sh_addr +
                               text_section->sh_size)
   {
      init_data.data_addr   = convert2(data_section->sh_addr);
      init_data.data_size   = convert2(data_section->sh_size);
      init_data.data_src    = convert2(text_section->sh_addr +
                                       text_section->sh_size);
   }
   if(bss_section != NULL)
   {
      init_data.bss_addr    = convert2(bss_section->sh_addr);
      init_data.bss_size    = convert2(bss_section->sh_size);
   }

   // Try to find "_init_data" symbol
   if(strtab_section == NULL)
   {
      printf("Warning: No string table section found\n");
      return(1);
   }
   else
   {
      // Load symbol table into memory
      strtab = malloc(strtab_section->sh_size);
      fseek(input, strtab_section->sh_offset, SEEK_SET);
      if(fread(strtab, strtab_section->sh_size, 1, input) <= 0)
      {
         printf("Error: Early end of file\n");
         return(1);               
      }      

      // Find "_init_data" symbol
      if(symtab_section == NULL)
      {
         // If there is no symbol table, we can't fill out the
         // initialization record. That might still be OK.
         printf("Warning: No symbol table section found\n");
      }
      else 
      {
         // Read through the symbol table entries 
         fseek(input, symtab_section->sh_offset, SEEK_SET);
         for(i=0; i<symtab_section->sh_size; i+=sizeof(Elf32_Sym))
         {
            if(fread(&symbol, sizeof(Elf32_Sym), 1, input) <= 0)
            {
               printf("Error: Early end of file\n");
               return(1);               
            }      
            if(strcmp("_init_data", &strtab[convert4(symbol.st_name)]) == 0)
            {
               init_data_addr = convert4(symbol.st_value);
               break;
            }
         }
      }
      free(strtab);
   }

   // Open output file   
   output = fopen(argv[2], "wb");
   if(output == NULL)
   {
      // Could not open file
      printf("Error: Cannot open \"%s\"\n", argv[2]);
      return(1);
   }

   // Calculate image size (.text + .data)
   image_size = text_section->sh_size;
   if(data_section != NULL)
     image_size += data_section->sh_size;

   // Write EA5 header
   //
   // The EA5 loader assumes the entry point is the load address
   ea5_header[0] = 0;                                // Last in file chain
   ea5_header[1] = convert2(image_size + 6);         // Image + header size
   ea5_header[2] = convert2(text_section->sh_addr);  // Load address
   fwrite(ea5_header, 6, 1, output);

   // Copy all .text section data to "_init_data" symbol
   fseek(input, text_section->sh_offset, SEEK_SET);
   size = text_section->sh_size;
   if(init_data_addr == 0)
   {
      printf("Warning: no \"_init_data\" symbol found\n");
   }
   else
   {
      if(copy_block(input, output, 
                    init_data_addr - text_section->sh_addr) == 0)
      {
         printf("Error: Early end of file\n");
         return(1);               
      }
      // Save initialization record to the output file
      fwrite(&init_data, sizeof(init_data_struct), 1, output);

      // Skip the initialization record bytes in the input file
      if(fread(&init_data, sizeof(init_data_struct), 1, input) <= 0)
      {
         printf("Early end of file, giving up\n");
         return(1);               
      }

      // Adjust size for remaining part      
      size -= init_data_addr - text_section->sh_addr + sizeof(init_data_struct);
   }

   // Copy all .text data after "_init_data" symbol
   if(copy_block(input, output, size) == 0)
   {
      printf("Error: Early end of file\n");
      return(1);               
   }

   // Copy .data section
   if(data_section != NULL)
   {
      fseek(input, data_section->sh_offset, SEEK_SET);
      if(copy_block(input, output, data_section->sh_size) == 0)
      {
         printf("Error: Early end of file\n");
         return(1);               
      }
   }

   // All done, clean up
   fclose(input);
   fclose(output);

   return(0);
}


/*
 *===========================================================================
 *                               reverseBytes4
 *===========================================================================
 * Description: Reverse the order of byytes in a 4-byte value
 *
 * Parameters:  value - The value to reverse
 *
 * Returns:     int - The reversed value
 */
static unsigned int reverseBytes4(unsigned int value)
{
   return( ((value & 0x000000FF) << 24) |
           ((value & 0x0000FF00) << 8 ) |
           ((value & 0x00FF0000) >> 8 ) |
           ((value & 0xFF000000) >> 24) );
}


/*
 *===========================================================================
 *                               reverseBytes2
 *===========================================================================
 * Description: Reverse the order of byytes in a 2-byte value
 *
 * Parameters:  value - The value to reverse
 *
 * Returns:     int - The reversed value
 */
static unsigned short reverseBytes2(unsigned short value)
{
   return( ((value & 0x00FF) << 8) |
           ((value & 0xFF00) >> 8) );
}


/*
 *===========================================================================
 *                               dontReverseBytes4
 *===========================================================================
 * Description: Do nothing to the value, just return it
 *
 * Parameters:  value - The value to ignore
 *
 * Returns:     int - The ignored value
 */
static unsigned int dontReverseBytes4(unsigned int value)
{
   return(value);
}


/*
 *===========================================================================
 *                            dontReverseBytes2
 *===========================================================================
 * Description: Do nothing to the value, just return it
 *
 * Parameters:  value - The value to ignore
 *
 * Returns:     int - The ignored value
 */
static unsigned short dontReverseBytes2(unsigned short value)
{
   return(value);
}


/*
 *===========================================================================
 *                            convertElfHeader
 *===========================================================================
 * Description: Convert the ELF header to/from native format
 *
 * Parameters:  ptr - Pointer to the header
 *
 * Returns:     None
 */
void convertElfHeader(Elf32_Ehdr *ptr)
{
   ptr->e_type      = convert2(ptr->e_type     );
   ptr->e_machine   = convert2(ptr->e_machine  );
   ptr->e_version   = convert4(ptr->e_version  );
   ptr->e_entry     = convert4(ptr->e_entry    );
   ptr->e_phoff     = convert4(ptr->e_phoff    );
   ptr->e_shoff     = convert4(ptr->e_shoff    );
   ptr->e_flags     = convert4(ptr->e_flags    );
   ptr->e_ehsize    = convert2(ptr->e_ehsize   );
   ptr->e_phentsize = convert2(ptr->e_phentsize);
   ptr->e_phnum     = convert2(ptr->e_phnum    );
   ptr->e_shentsize = convert2(ptr->e_shentsize);
   ptr->e_shnum     = convert2(ptr->e_shnum    );
   ptr->e_shstrndx  = convert2(ptr->e_shstrndx );
}


/*
 *===========================================================================
 *                            convertSectionHeader
 *===========================================================================
 * Description: Convert the section header to/from native format
 *
 * Parameters:  ptr - Pointer to the header
 *
 * Returns:     None
 */
void convertSectionHeader(Elf32_Shdr *ptr)
{
   ptr->sh_name      = convert4(ptr->sh_name     );
   ptr->sh_type      = convert4(ptr->sh_type     );
   ptr->sh_flags     = convert4(ptr->sh_flags    );
   ptr->sh_addr      = convert4(ptr->sh_addr     );
   ptr->sh_offset    = convert4(ptr->sh_offset   );
   ptr->sh_size      = convert4(ptr->sh_size     );
   ptr->sh_link      = convert4(ptr->sh_link     );
   ptr->sh_info      = convert4(ptr->sh_info     );
   ptr->sh_addralign = convert4(ptr->sh_addralign);
   ptr->sh_entsize   = convert4(ptr->sh_entsize  );
}


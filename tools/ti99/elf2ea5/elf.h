// This code is based on version 1.2 of the ELF specification


#ifndef ELFFORMAT_H
#define ELFFORMAT_H

/*
 *******************************************************************************
 *                               Header Files
 *******************************************************************************
 */


/*
 *******************************************************************************
 *                             Defined Constants
 *******************************************************************************
 */

// Size of the ELF identification string
#define EI_NIDENT 16


// These are indexes into the ELF file header "e_ident" arra
enum
{
   EI_MAG0    = 0,  // File identification (0x7F)
   EI_MAG1    = 1,  // File identification ('E')
   EI_MAG2    = 2,  // File identification ('L')
   EI_MAG3    = 3,  // File identification ('F')
   EI_CLASS   = 4,  // File class
   EI_DATA    = 5,  // Data encoding
   EI_VERSION = 6,  // File version
   EI_PAD     = 7   // Start of padding bytes
};

// These are values for the Elf32_Ehdr.e_ident[EI_CLASS] record
enum
{
   ELFCLASSNONE = 0,  // Invalid class
   ELFCLASS32   = 1,  // 32-bit objects
   ELFCLASS64   = 2   // 64-bit objects
};

// These are values for the Elf32_Ehdr.e_ident[EI_DATA] record
enum
{
   ELFDATANONE = 0,  // Invalid data encoding
   ELFDATA2LSB = 1,  // Data stored in LSB format (0x01020304 -> [04][03][02][01])
   ELFDATA2MSB = 2,  // Data stored in MSB format (0x01020304 -> [01][02][03][04])
};

// These are special section ID numbers
enum
{
   SHN_UNDEF     = 0,       // An unused or undefined section
   SHN_LORESERVE = 0xff00,  // Lower limit of reserved section ID's
   SHN_LOPROC    = 0xff00,  // Lower limit of processor-specific section ID's
   SHN_HIPROC    = 0xff1f,  // Upper limit of processor-specific section ID's
   SHN_ABS       = 0xfff1,  // Symbols located reletive to ABS cannot be relocated
   SHN_COMMON    = 0xfff2,  // Symbols located relative to COMMON are treated as unallocated global data
   SHN_HIRESERVE = 0xffff   // Upper limit of reserved section ID's
};

// Section type ID's
enum
{
   SHT_NULL     = 0,           // Section is unused
   SHT_PROGBITS = 1,           // Section holds program data
   SHT_SYMTAB   = 2,           // Section holds a symbol table
   SHT_STRTAB   = 3,           // Section holds a string table
   SHT_RELA     = 4,           // Section holds relocation info with explicit addends
   SHT_HASH     = 5,           // Section holds symbol hash table
   SHT_DYNAMIC  = 6,           // Section holds dynamic linking information
   SHT_NOTE     = 7,           // Section holds note information
   SHT_NOBITS   = 8,           // Section occupies no space in the file
   SHT_REL      = 9,           // Section holds relocation info without explicit addends
   SHT_SHLIB    = 10,          // Undefined usage
   SHT_DYNSYM   = 11,          // Undefined usage
   SHT_LOPROC   = 0x70000000,  // Lower bound on sections with processor-specific usage 
   SHT_HIPROC   = 0x7fffffff,  // Upper bound on sections with processor-specific usage 
   SHT_LOUSER   = 0x80000000,  // Lower bound on sections for application usage 
   SHT_HIUSER   = 0xffffffff   // Upper bound on sections for application usage 
};

// These are section attribute flags
enum
{
   SHF_WRITE     = 0x1,        // Section is writable 
   SHF_ALLOC     = 0x2,        // Allocate memory for this section
   SHF_EXECINSTR = 0x4,        // Section is executable
   SHF_MASKPROC  = 0xf0000000  // Mask of processor-specific flags
};

// These are symbol binding ID's (stored in upper 4 bits of Elf32_Sym.st_info)
enum
{
   STB_LOCAL  = 0,   // Symbol is local to this file
   STB_GLOBAL = 1,   // Symbol is visible to other files
   STB_WEAK   = 2,   // Similar to GLOBAL, but lower precedence
   STB_LOPROC = 13,  // Lower limit for processor-specific types
   STB_HIPROC = 15   // Upper limit for processor-specific types
};

// These are symbol type ID's (stored in lower 4 bits of Elf32_Sym.st_info)
enum
{
   STT_NOTYPE  = 0,   // Type not specified
   STT_OBJECT  = 1,   // This is a data object
   STT_FUNC    = 2,   // This is a function or executable code
   STT_SECTION = 3,   // This is a section 
   STT_FILE    = 4,   // The source code file for this object file
   STT_LOPROC  = 13,  // Lower limit for processor-specific bindings
   STT_HIPROC  = 15   // Upper limit for processor-specific bindings
};

// These are relocation type ID's (derived from existing object files, possibly incorrect or incomplete)
enum
{
   R_PPC_REL14     = 0x0B,  // A 14-bit relative address, as used in conditional branch instructions
   R_PPC_REL24     = 0x0A,  // A 24-bit relative address, as used in unconditional branch instructions
   R_PPC_ADDR16_HA = 0x06,  // The upper 16-bits of an absolute address
   R_PPC_ADDR16_LO = 0x04,  // The lower 16-bits of an absolute address
   R_PPC_ADDR32    = 0x01   // A full 32-bit absolute address
};


/*
 *******************************************************************************
 *                                Data Types
 *******************************************************************************
 */

typedef unsigned int   Elf32_Addr;   // Unsigned program address
typedef unsigned short Elf32_Half;   // Unsigned medium integer
typedef unsigned int   Elf32_Off;    // Unsigned file offset
typedef int            Elf32_Sword;  // Signed large integer
typedef unsigned int   Elf32_Word;   // Unsigned large integer

// This defines the ELF header
typedef struct 
{
   unsigned char e_ident[EI_NIDENT];  // Elf header identifier
   Elf32_Half    e_type;              // Object file type
   Elf32_Half    e_machine;           // Machine archetecture ID
   Elf32_Word    e_version;           // ELF version number
   Elf32_Addr    e_entry;             // Virtual address of entry point
   Elf32_Off     e_phoff;             // File index to program header table
   Elf32_Off     e_shoff;             // File index to section header table
   Elf32_Word    e_flags;             // Processor specific flags
   Elf32_Half    e_ehsize;            // Size of the ELF header
   Elf32_Half    e_phentsize;         // Size of a program header entry
   Elf32_Half    e_phnum;             // Number of program header entries
   Elf32_Half    e_shentsize;         // Size of a section header entry
   Elf32_Half    e_shnum;             // Number of section header entries
   Elf32_Half    e_shstrndx;          // Section index for section header string table
} Elf32_Ehdr;

// This defines a section header entry
typedef struct 
{
   Elf32_Word sh_name;       // Index in the string table of this section's name
   Elf32_Word sh_type;       // Section type ID
   Elf32_Word sh_flags;      // Section flags
   Elf32_Addr sh_addr;       // Address at which this section appears in process memory
   Elf32_Off  sh_offset;     // File index to start of the section
   Elf32_Word sh_size;       // Size of this section in the file
   Elf32_Word sh_link;       // Section index link
   Elf32_Word sh_info;       // Extra section info
   Elf32_Word sh_addralign;  // Alignment constraints
   Elf32_Word sh_entsize;    // Size of fixed-size entries (if any) in this section
} Elf32_Shdr;

// This defines a symbol table entry
typedef struct 
{
   Elf32_Word    st_name;    // Index in the string table of this section's name
   Elf32_Addr    st_value;   // In relocatable files, this is the alignment of 
                             // the symbol if it is in a COMMON section. 
                             // Otherwise, this is the offset to the symbol from
                             // the start of the section
                             // In exectuable and shared object files, this is
                             // the virtual address of this symbol.

   Elf32_Word    st_size;    // Size of the object
   unsigned char st_info;    // Binding << 4 | Type 
   unsigned char st_other;   // Undefined, not used
   Elf32_Half    st_shndx;   // Index of section this object is in
} Elf32_Sym;

// This defines a relocation record for REL sections
typedef struct 
{
   Elf32_Addr r_offset;  // Executables or shared object: virtual address to fixup
                         // Others: Offset from section start to fixup
   Elf32_Word r_info;    // Symbol index << 8 | Type
} Elf32_Rel;

// This defines a relocation record for RELA sections
typedef struct
{
   Elf32_Addr  r_offset;  // Executables or shared object: virtual address to fixup
                          // Others: Offset from section start to fixup
   Elf32_Word  r_info;    // Symbol index << 8 | Type
   Elf32_Sword r_addend;  // Value to add to the fixup value
} Elf32_Rela;


#endif

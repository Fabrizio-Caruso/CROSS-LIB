//
//      srecord - manipulate eprom load files
//      Copyright (C) 2000-2008, 2010, 2013 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 3 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program. If not, see
//      <http://www.gnu.org/licenses/>.
//

#include <srec_cat/arglex3.h>


srec_cat_arglex3::srec_cat_arglex3(int argc, char **argv) :
    arglex_tool(argc, argv)
{
    static const table_ty table[] =
    {
        { "-Address_Length", token_address_length, },
        { "-Carriage_Return_Life_Feed", token_crlf, },
        { "-Data_Only", token_data_only, },
        { "-DISable", token_disable, },
        { "-ENable", token_enable, },
        { "-Execution_Start_Address", token_execution_start_address, },
        { "-GOTO", token_execution_start_address, },
        { "-HEAder", token_header, },
        { "-Line_LENGth", token_line_length, },
        { "-Line_Termination", token_line_termination, },
        { "-End_Of_Line", token_line_termination, },
        { "-Output_Block_Size", token_output_block_size, },
        { "-Output_Block_Packing", token_output_block_packing, },
        { "-Output_Block_Alignment", token_output_block_align, },

        //
        // This option is intentionally undocumented.  It is preserved
        // for backwards compatibility.
        //
        { "-Start_Address", token_execution_start_address, },

        SRECORD_ARGLEX_END_MARKER
    };

    table_set(table);
}


srec_cat_arglex3::~srec_cat_arglex3()
{
}


// vim: set ts=8 sw=4 et :

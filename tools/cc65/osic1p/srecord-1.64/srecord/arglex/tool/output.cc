//
// srecord - manipulate eprom load files
// Copyright (C) 2001-2004, 2006-2013 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include <srecord/arglex/tool.h>
#include <srecord/output/file/aomf.h>
#include <srecord/output/file/ascii_hex.h>
#include <srecord/output/file/asm.h>
#include <srecord/output/file/atmel_generic.h>
#include <srecord/output/file/basic.h>
#include <srecord/output/file/binary.h>
#include <srecord/output/file/brecord.h>
#include <srecord/output/file/c.h>
#include <srecord/output/file/coe.h>
#include <srecord/output/file/cosmac.h>
#include <srecord/output/file/dec_binary.h>
#include <srecord/output/file/emon52.h>
#include <srecord/output/file/fairchild.h>
#include <srecord/output/file/fastload.h>
#include <srecord/output/file/formatted_binary.h>
#include <srecord/output/file/forth.h>
#include <srecord/output/file/four_packed_code.h>
#include <srecord/output/file/hexdump.h>
#include <srecord/output/file/idt.h>
#include <srecord/output/file/intel.h>
#include <srecord/output/file/intel16.h>
#include <srecord/output/file/logisim.h>
#include <srecord/output/file/mif.h>
#include <srecord/output/file/mem.h>
#include <srecord/output/file/coe.h>
#include <srecord/output/file/mips_flash.h>
#include <srecord/output/file/mos_tech.h>
#include <srecord/output/file/motorola.h>
#include <srecord/output/file/msbin.h>
#include <srecord/output/file/needham.h>
#include <srecord/output/file/os65v.h>
#include <srecord/output/file/ppb.h>
#include <srecord/output/file/ppx.h>
#include <srecord/output/file/signetics.h>
#include <srecord/output/file/spasm.h>
#include <srecord/output/file/spectrum.h>
#include <srecord/output/file/stewie.h>
#include <srecord/output/file/tektronix_extended.h>
#include <srecord/output/file/tektronix.h>
#include <srecord/output/file/ti_tagged.h>
#include <srecord/output/file/ti_tagged_16.h>
#include <srecord/output/file/ti_txt.h>
#include <srecord/output/file/trs80.h>
#include <srecord/output/file/vhdl.h>
#include <srecord/output/file/vmem.h>
#include <srecord/output/file/wilson.h>


srecord::output::pointer
srecord::arglex_tool::get_output()
{
    //
    // skip the -output token
    //
    if (token_cur() == token_output)
        token_next();

    //
    // determine the file name
    //
    std::string fn = "-";
    switch (token_cur())
    {
    case token_stdio:
        token_next();
        // fall through...

    default:
        if (stdout_used)
        {
            fatal_error
            (
                "the standard output may only be named once on the command line"
            );
            // NOTREACHED
        }
        stdout_used = true;
        break;

    case token_string:
        fn = value_string();
        token_next();
        break;
    }

    //
    // determine the file format
    //
    srecord::output::pointer ofp;
    switch (token_cur())
    {
    case token_motorola:
        token_next();
        // fall through...

    default:
        ofp = srecord::output_file_motorola::create(fn);
        break;

    case token_aomf:
        token_next();
        ofp = srecord::output_file_aomf::create(fn);
        break;

    case token_ascii_hex:
        token_next();
        ofp = srecord::output_file_ascii_hex::create(fn);
        break;

    case token_assembler:
        token_next();
        ofp = srecord::output_file_asm::create(fn);
        break;

    case token_atmel_generic_be:
        token_next();
        ofp = srecord::output_file_atmel_generic::create(fn, endian_big);
        break;

    case token_atmel_generic_le:
        token_next();
        ofp = srecord::output_file_atmel_generic::create(fn, endian_little);
        break;

    case token_basic_data:
        token_next();
        ofp = srecord::output_file_basic::create(fn);
        break;

    case token_binary:
        token_next();
        ofp = srecord::output_file_binary::create(fn);
        break;

    case token_brecord:
        token_next();
        ofp = srecord::output_file_brecord::create(fn);
        break;

    case token_c_array:
        token_next();
        ofp = srecord::output_file_c::create(fn);
        break;

    case token_cosmac:
        token_next();
        ofp = srecord::output_file_cosmac::create(fn);
        break;

    case token_dec_binary:
        token_next();
        ofp = srecord::output_file_dec_binary::create(fn);
        break;

    case token_emon52:
        token_next();
        ofp = srecord::output_file_emon52::create(fn);
        break;

    case token_fairchild:
        token_next();
        ofp = srecord::output_file_fairchild::create(fn);
        break;

    case token_fast_load:
        token_next();
        ofp = srecord::output_file_fastload::create(fn);
        break;

    case token_formatted_binary:
        token_next();
        ofp = srecord::output_file_formatted_binary::create(fn);
        break;

    case token_forth:
        token_next();
        ofp = srecord::output_file_forth::create(fn);
        break;

    case token_four_packed_code:
        token_next();
        ofp = srecord::output_file_four_packed_code::create(fn);
        break;

    case token_hexdump:
        token_next();
        ofp = srecord::output_file_hexdump::create(fn);
        break;

    case token_intel:
        token_next();
        ofp = srecord::output_file_intel::create(fn);
        break;

    case token_idt:
        token_next();
        ofp = srecord::output_file_idt::create(fn);
        break;

    case token_intel16:
        token_next();
        ofp = srecord::output_file_intel16::create(fn);
        break;

    case token_logisim:
        token_next();
        ofp = srecord::output_file_logisim::create(fn);
        break;

    case token_lattice_memory_initialization_format:
        token_next();
        ofp = srecord::output_file_mem::create(fn);
        break;

    case token_memory_initialization_file:
        token_next();
        ofp = srecord::output_file_mif::create(fn);
        break;

    case token_mips_flash_be:
        token_next();
        ofp = srecord::output_file_mips_flash::create_be(fn);
        break;

    case token_mips_flash_le:
        token_next();
        ofp = srecord::output_file_mips_flash::create_le(fn);
        break;

    case token_mos_tech:
        token_next();
        ofp = srecord::output_file_mos_tech::create(fn);
        break;

    case token_msbin:
        token_next();
        ofp = srecord::output_file_msbin::create(fn);
        break;

    case token_needham_hex:
        token_next();
        ofp = srecord::output_file_needham::create(fn);
        break;

    case token_ohio_scientific:
        token_next();
        ofp = srecord::output_file_os65v::create(fn);
        break;

    case token_ppb:
        token_next();
        ofp = srecord::output_file_ppb::create(fn);
        break;

    case token_ppx:
        token_next();
        ofp = srecord::output_file_ppx::create(fn);
        break;

    case token_signetics:
        token_next();
        ofp = srecord::output_file_signetics::create(fn);
        break;

    case token_spasm_be:
        token_next();
        ofp = srecord::output_file_spasm::create(fn, endian_big);
        break;

    case token_spasm_le:
        token_next();
        ofp = srecord::output_file_spasm::create(fn, endian_little);
        break;

    case token_spectrum:
        token_next();
        ofp = srecord::output_file_spectrum::create(fn);
        break;

    case token_stewie:
        token_next();
        ofp = srecord::output_file_stewie::create(fn);
        break;

    case token_tektronix:
        token_next();
        ofp = srecord::output_file_tektronix::create(fn);
        break;

    case token_tektronix_extended:
        token_next();
        ofp = srecord::output_file_tektronix_extended::create(fn);
        break;

    case token_ti_tagged:
        token_next();
        ofp = srecord::output_file_ti_tagged::create(fn);
        break;

    case token_ti_tagged_16:
        token_next();
        ofp = srecord::output_file_ti_tagged_16::create(fn);
        break;

    case token_ti_txt:
        token_next();
        ofp = srecord::output_file_ti_txt::create(fn);
        break;

    case token_trs80:
        token_next();
        ofp = srecord::output_file_trs80::create(fn);
        break;

    case token_vhdl:
        token_next();
        ofp = srecord::output_file_vhdl::create(fn);
        break;

    case token_vmem:
        token_next();
        ofp = srecord::output_file_vmem::create(fn);
        break;

    case token_wilson:
        token_next();
        ofp = srecord::output_file_wilson::create(fn);
        break;

    case token_xilinx_coefficient_file:
        token_next();
        ofp = srecord::output_file_coe::create(fn);
        break;
    }

    //
    // Parse extra command line options for this format, if any.
    //
    //assert(ofp);
    ofp->command_line(this);

    //
    // return the stream determined
    //
    return ofp;
}


// vim: set ts=8 sw=4 et :

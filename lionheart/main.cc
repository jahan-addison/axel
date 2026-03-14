/*****************************************************************************
 * Copyright (c) Jahan Addison
 *
 * This software is dual-licensed under the Apache License, Version 2.0 or
 * the GNU General Public License, Version 3.0 or later.
 *
 * You may use this work, in part or in whole, under the terms of either
 * license.
 *
 * See the LICENSE.Apache-v2 and LICENSE.GPL-v3 files in the project root
 * for the full text of these licenses.
 ****************************************************************************/

#include <iostream>              // for char_traits, cerr
#include <lionheart/args.h>      // for args
#include <lionheart/assembler.h> // for Assembler, Assembler_Error
#include <lionheart/grammar.h>   // for MC6800_GRAMMAR
#include <lionheart/parser.h>    // for Parser, Parser_Error
#include <ostream>               // for basic_ostream, operator<<
#include <string>                // for basic_string
#include <string_view>           // for basic_string_view

/******************************************************
 *                 /\
 *   Lionheart  ( ;`~v/~~~ ;._
 *              ,/'"/^) ' < o\  '".~'\\\--,
 *            ,/",/W  u '`. ~  >,._..,   )'
 *          ,/'  w  ,U^v  ;//^)/')/^\;~)'
 *        ,/"'/   W` ^v  W |;         )/'
 *      ;''  |  v' v`" W }  \\
 *    "    .'\    v  `v/^W,) '\)\.)\/)
 *              `\   ,/,)'   ''')/^"-;'
 *                  \
 *                ".
/******************************************************/

int main(int argc, char* argv[])
{
    try {
        auto config = lionheart::args::parse_args(argc, argv);

        if (!config.has_value()) {
            return 1;
        }

        auto mc6800_program =
            lionheart::read_file_from_path(config->input_file);
        auto parser = lionheart::Parser{ lionheart::grammar::MC6800_GRAMMAR };

        parser.parse(mc6800_program);

        auto assembler = lionheart::Assembler{ parser.symbol_table(),
            parser.instructions() };
        assembler.encode();
        assembler.emit(std::string{ config->output_file.value_or("o.bin") });

    } catch (lionheart::Parser_Error const& e) {
        std::cerr << "\033[31m" << e.what() << "\033[0m\n";
        return 1;
    } catch (lionheart::Assembler_Error const& e) {
        std::cerr << "\033[31m" << e.what() << "\033[0m\n";
        return 1;
    }
}
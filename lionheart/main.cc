#include <lionheart/grammar.h> // for Grammar, MC6800_GRAMMAR
#include <lionheart/parser.h>  // for Parser
#include <string_view>         // for basic_string_view

int main()
{
    auto test_program = R"(
        REDIS   = $FE3A
        START:
        JSR REDIS
	    LDA A #$01
	    BRA OUT
    )";
    auto parser = lionheart::Parser{ lionheart::grammar::MC6800_GRAMMAR };
    parser.parse(test_program);
}
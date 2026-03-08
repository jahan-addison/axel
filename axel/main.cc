#include <axel/grammar.h> // for Grammar, MC6800_GRAMMAR
#include <axel/parser.h>  // for Parser
#include <string_view>    // for basic_string_view

int main()
{
    auto test_program = R"(
        REDIS	= $FE3A
        START:
        JSR REDIS	; SET UP FIRST DISPLAY ADDRESS
	    LDA A #$01	; FIRST SEGMENT CODE
	    BRA OUT
    )";
    auto parser = axel::Parser{ axel::grammar::MC6800_GRAMMAR };
    parser.parse(test_program);
}
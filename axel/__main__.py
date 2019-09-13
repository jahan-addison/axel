from lexer import Lexer

with open('./examples/example-1.asm') as f:
    test = Lexer(f.read())
    for token in test:
        print(f'"{token}"')

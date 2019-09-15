from typing import List
import pytest
from axel.lexer import Lexer


@pytest.fixture
def lexer() -> Lexer:
    def _make_lexer(source):
        return Lexer(source)

    return _make_lexer

@pytest.fixture
def whitespace() -> List[str]:
    return ['''
        SAME	LDA B DIGADD+1	; FIX DISPLAY ADDRESS
            ADD B #$10
        ''',
        '      LDA B DIGADD+1',
        '''    ; This is a comment
        ADD B #$10
        ''',
        '''LDA B DIGADD+1 ; This is a comment \nADD B #$10''']


def test_skip_whitespace_comments(lexer, whitespace):
    test = lexer(whitespace[0])
    test._skip_whitespace_and_comments()
    assert test.pointer == 'S'
    test = lexer(whitespace[1])
    test._skip_whitespace_and_comments()
    assert test.pointer == 'L'
    test = lexer(whitespace[2])
    test._skip_whitespace_and_comments()
    assert test.pointer == 'A'


def test_read_term(lexer):
    test = lexer(' ADD  B   #$10  ')
    term1 = test._read_term()
    term2 = test._read_term()
    term3 = test._read_term()

    assert term1 == 'ADD'
    assert term2 == 'B'
    assert term3 == '#$10'


def test_skip_to_next_line(lexer, whitespace):
    test = lexer(whitespace[3])
    test._skip_to_next_line()
    assert test.pointer == 'A'


def test_peek_next(lexer):
    test = lexer('ABA  #$10')
    test._pointer = 3
    peek = test._peek_next()
    assert peek == '#$10'

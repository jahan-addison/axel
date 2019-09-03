from typing import List
from axel.lexer import Lexer
import pytest

@pytest.fixture
def lexer():
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
        ''']


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

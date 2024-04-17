#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <regex>
#include <vector>

using namespace std;

enum token_type {
    T_IGNORE = 0,
    T_EOL,
    T_LABEL,
    T_ALPHA,
    T_CMP,
    T_OR,
    T_JMP,
    T_JE,
    T_JNE,
    T_DRAW,
    T_ERASE,
    T_LEFT,
    T_RIGHT,
    T_HALT,
    T_FAIL,
    T_BLANK,
    T_NUMBER,
    T_PLUS,
    T_MINUS,
    T_CHARACTER,
    T_STRING,
    T_BAD
};

inline string tstr[] = {
    "T_IGNORE",
    "T_EOL",
    "T_LABEL",
    "T_ALPHA",
    "T_CMP",
    "T_OR",
    "T_JMP",
    "T_JE",
    "T_JNE",
    "T_DRAW",
    "T_ERASE",
    "T_LEFT",
    "T_RIGHT",
    "T_HALT",
    "T_FAIL",
    "T_BLANK",
    "T_NUMBER",
    "T_PLUS",
    "T_MINUS",
    "T_CHARACTER",
    "T_STRING",
    "T_BAD"
};

struct token {
    token_type id;
    string token_str;
    int line_no;
    int ram_addr;
    tm_word value;
};

typedef vector <token> token_line;
typedef vector <token_line> token_lines;

struct symbol {
	int line_no;
	string str;
	tm_word data;
};

#endif /* __TOKEN_H__ */

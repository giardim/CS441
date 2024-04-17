#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <filesystem>

#include <cstdlib>
#include <ctype.h>
#include <cstring>

#include "encodings.h"
#include "token.h"
#include "parser.h"
#include "listing.h"

/*
 * Uncomment the definition below to trace the lexer
 */
//#define DEBUG_TRACE

using namespace std;

typedef void (*token_handler_ptr)(token &);

struct token_regex {
	regex expr;
	token_type id;
	token_handler_ptr handler;
};


vector <tm_word> bin_words;


void read_file(const string path, vector <string> &asm_lines);
void initialize_lexical_regex(vector <token_regex> &patterns);
token get_token(const vector <token_regex>& patterns, const string& s, int line_no);
void tokenize_line(
	string asm_line,
	token_line &token_line,
	vector <token_regex> &patterns,
	int line_no
);
void tokenize_all_lines(
	token_lines &token_lines,
	vector <string> &asm_lines,
	vector <token_regex> &patterns
);
void debug_print_lexed_input(token_lines &token_lines, vector <string> &asm_lines);

tm_sword string_to_value(string s, tm_sword base);

/*
 * Token handler's to be called when certain tokens are identified.
 */
void handle_decimal_number(token &t);
void handle_prefixed_number(token &t);
void handle_character(token &t);

int main(int argc, char *argv[]) {
	if(argc != 2) {
		cerr << "Usage:   tnasm <file>" << endl;
		exit(EXIT_FAILURE);
	}

	filesystem::path asm_filename(argv[1]);

	if(asm_filename.extension() != ".asm") {
		cerr << "Expected a \".asm\" file..." << endl;
		cerr << "Usage:   tmasm <file>" << endl;
		exit(EXIT_FAILURE);
	}

	filesystem::path bin_filename = asm_filename.filename().replace_extension(".bin");

	vector <string> asm_lines;
	read_file(asm_filename.string(), asm_lines);

	vector <token_regex> patterns;
	initialize_lexical_regex(patterns);

	token_lines token_lines;
	tokenize_all_lines(token_lines, asm_lines, patterns);
	#ifdef DEBUG_TRACE
	cerr << "\n\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\n";
	debug_print_lexed_input(token_lines, asm_lines);
	#endif /* DEBUG_TRACE */

	if(parse(token_lines, asm_lines)) {
		ofstream bin_file(bin_filename, ios::binary);
		bin_file.write(reinterpret_cast<const char*>(bin_words.data()), bin_words.size() * sizeof(tm_word));
		generate_listing();
    }
    else {
        cerr << "There were errors.  No binary output." << endl;
    }


	return EXIT_SUCCESS;
}


void read_file(const string path, vector <string> &asm_lines) {
	ifstream f(path);
	string s;
	while(getline(f, s)) {
#if false /* I think we want labels to be case sensitive for this Turing assembler */
		/* lowercase all input to achieve case-insensitivity */
		for(auto &c : s) {
			if(isupper(c)) {
				c = c- 'A' + 'a';
			}
		}
#endif /* false */
		asm_lines.push_back(s);
	}
	f.close();

	return;
}


token_regex regex_token(string s, token_type id, token_handler_ptr handler) {
	token_regex tr;
	tr.expr = regex(s, regex_constants::icase);
	tr.id = id;
	tr.handler = handler;

	return tr;
}

void initialize_lexical_regex(vector <token_regex> &patterns) {
    patterns.push_back(regex_token("[ \\t\\b\\v\\f\\r]+", T_IGNORE, nullptr));
	patterns.push_back(regex_token(";.*", T_IGNORE, nullptr));
	patterns.push_back(regex_token("![^ \\t\\b\\v\\r\\n;]+", T_LABEL, nullptr));

	patterns.push_back(regex_token("alpha", T_ALPHA, nullptr));
	patterns.push_back(regex_token("cmp", T_CMP, nullptr));
	patterns.push_back(regex_token("or", T_OR, nullptr));
	patterns.push_back(regex_token("jmp", T_JMP, nullptr));
	patterns.push_back(regex_token("je", T_JE, nullptr));
	patterns.push_back(regex_token("jne", T_JNE, nullptr));
	patterns.push_back(regex_token("draw", T_DRAW, nullptr));
	patterns.push_back(regex_token("erase", T_ERASE, nullptr));
	patterns.push_back(regex_token("left", T_LEFT, nullptr));
	patterns.push_back(regex_token("right", T_RIGHT, nullptr));
	patterns.push_back(regex_token("halt", T_HALT, nullptr));
	patterns.push_back(regex_token("fail", T_FAIL, nullptr));

	patterns.push_back(regex_token("blank", T_BLANK, nullptr));
	patterns.push_back(regex_token("[0-9](_*[0-9])*", T_NUMBER, handle_decimal_number));
	patterns.push_back(regex_token("0x(_*[0-9a-f])+", T_NUMBER, handle_prefixed_number));
	patterns.push_back(regex_token("0b(_*[01])+", T_NUMBER, handle_prefixed_number));
	patterns.push_back(regex_token("\\+", T_PLUS, nullptr));
	patterns.push_back(regex_token("-", T_MINUS, nullptr));
	patterns.push_back(regex_token("'.'", T_CHARACTER, handle_character));
	patterns.push_back(regex_token("\\\"[^\\\"\\n\\r]+\\\"", T_STRING, nullptr));
	patterns.push_back(regex_token(".", T_BAD, nullptr));

	return;
}


token get_token(const vector <token_regex>& patterns, const string& s, int line_no) {
	token t;
	t.line_no = line_no;
	int longest = 0;

	#ifdef DEBUG_TRACE
	string tok = "NO MATCHING TOKEN"; // should NEVER see this
	#endif /* DEBUG_TRACE */

	token_handler_ptr handler = nullptr;

	for (const auto &pattern : patterns) {
		smatch match;
		if (regex_search(s, match, pattern.expr, regex_constants::match_continuous)) {
			int length = match.length();
			if (length > longest) {
				longest = length;
				t.id = pattern.id;
				t.token_str = s.substr(0, longest);
				handler = pattern.handler;

				#ifdef DEBUG_TRACE
				tok = tstr[pattern.id];
				#endif /* DEBUG_TRACE */
			}
		}
	}

	if(handler != nullptr) {
		handler(t);
	}

	#ifdef DEBUG_TRACE
	cerr << "Line " << line_no << ": " << tok << " matched \"";
	if(s.length() <= 10) {
		cerr << s;
	}
	else {
		cerr << s.substr(0, 10) << "...";
	}
	cerr << "\"" << " as \"" << t.token_str << "\"" << endl;
	#endif /* DEBUG_TRACE */

	return t;
}


void tokenize_line(
	string asm_line,
	token_line &token_line,
	vector <token_regex> &patterns,
	int line_no
) {
	string s(asm_line);
	while(s.length() > 0) {
        token t = get_token(patterns, s, line_no);
		if(t.id != T_IGNORE && t.id != T_BAD) {
			token_line.push_back(t);
        }
        else if(t.id == T_BAD) {
			cerr << " Line " << line_no << ": Unexpected character '";
			cerr << t.token_str << "'" << endl;
			exit(EXIT_FAILURE);
		}

		/* Advance the string just past the token */
		s = s.substr(t.token_str.length());
	}

	return;
}


void tokenize_all_lines(
	token_lines &token_lines,
	vector <string> &asm_lines,
	vector <token_regex> &patterns
) {
		for(unsigned int i = 0; i < asm_lines.size(); i++) {
		token_line token_line;
		tokenize_line(asm_lines[i], token_line, patterns, i + 1);
		if(token_line.size() > 0) {
			token t;
			t.id = T_EOL;
			t.line_no = i + 1;
			token_line.push_back(t);
			token_lines.push_back(token_line);
		}
	}

	return;
}


void debug_print_lexed_input(token_lines &token_lines, vector <string> &asm_lines) {
	int current_i = 1;
	for(unsigned int i = 0; i < token_lines.size(); i++) {
		token_line line = token_lines[i];
		/* catch up current line to wherever this token was */
		while(current_i < line[0].line_no) {
			cerr << current_i << ": " << asm_lines[current_i - 1] << endl;
			current_i++;
		}

		cerr << current_i << ":";
		for(auto t: line) {
			cerr << "  " << tstr[t.id];
		}
		cerr << endl;
		current_i++;
	}
	return;
}


tm_sword string_to_value(string s, tm_sword base) {
	const char table[] = "0123456789abcdef";
	tm_sword result = 0;

	for(auto c : s) {
		if(c != '_') {
			result *= base;
			result += (strchr(table, tolower(c)) - table) / sizeof(char);
		}
	}

	return result;
}


void handle_decimal_number(token &t) {
	/*
	 * "[0-9](_*[0-9])*"
	 */
	t.value.s = string_to_value(t.token_str, 10);

	return;
}


void handle_prefixed_number(token &t) {
	/*
	 * 0x(_*[0-9a-f])+
 	 * 0b(_*[01])+
	 */
	tm_sword base = 0;  // 0 is a bad base
	switch(t.token_str[1]) {
	case 'x':
	case 'X':
		base = 16;
		break;
	case 'b':
	case 'B':
		base = 2;
		break;
	default:
		break;
	}

	if(base != 0) {
		t.value.s = string_to_value(t.token_str, base);
	}
	else {
		t.value.s = 0;
	}

	return;
}

void handle_character(token &t) {
	/*
	 * "'.'"
	 */
	t.value.u = t.token_str[1];

	return;
}


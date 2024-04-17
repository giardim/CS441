#include <iostream>
#include <memory>
#include <map>
 
#include "encodings.h"
#include "token.h"
#include "parser.h"
#include "listing.h"

using namespace std;

token_line parse_line;  // The current line of tokens being parsed
unsigned int tnext;  // index of the next token in the line to consider
int pass;
tm_uword address;

map <string, tm_word> labels;
bool labels_updated_this_pass;

bool parse(token_lines &parse_lines, vector <string> asm_lines);

shared_ptr <token> term(token_type id);
bool p_loc();
shared_ptr <token> p_label_line();

bool p_code_alpha_line();
bool p_code_cmp_line();
bool p_code_jmp_line();
bool p_code_draw_line();
bool p_code_move_line();
bool p_code_stop_line();

shared_ptr <tm_word> p_immediate();
shared_ptr <tm_word> p_number();
shared_ptr <token> p_plus_or_minus();
shared_ptr <token> p_letter();

tm_uword token_to_opcode(int id);

shared_ptr <token> p_code_cmp_inst();
shared_ptr <token> p_code_jmp_inst();
shared_ptr <token> p_code_move_inst();


extern vector <tm_word> bin_words;

/**
 * @brief
 *   Parses the token, line by line, to generate the encoded version of the
 *   program.
 *
 * @return
 *   True if entire program is correct.  False otherwise.
 */
bool parse(token_lines &parse_lines, vector <string> asm_lines) {
    bool result = true;  // assume the assembly input is syntactically correct

    /*
     * In pass 1:
     *   - create the labels symbol table and determine the address of each
     *   - report errors for any line of code that doesn't match the p_loc()
     *     grammar
     */
    pass = 1;
    address = 0x0000;

    for(auto &line: parse_lines) {
        int line_no = line[0].line_no;
        parse_line = line;
        if(p_loc() == false) {
            result = false;
            cerr << "Error, line(" << line_no << "): " << asm_lines[line_no - 1] << endl;
        }
    }

    /* If anything went wrong, there's no point in going on to pass 2 */
    if(result == false) {
        return false;
    }

    /*
     * In passes 2+:
     *   - generate encoded instructions, and add them to bin_words
     *   - generate listing data
     */
    labels_updated_this_pass = true;

    while(labels_updated_this_pass && (result == true)) {
        labels_updated_this_pass = false;
        pass++;
        address = 0x0000;
        bin_words.clear();
        listing_blocks.clear();

        /*
         * TODO: the parser should auto-detect if it can use the teeny
         * form of some instructions
         */
        for(auto &line: parse_lines) {
            parse_line = line;
            listing_block lb;
            lb.line_no = line[0].line_no;
            lb.asm_line = asm_lines[lb.line_no - 1];
            lb.address = address;
            if(p_loc() == false) {
                result = false;
                /* TODO: assumption that failed p_X_line() reported error already */
            }
            int words_used = address - lb.address;
            if(words_used > 0) {
                lb.uses_words = true;
                auto word = bin_words.end() - words_used;
                while(word != bin_words.end()) {
                    lb.bin_uwords.push_back((*word).u);
                    word++;
                }
            }
            listing_blocks.push_back(lb);
        }
    }

    if(result) {
        cerr << "All labels resolved after " << pass << " passes." << endl << endl;
    }

    return result;
}

/**
 * @brief
 *   Verifies the next token in the line is a particular type and moves to the
 *   next
 *
 * @param id
 *   The type of the token to check for
 *
 * @return
 *   A smart pointer to the matching token or nullptr if the type doesn't match
 *
 * @note
 *   It is important to realize the global tnext token index will always be
 *   incremented, regardless of whether the token type matched.
 */
shared_ptr <token> term(token_type id) {
    shared_ptr <token> val = nullptr;
    if(tnext < parse_line.size() && parse_line[tnext].id == id) {
        val = shared_ptr <token>(new token(parse_line[tnext]));
    }
    tnext++;
    return val;
}

bool p_loc() {
    int save = tnext = 0;
    return (tnext = save, p_label_line()) ||
           (tnext = save, p_code_alpha_line()) ||
           (tnext = save, p_code_cmp_line()) ||
           (tnext = save, p_code_jmp_line()) ||
           (tnext = save, p_code_draw_line()) ||
           (tnext = save, p_code_move_line()) ||
           (tnext = save, p_code_stop_line());
}

/*
 * label_line ::= LABEL.
 */
shared_ptr <token> p_label_line() {
    shared_ptr <token> val = nullptr, label;
    if((label = term(T_LABEL)) && term(T_EOL)) {
        if(pass == 1) {
            bool label_exists = (labels.count(label->token_str) > 0);
            if(!label_exists) {
                /* New label found */
                labels[label->token_str] = tm_word{.u = address};
            }
            else {
                cerr << "ERROR, Line " << label->line_no << ": ";
                cerr << "Constant " << label->token_str << " already defined" << endl;
            }
        }
        else if(pass > 1) {
            if(address != labels[label->token_str].u) {
                cerr << label->token_str << " changed from ";
                cerr << hex << labels[label->token_str].u << dec << " to ";
                cerr << hex << address << " in pass " << dec << pass << endl;

                labels[label->token_str] = tm_word{.u = address};
                labels_updated_this_pass = true;
            }
        }

        val = label;
    }
    return val;
}

/*
 * code_alpha_line ::= ALPHA CHARACTER.
 * code_alpha_line ::= ALPHA STRING.
 */
bool p_code_alpha_line() {
    bool result = false;
    shared_ptr <token> oper, arg;
    int save = tnext;
    if(
        (tnext = save,
         (oper = term(T_ALPHA)) && (arg = term(T_CHARACTER)) && term(T_EOL))
        ||
        (tnext = save,
         (term(T_ALPHA)) && (arg = term(T_STRING)) && term(T_EOL))
    ) {

        tm_instruction inst;
        inst.line_num = oper->line_no;

        inst.encoding.alpha.opcode = token_to_opcode(oper->id);
        inst.encoding.alpha.rsvd = 0;
        
        if(arg->id == T_CHARACTER) {
            inst.encoding.alpha.letter = arg->value.u;
            if(pass > 1) {
                bin_words.push_back(inst.encoding.word);
            }
            address += 1;
        }
        else {
            /* We have a string... skip the quotes at the front and back */
            for(unsigned i = 1; i < arg->token_str.length() - 1; i++) {
                inst.encoding.alpha.letter = arg->token_str[i];
                if(pass > 1) {
                    bin_words.push_back(inst.encoding.word);
                }
                address += 1;
            }
        }

        result = true;
    }

    return result;
}

/*
 * code_cmp_line ::= code_cmp_inst letter.
 */
bool p_code_cmp_line() {
    bool result = false;
    shared_ptr <token> oper, arg;
    int save = tnext;
    if(
        (tnext = save,
         (oper = p_code_cmp_inst()) && (arg = p_letter()) && term(T_EOL))
    ) {

        tm_instruction inst;
        inst.line_num = oper->line_no;

        inst.encoding.cmp.opcode = token_to_opcode(oper->id);
        inst.encoding.cmp.rsvd = 0;
        inst.encoding.cmp.oring = (oper->id == T_OR);

        if(arg->id == T_CHARACTER) {
            inst.encoding.cmp.letter = arg->value.u;
            inst.encoding.cmp.blank = 0;
        }
        else {
            /* we have a BLANK */
            inst.encoding.cmp.letter = 0;
            inst.encoding.cmp.blank = 1;
        }

        if(pass > 1) {
            bin_words.push_back(inst.encoding.word);
        }
        address += 1;

        result = true;
    }

    return result;
}

/*
 * code_cmp_inst ::= CMP.
 * code_cmp_inst ::= OR.
 */
shared_ptr <token> p_code_cmp_inst() {
    shared_ptr <token> result;
    int save = tnext;

    (tnext = save, result = term(T_CMP)) ||
    (tnext = save, result = term(T_OR));

    return result;
}

/*
 * letter ::= BLANK.
 * letter ::= CHARACTER.
 */
shared_ptr <token> p_letter() {
    shared_ptr <token> result;
    int save = tnext;

    (tnext = save, result = term(T_BLANK)) ||
    (tnext = save, result = term(T_CHARACTER));

    return result;
}


/*
 * code_jmp_line ::= code_jmp_inst immediate.
 */
bool p_code_jmp_line() {
    bool result = false;
    shared_ptr <token> oper;
    shared_ptr <tm_word> arg;
    int save = tnext;
    if(
        (tnext = save,
         (oper = p_code_jmp_inst()) && (arg = p_immediate()) && term(T_EOL))
    ) {

        tm_instruction inst;
        inst.line_num = oper->line_no;

        inst.encoding.jmp.opcode = token_to_opcode(oper->id);
        switch(oper->id) {
        case T_JMP:
            inst.encoding.jmp.eq = 1;
            inst.encoding.jmp.ne = 1;
            break;
        case T_JE:
            inst.encoding.jmp.eq = 1;
            inst.encoding.jmp.ne = 0;
            break;
        case T_JNE:
            inst.encoding.jmp.eq = 0;
            inst.encoding.jmp.ne = 1;
            break;
        default:
            /* really this should never happen */
            inst.encoding.jmp.eq = 0;
            inst.encoding.jmp.ne = 0;
            break;
        }

        inst.encoding.jmp.addr = arg->u;

        if(pass > 1) {
            bin_words.push_back(inst.encoding.word);
        }
        address += 1;

        result = true;
    }

    return result;
}

/*
 * code_jmp_inst ::= JMP.
 * code_jmp_inst ::= JE.
 * code_jmp_inst ::= JNE.
 */
shared_ptr <token> p_code_jmp_inst() {
    shared_ptr <token> result;
    int save = tnext;

    (tnext = save, result = term(T_JMP)) ||
    (tnext = save, result = term(T_JE)) ||
    (tnext = save, result = term(T_JNE));

    return result;
}


/*
 * code_draw_line ::= DRAW letter.
 * code_draw_line ::= ERASE.
 */
bool p_code_draw_line() {
    bool result = false;
    shared_ptr <token> oper, arg;
    int save = tnext;
    if(
        (tnext = save,
         (oper = term(T_DRAW)) && (arg = p_letter()) && term(T_EOL))
        ||
        (tnext = save,
         (oper = term(T_ERASE)) && term(T_EOL))
    ) {

        tm_instruction inst;
        inst.line_num = oper->line_no;

        inst.encoding.draw.opcode = token_to_opcode(oper->id);
        inst.encoding.draw.rsvd = 0;

        if(oper->id == T_DRAW) {
            if(arg->id == T_CHARACTER) {
                inst.encoding.draw.blank = 0;
                inst.encoding.draw.letter = arg->value.u;
            }
            else {
                inst.encoding.draw.blank = 1;
                inst.encoding.draw.letter = 0;
            }
        }
        else {
            /* it's an ERASE instruction */
            inst.encoding.draw.blank = 1;
            inst.encoding.draw.letter = 0;
        }

        if(pass > 1) {
            bin_words.push_back(inst.encoding.word);
        }
        address += 1;

        result = true;
    }

    return result;
}


/*
 * code_move_line ::= code_move_inst.
 * code_move_line ::= code_move_inst number.
 */
bool p_code_move_line() {
    bool result = false;
    shared_ptr <token> oper;
    shared_ptr <tm_word> arg;
    int save = tnext;
    if(
        (tnext = save,
         (oper = p_code_move_inst()) && (arg = p_number()) && term(T_EOL))
        ||
        (tnext = save,
         (oper = p_code_move_inst()) && term(T_EOL))
    ) {

        tm_instruction inst;
        inst.line_num = oper->line_no;

        inst.encoding.move.opcode = token_to_opcode(oper->id);
        inst.encoding.move.amount = (arg ? arg->u : 1) * (oper->id == T_RIGHT ? 1 : -1);

        if(pass > 1) {
            bin_words.push_back(inst.encoding.word);
        }
        address += 1;

        result = true;
    }

    return result;
}

/*
 * code_move_inst ::= LEFT.
 * code_move_inst ::= RIGHT.
 */
shared_ptr <token> p_code_move_inst() {
    shared_ptr <token> result;
    int save = tnext;

    (tnext = save, result = term(T_LEFT)) ||
    (tnext = save, result = term(T_RIGHT));

    return result;
}


/*
 * code_stop_line ::= HALT.
 * code_stop_line ::= FAIL.
 */
bool p_code_stop_line() {
    bool result = false;
    shared_ptr <token> oper;
    int save = tnext;
    if(
        (tnext = save,
         (oper = term(T_HALT)) && term(T_EOL))
        ||
        (tnext = save,
         (oper = term(T_FAIL)) && term(T_EOL))
    ) {

        tm_instruction inst;
        inst.line_num = oper->line_no;

        inst.encoding.stop.opcode = token_to_opcode(oper->id);
        inst.encoding.stop.rsvd = 0;
        inst.encoding.stop.halt = (oper->id == T_HALT);

        if(pass > 1) {
            bin_words.push_back(inst.encoding.word);
        }
        address += 1;

        result = true;
    }

    return result;
}


/*
 * raw_value ::= number.
 * raw_value ::= CHARACTER.
 */
shared_ptr<tm_word> p_raw_value() {
    shared_ptr<tm_word> val = nullptr;
    shared_ptr<token> num, c;
    int save = tnext;
    if ((tnext = save, c = term(T_CHARACTER)))
    {
        val = shared_ptr<tm_word>(new tm_word(c->value));
    }
    else if ((tnext = save, val = p_number()))
    {
        /* nothing to do */
    }

    return val;
}

/*
 * immediate ::= raw_value.
 * immediate ::= LABEL.
 */
shared_ptr <tm_word> p_immediate() {
    shared_ptr <tm_word> val = nullptr;
    shared_ptr <token> label;
    int save = tnext;

    if((tnext = save, val = p_raw_value())) {
        /* nothing to do */
    }
    else if((tnext = save, label = term(T_LABEL))) {
        /* TODO: look up label's address */
        if(pass == 1) {
            /*
             * Label address is likely unknown and definitely unecessary in this
             * pass, so just recognize the immediate grammar was satisfied
             * with an arbitrary value.
             */
            val = shared_ptr <tm_word>(new tm_word(label->value));
        }
        else if(pass > 1) {
            if(labels.count(label->token_str) > 0) {
                val = shared_ptr <tm_word>(new tm_word(labels[label->token_str]));
            }
            else {
                cerr << "Error, Line(" << label->line_no << "): ";
                cerr << label->token_str << " is not defined" << endl;
            }
        }
    }

    return val;
}

/*
 * number ::= NUMBER.
 * number ::= plus_or_minus NUMBER.
 */
shared_ptr <tm_word> p_number() {
    shared_ptr <tm_word> val = nullptr;
    shared_ptr <token> ident, num, sign;
    int save = tnext;
    if((tnext = save, num = term(T_NUMBER))) {
        val = shared_ptr <tm_word>(new tm_word(num->value));
    }
    else if(tnext = save, (sign = p_plus_or_minus()) && (num = term(T_NUMBER))) {
        val = shared_ptr <tm_word>(new tm_word(num->value));
        if(sign->id == T_MINUS) {
            val->s *= -1;
        }
    }
    return val;
}

/*
 * plus_or_minus ::= PLUS.
 * plus_or_minus ::= MINUS.
 */
shared_ptr <token> p_plus_or_minus() {
    shared_ptr <token> val = nullptr;
    int save = tnext;

    (tnext = save, val = term(T_PLUS)) ||
    (tnext = save, val = term(T_MINUS));

    return val;
}


tm_uword token_to_opcode(int id) {
    tm_uword result;
    switch(id) {
    case T_ALPHA:   result = TM_OPCODE_ALPHA;   break;
    case T_CMP:     result = TM_OPCODE_CMP;     break;
    case T_OR:      result = TM_OPCODE_CMP;     break;
    case T_JMP:     result = TM_OPCODE_JMP;     break;
    case T_JE:      result = TM_OPCODE_JMP;     break;
    case T_JNE:     result = TM_OPCODE_JMP;     break;
    case T_DRAW:    result = TM_OPCODE_DRAW;    break;
    case T_ERASE:   result = TM_OPCODE_DRAW;    break;
    case T_LEFT:    result = TM_OPCODE_MOVE;    break;
    case T_RIGHT:   result = TM_OPCODE_MOVE;    break;
    case T_HALT:    result = TM_OPCODE_STOP;    break;
    case T_FAIL:    result = TM_OPCODE_STOP;    break;

    default:
        cerr << "Error, line(" << parse_line[0].line_no << "): ";
        cerr << "token_to_opcode() has unknown id, " << id << endl;
        /*
         * TODO: We should cleanly terminiate the assembler here
         */
        result = 1234;
    }

    return result;
}

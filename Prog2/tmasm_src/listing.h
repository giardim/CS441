#ifndef __LISTING_H__
#define __LISTING_H__

#include <iostream>
#include <vector>

#include "encodings.h"
#include "token.h"
#include "listing.h"

struct listing_block {
    string asm_line;
    bool uses_words = false;
    int line_no;
    tm_uword address;
    vector <tm_uword> bin_uwords;
};

extern vector <listing_block> listing_blocks;

void generate_listing();

#endif  /* __LISTING_H__ */

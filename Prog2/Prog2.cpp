/****************************
*Author: Michael Giardina
*Date 22 FEB 2024
***************************/
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <deque>
#include "encodings.h"

#define RAM_INSTRUCTIONS (1 << 11)
#define RAM_BYTES (RAM_INSTRUCTIONS * sizeof(tm_encoding))
#define BLANK 256

int main(int argc, char *argv[]) {
    std::string TapeString = "";
    int totalMoves = 0;
    int totalInstructions = 0;

	//make sure the user entered all required files
    if(argc < 3) {
        std::cerr << "Turning machine requires two arguments: \n1) A binary file\n2) A tape file\n" << std::endl;
        exit(EXIT_FAILURE);
    }
	
	//open the binary file
    std::ifstream f(argv[1], std::ios::binary);

    if (!f.is_open()){
        std::cerr << "Could not open the binary file\n";
        exit(EXIT_FAILURE);
    }
   	
	//read the binary file
	tm_encoding RAM[RAM_INSTRUCTIONS];
	memset(RAM, 0, RAM_BYTES);
	f.read((char *)RAM, RAM_BYTES);
	f.close();
	
	//open the tape file
    std::ifstream t(argv[2]);

    if (!t.is_open()){
        std::cerr << "Could not open the tape file\n";
    }
	
	//enter the while loop
    while (getline(t, TapeString)){
    //Initialize values
	int PC = 0;
    int alphabet[255] = {0};
    int isEqual = 0;
    int isHalt = 0;
    int head = 0;
    int blanks = 0;
    std::deque<uint16_t> tape;
	std::string status = "";
    int moves = 0;
    int instructions = 0;
	
	//clear the tape
    tape.clear();
	
	//initialize the tape
    for (int i = 0; i < TapeString.size(); ++i){
        tape.push_back(TapeString[i]);
    }

	//Go through the instructions until you hit the halt instruction
    while(!isHalt) {
		//fetch
        tm_encoding IR = RAM[PC];
        
		//decode
		switch(IR.generic.opcode) {
        
		//execute

		//add the letter in the instruction to the alphabet
		case TM_OPCODE_ALPHA:
            alphabet[IR.alpha.letter] = 1;
            break;

		//compare the value at the head with the letter in the instruction or blank
        case TM_OPCODE_CMP:
            if (IR.cmp.oring){
                if (IR.cmp.blank){
                    isEqual |= tape[head] == BLANK ? 1 : 0;
              }
                else {
                    isEqual |= tape[head] == IR.cmp.letter ? 1 : 0;
                }  
            }
            else {
                if (IR.cmp.blank){
                    isEqual = tape[head] == BLANK ? 1 : 0;
                }
                else {
                    isEqual = tape[head] == IR.cmp.letter ? 1 : 0;
                }
            }
            break;

		//jump to the address in the instruction	
        case TM_OPCODE_JMP:
            if(IR.jmp.eq && IR.jmp.ne) {
                PC = IR.jmp.addr;
                --PC;
            }
            else if(IR.jmp.eq && isEqual) {
                PC = IR.jmp.addr;
                --PC;
            }
            else if(IR.jmp.ne && !isEqual) {
                PC = IR.jmp.addr;
                --PC;
            }
            else {
               //std::cout << "nop  ; This should never happen" << std::endl;
                break;
            }
            break;
		
		//draw the value in the instruction to the head
        case TM_OPCODE_DRAW:
            if (head < 0){
                for (int i = 0; i < std::abs(head); ++i){
                    tape.push_front(BLANK);
                }
                head = 0;
            } else if (head >= tape.size()){
                for (int i = 0; i < (head - tape.size()) + 1; ++i){
                    tape.push_back(BLANK);
                }
            }

            if(IR.draw.blank) {
                tape[head] = BLANK;
            }
            else {
				if (alphabet[IR.draw.letter]){
					tape[head] = IR.draw.letter;
				}
				else {
					std::cerr << "***LETTER " << (char) IR.draw.letter << " IS NOT IN THE ALPHABET***\n";
				}
            }

            break;
        
		//move the head the value in the instruction
		case TM_OPCODE_MOVE: 
            moves += std::abs(IR.move.amount);
            head += IR.move.amount;
            if (head < 0){
                for (int i = 0; i < std::abs(head); ++i){
                    tape.push_front(BLANK);
                }
                head = 0;
            } else if (head >= tape.size()){
                for (int i = 0; i < (head - tape.size()) + 1; ++i){
                    tape.push_back(BLANK);
                }
            }
            break;

		//the instruction set ended
        case TM_OPCODE_STOP:
            isHalt = 1;
			if (IR.stop.halt){
				status = "Halted";
			}
			else {
				status = "Failed";
			}
            break;

        default:
            std::cout << "unknown opcode " << IR.generic.opcode;
            std::cout << " at PC " << PC << std::endl;
        }

        ++instructions;
        PC++;
    }
	
	//print out the number of moves and instructions
    std::cout << status << " after " << (int)moves << " moves and " << (int)instructions << " instructions executed\n";
    totalInstructions += instructions;
    totalMoves += moves;

	//print out the new tape
    for (int i = 0; i < tape.size(); ++i){
		if (tape[i] == BLANK){
			std::cout << "";
            ++blanks;
		}
		else {
			std::cout << (char)tape[i];
		}
	}
	std::cout << std::endl;
	for (int i = 0; i < head - blanks; ++i){
		std::cout << " ";
	}

	//print out the head
	std::cout << "^\n\n";
}
	
	//print out the totals
	std::cout << "Totals across all tapes..." << std::endl;
	std::cout << totalMoves << " moves" << std::endl;
	std::cout << totalInstructions << " instructions" << std::endl;
    t.close();
	exit(EXIT_SUCCESS);
}




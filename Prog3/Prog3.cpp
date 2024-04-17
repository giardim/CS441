/***************************************
*Author: Michael Giardina
*Date: 5 Apr 2024
*Language: C++ (g++ 12.2.0)
*Purpose: A program to simulate the branch prediction of a 
	CPU
***************************************/
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <unordered_map>
#include <cstdint>

void fixedFalse(std::string history){
	uint64_t correctPred = 0;
	uint64_t branchCnt = history.size();
	//Go through the branch history, if the branch is not taken, increment the correct prediction
	for (auto i : history){
		if (i == '.'){
			++correctPred;
		}
	}
	std::cout << "Fixed-F: " << correctPred << " / " << branchCnt << std::endl;
	return;
}

void staticFalse(std::string history){
	uint64_t correctPred = 0;
	uint64_t branchCnt = history.size();
	char currentPred = '.';
	
	//If the first branch in the history is not taken, increment the correct prediction
	if (history[0] == currentPred){
		++correctPred;
	}

	//Set the new prediction to the first branch in the history
	currentPred = history[0];

	//List through the rest of the history, if the history matches the prediction, increment the correct prediction
	for (int i = 1; i < branchCnt; ++i){
		if(history[i] == currentPred){
			++correctPred;
		}
	}
	
	std::cout << "Staic-F: " << correctPred << " / " << branchCnt << std::endl;
	return;
}

void bimodal(std::string history){
	int PE = 0b01;
	uint64_t correctPred = 0;	
	uint64_t branchCnt = history.size();

	//Create a map so we don't have to convert the branch to a number
	//Or the PE register to a character
	std::unordered_map <char, int> predMap = {{'.', 0}, {'@', 1}};

	for (int i = 0; i < branchCnt; ++i){
		//If the branch history matches the prediction bit in the PE register, set the counter bit to zero
		//and increment the correct prediction counter
		if (predMap[history[i]] == ((PE & (1 << 1)) >> 1)){
			PE &= (~0U << 1);
			++correctPred;
		}
		else{
			//If it is not correct, increment the PE register
			++PE;
		}
	}
	std::cout << "Bimodal: " << correctPred << " / " << branchCnt << std::endl;  
	return;
}

void twoLayer(std::string history){
	int PE[4] = {0b01, 0b11, 0b01, 0b11};
	int prevBits = 0b00;
	uint64_t correctPred = 0;	
	uint64_t branchCnt = history.size();
	
	//Create a map so we don't have to convert the branch to a number
	//Or the PE register to a character
	std::unordered_map <char, int> predMap = {{'.', 0}, {'@', 1}};

	for (int i = 0; i < branchCnt; ++i){
		//If the branch history matches the prediction bit in the PE register, set the counter bit to zero
		//and increment the correct prediction counter
		if (predMap[history[i]] == ((PE[prevBits] & (1 << 1)) >> 1)){
			PE[prevBits] &= (~0U << 1);
			++correctPred;
		}
		else{
			//If it is not correct, increment the PE register
			++PE[prevBits];
		}
		prevBits <<= 1;
		prevBits |= predMap[history[i]];
		prevBits &= ~(~0U << 2);
	}
	std::cout << "Bimodal: " << correctPred << " / " << branchCnt << std::endl; 
	return;
}

int main(int argc, char* argv []){
	std::ifstream traceFile;
	std::string history;

	//Make sure the user enters the correct number of arguments
	if (argc < 2){
		std::cout << "***Missing Argument - Must include file of branch history***\n";
		exit(EXIT_FAILURE);
	}
	
	traceFile.open(argv[1]);

	//Make sure the file opens
	if (!traceFile.is_open()){
		std::cout << "**Could not open the trace file***\n";
		exit(EXIT_FAILURE);
	}
	
	//Each line represents a new branch history, while there are still lines, cacluate the accuracy of each prediction algorithm
	while (getline(traceFile, history)){
		fixedFalse(history);
		staticFalse(history);
		bimodal(history);
		twoLayer(history);
		std::cout << std::endl;
	}

	exit(EXIT_SUCCESS);	
}

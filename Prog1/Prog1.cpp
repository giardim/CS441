/*******************************************
Author: Michael Giardina
Purpose: Given an input file which gives the rules of a virtual machine 
    and strings to test against the rules, this program will return if the string is
    within the language of the machine
Language: C++ (g++ 12.2.2 compiler) 
*******************************************/

//Includes
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_map>

int main (int argc, char *argv[]){
    //Variables
    int stateCnt = 0;
    int initState = 0;
    int currState = 0;
    std::string alphabet = "";
    std::vector<bool> finalStates;
    std::vector<std::vector<int>> trans;
    std::vector<std::string> testStrings;
    std::ifstream machineRules;
    std::string inputString;
    std::stringstream converter;
    std::unordered_map<char, int> cols;

    //Ensure the user gives 2 arguments 
    if (argc < 2){
        std::cout << "Prog1 must be used with a file argument\n";
        exit(EXIT_FAILURE);
    }
    
    //Open the file
    machineRules.open(argv[1]);
    
    //Check if the file opened properly
    if (!machineRules.is_open()){
        std::cout << "Could not open file: " << argv[1] << std::endl;
        exit(EXIT_FAILURE);
    }
    
    //get number of state
    getline(machineRules, inputString); 
    converter << inputString;
    converter >> stateCnt;
    converter.clear();

    //get initial state
    getline(machineRules, inputString);
    converter << inputString;
    converter >> initState;
    converter.clear();
    currState = initState;
   
    //resize *SHOULD* initialize all values as false
    finalStates.resize(stateCnt + 1);

    //set the final states of the machine to true
    getline(machineRules, inputString);
    inputString.erase(remove_if(inputString.begin(), inputString.end(), isspace), inputString.end()); 
    for (int i = 0; i < inputString.size(); ++i){
        int tempState = 0;
        converter << inputString[i];
        converter >> tempState;
        converter.clear();
        finalStates[tempState] = true;
    }
    finalStates[0] = false;
    
    //get the alphabet
    getline (machineRules, alphabet); 
    alphabet.erase(remove_if(alphabet.begin(), alphabet.end(), isspace), alphabet.end()); 
    for (int i = 0; i < alphabet.size(); ++i){
       cols[alphabet[i]] = i; 
    }

    //get the transitions
    trans.resize(stateCnt + 1, std::vector<int>(alphabet.size()));
    for (int i = 1; i < trans.size(); ++i){
         getline(machineRules, inputString);
         inputString.erase(remove_if(inputString.begin(), inputString.end(), isspace), inputString.end());
        for (int j = 0; j < inputString.size(); ++j){
            int temp = 0;
            converter << inputString[j];
            converter >> temp;
            trans[i][j] = temp;
            converter.clear();
        }
    }
   
   //check if the strings are in the language
   while (getline(machineRules, inputString)){
    currState = initState;
    inputString.erase(remove_if(inputString.begin(), inputString.end(), isspace), inputString.end());
    for (int i = 0; i < inputString.size(); ++i){
        currState = trans[currState][cols[inputString[i]]];
        if (currState == 0){           
            break;
        }
        
    }

    if (inputString.size() == 0){
        inputString = "<empty>";
    }
    if (finalStates[currState]){
        std::cout << "good " << inputString;
    }
    else{
        std::cout << "bad  " << inputString;
    }
        std::cout << std::endl;
   }
   
   //close the file to prevent memory corruption
   machineRules.close();

   exit(EXIT_SUCCESS);
}

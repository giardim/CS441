/***************************************
*Author: Michael Giardina
*Date: 18 Apr 2024
*Language: C++ (g++ 12.2.0)
*Purpose: A program to simulate the set associative 
	cache of a cpu
***************************************/

#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdbool>


class Block{
private:
	bool isDirty;
	uint64_t tag;
	uint32_t lru;

public:
	Block(){
		this->isDirty = false;
		this->tag = 0;
		this->lru = 0;
	}

	bool getIsDirty(){
		return isDirty;
	}

	uint64_t getTag(){
		return tag;
	}

	uint32_t getLru(){
		return lru;
	}

	void setIsDirty(bool isDirty){
		this->isDirty = isDirty;
	}

	void setTag(uint64_t tag){
		this->tag = tag;
	}

	void setLru(uint32_t lru){
		this->lru = lru;
	}

	void incLru(){
		this->lru++;
	}
};

class Set{
private:
	std::vector<Block> blocks;	
	int bps;
public:
	Set (){
		//do nothing
	}

	void resizeSet(int bps){
		this -> bps = bps;
		blocks.resize(bps);	
	}

	bool checkSet(uint64_t tag){
		uint32_t maxIndex = 0;
		//Increment all LRU
		for (int i = 0; i < this->bps; ++i){
			blocks[i].incLru();
		}
		
		
		//Check if there are any non dirty bits
		for (int i = 0; i < this->bps; ++i){
			//Check if the tag is in the cache
			if (blocks[i].getTag() == tag){
				blocks[i].setLru(0);
				return true;
			}
			
			//If the block is not dirty, set the tag of that block
			if (!blocks[i].getIsDirty()){
				blocks[i].setIsDirty(true);
				blocks[i].setTag(tag);
				blocks[i].setLru(0);
				return false;
			}


			//If there are no dirty bits, find the largest LRU
			if (blocks[i].getLru() > blocks[maxIndex].getLru()){
				maxIndex = i;
			}
		}
		
		//replace the highest lru
		blocks[maxIndex].setLru(0);
		blocks[maxIndex].setTag(tag);
		return false;
	}
};


int main(int argc, char* argv[]){
	std::ifstream mem;
	std::stringstream ss;
	int temp = 0;
	int SETS = 0;
	int BPS = 0;
	int WPB = 0;
	int set = 0;
	int hits = 0;
	int shifts[3] = {0};
	int cntAddresses = 0;
	uint64_t tag = 0;
	std::string tempAddress = "";
	uint64_t address = 0;
	
	if (argc < 5){
		std::cout << "***Not enough arguments***\n\nYou need\n1) The executable\n2) The memory file\n3) The number of sets\n";
		std::cout << "4) The number of blocks per set\n5) The number of words per block\n\n";
		exit(EXIT_FAILURE);
	}
	
	mem.open(argv[1]);

	if (!mem.is_open()){
		std::cout << "***Could not open file***" << std::endl;
		exit(EXIT_FAILURE);
	}

	ss << argv[2];
	ss >> temp;
	
	if (temp < 0){
		std::cout << "***argument 3 cannot be negative***" << std::endl;
		exit(EXIT_FAILURE);

	}

	//number of sets exponent
	shifts[0] = temp;
	SETS =  1 << temp;

	ss.clear();
	ss << argv[3];
	ss >> temp;

	if (temp < 0){
		std::cout << "***argument 4 cannot be negative***" << std::endl;
		exit(EXIT_FAILURE);
	}

	//blocks per set exponent
	shifts[1] = temp;
	BPS = 1 << temp;

	ss.clear();
	ss << argv[4];
	ss >> temp;

	if (temp < 0){
		std::cout << "***argument 5  cannot be negative***" << std::endl;
		exit(EXIT_FAILURE);
	}		
	
	//words per block exponent
	shifts[2] = temp;
	WPB = 1 << temp;

	std::vector<Set> cache(SETS);

	for (int i = 0; i < SETS; ++i){
		cache[i].resizeSet(BPS);
	}	

	while (getline(mem, tempAddress)){
		ss.clear();
		tempAddress = tempAddress.substr(2, tempAddress.size()-1);
		address = strtoll(tempAddress.c_str(), NULL, 16);
		address >>= (shifts[2]);	
		set = address & ~(~0U  << shifts[0]); 
		tag = address >> (shifts[0]);
		if (cache[set].checkSet(tag)){
			++hits;	
		}
		++cntAddresses;
	}

	std::cout << "hits: " << hits << "/" << cntAddresses << std::endl;

	exit(EXIT_SUCCESS);
}

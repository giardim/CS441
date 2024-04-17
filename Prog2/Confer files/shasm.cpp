#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdint>
#include <sstream>

using namespace std;

string trim(const string& str);

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <input_file>" << endl;
    return 1;
  }

  ifstream input(argv[1]);
  ofstream output(string(argv[1]) + ".bin", ios::binary);

  if (!input || !output) {
    cerr << "Error opening files." << endl;
    return 1;
  }

  string line;
  while (getline(input, line)) {
    /*
     * Assume lines are blank, are labels (start w/ '!'), are comments (start w/ ';'),
     * or are hex-code (no leading "0x") with everything after on the line ignored.
     */

    // Read the hex string to an integer
    u_int16_t hex_value;
    stringstream ss(line);
    
    line = trim(line);
    if(line.empty() || line[0] == '!' || line[0] == ';') {
      continue;
    }

    // assuming it's a hex-code line  
    ss >> hex >> hex_value;

    // Write binary string to output file
    output.write((char *)&hex_value, sizeof(uint16_t));
  }

  input.close();
  output.close();

  return 0;
}

std::string trim(const std::string& str) {
  // Find the first non-whitespace character from the beginning.
  size_t start = str.find_first_not_of(" \t\n\r\v\f\b");
  // If no non-whitespace characters were found, return an empty string.
  if (start == std::string::npos) {
    return "";
  }
  // Find the last non-whitespace character from the end.
  size_t end = str.find_last_not_of(" \t\n\r\v\f\b");
  // Return a substring containing only the non-whitespace characters.
  return str.substr(start, end - start + 1);
}
all: tmasm turing

tmasm:
	g++ -o tmasm parser.cpp lexer.cpp listing.cpp --std=c++20

turing:
	g++ -o turing turing_mess.cpp 

clean:
	rm tmasm turing

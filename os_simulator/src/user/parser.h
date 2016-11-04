#pragma once
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
#include <cstdlib>

/* Structure for storing one command */
struct Command {
	std::string nazev;
	std::vector<char> params;
	std::string redirectStdin;
	std::string redirectStdout;
	std::string redirectAStdout;
	std::queue<std::string> data;
};

class parser {
public:	
	void saveData(); /* Saves data into command struct */					 
	bool parse(std::string input); /* Parsing string input */

	std::queue<Command> commandList; /* Vector of commands */

private:
	/* Special symbols */
	static const char PIPE = '|'; /* pipe */
	static const char STDIN = '<'; /* stdin redirect */
	static const char STDOUT = '>'; /* stdout redirect */
	static const char PARAMPREFIX = '/'; /* param prefix */
	static const char QUOTE = '"'; /* start of string */
	static const char SPACE = ' '; /* space */

	/* Temp bools */
	bool commandOK = false; /* Command is allowed */
	bool getParam = false; /* Next char is param */
	bool out = false; /* Temp Astdout or stdout */
	bool getStdin = false; /* Next is stdin */
	bool getAStdout = false; /* Next is append stdout */
	bool getStdout = false; /* Next is stdout */
	bool openQuote = false; /* Is processing line in quote */
	std::string temp = ""; /* Actual processing string */
};
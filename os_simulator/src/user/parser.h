#pragma once
#include <algorithm>
#include <string>
#include <queue>
#include <unordered_set>
#include <regex>
#include "vmstdio.h"

/// <summary>
/// Structure for storing one command
/// </summary>
struct Command {
	std::string name;	
	std::vector<std::string> params;
	std::string redirectStdin;
	std::string redirectStdout;
	std::string redirectAStdout;
};

/// <summary>
/// Class for parsing data from command line
/// </summary>
class parser {
	public:
	/// <summary>
	/// Saves data into command struct.
	/// </summary>
	bool saveData();

	/// <summary>
	/// Checks for spaces and consequently saves data.
	/// </summary>
	bool checkSpace();

	/// <summary>
	/// Parses command string.
	/// </summary>
	/// <param name="input">Input from stdin</param>
	/// <returns>True if parsing ok, false if not</returns>
	bool parse(std::string input);

	std::queue<Command> commandList; // Vector of commands

	private:
	// Special symbols
	static const char PIPE = '|'; // pipe
	static const char STDIN = '<'; // stdin redirect
	static const char STDOUT = '>'; // stdout redirect
	static const char QUOTE = '"'; // start of string
	static const char SPACE = ' '; // space

	// Temp bools
	bool commandOK = false; // Command is allowed
	bool pipe = false; // If pipe hasn't followed command
	bool out = false; // Temp Astdout or stdout
	bool getStdin = false; // Next is stdin
	bool getAStdout = false; // Next is append stdout
	bool getStdout = false; // Next is stdout
	bool openQuote = false; // Is processing line in quote
	std::string temp = ""; // Actual processing string
	size_t i; // Actual position in input
	size_t last; // Position of last char in input
};

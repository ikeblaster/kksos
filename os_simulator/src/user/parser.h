#pragma once
#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <unordered_set>
#include <regex>

/// <summary>
/// Structure for storing one command
/// </summary>
struct Command {
	std::string name;
	std::vector<char> params;
	std::string redirectStdin;
	std::string redirectStdout;
	std::string redirectAStdout;
	std::vector<std::string> data;
};

/// <summary>
/// Class for parsing data from command line
/// </summary>
class parser {
public:
	/// <summary>
	/// Saves data into command struct
	/// </summary>
	bool saveData();

	/// <summary>
	/// Saves data after space
	/// </summary>
	bool checkSpace();

	/// <summary>
	/// Parsing string input
	/// </summary>
	/// <param name="input">Input from stdin</param>
	/// <returns>True if parsing ok, false if not</returns>
	bool parse(std::string input);

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
	bool pipe = false; /* If pipe hasn't followed command */
	bool getParam = false; /* Next char is param */
	bool out = false; /* Temp Astdout or stdout */
	bool getStdin = false; /* Next is stdin */
	bool getAStdout = false; /* Next is append stdout */
	bool getStdout = false; /* Next is stdout */
	bool openQuote = false; /* Is processing line in quote */
	std::string temp = ""; /* Actual processing string */
	int i; /* Actual position in input */
	size_t last; /* Position of last char in input */
};
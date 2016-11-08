#pragma once
#include <iostream>
#include <string>
#include <queue>
#include <unordered_set>

/// <summary>
/// Structure for storing one command
/// </summary>
struct Command {
	std::string nazev;
	std::vector<char> params;
	std::string redirectStdin;
	std::string redirectStdout;
	std::string redirectAStdout;
	std::queue<std::string> data;
};

/// <summary>
/// Class for parsing data from command line
/// </summary>
class parser {
	public:	
	/// <summary>
	/// Saves data into command struct
	/// </summary>
	void saveData();

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
	bool getParam = false; /* Next char is param */
	bool out = false; /* Temp Astdout or stdout */
	bool getStdin = false; /* Next is stdin */
	bool getAStdout = false; /* Next is append stdout */
	bool getStdout = false; /* Next is stdout */
	bool openQuote = false; /* Is processing line in quote */
	std::string temp = ""; /* Actual processing string */
};
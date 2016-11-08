#include "Parser.h"

static std::unordered_set<std::string> Commands{ "echo", "cd", "dir", "md", "rd", "type", "wc", "sort", "ps", "rgen", "freq" }; /* Allowed commands */

void parser::saveData() 
{
	if (temp != "" && temp != " ") {
		if (getStdin) {
			commandList.back().redirectStdin = temp; /* add stdin for command to struct */
			getStdin = false;
			temp = "";
		}
		else if (getStdout) {
			commandList.back().redirectStdout = temp; /* add stdout for command to struct */
			getStdout = false;
			temp = "";
		}
		else if (getAStdout) {
			commandList.back().redirectAStdout = temp; /* add append stdout for command to struct */
			getAStdout = false;
			temp = "";
		}
		else {
			if (temp != "") {
				commandList.back().data.push(temp); /* add data for command to struct */
				temp = "";
			}
		}
	}
	else {
		temp = "";
	}
}

bool parser::parse(std::string input)
{
	/* processes input string char by char */
	for (char& c : input) {
		/* Firstly must be placed allowed command */
		if (!commandOK) {
			if (c != ' ') {
				temp += c;
			}
			/* If command is allowed */
			if (Commands.count(temp)) {
				commandOK = true;
				commandList.push(Command()); /* add to vector new command */
				commandList.back().nazev = temp; /* add command name to struct */
				temp = "";
			}
			else {
				if (temp.length() > 4) {
					std::cout << temp << " is not allowed command!" << std::endl;
					return false;
				}
			}
		}
		/* Processing data for current command */
		else {
			/* If line in quotes is open */
			if (openQuote) {
				if (c != '"') {
					temp += c;
				}
				else {
					openQuote = false;
					saveData(); /* Save data for command */
				}
			}
			else if (out) {
				if (c != '>') {
					getStdout = true;
					temp += c;
				}
				else {
					getAStdout = true;
				}
				out = false;
			}
			/* Processing current character */
			else {
				switch (c) {
					/* Saves data and prepares values for new command */
				case PIPE:
					saveData(); /* Save data for command */
					/* vaules for new command */
					commandOK = false;
					temp = "";
					break;
					/* Start of param */
				case PARAMPREFIX:
					saveData(); /* Save data for command */
					getParam = true;
					break;
					/* Start of stdin */
				case STDIN:
					saveData(); /* Save data for command */
					getStdin = true;
					break;
					/* Start of stdout */
				case STDOUT:
					saveData(); /* Save data for command */
					out = true;
					break;
					/* Start of line in quotes */
				case QUOTE:
					openQuote = true;
					temp = "";
					break;
					/* For space */
				case SPACE:
					saveData(); /* Save data for command */
					break;
				default:
					/* Save parameter */
					if (getParam) {
						if (isalpha(c) || c == '?') {
							commandList.back().params.push_back(c); /* add param for command to struct */
							getParam = false;
						}
						else {
							std::cout << "Parammeter: " << c << " is not supported" << std::endl;
							return false;
						}
					}
					else {
						temp += c;
					}
					break;
				}
			}
		}
	}

	saveData(); /* Save data for command */

	/* Prints all commands from line - pak zmenime na volani trid pro obsluhu jednotlivych prikazu */
	while (!(commandList.empty())) {
		std::cout << std::endl;
		std::cout << "nazev: " << commandList.front().nazev << std::endl;
		while (!(commandList.front().params.empty())) { std::cout << "params: " << commandList.front().params.back(); commandList.front().params.pop_back(); std::cout << std::endl; }
		while (!(commandList.front().data.empty())) { std::cout << "data: " << commandList.front().data.front() << std::endl; commandList.front().data.pop(); }
		std::cout << "stdin: " << commandList.front().redirectStdin << std::endl;
		std::cout << "stdout: " << commandList.front().redirectStdout << std::endl;
		std::cout << "Astdout: " << commandList.front().redirectAStdout << std::endl;

		commandList.pop();
	}

	return true;
}
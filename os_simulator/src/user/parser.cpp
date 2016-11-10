#include "Parser.h"

static std::unordered_set<std::string> Commands{ "echo", "cd", "dir", "md", "rd", "type", "wc", "sort", "ps", "rgen", "freq", "shell" }; /* Allowed commands */

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
				commandList.back().data.push_back(temp); /* add data for command to struct */
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
				pipe = false;
				commandList.push(Command()); /* add to vector new command */
				commandList.back().nazev = temp; /* add command name to struct */
				temp = "";
			}
			else {
				if (temp.length() > 4) {
					std::cout << "This command is not allowed!" << std::endl;
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
					pipe = true;
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
	/* Whole input was processed */
	
	/* Check if pipe has follower */
	if (!commandOK && pipe) {
		std::cout << "After pipe must follow a valid command!" << std::endl;
		return false;
	}
	/* Save data for command */
	else if (!commandList.empty()) {
		saveData(); /* Save data for command */
	}	
	/* Input isn't valid command */
	else {
		if(temp != "") {
			std::cout << "This command is not allowed!" << std::endl;
		}
		return false;
	}

	return true;
}
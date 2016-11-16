#include "parser.h"

std::regex validCommand("^[A-z]+$"); /* regex for commands */
std::regex validFilename("^[a-zA-Z0-9\.\\\, ]+$"); /* regex for filename */
static std::unordered_set<char> specialSymbols{ ' ', '<', '>', '|', '/', '.' }; /* Symbols with special meaning */
char* errMsg = nullptr;

bool parser::checkSpace()
{
	if (temp != "" && temp != " ") {
		if (!saveData()) /* Save data for command */
			return false;
	}
	else {
		temp = "";
	}
	return true;
}

bool parser::saveData()
{
	if (getStdin) {		
		if (!std::regex_match(temp, validFilename)) {
			std::cout << "Stdin is not valid" << std::endl;
			return false;
		}

		commandList.back().redirectStdin = temp; /* add stdin for command to struct */
		getStdin = false;
		temp = "";
	}
	else if (getStdout) {
		if (!std::regex_match(temp, validFilename)) {
			std::cout << "Stdout is not valid" << std::endl;
			return false;
		}

		commandList.back().redirectStdout = temp; /* add stdout for command to struct */
		getStdout = false;
		temp = "";
	}
	else if (getAStdout) {
		if (!std::regex_match(temp, validFilename)) {
			std::cout << "Stdout is not valid" << std::endl;
			return false;
		}

		commandList.back().redirectAStdout = temp; /* add append stdout for command to struct */
		getAStdout = false;
		temp = "";
	}
	else {
		if (temp != "" && temp != " ") {
			commandList.back().data.push_back(temp); /* add data for command to struct */
			temp = "";
		}
	}

	return true;
}

bool parser::parse(std::string input)
{
	last = input.length() - 1;

	/* processes input string char by char */
	for (i = 0; i < input.length(); i++) {
		char& c = input.at(i); //current char

		/* Firstly must be placed allowed command */
		if (!commandOK) {
			if (i == last || specialSymbols.count(c)) {
				if (i == last) {
					if(c != ' ')
						temp += c;
				}
				/* Checks if command is valid */
				if (std::regex_match(temp, validCommand)) {
					commandOK = true;
					pipe = false;
					commandList.push(Command()); /* add to vector new command */
					commandList.back().name = temp; /* add command name to struct */
					temp = "";
					if(c != ' ' && i != last)
						i -= 1; //if actual char is special symbol, we need use it in next step again
				}
				else {
					if (!temp.empty()) {
						std::cout << "Command " << temp << " is not allowed!" << std::endl;
						return false;
					}
				}
			}
			else {
				temp += c;
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
					if (!saveData()) /* Save data for command */
						return false;
				}
			}
			else if (out) {
				if (c != '>') {
					getStdout = true;
					if(c != ' ')
						temp += c;
					if (c == '"')
						i -= 1;
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
					if (!saveData()) /* Save data for command */
						return false;
					/* vaules for new command */
					commandOK = false;
					pipe = true;
					temp = "";
					break;
				/* Start of param */
				case PARAMPREFIX:
					if (!saveData()) /* Save data for command */
						return false;
					getParam = true;
					break;
				/* Start of stdin */
				case STDIN:
					if (!saveData()) /* Save data for command */
						return false;
					getStdin = true;
					break;
				/* Start of stdout */
				case STDOUT:
					if (!saveData()) /* Save data for command */
						return false;
					out = true;
					break;
				/* Start of line in quotes */
				case QUOTE:
					openQuote = true;
					temp = "";
					break;
				/* For space */
				case SPACE:
					if (!checkSpace()) /* Check if is something to save */
						return false;					
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
	} /* Whole input was processed */

	/* Check if pipe has follower */
	if (!commandOK && pipe) {
		std::cout << "After pipe must follow a valid command!" << std::endl;
		return false;
	}
	/* Check if quoted data are closed */
	else if (openQuote) {
		std::cout << "Quoted data are not closed!" << std::endl;
		return false;
	}
	/* Save data for command */
	else if (!commandList.empty()) {
		if (!saveData()) /* Save data for command */
			return false;
	}
	/* Input isn't valid command */
	else {
		if(temp != "") {
			std::cout << "Command " << temp << " is not allowed!" << std::endl;
		}
		return false;
	}

	return true;
}
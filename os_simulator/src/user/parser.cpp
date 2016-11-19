#include "parser.h"

std::regex validCommand("^[a-zA-Z]+$"); /* regex for commands */
std::regex validFilename("[<>:\"/|?*\x01-\x1F]"); /* regex for filename */
static std::unordered_set<char> specialSymbols{ ' ', '<', '>', '|', '/', '.' }; /* Symbols with special meaning */

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
		if (std::regex_search(temp, validFilename)) {
			vmprintf(THANDLE_STDERR, "Stdin is not valid.\n");
			return false;
		}

		commandList.back().redirectStdin = temp; /* add stdin for command to struct */
		getStdin = false;
		temp = "";
	}
	else if (getStdout) {
		if (std::regex_search(temp, validFilename)) {
			vmprintf(THANDLE_STDERR, "Stdout is not valid.\n");
			return false;
		}

		commandList.back().redirectStdout = temp; /* add stdout for command to struct */
		getStdout = false;
		temp = "";
	}
	else if (getAStdout) {
		if (std::regex_search(temp, validFilename)) {
			vmprintf(THANDLE_STDERR, "Stdout is not valid.\n");
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
		unsigned char c = input.at(i); // current char

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
					std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower); /* transform command to lowercase */
					commandList.push(Command()); /* add to vector new command */
					commandList.back().name = temp; /* add command name to struct */
					temp = "";
					if(c != ' ' && i != last)
						i -= 1; //if actual char is special symbol, we need use it in next step again
				}
				else {
					if (!temp.empty()) {
						vmprintf(THANDLE_STDERR, "Command %s is not allowed!\n", temp.c_str());
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
							vmprintf(THANDLE_STDERR, "Parameter %c is not supported.\n", c);
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
		vmprintf(THANDLE_STDERR, "After pipe must follow a valid command!\n");
		return false;
	}
	/* Check if quoted data are closed */
	else if (openQuote) {
		vmprintf(THANDLE_STDERR, "Quoted data are not closed!\n");
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
			vmprintf(THANDLE_STDERR, "Command %s is not allowed!\n", temp.c_str());
		}
		return false;
	}

	return true;
}
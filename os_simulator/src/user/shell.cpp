#include "parser.h"
#include "vmstdio.h"


size_t __stdcall shell(const CONTEXT &regs)
{
	{
		size_t written;

		THandle testtxt = Create_File("test.txt", FH_OPEN_OR_CREATE); // nahradte systemovym resenim, zatim viz Console u CreateFile na MSDN
		const char* hello = "Hello world!\nAhoj svete!\n\nC test\nZ\nA";
		Write_File(testtxt, hello, strlen(hello), written);

		THandle testtxt2 = Create_File("test.txt", FH_OPEN_OR_CREATE); // nahradte systemovym resenim, zatim viz Console u CreateFile na MSDN

		Close_File(testtxt);

		testtxt = Create_File("cmd.txt", FH_OPEN_EXISTING);
		if (testtxt == INVALID_THANDLE) {
			testtxt = Create_File("cmd.txt", FH_OPEN_OR_CREATE); // nahradte systemovym resenim, zatim viz Console u CreateFile na MSDN
			const char* hello2 = "sort test.txt";
			Write_File(testtxt, hello2, strlen(hello2), written);
			Close_File(testtxt);
		}

		testtxt = Create_File("test.txt", FH_OPEN_EXISTING); // nahradte systemovym resenim, zatim viz Console u CreateFile na MSDN
		char buffer[255];
		size_t read;
		Read_File(testtxt, (const void*) &buffer, 255, read);
		buffer[read] = 0;
		Close_File(testtxt);
	}

	bool stdinIsRedirected = (Probe_File(THANDLE_STDIN, PROBE__IS_INTERACTIVE) == FALSE);
	bool runShell = true;

	/* Shell loop */
	while (runShell) {
		if (!stdinIsRedirected) {
			vmprintf("%s>", Get_Cwd().c_str());
		}

		auto line = vmgetline();
		if (line == nullptr) break;

		if (stdinIsRedirected) {
			vmprintf("%s>%s\n", Get_Cwd().c_str(), line.get());
		}

		parser p;
		/* If is parsing ok */
		if (p.parse(line.get())) {
			int commandOrder = 0;
			std::vector<pid_t> processes; //all processes from command line
			std::vector<std::pair<THandle, THandle>> pipes; //pipes for commands

			/* Sets up pipes */
			if(p.commandList.size() > 1) {
				for (int i = 0; i < p.commandList.size() - 1; i++) {
					THandle pRead, pWrite;
					Create_Pipe(pRead, pWrite);
					pipes.push_back(std::make_pair(pRead, pWrite));
				}
			}

			/* Processes whole input command */
			while (p.commandList.size() > 0) {
				Command command = p.commandList.front();
				THandle hstdin;
				THandle hstdout;
				THandle hstderr = THANDLE_STDERR;
				bool closeHstdin = true;
				bool closeHstdout = true;

				/* Sets up Handler for stdin */
				if (command.redirectStdin.length() > 0) { // stdin from file
					hstdin = Create_File(command.redirectStdin, FH_OPEN_EXISTING | FH_SHARED_READ);
					if (commandOrder != 0)
						Close_File(pipes.at(commandOrder - 1).first); // close read end of pipe

					if (hstdin == INVALID_THANDLE) {
						vmprintf(THANDLE_STDERR, "The system cannot open the file specified.\n");
						break; // unable to open file
					}
				}
				else if (commandOrder == 0) { // stdin from console (only for first command)
					hstdin = THANDLE_STDIN;
					closeHstdin = false;
				}
				else { // stdin from pipe
					hstdin = pipes.at(commandOrder - 1).first;
				}

				/* Sets up Handler for stdout */
				if (command.redirectStdout.length() > 0) { // stdout into file
					hstdout = Create_File(command.redirectStdout, FH_CREATE_ALWAYS);
					if (p.commandList.size() > 1)
						Close_File(pipes.at(commandOrder).second); // close write end of pipe

					if (hstdout == INVALID_THANDLE) {
						vmprintf(THANDLE_STDERR, "The system cannot open the file specified.\n");
						if (closeHstdin) Close_File(hstdin);
						break; // unable to open file
					}
				}
				else if (command.redirectAStdout.length() > 0) { // append stdout into file
					hstdout = Create_File(command.redirectAStdout, FH_OPEN_OR_CREATE | FH_FILE_APPEND);
					if (p.commandList.size() > 1)
						Close_File(pipes.at(commandOrder).second); // close write end of pipe

					if (hstdout == INVALID_THANDLE) {
						vmprintf(THANDLE_STDERR, "The system cannot open the file specified.\n");
						if (closeHstdin) Close_File(hstdin);
						break; // unable to open file
					}
				}
				else if (p.commandList.size() == 1) { // stdout to console (only for last command)
					hstdout = THANDLE_STDOUT;
					closeHstdout = false;
				}
				else { // stdout to pipe
					hstdout = pipes.at(commandOrder).second;
				}


				/* Prepare commands to launch */
				if (command.name == "cd") { /* cd command */
					if (command.params.size() > 0) {
						Set_Cwd(command.params.at(0));
					}
					else {
						vmprintf(hstdout, "%s\n", Get_Cwd().c_str());
					}
				}
				else if (command.name == "rd") { /* rd command */
					if (command.params.size() > 0) {
						if (!Remove_Directory(command.params.at(0))) {
							vmprintf(hstderr, "Failed.\n");
						}
					}
				}
				else if (command.name == "md") { /* md command */
					if (command.params.size() > 0) {
						if (!Make_Directory(command.params.at(0))) {
							vmprintf(hstderr, "Failed.\n");
						}
					}
				}
				else if (command.name == "exit") { /* exit command */
					runShell = false;
				}
				else { /* Creating process for users programs */
					pid_t process = Create_Process(command.name, command.params, hstdin, hstdout, hstderr);
					if (process == -1) {
						vmprintf(THANDLE_STDERR, "'%s' is not recognized as an internal or external command\nor operable program.\n", command.name.c_str());
					}
					else {
						processes.push_back(process); // add command into vector
					}
				}


				// close handles owned by shell (except its own stdin/out)
				if (closeHstdin) Close_File(hstdin);
				if (closeHstdout) Close_File(hstdout);

				p.commandList.pop();
				commandOrder++;
			}

			//join all processes
			while (!processes.empty()) {
				Join_Process(processes.back());
				processes.pop_back();
			}

			if(runShell) vmprintf("\n");
		}
		else {
			//vmprintf("Parsing failed\n");
		}
		Probe_File(THANDLE_STDIN, PROBE__CLEAR_BUFFER); // reset stdin buffer
	}


	return 0;
}
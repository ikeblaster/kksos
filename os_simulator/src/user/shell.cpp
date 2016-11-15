#include "shell.h"


size_t __stdcall shell(const CONTEXT &regs)
{
	PROCESSSTARTUPINFO psi = *(PROCESSSTARTUPINFO*)regs.Rcx;

	{
		size_t written;

		THandle testtxt = Create_File("test.txt", FH_OPEN_OR_CREATE); // nahradte systemovym resenim, zatim viz Console u CreateFile na MSDN
		const char* hello = "Hello world!\nAhoj svete!\n\nC test\nZ\nA";
		Write_File(testtxt, hello, strlen(hello), written);
		Close_File(testtxt);

		testtxt = Create_File("test.txt", FH_OPEN_EXISTING); // nahradte systemovym resenim, zatim viz Console u CreateFile na MSDN
		char buffer[255];
		char *p_buffer = &buffer[0];
		size_t read;
		Read_File(testtxt, (const void **)&p_buffer, 255, read);
		buffer[read] = 0;
		Close_File(testtxt);
	}


	/* Shell loop */
	while (true) {
		vmprintf("%s>", Get_Cwd().c_str());

		auto line = vmgetline();
		if(line == nullptr) break;

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
				THandle hstdin = nullptr;
				THandle hstdout = nullptr;
				THandle hstderr = THANDLE_STDERR;
				bool closeHstdin = true;
				bool closeHstdout = true;


				/* Sets up Handler for stdin */
				if (command.redirectStdin.length() > 0) { // stdin from file
					hstdin = Create_File(command.redirectStdin.c_str(), FH_OPEN_EXISTING);
					if (commandOrder != 0)
						Close_File(pipes.at(commandOrder - 1).first); //close read end of pipe

					if (hstdin == nullptr) {
						vmprintf("Error: Unable to open file %s\n", command.redirectStdin.c_str());
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
					hstdout = Create_File(command.redirectStdout.c_str(), FH_OPEN_OR_CREATE);
					if (p.commandList.size() > 1)
						Close_File(pipes.at(commandOrder).second); // close write end of pipe

					if (hstdout == nullptr) {
						vmprintf("Error: Unable to open file %s\n", command.redirectStdout.c_str());
						break; // unable to open file
					}
				}
				else if (command.redirectAStdout.length() > 0) { // append stdout into file
					hstdout = Create_File(command.redirectAStdout.c_str(), FH_OPEN_OR_CREATE | FH_FILE_APPEND);
					if (p.commandList.size() > 1)
						Close_File(pipes.at(commandOrder).second); // close write end of pipe

					if (hstdout == nullptr) {
						vmprintf("Error: Unable to open file %s\n", command.redirectAStdout.c_str());
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
				if (command.name == "cd") { /* Launch cd command */
					if (command.data.size() > 0) {
						Set_Cwd(command.data.at(0));
					}
					else {
						vmprintf(hstdout, "%s\n", Get_Cwd().c_str());
					}
				}
				else if (command.name == "rd") { /* Launch rd command */
					if (command.data.size() > 0) {
						Remove_Directory(command.data.at(0));
					}
				}
				else if (command.name == "md") { /* Launch md command */
					if (command.data.size() > 0) {
						Make_Directory(command.data.at(0));
					}
				}
				else if (command.name == "chkdir") { /* Launch md command */
					if (command.data.size() > 0) {
						Check_Cwd(command.data.at(0));
					}
				}
				else { /* Creating process for users programs */
					pid_t process = Create_Process(command.name, command.params, command.data, hstdin, hstdout, hstderr);
					processes.push_back(process); // add command into vector					
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

			Seek_File(THANDLE_STDIN, 0, std::ios_base::end); // reset stdin buffer
		}
		else {
			//vmprintf("Parsing failed\n");
		}
	}
	
	
	/*
	THandle pRead1, pWrite1;
	THandle pRead2, pWrite2;

	Create_Pipe(pRead1, pWrite1);
	Create_Pipe(pRead2, pWrite2);

	int p1 = Create_Process("sort", {}, { "test.txt" }, Get_Std_Handle(ITHANDLE_STDIN), pWrite1, Get_Std_Handle(ITHANDLE_STDERR));
	int p2 = Create_Process("wc", {}, {}, pRead1, pWrite2, Get_Std_Handle(ITHANDLE_STDERR));
	int p3 = Create_Process("sort", {}, {}, pRead2, Get_Std_Handle(ITHANDLE_STDOUT), Get_Std_Handle(ITHANDLE_STDERR));

	Close_File(pRead1);
	Close_File(pRead2);
	Close_File(pWrite1);
	Close_File(pWrite2);

	Join_Process(p1);
	Join_Process(p2);
	Join_Process(p3);

	vmprintf("\n");

	vmprintf("%s>\n", Get_Cwd().c_str());
	Set_Cwd("slozka");
	vmprintf("%s>\n", Get_Cwd().c_str());
	Set_Cwd("podslozka");
	vmprintf("%s>\n", Get_Cwd().c_str());
	Set_Cwd("\\");
	vmprintf("%s>\n", Get_Cwd().c_str());
	*/


	//THandle conout = Create_File("STDOUT", FILE_SHARE_WRITE); // nahradte systemovym resenim, zatim viz Console u CreateFile na MSDN
	//const char* hello = "Hello world!\n";
	//size_t written;
	//Write_File(conout, hello, strlen(hello), written);
	//Close_File(conout);

	return 0;
}
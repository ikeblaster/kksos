#include "shell.h"


size_t __stdcall shell(const CONTEXT &regs)
{
	PROCESSSTARTUPINFO psi = *(PROCESSSTARTUPINFO*)regs.Rcx;

	{
		size_t written;

		THandle testtxt = Create_File("test.txt", 0); // nahradte systemovym resenim, zatim viz Console u CreateFile na MSDN
		const char* hello = "Hello world!\nAhoj svete!\n\nC test\nZ\nA";
		Write_File(testtxt, hello, strlen(hello), written);
		Close_File(testtxt);

		testtxt = Create_File("test.txt", OPEN_EXISTING); // nahradte systemovym resenim, zatim viz Console u CreateFile na MSDN
		char buffer[255];
		char *p_buffer = &buffer[0];
		size_t read;
		Read_File(testtxt, (const void **)&p_buffer, 255, read);
		buffer[read] = 0;
		Close_File(testtxt);
	}


	/* Shell loop */
	while (true) {
<<<<<<< HEAD
		vmprintf("%s>", Get_Cwd().c_str()); 

		std::unique_ptr<const char[]> line;
		while(true) {
			line = vmgetline();
			if (line == nullptr) break;
			if (strlen(line.get()) != 0) break; // fix problemu: 1) spustit wc 2) napsat test, ctrl-z, enter
		}

		if(line == nullptr) break;
=======
		vmprintf("%s>", Get_Cwd().c_str());		

		auto line = vmgetline(input);		
		if (line == nullptr) break;	
>>>>>>> origin/master

		parser p;
		/* If is parsing ok */
		if (p.parse(line.get())) {			
			int commandOrder = 0; 
			std::vector<int> processes; //all processes from command line
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
				THandle hstdin = nullptr;
				THandle hstdout = nullptr;
				THandle hstderr = Get_Std_Handle(IHANDLE_STDERR);

				/* Sets up Handler for stdin */
				if (p.commandList.front().redirectStdin.length() > 0) { //stdin from file
					hstdin = Create_File(p.commandList.front().redirectStdin.c_str(), OPEN_EXISTING); 
					if(commandOrder != 0)
						Close_File(pipes.at(commandOrder - 1).first); //close read end of pipe
				}
				else if (commandOrder == 0) { //stdin from console (only for first command)
					hstdin = Get_Std_Handle(IHANDLE_STDIN);					
				}
				else { //stdin from pipe
					hstdin = pipes.at(commandOrder - 1).first;
				}

				/* Sets up Handler for stdout */
				if (p.commandList.front().redirectStdout.length() > 0) { //stdout into file
					hstdout = Create_File(p.commandList.front().redirectStdout.c_str(), 0);
					if(p.commandList.size() > 1)
						Close_File(pipes.at(commandOrder).second); //close write end of pipe
				}
				else if (p.commandList.front().redirectAStdout.length() > 0) { //append stdout into file
					hstdout = Create_File(p.commandList.front().redirectAStdout.c_str(), OPEN_EXISTING);
					Seek_File(hstdout, std::ios_base::end, 0); //move to end of file // TODO: predelat jako flag FILE_APPEND?
					if (p.commandList.size() > 1)
						Close_File(pipes.at(commandOrder).second); //close write end of pipe
				}
				else if (p.commandList.size() == 1) { //stdout to console (only for last command)
					hstdout = Get_Std_Handle(IHANDLE_STDOUT);
				}
				else { //stdout to pipe
					hstdout = pipes.at(commandOrder).second;
				}

				/* Prepare commands to launch */
				if (p.commandList.front().name == "cd") { /* Launch cd command */
					
				}				
				else if (p.commandList.front().name == "rd") { /* Launch rd command */

				}				
				else if (p.commandList.front().name == "md") { /* Launch md command */

				}
				else { /* Creating process for users programs */					
					int process = Create_Process(p.commandList.front().name, p.commandList.front().params, p.commandList.front().data,
						hstdin, hstdout, hstderr);
					processes.push_back(process); //add command into vector					
				}

				p.commandList.pop();
				commandOrder++;
			}

			//join all processes
			while (!processes.empty()) {
				Join_Process(processes.back());
				processes.pop_back();
			}	
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

	int p1 = Create_Process("sort", {}, { "test.txt" }, Get_Std_Handle(IHANDLE_STDIN), pWrite1, Get_Std_Handle(IHANDLE_STDERR));
	int p2 = Create_Process("wc", {}, {}, pRead1, pWrite2, Get_Std_Handle(IHANDLE_STDERR));
	int p3 = Create_Process("sort", {}, {}, pRead2, Get_Std_Handle(IHANDLE_STDOUT), Get_Std_Handle(IHANDLE_STDERR));

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
	vmprintf("%s>\n", Get_Cwd().c_str());*/



	//THandle conout = Create_File("STDOUT", FILE_SHARE_WRITE); // nahradte systemovym resenim, zatim viz Console u CreateFile na MSDN
	//const char* hello = "Hello world!\n";
	//size_t written;
	//Write_File(conout, hello, strlen(hello), written);
	//Close_File(conout);

	return 0;
}
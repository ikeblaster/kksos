#include "shell.h"


size_t __stdcall shell(const CONTEXT &regs)
{
	PROCESSSTARTUPINFO psi = *(PROCESSSTARTUPINFO*) regs.Rcx;

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
	vmprintf("%s>\n", Get_Cwd().c_str());



	//THandle conout = Create_File("STDOUT", FILE_SHARE_WRITE); // nahradte systemovym resenim, zatim viz Console u CreateFile na MSDN
	//const char* hello = "Hello world!\n";
	//size_t written;
	//Write_File(conout, hello, strlen(hello), written);
	//Close_File(conout);


	/*
	std::cout << std::endl << "-------------------" << std::endl;

	parser p;
	if(p.parse("sort < vstup.txt < vstup2.txt nejaky data \"asd asd as < cesta\\k\\souboru\" >> vystupniSoubor.txt | wc /W/D>vystupniProWC.txt \"params for\" wcc1|dir path\\to\\dir")) {
		std::cout << " Parsed successfully" << std::endl;
	}
	else {
		std::cout << " Parsing failed" << std::endl;
	}*/

	return 0;
}

#include "shell.h"
#include "rtl.h"
#include "parser.h"

size_t __stdcall shell(const CONTEXT &regs) 
{
	size_t written;


	THandle testtxt = Create_File("test.txt", 0); // nahradte systemovym resenim, zatim viz Console u CreateFile na MSDN
	const char* hello = "Hello world!\n";
	Write_File(testtxt, hello, strlen(hello), written);
	Close_File(testtxt);

	testtxt = Create_File("test.txt", OPEN_EXISTING); // nahradte systemovym resenim, zatim viz Console u CreateFile na MSDN
	char buffer[255];
	char *p_buffer = &buffer[0];
	size_t read;
	Read_File(testtxt, (const void **)&p_buffer, 255, read);
	buffer[read] = 0;
	Close_File(testtxt);

	THandle conout = Create_File("CONOUT$", 0); // nahradte systemovym resenim, zatim viz Console u CreateFile na MSDN
	Write_File(conout, buffer, 255, written);
	Close_File(conout);


	//THandle conout = Create_File("CONOUT$", FILE_SHARE_WRITE); // nahradte systemovym resenim, zatim viz Console u CreateFile na MSDN
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

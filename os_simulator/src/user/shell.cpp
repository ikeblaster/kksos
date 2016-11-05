#include "shell.h"
#include "rtl.h"
#include "parser.h"

size_t __stdcall shell(const CONTEXT &regs) 
{
	THandle conout = Create_File("CONOUT$", FILE_SHARE_WRITE); // nahradte systemovym resenim, zatim viz Console u CreateFile na MSDN
	const char* hello = "Hello world!";
	size_t written;
	Write_File(conout, hello, strlen(hello), written);
	Close_File(conout);


	std::cout << std::endl << "-------------------" << std::endl;

	parser p;
	if(p.parse("sort < vstup.txt < vstup2.txt nejaky data \"asd asd as < cesta\\k\\souboru\" >> vystupniSoubor.txt | wc /W/D>vystupniProWC.txt \"params for\" wcc1|dir path\\to\\dir")) {
		std::cout << " Parsed successfully" << std::endl;
	}
	else {
		std::cout << " Parsing failed" << std::endl;
	}

	return 0;
}

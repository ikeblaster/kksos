#include "shell.h"
#include "parser.h"
#include "sort.h"
#include "wc.h"


size_t __stdcall shell(const CONTEXT &regs)
{

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

		//THandle conout = Create_File(nullptr, IHANDLE_STDOUT); // nahradte systemovym resenim, zatim viz Console u CreateFile na MSDN
		//Write_File(conout, buffer, read, written);
		//Close_File(conout);

		/*testtxt = Create_File(nullptr, IHANDLE_CONSOLE);
		Read_File(testtxt, (const void **) &p_buffer, 1, read);
		Close_File(testtxt);*/

		//printf("%s\n\n", vmgetline().get());
	}

	CONTEXT sortregs;
	std::vector<std::string> args{ "test.txt" };
	sortregs.Rcx = (decltype(regs.Rcx)) &args;

	sort(sortregs);

	wc(sortregs);


	//std::string text = buffer.str(); // text will now contain "Bla\n"


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

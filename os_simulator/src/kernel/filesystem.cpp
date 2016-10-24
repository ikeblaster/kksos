#include <cstdio>
#include "filesystem.h"

int main() {
	
	fsfile *soubor = new fsfile;
	soubor->data = "hello world";

	fsdir slozka;
	slozka.children.insert(std::make_pair("aaa", soubor));

	fsfile *tmp = (fsfile*)slozka.children.at("aaa");
	printf(tmp->data.c_str());

	return 0;
}

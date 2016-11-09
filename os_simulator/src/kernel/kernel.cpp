#include "kernel.h"

HMODULE User_Programs;

void Set_Error(const bool failed, CONTEXT &regs) {
	if (failed) {
		stc(regs.EFlags);
		regs.Rax = GetLastError();
	}
	else
		clc(regs.EFlags);
}


void Initialize_Kernel() {
	User_Programs = LoadLibrary(L"user.dll");	
}

void Shutdown_Kernel() {
	FreeLibrary(User_Programs);
}

void SysCall(CONTEXT &regs) {

	switch (Get_AH((__int16) regs.Rax)) {
		case scIO:		
			HandleIO(regs);
			break;
		case scProcess:
			HandleProcess(regs);
			break;
	}

}


void Initialize_FS() {
	FileSystem::fs = new FileSystem::Directory();
	FileSystem::fs_root = FileSystem::fs->createDirectory("C:");
	FileSystem::fs_root->createDirectory("slozka")->createDirectory("podslozka"); // TODO: smazat
}

void Shutdown_FS() {
	delete FileSystem::fs; 
}


void Run_VM() {

	Initialize_FS();

	/*
	auto* dir = cdrive->createDirectory("dir");
	auto* subdir = dir->createDirectory("subdir");
	auto* ssf = subdir->createFile("subsubfile.txt");

	//printf("%s\n", Path::generate(Path::getDriveRoot(subdir)).c_str());


	// vytvareni/otevirani souboru - bud je soubor nalezen, nebo je nalezena cela cesta krome souboru, ktery vytvorime (obdobne to bude fungovat pro mkdir)
	Directory* directory;
	File* file;

	std::string path = "C:\\dir\\somefile.txt";

	RESULT res = Path::parse(dir, path, &directory, &file);

	if (res == RESULT::MISSING_LAST_PART) {
		file = directory->createFile(Path::getBasename(path));
		res = RESULT::OK;
	}
	if (res == RESULT::OK) {
		printf("Directory: %s\n", Path::generate(directory).c_str());
		printf("File: %s\n", Path::generate(file).c_str());
	}

	delete fs;

	printf("-------------------\n");*/

	/*File* file = root->createFile("ahoj");
	file->setData("ahoj");
	FileHandle* fh = new FileHandle(file,false);
	size_t written = 0;

	char buff[100] = { 0 };
	char* p = &buff[0];

	fh->readFile(&p, 100, &written);

	int posseek = fh->seek(0, std::ios_base::end);
	fh->writeFile(" svete", 6, &written);

	int pos = fh->tell();

	fh->closeFile();

	printf("%s\n", buff);
	printf("%s %d %d\n\n", file->getData().c_str(), posseek, pos);*/


	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	Initialize_Kernel();

	FileSystem::IHandle* console = FileSystem::Utils::CreateHandle(nullptr, nullptr, IHANDLE_CONSOLE);

	PROCESSSTARTUPINFO psi;
	psi.process_name = "shell";
	psi.p_stdin = console;
	psi.p_stdout = console;
	psi.p_stderr = console;

	int pid = Process::create_process(psi);
	Process::join_process(pid);
	
	console->close();
	

	//// spustime shell - v realnem OS bychom ovsem spousteli login
	//TEntryPoint shell = (TEntryPoint)GetProcAddress(User_Programs, "shell");
	//if (shell) {
	//	CONTEXT regs; // ted je regs jenom nejak vyplneno kvuli preakladci
	//	GetThreadContext(GetCurrentThread(), &regs); // ale pak bude jeden z registru ukazovat na nejaky startup info blok
	//	/*Process::current_thread_pcb = new Process::PCB();
	//	Process::current_thread_pcb->psi = psi;*/

	//	shell(regs);
	//}

	Shutdown_FS();
	Shutdown_Kernel();
}


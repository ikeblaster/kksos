#pragma once
#include <Windows.h>
#include <string>
#include <vector>

// Filehandle constants
#define FH_OPEN_EXISTING 1
#define FH_OPEN_OR_CREATE 2
#define FH_CREATE_ALWAYS 4
#define FH_FILE_APPEND 8
#define FH_SHARED_READ 16 // default file access is "exclusive open (rw)"

// Filesystem constants
#define FS_MAKE_MISSING_DIRS 1

// FSHandle constants
#define PROBE__CLEAR_BUFFER 1
#define PROBE__IS_INTERACTIVE 2
#define PROBE__SET_LINEMODE 3
#define PROBE__SET_CHARMODE 4
#define PROBE__FILE_ACCESS 5

// Standard stream handles
#define THANDLE_STDIN (THandle) 0
#define THANDLE_STDOUT (THandle) 1
#define THANDLE_STDERR (THandle) 2
#define INVALID_THANDLE (THandle) -1

#define CHAR_EOF 0x1A

typedef DWORD64 pid_t;
typedef DWORD64 flags_t;
typedef HANDLE THandle;

typedef size_t (__stdcall *TEntryPoint)(const CONTEXT syscall); // vstupni bod uzivatelskeho programu, v rcx je pointer ja std::list<std::string> s argumenty

struct PROCESSSTARTUPINFO
{
	std::string process_name;
	std::vector<std::string> params;

	TEntryPoint subprocess_entry = nullptr;

	THandle h_stdin;
	THandle h_stdout;
	THandle h_stderr;
};

/*

   Cisla funkci OS:
	 AH - major cislo aka id skupiny fci
	 AL - minor cislo aka cisle konkretni fce

	 pokud je po volani nastaven carry flag, tj. CONTEXT::Eflags & CF != 0, pak nastala chyba
	 
	  AH == 1 : IO operace
		AL: cislo operace
			1 - otevri soubor				 IN: rdx je pointer na std::string udavajici cestu k souboru; rcx jsou flags k otevreni souboru
											OUT: rax je handle nove otevreneho souboru
			2 - zapis do souboru			 IN: rdx je handle souboru, rdi je pointer na buffer, rcx je pocet bytu v bufferu k zapsani
											OUT: rax je pocet zapsanych bytu
			3 - cti ze souboru			     IN: rdx je handle souboru, rdi je pointer na buffer, rcx je max. pocet bytu k precteni 
											OUT: rax je pocet prectenych bytu
			4 - nastav pozici v souboru	     IN: rdx je handle souboru, rdi je nova pozice, rcx je "smer" kurzoru 
											OUT: rax >= 0 je pozice v souboru, jinak chyba
			5 - zavri soubor				 IN: rdx je handle souboru k zavreni
			6 - vytvor pipe 				OUT: rcx je handle pro cteci konec roury, rdx pro zapisovaci konec roury
			7 - vytvor adresar				 IN: rdx je pointer na std::string udavajici cestu ke slozce
			8 - ziskej obsah adresare		 IN: rdx je pointer na std::list<std::string>, do ktereho se zapisi jednotlive polozky
			9 - odstran adresar				 IN: rdx je pointer na std::string udavajici cestu ke slozce
		   10 - probe souboru				 IN: rdx je handle; rcx jsou flags pro probe
											OUT: rax je navratova hodnota dle probe funkce
								
	 
	  AH == 2 : Process operace
		AL: cislo operace
			1 - vytvor proces				 IN: rcx je pointer na PROCESSSTARTUPINFO strukturu
											OUT: rax >= 0 je PID noveho procesu, jinak chyba
			2 - cekej na proces				 IN: rcx je PID procesu
			3 - ziskej aktualni CWD			 IN: rcx je pointer na std::string
			4 - zmen CWD					 IN: rcx je pointer na std::string
			5 - ziskej seznam procesu		 IN: rdx je pointer na std::list<std::string>, do ktereho se zapisi jednotlive polozky


*/


// ah hodnoty 
const __int8 scIO = 1; // IO operace
const __int8 scProcess = 2;

// al hodnoty pro scIO 
const __int8 scCreateFile = 1;
const __int8 scWriteFile = 2;
const __int8 scReadFile = 3;
const __int8 scSeekFile = 4;
const __int8 scCloseFile = 5;
const __int8 scCreatePipe = 6;
const __int8 scMakeDirectory = 7;
const __int8 scListDirectory = 8;
const __int8 scRemoveDirectory = 9;
const __int8 scProbeFile = 10;

// al hodnoty pro scProcess
const __int8 scCreateProcess = 1;
const __int8 scJoinProcess = 2;
const __int8 scGetCwd = 3;
const __int8 scSetCwd = 4;
const __int8 scListProcesses = 5;




const __int64 CarryFlag = 1;
constexpr DWORD clc(const DWORD flags) {
	return flags & (~CarryFlag);
}

constexpr DWORD stc(const DWORD flags) {
	return flags | CarryFlag;
}

constexpr bool test_cf(const DWORD flags) {
	return flags & CarryFlag;
}

constexpr __int16 Compose_AX(const __int8 ah, const __int8 al) {
	return (ah << 8) | al;
}

constexpr __int8 Get_AH(const __int16 ax) {
	return ax >> 8;
}

constexpr __int8 Get_AL(const __int16 ax) {
	return ax & 0xFF;
}

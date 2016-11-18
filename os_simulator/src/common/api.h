#pragma once
#include <Windows.h>
#include <string>
#include <vector>

// Filehandle constants
#define FH_OPEN_EXISTING 1
#define FH_OPEN_OR_CREATE 2
#define FH_CREATE_ALWAYS 4
#define FH_FILE_APPEND 8
#define FH_SHARED_READ 16 // default file access is "exclusive open"

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

typedef size_t (__stdcall *TEntryPoint)(const CONTEXT syscall); // vstupni bod uzivatelskeho programu
typedef void (__stdcall *TSysCall)(CONTEXT &context); // prototyp funkce, ktera realizuje syscall

typedef struct PROCESSSTARTUPINFO
{
	std::string process_name;
	std::vector<char> params;
	std::vector<std::string> data;

	TEntryPoint subprocess_entry = nullptr;

	THandle h_stdin;
	THandle h_stdout;
	THandle h_stderr;

} PROCESSSTARTUPINFO;

/*

   Cisla funkci OS:
	 AH - major cislo aka id skupiny fci
	 AL - minor cislo aka cisle konkretni fce

	 pokud je po volani nastaven carry flag, tj. CONTEXT::Eflags & CF != 0, pak Rax je kod chyby

	  AH == 1 : IO operace
		AL: cislo IO operace	// konzole je take jenom soubor
			1 - otevrit soubor				 IN: rdx je pointer na null-terminated string udavajici file_name; rcx jsou flags k otevreni souboru
											OUT: rax je handle nove otevreneho souboru
			2 - zapis do souboru			 IN: rdx je handle souboru, rdi je pointer na buffer, rcx je pocet bytu v bufferu k zapsani
											OUT: rax je pocet zapsanych bytu
			3 - cti ze souboru
			4 - nastav pozici v souboru
			5 - zavri soubor				 IN: rdx  je handle souboru k zavreni
											OUT: rax != 0 je uspech, jinak chyba


												

   Dalsi cisla si doplnte dle potreby


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
const __int8 scGetStdHandle = 3;
const __int8 scSetStdHandle = 4;
const __int8 scGetCwd = 5;
const __int8 scSetCwd = 6;
const __int8 scListProcesses = 7;
const __int8 scCheckCwd = 8;




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

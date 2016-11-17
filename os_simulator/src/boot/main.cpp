#include <Windows.h>

#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h> 
#include <crtdbg.h>

typedef void(__stdcall *TRun_VM)();


BOOL WINAPI HandlerRoutine(_In_ DWORD dwCtrlType) {
	return dwCtrlType == CTRL_C_EVENT || dwCtrlType == CTRL_CLOSE_EVENT;
}

int main() {

	SetConsoleCtrlHandler(HandlerRoutine, TRUE); // ctrl-c/close event gracefully kills threads and let cleanup run

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // TODO: delete memleak detector

	HMODULE kernel = LoadLibrary(L"kernel.dll");
	TRun_VM vm = (TRun_VM)GetProcAddress(kernel, "Run_VM");
	if (vm) vm();
	FreeLibrary(kernel);

	system("pause"); // TODO: smazat, jen pro debug

	return 0;
}

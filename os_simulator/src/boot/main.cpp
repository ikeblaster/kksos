#include <Windows.h>

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h> 
#include <crtdbg.h>
#endif

typedef void(__stdcall *TRun_VM)();


BOOL WINAPI HandlerRoutine(_In_ DWORD dwCtrlType) {
	return dwCtrlType == CTRL_C_EVENT || dwCtrlType == CTRL_CLOSE_EVENT;
}

int main() {

	#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	SetConsoleCtrlHandler(HandlerRoutine, TRUE); // ctrl-c/close event gracefully kills threads and let cleanup run

	HMODULE kernel = LoadLibrary(L"kernel.dll");
	TRun_VM vm = (TRun_VM)GetProcAddress(kernel, "Run_VM");
	if (vm) vm();
	FreeLibrary(kernel);

	#ifdef _DEBUG
	system("pause");
	#endif

	return 0;
}

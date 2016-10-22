#include <Windows.h>

typedef void(__stdcall *TRun_VM)();

int main() {
	HMODULE kernel = LoadLibrary(L"kernel.dll");
	TRun_VM vm = (TRun_VM)GetProcAddress(kernel, "Run_VM");
	if (vm) vm();
	FreeLibrary(kernel);

	system("pause"); // TODO: smazat, jen pro debug

	return 0;
}

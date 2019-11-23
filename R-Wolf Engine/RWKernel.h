#pragma once
#include <string>
#include <Windows.h>

inline auto attachLibrary(const char* lib, HMODULE &hmLib, bool feedback = true) {
	hmLib = LoadLibrary(lib);
	if (hmLib && feedback) {
		return true;
	}
	else {
		string report = "Не удалось подключить библиотеку: ";
		report += lib;
		report += "!\nУбедитесь в совместимости библиотеки с ядром движка.";
		MessageBox(NULL, report.c_str(), "Произошла ошибка", MB_OK | MB_ICONERROR);
		return false;
	}
}

namespace Kernel {
	__declspec(selectany) HMODULE library;
	inline auto initLibrary() {
		return attachLibrary("resources/modules/RWKernel.dll", library);
	}
	//typedef bool(__stdcall* Kernel_GetFeedback)(void);
	//typedef int(__stdcall* Kernel_KernelMain)(void);
	
	//Kernel_KernelMain KernelMain = reinterpret_cast<Kernel_KernelMain>(GetProcAddress(library, "KernelMain"));
	//Kernel_GetFeedback getFeedback = reinterpret_cast<Kernel_GetFeedback>(GetProcAddress(library, "getFeedback"));
}
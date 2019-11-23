#include <memory>
#include <Windows.h>
#pragma comment(lib, "user32.lib")

#define method __declspec(dllexport) auto __stdcall

namespace Kernel {
	method getFeedback() {
		MessageBox(NULL, "Hi", "Feedback", MB_OK);
		
		return true;
	}
}
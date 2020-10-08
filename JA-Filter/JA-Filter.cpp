#include <iostream>
#include <Windows.h>

typedef int(_fastcall* MyProc1)();

int main()
{
    HINSTANCE dllHandle = NULL;
    dllHandle = LoadLibrary(L"ASMFilter.dll");
    MyProc1 filter = (MyProc1)GetProcAddress(dllHandle, "filter");
    int i = filter();
    std::cout << "Hello World!\n"<< i;

}

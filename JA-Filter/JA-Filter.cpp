
#include <iostream>

extern "C" int _stdcall filter();

int main()
{
    int i = filter();
    std::cout << "Hello World!\n"<< i;
}

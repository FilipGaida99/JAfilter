#include <iostream>
#include <string>
#if _DEBUG
#include "vld.h"
#endif
#include "LibraryCaller.h"

int main(int argc, char* arg[])
{
    LibraryCaller library;
    std::string args;
    for (int i = 1; i < argc; i++) {
        args += arg[i];
        args += " ";
    }
    do {
        switch (library.ParseArgs(args)) {
        case Passed:
            if (library.Run())
                std::cout << "Zakonczono powodzeniem.\n";
            break;
        case ValueMiss:
            std::cout << "Jeden z argumentow nie otrzymal odpowiedniej wartosci.\n";
            break;
        case Exit:
            return 0;
        }
        std::cout << "Podaj nowe ustawienia (lub komende -exit): \n";
    } while (std::getline(std::cin,args));

    return 0;
}

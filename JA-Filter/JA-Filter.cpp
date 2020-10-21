//Temat: Filtrowanie uwypuklające skierowane na zachód.
//Algorytm będzie filtrował każdy z kolorów pixeli zgodnie z maską
//1 0 -1
//1 1 -1
//1 0 -1
//Wykonał: Filip Gaida gr.3 semestr 5, 2020/2021

#include <iostream>
#include <string>
#if _DEBUG
#include "vld.h"
#endif
#include "LibraryCaller.h"

//Procedura służąca do automatycznego testu wydajności.
void TestPerformance(LibraryCaller& library) {
    int testingThreads[] = { 1,2,3,4,5,6,7,8,16,32,48,64 };
    for (auto threads : testingThreads) {
        library.ParseArgs("-i pertest.bmp -o persave.bmp -cpp -thr " + std::to_string(threads));
        for (int i = 0; i < 10; i++) {
            library.Run();
        }
        library.ParseArgs("-asm");
        for (int i = 0; i < 10; i++) {
            library.Run();
        }
    }

}

int main(int argc, char* arg[])
{
    LibraryCaller library;
    std::string args;
    //Rozruch w trybie testowania.
    if (argc>1 && !strcmp(arg[1], "test")) {
        TestPerformance(library);
        return 0;
    }

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

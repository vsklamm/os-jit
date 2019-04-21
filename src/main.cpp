#include <iostream>

#include "compiler.h"

hello_compiler comp;

void test1()
{
    auto f = comp.build(); // empty string
    f.run();
}

void test2()
{
    auto f = comp.build("example\n");
    f.run();
}

void test3()
{
    auto f = comp.build("HDD\n");
    f.run();
    comp.patch(f, "SS");
    f.run();
}

void test4()
{
    auto f = comp.build("Text before several zeros in memory\n");
    comp.patch(f, "Text after several zeros in memory\n", 9999);
    f.run();
}

void help()
{
    std::cout << "Скучно: выводит введённый текст" << std::endl
              << "Патч: заменяет подстроку в <text_string> на введённую <patch_string> с <index>" << std::endl
              << "Usage: text_string" << std::endl
              << "       patch_string" << std::endl
              << "       index" << std::endl
              << "Ремапит память, если индекс больше text_string и выводит весь текст как он есть в памяти" << std::endl;
}

int main()
{
//    test1();
//    test2();
//    test3();
//    test4();

    help();

    std::string msg;
    std::string patch;
    std::string pos;
    while(true)
    {
        if(std::cin.eof())
        {
            break;
        }

        std::getline(std::cin, msg);
        std::getline(std::cin, patch);
        std::getline(std::cin, pos);
        int p = pos.empty() ? 0 : std::stoi(pos);

        auto f = comp.build(msg);
        f.run();
        std::cout << "\n";
        comp.patch(f, patch, p);
        f.run();
        std::cout << "\n";
    }
}



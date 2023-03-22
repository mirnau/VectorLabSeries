#pragma once
#include "TestLevel.h"

#if DEL == 1 
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG
#include <crtdbg.h>
#endif DEL


#include "Vector.hpp"

#include <type_traits>
#include <iostream>
using std::cout;
#include <cassert>

void TestBasic();
void TestVector();
void TestIter();

int main() {
#if DEL == 1
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif DEL
    std::locale::global(std::locale("swedish"));

#ifdef WRITEDESC
    WriteDesc<int>();
#endif

    std::cout <<
        "�ndra i TestLevel.h f�r att kontrollera vad som testas\n"
        "G� stegvis upp med LEVEL fr�n 1 s� l�ngt det beh�vs,  �ndra den sen till 99\n"
        "Och kontrollera vilket betyg som testas genom att definiera VG_BETYG eller G_BETYG\n"
        "Change the include of Vector.hpp to your own file\n\n"
        ;
    std::cout << "Testing Vector del " << DEL << "\n";
    std::cout << "Selected is " << 
#ifdef VG_BETYG
        "VG"
#else
#ifdef G_BETYG
        "G"
#else
        "????"
#endif
#endif
        << ", Level is: " << LEVEL << "\n\n";

    TestBasic();
    TestVector();
    TestIter();

#if DEL == 1
    new int; cout << "det finns en minnesl�cka i main, avsiktligt!\n s� ni kan se att er minnesl�ckstest fungerar\n";
#elif DEL == 2
    Dalloc<double>().allocate(4);
    //Dalloc<double>().allocate(2);
    //Dalloc<double>().allocate(1); cout << "det finns en minnesl�cka i main, avsiktligt!\n s� ni kan se att er minnesl�ckstest fungerar\n";
#else
    static_assert(false);
#endif DEL
}

//#include "Dhelper.h"
//bool Dhelper::checkDhelper{};


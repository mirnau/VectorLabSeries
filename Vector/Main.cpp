#include <crtdbg.h>
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif  // _DEBUG

#include "TestLevel.h"

#include "Vector.hpp"
#include "VectIterTest.h"

#include <type_traits>
#include <iostream>
#include <cassert>

using std::cout;

void TestBasic();
void TestVector();

int main() {
#ifdef DBG_NEW
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    std::locale::global(std::locale("swedish"));
    TestBasic();
    TestVector();
    TestIter();

    new int; cout << "det finns en minnesläcka i main, avsiktligt!\n så ni kan se att er minnesläckstest fungerar\n";
//   _CrtDumpMemoryLeaks();
    std::cin.get();
}


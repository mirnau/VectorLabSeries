#include "TestLevel.h"

#if (defined(LEVEL)  && (LEVEL>80) ||  !defined(LEVEL))   && (defined(VG_BETYG) || defined(G_BETYG))

#define CHAR char

#include "Vector.hpp"

#include "Dhelper.h"

//Generates all functions declared
//Uncomment this to check that all functions compiles.
//template class Vector<int>; 

#include <iostream>
using std::cout;
using std::endl;
#include <cassert>
#include <type_traits>

void TestRolOp();
void TestCapacity();

//template<class X>
//bool operator==(const Vector<X>& lhs, const CHAR* rhs) {
//    return lhs == Vector<X>(rhs);
//}

template <class X>
struct IsConstOrConstRef {
    static const bool value = std::is_const_v<std::remove_reference_t<X>>;
};

template <class X>
bool IsConstOrConstRefFun(X& x) {
    return std::is_const_v<std::remove_reference_t<X>>;
};

template<class X>
void CheckVec(const Vector<X>& v) {
    assert(v.Invariant());
    assert(v.size() <= v.capacity());
    for (size_t i = 0; i < v.capacity(); ++i)
        auto x = v[i];   // eller? X x(v[i]);
}
template<>
void CheckVec<Dhelper>(const Vector<Dhelper>& v) {
    assert(v.Invariant());
    assert(v.size() <= v.capacity());
    size_t i = 0;
    for (; i < v.size(); ++i)
        assert(v[i].IsConstr());
    for (; i < v.capacity(); ++i)
        assert(v[i].IsNotConstr());
}

//Test av push_back&&
void TestPushBack() {
    Vector<Dhelper> a;
    Dhelper c;
    a.reserve(4);
    CheckVec(a);
    a.push_back(c);
    CheckVec(a);
#ifdef VG_BETYG
    c.IsConstr();
    a.push_back(std::move(c));
    c.IsMoved();
#endif
}

template<class T>
void TestAss(Vector<T>& (Vector<T>::* Ass)(const Vector<T>&), bool Efficient) {
    {
        Vector<Dhelper> Bar("Bar");
        Vector<Dhelper> Fox("Fo");
        Vector<Dhelper> Foo("Fooo");
        (Fox.*Ass)(Bar);
        (Foo.*Ass)(Bar);
        assert(Foo == Fox && Fox == Bar && Bar == "Bar");
        assert(Fox.data() != Bar.data());
        Vector<Dhelper> Tom;
        (Tom.*Ass)(Fox); //Assignment to empty
        assert(Tom == Fox);
    }
    //Vector& as return value
    {
        Vector<Dhelper> Bar("Bar");
        Vector<Dhelper> Fox("Fox");
        Vector<Dhelper> Foo("Foo");
        ((Foo.*Ass)(Fox).*Ass)(Bar);
        assert(Fox == "Fox" && Foo == Bar && Bar == "Bar");
    }
    //check different size and cpacity
    {
        Vector<Dhelper> Bar("Barf");
        Vector<Dhelper> Fox; Fox.reserve(3);
        auto buffert = Fox.data();
        (Fox.*Ass)(Bar);
        assert(Fox == Bar);
        assert(buffert != Fox.data());
    }
    {
        Vector<Dhelper> Bar("Barf");
        Vector<Dhelper> Fox; Fox.reserve(4);
        auto buffert = Fox.data();
        (Fox.*Ass)(Bar);
        assert(Fox == Bar);
        if (Efficient)
            assert(buffert == Fox.data());
    }
    {
        Vector<Dhelper> Bar("Barf");
        Vector<Dhelper> Fox; Fox.reserve(5);
        auto buffert = Fox.data();
        (Fox.*Ass)(Bar);
        assert(Fox == Bar);
        if (Efficient)
            assert(buffert == Fox.data());
    }
    //use size and not capacity when allocating new space
    {
        Vector<Dhelper> Bar("BarBar");
        Vector<Dhelper> Fox("Fox");
        Fox.shrink_to_fit();
        Bar.reserve(1000000LL);
        (Fox.*Ass)(Bar);
        assert(Fox.capacity() < 500);
    }
}

#ifdef VG_BETYG
void TestVGAssignment() {
    TestAss(&Vector<Dhelper>::AssSimple, false);
    TestAss(&Vector<Dhelper>::AssStrong, false);
    TestAss(&Vector<Dhelper>::AssFast, true);
}
#endif

void TestAssignment() {
    TestAss(&Vector<Dhelper>::Ass, false);

    //Kedje tilldelning
    {
        Vector<Dhelper> Bar("Bar");
        Vector<Dhelper> Fox("Fox");
        Vector<Dhelper> Foo("Foo");
        Foo = Fox = Bar;
        assert(Foo == Fox && Fox == Bar && Bar == "Bar");
    }
    //Vector& as return value
    {
        Vector<Dhelper> Bar("Bar");
        Vector<Dhelper> Fox("Fox");
        Vector<Dhelper> Foo("Foo");
        (Foo = Fox) = Bar;
        assert(Fox == "Fox" && Foo == Bar && Bar == "Bar");
    }
    {
        //	-	operator =(Str�ng str�ng)
        Vector<Dhelper> v2, v3;
        v2 = "hej";
        v3 = "faxen";
        assert((v2 = v3) == v3);
        v2 = v3;
        assert(v2 == v3);
        assert((v2 = v2) == v3);	//self assignment
        //Ej samma buffert
        v2 = "hej";
        v3 = v2;
        v2[0] = 'x'; assert(v3[0] == 'h');
        v3[1] = 'y'; assert(v2[1] == 'e');
    }
    {//Kedjat assignment
        Vector<Dhelper> v1("foo"), v2("bar"), v3("hej");
        v3 = v2 = v1;
        assert(v3 == "foo");
        assert(v3 == v2);
        assert(v1 == v2);
    }
}

void TestVector() {
    {//Vector<Dhelper>(char *)
        Vector<Dhelper> Foo("Foo");
        const Vector<Dhelper> FooC("Foo");
        Vector<Dhelper> Bar("Bar");
        const Vector<Dhelper> BarC("Bar");
        CheckVec(Foo); CheckVec(FooC);
        CheckVec(Bar); CheckVec(BarC);
    }
    {//check empty vectors;
        Vector<Dhelper> v1;
        assert(v1.capacity() == 0);
        assert(v1.data() == nullptr);
        Vector<Dhelper> v2(v1);
        assert(v2 == "");
    }
    {//Move constructor
        Vector<Dhelper> a("foo");
        Vector<Dhelper> b(std::move(a));
        assert(b == "foo" && a.data() == nullptr);
    }
    {//Vector<Dhelper>(Copy constructor)
        Vector<Dhelper> v1("foo"); assert(v1 == "foo");
        Vector<Dhelper> v2(v1); assert(v2 == "foo");
        Vector<Dhelper> v3("bar");  assert(v3 == "bar");

        //-	~Vector<Dhelper>() Kom ih�g destruktorn!
        delete new Vector<Dhelper>("hej");
    }



    {// Move assignment
        Vector<Dhelper> Foo("Foo");
        const Vector<Dhelper> FooC("Foo");
        Vector<Dhelper> Bar("Bar");
        const Vector<Dhelper> BarC("Bar");
        CheckVec(Foo);
        CheckVec(Bar);
        Foo = std::move(Bar);
        CheckVec(Foo);
        CheckVec(Bar);
        assert(Foo == "Bar");
        assert(Bar == "" || Bar == "Foo");
        assert(Bar == "" || Bar == "Foo");  // //Till�t swap l�sning
        //assuming that we now have a nullptr in Bar
//we test that we can do a push_back;
        Bar.push_back('x');
        assert(Bar.capacity() > 0);
    }

    {//-	operator[](size_t i) som indexerar utan range check.
        Vector<Dhelper> vecFoo("Foo");
        const Vector<Dhelper> vecCFoo("Foo");
        Vector<Dhelper> vecBar("Bar");
        const Vector<Dhelper> vecBarC("Bar");

        vecBar = "bar";
        vecBar[-1]; vecBar[1000];	//No error
        assert(vecBar[1] == 'a');
        vecBar[1] = 'y';
        assert(vecBar[1] == 'y');

        //-	operator[](size_t i) const.
        assert(IsConstOrConstRefFun(vecBarC[1]));
        assert(vecBarC[1] == 'a');
    }
    {//-	at(size_t i) som indexerar med range check
        Vector<Dhelper> vecBar("Bar");
        const Vector<Dhelper> vecBarC("Bar");

        try {
            vecBar.at(-1);
            assert(false);
        }
        catch (std::out_of_range&) {
            goto exitlabel;
        };
        assert(false);
    exitlabel:
        try {
            vecBar.at(3);
            assert(false);
        }
        catch (std::out_of_range&) {};

        //- at indexerar
        try {
            vecBar.at(vecBar.size());
            assert(false);
        }
        catch (std::out_of_range&) {};

        //-	at(size_t i) 
        vecBar = "bar";
        assert(vecBar.at(1) == 'a');
        vecBar.at(1) = 'y';
        assert(vecBar.at(1) == 'y');
        assert(vecBarC.at(1) == 'a');
        assert(IsConstOrConstRefFun(vecBarC.at(1)));
    }

    // data
    {
        Vector<Dhelper> vecBar("Bar");
        const Vector<Dhelper> vecBarC("Bar");

        assert(vecBar.data() == &vecBar[0]);
        assert(!IsConstOrConstRefFun(*vecBar.data()));
        assert(IsConstOrConstRefFun(*vecBarC.data()));
    }

    //-	push_back(Dhelper c) l�gger till ett tecken sist.
    {
        Vector<Dhelper> vecBar("Bar");
        const Vector<Dhelper> vecBarC("Bar");

        vecBar.push_back('a');
        assert(vecBar == "Bara");
    }


    // resize
    {
        Vector<Dhelper> vecBar("Bar");
        const Vector<Dhelper> vecBarC("Bar");
        //size up
        vecBar.shrink_to_fit();
        auto buf = vecBar.data();
        vecBar.resize(6);
        assert(vecBar[2] == 'r' && vecBar[3] == (CHAR)0 && vecBar[4] == (CHAR)0 && vecBar[5] == (CHAR)0);
        assert(buf != vecBar.data() && vecBar.capacity() >= 6 && vecBar.size() == 6);
        buf = vecBar.data();
        vecBar.resize(5);
        assert(buf == vecBar.data() && vecBar.capacity() >= 6 && vecBar.size() == 5);
        assert(vecBar[5].FLAG == DD);  //Is last item deconstructed?
    }
    {//minitest push_back &&
        Vector<Dhelper> vecBar("Bar");
        vecBar.push_back('a');
        assert(vecBar == "Bara");
    }

    //-	operator== 
    //testas �verallt!

    {//Swap
        Vector<Dhelper> Bar("Bar");
        Vector<Dhelper> Foo("Foo");
        swap(Bar, Foo);
        assert(Foo == "Bar" && Bar == "Foo");
        //Test for efficient swap!
        auto fptr = Foo.data();
        auto bptr = Bar.data();
        swap(Foo, Bar);
        assert(fptr == Bar.data() && bptr == Foo.data());
    }

    TestRolOp();
    TestCapacity();
    TestPushBack();

#ifdef VG_BETYG
    TestVGAssignment();
#endif
}

void TestPushBackReallocation() {
    Vector<Dhelper> vec("hej");
    assert(vec.size() <= vec.capacity());
    assert(vec.size() == 3);
    assert(vec == "hej");
    //we try to take 20 empty places: (size+1 < capacity)
    while (vec.size() + 20 >= vec.capacity() && vec.size() < 1000)
        vec.push_back('A' + rand() % 32);
    assert(vec.size() < 1000);	//If this fail it prbably the case that capacity is increased with a constant.

    auto internalBuf = &vec[0];
    auto cap = vec.capacity();
    auto siz = vec.size();
    size_t i;
    for (i = siz + 1; i <= cap; ++i) {
        vec.push_back((int)i + 'a');
        assert(internalBuf == &vec[0]);
        assert(cap == vec.capacity());
        assert(i == vec.size());
    }
    vec.push_back((int)i);
    assert(internalBuf != &vec[0]);
    assert(cap < vec.capacity());
    assert(i == vec.size());
}


template<class C>
void TestRel(const C& lhs, const C& rhs,
    bool(*trueFun)(const C& lhs, const C& rhs),
    bool(*falseFun)(const C& lhs, const C& rhs)) {
    assert(trueFun(lhs, rhs));
    assert(falseFun(lhs, rhs));
}

#define RelTest(a, b, c, d)  a c b && !(a d b)

void TestRolOp() {
    Vector<Dhelper> vecAbcdef("Abcdef");
    Vector<Dhelper> vecBbcdef("Bbcdef");
    Vector<Dhelper> vecAbcdeF("AbcdeF");
    Vector<Dhelper> vecAbcdefg("Abcdefg");

    { bool f = RelTest(vecAbcdef, vecAbcdef, == , != ); assert(f); }
    { bool f = RelTest(vecAbcdef, vecAbcdeF, != , == ); assert(f); }
    { bool f = RelTest(vecAbcdef, vecBbcdef, != , == ); assert(f); }
    { bool f = RelTest(vecAbcdef, vecAbcdefg, != , == ); assert(f); }
    { bool f = RelTest(vecAbcdefg, vecAbcdef, != , == ); assert(f); }

    { bool f = RelTest(vecAbcdef, vecAbcdef, == , < ); assert(f); }
    { bool f = RelTest(vecAbcdef, vecAbcdeF, > , < ); assert(f); }
    { bool f = RelTest(vecAbcdef, vecBbcdef, <, >); assert(f); }
    { bool f = RelTest(vecAbcdef, vecAbcdefg, <, >); assert(f); }

    { bool f = RelTest(vecAbcdef, vecAbcdef, <= , != ); assert(f); }
    { bool f = RelTest(vecAbcdef, vecAbcdeF, >= , <= ); assert(f); }
    { bool f = RelTest(vecAbcdef, vecBbcdef, <= , >= ); assert(f); }
    { bool f = RelTest(vecAbcdef, vecAbcdefg, <= , >= ); assert(f); }

    Vector<Dhelper> vecA("A");
    Vector<Dhelper> vecAxxx("Axxx");
    Vector<Dhelper> vecB("B");

    { bool f = RelTest(vecA, vecAxxx, < , >= ); assert(f); }
    { bool f = RelTest(vecAxxx, vecA, > , <= ); assert(f); }
    { bool f = RelTest(vecB, vecAxxx, > , <= ); assert(f); }
    { bool f = RelTest(vecAxxx, vecB, < , >= ); assert(f); }

}

void TestCapacity() {
    Vector<Dhelper> vecBar("Bar");

    //-	size(), capacity() and reloccation test;
    TestPushBackReallocation();


    //	reserve()
    auto internalBuf = &vecBar[0];
    auto cap = vecBar.capacity();
    auto siz = vecBar.size();

    vecBar.reserve(cap);
    assert(internalBuf == &vecBar[0]);
    assert(cap == vecBar.capacity());
    assert(siz == vecBar.size());

    vecBar.reserve(cap + 1);
    assert(internalBuf != &vecBar[0]);
    assert(cap < vecBar.capacity());
    assert(siz == vecBar.size());

    {
        auto internalBuf = &vecBar[0];
        auto cap = vecBar.capacity();
        auto siz = vecBar.size();

        vecBar.reserve(cap - 1);
        assert(internalBuf == &vecBar[0]);
        assert(cap == vecBar.capacity());
        assert(siz == vecBar.size());
    }

    // shrink_to_fit
    vecBar = "hej";
    vecBar.reserve(10);
    internalBuf = &vecBar[0];
    cap = vecBar.capacity();
    siz = vecBar.size();

    vecBar.shrink_to_fit();
    assert(internalBuf != &vecBar[0]);
    assert(vecBar.capacity() == vecBar.size());
    assert(vecBar == "hej");

    cout << "\nVectorTest f�r "
#ifdef VG_BETYG
        "V�l "
#endif
        "Godk�nt klar\n";
}

//Test of typedefs!
using std::is_same;
static_assert(is_same<int, Vector<int>::value_type>::value, "typdef missing");
static_assert(is_same<size_t, Vector<int>::size_type>::value, "typdef missing");
static_assert(is_same<ptrdiff_t, Vector<int>::difference_type>::value, "typdef missing");
static_assert(is_same<int&, Vector<int>::reference>::value, "typdef missing");
static_assert(is_same<const int&, Vector<int>::const_reference>::value, "typdef missing");
static_assert(is_same<int*, Vector<int>::pointer>::value, "typdef missing");
static_assert(is_same<const int*, Vector<int>::const_pointer>::value, "typdef missing");
static_assert(is_same<Vector<int>::iterator, Vector<int>::iterator>::value, "typdef missing");
static_assert(is_same<Vector<int>::const_iterator, Vector<int>::const_iterator>::value, "typdef missing");
static_assert(is_same<Vector<int>::reverse_iterator, Vector<int>::reverse_iterator>::value, "typdef missing");
static_assert(is_same<Vector<int>::const_reverse_iterator, Vector<int>::const_reverse_iterator>::value, "typdef missing");

#else
void TestVector() {}
#endif

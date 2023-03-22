//Exempel på const och reverse iterators i en klass

//CT=char for iterator, const char for const_iterator
//DIR = +1 for iterator, -1 for reverse_iterator
template<class CT, int DIR>
class StrIter {
    CT* _ptr;
public:
    using value_type = char;
    StrIter(CT* ptr = nullptr) :_ptr(ptr) {}
    ~StrIter() = default;
    StrIter(const StrIter&) = default;

    StrIter& operator=(const StrIter&) = default;
    StrIter& operator++() {
        _ptr += DIR;
        return *this;
    }
    CT& operator*() {
        return *_ptr;
    }
    CT* operator->() {
        return _ptr;
    }
    friend bool operator==(const StrIter& lhs, const StrIter& rhs) {
        return lhs._ptr == rhs._ptr;
    }
    friend bool operator!=(const StrIter& lhs, const StrIter& rhs) {
        return lhs._ptr != rhs._ptr;
    }
    friend bool operator<(const StrIter& lhs, const StrIter& rhs) {
        return lhs._ptr < rhs._ptr;
    }
    //etc.

};

//T* first1 = lhs.m_arrayPtr;
//T* last1 = lhs.m_arrayPtr + lhs.m_size;
//
//T* first2 = rhs.m_arrayPtr;
//T* last2 = rhs.m_arrayPtr + rhs.m_size;
//
//for (; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2)
//{
//	if (*first1 < *first2)
//	{
//		return false;
//	}
//	if (*first2 < *first1)
//	{
//		return false;
//	}
//	else
//	{	//Different length
//		if (*first1 + 1 != 0 &&
//			*first2 + 1 != 0 &&
//			lhs.m_size > rhs.m_size)
//		{
//			return false;
//		}
//		else if (lhs.m_size < rhs.m_size)
//		{
//			return false;
//		}
//		//Same length
//		return true;
//	}
//}
//
//if (last1 == last2 &&
//	lhs.m_size == 0 &&
//	rhs.m_size == 0)
//{
//	return true;
//}
//

//int count = 0; //replace count 
//Vector<T> greater;
//Vector<T> smaller;

//if (lhs.size() >= other.size()) {
//	greater = lhs;
//	smaller = other;
//}

//else {
//	greater = other;
//	smaller = lhs;
//}

//for (T* ptr = smaller.m_arrayPtr; ptr < smaller.m_arrayPtr + smaller.m_size; ++ptr)
//{

//	if (*ptr != *(greater.m_arrayPtr + count))
//	{
//		return false;
//	}

//	++count;
//}

//return true;

//if (lhs.m_size == rhs.size)
//{
//
//}
//else
//{
//	size_t size = lhs.m_size >= rhs.m_size ? lhs.m_size : rhs.m_size;
//
//	for (T* lhsPtr = lhs.m_arrayPtr, rhsPtr = rhs.m_arrayPtr; lhsPtr < size && rhsPtr < size; lhsPtr++, rhsPtr++)
//	{
//		//
//		//00001
//		//  ^
//		//11
//		//  ^
//
//		if (lhsPtr == NULL)
//		{
//			for (T* ptr = lhsPtr; ptr < size; ptr++)
//			{
//				if (ptr != NULL)
//				{
//					return false;
//				}
//			}
//			return true;
//
//		}
//		else if (rhsPtr == NULL)
//		{
//			for (T* ptr = rhsPtr; ptr < size; ptr++)
//			{
//				if (ptr != NULL)
//				{
//					return false;
//				}
//			}
//			return true;
//		}
//
//	}
//}


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
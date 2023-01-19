#pragma once
#include <iostream>
#include <compare>

template <class T>
class Vector
{
private:
	static const int _initCapacity = 4;
	T* _arrayPtr;

	size_t _size;
	size_t _capacity;

public:

	////Constructors & Destrctor
	Vector() noexcept
	{
		T tArray[_initCapacity];
		_arrayPtr = tArray;

		_size = 0;
		_capacity = _initCapacity;
		invariant();
	};

	Vector(const char* other)
	{
		_size = 0;
		while (other[_size] != '\0')
			_size++;

		_capacity = _size;

		_arrayPtr = new T[_capacity];

		for (size_t i = 0; i < _size; i++)
		{
			_arrayPtr[i] = other[i];
		}

		invariant();
	};

	Vector(const Vector& other)
	{
		this->_size = other.size();
		this->_capacity = other.capacity();

		_arrayPtr = new T[other.size()];

		for (size_t i = 0; i < other.size(); i++)
		{
			*(_arrayPtr + i) = *(other._arrayPtr + i);
		}
	};

	//Vector(Vector&& other) noexcept;
	~Vector() noexcept
	{
		invariant();
	};

	////Methods
	size_t size() const noexcept
	{
		return _size;
	};

	size_t capacity() const noexcept
	{
		return _capacity;
	}

	const T& at(size_t i) const
	{
		if (i <= _capacity)
			return *(_arrayPtr + i);
		else
			throw std::out_of_range("Index is out of Range");
	};



	bool invariant() const
	{
		//if (!(_capacity >= _size))
		//	return false;
		//if (!arrayPtr)
		//	return false;
		//else
		//	return true;

		return
			_capacity >= _size &&
			_arrayPtr;

	};

#define CHECK assert((invariant()))


	//void reserve(size_t n);
	//void shrink_to_fit();
	//void resize(size_t n);
	//void push_back(const T& c);

	//[const] T& at(size_t i)[const];
	//[const] T* data()[const] noexcept;

	////Operator overloading
	//Vector& operator=(const Vector& other);
	//Vector& operator=(Vector&& other) noexcept;
	//
	//[const] T& operator[](size_t i)[const];


	friend int operator<=>(const Vector& lhs, const Vector& rhs)
	{
		if(lhs.size() > rhs.size())
			return 1;
		else if (lhs.size() < rhs.size())
			return -1;
		else
			return 0;
	};


	friend bool operator==(const Vector& lhs, const Vector& rhs)
	{
		int count = 0;

		if (lhs._size != rhs._size)
			return false;

		else
			for (T* ptr = lhs._arrayPtr; ptr < lhs._arrayPtr + lhs._size; ++ptr)
			{
				if (*ptr != *(rhs._arrayPtr + count))
				{
					return false;
				}
				++count;
			}

		return true;
	};

	friend std::ostream& operator<<(std::ostream& cout, const Vector& other)
	{
		for (size_t i = 0; i < other.size(); ++i)
			cout << other[i];
		return cout;
	};

	const T& operator[](size_t i) const
	{
		return *(_arrayPtr + i);
	};

	T& operator[](size_t i)
	{
		return *(_arrayPtr + i);
	};

	//VS-studio
	//auto operator<=>(const Vector& a) const default;

	////Iterators
	//iterator begin() noexcept;
	//iterator end() noexcept;
	//const_iterator begin() const noexcept;
	//const_iterator end() const noexcept;
	//const_iterator cbegin() const noexcept;
	//const_iterator cend() const noexcept;

	//reverse_iterator begin() noexcept;
	//reverse_iterator end() noexcept;
	//const_reverse_iterator begin() const noexcept;
	//const_reverse_iterator end() const noexcept;
	//const_reverse_iterator cbegin() const noexcept;
	//const_reverse_iterator cend() const noexcept;

};


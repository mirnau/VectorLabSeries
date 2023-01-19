#pragma once
#include <iostream>
#include <compare>
#include <utility>

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
		//T tArray[_initCapacity];
		//_arrayPtr = tArray;
		_arrayPtr = new T[_initCapacity];

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

		//Creates a new instance, hence no delete
			_arrayPtr = new T[_capacity + 1];

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

		CopyArray(other);
	};

	Vector(Vector&& other) noexcept :
		_arrayPtr(other._arrayPtr), 
		_size(other._size), 
		_capacity(other._capacity)
	{
		other._arrayPtr = nullptr;
		other._size = 0;
		other._capacity = 0;
		invariant();
	};

	~Vector() noexcept
	{
		invariant();
		delete[] _arrayPtr;
	};

	////Methods
	size_t size() const noexcept
	{
		invariant();
		return _size;
	};

	size_t capacity() const noexcept
	{
		invariant();
		return _capacity;
	};

	const T& at(size_t i) const
	{
		invariant();
		if (i <= _capacity)
			return *(_arrayPtr + i);
		else
			throw std::out_of_range("Index is out of Range");
	};

	void push_back(const T& c)
	{
		if (_size + 1 > _capacity)
			reserve(_capacity * 2);

		*(_arrayPtr + _size) = c;
		++_size;
		invariant();
	};

	void CopyArray(const Vector& other)
	{
		for (size_t i = 0; i < other.size(); i++)
		{
			*(_arrayPtr + i) = *(other._arrayPtr + i);
		}
		invariant();
	};

	void reserve(size_t n)
	{
		if (n > _capacity)
		{
			auto a = new T[n + 1];

			for (size_t i = 0; i < _capacity; i++)
			{
				*(a + i) = *(_arrayPtr + i);
			}

			_capacity = n;
			delete[] a;
		}
		invariant();
	};

	void resize(size_t n)
	{
		if (n == _size)
		{
			return;
		}

		int difference = n - _size;

		if (difference > 0)
		{
			if (_capacity < n)
			{
				reserve(difference);

				for (size_t i = _size; i < n; i++)
				{
					*(_arrayPtr + i) = *new T();
				}
				_size = n;
				_capacity = n;
			}
			else
			{
				_size = n;
				shrink_to_fit();
			}
		}
	}

	void shrink_to_fit()
	{
		T* temp = new T[_size + 1]{};

		for (size_t i = 0; i < _size; ++i)
		{
			*(temp + i) = *(_arrayPtr + i);
		}

		delete[] _arrayPtr;
		_arrayPtr = temp;
		temp = nullptr;

		_capacity = _size;
		invariant();
	}

	bool invariant() const
	{
		//if (!(_capacity >= _size))
		//	return false;
		//if (!arrayPtr)
		//	return false;
		//else
		//	return true;

		return
			_capacity >= _size;

	};

#define CHECK assert((invariant()))


	//void reserve(size_t n);
	//void shrink_to_fit();
	//void resize(size_t n);

	friend int operator<=>(const Vector& lhs, const Vector& rhs)
	{
		if (lhs.size() > rhs.size())
			return 1;
		else if (lhs.size() < rhs.size())
			return -1;
		else
			return 0;
	};

	Vector& operator=(const Vector& other)
	{
		if (this == &other)
			return *this;
		
		if (other.size() > _capacity)
		{
			reserve(other.size());
			_size = _capacity;
		}

		for (size_t i = 0; i < other.size(); i++)
		{
			*(_arrayPtr + i) = *(other._arrayPtr + i);
		}

		invariant();
		return *this;
	};

	Vector& operator=(Vector&& other) noexcept
	{
		if (*this == other)
			return *this;
		if (other.size() > _capacity)
		{ 
			reserve(other.size());
			_size = _capacity;
		}

		for (size_t i = 0; i < other.size(); i++)
		{
			*(_arrayPtr + i) = *(other._arrayPtr + i);
		}

		delete[] other._arrayPtr;
		other._arrayPtr = nullptr;
		other._size = 0;
		other._capacity = 0;

		invariant();
		return *this;
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
		invariant();
		return cout;
	};

	const T& operator[](size_t i) const
	{
		invariant();
		return *(_arrayPtr + i);
	};

	T& operator[](size_t i)
	{
		invariant();
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


#pragma once
#include <iostream>
#include <compare>
#include <utility>

#include "VectorIterator.hpp"


template <class T>
class Vector
{
private:
	static const int _initCapacity = 4;
	T* m_arrayPtr;

	size_t m_size;
	size_t m_capacity;

public:
	using iterator = VectorIterator<Vector<T>, Vector<T>, 1>;
	using const_iterator = VectorIterator<Vector<T>, const Vector<T>, 1>;
	using reverse_iterator = VectorIterator<Vector<T>, Vector<T>, -1>;
	using const_reverse_iterator = VectorIterator<Vector<T>, const Vector< T>, -1>;
	using value_type = T;

	////Constructors & Destrctor
	Vector() noexcept
	{
		//T tArray[_initCapacity];
		//_arrayPtr = tArray;
		m_arrayPtr = new T[_initCapacity];

		m_size = 0;
		m_capacity = _initCapacity;
		invariant();
	};

	Vector(const char* other)
	{
		m_size = 0;
		while (other[m_size] != '\0')
			m_size++;

		m_capacity = m_size;

		//Creates a new instance, hence no delete
		m_arrayPtr = new T[m_capacity + 1];

		for (size_t i = 0; i < m_size; i++)
		{
			m_arrayPtr[i] = other[i];
		}

		invariant();
	};

	Vector(const Vector& other)
	{
		this->m_size = other.size();
		this->m_capacity = other.capacity();

		m_arrayPtr = new T[other.size()];

		CopyArray(other);
	};

	Vector(Vector&& other) noexcept :
		m_arrayPtr(other.m_arrayPtr),
		m_size(other.m_size),
		m_capacity(other.m_capacity)
	{
		other.m_arrayPtr = nullptr;
		other.m_size = 0;
		other.m_capacity = 0;
		invariant();
	};

	~Vector() noexcept
	{
		invariant();
		delete[] m_arrayPtr;
	};

	////Methods
	size_t size() const noexcept
	{
		invariant();
		return m_size;
	};

	size_t capacity() const noexcept
	{
		invariant();
		return m_capacity;
	};

	T& at(size_t i) 
	{
		invariant();
		if (i <= m_capacity)
			return *(m_arrayPtr + i);
		else
			throw std::out_of_range("Index is out of Range");
	};

	T& at(size_t i) const
	{
		invariant();
		if (i <= m_capacity)
			return *(m_arrayPtr + i);
		else
			throw std::out_of_range("Index is out of Range");
	};

	void push_back(const T& c)
	{
		if (m_size + 1 > m_capacity)
			reserve(m_capacity * 2);

		*(m_arrayPtr + m_size) = c;
		++m_size;
		invariant();
	};

	void CopyArray(const Vector& other)
	{
		for (size_t i = 0; i < other.size(); i++)
		{
			*(m_arrayPtr + i) = *(other.m_arrayPtr + i);
		}
		invariant();
	};

	void reserve(size_t n)
	{
		if (n > m_capacity)
		{
			T* a = new T[n + 1];

			for (size_t i = 0; i < m_capacity; i++)
			{
				*(a + i) = *(m_arrayPtr + i);
			}

			delete[] m_arrayPtr;
			m_arrayPtr = a;
			m_capacity = n;

			a = nullptr;
		}

		invariant();
	};

	friend void swap(Vector<T>& lhs, Vector<T>& rhs)
	{
		Vector<T> temp(std::move(lhs));
		lhs = std::move(rhs);
		rhs = std::move(temp);

	};

	void resize(size_t n)
	{
		if (n == m_size)
		{
			return;
		}

		int difference = n - m_size;

		if (difference > 0)
		{
			if (m_capacity < n)
			{
				reserve(n);

				for (size_t i = m_size; i < n; i++)
				{
					*(m_arrayPtr + i) = T();
				}

				m_size = n;
				m_capacity = n;
			}
			else
			{
				m_size = n;
				shrink_to_fit();
			}
		}
	}

	void shrink_to_fit()
	{
		T* temp = new T[m_size + 1]{};

		for (size_t i = 0; i < m_size; ++i)
		{
			*(temp + i) = *(m_arrayPtr + i);
		}

		delete[] m_arrayPtr;
		m_arrayPtr = temp;
		temp = nullptr;

		m_capacity = m_size;
		invariant();
	}

	const T* data() const noexcept
	{
		return m_arrayPtr;
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
			m_capacity >= m_size;

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

		if (other.size() > m_capacity) {
			m_size = other.size();
			reserve(m_size);
		}

		for (size_t i = 0; i < other.size(); i++)
			*(m_arrayPtr + i) = *(other.m_arrayPtr + i);

		m_size = other.m_size;

		invariant();
		return *this;
	};

	Vector& operator=(Vector&& other) noexcept
	{
		if (*this == other)
			return *this;
		if (other.size() > m_capacity)
		{
			reserve(other.size());
			m_size = m_capacity;
		}

		for (size_t i = 0; i < other.size(); i++)
		{
			*(m_arrayPtr + i) = *(other.m_arrayPtr + i);
		}

		delete[] other.m_arrayPtr;
		other.m_arrayPtr = nullptr;
		other.m_size = 0;
		other.m_capacity = 0;

		invariant();
		return *this;
	};


	friend bool operator==(const Vector& lhs, const Vector& other)
	{
		int count = 0; //replace count 
		Vector<T> greater;
		Vector<T> smaller;

		if (lhs.size() >= other.size()) {
			greater = lhs;
			smaller = other;
		}

		else {
			greater = other;
			smaller = lhs;
		}


		for (T* ptr = smaller.m_arrayPtr; ptr < smaller.m_arrayPtr + smaller.m_size; ++ptr)
		{

			if (*ptr != *(greater.m_arrayPtr + count))
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
		invariant();
		return *(m_arrayPtr + i);
	};

	T& operator[](size_t i)
	{
		invariant();
		return *(m_arrayPtr + i);
	};

	//Iterators

	iterator begin() noexcept
	{
		return iterator(m_arrayPtr);
	};

	iterator end() noexcept
	{
		return iterator(m_arrayPtr + m_size);
	};

	const_iterator cbegin() const noexcept
	{
		return const_iterator(begin());
	};

	const_iterator cend() const noexcept
	{
		return const_iterator(end());
	};

	//REVERSE ITERATOR Begin

	reverse_iterator rbegin() noexcept
	{
		return reverse_iterator(m_arrayPtr + m_size);
	};

	reverse_iterator rend() noexcept
	{
		return reverse_iterator(m_arrayPtr);
	};

	const_reverse_iterator crbegin() const noexcept
	{
		return const_reverse_iterator(rbegin());
	};
	
	const_reverse_iterator crend() const noexcept
	{
		return const_reverse_iterator(rend());
	};
};
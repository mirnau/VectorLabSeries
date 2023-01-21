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
	using iterator = VectorIterator<Vector<T>, const Vector<T>, 1>;
	using const_iterator = VectorIterator<Vector<T>, const Vector<T>, 1>;
	using reverse_iterator = VectorIterator<Vector<T>, Vector<const T>, -1>;
	using reverse_const_iterator = VectorIterator<Vector<T>, Vector<const T>, -1>;
	using value_type = T;

	////Constructors & Destrctor
	Vector() noexcept
	{
		//T tArray[_initCapacity];
		//_arrayPtr = tArray;
		m_arrayPtr = new T[_initCapacity];

		m_size = 0;
		m_capacity = _initCapacity;
		Invariant();
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

		Invariant();
	};

	Vector(const Vector& other)
	{
		this->m_size = other.Size();
		this->m_capacity = other.Capacity();

		m_arrayPtr = new T[other.Size()];

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
		Invariant();
	};

	~Vector() noexcept
	{
		Invariant();
		delete[] m_arrayPtr;
	};

	////Methods
	size_t Size() const noexcept
	{
		Invariant();
		return m_size;
	};

	size_t Capacity() const noexcept
	{
		Invariant();
		return m_capacity;
	};

	const T& at(size_t i) const
	{
		Invariant();
		if (i <= m_capacity)
			return *(m_arrayPtr + i);
		else
			throw std::out_of_range("Index is out of Range");
	};

	void push_back(const T& c)
	{
		if (m_size + 1 > m_capacity)
			Reserve(m_capacity * 2);

		*(m_arrayPtr + m_size) = c;
		++m_size;
		Invariant();
	};

	void CopyArray(const Vector& other)
	{
		for (size_t i = 0; i < other.Size(); i++)
		{
			*(m_arrayPtr + i) = *(other.m_arrayPtr + i);
		}
		Invariant();
	};

	void Reserve(size_t n)
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

		Invariant();
	};

	friend void swap(Vector<T>& lhs, Vector<T>& rhs)
	{
		Vector<T> temp(std::move(lhs));
		lhs = std::move(rhs);
		rhs = std::move(temp);

	};

	void Resize(size_t n)
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
				Reserve(n);

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
				ShrinkToFit();
			}
		}
	}

	void ShrinkToFit()
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
		Invariant();
	}

	const T* Data() const noexcept
	{
		return m_arrayPtr;
	}

	bool Invariant() const
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
		if (lhs.Size() > rhs.Size())
			return 1;
		else if (lhs.Size() < rhs.Size())
			return -1;
		else
			return 0;
	};

	Vector& operator=(const Vector& other)
	{
		if (this == &other)
			return *this;

		if (other.Size() > m_capacity) {
			m_size = other.Size();
			Reserve(m_size);
		}

		for (size_t i = 0; i < other.Size(); i++)
			*(m_arrayPtr + i) = *(other.m_arrayPtr + i);

		m_size = other.m_size;

		Invariant();
		return *this;
	};

	Vector& operator=(Vector&& other) noexcept
	{
		if (*this == other)
			return *this;
		if (other.Size() > m_capacity)
		{
			Reserve(other.Size());
			m_size = m_capacity;
		}

		for (size_t i = 0; i < other.Size(); i++)
		{
			*(m_arrayPtr + i) = *(other.m_arrayPtr + i);
		}

		delete[] other.m_arrayPtr;
		other.m_arrayPtr = nullptr;
		other.m_size = 0;
		other.m_capacity = 0;

		Invariant();
		return *this;
	};


	friend bool operator==(const Vector& lhs, const Vector& other)
	{
		int count = 0; //replace count 
		Vector<T> greater;
		Vector<T> smaller;

		if (lhs.Size() >= other.Size()) {
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
		for (size_t i = 0; i < other.Size(); ++i)
			cout << other[i];
		return cout;
	};

	const T& operator[](size_t i) const
	{
		Invariant();
		return *(m_arrayPtr + i);
	};

	T& operator[](size_t i)
	{
		Invariant();
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


	const_iterator begin() const noexcept
	{
		return const_iterator(m_arrayPtr);
	};

	const_iterator end() const noexcept
	{
		return const_iterator(m_arrayPtr + m_size);
	};

	const_iterator cbegin() const noexcept
	{
		return const_iterator(m_arrayPtr);
	};

	const_iterator cend() const noexcept
	{
		return const_iterator(m_arrayPtr + m_size);
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

	//reverse_const_iterator crbegin() const noexcept
	//{
	//	return reverse_const_iterator(m_arrayPtr + m_size);
	//};
	//
	//reverse_const_iterator crend() const noexcept
	//{
	//	return reverse_const_iterator(m_arrayPtr);
	//};
	//
	//reverse_const_iterator crbegin() const noexcept
	//{
	//	return reverse_const_iterator(m_arrayPtr + m_size);
	//};
	//
	//reverse_const_iterator cend() const noexcept
	//{
	//	return reverse_const_iterator(m_arrayPtr);
};
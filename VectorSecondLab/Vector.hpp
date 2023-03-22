#pragma once
#include <iostream>
#include <compare>
#include <utility>
#include "VectorIterator.hpp"
#include <cassert>
#define CHECK assert(invariant())

template <class T>
class Vector
{
public:
	using iterator = VectorIterator<Vector<T>, 1>;
	using const_iterator = VectorIterator<Vector<const T>, 1>;
	using reverse_iterator = VectorIterator<Vector<T>, -1>;
	using const_reverse_iterator = VectorIterator<Vector<const T>, -1>;
	using value_type = T;
	using pointer = T*;
	using reference = T&;

private:
	T* m_arrayPtr;
	size_t m_size;
	size_t m_capacity;

public:
	////Constructors & Destrctor
	Vector() noexcept
	{
		m_arrayPtr = nullptr;

		m_size = 0;
		m_capacity = 0;
		CHECK;
	};

	Vector(const char* other)
	{
		m_size = strlen(other);
		m_capacity = m_size;

		if (m_size == 0)
		{
			++m_capacity;
		}

		m_arrayPtr = new T[m_capacity + 1];

		for (size_t i = 0; i < m_size; ++i)
		{
			m_arrayPtr[i] = other[i];
		}

		CHECK;
	};

	Vector(const Vector& other)
	{
		this->m_size = other.size();
		this->m_capacity = other.capacity();

		if (other.m_arrayPtr == nullptr)
		{
			m_arrayPtr = nullptr;
		}
		else
		{
			m_arrayPtr = new T[other.size()];

			for (size_t i = 0; i < other.size(); i++)
			{
				m_arrayPtr[i] = *(other.m_arrayPtr + i);
			}
		}

		CHECK;
	};

	Vector(Vector&& other) noexcept :
		m_arrayPtr(other.m_arrayPtr),
		m_size(other.m_size),
		m_capacity(other.m_capacity)
	{
		other.m_arrayPtr = nullptr;
		other.m_size = 0;
		other.m_capacity = 0;
		CHECK;
	};

	~Vector() noexcept
	{
		CHECK;
		delete[] m_arrayPtr;
	};

	////Methods
	size_t size() const noexcept
	{
		return m_size;
	};

	size_t capacity() const noexcept
	{
		return m_capacity;
	};

	T& at(size_t i)
	{
		if (i < m_capacity)
			return m_arrayPtr[i];
		else
			throw std::out_of_range("Index is out of Range");
	};

	const T& at(size_t i) const
	{
		if (i < m_capacity)
			return m_arrayPtr[i];
		else
			throw std::out_of_range("Index is out of Range");
	};

	void push_back(const T& c)
	{
		if (m_arrayPtr == nullptr)
		{
			m_size = 0;
			m_capacity = 1;
			m_arrayPtr = new T[m_capacity];
		}

		else if (m_size + 1 > m_capacity)
		{
			reserve(2 * (m_capacity == 0 ? 1 : m_capacity));
		}

		*(m_arrayPtr + m_size) = c;
		++m_size;

		CHECK;
	};

	void reserve(size_t n)
	{
		if (n > m_capacity)
		{
			T* a = new T[n + 1];

			for (size_t i = 0; i < m_capacity; i++)
			{
				*(a + i) = m_arrayPtr[i];
			}

			delete[] m_arrayPtr;
			m_arrayPtr = a;
			m_capacity = n;

			a = nullptr;
		}

		CHECK;
	};

	friend void swap(Vector<T>& lhs, Vector<T>& rhs)
	{
		Vector<T> temp = std::move(lhs);
		lhs = std::move(rhs);
		rhs = std::move(temp);
	};

	void resize(size_t n)
	{
		if (n == m_size)
		{
			return;
		}

		if (n > 0)
		{
			if (m_capacity < n)
			{
				reserve(n);

				for (size_t i = m_size; i < n; i++)
				{
					m_arrayPtr[i] = T();
				}
			}

			m_size = n;
		}
	}

	void shrink_to_fit()
	{
		T* temp = new T[m_size + 1];

		for (size_t i = 0; i < m_size; ++i)
		{
			*(temp + i) = m_arrayPtr[i];
		}

		delete[] m_arrayPtr;
		m_arrayPtr = temp;
		temp = nullptr;

		m_capacity = m_size;
		CHECK;
	}

	T* data() noexcept
	{
		return m_arrayPtr;
	}

	const T* data() const noexcept
	{
		return m_arrayPtr;
	}

	bool invariant() const
	{
		if (m_arrayPtr != nullptr)
		{
			if (m_capacity <= 0)
			{
				return false;
			}
		}

		return
			m_capacity >= m_size;

	}

	friend int operator<=>(const Vector& lhs, const Vector& rhs)
	{
		T* first1 = lhs.m_arrayPtr;
		T* last1 = lhs.m_arrayPtr + lhs.m_size;

		T* first2 = rhs.m_arrayPtr;
		T* last2 = rhs.m_arrayPtr + rhs.m_size;

		for (; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2)
		{
			if (*first1 < *first2)
			{
				return -1;
			}
			if (*first2 < *first1)
			{
				return 1;
			}
			else
			{	//Different length
				if (lhs.m_size > rhs.m_size)
				{
					return 1;
				}
				else if (lhs.m_size < rhs.m_size)
				{
					return -1;
				}
				//Same length
			}
		}

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
		else
		{
			m_size = other.size();
		}

		for (size_t i = 0; i < m_size; i++)
			m_arrayPtr[i] = other.m_arrayPtr[i];

		CHECK;
		return *this;
	};

	Vector& operator=(Vector&& other) noexcept
	{
		delete[] m_arrayPtr; //Fråga: Varken delete eller delete[] ger minnläcka, vilken är rätt här?
		
		m_arrayPtr = other.m_arrayPtr;
		m_capacity = other.m_capacity;
		m_size = other.m_size;
		
		other.m_arrayPtr = nullptr;
		other.m_size = 0;
		other.m_capacity = 0;

		CHECK;
		return *this;
	};

	friend bool operator==(const Vector& lhs, const Vector& rhs)
	{
		if (lhs.size() != rhs.size())
		{
			return false;
		}

		T* first1 = lhs.m_arrayPtr;
		T* last1 = lhs.m_arrayPtr + lhs.m_size;

		T* first2 = rhs.m_arrayPtr;
		T* last2 = rhs.m_arrayPtr + rhs.m_size;

		for (; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2)
		{
			if (*first1 != *first2)
			{
				return false;
			}
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
		return m_arrayPtr[i];
	};

	T& operator[](size_t i)
	{
		invariant();
		return m_arrayPtr[i];
	};

	//Iterators

	iterator begin() noexcept
	{
		return iterator(m_arrayPtr);
	};

	const_iterator begin()const noexcept
	{
		return const_iterator(m_arrayPtr);
	};

	iterator end() noexcept
	{
		return iterator(m_arrayPtr + m_size);
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
		return reverse_iterator(m_arrayPtr + m_size -1);
	};

	const_reverse_iterator rbegin() const noexcept
	{
		return const_reverse_iterator(m_arrayPtr + m_size -1);
	};

	reverse_iterator rend() noexcept
	{
		return reverse_iterator(m_arrayPtr -1);
	};

	const_reverse_iterator rend() const noexcept
	{
		return const_reverse_iterator(m_arrayPtr -1);
	};

	const_reverse_iterator crbegin() const noexcept
	{
		return const_reverse_iterator(m_arrayPtr + m_size -1);
	};

	const_reverse_iterator crend() const noexcept
	{
		return const_reverse_iterator(m_arrayPtr -1);
	};
};

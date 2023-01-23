#pragma once
#include <iostream>
#include <compare>
#include <utility>
#include "VectorIterator.hpp"

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
		invariant();
	};

	Vector(const char* other)
	{
		m_size = strlen(other);

		m_capacity = m_size;

		m_arrayPtr = new T[m_capacity + 1];

		for (size_t i = 0; i < m_size; ++i)
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
		int saftey = 0;

		if (m_arrayPtr == nullptr)
		{
			saftey = 1;
			m_arrayPtr = new T[m_size + 1];
		}

		if (m_size + 1 > m_capacity)
		{
			reserve(saftey + m_capacity * 2);
		}

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
		std::swap(lhs.m_arrayPtr, rhs.m_arrayPtr);
		/*Vector<T> temp = std::move(lhs.m_arrayPtr);
		lhs = std::move(rhs.m_arrayPtr);
		rhs = std::move(temp.m_arrayPtr);*/
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
					*(m_arrayPtr + i) = T();
				}
			}

			m_size = n;
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

	//friend int operator<=>(const Vector& lhs, const Vector& rhs)
	//{
	//	if (lhs.m_size > rhs.m_size)
	//		return 1;
	//	else if (lhs.m_size < rhs.m_size)
	//		return -1;
	//	else
	//		return 0;
	//};

	friend int operator<=>(const Vector& lhs, const Vector& rhs)
	{
		T* first1 = lhs.m_arrayPtr;
		T* last1 = lhs.m_arrayPtr + lhs.m_size;

		T* first2 = rhs.m_arrayPtr;
		T* last2 = rhs.m_arrayPtr + rhs.m_size;
		
		//std::cout << std::strcmp(rhs.m_arrayPtr, lhs.m_arrayPtr) << std::end;

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
				return strcmp(lhs.m_arrayPtr, rhs.m_arrayPtr);
			}
		}
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
			m_arrayPtr[i] = other.m_arrayPtr[i];


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
		else
		{
			m_size = other.m_size;
			shrink_to_fit();
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


	friend bool operator==(const Vector& lhs, const Vector& rhs)
	{
		T* first1 = lhs.m_arrayPtr;
		T* last1 = lhs.m_arrayPtr + lhs.m_size;

		T* first2 = rhs.m_arrayPtr;
		T* last2 = rhs.m_arrayPtr + rhs.m_size;

		for (; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2)
		{
			if (*first1 < *first2)
			{
				return false;
			}
			if (*first2 < *first1)
			{
				return false;
			}
			else
			{	//Different length
				if (*first1 + 1 != 0 &&
					*first2 + 1 != 0 &&
					lhs.m_size > rhs.m_size)
				{
					return false;
				}
				else if (lhs.m_size < rhs.m_size)
				{
					return false;
				}
			}
		}

		return true;

		//if (lhs.m_size == 0 &&
		//	rhs.m_size == 0)
		//{
		//	return true;
		//}
		//else
		//{
		//	return false;
		//}
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
		return reverse_iterator(m_arrayPtr + m_size);
	};

	const_reverse_iterator rbegin() const noexcept
	{
		return const_reverse_iterator(m_arrayPtr + m_size);
	};

	reverse_iterator rend() noexcept
	{
		return reverse_iterator(m_arrayPtr);
	};

	const_reverse_iterator rend() const noexcept
	{
		return const_reverse_iterator(m_arrayPtr);
	};

	const_reverse_iterator crbegin() const noexcept
	{
		return const_reverse_iterator(m_arrayPtr + m_size);
	};

	const_reverse_iterator crend() const noexcept
	{
		return const_reverse_iterator(m_arrayPtr);
	};
};

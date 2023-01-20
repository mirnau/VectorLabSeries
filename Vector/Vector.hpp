#pragma once
#include <iostream>
#include <compare>
#include <utility>

template <class T>
class Vector
{
private:
	static const int _initCapacity = 4;
	T* m_arrayPtr;

	size_t m_size;
	size_t m_capacity;

public:

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

	const T& at(size_t i) const
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
		
		if (other.size() > m_capacity)
		{
			reserve(other.size());
			m_size = m_capacity;
		}

		for (size_t i = 0; i < other.size(); i++)
		{
			*(m_arrayPtr + i) = *(other.m_arrayPtr + i);
		}

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


	friend bool operator==(const Vector& lhs, const Vector& rhs)
	{
		int count = 0;
		if (lhs.m_size != rhs.m_size)
			return false;

		else
			for (T* ptr = lhs.m_arrayPtr; ptr < lhs.m_arrayPtr + lhs.m_size; ++ptr)
			{
				if (*ptr != *(rhs.m_arrayPtr + count))
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
		return *(m_arrayPtr + i);
	};

	T& operator[](size_t i)
	{
		invariant();
		return *(m_arrayPtr + i);
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


#pragma once
#include <iostream>
#include <compare>
#include <utility>
#include "VectorIterator.hpp"
#include <cassert>
#include <exception>

#include "Dalloc.hpp"

#define CHECK assert(invariant())

template <class T>
class Vector
{
public:
	using iterator = VectorIterator<T, 1>;
	using const_iterator = VectorIterator<const T, 1>;
	using reverse_iterator = VectorIterator<T, -1>;
	using const_reverse_iterator = VectorIterator<const T, -1>;

	using value_type = T;
	using pointer = T*;
	using reference = T&;
	using size_type = size_t;
	using difference_type = std::ptrdiff_t;
	using const_reference = const T&;
	using const_pointer = const T*;

#pragma region struktorer
public:

	Vector() noexcept
		:
		m_ptr(nullptr),
		m_size(0),
		m_capacity(0)
	{
		CHECK;
	}

	Vector(const char* other) :
		Vector(std::strlen(other), other, other + std::strlen(other)) {}

	Vector(const Vector& other)
		: Vector(other.size(), other.begin(), other.end())
	{
		CHECK;
	}

	Vector(Vector&& other) noexcept
		:
		m_ptr(other.m_ptr),
		m_size(other.m_size),
		m_capacity(other.m_capacity)
	{
		other.m_ptr = nullptr;
		other.m_size = 0;
		other.m_capacity = 0;
		CHECK;
	}

	template<typename I>
	Vector(size_t capacity, const I& begin, const I& end)
		:
		m_size(0),
		m_capacity(capacity),
		m_ptr(nullptr)
	{
		try
		{
			if (m_capacity == 0)
				return;

			if (begin == nullptr || end == nullptr)
				throw std::exception("");

			m_ptr = Allocate();

			for (I i = begin; i != end; ++i)
			{
				push_back(*i);
			}

			//Tycker det är snyggare att aldrig göra en alloc
			//if (!invariant())
			//	throw std::exception("");
		}
		catch (...)
		{
			Deallocate();

			m_capacity = 0;
			m_size = 0;
			m_ptr = nullptr;
			throw;
		}
	}

	~Vector() noexcept
	{
		Deallocate();
	}

#pragma endregion struktorer

private:

	T* Allocate()
	{
		return m_allocator.allocate(m_capacity);
	}

	void Deallocate()
	{
		for (T* i = m_ptr; i < m_ptr + m_size; i++)
		{
			m_ptr->~T();
		}

		m_allocator.deallocate(m_ptr, m_capacity);
	}

public:

	// Methods
	size_type size() const noexcept
	{
		return m_size;
	}

	size_type capacity() const noexcept
	{
		return m_capacity;
	}

	reference at(size_type i)
	{
		return (i < m_capacity) ? m_ptr[i] : throw std::out_of_range("Index is out of Range");
	}

	const_reference at(size_type i) const
	{
		return (i < m_capacity) ? m_ptr[i] : throw std::out_of_range("Index is out of Range");
	}

	void reserve(size_type new_capacity)
	{
		if (new_capacity > m_capacity)
		{
			T* temp = m_allocator.allocate(new_capacity);

			size_type constructedElements = 0;

			try
			{
				for (size_type i = 0; i < m_size; ++i)
				{
					new(temp + i)T(m_ptr[i]);
					constructedElements = i;
				}
			}
			catch (...)
			{
				m_size = constructedElements;
				Deallocate();
				m_size = 0;
				m_capacity = 0
			}

			Deallocate();
			m_ptr = temp;
			temp = nullptr;

			m_capacity = new_capacity;
		}

		CHECK;
	}

	friend void swap(Vector<T>& lhs, Vector<T>& rhs)
	{
		Vector<T> temp = std::move(lhs);
		lhs = std::move(rhs);
		rhs = std::move(temp);
	}

	void resize(size_type new_size)
	{
		if (new_size == m_size)
		{
			return;
		}

		if (new_size < m_size)
		{
			for (T* i = m_ptr + new_size; i != m_ptr + m_size + 1; i++)
			{
				i->~T();
			}
		}

		if (m_capacity < new_size)
		{
			reserve(new_size);

			for (size_type i = m_size; i < new_size; i++)
			{
				new(m_ptr + i)T{};
			}
		}

		m_size = new_size;
	}

	pointer data() noexcept
	{
		return m_ptr;
	}

	const_pointer data() const noexcept
	{
		return m_ptr;
	}

	void shrink_to_fit()
	{
		pointer temp = m_allocator.allocate(m_size);

		for (size_type i = 0; i < m_size; ++i)
		{
			new(temp + i)T();
			temp[i] = m_ptr[i];
		}

		Deallocate();
		m_ptr = temp;
		temp = nullptr;

		m_capacity = m_size;
		CHECK;
	}

	void push_back(const T&& c)
	{
		push_back(c);
	}

	void push_back(const T& c)
	{
		if (m_ptr == nullptr)
		{
			m_size = 0;
			m_capacity = 1;
			m_ptr = Allocate();
		}
		else if (m_size + 1 > m_capacity)
		{
			reserve(2 * (m_capacity == 0 ? 1 : m_capacity));
		}

		new(m_ptr + m_size)T(c);
		++m_size;

		CHECK;
	}

	template<class... Args>
	reference emplace_back(Args&&... Targs)
	{
		reserve(m_size + sizeof...(Targs));
		return *new (m_ptr + m_size++) T(std::forward<Args>(Targs)...);
	}

	bool invariant() const
	{
		if (m_ptr != nullptr)
		{
			if (m_capacity <= 0)
			{
				return false;
			}
		}
		else
		{
			if (m_capacity != 0)
			{
				return false;
			}
		}

		return
			m_capacity >= m_size;
	}

	friend int operator<=>(const Vector& lhs, const Vector& rhs)
	{
		pointer first1 = lhs.m_ptr;
		pointer last1 = lhs.m_ptr + lhs.m_size;

		pointer first2 = rhs.m_ptr;
		pointer last2 = rhs.m_ptr + rhs.m_size;

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
	}

	Vector& operator=(const Vector& other)
	{
		return AssFast(other);
		//FastAss(other);
		//StrongAss(other);

		//if (this == &other)
		//	return *this;

		//if (other.size() > m_capacity) {
		//	m_size = other.size();
		//	reserve(m_size);
		//}
		//else
		//{
		//	m_size = other.size();
		//}

		//for (size_type i = 0; i < m_size; i++)
		//	m_ptr[i] = other.m_ptr[i];

		//CHECK;
		//return *this;
	}

	Vector& operator=(Vector&& other) noexcept
	{
		m_allocator.deallocate(m_ptr, m_capacity);

		m_ptr = other.m_ptr;
		m_capacity = other.m_capacity;
		m_size = other.m_size;

		other.m_ptr = nullptr;
		other.m_size = 0;
		other.m_capacity = 0;

		CHECK;
		return *this;
	}

	friend bool operator==(const Vector& lhs, const Vector& rhs)
	{
		if (lhs.size() != rhs.size())
		{
			return false;
		}

		pointer first1 = lhs.m_ptr;
		pointer last1 = lhs.m_ptr + lhs.m_size;

		pointer first2 = rhs.m_ptr;
		pointer last2 = rhs.m_ptr + rhs.m_size;

		for (; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2)
		{
			if (*first1 != *first2)
			{
				return false;
			}
		}

		return true;
	}

	friend std::ostream& operator<<(std::ostream& cout, const Vector& other)
	{
		for (size_type i = 0; i < other.size(); ++i)
			cout << other[i];
		return cout;
	}

	const_reference operator[](size_t i) const
	{
		return m_ptr[i];
	}

	reference operator[](size_t i)
	{
		invariant();
		return m_ptr[i];
	}

#pragma region assignmentoperators

	Vector& Ass(const Vector& rhs)
	{
		return *this = rhs;
	}

	Vector& AssFast(const Vector& rhs)
	{
		if (this == &rhs)
			return *this;

		if (rhs.m_size > m_capacity)
		{
			Deallocate();
			m_capacity = rhs.m_size;

			m_ptr = Allocate();
			m_size = 0;
		}

		for (size_t i = 0; i < m_size && i < rhs.m_size; i++)
			m_ptr[i] = rhs.m_ptr[i];

		if (m_size < rhs.m_size)
		{
			for (m_size; m_size < rhs.m_size; ++m_size) {
				new(m_ptr + m_size) T(rhs.m_ptr[m_size]);
			}
		}
		else
			resize(rhs.m_size);

		CHECK;
		return *this;
	}

	Vector& AssStrong(const Vector& rhs) //rollback
	{
		Vector temp(rhs);
		std::swap(*this, temp);
		CHECK;
		return *this;
	}

	Vector& AssSimple(const Vector& rhs) //garantera inga minnesläckor
	{

		Vector temp(rhs);
		std::swap(*this, temp);
		CHECK;
		return *this;
	}

#pragma region 

#pragma region ITERATORS

	iterator begin() noexcept
	{
		return iterator(m_ptr);
	};

	const_iterator begin()const noexcept
	{
		return const_iterator(m_ptr);
	};

	iterator end() noexcept
	{
		return iterator(m_ptr + m_size);
	};

	const_iterator end() const noexcept
	{
		return const_iterator(m_ptr + m_size);
	};

	const_iterator cbegin() const noexcept
	{
		return const_iterator(m_ptr);
	};

	const_iterator cend() const noexcept
	{
		return const_iterator(m_ptr + m_size);
	};

	//REVERSE ITERATOR Begin

	reverse_iterator rbegin() noexcept
	{
		return reverse_iterator(m_ptr + m_size - 1);
	};

	const_reverse_iterator rbegin() const noexcept
	{
		return const_reverse_iterator(m_ptr + m_size - 1);
	};

	reverse_iterator rend() noexcept
	{
		return reverse_iterator(m_ptr - 1);
	};

	const_reverse_iterator rend() const noexcept
	{
		return const_reverse_iterator(m_ptr - 1);
	};

	const_reverse_iterator crbegin() const noexcept
	{
		return const_reverse_iterator(m_ptr + m_size - 1);
	};

	const_reverse_iterator crend() const noexcept
	{
		return const_reverse_iterator(m_ptr - 1);
	};

#pragma endregion

private:
	T* m_ptr;
	size_t m_size;
	size_t m_capacity;
	Dalloc<T> m_allocator;
};
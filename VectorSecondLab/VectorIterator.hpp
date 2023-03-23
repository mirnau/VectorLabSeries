#pragma once
#include <utility>
#include<vector>

template<class T, int dir>
class VectorIterator
{
public:
	using iterator_category = std::random_access_iterator_tag;

	using value_type = T;
	using pointer = T*;
	using reference = T&;
	using size_type = size_t;
	using difference_type = std::ptrdiff_t;
	using const_reference = const T&;
	using const_pointer = const T*;

private:
	pointer m_ptr; //Can be any provided type of pointer

public:
	VectorIterator(pointer p) noexcept 
		:
		m_ptr(p)
	{}

	VectorIterator() noexcept 
		:
		m_ptr(nullptr)
	{}

	VectorIterator(VectorIterator&) noexcept = default;

	//Takes an instance and returns a copy
	VectorIterator(const VectorIterator& other) 
		:
		m_ptr(other.m_ptr)
	{}

	VectorIterator& operator=(VectorIterator&) = default;

	VectorIterator& operator=(const VectorIterator& other)
	{
		m_ptr = other.m_ptr;
		return *this;
	}

	reference operator*() const noexcept
	{
		return *m_ptr;
	}

	pointer operator->() const noexcept
	{
		return m_ptr;
	}

	reference operator[](size_t i) const noexcept
	{
		return m_ptr[i * dir];
	}

	VectorIterator& operator++() noexcept
	{
		m_ptr += dir;
		return *this;
	}

	VectorIterator& operator--() noexcept
	{
		m_ptr -= dir;
		return *this;
	}

	VectorIterator operator++(int) noexcept
	{
		VectorIterator temp = *this;
		++(*this);
		return temp;
	}

	VectorIterator operator--(int) noexcept
	{
		VectorIterator temp = *this;
		--(*this);
		return temp;
	}

	VectorIterator& operator += (difference_type i) noexcept
	{
		m_ptr += i;
		return *this;
	}

	VectorIterator operator+(difference_type rhs) const noexcept
	{
		return VectorIterator(m_ptr + rhs * dir);
	}

	VectorIterator operator-(difference_type rhs) const noexcept
	{
		return VectorIterator(m_ptr - rhs * dir);
	}

	difference_type operator-(const VectorIterator& other) const noexcept
	{
		difference_type d = m_ptr - other.m_ptr;
		d *= dir;
		return d;
	}

	friend bool operator==(const VectorIterator& lhs, const VectorIterator& rhs) noexcept
	{
		return lhs.m_ptr == rhs.m_ptr;
	}

	friend int operator<=>(const VectorIterator& lhs, const VectorIterator& rhs) noexcept
	{
		if (lhs.m_ptr > rhs.m_ptr)
			return 1 *dir;
		else if (lhs.m_ptr < rhs.m_ptr)
			return -1 * dir;
		else
			return 0;
	}
};


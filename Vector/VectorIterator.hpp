#pragma once
#include <utility>
#include<vector>

template<class Vector, int dir>
class VectorIterator
{
public:
	using iterator_category = std::random_access_iterator_tag;
	using value_type = Vector::value_type;
	using pointer = Vector::pointer;
	using reference = Vector::reference;
	using difference_type = std::ptrdiff_t;

private:
	pointer m_ptr; //Can be any provided type of pointer

public:
	VectorIterator(pointer p) :
		m_ptr(p)
	{

	};

	VectorIterator() :
		m_ptr(nullptr)
	{

	};

	VectorIterator(VectorIterator&) = default;

	VectorIterator& operator=(VectorIterator&) = default;

	//Takes an instance and returns a copy
	VectorIterator(const VectorIterator& other) :
		m_ptr(other.m_ptr)
	{

	};

	VectorIterator& operator=(const VectorIterator& other)
	{
		m_ptr = other.m_ptr;
		return *this;
	};

	reference operator*() const
	{
		return *m_ptr;
	};

	pointer operator->() const
	{
		return m_ptr;
	};

	reference operator[](size_t i) const
	{
		return m_ptr[i * dir];
	};

	VectorIterator& operator++()
	{
		m_ptr += dir;
		return *this;
	};

	VectorIterator& operator--()
	{
		m_ptr -= dir;
		return *this;
	};

	//What is going on here!!!
	VectorIterator operator++(int)
	{
		VectorIterator temp = *this;
		++(*this);
		return temp;
	};

	//What is going on here!!!
	VectorIterator operator--(int)
	{
		VectorIterator temp = *this;
		--(*this);
		return temp;
	};

	VectorIterator& operator += (difference_type i)
	{
		m_ptr += i;
		return *this;
	};

	VectorIterator operator+(difference_type rhs) const
	{
		return VectorIterator(m_ptr + rhs * dir);
	};

	VectorIterator operator-(difference_type rhs) const
	{
		return VectorIterator(m_ptr - rhs * dir);
	};

	difference_type operator-(const VectorIterator& other) const
	{
		difference_type d = m_ptr - other.m_ptr;
		d *= dir;
		return d;
	};

	friend bool operator==(const VectorIterator& lhs, const VectorIterator& rhs)
	{
		return lhs.m_ptr == rhs.m_ptr;
	};

	friend int operator<=>(const VectorIterator& lhs, const VectorIterator& rhs)
	{
		if (lhs.m_ptr > rhs.m_ptr)
			return 1 *dir;
		else if (lhs.m_ptr < rhs.m_ptr)
			return -1 * dir;
		else
			return 0;
	};
};


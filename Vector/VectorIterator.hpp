#pragma once
#include <utility>
#include<vector>

template<class V, class CV, int dir>
class VectorIterator
{
public:
	using iterator_category = std::random_access_iterator_tag;
	using value_type = typename V::value_type;
	using pointer = value_type*;
	using reference = value_type&;
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
		return m_ptr[i];
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
		return *temp;
	};

	//What is going on here!!!
	VectorIterator operator--(int)
	{
		VectorIterator temp = *this;
		--m_ptr;
		return temp;
	};

	VectorIterator& operator += (difference_type i)
	{
		m_ptr += i;
		return *this;
	};

	VectorIterator operator+(difference_type rhs) const
	{
		return VectorIterator(m_ptr + rhs);
	};
	
	VectorIterator operator-(difference_type rhs) const
	{
		return VectorIterator(m_ptr - rhs);
	};

	difference_type operator-(const VectorIterator& other) const
	{
		return m_ptr - other.m_ptr;
	};

	friend bool operator==(const VectorIterator& lhs, const VectorIterator& rhs)
	{
		return lhs.m_ptr == rhs.m_ptr;
	};

	friend int operator<=>(const VectorIterator& lhs, const VectorIterator& rhs)
	{
		if (lhs.m_ptr > rhs.m_ptr)
			return 1;
		else if (lhs.m_ptr < rhs.m_ptr)
			return -1;
		else
			return 0;
	};
};


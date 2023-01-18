#pragma once

template <class T>
class Vector
{
private:
	size_t _size;
	size_t _capacity;

public:

	////Constructors & Destrctor
	Vector() noexcept
	{
		_size = 0;
		_capacity = 0;
	};
	//Vector(const char* other);
	//Vector(const Vector& other);
	//Vector(Vector&& other) noexcept;

	//~Vector() noexcept;

	////Methods
	size_t size() const noexcept
	{
		return _size;
	};

	//size_t capacity() const noexcept
	//{
	//	return _capacity;
	//}

	//bool invariant() const
	//{
	//	return 0;
	//};

#define CHECK assert((invariant()))
	

	//void reserve(size_t n);
	//void shrink_to_fit();
	//void resize(size_t n);
	//void push_back(const T& c);

	//[const] T& at(size_t i)[const];
	//[const] T* data()[const] noexcept;

	////Operator overloading
	//Vector& operator=(const Vector& other);
	//Vector& operator=(Vector&& other) noexcept;
	//
	//[const] T& operator[](size_t i)[const];

	//friend bool operator==(const Vector& lhs,
	//	const Vector& other);

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


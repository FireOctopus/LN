#include<exception>
#include "MyVector.h"

MyVector::MyVector() : MyVector(10)
{}

MyVector::MyVector(const size_t size)
{
	this->m_isDef = true;
	this->m_size = size;
	this->m_top = 1;
	this->m_array = (int*)calloc(size, sizeof(int));
	if (!this->m_array)
		throw std::bad_alloc();
}

MyVector::MyVector(const MyVector &vect)
{
	this->m_isDef = vect.m_isDef;
	this->m_size = vect.m_size;
	this->m_top = vect.m_top;
	this->m_array = (int*)calloc(this->m_size, sizeof(int));
	if (!this->m_array) {
		free(vect.m_array);
		throw std::bad_alloc();
	}
	memcpy(this->m_array, vect.m_array, this->m_size * sizeof(int));
}

MyVector& MyVector::operator=(const MyVector& vect)
{
	if (&vect == this)
		return *this;
	free(this->m_array);
	this->m_array = (int*)calloc(vect.m_size, sizeof(int));
	if (!this->m_array) {
		free(vect.m_array);
		throw std::bad_alloc();
	}
	memcpy(this->m_array, vect.m_array, vect.m_size * sizeof(int));
}

MyVector::~MyVector()
{
	free(this->m_array);
}

size_t MyVector::getSize() const
{
	return this->m_isDef ? 0 : this->m_top;
}

int MyVector::getValue(size_t pos) const
{
	return this->m_array[pos];
}

void MyVector::setValue(size_t pos, const int value) const
{
	this->m_array[pos] = value;
}


void MyVector::ensureCapacity(long long newSize)
{
	int* check = nullptr;
	if (check = (int*)realloc(this->m_array, newSize * sizeof(int))) {
		this->m_array = check;
		this->m_size = newSize;
	} else {
		if (check = (int*)realloc(this->m_array, (this->m_size + 1) * sizeof(int))) {
			this->m_array = check;
			this->m_size++;
		} else
			throw std::bad_alloc();
	}
}

void MyVector::push(const int &value)
{
	if (this->m_size > this->m_top) {
		if (this->m_top == 1 && this->m_isDef) {
			this->m_top--;
			this->m_isDef = false;
		}
		this->m_array[this->m_top++] = value;
	} else {
		try {
			this->ensureCapacity((this->m_size) * 2);
			this->m_array[this->m_top++] = value;
		} catch (const int errorCode) {
			throw "ERROR: Memory allocation failed";
		}
	}
}

void MyVector::pop()
{
	this->m_top--;
}

void MyVector::resize(size_t newSize)
{
	*this = *(new MyVector(newSize));
	this->m_isDef = false;
	this->m_top = newSize;
}
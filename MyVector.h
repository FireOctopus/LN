#ifndef _MyVector_H
#define _MyVector_H
#include <cstdlib>
#include <cstring>
class MyVector {
public:
	MyVector();
	MyVector(const MyVector& vect);
	MyVector(const size_t size);
	~MyVector();
	MyVector &operator = (const MyVector &vect);
	void push(const int &value);
	void pop();
	void resize(size_t newSize);
	size_t getSize() const;
	int getValue(size_t pos) const;
	void setValue(size_t pos, const int value) const;
private:
	size_t m_size{};
	size_t m_top{};
	bool m_isDef{};
	int* m_array{};
	void ensureCapacity(long long newSize);
};
#endif
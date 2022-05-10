#ifndef _LN_H
#define _LN_H
#include <cmath>
#include <string>
#include <string_view>
#include "MyVector.h"

class LN {
public:
	explicit LN(long long num = 0);
	LN(const LN& ln);
	LN(LN&& ln);
	explicit LN(const char* str);
	explicit LN(const bool isNaN);
	explicit LN(const std::string_view str);
	~LN();
	LN &operator = (const LN &a);
	LN &operator =(LN &&a);
	friend bool operator == (const LN &a, const LN &b);
	friend bool operator != (const LN &a, const LN &b);
	friend bool operator < (const LN &a, const LN &b);
	friend bool operator > (const LN &a, const LN &b);
	friend bool operator <= (const LN &a, const LN &b);
	friend bool operator >= (const LN &a, const LN &b);
	friend LN operator + (const LN &a, const LN &b);
	friend LN operator - (const LN &a, const LN &b);
	friend LN operator - (const LN &a);
	friend LN operator * (const LN &a, const LN &b);
	friend LN operator / (const LN &a, const LN &b);
	friend LN operator % (const LN &a, const LN &b);
	friend LN operator""_ln(const char *s);
	LN operator +=(const LN &a);
	LN operator -=(const LN &a);
	LN operator *=(const LN &a);
	LN operator /=(const LN &a);
	LN operator %=(const LN &a);
	operator long long() const;
	operator bool() const;
	std::string toString() const;
	LN sqrt() const;
private:
	bool m_isNaN{};
	bool m_isNeg{};
	MyVector *m_digits{};
	LN(const bool isNeg, const MyVector* digits);
	LN negate() const;
	int compare(const LN &a) const;
	void shift();
	void removeZeros();
	LN add(const LN &a) const;
	LN sub(const LN &a) const;
};

#endif
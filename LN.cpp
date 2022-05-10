#include "LN.h"
#include<climits>
constexpr int BASE = 1000000000;

LN::LN(long long num)
{
    this->m_digits = new MyVector();
    if (!num)
        this->m_digits->push(0);
    if (num < 0) {
        num *= -1;
        this->m_isNeg = true;
    } else
        this->m_isNeg = false;
    while (num) {
        this->m_digits->push(num % BASE);
        num /= BASE;
    }    
}

LN::LN(const LN &ln)
{
    this->m_isNaN = ln.m_isNaN;
    this->m_isNeg = ln.m_isNeg;
    this->m_digits = new MyVector(*ln.m_digits);
}

LN::LN(LN &&ln) : m_digits(ln.m_digits), m_isNeg(ln.m_isNeg), m_isNaN(ln.m_isNaN)
{
    ln.m_digits = nullptr;
}

LN::LN(const bool isNeg, const MyVector *digits)
{
    this->m_isNeg = isNeg;
    this->m_digits = new MyVector(*digits);
}

LN::LN(const char* str)
{
    this->m_digits = new MyVector();
    this->m_isNeg = (str[0] == '-') ? true : false;
    for (int i = strlen(str) - 1; i >= (this->m_isNeg ? 1 : 0); i -= 9) {
        int dig, curPos;
        dig = 0;
        curPos = i < 9 ? (this->m_isNeg ? 1 : 0) : i - 8;
        while (curPos <= i)
            dig = (str[curPos++] - '0') + dig * 10;
        this->m_digits->push(dig);
    }
}

LN::LN(const std::string_view str)
{
    this->m_digits = new MyVector();
    this->m_isNeg = (str[0] == '-') ? true : false;
    for (int i = str.length() - 1; i >= (this->m_isNeg ? 1 : 0); i -= 9) {
        int dig, curPos;
        dig = 0;
        curPos = i < 9 ? (this->m_isNeg ? 1 : 0) : i - 8;
        while (curPos <= i)
            dig = (str[curPos++] - '0') + dig * 10;
        this->m_digits->push(dig);
    }
}

LN::LN(const bool isNaN)
{
    this->m_isNaN = isNaN;
    this->m_isNeg = false;
    this->m_digits = new MyVector();
}

LN::~LN()
{
    delete this->m_digits;
}

LN& LN::operator=(const LN& a)
{
    if (&a == this)
        return *this;
    delete this->m_digits;
    this->m_digits = new MyVector(*a.m_digits);
    return *this;
}

LN& LN::operator=(LN&& a)
{
    if (&a == this)
        return *this;
    delete this->m_digits;
    this->m_isNaN = a.m_isNaN;
    this->m_isNeg = a.m_isNeg;
    this->m_digits = a.m_digits;
    a.m_digits = nullptr;
    return *this;
}

LN LN::negate() const
{
    if (this->compare(LN())) 
        return *(new LN(!this->m_isNeg, this->m_digits));
    return *this;
}

int LN::compare(const LN &a) const
{
    if (!a.m_digits->getSize())
        a.m_digits->push(0);
    if (!this->m_isNeg && !a.m_isNeg) {
        if (this->m_digits->getSize() > a.m_digits->getSize())
            return 1;
        else if (this->m_digits->getSize() < a.m_digits->getSize())
            return -1;
        else
            for (long long i = this->m_digits->getSize() - 1; i >= 0; i--) {
                if (this->m_digits->getValue(i) != a.m_digits->getValue(i))
                    return this->m_digits->getValue(i) > a.m_digits->getValue(i) ? 1 : -1;
            }
        return 0;
    } else if (this->m_isNeg != a.m_isNeg) {
        return this->m_isNeg ? -1 : 1;
    } else {
        LN b = LN(!a.m_isNeg, a.m_digits);
        return -LN(!this->m_isNeg, this->m_digits).compare(b);
    }
}

void LN::removeZeros()
{
    while (this->m_digits->getSize() > 1 && !this->m_digits->getValue(this->m_digits->getSize() - 1)) {
        this->m_digits->pop();
    }
    if (this->m_digits->getSize() == 1 && this->m_digits->getValue(0) == 0)
        this->m_isNeg = false;
}

LN LN::add(const LN &a) const
{
    if (this->m_isNaN || a.m_isNaN)
        return *(new LN(true));
    if (this->m_isNeg == a.m_isNeg) {
        MyVector* result = new MyVector();
        short int carry = 0;
        int t;
        for (size_t i = 0; i < std::min(this->m_digits->getSize(), a.m_digits->getSize()); i++) {
            t = this->m_digits->getValue(i) + a.m_digits->getValue(i) + carry;
            result->push(t % BASE);
            carry = t / BASE;
        }
        if (this->m_digits->getSize() > a.m_digits->getSize()) {
            for (size_t i = a.m_digits->getSize(); i < this->m_digits->getSize(); i++) {
                t = this->m_digits->getValue(i) + carry;
                result->push(t % BASE);
                carry = t / BASE;
            }
        } else {
            for (size_t i = this->m_digits->getSize(); i < a.m_digits->getSize(); i++) {
                t = a.m_digits->getValue(i) + carry;
                result->push(t % BASE);
                carry = t / BASE;
            }
        }
        if (carry)
            result->push(carry);
        return *(new LN(this->m_isNeg, result));
    } else {
        if (!this->compare(a.negate()))
            return *(new LN());
        else
            return a.m_isNeg ? this->sub(a) : a.sub(*this);
    }
}

LN LN::sub(const LN &a) const
{
    if (this->m_isNaN || a.m_isNaN)
        return *(new LN(true));
    if (!a.m_digits->getSize())
        return *this;
    LN result = *this;
    short int carry = 0;
    if (!this->m_isNeg && !a.m_isNeg) {
        if (this->compare(a) >= 0) {
            for (size_t i = 0; i < a.m_digits->getSize() || carry != 0; i++) {
                int t = result.m_digits->getValue(i) - carry - (i < a.m_digits->getSize() ? a.m_digits->getValue(i) : 0);
                result.m_digits->setValue(i, t);
                carry = result.m_digits->getValue(i) < 0;
                if (carry)
                    result.m_digits->setValue(i, result.m_digits->getValue(i) + BASE);
            }
            return result;
        } else
            return a.sub(*this).negate();
    }
    if (this->m_isNeg != a.m_isNeg)
        return a.m_isNeg ? this->add(a.negate()) : a.add(this->negate());
    return a.negate().sub(this->negate());
}

void LN::shift()
{
    if (this->m_digits->getSize() == 0) {
        this->m_digits->push(0);
        return;
    }
    this->m_digits->push(this->m_digits->getValue(this->m_digits->getSize() - 1));
    for (size_t i = this->m_digits->getSize() - 2; i > 0; i--)
        this->m_digits->setValue(i, this->m_digits->getValue(i - 1));
    this->m_digits->setValue(0, 0);
}

LN LN::sqrt() const
{
    LN r = *this;
    LN two = *(new LN (2LL));
    LN l = *(new LN());
    LN one = *(new LN(1LL));
    while (r - l > one) {
        LN m = (l + r) / two;
        if (m * m <= *this)
            l = m;
        else
            r = m - one;
    }
    if (r * r <= *this)
        l = r;
    return l;
}

std::string LN::toString() const
{
    if (this->m_isNaN) {
        return "NaN";
    }
    if (this->m_digits->getSize() == 0) {
        return "0";
    }
    std::string res = this->m_isNeg ? "-" : "";
    res += std::to_string(this->m_digits->getValue(this->m_digits->getSize() - 1));
    for (int i = this->m_digits->getSize() - 2; i >= 0; i--) {
        int t = this->m_digits->getValue(i);
        res += std::string(t > 0 ? 9 - floor(log10(t)) - 1 : 8, '0') + std::to_string(t);
    }
    return res;
}

bool operator == (const LN &a, const LN &b)
{
    return !a.compare(b);
}

bool operator != (const LN &a, const LN &b)
{
    return a.compare(b);
}

bool operator < (const LN &a, const LN &b)
{
    return a.compare(b) < 0;
}

bool operator > (const LN &a, const LN &b)
{
    return a.compare(b) > 0;
}

bool operator <= (const LN &a, const LN &b)
{
    return a.compare(b) <= 0;
}

bool operator >= (const LN &a, const LN &b)
{
    return a.compare(b) >= 0;
}

LN operator + (const LN &a, const LN &b)
{
    return a.add(b);
}

LN operator - (const LN &a, const LN &b)
{
    return a.sub(b);
}

LN operator - (const LN &a)
{
    return a.negate();
}

LN operator * (const LN &a, const LN &b)
{
    LN res = *(new LN());
    res.m_digits->resize(a.m_digits->getSize() + b.m_digits->getSize());
    for (size_t i = 0; i < a.m_digits->getSize(); i++) {
        int carry = 0;
        for (size_t j = 0; j < b.m_digits->getSize() || carry; j++) {
            long long cur = res.m_digits->getValue(i + j) + a.m_digits->getValue(i) * 1LL * (j < b.m_digits->getSize() ? b.m_digits->getValue(j) : 0) + carry;
            res.m_digits->setValue(i + j, (int)(cur % BASE));
            carry = (int) (cur / BASE);
        }
    }
    res.m_isNeg = a.m_isNeg != b.m_isNeg;
    res.removeZeros();
    return res;
}


LN operator / (const LN &a, const LN &b)
{
    if (a.m_isNaN || b.m_isNaN || !b.compare(LN()))
        return *(new LN(true));
    if (a.m_digits->getValue(0) == 0 && a.m_digits->getSize() == 1 || !a.m_digits->getSize())
        return a;
    if (b.m_digits->getSize() == 1) {
        int carry = 0;
        for (int i = (int)a.m_digits->getSize() - 1; i >= 0; --i) {
            long long cur = a.m_digits->getValue(i) + carry * 1ll * BASE;
            a.m_digits->setValue(i, int(cur / b.m_digits->getValue(0)));
            carry = int(cur % b.m_digits->getValue(0));
        }
        LN res = a;
        res.removeZeros();
        return res;
    }
    LN c = b;
    c.m_isNeg = false;
    LN res = *(new LN()), cur = *(new LN());
    res.m_digits->resize(a.m_digits->getSize());
    for (long long i = static_cast<long long> (a.m_digits->getSize()) - 1; i >= 0; i--) {
        cur.shift();
        cur.m_digits->setValue(0, a.m_digits->getValue(i));
        cur.removeZeros();
        int x = 0, l = 0, r = BASE;
        while (l <= r) {
            int m = (l + r) / 2;
            LN t = c * (*(new LN((long long)m)));
            if (t <= cur) {
                x = m;
                l = m + 1;
            } else
                r = m - 1;
        }
        res.m_digits->setValue(i, x);
        res.removeZeros();
        cur = cur - c * (*(new LN((long long)x)));
    }
    res.m_isNeg = a.m_isNeg != b.m_isNeg;
    res.removeZeros();
    return res;
}

LN operator % (const LN &a, const LN &b)
{
    LN res = a - (a / b) * b;
    if (res.m_isNeg)
        res += b;
    res.removeZeros();
    return res;
}

LN operator""_ln(const char* s)
{
    return LN(s);
}

LN LN::operator += (const LN &a)
{
    return *this + a;
}

LN LN::operator -= (const LN &a)
{
    return *this - a;
}

LN LN::operator *= (const LN &a)
{
    return *this * a;
}

LN LN::operator /= (const LN &a)
{
    return *this / a;
}

LN LN::operator %= (const LN &a)
{
    return *this % a;
}

LN::operator bool() const
{
    return *this != LN(0ll);
}

LN::operator long long() const
{
    if (*this > LN(LLONG_MAX) || *this < LN(LLONG_MIN))
        throw -1;
    return atoll(this->toString().c_str());
}
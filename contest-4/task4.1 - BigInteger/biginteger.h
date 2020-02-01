/*  Task:
    Напишите класс BigInteger для работы с длинными целыми числами. Должны поддерживаться операции:
    сложение, вычитание, умножение, деление, остаток по модулю, работающие так же, как и для int; 
    составное присваивание с этими операциями;
    унарный минус, префиксный и постфиксный инкремент и декремент,
    операторы сравнения == != < > <= >=
    вывод в поток и ввод из потока (возможен ввод из пустого потока);
    метод toString(), возвращающий строковое представление числа;
    конструирование из int (в том числе неявное преобразование, когда это надо);
    неявное преобразование в bool, когда это надо (должно работать в условных выражениях).

    Асимптотические ограничения на время работы операторов в контесте не проверяются, но реализация должна работать за:
    Сложение, вычитание, унарный минус, операторы сравнения – O(n),
    Умножение -- о(n^1.6)  (о-малое, заметьте) 
    Деление, остаток по модулю – O(n2),
    где n – количество разрядов большего числа (по модулю).


    developed by Crashdown    
*/
#include <iostream>
#include <vector>
#include <string>

class BigInteger
{
public:
    BigInteger();
    BigInteger(int a);
    BigInteger(std::string& s);

    BigInteger operator+=(const BigInteger& other);
    BigInteger operator*=(const BigInteger& other);
    BigInteger operator-=(const BigInteger& other);
    BigInteger operator/=(const BigInteger& other);
    BigInteger operator%=(const BigInteger& other);

    friend BigInteger operator+(const BigInteger& a, const BigInteger& b);
    friend BigInteger operator*(const BigInteger& a, const BigInteger& b);
    friend BigInteger operator-(const BigInteger& a, const BigInteger& b);
    friend BigInteger operator/(const BigInteger& a, const BigInteger& b);
    friend BigInteger operator%(const BigInteger& a, const BigInteger& b);

    bool operator==(const BigInteger& other) const;
    bool operator!=(const BigInteger& other) const;
    bool operator<(const BigInteger& other) const;
    bool operator<=(const BigInteger& other) const;
    bool operator>(const BigInteger& other) const;
    bool operator>=(const BigInteger& other) const;

    BigInteger operator-();

    BigInteger& operator++();
    BigInteger operator++(int);
    BigInteger& operator--();
    BigInteger operator--(int);

    std::string toString() const;

    explicit operator bool();

    friend std::istream& operator>>(std::istream& is, BigInteger& a);
    friend std::ostream& operator<<(std::ostream& os, const BigInteger& a);


private:
    std::vector< int > _buffer;
    bool _is_positive = true;

    //  Summation and difference functions which ignores positive/negative flags
    void add(BigInteger& a, const BigInteger& b);
    void sub(BigInteger& a, const BigInteger& b);

    //  Comparation which ignores positive/negative flags
    bool cmp_abs(const BigInteger& a, const BigInteger& b) const;

    //  Remove extra zeroes
    void crop();

    //  Fill with zeroes to next power of two after n
    void fill(int n);

    //  Divide a by b if result is digit
    int simple_div(const BigInteger& a, const BigInteger& b);

    //  Reverse _buffer
    void reverse();

    //  If number is equal to zero, change _is_positive to true
    void zero_normalize();

    BigInteger karatsuba_mul(const BigInteger& a, const BigInteger& b);

};


BigInteger::BigInteger(int a){

    _is_positive = (a >= 0);

    if(a < 0){
        a *= -1;
    }

    _buffer.clear();

    if(a == 0){
        _buffer.push_back(a);
    }

    while(a > 0){
        _buffer.push_back(a % 10);
        a /= 10;
    }

    zero_normalize();
}

BigInteger::BigInteger() : BigInteger(0) {}

BigInteger::BigInteger(std::string& s){

    int first = (s[0] == '-') ? 1 : 0;

    _is_positive = (first == 0);

    for(int i = s.size() - 1; i >= first; --i){
        _buffer.push_back(s[i] - '0');
    }

    zero_normalize();
}

std::string BigInteger::toString() const{
    std::string tmp = _is_positive ? "" : "-";

    if(cmp_abs(*this, 1)){
        return "0";
    }

    if(_buffer.size() == 0 && _buffer[0] == 0){
        tmp = '0';
        return tmp;
    }

    int first = _buffer.size() - 1;
    while(first > 0 && _buffer[first] == 0){
        --first;
    }

    for(int i = first; i >= 0; --i){
        tmp += ('0' + _buffer[i]);
    }

    return tmp;
}

void BigInteger::add(BigInteger& a, const BigInteger& b){
    int transit = 0;

    for(int i = 0; i < (int)std::max(a._buffer.size(), b._buffer.size()); ++i){
        int to_write = transit;

        if(i < (int)a._buffer.size()){
            to_write += a._buffer[i];
        } else {
            a._buffer.push_back(0);
        }

        if(i < (int)b._buffer.size()){
            to_write += b._buffer[i];
        }

        transit = to_write / 10;
        a._buffer[i] = to_write % 10;
    }

    if(transit > 0){
        a._buffer.push_back(transit);
    }

    a.crop();
}

void BigInteger::sub(BigInteger& a, const BigInteger& b){
    bool transit = 0;

    for(int i = 0; i < (int)std::max(a._buffer.size(), b._buffer.size()); ++i){
        int to_write = transit ? -1 : 0;

        if(i < (int)a._buffer.size()){
            to_write += a._buffer[i];
        } else {
            a._buffer.push_back(0);
        }

        if(i < (int)b._buffer.size()){
            to_write -= b._buffer[i];
        }

        transit = to_write < 0;
        a._buffer[i] = transit ? to_write + 10 : to_write;
    }

    if(transit){
        a._buffer.push_back(transit);
    }

    a.crop();
}

BigInteger BigInteger::operator+=(const BigInteger& other){
    if(_is_positive == other._is_positive){
        add(*this, other);
        return *this;
    }

    if(_is_positive && !other._is_positive){
        if(cmp_abs(other, *this)){
            sub(*this, other);
            return *this;
        }

        BigInteger tmp = other;
        sub(tmp, *this);
        *this = tmp;
        return *this;
    }

    if(cmp_abs(*this, other)){
        BigInteger tmp = other;
        sub(tmp, *this);
        *this = tmp;
        return *this;
    }

    sub(*this, other);
    return *this;
}

BigInteger BigInteger::operator-=(const BigInteger& other){
    if(_is_positive && !other._is_positive){
        add(*this, other);
        return *this;
    }

    if(_is_positive && other._is_positive){
        if(cmp_abs(other, *this)){
            sub(*this, other);
            return *this;
        }

        BigInteger tmp = other;
        sub(tmp, *this);
        *this = tmp;
        _is_positive = false;
        zero_normalize();
        return *this;
    }

    if(!_is_positive && other._is_positive){
        add(*this, other);
        return *this;
    }

    if(cmp_abs(other, *this)){
        sub(*this, other);
        return *this;
    }

    BigInteger tmp = other;
    sub(tmp, *this);
    *this = tmp;
    _is_positive = true;
    return *this;
}

BigInteger operator+(const BigInteger& a, const BigInteger& b){
    BigInteger res = a;
    return res += b;
}

BigInteger operator-(const BigInteger& a, const BigInteger& b){
    BigInteger res = a;
    return res -= b;
}

void BigInteger::crop(){
    while(_buffer.size() > 0 && _buffer[_buffer.size() - 1] == 0){
        _buffer.pop_back();
    }

    if(_buffer.size() == 0){
        _is_positive = true;
        _buffer.push_back(0);
    }

    zero_normalize();
}

bool BigInteger::cmp_abs(const BigInteger& a, const BigInteger& b) const {
    int a_start = a._buffer.size() - 1;
    int b_start = b._buffer.size() - 1;

    while(a_start > 0 && a._buffer[a_start] == 0){
        --a_start;
    }

    while(b_start > 0 && b._buffer[b_start] == 0){
        --b_start;
    }

    if(a_start != b_start){
        return a_start < b_start;
    }

    for(int i = a_start; i >= 0; --i){
        if(a._buffer[i] != b._buffer[i]){
            return a._buffer[i] < b._buffer[i];
        }
    }

    return false;
}

void BigInteger::zero_normalize(){
    for(int i = 0; i < (int)_buffer.size(); ++i){
        if(_buffer[i] != 0){
            return;
        }
    }

    _is_positive = true;    
}

bool BigInteger::operator<(const BigInteger& other) const {

    if(!_is_positive && other._is_positive){
        return true;
    }

    if(_is_positive && !other._is_positive){
        return false;
    }

    if(_is_positive){
        return cmp_abs(*this, other);
    }

    return cmp_abs(other, *this);
}

bool BigInteger::operator>(const BigInteger& other) const {
    return (other < *this);
}

bool BigInteger::operator==(const BigInteger& other) const {
    return (!(*this < other) && !(other < *this));
}

bool BigInteger::operator!=(const BigInteger& other) const {
    return !(*this == other);
}

bool BigInteger::operator<=(const BigInteger& other) const {
    return (*this < other) || (*this == other);
}

bool BigInteger::operator>=(const BigInteger& other) const {
    return (other < *this) || (*this == other);
}

BigInteger::operator bool() {
    return *this != 0;
}

std::istream& operator>>(std::istream& is, BigInteger& a){
    std::string str;
    is >> str;
    BigInteger _a(str);
    a = _a;
    return is;
}

std::ostream& operator<<(std::ostream& os, const BigInteger& a){
    std::string str = a.toString();
    os << str;
    return os;
}

BigInteger BigInteger::operator-(){
    if(*this == 0){
        return *this;
    }
    BigInteger tmp = *this;
    tmp._is_positive = !tmp._is_positive;
    return tmp;
}


BigInteger& BigInteger::operator++(){
    *this += 1;
    return *this;
}

BigInteger& BigInteger::operator--(){
    *this -= 1;
    return *this;
}

BigInteger BigInteger::operator++(int){
    BigInteger tmp = *this;
    ++(*this);
    return tmp;
}

BigInteger BigInteger::operator--(int){
    BigInteger tmp = *this;
    --(*this);
    return tmp;
}

BigInteger BigInteger::operator*=(const BigInteger& other){
    BigInteger first = *this;
    BigInteger second = other;

    bool __is_positive = (_is_positive == other._is_positive);

    int len = std::max(first._buffer.size(), second._buffer.size());
    first.fill(len);
    second.fill(len);

    first = karatsuba_mul(first, second);

    int transit = 0;
    for(int i = 0; i < (int)first._buffer.size() - 1; ++i){
        if(first._buffer[i] >= 10){
            transit = first._buffer[i] / 10;
        }

        if(first._buffer[i] < 0){
            transit = (first._buffer[i] + 1) / 10 - 1;
        }

        first._buffer[i + 1] += transit;
        first._buffer[i] -= 10 * transit;

        transit = 0;
    }

    first._is_positive = __is_positive;

    *this = first;
    crop();

    zero_normalize();
    return *this;
}

BigInteger operator*(const BigInteger& a, const BigInteger& b){
    BigInteger res = a;
    res *= b;
    return res ;
}

void BigInteger::fill(int n){
    while(n & (n - 1)){
        ++n;
    }
    _buffer.resize(n, 0);
}

BigInteger BigInteger::karatsuba_mul(const BigInteger& a, const BigInteger& b){
    BigInteger res;
    int len = a._buffer.size();
    res._buffer.resize(len * 2, 0);

    if(len < 5){
        //  Simple multiplication (without transits)
        for(int i = 0; i < (int)a._buffer.size(); ++i){
            for(int j = 0; j < (int)b._buffer.size(); ++j){
                res._buffer[i + j] += a._buffer[i] * b._buffer[j];
            }
        }

        return res;
    }

    //  Karatsuba multiplication
    //  a0, b0 - first halfs of a and b
    //  a1, b1 - second halfs of a and b
    //  a * b = a0b0 + ((a0 + a1)(b0 + b1) - a0b0 - a1b1)*10^m + a1b1*10^2m

    BigInteger a0 = BigInteger(0);
    a0._buffer.resize(len / 2, 0);
    for(int i = 0; i < len / 2; ++i){
        a0._buffer[i] = a._buffer[i];
    }

    BigInteger b0 = BigInteger(0);
    b0._buffer.resize(len / 2, 0);
    for(int i = 0; i < len / 2; ++i){
        b0._buffer[i] = b._buffer[i];
    }

    BigInteger a1 = BigInteger(0);
    a1._buffer.resize(len / 2, 0);
    for(int i = 0; i < len / 2; ++i){
        a1._buffer[i] = a._buffer[len / 2 + i];
    }

    BigInteger b1 = BigInteger(0);
    b1._buffer.resize(len / 2, 0);
    for(int i = 0; i < len / 2; ++i){
        b1._buffer[i] = b._buffer[len / 2 + i];
    }

    a0._is_positive = true;
    a1._is_positive = true;
    b0._is_positive = true;
    b1._is_positive = true;

    BigInteger sum_a = BigInteger(0);
    BigInteger sum_b = BigInteger(0);
    sum_a._buffer.resize(len / 2, 0);
    sum_b._buffer.resize(len / 2, 0);

    for(int i = 0; i < len / 2; ++i){
        sum_a._buffer[i] = a0._buffer[i] + a1._buffer[i];
        sum_b._buffer[i] = b0._buffer[i] + b1._buffer[i];
    }

    BigInteger mul_0 = karatsuba_mul(a0, b0);
    BigInteger mul_1 = karatsuba_mul(a1, b1);

    mul_0._is_positive = true;
    mul_1._is_positive = true;

    BigInteger summs_mul = karatsuba_mul(sum_a, sum_b);

    for(int i = 0; i < len; ++i){
        summs_mul._buffer[i] -= (mul_0._buffer[i] + mul_1._buffer[i]);
    }

    for(int i = 0; i < len; ++i){
        res._buffer[i] = mul_0._buffer[i];
    }

    for(int i = 0; i < len; ++i){
        res._buffer[len + i] = mul_1._buffer[i];
    }

    for(int i = 0; i < len; ++i){
        res._buffer[len / 2 + i] += summs_mul._buffer[i];
    }

    return res;
}

BigInteger operator/(const BigInteger& a, const BigInteger& b){
    BigInteger res = a;
    return res /= b;
}

int BigInteger::simple_div(const BigInteger& a, const BigInteger& b){

    int res = 0;
    while(res * b <= a){
        ++res;
    }
    --res;

    return res;
}

void BigInteger::reverse(){
    for(int i = 0; i < (int)_buffer.size() / 2; ++i){
        std::swap(_buffer[i], _buffer[_buffer.size() - 1 - i]);
    }
}

BigInteger BigInteger::operator/=(const BigInteger& other){

    BigInteger a_pos = *this;
    BigInteger b_pos = other;
    a_pos._is_positive = true;
    b_pos._is_positive = true;

    zero_normalize();

    if(cmp_abs(a_pos, b_pos)){
        *this = BigInteger(0);
        return *this;
    }

    if(cmp_abs(other, 1)){
        *this = BigInteger(0);
        return *this;
    }

    if(other == 1){
        return *this;
    }

    if(other == -1){
        _is_positive = !_is_positive;
        return *this;
    }

    BigInteger res = BigInteger(0);
    res._buffer.resize(_buffer.size() + 1, 0);
    BigInteger transit = BigInteger(0);

    int pos = 0;

    while(pos <= (int)_buffer.size()){
        if(transit < b_pos){
            transit = transit * 10 + _buffer[_buffer.size() - 1 - pos];
            res._buffer[pos] = 0;
        } else {
            res._buffer[pos] = simple_div(transit, b_pos);
            transit -= b_pos * res._buffer[pos];

            if(pos < (int)_buffer.size()){
                transit = transit * 10 + _buffer[_buffer.size() - 1 - pos];
            }
        }
        ++pos;
    }

    res.reverse();
    res.crop();

    res._is_positive = (_is_positive == other._is_positive);

    *this = res;
    zero_normalize();
    return *this;
}

BigInteger operator%(const BigInteger& a, const BigInteger& b){
    BigInteger res = a;
    return res %= b;
}

BigInteger BigInteger::operator%=(const BigInteger& other){
    *this = *this - (*this / other) * other;
    return *this;
}

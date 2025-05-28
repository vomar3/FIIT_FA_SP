//
// Created by Des Caldnd on 5/27/2024.
//

#include "../include/big_int.h"
#include <ranges>
#include <exception>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>

unsigned long long BASE = 1ULL << (8 * sizeof(unsigned int));

bool is_zero(const std::vector<unsigned int, pp_allocator<unsigned int>> &digits) {
    return digits.size() == 1 && digits[0] == 0;
}

void removing_zeros(std::vector<unsigned int, pp_allocator<unsigned int>> &digits) {
    while (digits.size() > 1 && digits.back() == 0) {
        digits.pop_back();
    }
}

// strong_ordering : less, equal, greater
std::strong_ordering big_int::operator<=>(const big_int &other) const noexcept
{
    if (_sign != other._sign) {
        if (_sign) {
            return std::strong_ordering::greater;
        }

        return std::strong_ordering::less;
    }

    if (_digits.size() != other._digits.size()) {
        if (_sign) {
            return _digits.size() <=> other._digits.size();
        }

        return other._digits.size() <=> _digits.size();
    }

    for (int i = static_cast<int>(_digits.size()) - 1; i >= 0; --i) {
        if (_digits[i] != other._digits[i]) {
            if (_sign) {
                return _digits[i] <=> other._digits[i];
            }

            return other._digits[i] <=> _digits[i];
        }
    }

    return std::strong_ordering::equal;
}

big_int::operator bool() const noexcept
{
    return !is_zero(_digits);
}

big_int &big_int::operator++() &
{
    *this += big_int(1, _digits.get_allocator());
    return *this;
}

big_int big_int::operator++(int)
{
    auto tmp = *this;
    ++(*this);
    return tmp;
}

big_int &big_int::operator--() &
{
    *this -= big_int(1, _digits.get_allocator());
    return *this;
}

big_int big_int::operator--(int)
{
    auto tmp = *this;
    --(*this);
    return tmp;
}

big_int &big_int::operator+=(const big_int &other) &
{
    return plus_assign(other, 0);
}

big_int &big_int::operator-=(const big_int &other) &
{
    return minus_assign(other, 0);
}

big_int big_int::operator+(const big_int &other) const
{
    big_int tmp = *this;
    return tmp += other;
}

big_int big_int::operator-(const big_int &other) const
{
    big_int tmp = *this;
    return tmp -= other;
}

big_int big_int::operator*(const big_int &other) const
{
    big_int tmp = *this;
    return tmp *= other;
}

big_int big_int::operator/(const big_int &other) const
{
    big_int tmp = *this;
    return tmp /= other;
}

big_int big_int::operator%(const big_int &other) const
{
    big_int tmp = *this;
    return tmp %= other;
}

big_int big_int::operator&(const big_int &other) const
{
    big_int tmp = *this;
    return tmp &= other;
}

big_int big_int::operator|(const big_int &other) const
{
    big_int tmp = *this;
    return tmp |= other;
}

big_int big_int::operator^(const big_int &other) const
{
    big_int tmp = *this;
    return tmp ^= other;
}

big_int big_int::operator<<(size_t shift) const
{
    big_int tmp = *this;
    return tmp <<= shift;
}

big_int big_int::operator>>(size_t shift) const
{
    big_int tmp = *this;
    return tmp >>= shift;
}

big_int &big_int::operator%=(const big_int &other) &
{
    return modulo_assign(other, decide_div(other._digits.size()));
}

big_int big_int::operator~() const
{
    big_int result(*this);

    for (auto &digit: result._digits) {
        digit = ~digit;
    }

    removing_zeros(result._digits);
    return result;
}

big_int &big_int::operator&=(const big_int &other) &
{
    size_t max_size = std::max(_digits.size(), other._digits.size());
    _digits.resize(max_size, 0);

    for (size_t i = 0; i < max_size; ++i) {
        if (i < other._digits.size()) {
            _digits[i] &= other._digits[i];
        } else {
            _digits[i] = 0;
        }
    }

    removing_zeros(_digits);
    return *this;
}

big_int &big_int::operator|=(const big_int &other) &
{
    size_t max_size = std::max(_digits.size(), other._digits.size());
    _digits.resize(max_size, 0);

    for (size_t i = 0; i < max_size; ++i) {
        if (i < other._digits.size()) {
            _digits[i] |= other._digits[i];
        }
    }

    removing_zeros(_digits);
    return *this;
}

big_int &big_int::operator^=(const big_int &other) &
{
    size_t max_size = std::max(_digits.size(), other._digits.size());
    _digits.resize(max_size, 0);
    for (size_t i = 0; i < max_size; ++i) {
        if (i < other._digits.size()) {
            _digits[i] ^= other._digits[i];
        }
    }

    removing_zeros(_digits);
    return *this;
}

big_int &big_int::operator<<=(size_t shift) &
{
    if (shift == 0 || is_zero(_digits)) {
        return *this;
    }

    unsigned long long carry = 0;
    size_t total_bits = shift;

    for (unsigned int & _digit : _digits) {
        unsigned long long value = (static_cast<unsigned long long>(_digit) << total_bits) | carry;
        _digit = static_cast<unsigned int>(value % BASE);
        carry = value / BASE;
    }

    if (carry > 0) {
        _digits.push_back(static_cast<unsigned int>(carry));
    }

    removing_zeros(_digits);
    return *this;
}

big_int &big_int::operator>>=(size_t shift) &
{
    if (shift == 0 || is_zero(_digits)) {
        return *this;
    }

    size_t digit_bits = 8 * sizeof(unsigned int);

    if (shift >= digit_bits * _digits.size()) {
        _digits.clear();
        _digits.push_back(0);
        _sign = true;
        return *this;
    }

    size_t int_shift = shift / digit_bits;
    size_t bit_shift = shift % digit_bits;

    if (int_shift > 0) {
        _digits.erase(_digits.begin(), _digits.begin() + int_shift);
    }

    if (bit_shift > 0) {
        unsigned long long carry = 0;

        for (int i = static_cast<int>(_digits.size()) - 1; i >= 0; --i) {
            unsigned long long value = (carry << (8 * sizeof(unsigned int))) | _digits[i];
            _digits[i] = static_cast<unsigned int>(value >> bit_shift);
            carry = (value & ((1ULL << bit_shift) - 1));
        }
    }

    removing_zeros(_digits);

    if (is_zero(_digits)) {
        _sign = true;
    }

    return *this;
}

big_int &big_int::plus_assign(const big_int &other, size_t shift) &
{
    if (_sign != other._sign) {
        big_int temp(other);
        temp._sign = _sign;
        return minus_assign(temp, shift);
    }

    size_t max_size = std::max(_digits.size(), other._digits.size() + shift);
    _digits.resize(max_size, 0);

    uint16_t carry = 0;
    for (size_t i = 0; i < max_size; ++i) {
        uint32_t a = (i < _digits.size()) ? _digits[i] : 0;
        uint32_t b = (i >= shift && (i - shift) < other._digits.size())
                     ? other._digits[i - shift] : 0;

        Split32 sa = split(a);
        Split32 sb = split(b);

        uint32_t lo_sum = sa.lo + sb.lo + carry;
        carry = (lo_sum >> 16);

        uint32_t hi_sum = sa.hi + sb.hi + carry;
        carry = (hi_sum >> 16);

        _digits[i] = (hi_sum << 16) | (lo_sum & 0xFFFF);
    }

    if (carry > 0) {
        _digits.push_back(carry);
    }

    removing_zeros(_digits);

    if (is_zero(_digits)) {
        _sign = true;
    }
    return *this;
}

big_int &big_int::minus_assign(const big_int &other, size_t shift) &
{
    if (is_zero(other._digits)) {
        return *this;
    }

    if (_sign != other._sign) {
        big_int temp(other);
        temp._sign = _sign;
        return plus_assign(temp, shift);
    }

    big_int abs_this(*this);
    abs_this._sign = true;
    big_int abs_other(other);
    abs_other._sign = true;
    abs_other <<= shift;

    bool result_sign = _sign;
    if ((abs_this <=> abs_other) == std::strong_ordering::less) {
        result_sign = !result_sign;
        std::swap(abs_this._digits, abs_other._digits);
    }

    size_t max_size = std::max(abs_this._digits.size(), abs_other._digits.size());
    std::vector<unsigned int, pp_allocator<unsigned int>> result(max_size, 0, _digits.get_allocator());
    long long borrow = 0;

    for (size_t i = 0; i < max_size; ++i) {
        long long difference = borrow;

        if (i < abs_this._digits.size()) {
            difference += abs_this._digits[i];
        }

        if (i < abs_other._digits.size()) {
            difference -= abs_other._digits[i];
        }

        if (difference < 0) {
            difference += static_cast<long long>(BASE);
            borrow = -1;
        } else {
            borrow = 0;
        }

        result[i] = static_cast<unsigned int>(difference);
    }

    _digits = std::move(result);
    _sign = result_sign;
    removing_zeros(_digits);

    if (is_zero(_digits)) {
        _sign = true;
    }

    return *this;
}

big_int &big_int::operator*=(const big_int &other) &
{
    return multiply_assign(other, decide_mult((other._digits.size())));
}

big_int &big_int::operator/=(const big_int &other) &
{
    return divide_assign(other, decide_div(other._digits.size()));
}

std::string big_int::to_string() const
{
    if (is_zero(_digits)) {
        return "0";
    }

    std::string answer;
    big_int tmp = *this;
    tmp._sign = true;

    while (tmp) {
        auto value = tmp % 10;
        answer += static_cast<char> (value._digits[0] + '0');
        tmp /= 10;
    }

    if (!_sign) {
        answer += '-';
    }

    std::reverse(answer.begin(), answer.end());

    return answer;
}

std::ostream &operator<<(std::ostream &stream, const big_int &value)
{
    stream << value.to_string();
    return stream;
}

std::istream &operator>>(std::istream &stream, big_int &value)
{
    std::string val;
    stream >> val;
    value = big_int(val, 10, value._digits.get_allocator());
    return stream;
}

bool big_int::operator==(const big_int &other) const noexcept
{
    return std::strong_ordering::equal == (*this <=> other);
}

big_int::big_int(const std::vector<unsigned int, pp_allocator<unsigned int>> &digits, bool sign) : _digits(digits), _sign(sign)
{
    removing_zeros(_digits);

    if (_digits.empty()) {
        _digits.push_back(0);
        _sign = true;
    }
}

big_int::big_int(std::vector<unsigned int, pp_allocator<unsigned int>> &&digits, bool sign) noexcept : _digits(std::move(digits)), _sign(sign)
{
    removing_zeros(_digits);

    if (_digits.empty()) {
        _digits.push_back(0);
        _sign = true;
    }
}

big_int::big_int(const std::string &num, unsigned int radix, pp_allocator<unsigned int> allocator) : _sign(true), _digits(allocator)
{
    if (radix < 2 || radix > 36) {
        throw std::invalid_argument("Radix must be between 2 and 36");
    }

    if (num.empty()) {
        _digits.push_back(0);
        return;
    }

    std::string num1 = num;
    bool is_neg = false;
    if (num[0] == '-') {
        is_neg = true;
        num1 = num1.substr(1);
    } else if (num1[0] == '+') {
        num1 = num1.substr(1);
    }

    while (num1.size() > 1 && num1[0] == '0') {
        num1 = num1.substr(1);
    }

    if (num1.empty()) {
        _digits.push_back(0);
        return;
    }

    _digits.push_back(0);
    for (char c : num1) {
        unsigned int digit;

        if (c >= '0' && c <= '9') {
            digit = c - '0';
        } else if (c >= 'A' && c <= 'Z') {
            digit = 10 + (c - 'A');
        } else if (c >= 'a' && c <= 'z') {
            digit = 10 + (c - 'a');
        } else {
            throw std::invalid_argument("Invalid character in number string");
        }

        if (digit >= radix) {
            throw std::invalid_argument("Digit out of range for given radix");
        }

        *this *= radix;
        *this += big_int(static_cast<long long>(digit), allocator);
    }

    _sign = !is_neg;
    if (is_zero(_digits)) {
        _sign = true;
    }
}

big_int::big_int(pp_allocator<unsigned int> allocator) : _sign(true), _digits(allocator)
{
    _digits.push_back(0);
}

big_int &big_int::multiply_assign(const big_int &other, big_int::multiplication_rule rule) &
{
    if (is_zero(_digits)) {
        return *this;
    }

    if (is_zero(other._digits)) {
        _digits = {0};
        _sign = true;
        return *this;
    }

    if (rule == multiplication_rule::Karatsuba) {
        big_int result = multiply_karatsuba(*this, other);
        _digits = std::move(result._digits);
        _sign = (_sign == other._sign);
        removing_zeros(_digits);
        return *this;
    }

    big_int result(_digits.get_allocator());
    result._digits.resize(_digits.size() + other._digits.size(), 0);

    constexpr size_t shift = sizeof(unsigned int) * 4;
    constexpr unsigned int half_mask = (1u << shift) - 1;

    for (size_t i = 0; i < _digits.size(); ++i) {
        for (size_t j = 0; j < other._digits.size(); ++j) {
            const unsigned int a_low = _digits[i] & half_mask;
            const unsigned int a_high = _digits[i] >> shift;
            const unsigned int b_low = other._digits[j] & half_mask;
            const unsigned int b_high = other._digits[j] >> shift;

            const unsigned int a_low_b_low = a_low * b_low;
            const unsigned int a_low_b_high = a_low * b_high;
            const unsigned int a_high_b_low = a_high * b_low;
            const unsigned int a_high_b_high = a_high * b_high;

            const size_t pos = i + j;
            result.plus_assign(a_low_b_low, pos);
            result.plus_assign(a_low_b_high << shift, pos);
            result.plus_assign(a_low_b_high >> shift, pos + 1);
            result.plus_assign(a_high_b_low << shift, pos);
            result.plus_assign(a_high_b_low >> shift, pos + 1);
            result.plus_assign(a_high_b_high, pos + 1);
        }
    }

    _sign = (_sign == other._sign);
    _digits = std::move(result._digits);
    removing_zeros(_digits);
    return *this;
}

big_int &big_int::divide_assign(const big_int &other, big_int::division_rule rule) &
{
    if (is_zero(_digits)) {
        return *this;
    }

    if (is_zero(other._digits)) {
        throw std::invalid_argument("Division by zero");
    }

    big_int abs_this(*this);
    abs_this._sign = true;
    big_int abs_other(other);
    abs_other._sign = true;

    if (abs_this < abs_other) {
        _digits.clear();
        _digits.push_back(0);
        _sign = true;
        return *this;
    }

    std::vector<unsigned int, pp_allocator<unsigned int>> quotient(_digits.size(), 0, _digits.get_allocator());
    big_int remain(_digits.get_allocator());
    remain._digits.clear();
    remain._digits.push_back(0);

    for (int i = static_cast<int>(_digits.size()) - 1; i >= 0; i--) {
        remain._digits.insert(remain._digits.begin(), _digits[i]);
        while (remain._digits.size() > 1 && remain._digits.back() == 0) {
            remain._digits.pop_back();
        }

        if (remain._digits.empty()) {
            remain._sign = true;
        }

        unsigned long long left = 0, q = 0, right = BASE;
        while (left <= right) {
            unsigned long long mid = left + (right - left) / 2;
            big_int temp = abs_other * big_int(static_cast<long long>(mid), _digits.get_allocator());
            if (remain >= temp) {
                q = mid;
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }

        if (q > 0) {
            big_int temp = abs_other * big_int(static_cast<long long>(q), _digits.get_allocator());
            remain -= temp;
        }

        quotient[i] = static_cast<unsigned int>(q);
    }

    if (_sign != other._sign) {
        _sign = false;
    } else {
        _sign = true;
    }

    _digits = std::move(quotient);
    removing_zeros(_digits);
    return *this;
}

big_int &big_int::modulo_assign(const big_int &other, big_int::division_rule rule) &
{
    if (is_zero(_digits)) {
        return *this;
    }

    if (is_zero(other._digits)) {
        throw std::invalid_argument("Division by zero");
    }

    big_int abs_this(*this);
    abs_this._sign = true;
    big_int abs_other(other);
    abs_other._sign = true;

    if (abs_this < abs_other) {
        _sign = true;
        return *this;
    }

    big_int remain(_digits.get_allocator());
    remain._digits.clear();
    remain._digits.push_back(0);

    for (int i = static_cast<int>(_digits.size()) - 1; i >= 0; i--) {
        remain._digits.insert(remain._digits.begin(), _digits[i]);
        while (remain._digits.size() > 1 && remain._digits.back() == 0) {
            remain._digits.pop_back();
        }

        if (remain._digits.empty()) {
            remain._sign = true;
        }

        unsigned long long left = 0, q = 0, right = BASE;
        while (left <= right) {
            unsigned long long mid = left + (right - left) / 2;
            big_int temp = abs_other * big_int(static_cast<long long>(mid), _digits.get_allocator());
            if (remain >= temp) {
                q = mid;
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }

        if (q > 0) {
            big_int temp = abs_other * big_int(static_cast<long long>(q), _digits.get_allocator());
            remain -= temp;
        }
    }

    _digits = std::move(remain._digits);
    _sign = true;
    removing_zeros(_digits);
    return *this;
}

big_int::multiplication_rule big_int::decide_mult(size_t rhs) const noexcept {
    if (rhs > karatsuba) {
        return multiplication_rule::Karatsuba;
    }

    return multiplication_rule::trivial;
}

big_int::division_rule big_int::decide_div(size_t rhs) const noexcept {
    return division_rule::trivial;
}

big_int operator""_bi(unsigned long long n)
{
    return {n};
}

big_int multiply_karatsuba(const big_int &a, const big_int &b) {
    if (a._digits.size() < karatsuba || b._digits.size() < karatsuba) {
        big_int result = a;
        result.multiply_assign(b, big_int::multiplication_rule::trivial);
        return result;
    }

    size_t max_half_size = std::max(a._digits.size(), b._digits.size()) / 2;

    big_int low1(a._digits.get_allocator()), high1(a._digits.get_allocator());
    big_int low2(b._digits.get_allocator()), high2(b._digits.get_allocator());

    low1._digits.assign(a._digits.begin(), a._digits.begin() + static_cast<long long>(std::min(max_half_size, a._digits.size())));
    if (a._digits.size() > max_half_size) {
        high1._digits.assign(a._digits.begin() + static_cast<long long>(max_half_size), a._digits.end());
    }

    low2._digits.assign(b._digits.begin(), b._digits.begin() + static_cast<long long>(std::min(max_half_size, b._digits.size())));
    if (b._digits.size() > max_half_size) {
        high2._digits.assign(b._digits.begin() + static_cast<long long>(max_half_size), b._digits.end());
    }

    low1._sign = high1._sign = low2._sign = high2._sign = true;

    big_int z0 = multiply_karatsuba(low1, low2);
    big_int z2 = multiply_karatsuba(high1, high2);

    big_int sum1 = low1;
    sum1.plus_assign(high1);

    big_int sum2 = low2;
    sum2.plus_assign(high2);

    big_int z1 = multiply_karatsuba(sum1, sum2);
    z1.minus_assign(z0);
    z1.minus_assign(z2);

    z2 <<= max_half_size * 2 * 32;
    z1 <<= max_half_size * 32;

    z2.plus_assign(z1);
    z2.plus_assign(z0);

    if (a._sign == b._sign) {
        z2._sign = true;
    } else {
        z2._sign = false;
    }

    removing_zeros(z2._digits);
    return z2;
}
#pragma once

#include <iostream>
#include <numeric>
#include <compare>
#include <stdexcept>
#include <cstdint>

class Rational {
public:
    // Constructors
    Rational() : numerator_(0), denominator_(1) {}
    Rational(int numerator) : numerator_(numerator), denominator_(1) {}
    Rational(int numerator, int denominator) : numerator_(numerator), denominator_(denominator) {
        if (denominator_ == 0) {
            throw std::invalid_argument("Denominator cannot be zero");
        }
        Normalize();
    }
    Rational(const Rational& other) = default;
    Rational& operator=(const Rational& other) = default;

    // Accessors
    int GetNumerator() const { return numerator_; }
    int GetDenominator() const { return denominator_; }

    // Inversion
    Rational Inv() const {
        if (numerator_ == 0) {
            throw std::logic_error("Cannot invert zero");
        }
        return Rational(denominator_, numerator_);
    }

    // Unary
    Rational operator+() const { return *this; }
    Rational operator-() const { return Rational(-numerator_, denominator_); }

    // Arithmetic
    friend Rational operator+(const Rational& lhs, const Rational& rhs) {
        return Rational(lhs.numerator_ * rhs.denominator_ + rhs.numerator_ * lhs.denominator_,
                        lhs.denominator_ * rhs.denominator_);
    }
    friend Rational operator-(const Rational& lhs, const Rational& rhs) {
        return Rational(lhs.numerator_ * rhs.denominator_ - rhs.numerator_ * lhs.denominator_,
                        lhs.denominator_ * rhs.denominator_);
    }
    friend Rational operator*(const Rational& lhs, const Rational& rhs) {
        return Rational(lhs.numerator_ * rhs.numerator_,
                        lhs.denominator_ * rhs.denominator_);
    }
    friend Rational operator/(const Rational& lhs, const Rational& rhs) {
        if (rhs.numerator_ == 0) {
            throw std::domain_error("Division by zero");
        }
        return Rational(lhs.numerator_ * rhs.denominator_,
                        lhs.denominator_ * rhs.numerator_);
    }

    // Compound assignment
    Rational& operator+=(const Rational& other) {
        *this = *this + other;
        return *this;
    }
    Rational& operator-=(const Rational& other) {
        *this = *this - other;
        return *this;
    }
    Rational& operator*=(const Rational& other) {
        *this = *this * other;
        return *this;
    }
    Rational& operator/=(const Rational& other) {
        *this = *this / other;
        return *this;
    }

    // Comparison
    friend bool operator==(const Rational& lhs, const Rational& rhs) noexcept {
        return lhs.numerator_ == rhs.numerator_ && lhs.denominator_ == rhs.denominator_;
    }
    friend auto operator<=>(const Rational& lhs, const Rational& rhs) noexcept {
        // compare cross-products safely in 64-bit
        return (static_cast<std::int64_t>(lhs.numerator_) * rhs.denominator_)
             <=> (static_cast<std::int64_t>(rhs.numerator_) * lhs.denominator_);
    }

    // Stream IO
    friend std::ostream& operator<<(std::ostream& os, const Rational& r) {
        if (r.denominator_ == 1) {
            os << r.numerator_;
        } else {
            os << r.numerator_ << " / " << r.denominator_;
        }
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Rational& r) {
        int num{}, den{1};
        char sep{};
        if (!(is >> num)) {
            return is;
        }
        is >> std::ws;
        if (is.peek() == '/') {
            is >> sep >> den;
            if (!is || den == 0) {
                is.setstate(std::ios::failbit);
                return is;
            }
        }
        r = Rational(num, den);
        return is;
    }

private:
    int numerator_;
    int denominator_;

    void Normalize() {
        // Move sign to numerator and reduce
        if (denominator_ < 0) {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }
        int g = std::gcd(std::abs(numerator_), denominator_);
        numerator_ /= g;
        denominator_ /= g;
    }
};

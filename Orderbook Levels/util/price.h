#pragma once

#include <cstdint>
#include <cmath>
#include <limits>

template <size_t Decimals>
class ScaledInteger {
public:
    constexpr explicit ScaledInteger(double v) 
        : t(v * pow(10, Decimals) + (-2 * std::signbit(v) + 1)* 0.5) {
     }

    explicit operator double() const {
         return 1.0 / pow(10, Decimals) * t; 
    }

    static constexpr double max() {
        return std::numeric_limits<int64_t>::max() / pow(10, Decimals);
    }
    static constexpr double min() {
        return std::numeric_limits<int64_t>::min() / pow(10, Decimals);
    }

    bool operator==(const ScaledInteger& other) const { return t == other.t; }

    bool operator<(const ScaledInteger& other) const { return t < other.t; }

    bool operator<=(const ScaledInteger& other) const { return t <= other.t; }

    bool operator>(const ScaledInteger& other) const { return t > other.t; }

    bool operator>=(const ScaledInteger& other) const { return t >= other.t; }

    int64_t t;

private:
    static constexpr int64_t pow(int64_t v, int e) {
        return (e == 0) ? 1 : v * pow(v, e - 1);
    }
};

using Price = ScaledInteger<5>;




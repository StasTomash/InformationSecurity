#ifndef BIG_INTEGER_BIG_INTEGER_H
#define BIG_INTEGER_BIG_INTEGER_H

#include <vector>
#include <iostream>
#include <complex>

namespace BigInt {

    enum BigIntegerSign {
        MINUS,
        PLUS
    };

    typedef std::complex<long double> fft_base;

    void fft(std::vector<fft_base> &arg, bool invert = false);

    struct InvalidBigIntegerStringException : public std::exception {
        [[nodiscard]] const char *what() const noexcept override {
            return "Invalid string value for BigInteger cast";
        }
    };
    struct InvalidConsoleArguments : public std::exception {
        [[nodiscard]] const char* what () const noexcept override {
            return "Invalid arguments. See \'help\' command for reference";
        }
    };

    class BigInteger {
        protected:
        static const int CELL_LENGTH = 4;
        static const int MODULO = 10000;

        BigIntegerSign sign;
        std::vector<int> data;

        void cleanup_zeroes();
        [[nodiscard]] BigInteger scale(int n) const;
        [[nodiscard]] BigInteger scalar_mult(int n) const;
        static BigInteger getIntOfLen(int len);
        friend std::ostream &operator<<(std::ostream &os, const BigInteger &rhs);
        friend std::istream &operator>>(std::istream &is, BigInteger &rhs);

        public:
        explicit BigInteger(int x);
        explicit BigInteger(const std::string &s);
        [[nodiscard]] int toInt() const;
        [[nodiscard]] std::string toString() const;
        [[nodiscard]] bool isOdd() const;
        BigInteger(const BigInteger &) = default;
        BigInteger();
        [[nodiscard]] BigInteger abs() const;
        static BigInteger rand(const BigInteger& maxVal);
        void inverse_sign();

        bool operator==(const BigInteger &rhs) const;
        bool operator!=(const BigInteger &rhs) const;
        bool operator<(const BigInteger &rhs) const;
        bool operator<=(const BigInteger &rhs) const;
        bool operator>(const BigInteger &rhs) const;
        bool operator>=(const BigInteger &rhs) const;

        BigInteger operator+() const;
        BigInteger operator-() const;
        BigInteger operator+(const BigInteger &rhs) const;
        BigInteger operator-(const BigInteger &rhs) const;
        BigInteger operator*(const BigInteger &rhs) const;
        BigInteger operator/(const BigInteger &rhs) const;
        BigInteger operator%(const BigInteger &rhs) const;
        BigInteger &operator*=(const BigInteger &rhs);
        BigInteger &operator+=(const BigInteger &rhs);
        BigInteger &operator-=(const BigInteger &rhs);
        BigInteger &operator/=(const BigInteger &rhs);
        BigInteger &operator%=(const BigInteger &rhs);
        const BigInteger operator++(int);

        [[nodiscard]] BigInteger pow(BigInteger exp, const BigInteger &modulo = BigInteger(0)) const;
        [[nodiscard]] BigInteger sqrt() const;

        static BigInteger getRandOfLen(int len);
        static BigInteger getRandOfBitLen(int len);

        [[nodiscard]] std::string convertToBase2() const;
        [[nodiscard]] std::string convertToBase8() const;
        [[nodiscard]] std::string convertToBase10() const;
        [[nodiscard]] std::string convertToBase64() const;
    };

    std::ostream &operator<<(std::ostream &os, const BigInteger &rhs);
    std::istream &operator>>(std::istream &is, BigInteger &rhs);
}

#endif //BIG_INTEGER_BIG_INTEGER_H
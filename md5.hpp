/**
 * @file md5.hpp
 * @author André Lucas Maegima
 * @brief Hash class using md5 algorithm.
 * @version 1.1
 * @date 2024-03-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _MD5_HPP_
#define _MD5_HPP_

#include <sstream>
#include <fstream>

using namespace std;

struct Digest {
    unsigned int A, B, C, D;
    Digest(unsigned int A, unsigned int B, unsigned int C, unsigned D) : A(A), B(B), C(C), D(D) {};
    Digest operator+=(const Digest &rhs) {
        this->A += rhs.A;
        this->B += rhs.B;
        this->C += rhs.C;
        this->D += rhs.D;
        return *this;
    }
     operator std::string() const { 
        char str[33];
        unsigned char *p = (unsigned char*) this;
        sprintf(str, "%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x", 
            p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7],
            p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15]
        );
        return str;
    }
};

class Hash {
private:
    /**
     * @brief Shift values for md5 steps.
     * 
     */
    constexpr static inline unsigned int shift[] = { 
        7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
        5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
        4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
        6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
    };
    /**
     * @brief Precomputed binary integer part of the sines of integers (Radians).
     * 
     */
    constexpr static inline unsigned int sequence[] = { 
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 
    };
    /**
     * @brief Copy n bytes from source buffer to destination buffer. 
     * 
     * @param dst Destination buffer.
     * @param src Source buffer.
     * @param n Number of bytes to copy.
     */
    void bffcpy(void *dst, void *src, size_t n);
    /**
     * @brief Return Size for zero termined char buffer.
     * 
     * @param bff Char buffer.
     * @return size_t Buffer size.
     */
    size_t strsize(const char *bff);
    /**
     * @brief Write n zeros bytes to buffer.
     * 
     * @param bff Buffer to write zeros.
     * @param n Number of bytes to write zero.
     */
    void bffzero(char *bff, size_t n);
    /**
     * @brief Move all bits in x to the left c times, with the leftmost (most significant bit) becoming the rightmost.
     * 
     * @param x Input to rotate bits left.
     * @param c How many bits to move.
     * @return unsigned int Leftrotated input c times.
     */
    unsigned int leftrotate(unsigned int x, unsigned int c);
    /**
     * @brief Run md5 step for 64 bytes sized buffer input.
     * 
     * @param wd 64 bytes to apply md5 step.
     * @param digest initial Digest for step.
     * @return Digest Step result.
     */
    Digest md5step(unsigned int *wd, const Digest &digest);
public:
    /**
     * @brief Construct a new Hash object
     * 
     */
    Hash();

    Digest md5(std::stringstream* stream);

    Digest md5(const char *msg);

    Digest md5(std::fstream *msg, u_int64_t size);
};

#endif
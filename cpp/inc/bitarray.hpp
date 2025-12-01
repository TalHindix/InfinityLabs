/*****************************************************************************
 * Exercise:    BitArray Ex
 * Date:        27/11/2025
 * Developer:   Tal Hindi
 * Reviewer:    Baruchi
 * Status:      Approved
 *****************************************************************************/

#ifndef __ILRD_BITARRAY_
#define __ILRD_BITARRAY_

#include <cstddef>      // std::size_t
#include <climits>      // CHAR_BIT
#include <string>       // std::string
#include <algorithm>    // std::transform
#include <numeric>      // std::accumulate
#include <stdexcept>    // std::out_of_range

namespace ilrd
{

template <std::size_t SIZE>
class BitArray
{
private:
    class BitProxy;

public:
    explicit BitArray();

    bool operator[](std::size_t idx) const;
    BitProxy operator[](std::size_t idx);

    BitArray& operator|=(const BitArray& other);
    BitArray& operator&=(const BitArray& other);
    BitArray& operator^=(const BitArray& other);

    bool operator==(const BitArray& other) const;
    bool operator!=(const BitArray& other) const;

    void Set(std::size_t idx, bool val);
    void Set(bool val);
    void Flip(std::size_t idx);
    void Flip();

    bool Get(std::size_t idx) const;
    std::size_t Count() const;
    std::string ToString() const;

private:
    static const std::size_t BITS_PER_WORD = CHAR_BIT * sizeof(std::size_t);
    static const std::size_t NUM_WORDS = (SIZE + BITS_PER_WORD - 1) / BITS_PER_WORD;
    static const std::size_t LAST_WORD_MASK =
        (SIZE % BITS_PER_WORD) ?
        ((std::size_t(1) << (SIZE % BITS_PER_WORD)) - std::size_t(1)) :
        ~std::size_t(0);

    std::size_t m_bits[NUM_WORDS];

    std::size_t GetWordIndex(std::size_t bitIdx) const;
    std::size_t GetBitOffset(std::size_t bitIdx) const;
    std::size_t CreateMask(std::size_t bitIdx) const;
    void ValidateIndex(std::size_t idx) const;
    void ClearExtraBits();

    bool GetBitUnchecked(std::size_t idx) const;
    void SetBitUnchecked(std::size_t idx, bool val);

    static std::size_t CountBitsInWord(std::size_t word);

    class BitProxy
    {
    public:
        BitProxy(BitArray& bitArr, std::size_t idx);

        BitProxy& operator=(bool val);
        BitProxy& operator=(const BitProxy& other);
        BitProxy& Flip();
        operator bool() const;

    private:
        BitArray& m_arr;
        std::size_t m_idx;
    };
};

/*****************************************************************************
 *                          Implementation
 *****************************************************************************/

template<std::size_t SIZE>
BitArray<SIZE>::BitArray() : m_bits()
{
    std::fill(m_bits, m_bits + NUM_WORDS, std::size_t(0));
}

template<std::size_t SIZE>
std::size_t BitArray<SIZE>::GetWordIndex(std::size_t bitIdx) const
{
    return bitIdx / BITS_PER_WORD;
}

template<std::size_t SIZE>
std::size_t BitArray<SIZE>::GetBitOffset(std::size_t bitIdx) const
{
    return bitIdx % BITS_PER_WORD;
}

template<std::size_t SIZE>
std::size_t BitArray<SIZE>::CreateMask(std::size_t bitIdx) const
{
    return std::size_t(1) << GetBitOffset(bitIdx);
}

template<std::size_t SIZE>
void BitArray<SIZE>::ValidateIndex(std::size_t idx) const
{
    if (idx >= SIZE)
    {
        throw std::out_of_range("BitArray index out of range");
    }
}

template<std::size_t SIZE>
void BitArray<SIZE>::ClearExtraBits()
{
    if (NUM_WORDS > std::size_t(0))
    {
        m_bits[NUM_WORDS - 1] &= LAST_WORD_MASK;
    }
}

template<std::size_t SIZE>
bool BitArray<SIZE>::GetBitUnchecked(std::size_t idx) const
{
    return (m_bits[GetWordIndex(idx)] & CreateMask(idx)) != std::size_t(0);
}

template<std::size_t SIZE>
void BitArray<SIZE>::SetBitUnchecked(std::size_t idx, bool val)
{
    std::size_t wordIdx = GetWordIndex(idx);
    std::size_t mask = CreateMask(idx);

    if (val)
    {
        m_bits[wordIdx] |= mask;
    }
    else
    {
        m_bits[wordIdx] &= ~mask;
    }
}

template<std::size_t SIZE>
std::size_t BitArray<SIZE>::CountBitsInWord(std::size_t word)
{
    word = (word & 0x5555555555555555UL) +
           ((word >> 1) & 0x5555555555555555UL);
    word = (word & 0x3333333333333333UL) +
           ((word >> 2) & 0x3333333333333333UL);
    word = (word & 0x0f0f0f0f0f0f0f0fUL) +
           ((word >> 4) & 0x0f0f0f0f0f0f0f0fUL);
    word = (word & 0x00ff00ff00ff00ffUL) +
           ((word >> 8) & 0x00ff00ff00ff00ffUL);
    word = (word & 0x0000ffff0000ffffUL) +
           ((word >> 16) & 0x0000ffff0000ffffUL);
    word = (word & 0x00000000ffffffffUL) +
           ((word >> 32) & 0x00000000ffffffffUL);

    return word;
}

template<std::size_t SIZE>
bool BitArray<SIZE>::Get(std::size_t idx) const
{
    ValidateIndex(idx);
    return GetBitUnchecked(idx);
}

template<std::size_t SIZE>
void BitArray<SIZE>::Set(std::size_t idx, bool val)
{
    ValidateIndex(idx);
    SetBitUnchecked(idx, val);
}

template<std::size_t SIZE>
void BitArray<SIZE>::Set(bool val)
{
    std::size_t fillValue = val ? ~std::size_t(0) : std::size_t(0);
    std::fill(m_bits, m_bits + NUM_WORDS, fillValue);
    ClearExtraBits();
}

template<std::size_t SIZE>
void BitArray<SIZE>::Flip(std::size_t idx)
{
    ValidateIndex(idx);
    m_bits[GetWordIndex(idx)] ^= CreateMask(idx);
}

template<std::size_t SIZE>
void BitArray<SIZE>::Flip()
{
    for (std::size_t i = std::size_t(0); i < NUM_WORDS; ++i)
    {
        m_bits[i] = ~m_bits[i];
    }
    ClearExtraBits();
}

template<std::size_t SIZE>
std::size_t BitArray<SIZE>::Count() const
{
    std::size_t total = std::size_t(0);

    for (std::size_t i = std::size_t(0); i < NUM_WORDS; ++i)
    {
        total += CountBitsInWord(m_bits[i]);
    }

    return total;
}

template<std::size_t SIZE>
std::string BitArray<SIZE>::ToString() const
{
    std::string result;
    result.reserve(SIZE);

    for (std::size_t i = SIZE; i > std::size_t(0); --i)
    {
        result += GetBitUnchecked(i - std::size_t(1)) ? '1' : '0';
    }

    return result;
}

template<std::size_t SIZE>
bool BitArray<SIZE>::operator[](std::size_t idx) const
{
    return Get(idx);
}

template<std::size_t SIZE>
typename BitArray<SIZE>::BitProxy
BitArray<SIZE>::operator[](std::size_t idx)
{
    ValidateIndex(idx);
    return BitProxy(*this, idx);
}

template<std::size_t SIZE>
BitArray<SIZE>&
BitArray<SIZE>::operator|=(const BitArray& other)
{
    for (std::size_t i = std::size_t(0); i < NUM_WORDS; ++i)
    {
        m_bits[i] |= other.m_bits[i];
    }
    ClearExtraBits();
    return *this;
}

template<std::size_t SIZE>
BitArray<SIZE>&
BitArray<SIZE>::operator&=(const BitArray& other)
{
    for (std::size_t i = std::size_t(0); i < NUM_WORDS; ++i)
    {
        m_bits[i] &= other.m_bits[i];
    }
    ClearExtraBits();
    return *this;
}

template<std::size_t SIZE>
BitArray<SIZE>&
BitArray<SIZE>::operator^=(const BitArray& other)
{
    for (std::size_t i = std::size_t(0); i < NUM_WORDS; ++i)
    {
        m_bits[i] ^= other.m_bits[i];
    }
    ClearExtraBits();
    return *this;
}

template<std::size_t SIZE>
bool BitArray<SIZE>::operator==(const BitArray& other) const
{
    return std::equal(m_bits, m_bits + NUM_WORDS, other.m_bits);
}

template<std::size_t SIZE>
bool BitArray<SIZE>::operator!=(const BitArray& other) const
{
    return !(*this == other);
}

/*****************************************************************************
 *                          BitProxy Implementation
 *****************************************************************************/

template<std::size_t SIZE>
BitArray<SIZE>::BitProxy::BitProxy(BitArray& bitArr, std::size_t idx)
    : m_arr(bitArr), m_idx(idx)
{
}

template<std::size_t SIZE>
BitArray<SIZE>::BitProxy::operator bool() const
{
    return m_arr.GetBitUnchecked(m_idx);
}

template<std::size_t SIZE>
typename BitArray<SIZE>::BitProxy&
BitArray<SIZE>::BitProxy::operator=(bool val)
{
    m_arr.SetBitUnchecked(m_idx, val);
    return *this;
}

template<std::size_t SIZE>
typename BitArray<SIZE>::BitProxy&
BitArray<SIZE>::BitProxy::operator=(const BitProxy& other)
{
    bool otherVal = other;
    return *this = otherVal;
}

template<std::size_t SIZE>
typename BitArray<SIZE>::BitProxy&
BitArray<SIZE>::BitProxy::Flip()
{
    m_arr.Flip(m_idx);
    return *this;
}

} // namespace ilrd

#endif // __ILRD_BITARRAY_

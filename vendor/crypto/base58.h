// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

// modified for sumus

/**
 * Why base-58 instead of standard base-64 encoding?
 * - Don't want 0OIl characters that look the same in some fonts and
 *      could be used to create visually identical looking data.
 * - A string with non-alphanumeric characters is not as easily accepted as input.
 * - E-mail usually won't line-break if there's no punctuation to break at.
 * - Double-clicking selects the whole string as one word if it's all alphanumeric.
 */
#ifndef BITCOIN_BASE58_H
#define BITCOIN_BASE58_H

#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <sstream>

#include "milecsa_utils.hpp"

using std::string;
using std::vector;
using std::array;

uint_least32_t Crc32(unsigned char *buf, size_t len);
bool HexStringToUchar(const string& hexString, unsigned char& value);

template<size_t N>
string BinToHex(const std::array<unsigned char, N>& data)
{
    string s;
    for (size_t i = 0; i < N; i++)
    {
        s += StringFormat("%02x", data[i]);
    }
    return s;
}

bool HexToBin(const string& hexString, vector<unsigned char>& data, string& errorDescription);

string BinToHex(const vector<unsigned char>& data);
/**
 * Encode a byte sequence as a base58-encoded string.
 * pbegin and pend cannot be nullptr, unless both are.
 */
string EncodeBase58(const unsigned char* pbegin, const unsigned char* pend);

/**
 * Encode a byte vector as a base58-encoded string
 */
string EncodeBase58(const vector<unsigned char>& vch);

/**
 * Encode a byte array as a base58-encoded string
 */
template<size_t N>
string EncodeBase58(const array<unsigned char, N>& arr)
{
    return EncodeBase58(arr.data(), arr.data() + N);
}

/**
 * Decode a base58-encoded string (psz) into a byte vector (vchRet).
 * return true if decoding is successful.
 * psz cannot be nullptr.
 */
bool DecodeBase58(const char* psz, vector<unsigned char>& vchRet);

/**
 * Decode a base58-encoded string (str) into a byte vector (vchRet).
 * return true if decoding is successful.
 */
bool DecodeBase58(const string& str, vector<unsigned char>& vchRet);

/**
 * Decode a base58-encoded string (str) into a byte array (arrRet).
 * return true if decoding is successful.
 */
template<size_t N>
bool DecodeBase58(const string& str, array<unsigned char, N>& arrRet, string& errorDescription)
{
    errorDescription = "";

    vector<unsigned char> v;

    if (!DecodeBase58(str.c_str(), v))
    {
        errorDescription = "decode error";
        return false;
    }

    if (v.size() != N)
    {
        std::stringstream errorMessage;
        errorMessage << "size of decoded vector is not equal to expected size: " << v.size() << " <> " << N;
        errorDescription = errorMessage.str();
        return false;
    }

    std::copy_n(v.begin(), N, arrRet.begin());

    return true;
}

/**
 * Encode a byte vector into a base58-encoded string, including checksum
 */
string EncodeBase58Check(const vector<unsigned char>& vchIn);

/**
 * Encode a byte array into a base58-encoded string, including checksum
 */
template<size_t N>
string EncodeBase58Check(const array<unsigned char, N>& arr)
{
    // add 4-byte hash check to the end
    vector<unsigned char> vch;
    vch.assign(arr.begin(), arr.end());

    // uint256 hash = Hash(vch.begin(), vch.end());
    // vch.insert(vch.end(), (unsigned char*)&hash, (unsigned char*)&hash + 4);

    uint_least32_t crc32 = Crc32(&vch[0], vch.size());
    // little endian
    vch.push_back(static_cast<unsigned char>(crc32 & 0xff));
    vch.push_back(static_cast<unsigned char>((crc32 >> 8) & 0xff));
    vch.push_back(static_cast<unsigned char>((crc32 >> 16) & 0xff));
    vch.push_back(static_cast<unsigned char>((crc32 >> 24) & 0xff));

    return EncodeBase58(vch);
}

/**
 * Decode a base58-encoded string (psz) that includes a checksum into a byte
 * vector (vchRet), return true if decoding is successful
 */
bool DecodeBase58Check(const char* psz, vector<unsigned char>& vchRet);

/**
 * Decode a base58-encoded string (str) that includes a checksum into a byte
 * vector (vchRet), return true if decoding is successful
 */
bool DecodeBase58Check(const string& str, vector<unsigned char>& vchRet);

/**
 * Decode a base58-encoded string (str) that includes a checksum into a byte array (arrRet).
 * return true if decoding is successful.
 */
template<size_t N>
bool DecodeBase58Check(const string& str, array<unsigned char, N>& arrRet, string& errorDescription)
{
    errorDescription = "";

    vector<unsigned char> v;

    if (!DecodeBase58Check(str.c_str(), v))
    {
        errorDescription = "base58 check string decode error";
        return false;
    }

    if (v.size() != N)
    {
        std::stringstream errorMessage;
        errorMessage << "size of decoded vector is not equal to expected size: " << v.size() << " <> " << N;
        errorDescription = errorMessage.str();
        return false;
    }

    std::copy_n(v.begin(), N, arrRet.begin());

    return true;
}

#endif // BITCOIN_BASE58_H

#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "crypto_types.h"
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
typedef boost::multiprecision::uint256_t uint256_t;

string UInt256ToDecString(const uint256_t& ui)
{
    std::stringstream ss;
    ss << std::dec << ui;
    return ss.str();
}

string UInt256ToHexString(const uint256_t& ui)
{
    std::stringstream ss;
    ss << std::hex << ui;
    return ss.str();
}

bool StringToUInt256(const string& s, uint256_t& ui, bool hexString)
{
    std::stringstream ss;

    try
    {
        // note: boost 1.53 in CentOS doesn't support stream modifiers like std::hex for multiprecision types
        // but prefix '0x' for hex numbers supported in boost 1.53 and in boost 1.64 as well
        // ss << (hexString ? std::hex : std::dec) << s;
        // ss >> (hexString ? std::hex : std::dec) >> ui;
        ss << (hexString ? "0x" : "") << s;
        ss >> (hexString ? std::hex : std::dec) >> ui;
    }
    catch (const boost::exception& e)
    {
        return false;
    }
    // check that all symbols are processed e.g. 0xfz -> error
    if (ss.fail() || !ss.eof())
    {
        return false;
    }

    return true;
}

void UInt256ToVector(const uint256_t& ui, vector<unsigned char>& v, bool littleEndian)
{
    v.clear();
    v.reserve(sizeof(ui));
 
    size_t size = ui.backend().size();
    const boost::multiprecision::limb_type* p = ui.backend().limbs();
    size_t limbTypeSize = sizeof(boost::multiprecision::limb_type);

    for (size_t i = 0; i < size; ++i, ++p)
    {
        boost::multiprecision::limb_type t = *p;
        //
        for (size_t j = 0; j < limbTypeSize; j++)
        {
            v.push_back(t & 0xff);
            t >>= 8;
        }
    }

    // add padding with zeros
    v.resize(32);
    
    // if big endian specified
    // note: optimize later if needed!
    if (!littleEndian)
    {
        std::reverse(v.begin(), v.end());
    }
}

bool VectorToUInt256(const vector<unsigned char>& v, uint256_t& ui, bool littleEndian)
{
    if (v.size() != 32)
    {
        return false;
    }

    vector<unsigned char> inputVector(v);

    // if big endian specified
    // note: optimize later if needed!
    if (!littleEndian)
    {
        std::reverse(inputVector.begin(), inputVector.end());
    }

    ui = 0;

    // process inputVector in little endian format
    // note: optimize later
    for (size_t i = 0, offset = 0; i < inputVector.size(); i++, offset += 8)
    {
        ui |= static_cast<uint256_t>(inputVector[i]) << offset;
    }

    return true;
}

string UInt64ToDecString(const uint64_t& ui)
{
    std::stringstream ss;
    ss << std::dec << ui;
    return ss.str();
}

bool DecStringToUint(const string& decString, unsigned int& value)
{
    std::stringstream ss;

    if (decString.empty())
    {
        return false;
    }

    ss << std::dec << decString;
    ss >> value;

    if (ss.fail() || !ss.eof())
    {
        return false;
    }

    return true;
}

bool StringToUInt64(const string& s, uint64_t& ui, bool hexString)
{
    std::stringstream ss;

    try
    {
        // note: boost 1.53 in CentOS doesn't support stream modifiers like std::hex for multiprecision types
        // but prefix '0x' for hex numbers supported in boost 1.53 and in boost 1.64 as well
        // ss << (hexString ? std::hex : std::dec) << s;
        // ss >> (hexString ? std::hex : std::dec) >> ui;
        ss << (hexString ? "0x" : "") << s;
        ss >> (hexString ? std::hex : std::dec) >> ui;
    }
    catch (const std::exception& e)
    {
        return false;
    }
    // check that all symbols are processed e.g. 0xfz -> error
    if (ss.fail() || !ss.eof())
    {
        return false;
    }

    return true;
}

void UInt64ToVector(const uint64_t& ui, vector<unsigned char>& v, bool littleEndian)
{
    v.clear();
    v.resize(sizeof(uint64_t));

    if (littleEndian)
    {
        for (size_t i = 0; i < 8; i++)
        {
            v[i] = static_cast<unsigned char>((ui >> (i * 8)) & 0xff);
        }
    }
    else // big endian 
    {
        for (size_t i = 0; i < 8; i++)
        {
            v[7 - i] = static_cast<unsigned char>((ui >> (i * 8)) & 0xff);
        }
    }
}

bool VectorToUInt64(const vector<unsigned char>& v, uint64_t& ui, bool littleEndian)
{
    if (v.size() != sizeof(uint64_t))
    {
        return false;
    }

    ui = 0;

    if (littleEndian)
    {
        for (size_t i = 0, offset = 0; i < v.size(); i++, offset += 8)
        {
            ui |= static_cast<uint64_t>(v[i]) << offset;
        }
    }
    else // big endian 
    {
        for (size_t i = 0, offset = 0; i < v.size(); i++, offset += 8)
        {
            ui |= static_cast<uint64_t>(v[7 - i]) << offset;
        }
    }

    return true;
}
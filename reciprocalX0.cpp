//
//  main.cpp
//  reciprocalX0
//
//  Created by Chris Larsen on 8/13/22.
//  Copyright Chris Larsen, 2022
//
// This code generates a table of significand values to be used as
// an initial guess for computing the reciprocal of a floating point
// number. A 1 digit needs to be prepended to the table entry to get
// an 8-bit approximation of the reciprocal. The lookup is done using
// an 10-bit index into the table. The index is computed from the
// significand of the number for which we're trying to find the
// reciprocal. Start by stripping off the most significant 1 bit of
// the significand. Than use the remaining 10 most significant bits
// as the index value. Conveniently, IEEE 754 binary floating point
// numbers are stored with the leading 1 bit already stripped off of
// the significand. The user just needs to extract the 10 most
// significant bits of the significand field for the number.

#include <iostream>
#include <iomanip>

inline const int floatToRawIntBits(const float f) {
    const int& i = reinterpret_cast<const int&>(f);
    return i;
}

inline float intBitsToFloat(const int i) {
    const float& f = reinterpret_cast<const float&>(i);
    return f;
}

int main(int argc, const char * argv[]) {
    const int delta = 1 << (23 - 10);
    
    // 0x3F800000 is the bit pattern which corresponds to 1.0f
    // for 32-bit IEEE 754 binary floating point numbers.
    // 0x40000000 is the bit pattern which corresponds to 2.0f
    // for 32-bit IEEE 754 binary floating point numbers.
    // I could also have used "floatToRawIntBits(1.0f)" and
    // "floatToRawIntBits(2.0f)" as the limits for the loop.
    for (int i = 0x3F800000; i < 0x40000000; i += delta) {
        float D = intBitsToFloat(i);
        float r = 1.0f / D;
        int o = floatToRawIntBits(r);
        std::cout << "0x" << std::setw(2) << std::setfill('0') << std::hex
                  << ((o >> (23-7)) & 0x7F) << ", \n";
    }
    
    return 0;
}

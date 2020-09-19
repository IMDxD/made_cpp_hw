#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Library-level functions.
 * You should use them in the main sections.
 */

uint64_t convertToUint64(double number) {
    return *((uint64_t *)(&number));
}

bool getBit(const uint64_t number, const uint8_t index) {
    uint64_t mask = 1;
    mask <<= index;
    uint64_t temp = mask & number;
    temp >>= index;
    return temp;
}

/**
 * Checkers here:
 */

bool checkForPlusZero(uint64_t number) {
    return number == 0x0000000000000000;  // all bits are 0
}

bool checkForMinusZero(uint64_t number) {
    return number == 0x8000000000000000;  // all bits are 0, last bit is 1
}

bool checkForPlusInf(uint64_t number) {
    return number == 0x7FF0000000000000;  // all exponent are 1, sign bit is 0, mantissa bits are 0
}

bool checkForMinusInf(uint64_t number) {
    return number == 0xFFF0000000000000;  // all exponent are 1, sign bit is 1, mantissa bits are 0
}

bool checkForPlusNormal(uint64_t number) {
    if (getBit(number, 63) == 1) {  // sign bit is 0
        return false;
    }
    uint64_t and_mask = number & 0x7FF0000000000000;    // remove mantisa bits
    uint64_t xor_mask = and_mask ^ 0x7FF0000000000000;  // swap exponent bits
    return and_mask && xor_mask;
}

bool checkForMinusNormal(uint64_t number) {
    if (getBit(number, 63) == 0) {  // sign bit is 1
        return false;
    }
    uint64_t and_mask = number & 0x7FF0000000000000;    // remove mantisa bits
    uint64_t xor_mask = and_mask ^ 0x7FF0000000000000;  // swap exponent bits
    return and_mask && xor_mask;
}

bool checkForPlusDenormal(uint64_t number) {
    if (getBit(number, 63) == 1) {  // sign bit is 0
        return false;
    }
    uint64_t exponent_mask = number & 0x7FF0000000000000;
    uint64_t mantisa_mask = number & 0x000FFFFFFFFFFFFF;
    return !exponent_mask && mantisa_mask;
}

bool checkForMinusDenormal(uint64_t number) {
    if (getBit(number, 63) == 0) {  // sign bit is 1
        return false;
    }
    uint64_t exponent_mask = number & 0x7FF0000000000000;
    uint64_t mantisa_mask = number & 0x000FFFFFFFFFFFFF;
    return !exponent_mask && mantisa_mask;
}

bool checkForSignalingNan(uint64_t number) {
    if (getBit(number, 51) == 1) {  // last mantissa bit is 0
        return false;
    }

    uint64_t exponent_mask = ~number & 0x7FF0000000000000;
    uint64_t mantisa_mask = number & 0x0007FFFFFFFFFFFF;
    return !exponent_mask && mantisa_mask;
}

bool checkForQuietNan(uint64_t number) {
    uint64_t and_mask = ~number & 0x7FF8000000000000;  // remove all mantisa bits, except last
    return !and_mask;
}

void classify(double number) {
    if (checkForPlusZero(convertToUint64(number))) {
        printf("Plus zero\n");
    }

    else if (checkForMinusZero(convertToUint64(number))) {
        printf("Minus zero\n");
    }

    else if (checkForPlusInf(convertToUint64(number))) {
        printf("Plus inf\n");
    }

    else if (checkForMinusInf(convertToUint64(number))) {
        printf("Minus inf\n");
    }

    else if (checkForPlusNormal(convertToUint64(number))) {
        printf("Plus normal\n");
    }

    else if (checkForMinusNormal(convertToUint64(number))) {
        printf("Minus normal\n");
    }

    else if (checkForPlusDenormal(convertToUint64(number))) {
        printf("Plus Denormal\n");
    }

    else if (checkForMinusDenormal(convertToUint64(number))) {
        printf("Minus Denormal\n");
    }

    else if (checkForSignalingNan(convertToUint64(number))) {
        printf("Signailing NaN\n");
    }

    else if (checkForQuietNan(convertToUint64(number))) {
        printf("Quiet NaN\n");
    }

    else {
        printf("Error.\n");
    }
}

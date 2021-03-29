#ifndef INCLUDED_PIC32_HELPERS_H
#define INCLUDED_PIC32_HELPERS_H

#define BIT_INV(x, b)           *x  ^= (1 << b)
#define BIT_CLR(x, b)           *x  = *x & ~(1 << b)
#define BIT_SET(x, b)           *x  |= (1 << b)

/**
 * @brief write a value to number of bits in a word 
 *
 * @param x pointer of word to write on
 * @param mask is the positive mask of the position to write
 * @param val the value shifted to write.
 *  the value should be shifted to the same position as the mask
 */

#define BIT_MASK(x, mask, val)  *x = (*x & ~mask) | (val & mask)

/**
 * @brief write a value to number of bits in a word 
 * @param x pointer of word to write on
 * @param len: the bit length, be aware, 2 bits (0b11) gives len = 3
 *  len is the number corresponding to a number of bits set to one
 *  example: if we want to set 3 bits, len = 0b111 = 7
 * @param shift the index of the lsb of bits to write
 * @param val value to write on the bits position
 */

#define BIT_WRITE(x, len, shift, val)  ((*x) = (*x & ~(len << shift)) | ((val << shift) & (len << shift)))

#endif /*INCLUDED_PIC32_HELPERS_H */
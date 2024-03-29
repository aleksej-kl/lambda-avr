/*
 * integers.h
 *
 * Functions to help with integers.
 *
 *  Created on: 07.03.2015
 *      Author: dode@luniks.net
 *
 */

#ifndef INTEGERS_H_
#define INTEGERS_H_

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

/**
 * Divides the given numerator by the given denominator,
 * rounds to the nearest int and returns it.
 * http://stackoverflow.com/a/18067292/709426
 */
int32_t divRoundNearest(int32_t num, int32_t den);

#endif /* INTEGERS_H_ */

/*
 * avrjunit.h
 *
 * AVRJUnit - Very basic unit test framework for AVR micro controller code.
 *
 *  Created on: 06.03.2015
 *      Author: dode@luniks.net
 *
 */

#include <stdbool.h>
#include <avr/pgmspace.h>

#ifndef AVRJUNIT_H_
#define AVRJUNIT_H_

/**
 * Returns false if the given expression is not true.
 */
#define assertTrue(exp) if (! (exp)) return false

/**
 * Returns false if the given expression is not false.
 */
#define assertFalse(exp) if (exp) return false

/**
 * Returns false if the given expression is not NULL.
 */
#define assertNull(exp) if ((exp) != NULL) return false

/**
 * Returns false if the given expression is NULL.
 */
#define assertNotNull(exp) if ((exp) == NULL) return false

/**
 * Computes the length of the given array.
 */
#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

/**
 * Function pointer for test functions taking no parameters and returning
 * true on success and false on failure.
 */
typedef bool (*FuncPtr)(void);

/**
 * A test case with its class, name and test function pointer,
 * which should return true on success and false on failure.
 * The strings class and name are expected to be stored in program space
 * like so: const char testName[] PROGMEM = "testName";
 */
typedef struct PROGMEM {
	PGM_P class;
	PGM_P name;
	FuncPtr function;
} TestCase;

/**
 * A class of tests with its size.
 */
typedef struct {
	const TestCase* tests;
	size_t size;
} TestClass;

/**
 * Begins a test suite run with the given name printed via USART in JUnit XML
 * format. The printed JUnit XML can be read and written to a file on the
 * receiving side with a command like so:
 * (stty speed 9600 sane -echo; cat > tests.xml) < /dev/ttyUSB0
 */
void beginSuite(char* const suite);

/**
 * Ends a test suite run.
 */
void endSuite(void);

/**
 * Runs the test cases in the given test class.
 */
void runClass(TestClass class);

#endif /* AVRJUNIT_H_ */

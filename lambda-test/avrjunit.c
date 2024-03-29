/*
 * avrjunit.c
 *
 * Functions to run basic unit tests on an AVR micro controller and
 * send a minimal JUnit test report XML via USART.
 *
 *  Created on: 06.03.2015
 *      Author: dode@luniks.net
 *
 */

#include <stdio.h>
#include <avr/pgmspace.h>
#include "avrjunit.h"
#include "usart.h"

void beginSuite(char* const suite) {
	printString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	char tsbuf[128];
	snprintf(tsbuf, sizeof(tsbuf),"<testsuite name=\"%s\">\n", suite);
	printString(tsbuf);
}

void endSuite() {
	printString("</testsuite>\n");

	// send EOT
	char eot[] = {0x04, '\0'};
	printString(eot);
}

void runClass(TestClass class) {
	for (size_t i = 0; i < class.size; i++) {
		char cbuf[24];
		char nbuf[64];
		char tcbuf[128];
		strncpy_P(cbuf, (PGM_P)pgm_read_word(&(class.tests[i].class)),
				sizeof(cbuf));
		strncpy_P(nbuf, (PGM_P)pgm_read_word(&(class.tests[i].name)),
				sizeof(nbuf));
		snprintf(tcbuf, sizeof(tcbuf),
				"<testcase classname=\"%s\" name=\"%s\">\n", cbuf, nbuf);
		printString(tcbuf);
		FuncPtr test = (FuncPtr)pgm_read_word(&(class.tests[i].function));
		bool result = test();
		if (! result) {
			// failure
			printString("<failure type=\"failure\">failed</failure>\n");
		}
		printString("</testcase>\n");
	}
}

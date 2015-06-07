/*
 * rules-test.c
 *
 * Unit tests for the lambda project.
 *
 *  Created on: 28.05.2015
 *      Author: dode@luniks.net
 *
 */

#include "avrjunit.h"
#include "rules.h"
#include "alert.h"

extern uint16_t age;
extern int8_t dir;
extern Rule rules[];

/* Module rules */

bool testAirgate50(void) {

	Measurement meas = {0, 0, 0};

	resetRules();
	dir = -1;
	reason(meas);
	assertFalse(rules[0].fired);

	resetRules();
	dir = 0;
	reason(meas);
	assertFalse(rules[0].fired);

	resetRules();
	dir = 1;
	reason(meas);
	assertFalse(rules[0].fired);

	meas.tempI = 500;

	resetRules();
	dir = -1;
	reason(meas);
	assertFalse(rules[0].fired);

	resetRules();
	dir = 0;
	reason(meas);
	assertFalse(rules[0].fired);

	resetRules();
	dir = 1;
	reason(meas);
	assertTrue(rules[0].fired);

	cancelAlert(false);

	return true;
}

bool testAirgate25(void) {

	Measurement meas = {999, 0, 0};

	resetRules();
	dir = 1;
	reason(meas);
	assertFalse(rules[1].fired);

	resetRules();
	dir = 0;
	reason(meas);
	assertFalse(rules[1].fired);

	resetRules();
	dir = -1;
	reason(meas);
	assertFalse(rules[1].fired);

	meas.tempI = 799;

	resetRules();
	dir = 1;
	reason(meas);
	assertFalse(rules[1].fired);

	resetRules();
	dir = 0;
	reason(meas);
	assertFalse(rules[1].fired);

	resetRules();
	dir = -1;
	reason(meas);
	assertTrue(rules[1].fired);

	cancelAlert(false);

	return true;
}

bool testAirgateClose(void) {

	Measurement meas = {999, 0, 0};

	resetRules();
	dir = 1;
	reason(meas);
	assertFalse(rules[2].fired);

	resetRules();
	dir = 0;
	reason(meas);
	assertFalse(rules[2].fired);

	resetRules();
	dir = -1;
	reason(meas);
	assertFalse(rules[2].fired);

	meas.tempI = 399;

	resetRules();
	dir = 1;
	reason(meas);
	assertFalse(rules[2].fired);

	resetRules();
	dir = 0;
	reason(meas);
	assertFalse(rules[2].fired);

	resetRules();
	dir = -1;
	reason(meas);
	assertTrue(rules[2].fired);

	cancelAlert(false);

	return true;
}

bool testTooRich(void) {

	Measurement meas = {0, 0, 0};
	dir = 0;

	meas.tempI = 99;
	meas.lambda = 2000;
	resetRules();
	reason(meas);
	assertFalse(rules[3].fired);

	meas.tempI = 100;
	meas.lambda = 2000;

	resetRules();
	reason(meas);
	assertFalse(rules[3].fired);

	meas.tempI = 99;
	meas.lambda = 1199;
	resetRules();
	reason(meas);
	assertFalse(rules[3].fired);

	meas.tempI = 100;
	meas.lambda = 1199;
	resetRules();
	reason(meas);
	assertTrue(rules[3].fired);

	meas.lambda = 1300;
	age = 0;
	reason(meas);
	assertFalse(rules[3].fired);

	cancelAlert(false);

	return true;
}

bool testFireOut(void) {

	resetRules();
	Measurement meas = {0, 0, 0};
	dir = 1;

	meas.tempI = 50;
	age = 0;
	reason(meas);
	assertFalse(rules[4].fired);

	meas.tempI = 100;
	age = 0;
	reason(meas);
	assertFalse(rules[4].fired);

	meas.tempI = 125;
	age = 0;
	reason(meas);
	assertFalse(rules[4].fired);

	meas.tempI = 99;
	age = 0;
	reason(meas);
	assertTrue(rules[4].fired);

	meas.tempI = 125;
	age = 0;
	reason(meas);
	assertFalse(rules[4].fired);

	cancelAlert(false);

	return true;
}

bool testReasonDirBurnUp(void) {

	resetRules();
	Measurement meas = {0, 0, 2000};

	age = 0;
	reason(meas);
	assertTrue(dir == 0);

	meas.tempI = 9;
	meas.lambda = 2000;
	age = 180;
	reason(meas);
	assertTrue(dir == 0);

	meas.tempI = 100;
	meas.lambda = 2000;
	age = 180;
	reason(meas);
	assertTrue(dir == 1);

	meas.tempI = 790;
	meas.lambda = 2000;
	age = 180;
	reason(meas);
	assertTrue(dir == 1);

	meas.tempI = 790;
	meas.lambda = 2000;
	age = 180;
	reason(meas);
	assertTrue(dir == 0);

	meas.tempI = 800;
	meas.lambda = 2000;
	age = 180;
	reason(meas);
	assertTrue(dir == 1);

	meas.tempI = 850;
	meas.lambda = 2000;
	age = 180;
	reason(meas);
	assertTrue(dir == 0);

	meas.tempI = 900;
	meas.lambda = 1999;
	age = 180;
	reason(meas);
	assertTrue(dir == 0);

	cancelAlert(false);

	return true;
}

bool testReasonDirBurnDown(void) {

	resetRules();
	Measurement meas = {999, 0, 1999};

	age = 0;
	reason(meas);
	assertTrue(dir == 0);

	meas.tempI = 900;
	meas.lambda = 1999;
	age = 180;
	reason(meas);
	assertTrue(dir == 0);

	meas.tempI = 800;
	meas.lambda = 1999;
	age = 180;
	reason(meas);
	assertTrue(dir == 0);

	meas.tempI = 750;
	meas.lambda = 1999;
	age = 180;
	reason(meas);
	assertTrue(dir == 0);

	meas.tempI = 700;
	meas.lambda = 2000;
	age = 180;
	reason(meas);
	assertTrue(dir == -1);

	meas.tempI = 700;
	meas.lambda = 2000;
	age = 180;
	reason(meas);
	assertTrue(dir == 0);

	meas.tempI = 100;
	meas.lambda = 2000;
	age = 180;
	reason(meas);
	assertTrue(dir == -1);

	cancelAlert(false);

	return true;
}

/* Test "class" */
static const char class[] PROGMEM = "rules";

/* Test names */
static const char testAirgate50_P[] PROGMEM = "testAirgate50";
static const char testAirgate25_P[] PROGMEM = "testAirgate25";
static const char testAirgateClose_P[] PROGMEM = "testAirgateClose";
static const char testTooRich_P[] PROGMEM = "testTooRich";
static const char testFireOut_P[] PROGMEM = "testFireOut";
static const char testReasonDirBurnUp_P[] PROGMEM = "testReasonDirBurnUp";
static const char testReasonDirBurnDown_P[] PROGMEM = "testReasonDirBurnDown";

/* Tests */
static TestCase const tests[] = {
		{class, testAirgate50_P, testAirgate50},
		{class, testAirgate25_P, testAirgate25},
		{class, testAirgateClose_P, testAirgateClose},
		{class, testTooRich_P, testTooRich},
		{class, testFireOut_P, testFireOut},
		{class, testReasonDirBurnUp_P, testReasonDirBurnUp},
		{class, testReasonDirBurnDown_P, testReasonDirBurnDown}
};

TestClass rulesClass = {tests, sizeof(tests) / sizeof(tests[0])};

/*
 * rules-test.c
 *
 * Unit tests for the lambda project.
 *
 *  Created on: 28.05.2015
 *      Author: dode@luniks.net
 *
 */

#include "string.h"
#include "avrjunit.h"
#include "rules.h"
#include "alert.h"
#include "interrupts.h"
#include "airgate.h"
#include "utils.h"

/* Module rules */

extern uint8_t measCount;
extern int8_t state;
extern Rule fastRules[];
extern Rule slowRules[];

static bool testAirgate50(void) {

	setDriverFault(false);
	Measurement meas = {0, 0, 0, 0};

	resetRules(true);
	resetAirgate(AIRGATE_OPEN);
	state = burning_down;
	reason(meas);
	assertFalse(slowRules[0].fired);

	resetRules(true);
	resetAirgate(AIRGATE_OPEN);
	state = undefined;
	reason(meas);
	assertFalse(slowRules[0].fired);

	resetRules(true);
	resetAirgate(AIRGATE_OPEN);
	state = firing_up;
	reason(meas);
	assertFalse(slowRules[0].fired);

	meas.tempI = TEMP_AIRGATE_50;
	meas.lambda = LAMBDA_TOO_LEAN;

	resetRules(true);
	resetAirgate(AIRGATE_OPEN);
	state = burning_down;
	reason(meas);
	assertFalse(slowRules[0].fired);

	resetRules(true);
	resetAirgate(AIRGATE_OPEN);
	state = undefined;
	reason(meas);
	assertFalse(slowRules[0].fired);

	resetRules(true);
	resetAirgate(AIRGATE_50);
	state = firing_up;
	reason(meas);
	stepUntilDone();
	assertFalse(slowRules[0].fired);
	assertTrue(AIRGATE_50 == getAirgate());

	resetRules(true);
	resetAirgate(AIRGATE_OPEN);
	state = firing_up;
	reason(meas);
	stepUntilDone();
	assertTrue(slowRules[0].fired);
	assertTrue(AIRGATE_50 == getAirgate());

	cancelAlert();

	return true;
}

static bool testAirgate25(void) {

	setDriverFault(false);
	Measurement meas = {999, 0, 0, 0};

	resetRules(true);
	resetAirgate(AIRGATE_OPEN);
	state = firing_up;
	reason(meas);
	assertFalse(slowRules[1].fired);

	resetRules(true);
	resetAirgate(AIRGATE_OPEN);
	state = undefined;
	reason(meas);
	assertFalse(slowRules[1].fired);

	resetRules(true);
	resetAirgate(AIRGATE_OPEN);
	state = burning_down;
	reason(meas);
	assertFalse(slowRules[1].fired);

	meas.tempI = TEMP_AIRGATE_25 - 1;
	meas.lambda = LAMBDA_TOO_LEAN;

	resetRules(true);
	resetAirgate(AIRGATE_OPEN);
	state = firing_up;
	reason(meas);
	assertFalse(slowRules[1].fired);

	resetRules(true);
	resetAirgate(AIRGATE_OPEN);
	state = undefined;
	reason(meas);
	assertFalse(slowRules[1].fired);

	resetRules(true);
	resetAirgate(AIRGATE_25);
	state = burning_down;
	reason(meas);
	stepUntilDone();
	assertFalse(slowRules[1].fired);
	assertTrue(AIRGATE_25 == getAirgate());

	resetRules(true);
	resetAirgate(AIRGATE_OPEN);
	state = burning_down;
	reason(meas);
	stepUntilDone();
	assertTrue(slowRules[1].fired);
	assertTrue(AIRGATE_25 == getAirgate());

	cancelAlert();

	return true;
}

static bool testAirgateClose(void) {

	setDriverFault(false);
	Measurement meas = {999, 0, 0, 0};

	resetRules(true);
	resetAirgate(AIRGATE_OPEN);
	state = firing_up;
	reason(meas);
	assertFalse(slowRules[2].fired);

	resetRules(true);
	resetAirgate(AIRGATE_OPEN);
	state = undefined;
	reason(meas);
	assertFalse(slowRules[2].fired);

	resetRules(true);
	resetAirgate(AIRGATE_OPEN);
	state = burning_down;
	reason(meas);
	assertFalse(slowRules[2].fired);

	meas.tempI = TEMP_AIRGATE_0 - 1;
	meas.lambda = LAMBDA_MAX;

	resetRules(true);
	resetAirgate(AIRGATE_OPEN);
	state = firing_up;
	reason(meas);
	assertFalse(slowRules[2].fired);

	resetRules(true);
	resetAirgate(AIRGATE_OPEN);
	state = undefined;
	reason(meas);
	assertFalse(slowRules[2].fired);

	resetRules(true);
	resetAirgate(AIRGATE_CLOSE);
	state = burning_down;
	reason(meas);
	stepUntilDone();
	assertFalse(slowRules[2].fired);
	assertTrue(AIRGATE_CLOSE == getAirgate());

	// airgate25 kicks in first and prevents airgateClose from running
	// (airgate busy)
	resetRules(true);
	resetAirgate(AIRGATE_OPEN);
	state = burning_down;
	reason(meas);
	stepUntilDone();
	assertFalse(slowRules[2].fired);
	assertFalse(AIRGATE_CLOSE == getAirgate());

	// now airgateClose kicks in
	resetRules(true);
	state = burning_down;
	reason(meas);
	stepUntilDone();
	assertTrue(slowRules[2].fired);
	assertTrue(AIRGATE_CLOSE == getAirgate());

	cancelAlert();

	return true;
}

static bool testTooRich(void) {

	setDriverFault(false);
	Measurement meas = {0, 0, 0, 1000};

	setHeaterState(heaterStateOn);

	meas.tempI = TEMP_FIRE_OUT;
	meas.lambda = LAMBDA_MAX;
	resetRules(true);
	resetAirgate(AIRGATE_OPEN);
	reason(meas);
	assertFalse(slowRules[3].fired);

	meas.tempI = TEMP_FIRE_OUT + 1;
	meas.lambda = LAMBDA_MAX;
	resetRules(true);
	resetAirgate(AIRGATE_OPEN);
	reason(meas);
	assertFalse(slowRules[3].fired);

	meas.tempI = TEMP_FIRE_OUT;
	meas.lambda = LAMBDA_TOO_RICH - 1;
	resetRules(true);
	resetAirgate(AIRGATE_OPEN);
	reason(meas);
	assertFalse(slowRules[3].fired);

	meas.tempI = TEMP_FIRE_OUT + 1;
	meas.lambda = LAMBDA_TOO_RICH - 1;
	resetRules(true);
	resetAirgate(AIRGATE_50);
	reason(meas);
	stepUntilDone();
	assertFalse(slowRules[3].fired);
	assertTrue(AIRGATE_50 == getAirgate());

	meas.tempI = TEMP_FIRE_OUT + 1;
	meas.lambda = LAMBDA_TOO_RICH - 1;
	resetRules(true);
	resetAirgate(AIRGATE_25);
	reason(meas);
	stepUntilDone();
	assertTrue(slowRules[3].fired);
	assertTrue(AIRGATE_50 == getAirgate());

	cancelAlert();

	return true;
}

static bool testTooLean(void) {

	setDriverFault(false);
	Measurement meas = {0, 0, 0, 1000};

	setHeaterState(heaterStateOn);

	meas.tempI = TEMP_AIRGATE_50;
	meas.lambda = LAMBDA_TOO_LEAN + 1;
	resetRules(true);
	resetAirgate(AIRGATE_OPEN);
	reason(meas);
	assertFalse(slowRules[4].fired);

	meas.tempI = TEMP_AIRGATE_50 + 1;
	meas.lambda = 1300;
	resetRules(true);
	resetAirgate(AIRGATE_OPEN);
	reason(meas);
	assertFalse(slowRules[4].fired);

	meas.tempI = TEMP_AIRGATE_50 + 1;
	meas.lambda = LAMBDA_TOO_LEAN + 1;
	resetRules(true);
	resetAirgate(AIRGATE_50);
	reason(meas);
	stepUntilDone();
	assertFalse(slowRules[4].fired);
	assertTrue(AIRGATE_50 == getAirgate());

	meas.tempI = TEMP_AIRGATE_50 + 1;
	meas.lambda = LAMBDA_TOO_LEAN + 1;
	resetRules(true);
	resetAirgate(AIRGATE_OPEN);
	reason(meas);
	stepUntilDone();
	assertTrue(slowRules[4].fired);
	assertTrue(AIRGATE_50 == getAirgate());

	cancelAlert();

	return true;
}

static bool testFireOut(void) {

	setDriverFault(false);
	Measurement meas = {0, 0, 0, 0};

	resetRules(true);
	resetAirgate(AIRGATE_OPEN);
	state = firing_up;

	meas.tempI = 50;
	measCount = 10;
	reason(meas);
	assertFalse(slowRules[5].fired);

	meas.tempI = TEMP_FIRE_OUT;
	measCount = 10;
	reason(meas);
	assertFalse(slowRules[5].fired);

	meas.tempI = TEMP_FIRE_OUT_RESET;
	measCount = 10;
	reason(meas);
	assertFalse(slowRules[5].fired);

	meas.tempI = TEMP_FIRE_OUT - 1;
	measCount = 10;
	reason(meas);
	assertTrue(slowRules[5].fired);

	meas.tempI = TEMP_FIRE_OUT_RESET;
	measCount = 10;
	reason(meas);
	assertFalse(slowRules[5].fired);

	cancelAlert();

	return true;
}

static bool testWarmStart(void) {

	setDriverFault(false);
	Measurement meas = {TEMP_AIRGATE_50, 0, 0, 0};

	setHeaterState(heaterStateOff);

	resetRules(true);
	resetAirgate(AIRGATE_50);
	slowRules[6].fired = false;
	reason(meas);
	stepUntilDone();
	assertTrue(AIRGATE_50 == getAirgate());
	assertTrue(heaterStateOff == getHeaterState());
	assertFalse(slowRules[6].fired);

	resetRules(true);
	resetAirgate(AIRGATE_50);
	slowRules[6].fired = false;
	state = firing_up;
	reason(meas);
	stepUntilDone();
	assertTrue(AIRGATE_OPEN == getAirgate());
	assertTrue(heaterStateOn == getHeaterState());
	assertTrue(slowRules[6].fired);

	cancelAlert();

	return true;
}

static bool testHeaterReady(void) {

	resetRules(true);
	resetTime();
	Measurement meas = {0, 0, 0, 0};

	setHeaterState(heaterStateOff);
	meas.current = milliAmpsReady;
	reason(meas);
	assertTrue(heaterStateOff == getHeaterState());

	setHeaterState(heaterStateOn);
	meas.current = 5000;
	reason(meas);
	assertTrue(heaterStateOn == getHeaterState());

	setHeaterState(heaterStateOn);
	meas.current = milliAmpsReady;
	reason(meas);
	assertTrue(heaterStateReady == getHeaterState());

	cancelAlert();

	return true;
}

static bool testHeaterFaultNoconn(void) {

	resetRules(true);
	resetTime();
	Measurement meas = {0, 0, 0, 0};
	state = firing_up;

	setHeaterState(heaterStateOn);
	meas.current = 0;
	reason(meas);
	assertTrue(heaterStateFault == getHeaterState());

	cancelAlert();

	return true;
}

static bool testHeaterFaultShort(void) {

	resetRules(true);
	resetTime();
	Measurement meas = {0, 0, 0, 0};
	state = firing_up;

	setHeaterState(heaterStateOn);
	meas.current = 8000;
	reason(meas);
	assertTrue(heaterStateFault == getHeaterState());

	cancelAlert();

	return true;
}

static bool testHeaterFaultNoheat(void) {

	resetRules(true);
	resetTime();
	Measurement meas = {0, 0, 0, 0};
	state = firing_up;

	setHeaterState(heaterStateOn);

	// more than 5 mins
	addTime(301);

	meas.current = 5000;
	reason(meas);
	assertTrue(heaterStateFault == getHeaterState());

	cancelAlert();

	return true;
}

static bool testHeaterTimeout(void) {

	resetRules(true);
	resetTime();
	Measurement meas = {0, 0, 0, 0};

	setHeaterState(heaterStateOn);

	// equal or more than 30 mins below TEMP_FIRE_OUT
	addTime(1800);

	meas.tempI = TEMP_FIRE_OUT - 1;
	meas.lambda = LAMBDA_MAX;
	meas.current = milliAmpsReady;
	reason(meas);
	assertTrue(heaterStateOff == getHeaterState());

	cancelAlert();

	return true;
}

static bool testDriverFault(void) {

	resetRules(true);
	resetTime();
	Measurement meas = {0, 0, 0, 0};

	setDriverFault(true);
	reason(meas);

	assertTrue(fastRules[3].fired);

	setDriverFault(false);
	reason(meas);

	assertFalse(fastRules[3].fired);

	cancelAlert();

	return true;
}

/* Test "class" */
static const char class[] PROGMEM = "rules";

/* Test names */
static const char testAirgate50_P[] PROGMEM = "testAirgate50";
static const char testAirgate25_P[] PROGMEM = "testAirgate25";
static const char testAirgateClose_P[] PROGMEM = "testAirgateClose";
static const char testTooRich_P[] PROGMEM = "testTooRich";
static const char testTooLean_P[] PROGMEM = "testTooLean";
static const char testFireOut_P[] PROGMEM = "testFireOut";
static const char testWarmStart_P[] PROGMEM = "testWarmStart";
static const char testHeaterReady_P[] PROGMEM = "testHeaterReady";
static const char testHeaterFaultNoconn_P[] PROGMEM = "testHeaterFaultNoconn";
static const char testHeaterFaultShort_P[] PROGMEM = "testHeaterFaultShort";
static const char testHeaterFaultNoheat_P[] PROGMEM = "testHeaterFaultNoheat";
static const char testHeaterTimeout_P[] PROGMEM = "testHeaterTimeout";
static const char testDriverFault_P[] PROGMEM = "testDriverFault";

/* Tests */
static TestCase const tests[] = {
 		{class, testAirgate50_P, testAirgate50},
		{class, testAirgate25_P, testAirgate25},
		{class, testAirgateClose_P, testAirgateClose},
		{class, testTooRich_P, testTooRich},
		{class, testTooLean_P, testTooLean},
 		{class, testFireOut_P, testFireOut},
		{class, testWarmStart_P, testWarmStart},
		{class, testHeaterReady_P, testHeaterReady},
		{class, testHeaterFaultNoconn_P, testHeaterFaultNoconn},
		{class, testHeaterFaultShort_P, testHeaterFaultShort},
		{class, testHeaterFaultNoheat_P, testHeaterFaultNoheat},
		{class, testHeaterTimeout_P, testHeaterTimeout},
		{class, testDriverFault_P, testDriverFault}
};

TestClass rulesClass = {tests, ARRAY_LENGTH(tests)};

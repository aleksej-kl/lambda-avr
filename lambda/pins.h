/*
 * pins.h
 *
 * Assignment of ADC and LCD pins.
 *
 *  Created on: 06.05.2015
 *      Author: dode@luniks.net
 */

#ifndef PINS_H_
#define PINS_H_

/** ADC pin for the type K thermocouple signal */
#define ADC_TEMPI PC5
/** ADC pin for the PT1000 resistance thermometer signal */
#define ADC_TEMPO PC0	// prototype: PC0
/** ADC pin for the LSM 11 oxygen sensor signal */
#define ADC_LAMBDA PC2	// prototype: PC2

/* Pins for the LCD */
#define LCD_PORT PORTD
#define LCD_DDR	 DDRD
#define LCD_RS   PD6	// prototype: PD6
#define LCD_EN   PD7	// prototype: PD7
#define LCD_DB4  PD5
#define LCD_DB5  PD2	// prototype: PD2
#define LCD_DB6  PD3
#define LCD_DB7  PD4	// prototype: PD4

#endif /* PINS_H_ */

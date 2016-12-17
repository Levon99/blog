/**
 * Copyright (c) 2016, Łukasz Marcin Podkalicki <lpodkalicki@gmail.com>
 * ATtiny13/015
 * Two-Tone Alarm.
 * --
 * Settings:
 *  FUSE_L=0x6A
 *  FUSE_H=0xFF
 *  F_CPU=1200000
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#define	SPEAKER_PIN	PB0

#define	N_1	(_BV(CS00))
#define	N_8	(_BV(CS01))
#define	N_64	(_BV(CS01)|_BV(CS00))
#define	N_256	(_BV(CS02))
#define	N_1024	(_BV(CS02)|_BV(CS00))

static void twotone_alarm(uint8_t type);
static void tone_loop(uint8_t value, uint8_t N, uint8_t max, uint8_t delay, uint8_t pause);
static void timer_set(uint8_t OCRxn, uint8_t N);
static void sleep(uint8_t ms);


int
main(void)
{

	/* setup */
	DDRB |= _BV(SPEAKER_PIN); // set speaker pin as OUTPUT
	TCCR0A |= _BV(WGM01); // set timer mode to Fast PWM
	TCCR0A |= _BV(COM0A0); // connect PWM pin to Channel A of Timer0

	/* loop */
	while (1) {
		twotone_alarm(1);
	}
}


void
twotone_alarm(uint8_t type)
{

	switch(type) {
	case 1:
		tone_loop(123, N_8, 6, 10, 10);
		tone_loop(22, N_8, 6, 10, 0);
		break;
	default:
	case 0:
		tone_loop(32, N_8, 6, 10, 10);
		tone_loop(22, N_8, 6, 10, 0);
		break;
	}
}


void
tone_loop(uint8_t value, uint8_t N, uint8_t max, uint8_t delay, uint8_t pause)
{
	uint8_t i;

	for (i = 0; i < max; ++i) {
		timer_set(value--, N);
		sleep(delay);
	}

	sleep(pause);
}


void
timer_set(uint8_t OCRxn, uint8_t N)
{

	TCCR0B = (TCCR0B & ~((1<<CS02)|(1<<CS01)|(1<<CS00))) | N;
        OCR0A = OCRxn - 1;
}


void
sleep(uint8_t ms)
{
	uint8_t i;

	for (i = 0; i < ms; ++i) {
		_delay_ms(1);
	}
}

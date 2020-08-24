/*
 * Battery_Testing.h
 *
 * Created: 11/5/2012 2:38:48 PM
 *  Author: smithn
 */ 


#ifndef BATTERY_TESTING_H_
#define BATTERY_TESTING_H_
#include "Queue.h"

extern Queue Voltage;//Queue that is getting a uint16_t type for voltage
extern Queue runtime;//runtime queue is getting sent a pointer to struct type Time'
extern Queue timeupdate;//semaphore for runtime pointer to time
extern Queue LCDMessage;//Queue that holds a pointer to the address of a uint8[32] array for LCD
extern Queue ButtonState;//is going to hold a value (0-3) to see if button has been pressed
extern Queue BottomUpdate;//hold a pointer to the address of a uint8 array
extern Queue resetclk; //going to hold a bool value to see if clock needs to be reset
extern Queue Current; //Queue that holds uint16_t type for current
extern Queue SwitchPos;//holds a 1 or 0 depending on switch position
extern Queue RPMSlider; //holds a uint which is a percentage of the rpm
extern Queue SliderQueue; //holds a uint which is a percentage for the voltage or amps

typedef struct {int16_t hr;int16_t min;int16_t sec;} Time;	

void TotalInitialization();


#endif /* BATTERY_TESTING_H_ */

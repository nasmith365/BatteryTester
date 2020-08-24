/*
 * UserInterface.h
 *
 * Created: 11/10/2012 1:43:02 PM
 *  Author: smithn
 */ 


#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_

const static uint8_t WelcomeMessage[32] PROGMEM = {"Plug in battery and press enter."};//welcome screen
const static uint8_t errorscreen[32] PROGMEM ={"     Error      Turn off system."};//Error Screen
const static uint8_t conclusionscreen[32] PROGMEM ={"Hit entr if doneelse hit nothing"};//Conclusion Screen
const static uint8_t Screen1Initial[32] PROGMEM ={"mA/V? RPM?  Hit XX.XX XXX% enter"};// screen 1
const static uint8_t Screen2Initial[32] PROGMEM ={" Time  RPM   V  XX:XX  XXX% XX.X"};//screen 2
const static uint8_t Screen3Initial[32] PROGMEM ={"mA   V  hrs  mAhXXX X.X XX.X XXX"};//Screen 3
	
void UIMain();

#endif /* USERINTERFACE_H_ */


/*
 * BinClock.c
 * Jarrod Olivier
 * Modified for EEE3095S/3096S by Keegan Crankshaw
 * August 2019
 * 
 * <MCHWIL006> <STHSAN007>
 * 19/08/2019
*/

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h> //For printf functions
#include <stdlib.h> // For system functions

#include "BinClock.h"
#include "CurrentTime.h"

//Global variables
//int hours, mins, secs;
//ong lastInterruptTime = 0; //Used for button debounce
int hours, mins, secs;
int dhours, dmins, dsecs;
long lastInterruptTime = 0;
long debounceTime = 300000; //Used for button debounce
struct timeval last_change;
//int RTC; //Holds the RTC instance

int RTC; //Holds the RTC instance

int HH,MM,SS;

void initGPIO(void){
	/* 
	 * Sets GPIO using wiringPi pins. see pinout.xyz for specific wiringPi pins
	 * You can also use "gpio readall" in the command line to get the pins
	 * Note: wiringPi does not use GPIO or board pin numbers (unless specifically set to that mode)
	 */
	printf("Setting up\n");
	wiringPiSetup(); //This is the default mode. If you want to change pinouts, be aware
	
	RTC = wiringPiI2CSetup(RTCAddr); //Set up the RTC
	
	//Set up the LEDS
	for(int i=0; i < sizeof(LEDS)/sizeof(LEDS[0]); i++){
	    pinMode(LEDS[i], OUTPUT);
	}
	
	//Set Up the Seconds LED for PWM
	//Write your logic here
	pinMode(SECS,PWM_OUTPUT);
	printf("LEDS done\n");
	
	//Set up the Buttons
	for(int j=0; j < sizeof(BTNS)/sizeof(BTNS[0]); j++){
		pinMode(BTNS[j], INPUT);
		pullUpDnControl(BTNS[j], PUD_UP);
	}
	
	//Attach interrupts to Buttons
	//Write your logic here
	wiringPiISR(BTNS[0], INT_EDGE_RISING, &hourInc);
	wiringPiISR(BTNS[1], INT_EDGE_RISING, &minInc);
	printf("BTNS done\n");
	printf("Setup done\n");
 }


/*
 * The main function
 * This function is called, and calls all relevant functions we've written
 */
int main(void){
	initGPIO();

	//Set random time (3:04PM)
	//You can comment this file out later
	wiringPiI2CWriteReg8(RTC, HOUR, 0x13+TIMEZONE);
	wiringPiI2CWriteReg8(RTC, MIN, 0x4);
	wiringPiI2CWriteReg8(RTC, SEC, 0x00);
	
	// Repeat this until we shut down
	for (;;){
	//Fetch the time from the RTC
		//Write your lo

	// printf("%d\n", hours);
	// printf("%d\n", mins);
	//Fetch the time from the RTC
	hours = wiringPiI2CReadReg8(RTC, HOUR);
	mins = wiringPiI2CReadReg8(RTC, MIN);
	secs = wiringPiI2CReadReg8(RTC, SEC);
	dhours = hexCompensation(hours);
	dmins = hexCompensation(mins);
	secs = secs - 0x80;
	dsecs = hexCompensation(secs);

	printf("The current time is: %d:%d:%d\n", dhours, dmins, dsecs);
	lightHours(dhours);
	lightMins(dmins);
	secPWM(dsecs);
	//Write your logic here

	//Function calls to toggle LEDs
	//Write your logic here

	// Print out the time we have stored on our RTC
	// printf("The current time is: %x:%x:%x\n", hours, mins, secs);
	// lightHours(hours);
	// lightMins(mins);

	//using a delay to make our program "less CPU hungry"
	delay(1000); //milliseconds
	return 0;
 }

/*
 * Change the hour format to 12 hours
 */
int hFormat(int hours){
	/*formats to 12h*/
	if (hours >= 24){
		hours = 0;
	}
	else if (hours > 12){
		hours -= 12;
	}
	return (int)hours;
}

/*
 * Turns on corresponding LED's for hours
 */
void lightHours(int units){
	// Write your logic to light up the hour LEDs here	
	int binaryNum[4]; 
  
    // counter for binary array 
    int i = 0; 
    while (units > 0) { 
  
        // storing remainder in binary array 
        binaryNum[i] = units % 2; 
        units =units / 2; 
        i++; 
    } 

   digitalWrite(0,binaryNum[0]);digitalWrite(2,binaryNum[1]);digitalWrite(3,binaryNum[2]);digitalWrite(25,binaryNum[3]);
}

/*
 * Turn on the Minute LEDs
 */
void lightMins(int units){
	//Write your logic to light up the minute LEDs here
          // Write your logic to light up the hour LEDs here
        int binaryNum[6];

    // counter for binary array
    int i = 0;
    while (units > 0) {

        // storing remainder in binary array
        binaryNum[i] = units % 2;
        units = units / 2;
        i++;
    }

   digitalWrite(7,binaryNum[0]);digitalWrite(22,binaryNum[1]);
   digitalWrite(21,binaryNum[2]);digitalWrite(27,binaryNum[3]);
   digitalWrite(4,binaryNum[4]);digitalWrite(6,binaryNum[5]);


}

/*
 * PWM on the Seconds LED
 * The LED should have 60 brightness levels
 * The LED should be "off" at 0 seconds, and fully bright at 59 seconds
 */
void secPWM(int units){
	// Write your logic here 
	// Write your logic here
	int s = units;
	// printf("seconds in secPWM %d\n", s);
	int ratio = s*1024/59;
	pwmWrite(SECS, ratio);
	printf("ratio of second %d\n", ratio);

}

/*
 * hexCompensation
 * This function may not be necessary if you use bit-shifting rather than decimal checking for writing out time values
 */
int hexCompensation(int units){
	/*Convert HEX or BCD value to DEC where 0x45 == 0d45 
	  This was created as the lighXXX functions which determine what GPIO pin to set HIGH/LOW
	  perform operations which work in base10 and not base16 (incorrect logic) 
	*/
	int unitsU = units%0x10;

	if (units >= 0x50){
		units = 50 + unitsU;
	}
	else if (units >= 0x40){
		units = 40 + unitsU;
	}
	else if (units >= 0x30){
		units = 30 + unitsU;
	}
	else if (units >= 0x20){
		units = 20 + unitsU;
	}
	else if (units >= 0x10){
		units = 10 + unitsU;
	}
	return units;
}


/*
 * decCompensation
 * This function "undoes" hexCompensation in order to write the correct base 16 value through I2C
 */
int decCompensation(int units){
	int unitsU = units%10;

	if (units >= 50){
		units = 0x50 + unitsU;
	}
	else if (units >= 40){
		units = 0x40 + unitsU;
	}
	else if (units >= 30){
		units = 0x30 + unitsU;
	}
	else if (units >= 20){
		units = 0x20 + unitsU;
	}
	else if (units >= 10){
		units = 0x10 + unitsU;
	}
	return units;
}


/*
 * hourInc
 * Fetch the hour value off the RTC, increase it by 1, and write back
 * Be sure to cater for there only being 23 hours in a day
 * Software Debouncing should be used
 */
void hourInc(void){
	//Debounce
	long interruptTime = millis();

	if (interruptTime - lastInterruptTime>200){
		printf("Interrupt 1 triggered, %x\n", hours);
		hours = wiringPiI2CReadReg8(RTC, HOUR);
	dhours = hexCompensation(hours);
	dhours = dhours + 1;
	dhours = hFormat(dhours);
	hours = decCompensation(dhours);
	wiringPiI2CWriteReg8(RTC, HOUR, hours);
		//Fetch RTC Time
		//Increase hours by 1, ensuring not to overflow
		//Write hours back to the RTC
	}
	lastInterruptTime = interruptTime;
}

/* 
 * minInc
 * Fetch the minute value off the RTC, increase it by 1, and write back
 * Be sure to cater for there only being 60 minutes in an hour
 * Software Debouncing should be used
 */
void minInc(void){
	long interruptTime = millis();

if (interruptTime - lastInterruptTime>500){
	printf("Interrupt 2 triggered, %x\n", mins);
	//Fetch RTC Time

	//Fetch RTC Time
	mins = wiringPiI2CReadReg8(RTC, MIN);
	dmins = hexCompensation(mins);
	dmins = dmins + 1;;
	dmins = mFormat(dmins);
	mins = decCompensation(dmins);
	wiringPiI2CWriteReg8(RTC, MIN, mins);
	//Increase minutes by 1, ensuring not to overflow
	//Write minutes back to the RTC	
	}
	lastInterruptTime = interruptTime;
}

//This interrupt will fetch current time from another script and write it to the clock registers
//This functions will toggle a flag that is checked in main
void toggleTime(void){
	long interruptTime = millis();

	if (interruptTime - lastInterruptTime>200){
		HH = getHours();
		MM = getMins();
		SS = getSecs();

		HH = hFormat(HH);
		HH = decCompensation(HH);
		wiringPiI2CWriteReg8(RTC, HOUR, HH);

		MM = decCompensation(MM);
		wiringPiI2CWriteReg8(RTC, MIN, MM);

		SS = decCompensation(SS);
		wiringPiI2CWriteReg8(RTC, SEC, 0b10000000+SS);

	}
	lastInterruptTime = interruptTime;
}

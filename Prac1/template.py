#!/usr/bin/python3
"""
Python Practical Template
Willie Macharia
Readjust this Docstring as follows:
Names: Willie Macharia>
Student Number: MCHWIL006
Prac: 1
Date: 28/07/2019
"""

# import Relevant Librares
import RPi.GPIO as GPIO
import time
GPIO.setmode(GPIO.BOARD)
GPIO.setwarnings(False)

# Logic that you write

def main():
    turnon()
    time.sleep(3)
    turnoff()
    time.sleep(3)

def turnon():
    print("It is working here")
    GPIO.setup(15,GPIO.OUT)
    GPIO.output(15,GPIO.HIGH)
def turnoff():
    print("It Led off  here")
    GPIO.output(15,GPIO.LOW)



# Only run the functions if 
if __name__ == "__main__":
    # Make sure the GPIO is stopped correctly
    try:
        while True:
            main()
    except KeyboardInterrupt:
        print("Exiting gracefully")
        # Turn off your GPIOs here
        GPIO.cleanup()
    except IOError:
        GPIO.cleanup()
        print("Some other error occurred")
        #print(e)

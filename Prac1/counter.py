

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
import itertools
GPIO.setmode(GPIO.BOARD) #set board mode to Board
GPIO.setwarnings(False)
global counter #set counter as global variable
counter=0
# Logic that you write
global lst   #set lst as a global variable
lst=list(itertools.product([0, 1], repeat=3))

def main():

    setUpGPIOS()  #set GPIOs first
   # lst = list(itertools.product([0, 1], repeat=3))
    GPIO.add_event_detect(8, GPIO.RISING, callback=countup,bouncetime=200) #detect when pin 8 becomes 1. Debounce 200
    GPIO.add_event_detect(16, GPIO.RISING, callback=countdown,bouncetime=200) #detect when pin 8 becomes 1. Debounce 200
   # print(lst[counter])
    GPIO.output(15,0) #set pin 15 to zero intially
    GPIO.output(11,0) #set pin 11 to zero intially
    GPIO.output(12,0) #set pin 12 to zero intially
    #GPIO.cleanup()
    message=input("press enter to quit") #break statement
   # turnon()
    #time.sleep(3)
    #turnoff()
    #time.sleep(3)
   # input("enter")
    #GPIO.clean()

def setUpGPIOS():  #setting up function
    print("It is working here")
    #setting up pin 15,11,12 AS output as three bits are needed to represent highest number in counter
    GPIO.setup(15,GPIO.OUT)
    GPIO.setup(11,GPIO.OUT)
    GPIO.setup(12,GPIO.OUT)
    #GPIO.output(15,GPIO.HIGH)
   #setting pin 8,16 as inputs and internal pull up and down resistors as down
    GPIO.setup(8,GPIO.IN,pull_up_down=GPIO.PUD_DOWN)
    GPIO.setup(16,GPIO.IN,pull_up_down=GPIO.PUD_DOWN)

def countup(channel):  #counting up function
     global counter
     if counter<=6: #if index is less than 6 it means it needs to add up  one morre index
      counter+=1
     else:     #else roll back to index zero
      counter=0
     print (lst[counter])
     GPIO.output(15,lst[counter][0])  #display the output of the first bit
     GPIO.output(12,lst[counter][1])  #display the output of the second bit
     GPIO.output(11,lst[counter][2])  #display the output of the third bit
def countdown(channel): #setting up the counting down function
     global counter
     if counter<=0:
      counter=7  #if index is less than 0 it means it needs to roll back to the highest index of 7
     else:
      counter-=1  #else decrement by 1
     print(lst[counter])
     GPIO.output(15,lst[counter][0]) #display the output of the first bit
     GPIO.output(12,lst[counter][1]) #display the output of the second bit
     GPIO.output(11,lst[counter][2]) #display the output of the third bit


# Only run the functions if 
if __name__ == "__main__":
    # Make sure the GPIO is stopped correctly
    try:
       #counter=0
      # setUpGPIOS()
       #while True:
       main()   #calling the main function
    except KeyboardInterrupt:
        print("Exiting gracefully")
        # Turn off your GPIOs here
        GPIO.cleanup()   #cleaning the GPIOS
    except IOError:
        GPIO.cleanup()
        print("Some other error occurred")
        #print(e)

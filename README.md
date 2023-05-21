# PingTimer
Nonblocking code for an Ultrasonic on ATMEGA328 or 1284

I built this library because I needed a way to use an ultrasonic sensor without having to wait for the echo.  

This library uses Timer1's input capture feature to handle the timing with hardware.  It is currently coded for an ATMega1284P but just needs some lines uncommented to work for an UNO or other 328P based board and could quickly and easily be ported to the Mega by adding the pin numbers.  

This library uses Timer1 hardware for all portions of a ping and response.  First it uses the PWM generator to generate the pulse on the trigger pin.  Then it uses the ICR interrupt to capture both the rising and falling times of the echo pin.  On the rising pin it sets up the timer for input capture and on the falling pin it saves the captured timer register.  

The library includes a pre-built object and can only be used for one ping timer as written.  It may be possible to expand to two if we use the other PWM output pin on Timer1. 



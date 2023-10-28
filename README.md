# **PingTimer**
## **Nonblocking code for an Ultrasonic Ping Sensor**

> I built this library because I needed a way to use an ultrasonic sensor without having to wait for the echo.  
> it currently only supports ATmega328(P) and ATmega 1284(P) chips. Additional support can be added on request

> This library uses Timer1's input capture feature to handle the timing with hardware.  It is currently coded for an ATMega1284P but just needs some lines uncommented to work for an UNO or other 328P based board and could quickly and easily be ported to the Mega by adding the pin numbers.  

> This library uses Timer1 hardware for all portions of a ping and response.  First it uses the PWM generator to generate the pulse on the trigger pin.  Then it uses the ICR interrupt to capture both the rising and falling times of the echo pin.  On the rising pin it sets up the timer for input capture and on the falling pin it saves the captured timer register.  

> The library includes a pre-built object and can only be used for one ping timer as written.  It may be possible to expand to two if we use the other PWM output pin on Timer1 or to more by using a mux.   

> Because it uses Timer1 hardware pins, there is no choice in pin selection.  
>> For a 328P based device like an UNO or Nano you **MUST** use **pin 9** for the trigger pin and **pin 8** for the echo pin. 
>> For a 1284P based device using the standard variant in MightyCore the trigger pin is **pin 13** and the echo pin is **pin 14**.  

# **Functions:**

## void begin();
>  Call this method once from setup to initialize the timer registers.

## void sendPing();
> Call this method to start the measurement process.  After calling this method, poll the hasNewData() method to determine when the measurement is finished. 

## boolean hasNewData();
>  Poll this method after calling sendPing().  It will return true once when new data is ready. 
>>  **returns boolean** - true if there is new measurement data to read.

## int16_t getDistanceMM();
>  Gets the distance of the latest measurement in millimeters.  Will return -1 if the measurement overflowed
>> **returns int16_t** the distance in millimeters of the last measurement or -1 if the last measurement overflowed. 

## boolean hasOverflowed();
>  Will return true if the distance is too far to measure.
>>  **returns boolean** - true if the distance is too great and the last measurement overflowed the timer. 


// include <LiquidCrystal.h>
// written by Bereket Kebede
// Dextrous Robotics
// Last updated: July 26, 2023

// Uses ATPMEGA328P, blue NANO
/*
 This sketch retrieves motion burst data and explains the output.
 */

#include <Arduino.h>
#include "ADNS3080.h"
#include <SPI.h>
// Include the libraries:
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#ifndef ADNS3080_h
#define ADNS3080_h 
#include "ADNS3080.tpp"
#endif


// SPI pins
#define PIN_RESET     9         
#define PIN_CS        10        
// sensor configuration 
#define LED_MODE      false     // If true, enables LED Mode
#define RESOLUTION    false     // If true, enable high resolution mode 
#define PIN_LED       3         // for blinking


// Define SDA and SCL pin for LCD:
#define SDAPin A4 // Data pin
#define SCLPin A5 // Clock pin
// Connect to LCD via I2C, default address 0x27 (A0-A2 not jumpered):
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,20,4); //Change to (0x27,16,2) for 1602 LCD

// Define variables:
long duration;
int distance;
int count_freq =0;

ADNS3080 <PIN_RESET, PIN_CS> sensor;

// Initial position
int8_t x = 0; // for sensor.motion burst
int8_t y = 0;
int count = 0;
unsigned long StartTime;
unsigned long CurrentTime;
unsigned long ElapsedTime ;
int8_t lock = 0;
float period = 0; // time for one loop
float frequency = 0; // how many loops run per sec



// Initial position
int8_t xx = 0; // for sensor.displacement burst
int8_t yy = 0;
int8_t occ_output;

void setup() {
  sensor.setup(LED_MODE,RESOLUTION);
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  //Initiate the LCD:
  lcd.init();
  lcd.backlight();
  StartTime = millis();
}

int convTwosComp(int b){
  //Convert from 2's complement
  if(b & 0x80){
    b = -1 * ((b ^ 0xff) + 1);
    }
  return b;
  }


void loop() {
  uint8_t  motion;      // Did motion occur? true or false
  int8_t   dx, dy;      // Displacement since last function call, for motion function
  //int8_t   dxx, dyy;      // Displacement since last function call,, for displacement function
  uint8_t  squal;       //  Surface quality value - Larger value means better tracking. If below ~15 the displacement is dissabled. Adjust focus for 30 or more.
  uint16_t shutter;     // Camera shutter rate in clock cycles.
  uint8_t  max_pixel;   // Maximum pixel value of current frame (max brightness)

  char my_output_flag[] = "serial_digital_output";
  unsigned long polling_freq;

  digitalWrite( PIN_LED, HIGH ); 
  
  sensor.motionBurst( &motion, &dx, &dy, &squal, &shutter, &max_pixel );
  int8_t dxx = convTwosComp(dx);
  int8_t dyy = convTwosComp(dy);

  occ_output = occlusion_sensor(max_pixel);
  float speed = dxx;

  Serial.print(dxx);
  Serial.print(",");
  Serial.print(dyy);
  Serial.print(",");
  Serial.print(occ_output);
  Serial.print(",");
  Serial.println(squal);


  //Serial.println(dyy);

  lcd.setCursor(0,0); // Set the cursor to column 1, line 1 (counting starts at zero)
  lcd.print("speed:"); // Prints string "Display = " on the LCD
  lcd.print(speed, 2); // Prints the measured distance
  //lcd.print(" cm/s"); // Prints the measured distance
  digitalWrite(PIN_LED,LOW);
  

  if (lock == 0){
      lock = lock + 1;
      //ang_speed = 1/ElapsedTime;
      period = ElapsedTime;
      //ang_speed = 1.00/ (ElapsedTime/1000.00);
      //if (lock =1){
      frequency = 1000.00/period;
      //ang_speed = 6.14 * frequency; // w = 2*pi*f
  }


  delay(50); //50ms very good so far
  



}


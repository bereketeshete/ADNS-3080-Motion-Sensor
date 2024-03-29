#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


#ifndef ADNS3080_h
#define ADNS3080_h


//------------ Constants and registers ---------------

// Signal delay time:
#define ADNS3080_T_IN_RST             500
#define ADNS3080_T_PW_RESET           10
#define ADNS3080_T_SRAD_MOT           75
#define ADNS3080_T_SWW                50
#define ADNS3080_T_SRAD               50
#define ADNS3080_T_LOAD               10
#define ADNS3080_T_BEXIT              4

// Pixel dimensions:
#define ADNS3080_PIXELS               30

// Registers: 
#define ADNS3080_PRODUCT_ID           0x00
#define ADNS3080_CONFIGURATION_BITS   0x0a
#define ADNS3080_MOTION_CLEAR         0x12
#define ADNS3080_FRAME_CAPTURE        0x13
#define ADNS3080_PIXEL_BURST          0x40
#define ADNS3080_MOTION_BURST         0x50
#define ADNS3080_PRODUCT_ID_VALUE     0x17

//--------------- Template Parameters ---------------- [ No characters after backlash! ]

#define TEMPLATE_TYPE           \
        uint8_t PIN_RESET,      \
        uint8_t PIN_NCS         
    
#define TEMPLATE_INPUTS         \
                PIN_RESET,      \
                PIN_NCS 
  
//---------------- Class definition ------------------
  
template <TEMPLATE_TYPE>
class ADNS3080 {  
  private:
    // Read and write registers:
    void writeRegister( const uint8_t, uint8_t );
    uint8_t readRegister( const uint8_t );
   
  public:   
    // Miscellaneous functions:
    void reset();
    bool setup( const bool=false, const bool=false );
    void motionClear();
    
    // Major outputs:
    void motionBurst( uint8_t*, int8_t*, int8_t*, uint8_t*, uint16_t*, uint8_t* );
    void displacement( int8_t*, int8_t* );
    void frameCapture( uint8_t[ADNS3080_PIXELS][ADNS3080_PIXELS] );
};

#include "ADNS3080.tpp"

//----------------External Functions ----------------------

float measure_frequency(unsigned long StartTime, unsigned long NextTime){
  float period = NextTime - StartTime;
  float frequency = 1.00/period;
  return frequency;
}

float direction(int8_t dx, int8_t dy){
  // RAD2DEG = angle * 180 / PI;
  float angle = atan2(dy, dx) * 180 / PI;
  return angle;
}

int8_t magni(int8_t dx, int8_t dy){
  int8_t d_avg = sqrt (pow (dx, 2) + pow (dy, 2));
  return d_avg;
}

float mag(float dx, float dy){
  float d_avg = sqrt (pow (dx, 2) + pow (dy, 2));
  return d_avg;
}


void occlusion_sensor(uint8_t max_pixel){
    if (max_pixel < 30) {
    digitalWrite(2, HIGH);
  } else {
    // object infront of it
    digitalWrite(2, LOW);
  }
}


#endif

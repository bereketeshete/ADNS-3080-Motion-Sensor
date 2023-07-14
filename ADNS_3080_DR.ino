//#include <LiquidCrystal.h>

// written by Bereket Kebede
// Dextrous Robotics
/*
 This sketch retrieves motion burst data and explains the output.
 */
//#include <ADNS3080.h>

// Uses ATPMEGA328 Old Bootloader

#include <Arduino.h>
#include <SPI.h>
// Include the libraries:
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

#endif


// SPI pins
#define PIN_RESET     9         
#define PIN_CS        10        

// sensor configuration 
#define LED_MODE      false     // If true, enables LED Mode
#define RESOLUTION    false     // If true, enable high resolution mode 


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


// Initial position
int xx = 0; // for sensor.displacement burst
int yy = 0;



void setup() {
  sensor.setup(LED_MODE,RESOLUTION);
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  //Initiate the LCD:
  lcd.init();
  lcd.backlight();
  StartTime = millis();
}

float direction(int8_t dx, int8_t dy){
  // RAD2DEG = angle * 180 / PI;
  float angle = atan2(dy, dx) * 180 / PI;
  return angle;
}

float mag(float dx, float dy){
  float d_avg = sqrt (pow (dx, 2) + pow (dy, 2));
  return d_avg;
}

int8_t magni(int8_t dx, int8_t dy){
  int8_t d_avg = sqrt (pow (dx, 2) + pow (dy, 2));
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
  

  sensor.motionBurst( &motion, &dx, &dy, &squal, &shutter, &max_pixel );

 
  //sensor.displacement( &dxx, &dyy );

  // Integrate displacements
  // x += dx;
  // y += dy;

  // xx += dxx;
  // yy += dyy;

  //print magnitude and angle of movt
  // float average_dist = mag(x,y);
  // float my_angle = direction(dx,dy);

  ////////////////////////////////////////
  // option 2
  
  // Serial.print("Average:");
  // Serial.println(average_dist);
  // Serial.print("angle:");
  // Serial.println(my_angle);

  //   // occlusions sensor
  // occlusion_sensor(max_pixel);

  // if (max_pixel < 20) {
  //   digitalWrite(2, HIGH);
  // } else {
  //   digitalWrite(2, LOW);
  // }


  ////////////////////////////////////////
  // option 3, measure requency
  
  // unsigned long CurrentTime = millis();
  // unsigned long ElapsedTime = CurrentTime - StartTime;
  // unsigned long threshold = 1000;

  // // how many loop updates until 1000ms/1 sec is reached
  // if (ElapsedTime < 1000){
  //   count = count+1;
  //   Serial.println(ElapsedTime);
  // }
  // else {
  //   // quit
  //   Serial.print("frequency is: ");
  //   Serial.print(count);
  //   Serial.println(" Hz");
  //     // Display the distance on the LCD:
  //   lcd.setCursor(0,0); // Set the cursor to column 1, line 1 (counting starts at zero)
  //   lcd.print("Poll Frequency "); // Prints string "Display = " on the LCD
  //   lcd.setCursor(0,1); // Set the cursor to column 1, line 1 (counting starts at zero)
  //   lcd.print(count); // Prints the measured distance
  //   lcd.print(" Hz "); // Prints "cm" on the LCD, extra spaces are needed to clear previously displayed characters
    
  // }


  /////////////////////////////////////
  // new (June 26)
  
  // Serial.print("Average:");
  // Serial.println(average_dist);
  // Serial.print("angle:");
  // Serial.println(my_angle);

  // lcd.setCursor(0,0); // Set the cursor to column 1, line 1 (counting starts at zero)
  // lcd.print("count"); // Prints string "Display = " on the LCD
  // lcd.setCursor(0,1); // Set the cursor to column 1, line 1 (counting starts at zero)
  // //lcd.print(round(average_dist)); // Prints the measured distance
  // lcd.print(average_dist); // Prints the measured distance

  // delay(500);

  // if (my_output_flag == "serial_digital_output"){
  //    Serial.print("Average:");
  //    Serial.println(average_dist);
  //    Serial.print("angle:");
  //    Serial.println(my_angle);
  // }
  // else {
  //   Serial.println("choose correct output flag");
  // }
  /////////////////////////////////////////////////////////


  // Serial.print("Distance:");
  // Serial.println(motion);

  //For serial plotter
  // Serial.print("dx:");
  // Serial.println(dx);
  // Serial.print("dy:");
  // Serial.println(dy);


  // magnitude and angle plotter
  // Serial.print("Distance=");
  // Serial.println(displacement);
  // Serial.print("Angle=");
  // Serial.println(angle);

  ////////////////////////////////////////
  // option 4

  // Serial.print( "motionsensor:" );
  // Serial.print("motn=");
  // Serial.println(motion);
  // Serial.print("squal=" );
  // Serial.print( squal );
  // Serial.print( "shutr=" );
  // Serial.print( shutter );
  // Serial.print( "maxpix=" );
  // Serial.print( max_pixel );

  // // Displacement:
  // Serial.print( "dx=" );
  // Serial.print( dx );
  // Serial.print( "dy=" );
  // Serial.print( dy );
  // Serial.print( "x=" );
  // Serial.print( x );
  // Serial.print( "y=" );
  // Serial.print( y );
  // Serial.println();

  // if (motion == 1) {
  //     digitalWrite(2, HIGH);
  //   } else {
  //     digitalWrite(2, LOW);
  //   }


  // a funtion motion w
  // if (max_pixel > 10) {
  //   digitalWrite(2, HIGH);
  // } else {
  //   digitalWrite(2, LOW);
  // }


  /////////////////////////////////////
  // ouput 9

  // occlusion_sensor(max_pixel);
  // Serial.print("Average:");
  // Serial.println(average_dist);
  // Serial.print("angle:");
  // Serial.println(my_angle);

  // lcd.setCursor(0,0); // Set the cursor to column 1, line 1 (counting starts at zero)
  // lcd.print("count:"); // Prints string "Display = " on the LCD
  // lcd.print(average_dist); // Prints the measured distance
  // lcd.setCursor(0,1); // Set the cursor to column 1, line 1 (counting starts at zero)
  // lcd.print("squal:");
  // lcd.print(squal);
  // lcd.print(" max:");
  // lcd.print(max_pixel);
  // delay(500);

  /////////////////////////////////////////////////////////
  // output 10

  // int8_t dxx = convTwosComp(dx);
  // int8_t dyy = convTwosComp(dx);

  // occlusion_sensor(max_pixel);
  // // Serial.print("x:");
  // // Serial.println(dxx);
  // // Serial.print("y:");
  // // Serial.println(dyy);

  // Serial.print( " dx=" );
  // Serial.print( dx );
  // Serial.print( " dy=" );
  // Serial.print( dy );
  // Serial.print( " x=" );
  // Serial.print( x );
  // Serial.print( " y=" );
  // Serial.print( y );
  // Serial.print( " mag=" );
  // Serial.print( y );
  // Serial.println(magni(x,y));

  // lcd.setCursor(0,0); // Set the cursor to column 1, line 1 (counting starts at zero)
  // lcd.print("x:"); // Prints string "Display = " on the LCD
  // lcd.print(dy); // Prints the measured distance
  // lcd.print(" | y:"); // Prints string "Display = " on the LCD
  // lcd.print(dy); // Prints the measured distance
  // lcd.setCursor(0,1); // Set the cursor to column 1, line 1 (counting starts at zero)
  // lcd.print("sq:");
  // lcd.print(squal);
  // lcd.print("|mag:");
  // lcd.print(magni(x,y));
  //delay(500);

   /////////////////////////////////////////////////////////
  // output 11

  int8_t dxx = convTwosComp(dx);
  int8_t dyy = convTwosComp(dy);

  occlusion_sensor(max_pixel);
  // Serial.print("x:");
  // Serial.println(dxx);
  // Serial.print("y:");
  // Serial.println(dyy);

  //float8_t = speed;
  float speed = dx;

  // Serial.print( " dx=" );
  // Serial.print( dx );
  // Serial.print( " dy=" );
  // Serial.print( dy );
  // Serial.print( " x=" );
  // Serial.print( x );
  // Serial.print( " y=" );
  // Serial.print( y );
  //Serial.print( " speed=" );
  Serial.println(speed);

  lcd.setCursor(0,0); // Set the cursor to column 1, line 1 (counting starts at zero)
  lcd.print("speed:"); // Prints string "Display = " on the LCD
  lcd.print(speed); // Prints the measured distance
  lcd.print(" cm/s"); // Prints the measured distance
  // lcd.setCursor(0,1); // Set the cursor to column 1, line 1 (counting starts at zero)
  // lcd.print(" | y:"); // Prints string "Display = " on the LCD
  // lcd.print(dy); // Prints the measured distance
  // lcd.print("sq:");
  // lcd.print(squal);
  // lcd.print("|mag:");
  // lcd.print(magni(x,y));
  // 500 works best so far
  //delay(500);

  /////////////////////////////////////////////////////////
  // output 12

//   //occlusion_sensor(max_pixel);
//   //float8_t = speed;
//   float speed = dx/20.00;

//   // Serial.print( " dx=" );
//   // Serial.print( dx );
//   // Serial.print( " dy=" );
//   // Serial.print( dy );
//   // Serial.print( " x=" );
//   // Serial.print( x );
//   // Serial.print( " y=" );
//   // Serial.print( y );
//   // Serial.print( " speed=" );
//   // Serial.println(ElapsedTime);
//   lcd.setCursor(0,0); // Set the cursor to column 1, line 1 (counting starts at zero)
//   lcd.print("speed:"); // Prints string "Display = " on the LCD
//   lcd.print(speed); // Prints the measured distance
//   lcd.print(" cm/s"); // Prints the measured distance

//   lcd.setCursor(0,1); // Set the cursor to column 1, line 1 (counting starts at zero)
//   lcd.print("poll:"); // Prints string "Display = " on the LCD
//   lcd.print(ElapsedTime); // Prints the measured distance
//   lcd.print(" cm/s"); // Prints the measured distance
//   //delay(200);


//   CurrentTime = millis();

//   // if (ElapsedTime < 1000){
//   //  count++;
//   //  Serial.println(ElapsedTime);
//   // }

//   unsigned long ElapsedTime = CurrentTime - StartTime;

//    if (ElapsedTime < 1000){
//    count++;
//    Serial.println(ElapsedTime);
//  }
//  else {
//    // return freq after one sec
//    count_freq ++;
//    CurrentTime = millis();
//    ElapsedTime = CurrentTime - StartTime;
//    Serial.print("frequency is: ");
//    Serial.print(count);
//    Serial.println(" Hz");
//  }

//   // if (lock == 0){
//   //   lock = lock + 1;
//   //   //ang_speed = 1/ElapsedTime;
//   //   //polling_freq = 1.00/ (ElapsedTime/100000.00);
//   //   // polling_freq = ElapsedTime;
//   //   Serial.print("poll=" );
//   //   //Serial.println(1000.00/(CurrentTime - StartTime), 5);
//   //   //Serial.println(1000.00/(ElapsedTime), 5);
//   //   Serial.println(ElapsedTime, 5);
//   // }

}

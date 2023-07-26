// written by Bereket Kebede
// Dextrous Robotics 
// Last update on July 14, 2023
// This sketch retrieves motion burst data and explains the output.
// Uses ATPMEGA328 Old Bootloader


#include </home/bereket/Arduino/ADNS_3080/ADNS_3080_DR/ADNS3080.h>
//#include <ADNS3080.h>


// SPI pins
#define PIN_RESET     9         
#define PIN_CS        10        
// sensor configuration 
#define LED_MODE      true   // If true, enables LED Mode
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
  int8_t dxx = convTwosComp(dx);
  int8_t dyy = convTwosComp(dy);

  occlusion_sensor(max_pixel);
  float speed = dx;
  Serial.println(speed);


  if (lock ==0)
    lock = lock +1;
    polling_freq = measure_frequency(StartTime, ElapsedTime);

  lcd.setCursor(0,0); // Set the cursor to column 1, line 1 (counting starts at zero)
  lcd.print("speed:"); // Prints string "Display = " on the LCD
  lcd.print(speed);
  lcd.setCursor(0,1); // Set the cursor to column 1, line 1 (counting starts at zero)
  lcd.print("poll"); // Prints string "Display = " on the LCD
  lcd.print(polling_freq); // Prints string "Display = " on the LCD
  lcd.print("hz"); // Prints string "Display = " on the LCD

  delay(200);
  
}



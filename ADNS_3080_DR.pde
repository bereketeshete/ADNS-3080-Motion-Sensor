import processing.serial.*;

Serial COMPort;  // Create object from Serial class

// First connect the ADNS 3080, followed by the motor 

String[] lines = new String[0];
int StartTime = millis();
int currentTime;
int ElapsedTime;
int lock =0;

void setup() 
{
  frameRate(5);
  size(200, 200);
  String portName = Serial.list()[0];
  String portName_2 = Serial.list()[1];
  COMPort = new Serial(this, portName, 9600);
  println(portName_2);
}

void draw() 
{
  //read when reset is pressed on Nema 17 
  //if (portName_2 == "/dev/ttyUSB1"){
    
  //}                                                                                                                                                                                                       
  
  if (COMPort.available() > 0) {  // If data is available,
    String read = COMPort.readString();  // read and store it to string read
    //read = "value : " + read;
    println(read);
    lines = append(lines, read);// append new read to string lines
   } 
   else {
    saveStrings("Experiment 23.txt", lines);//save string to file
  }
  currentTime = millis();
  ElapsedTime = currentTime - StartTime;
  
  //if (ElapsedTime >= 4000){
  //  //intiate the motor
  //  if (lock == 0){
  //    println("----------------------turn on motor");
  //    lock +=1;
  //  }
  //}
  
  if (ElapsedTime >= 20000){
    noLoop();
  }
  
}

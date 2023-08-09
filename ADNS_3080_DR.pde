// author: Bereket Kebede
// (c) Dextrous Robotics
// Last updated: August 8, 2023

// this code is for measuring the differential velocity of two points of interest

import processing.serial.*;

Serial COMPort, COMPort_2;  // Create object from Serial class
PFont f;

// First connect the ADNS 3080, followed by the motor 

String[] lines = new String[0];
String[] lines_2 = new String[0];
String boo = "boo";
int StartTime = millis();
int currentTime;
int ElapsedTime;
int lock =0;


String words = "apple bear cat dog";
String[] mango = split(words, ' ');


String portName = Serial.list()[0];
String portName_2 = Serial.list()[2];
String side_adns = "/dev/ttyUSB0";
String top_adns = "/dev/ttyUSB3";
int experiment_number = 34;

void setup() 
{
  f = createFont("Arial",24,true);
  textFont(f);
  fill(0);
  frameRate(60);
  size(800, 800);
  surface.setTitle("ADNS 3080");
  COMPort = new Serial(this, portName, 9600);
  COMPort_2 = new Serial(this, portName_2, 9600);
  println(portName_2);
  
}

void draw() 
{
  textAlign(CENTER);
  //text("ADNS 3080",width/2,20);
  background(0);
  //read when reset is pressed on Nema 17 
  line(width/2-20, height/2, width/2+20, height/2);
  read_top_sensor();
  read_side_sensor();
  currentTime = millis();
  ElapsedTime = currentTime - StartTime;
  
  //if (ElapsedTime >= 4000){
  //  //intiate the motor
  //  if (lock == 0){
  //    println("----------------------turn on motor");
  //    lock +=1;
  //  }
  //}
  
  if (ElapsedTime >= 60000){
    noLoop();
    text("Done.",width/4,60);
  }
  
}
////////////////////////// Functions  

void read_top_sensor(){
    if (portName_2.equals(top_adns) == true){                                                                                                                                                                                                
    String read_2 = COMPort_2.readString();  // read and store it to string read
    println(read_2);
    lines_2 = append(lines_2, read_2);// append new read to string lines
    saveStrings("../Data/Experiment_34_top.txt", lines_2);//save string to file
    //text(lines_2[0], width/2,40);
    if (read_2!= null){
      text("top sensor:", (width/2)-100,100);
      text(read_2.split("\n")[0], width/2,100);
      fill(0, 408, 612);
      //background(196);
    }
   }
   else {
     println("Correct top port can't be found");
   }  
 }
 
 void read_side_sensor(){
  if (portName.equals(side_adns) == true){                                                                                                                                                                                                
    String read = COMPort.readString();  // read and store it to string read
    println(read);
    lines = append(lines, read);// append new read to string lines  
    saveStrings("../Data/Experiment_34_side.txt", lines);//save string to file
    if (read != null){
      text("side sensor:", (width/2)-100,40);
      text(read.split("\n")[0], width/2,40);
      fill(0, 408, 612);
      //background(196);
    }
   } 
    else {  
     println("Correct side port can't be found");
   }  
 }

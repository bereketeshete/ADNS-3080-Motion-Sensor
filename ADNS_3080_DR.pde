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
String side_adns = "/dev/ttyUSB1";
String top_adns = "/dev/ttyUSB4";
int experiment_number = 34;
int diff_velocity;
int top_v, side_v;
int move_x, move_y;

void setup() 
{
  f = createFont("Arial",60,true);
  textFont(f);
  fill(0);
  frameRate(15); //15
  size(1400, 800);
  surface.setTitle("ADNS 3080");
  COMPort = new Serial(this, portName, 9600);
  COMPort_2 = new Serial(this, portName_2, 9600);
  println(portName_2);
  background(0);
  
 
  
}

void draw() 
{
  textAlign(CENTER);
  //text("ADNS 3080",width/2,20);
  //background(0);
  
  stroke(0);
  // regions for refresh
  fill(0);
  rect(580, 180, 200, 200);
  //rect(180, 460, 500, 150);
  line(780,460+75,1280,460+75);
  rect(880-50, 460-200, 400, 900);
  stroke(#FF0F03);
  
  
  //stroke(0);
 
      
  //read when reset is pressed on Nema 17 
  //line(width/2-20, height/2, width/2+20, height/2);
  top_v = read_top_sensor();
  side_v = read_side_sensor();
  currentTime = millis();
  ElapsedTime = currentTime - StartTime;
  //decide_movt();
  
  fill(#C48A43);
  rect(880, 460-top_v, 300, 150);
  line(780,460+75,1280,460+75);
  
  
  //if (ElapsedTime >= 60000){
  //  noLoop();
  //  text("Done.",width/4,60);
  //}
  
}
////////////////////////// Functions  

int read_top_sensor(){
    int good_2 = 0;
    if (portName_2.equals(top_adns) == true){                                                                                                                                                                                                
    String read_2 = COMPort_2.readString();  // read and store it to string read
    println(read_2);
    lines_2 = append(lines_2, read_2);// append new read to string lines
    saveStrings("../Data/Experiment_34_top.txt", lines_2);//save string to file
    //text(lines_2[0], width/2,40);
    
    if (read_2!= null){
      
      fill(0, 408, 612);
      //fill(0);
      //rect(360, 20, 120, 120);
      text(" top sensor :  ", (width/2)-300,240);
      text(read_2.split("\n")[0], width/2 - 20,240);
      fill(0, 408, 612);
      //background(196);
      good_2 = int(read_2.split("\n")[0]);
    }
   }
   else {
     println("Correct top port can't be found");
   }  
   return good_2;
 }
 
 int read_side_sensor(){
  int good = 0;
  if (portName.equals(side_adns) == true){                                                                                                                                                                                                
    String read = COMPort.readString();  // read and store it to string read
    println(read);
    lines = append(lines, read);// append new read to string lines  
    saveStrings("../Data/Experiment_34_side.txt", lines);//save string to file
    if (read != null){
      fill(0, 408, 612);
      //fill(0);
      //rect(360, 20, 120, 120);
      //background(196);
      text("side sensor :  ", (width/2)-300,340);
      text(read.split("\n")[0], width/2 - 20,340);
      fill(0, 408, 612);
      good = int(read.split("\n")[0]);
    }
   } 
    else {  
     println("Correct side port can't be found");
   }  
   return good;
 }
 
 void decide_movt(){
   
   diff_velocity = side_v + top_v;
   
   if (diff_velocity > 1 && ( abs(side_v) > 0 && abs(top_v) > 0 ) ){
     text("Translational", (width/2)-300,540);
   }
   else if (diff_velocity < 2 && ( abs(side_v) > 0 || abs(top_v) > 0 ) ){
     text("Rotational", (width/2)-300,540);
   }
   else
      text("No movement", (width/2)-300,540);
   
   
 }
 
 void program_motor(){
     //if (ElapsedTime >= 4000){
  //  //intiate the motor
  //  if (lock == 0){
  //    println("----------------------turn on motor");
  //    lock +=1;
  //  }
  //}
 }

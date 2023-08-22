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
String portName = Serial.list()[0]; // side adns
String portName_2 = Serial.list()[3];

String side_adns = "/dev/ttyUSB0";
String top_adns = "/dev/ttyUSB3";
int experiment_number = 34;
int diff_velocity;
int top_v, side_v;
int move_x, move_y;
int side_x, side_y, side_touch, side_squal;
int top_x, top_y, top_touch, top_squal;


void setup() 
{
  //f = createFont("Laksaman-48",55);
  f = createFont("Chandas",55);
  textFont(f);
  fill(0);
  frameRate(15); //15
  size(1400, 800);
  surface.setTitle("ADNS 3080");
  COMPort = new Serial(this, portName, 9600);
  COMPort_2 = new Serial(this, portName_2, 9600);
  println(portName_2);
  
  background(0);
  //DR LOGO
  textFont(f, 30);
  fill(#EAF21D);
  text("DEXTROUS ", (width/2)+500,40);
  text(" ROBOTICS", (width/2)+500,80);
  textFont(f, 55);
}


void draw() 
{
  textAlign(CENTER);
  text("ADNS 3080 GUI for R&D",width/2,50);
  //background(0);
  stroke(0);
  
  // regions for refresh
  fill(0);
  rect(480, 180, 400, 200); // data rectangle
  rect(80, 180, 400, 200); // data descrip rectangle
  //rect(180, 460, 500, 150);
  line(780,460+75,1280,460+75);
  rect(880-150, 100, 600, 900);// box GUI rectangle
  
  //center of rotation box
  rect(200, 380, 400, 350);
  stroke(#FF0F03);
  //stroke(0);
  //read when reset is pressed on Nema 17 
  //line(width/2-20, height/2, width/2+20, height/2);
  
  top_v = read_top_sensor();
  side_v = read_side_sensor();
  center_of_rotation(side_v,top_v);
  currentTime = millis();
  ElapsedTime = currentTime - StartTime;
  //decide_movt();
  
  line(780,460+75,1280,460+75);
  box_GUI(top_touch,side_touch);
  //if (ElapsedTime >= 60000){
  //  noLoop();
  //  text("Done.",width/4,60);
  //}
  
}
////////////////////////// Functions  

void box_GUI(int top_contact, int side_contact){
  stroke(#FF0F03);
  fill(#C48A43);
  //translate(10, 10);
  //translate(8, 4);
  //rotate(PI/4);
  //rectMode(CENTER);
  rect(880+top_y*2, 460-top_v*3, 300, 150);
  //rect(880+top_y, 460, 300, 150);
  
  stroke(0);
  
  if (side_contact ==1 ){
  fill(#13E822);
  circle(880+75+top_y*3,460+75-top_v*3,40);// top circle
  }
  if (top_contact == 1){
  fill(#13E822);
  circle(880+225+top_y*3,460+75-top_v*3,40);// side circle
  }
  fill(#C48A43);
  
}

int read_top_sensor(){
    //int good_2 = 0;
    if (portName_2.equals(top_adns) == true){         
    //if(true){
    String read_2 = COMPort_2.readString();  // read and store it to string read
    println(read_2);
    lines_2 = append(lines_2, read_2);// append new read to string lines
    saveStrings("../Data/Experiment_34_top.txt", lines_2);//save string to file
    //text(lines_2[0], width/2,40);
    
    if (read_2!= null){
      // fill(0, 408, 612);  /cyan
      fill(#FCFCFC);
      //fill(0);
      //rect(360, 20, 120, 120);
      text(" top sensor :  ", (width/2)-400,240);
      text(read_2.split("\n")[0], width/2 - 20,240);
      fill(0, 408, 612);
      //background(196);
      //good_2 = int(read_2.split("\n")[0]);
      top_x = int(read_2.split(",")[0]);
      top_y = int(read_2.split(",")[1]);
      top_touch = int(read_2.split(",")[2]);
    }
    
   }
   else {
     println("Correct top port can't be found");
   }  
   return top_x;
 }
 
 int read_side_sensor(){
  //int good = 0;
  if (portName.equals(side_adns) == true){                                                                                                                                                                                                
    String read = COMPort.readString();  // read and store it to string read
    println(read);
    lines = append(lines, read);// append new read to string lines  
    saveStrings("../Data/Experiment_34_side.txt", lines);//save string to file
    if (read != null){
      //fill(0, 408, 612);
      //fill(0);
      fill(#FCFCFC);
      //rect(360, 20, 120, 120);
      //background(196);
      text("side sensor :  ", (width/2)-400,340);
      text(read.split("\n")[0], width/2 - 20,340);
      //fill(0, 408, 612);
      //good = int(read.split("\n")[0]);
      side_x = int(read.split(",")[0]);
      side_touch = int(read.split(",")[2]);
      
    }
   } 
    else {  
     println("Correct side port can't be found");
   }  
   return side_x;
 }
 
 void decide_movt(){
   diff_velocity = side_v + top_v;
   if (diff_velocity > 1 && ( abs(side_v) > 0 && abs(top_v) > 0 ) ){
     text("Translational", (width/2)-300,440);
   }
   else if (diff_velocity < 2 && ( abs(side_v) > 0 || abs(top_v) > 0 ) ){
     text("Rotational", (width/2)-300,440);
   }
   else
      text("No movement", (width/2)-300,440);
 }
 
 
 // motor control for experiment
 void program_motor(){
     //if (ElapsedTime >= 4000){
  //  //intiate the motor
  //  if (lock == 0){
  //    println("----------------------turn on motor");
  //    lock +=1;
  //  }
  //}
 }
 
 
 float center_of_rotation(int m1, int m2){
   float cor = 0;
   float ratio = 0;
   int x_1 =-2 , x_2  = 2;
   
   if (m2!=0){
     ratio = m1/m2;
   }
   else
     ratio = 0;
   textFont(f, 30);
   text(" center of rotation ", (width/2)-300,540);
   text("("+cor+",0.0)", (width/2)-300,600);
   textFont(f, 55);
   
   if (ratio!=-1){
       cor = (x_1 + ratio * x_2)/(1+ratio); // / (1 + m1/m2);
       return cor;
   }
   else
   return cor;
 }
 
 void rotate_the_box(){
   
 }
 
 
 // angle of rotation using only vertical velocity
 int angle_of_rot(int top_y){
   int angle = top_y/150;
   return angle;
 }

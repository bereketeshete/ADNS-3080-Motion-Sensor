import processing.serial.*;

Serial COMPort, COMPort_2;  // Create object from Serial class

// First connect the ADNS 3080, followed by the motor 

String[] lines = new String[0];
String[] lines_2 = new String[0];

int StartTime = millis();
int currentTime;
int ElapsedTime;
int lock =0;


String portName = Serial.list()[0];
String portName_2 = Serial.list()[2];
String side_adns = "/dev/ttyUSB0";
String top_adns = "/dev/ttyUSB2";

void setup() 
{
  frameRate(60);
  size(200, 200);
  COMPort = new Serial(this, portName, 9600);
  COMPort_2 = new Serial(this, portName_2, 9600);
  println(portName_2);
}

void draw() 
{
  //read when reset is pressed on Nema 17 
  if (portName.equals(side_adns) == true){                                                                                                                                                                                                
    String read = COMPort.readString();  // read and store it to string read
    println(read);
    lines = append(lines, read);// append new read to string lines  
    saveStrings("Data/Experiment_30_side.txt", lines);//save string to file
   } 
    else {  
     println("Correct side port can't be found");
   }  
   
   //if (portName_2.equals(top_adns) == true){                                                                                                                                                                                                
   // String read_2 = COMPort_2.readString();  // read and store it to string read
   // println(read_2);
   // lines_2 = append(lines_2, read_2);// append new read to string lines
   // saveStrings("Data/Experiment_30_top.txt", lines_2);//save string to file
   //}
   //else {
   //  println("Correct top port can't be found");
   //}  
    
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

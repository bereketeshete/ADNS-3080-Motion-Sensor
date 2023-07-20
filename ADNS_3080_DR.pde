import processing.serial.*;

Serial COMPort;  // Create object from Serial class

String[] lines = new String[0];

void setup() 
{
  frameRate(5);
  String portName = Serial.list()[0];
  COMPort = new Serial(this, portName, 9600);
}

void draw() 
{
  //read when reset is pressed on Nema 17 
  if (COMPort.available() > 0) {  // If data is available,
    String read = COMPort.readString();  // read and store it to string read
    //read = "value : " + read;
    println(read);
    lines = append(lines, read);// append new read to string lines
  } else {
    saveStrings("Experiment 19.txt", lines);//save string to file
  }
}

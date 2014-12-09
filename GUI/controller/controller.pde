import processing.serial.*;
import controlP5.*;


Serial myPort;  // Create object from Serial class
Serial myPort2;
int val;        // Data received from the serial port
int emergencyStop = 0;
int feedSpeed = 0;
String status = "Idle";
int running = 0;
char portVal;

ControlP5 cp5;
Slider slider;

void setup() 
{
  size(1080, 720);
  
  cp5 = new ControlP5(this);
  slider = new Slider(cp5, "Speed Control");
  slider.setPosition(100, 250);
  slider.setWidth(250);
  slider.setHeight(25);
  slider.setRange(0, 100);
  slider.setNumberOfTickMarks(5);
  slider.setSliderMode(Slider.FLEXIBLE);
  
  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  String portName = Serial.list()[1];
  println(Serial.list()[1]);
  myPort = new Serial(this, portName, 115200);
  
  portName = Serial.list()[0];
  println(Serial.list()[0]);
  myPort2 = new Serial(this, portName, 115200);
}

void draw() {
  background(30, 30, 30);
  
  title();
  status();
  console();
  speedControl();
  emergencyStopButton();
  
  if(myPort.available() > 0){
    if(myPort.read() == 'B')
      emergencyStop();    
  }
  if(myPort2.available() > 0){
    if(myPort2.read() == 'B')
      emergencyStop();
  }
  
  if(feedSpeed != slider.getValue()){
     feedSpeed = (int) slider.getValue();
     println(feedSpeed);
     println(feedSpeed);
     switch(feedSpeed){
       case 0:
         myPort.write('0');
         break;
       case 25:
         myPort.write('1');
         
         println("ASDFSAFD");
         break;
       case 50:
         myPort.write('2');
         break;
       case 75:
         myPort.write('3');
         break;
       case 100:
         myPort.write('4');
         break;
       default:
         myPort.write('0');
         break;
     } 
  }
}

void status(){
  fill(255);
  textSize(24);
  text("Status: " + status, 100, 175, 5);
}

void console(){
  fill(255);
  textSize(24);
  text("Console", 100, 430, 5);
  rect(100, 440, 600, 200);
}

void speedControl(){
  fill(255);
  textSize(24);
  text("Feed Speed: " + slider.getValue() + "%", 100, 240, 5); 
}

void title(){
  fill(255);
  textSize(48);
  text("Unwinder-Rewinder Controller", 180, 100, 5);
}

void emergencyStop(){
  running = 0;
  slider.setValue(0);
   println("STOP!!!");
   status = "Idle";
   myPort.write('L');
}

void emergencyStopButton(){
  if(running == 0){
     fill(0, 255, 0);
     rect(800, 440, 200, 200);
      
     fill(255);
     textSize(24);
     text("Start", 875, 545, 5);
  }
  else{
    if(mousePressed && mouseX >= 800 && mouseX <= 1000 && mouseY >= 440 && mouseY < 640){
      fill(100, 0, 0);
      rect(800, 440, 200, 200);
      emergencyStop = 1;
      
      fill(255);
      textSize(24);
      text("Emergency Stop", 807, 545, 5);
    }
    else{   
      fill(255, 0, 0);
      rect(800, 440, 200, 200);
      
      fill(255);
      textSize(24);
      text("Emergency Stop", 807, 545, 5);
    }
  }
}

void start(){
  
}

void mouseReleased(){
  if(running == 0 && mouseX >= 800 && mouseX <= 1000 && mouseY >= 440 && mouseY < 640){
    running = 1;
    slider.setValue(25.0);
    status = "Running";
    myPort.write('H');
  }
  else if(running == 1 && emergencyStop == 1 && mouseX >= 800 && mouseX <= 1000 && mouseY >= 440 && mouseY < 640){
     emergencyStop();
  }
  else{
    emergencyStop = 0;
  }
}

/*
  // Wiring/Arduino code:
 // Read data from the serial and turn ON or OFF a light depending on the value
 
 char val; // Data received from the serial port
 int ledPin = 4; // Set the pin to digital I/O 4
 
 void setup() {
 pinMode(ledPin, OUTPUT); // Set pin as OUTPUT
 Serial.begin(9600); // Start serial communication at 9600 bps
 }
 
 void loop() {
 if (Serial.available()) { // If data is available to read,
 val = Serial.read(); // read it and store it in val
 }
 if (val == 'H') { // If H was received
 digitalWrite(ledPin, HIGH); // turn the LED on
 } else {
 digitalWrite(ledPin, LOW); // Otherwise turn it OFF
 }
 delay(100); // Wait 100 milliseconds for next reading
 }
 
 */

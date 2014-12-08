import processing.serial.*;
import controlP5.*;


Serial myPort;  // Create object from Serial class
int val;        // Data received from the serial port
int emergencyStop = 0;
float feedSpeed = 0;

ControlP5 cp5;
Slider slider;

void setup() 
{
  size(1080, 720);
  
  cp5 = new ControlP5(this);
  slider = new Slider(cp5, "Speed Control");
  slider.setPosition(200, 250);
  slider.setWidth(250);
  slider.setHeight(25);
  slider.setRange(0, 100);
  slider.setNumberOfTickMarks(5);
  slider.setSliderMode(Slider.FLEXIBLE);
  
  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 9600);
}

void draw() {
  background(30, 30, 30);
  
  title();
  speedControl();
  emergencyStopButton();
}

void speedControl(){
  fill(255);
  textSize(24);
  text("Feed Speed: " + slider.getValue() + "%", 200, 240, 5); 
}

void title(){
  fill(255);
  textSize(48);
  text("Unwinder-Rewinder Controller", 180, 100, 5);
}

void emergencyStop(){
  slider.setValue(0);
   println("STOP!!!"); 
}

void emergencyStopButton(){
  if(mousePressed && mouseX >= 800 && mouseX <= 1000 && mouseY >= 440 && mouseY < 640){
    fill(100, 0, 0);
    emergencyStop = 1;
  }
  else{   
    fill(255, 0, 0);
  }
  
  rect(800, 440, 200, 200);
  
  fill(255);
  textSize(24);
  text("Emergency Stop", 807, 545, 5);
}

void mouseReleased(){
  if(emergencyStop == 1 && mouseX >= 800 && mouseX <= 1000 && mouseY >= 440 && mouseY < 640){
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

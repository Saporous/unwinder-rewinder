// PWM library provides nice functions to initialize timers
#include <PWM.h>
// NewPing library is for the ultrasonic sensor
#include <NewPing.h>

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

int motor = 9;           // Arduino pin tied to Duncan motor that has an internal amplifier
int feeder = 10;         // Arduino pin tied to motor/amplifier module
int frequency = 20000;   // A frequency for the PWM that works with the motor - values greater than 16000 cannot be heard
int running = 0;          // a flag set by the GUI to control whether the motors run based upon dancer arm position
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

/*===============================================
 Runs once to initialize ports and begin communication
 115200 baud rate was given by NewPing example
 ===============================================*/
void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  InitTimersSafe();     // Start Timers 1 and 3

  //sets the frequency for the specified pin
  bool success = SetPinFrequencySafe(motor, frequency);

  //if the pin frequency was set successfully, turn pin 10 on
  if(success) {
    pinMode(motor, OUTPUT);
    digitalWrite(motor, LOW);    
  } 

  bool success2 = SetPinFrequencySafe(feeder, frequency); 
  if(success2) {  
    pinMode(feeder, OUTPUT);
    digitalWrite(feeder, LOW);
  }
}

void loop() {
  // Wait 10ms between pings (about 100 pings/sec).
  delay(10);
  // Send ping, get ping time in microseconds (uS).
  unsigned int distance = sonar.ping(); 

  // Print output, can be viewed with Serial Monitor (default Ctrl + Shift + M to open it)
  // US_ROUNDTRIP_CM = 57 in config
  //Serial.print("Ping: ");
  // Convert ping time to distance in cm and print result (0 = outside set distance range)
  //Serial.print(distance);// / US_ROUNDTRIP_CM); 
  //Serial.println("cm");

  // assign different speeds to the internal PWM to control the DC motor connected to the Amplifier module
  if(Serial.available()){
    int temp = (Serial.read() - 48);
    switch(temp){
    case 0:
      pwmWrite(feeder, 0);
      running = 0;
      break;
    case 1:
      pwmWrite(feeder, 220);
      running = 1;
      break;
    case 2:
      pwmWrite(feeder, 235);
      running = 1;
      break;
    case 3:
      pwmWrite(feeder, 245);
      running = 1;
      break;
    case 4:
      pwmWrite(feeder, 255);
      running = 1;
      break;
    default:
      pwmWrite(feeder, 0);  
      running = 0;
      break;
    }
  }


  // Approximation to shut off motor and report error once within about 5 cm
  if(running == 1){
    if(distance < 150){ 
      pwmWrite(motor, 0);
      running = 0;
      Serial.write('B');
    }
    // Values need to be tweaked
    else if(distance >= 150 && distance < 250)
      pwmWrite(motor, 90);
    else if(distance >= 250 && distance < 350)
      pwmWrite(motor, 85);
    else if(distance >= 350 && distance < 500)
      pwmWrite(motor, 75);
    else if(distance >= 500 && distance < 650)
      pwmWrite(motor, 70);
    else if(distance >= 650 && distance < 800)
      pwmWrite(motor, 65);
    else if(distance >= 800)
      pwmWrite(motor, 0);
  }
  // do not run motor if GUI signals an error condition
  else{
    pwmWrite(motor, 0);
  }
}








#include <PWM.h>
// NewPing library is for the ultrasonic sensor
#include <NewPing.h>

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

int motor = 9; // Arduino pin tied to motor/MOS SIG pin on the ultrasonic sensor.
int feeder = 10;
int frequency = 20000; // A frequency for the PWM that works with the motor - needs tuning

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

/*===============================================
 Runs once to initialize ports and begin communication
 115200 baud rate was given by NewPing example
 ===============================================*/
void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  InitTimersSafe(); 
  /*  
   //sets the frequency for the specified pin
   bool success = SetPinFrequencySafe(motor, frequency);
   
   //if the pin frequency was set successfully, turn pin 10 on
   if(success) {
   pinMode(motor, OUTPUT);
   digitalWrite(motor, LOW);    
   } 
   */
  bool success2 = SetPinFrequencySafe(feeder, frequency); 
  if(success2) {  
    pinMode(feeder, OUTPUT);
    digitalWrite(feeder, HIGH);
  }
}

void loop() {
  // Wait 5ms between pings (about 200 pings/sec).
  delay(50);
  // Send ping, get ping time in microseconds (uS).
  unsigned int distance = sonar.ping(); 

  // Print output, can be viewed with Serial Monitor (default Ctrl + Shift + M to open it)
  Serial.print("Ping: ");
  // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.print(distance);// / US_ROUNDTRIP_CM); 
  Serial.println("cm");

  if(Serial.available()){
    int temp = (Serial.read() - 48);
    switch(temp){
      case 0:
        pwmWrite(feeder, 0);
        break;
      case 1:
        pwmWrite(feeder, 200);
        break;
      case 2:
        pwmWrite(feeder, 215);
        break;
      case 3:
        pwmWrite(feeder, 230);
        break;
      case 4:
        pwmWrite(feeder, 255);
        break;
      default:
        pwmWrite(feeder, 0);  
        break;
    }
    Serial.println(temp);
  }
  // US_ROUNDTRIP_CM = 57 in config
  // Approximation to shut off motor once within about 5 cm
   if(distance < 150){ 
   pwmWrite(motor, 0);
   }
   // Values need to be tweaked - dependent on frequency of PWM
   else if(distance >= 150 && distance < 250)
   pwmWrite(motor, 150);
   else if(distance >= 250 && distance < 350)
   pwmWrite(motor, 175);
   else if(distance >= 350 && distance < 500)
   pwmWrite(motor, 200);
   else if(distance >= 500 && distance < 700)
   pwmWrite(motor, 240);
   else if(distance >= 700 && distance < 1000)
   pwmWrite(motor, 255);
   else 
   pwmWrite(motor, 255);
}



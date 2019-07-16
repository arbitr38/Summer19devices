#include <Stepper.h>

const int stepsPerRevolution = 200; // change this to fit the number of steps per revolution
// for your motor
//2050 = 360deg
//1025 = 180deg
//512.5 = 90deg
//5.6944 = 1deg

// initialize the stepper library on pins 8 through 11:
// the order of the numbers below is important, 8,10,11,9 does not equal 8,9,10,11,
//these numbers correspond to the pins on the digital side of the arduino uno
// 1N1 on pin 8
// 1N2 on pin 9
// 1N3 on pin 10
// 1N4 on pin 11
// powered via 5v
// Max speed is 14RPM @ 5v

Stepper myStepper(stepsPerRevolution, D2,D3,D4,D5);

void setup() {
// set the speed at 14 rpm:
myStepper.setSpeed(14);
// initialize the serial port:
Serial.begin(9600);
}

void loop() {
// step one revolution in one direction:
Serial.println("clockwise");
myStepper.step(stepsPerRevolution);
delay(500);

// step one revolution in the other direction:
Serial.println("counterclockwise");
myStepper.step(-stepsPerRevolution);
delay(500);
}

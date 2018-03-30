// BWB Hydration Monitor, brought to you by #TeamAntiThirst
// Authors: Caleb Perez and Annapurni Sriram
// Version 1.1, updated 03.29.2018
// This program reads the voltage of two output pins -- the two receiver transducers -- and uses the built-in Timer1 clock to track the time b/t signals.
// Uses TimerOne Library, linked here: https://playground.arduino.cc/Code/Timer1

#include <TimerOne.h>

// Setup two receiver pins as 2 and 4, presumably connected to each receiver transducers
const int receiverPin1 = 2;
const int receiverPin2 = 4;

// Setup several variables for computing deltaT
int state1 = 0; // states of pins -- high or low
int state2 = 0;
int prevState1 = 0;
int prevState2 = 0;
long counter1 = 0.0;  // stores Timer1 counter values
long counter2 = 0.0;
long deltaT = 0.0;
int period = 1; // period for timer in usec --> should be changed depending on time b/t signals -- we want this to be large enough so the clock doesn't rollover multiple times b/t measurements

void setup() {
  pinMode(receiverPin1, INPUT);
  pinMode(receiverPin2, INPUT);

  Timer1.initialize(period);
  
  Serial.begin(9600);
}

void loop() {
  // Read voltages of each pin
  state1 = digitalRead(receiverPin1);
  state2 = digitalRead(receiverPin2);

  // Update counters if signal is detected (i.e. state switches from LOW --> HIGH)
  if (state1 != prevState1) { // checks for switch in state, so we don't recalculate while signal is continously high
    if (state1 == HIGH) {
      counter1 = Timer1.read();
    } else {
      // not sure if we should do anything when switching to LOW state -- perhaps clear interrupts?
    }
  }

  if (state2 != prevState2) {
    if (state2 == HIGH) {
      counter2 = Timer2.read();
    } else {
      // not sure if we should do anything when switching to LOW state -- perhaps clear interrupts?
    }
  }

  // Update previous states for next iteration through loop
  prevState1 = state1;
  prevState2 = state2;

  // Calculate deltaT if measurements have been recorded in both counter1 and counter2
  if (counter1 != 0.0 && counter2 != 0.0) {
    if (counter2 < counter1) { // Check for clock rollover between measurements, and alter math if this occurs
      deltaT = counter2 + period - counter1;
    } else {
      deltaT = counter2 - counter1;
    }
    // reset counter1 and counter2 to initial states for future measurements
    counter1 = 0.0;
    counter2 = 0.0;
  }

  // Math we have yet to figure out
  hydration = deltaT * hydrationFactor;

  // May also be worth it to store "hydration" in an array, take multiple measurements, and then average -- can be easily added later once our logic here is validated
}

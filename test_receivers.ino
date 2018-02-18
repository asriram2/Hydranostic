#include <TimerOne.h>

const int receiverPin1 = 2;
const int receiverPin2 = 4;

int cycleCount = 0;
int state1 = 0;
int state2 = 0;
int prevState1 = 0;
int prevState2 = 0;
long counter1 = 0.0;
long counter2 = 0.0;
long deltaT = 0.0;
int period = 1;

void setup() {
  // put your setup code here, to run once:
  pinMode(receiverPin1, INPUT);
  pinMode(receiverPin2, INPUT);

  Timer1.initialize(period); // period in usec --> probs want to change this depending on time b/t signals
  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  state1 = digitalRead(receiverPin1);
  state2 = digitalRead(receiverPin2);

  if (state1 != prevState1) {
    if (state1 == HIGH) {
      counter1 = Timer1.read();
    } else {
      // perhaps clear interrups?
    }
  }

  if (state2 != prevState2) {
    if (state2 == HIGH) {
      counter2 = Timer2.read();
    } else {
      // dunno
    }
  }

  prevState1 = state1;
  prevState2 = state2;

  if (counter2 < counter1) {  // check for rollover between measurements
    deltaT = counter2 + period - counter1;
  } else {
    deltaT = counter2 - counter1;
  }

  hydration = deltaT * hydrationFactor;
}

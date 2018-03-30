// BwB Hydration Monitor brought to you by #TeamAntiThirst
// Updated: 03.29.2018 by A. Sriram
// Authors: Caleb Perez and Annapurni Sriram
// This program generates a 2 MHz signal through output pin 3 using the ATmega chip registers.
// Code was adapted from: https://forum.arduino.cc/index.php?topic=65434.0 and http://forum.arduino.cc/index.php/topic,62964.0.html

const int freqOutputPin = 3; 
// The OCR2A pin is the counter which will cause the counter to reset when encountered. ocra2aval is the value later assigned to OCR2A.
// This sets the frequency of the signal. Without pre-scaling, an OCRA2A value of 3 the output pin to toggle the value every four CPU 
// clock cycles. That is, the period is equal to eight clock cycles.
//
// With F_CPU = 16 MHz, the result is 2 MHz.
const int ocr2aval  = 3; 

// These are not needed to generate the code.  They are simply to remind the user
// what the frequency is.  Note that changing the value of "prescale" here does
// not affect the counter operation.  It's just for the printout vlaue
const int prescale  = 1; // no prescaling
const float period    = 2.0 * prescale * (ocr2aval+1) / (F_CPU/1.0e6);
const float freq      = 1.0e6 / period;

void setup()
{
   pinMode(freqOutputPin, OUTPUT);
   Serial.begin(9600);

   // Toggle OC2B on "compare match".
   // Normal count mode with OCRA as "top"
   TCCR2A = ((1 << WGM21) | (1 << COM2B0));

   // Prescale divider value = 1
   // If you want to change the prescale value, look at the ATmega
   // data sheet for the appropriate values of CS22:0
   TCCR2B = (1 << CS20);

   // Not really needed here, since TIMSK is initialized
   // to zero, but for general use, it's probably not
   // a bad idea to make sure timer 2 interrupts are
   // disabled.
   TIMSK2 = 0;
 
   // This is the "top" value at which the counter will reset
   OCR2A = ocr2aval;

   Serial.print("Period    = ");
   Serial.print(period); 
   Serial.println(" microseconds");
   Serial.print("Frequency = ");
   Serial.print(freq); 
   Serial.println(" Hz");
}

// Timer 2 Output is directly connected to the designated OCR pin, so
// program intervention is not required to do anything with it.
//
void loop(){

   
   // Do just about anything you want to here, but if you mess
   // with Timer 2, you may affect the output on its OCR pin.
   //
   
}

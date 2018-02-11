/*********************

Example code for the Adafruit RGB Character LCD Shield and Library

This code displays text on the shield, and also reads the buttons on the keypad.
When a button is pressed, the backlight changes color.

**********************/

// include the library code:
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>


// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// These #defines make it easy to set the backlight color
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7


const int S_START = 0;
const int S_INSTRUCT = 1;
const int S_SAMPLE = 2;
const int S_ANALYZE = 3;
const int S_RESULT = 4;
const int S_Hydra = 5;
const int S_MDeh = 6;
const int S_SDeh = 7;
const int S_MOD = 8;

uint8_t i=0;
int buzz = 6;

void setup() {
  // Debugging output
  Serial.begin(9600);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  

  
  // Print a message to the LCD. We track how long it takes since
  // this library has been optimized a bit and we're proud of it :)
  int time = millis(); //Unused, but may be useful later
  pinMode(buzz, OUTPUT);
}


void loop() 
{
  static int state = S_START; //Initializes in Start State
  


  uint8_t buttons = lcd.readButtons();
  static int set = 0;

  switch (state)
  {
    case S_START:
      //Beginning of the test - Waits for user input

      if (set == 0)
      {
        lcd.clear();
        lcd.setCursor(0, 0);  
        lcd.print("Begin Test?"); //Prompts user to start test
        lcd.setBacklight(WHITE);
        set = 1;
        Serial.print("Set: ");Serial.println(set); //Diagnostics
      } 

      if (buttons) {
        if (buttons & BUTTON_SELECT){

          set = 0;
          Serial.print("Set: ");Serial.println(set); //Diagnostics
          state = S_INSTRUCT; //Moves to start test when play is pushed
          delay(250);
        }

        else{
          set = 0;
          state = S_MOD; //Moves to modification menu if other button is pushed
          delay(250);
        }
      }


      
      break;

    case S_INSTRUCT:
      // Start of test, tells user to prepare instrument

      if (set == 0){
      lcd.clear();
      lcd.setCursor(0, 0);  
      lcd.print("Place Wand"); //Tells user step
      lcd.setCursor(0, 1);
      lcd.print("Press Play");
      lcd.setBacklight(WHITE);
      set = 1;
      }
      
       if (buttons)
       {

         set = 0;
         state = S_SAMPLE; //Moves to sampling state when the user indicates 
         delay(250);
         
        }
   

      
      break;

    case S_SAMPLE:
    // Sends and recieves signals to the transducers

      if (set == 0) //Updates display once, but prevents flashing with each loop
      {
        lcd.clear();
        lcd.setCursor(0, 0);  
        lcd.print("Taking Sample"); //Tells user step
        lcd.setCursor(0, 1);
        lcd.print("Please wait");
        lcd.setBacklight(WHITE);
        set = 1;
      } 


      for (int i = 0; i<=4; i++){
        
        tone(buzz, 120, 750);
        delay(1250);
        
      }
      //This area is where the code for the transmitter/reciever circuit
      //Need transmission at ~1MHZ (depending on Ultrasound Team)
      //Takes signal from two ports - must create a voltage trigger - this can be tested with a function generator
      //Logs the time between the two signals


      state = S_ANALYZE; 
      set = 0;
      delay(1000);

      
   
      break;
      
     case S_ANALYZE:
      // Computes stored data

 if (set == 0) //Updates display once, but prevents flashing with each loop
      {
      lcd.clear();
      lcd.setCursor(0, 0);  
      lcd.print("Thinking"); //Tells user step
      lcd.setCursor(0, 1);
      lcd.print("Please wait");
      lcd.setBacklight(WHITE);
      set = 1;
      }
      
      //This state may not be necessary, but this is where the the Arduino can compute the time logged from the sample step.
      //This will be a matter of computing the differences between the time and translating that to a hydration measurement. This scaling factor will later be determined through experimental data.

      state = S_RESULT;
      set = 0;
      delay(1000);
    
      
      
      break;


   
     case S_RESULT:
      //Provides output to user
     

       if (buttons)
          {
            if (buttons & BUTTON_SELECT){
          lcd.clear();
          lcd.setCursor(0, 0);  
          lcd.print("Hydrated"); //Tells user step
          lcd.setCursor(0, 1);
          lcd.print(" ");
          lcd.setBacklight(BLUE);
          
          state = S_Hydra;
          set = 0;
          delay(500);
     
          
      }
    else if (buttons & BUTTON_DOWN)
      {
          lcd.clear();
          lcd.setCursor(0, 0);  
          lcd.print("Severe"); //Tells user step
          lcd.setCursor(0, 1);
          lcd.print("Dehydration");
          lcd.setBacklight(RED);

          state = S_SDeh;
          set = 0;
          delay(500);
          
        }
        else{
          lcd.clear();
          lcd.setCursor(0, 0);  
          lcd.print("Mild"); //Tells user step
          lcd.setCursor(0, 1);
          lcd.print("Dehydration");
          lcd.setBacklight(YELLOW);
          
          state = S_MDeh;
          set = 0;
          delay(500);
        }
     
      }
          
      //I just provided to example options, we need to add one more "yellow" light situation   
    
      break;



      case S_Hydra:

      if (set == 0){

          lcd.clear();
          lcd.setCursor(0, 0);  
          lcd.print("Hydrated"); //Tells user step
          lcd.setCursor(0, 1);
          lcd.print(" ");
          lcd.setBacklight(BLUE);
          set = 1;
      }

      if (buttons){
        if (buttons & BUTTON_SELECT){

         set = 0;
         state = S_START;
         delay(250);
        }
        else if (buttons & BUTTON_DOWN){

        set = 0;
        state = S_MDeh;
        delay(250);
        }
        else if (buttons & BUTTON_UP){

        set = 0;
        state = S_SDeh;
        delay(250);
        }
        
      }

      break;

      case S_MDeh:

      if (set == 0){

          lcd.clear();
          lcd.setCursor(0, 0);  
          lcd.print("Mild"); //Tells user step
          lcd.setCursor(0, 1);
          lcd.print("Dehydration");
          lcd.setBacklight(YELLOW);
          set = 1;
      }

      if (buttons){
        if (buttons & BUTTON_SELECT){

         set = 0;
         state = S_START;
         delay(250);
        }
        else if (buttons & BUTTON_DOWN){

        set = 0;
        state = S_SDeh;
        delay(250);
        }
        else if (buttons & BUTTON_UP){

        set = 0;
        state = S_Hydra;
        delay(250);
        }
        
      }

      break;

      case S_SDeh:

      if (set == 0){

          lcd.clear();
          lcd.setCursor(0, 0);  
          lcd.print("Severe"); //Tells user step
          lcd.setCursor(0, 1);
          lcd.print("Dehydration");
          lcd.setBacklight(RED);
          set = 1;
      }

      if (buttons){
        if (buttons & BUTTON_SELECT){

         set = 0;
         state = S_START;
         delay(250);
        }
        else if (buttons & BUTTON_DOWN){

        set = 0;
        state = S_Hydra;
        delay(250);
        }
        else if (buttons & BUTTON_UP){

        set = 0;
        state = S_MDeh;
        delay(250);
        }
        
      }

      break;

      case S_MOD:

      if (set == 0) //Updates display once, but prevents flashing with each loop
      {
      lcd.clear();
      lcd.setCursor(0, 0);  
      lcd.print("Expert"); //Tells user step
      lcd.setCursor(0, 1);
      lcd.print("Mode");
      lcd.setBacklight(WHITE);
      set = 1;
      }

   //This is where a "settings" menu can be created to adjust certain parameters in the future. Right now it is just a place holder. 
   
         if (buttons & BUTTON_SELECT)
         {
         set = 0;
         state = S_START;
         delay(250);
         }
        

      
    
     break; 
      
   }//End of switch

}//End of loop
  
  


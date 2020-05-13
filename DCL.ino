//Ozon Led v.21.05.25
#include "FastLED.h"
#include "DMXSerial.h"
#include <avr/pgmspace.h>

// How many leds are in the strip?
#define NUM_LEDS 252

// Data pin that led data will be written out over
#define DATA_PIN 5

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];

//DMX Start Adress
#define DMX_START_ADRESS 151


//Variabels
int DmxInt = 0;  // variable to store the value coming from the DMX channel
int DmxIntOld = 0;  // variable to store the value coming from the DMX channel

int DmxShutter = 0;  // variable to store the value coming from the DMX channel
int DmxShutterOld = 0;  // variable to store the value coming from the DMX channel

int DmxRed = 0;  // variable to store the value coming from the DMX channel
int DmxRedOld = 0;  // variable to store the value coming from the DMX channel

int DmxGreen = 0;  // variable to store the value coming from the DMX channel
int DmxGreenOld = 0;  // variable to store the value coming from the DMX channel

int DmxBlue = 0;  // variable to store the value coming from the DMX channel
int DmxBlueOld = 0;  // variable to store the value coming from the DMX channel

int DmxMacro = 0;  // variable to store the value coming from the DMX channel
int DmxMacroOld = 0;  // variable to store the value coming from the DMX channel

int DmxSpeed = 0;  // variable to store the value coming from the DMX channel
int DmxSpeedOld = 0;  // variable to store the value coming from the DMX channel

int DmxBuffer = 8;  // Buffer size for reading the signal

long Speed = 0; // variable for speed
long previousMillis = 0; //variable for the past milliseconds 

long previousMillisInt = 0; //variable for the past milliseconds
long previousMillisShutter = 0; //variable for the past milliseconds
long previousMillisRed = 0; //variable for the past milliseconds
long previousMillisGreen = 0; //variable for the past milliseconds
long previousMillisBlue = 0; //variable for the past milliseconds
long previousMillisMacro = 0; //variable for the past milliseconds
long previousMillisSound = 0; //variable for the past milliseconds

int SoundSensorValue = 0;

int i = 0;
int i_int = 0;
int i_shutter = 0;
int i_red = 0;
int i_green = 0;
int i_blue = 0;
int i_macro = 0;
int i_speed = 0;

int shutter_bright = 0;
int macro_random_led = 0;

const int rows = 28;
const int colums = 22;

const PROGMEM  uint16_t octaMatrix[rows][colums] = {
 
  {0,246,245,0,0,238,237,0,0,230,229,0,0,222,221,0,0,217,0,0,0,0}, 
  {247,0,0,244,239,0,0,236,231,0,0,228,223,0,0,220,216,0,0,0,0,0}, 
  {248,0,0,243,240,0,0,235,232,0,0,227,224,0,0,219,215,0,0,0,0,0},
  {0,249,242,0,0,241,234,0,0,233,226,0,0,225,218,0,0,214,0,0,0,0},
  {0,118,182,0,0,189,190,0,0,197,198,0,0,205,206,0,0,213,0,0,0,0},
  {180,0,0,183,188,0,0,191,196,0,0,199,204,0,0,207,212,0,0,0,0,0},
  {179,0,0,184,187,0,0,192,195,0,0,200,203,0,0,208,211,0,0,0,0,0},
  {0,178,185,0,0,186,193,0,0,194,201,0,0,202,209,0,0,210,0,0,0,0},
  {0,174,173,0,0,166,165,0,0,0,0,0,0,158,157,0,0,153,0,0,0,0},
  {175,0,0,172,167,0,0,164,0,0,0,0,159,0,0,156,152,0,0,0,0,0},
  {176,0,0,171,168,0,0,163,0,0,0,0,160,0,0,155,151,0,0,0,0,0},
  {0,177,170,0,0,169,162,0,0,0,0,0,0,161,154,0,0,150,0,0,0,0},
  {0,128,129,0,0,134,135,0,0,0,0,0,0,139,140,0,0,145,0,0,0,149},
  {127,0,0,130,133,0,0,136,0,0,0,0,138,0,0,141,144,0,0,0,148,0},
  {126,0,0,131,0,0,0,0,0,0,0,0,137,0,0,142,143,0,0,0,147,0},
  {0,125,132,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,146},
  {0,124,117,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,92,0,0,91},
  {123,0,0,118,116,0,0,111,110,0,0,105,104,0,0,99,98,0,0,93,90,0},
  {122,0,0,119,115,0,0,112,109,0,0,106,103,0,0,100,97,0,0,94,89,0},
  {0,121,120,0,0,114,113,0,0,108,107,0,0,102,101,0,0,96,95,0,0,88},
  {0,47,48,0,0,55,56,0,0,63,64,0,0,71,72,0,0,79,80,0,0,87},
  {46,0,0,49,54,0,0,57,62,0,0,65,70,0,0,73,78,0,0,81,86,0},
  {45,0,0,50,53,0,0,58,61,0,0,66,69,0,0,74,77,0,0,82,85,0},
  {0,44,51,0,0,52,59,0,0,60,67,0,0,68,75,0,0,76,83,0,0,84},
  {0,40,39,0,0,32,31,0,0,24,23,0,0,16,15,0,0,8,7,0,0,256},
  {41,0,0,38,33,0,0,30,25,0,0,22,17,0,0,14,9,0,0,6,1,0},
  {42,0,0,37,34,0,0,29,26,0,0,21,18,0,0,13,10,0,0,5,2,0},
  {0,43,36,0,0,35,28,0,0,27,20,0,0,19,12,0,0,11,4,0,0,3}
 
};


int x = 0;
int y = 0;
int toggle_one = 0;
int volume_x = 0;
int volume_y = 0;

uint8_t gHue = 0; // rotating "base color" used by many of the patterns
#define FRAMES_PER_SECOND  120

// This function sets up the ledsand tells the controller about them
void setup() {
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);
  
  //Init Leds
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  
  //Init DMX
  DMXSerial.init(DMXReceiver);

  //Init Status Led
  pinMode(13, OUTPUT);
 
   //Init Sound Thresold
  pinMode(8, INPUT); 
}

// This function runs over and over, and is where you do the magic to light
// your leds.
void loop() {
  // Milliseconds since arduino is running  
  unsigned long currentMillis = millis();
    
    // Calculate how long no data packet was received
  unsigned long lastPacket = DMXSerial.noDataSince();
  
  if (lastPacket < 5000) {
    // read recent DMX values and set levels 
    digitalWrite(13, HIGH);
    
  
     //Read DMX and buffer it   
     if(DmxIntOld == DMXSerial.read(0+DMX_START_ADRESS)){
        i_int = i_int + 1; 
     }else{
         DmxIntOld = DMXSerial.read(0+DMX_START_ADRESS);
     }
     
     if(i_int == DmxBuffer){
       DmxInt = DmxIntOld;
       i_int = 0;
     }

     //Read DMX and buffer it   
     if(DmxShutterOld == DMXSerial.read(1+DMX_START_ADRESS)){
        i_shutter = i_shutter + 1; 
     }else{
         DmxShutterOld = DMXSerial.read(1+DMX_START_ADRESS);
     }
     
     if(i_shutter == DmxBuffer){
       DmxShutter = DmxShutterOld;
       i_shutter = 0;
     }     
     
     //Read DMX and buffer it   
     if(DmxRedOld == DMXSerial.read(2+DMX_START_ADRESS)){
        i_red = i_red + 1; 
     }else{
         DmxRedOld = DMXSerial.read(2+DMX_START_ADRESS);
     }
     
     if(i_red == DmxBuffer){
       DmxRed = DmxRedOld;
       i_red = 0;
     }
     
     //Read DMX and buffer it   
     if(DmxGreenOld == DMXSerial.read(3+DMX_START_ADRESS)){
        i_green = i_green + 1; 
     }else{
         DmxGreenOld = DMXSerial.read(3+DMX_START_ADRESS);
     }
     
     if(i_green == DmxBuffer){
       DmxGreen = DmxGreenOld;
       i_green = 0;
     }
     
     //Read DMX and buffer it   
     if(DmxBlueOld == DMXSerial.read(4+DMX_START_ADRESS)){
        i_blue = i_blue + 1; 
     }else{
         DmxBlueOld = DMXSerial.read(4+DMX_START_ADRESS);
     }
     
     if(i_blue == DmxBuffer){
       DmxBlue = DmxBlueOld;
       i_blue = 0;
     }
     
   
     //Read DMX and buffer it   
     if(DmxMacroOld == DMXSerial.read(5+DMX_START_ADRESS)){
        i_macro = i_macro + 1; 
     }else{
         DmxMacroOld = DMXSerial.read(5+DMX_START_ADRESS);
     }
     
     if(i_macro == DmxBuffer){
       DmxMacro = DmxMacroOld;
       i_macro = 0;
     }
       
      //Read DMX and buffer it   
     if(DmxSpeedOld == DMXSerial.read(6+DMX_START_ADRESS)){
        i_speed = i_speed + 1; 
     }else{
         DmxSpeedOld = DMXSerial.read(6+DMX_START_ADRESS);
     }
     
     if(i_speed == DmxBuffer){
       DmxSpeed = DmxSpeedOld;
       i_speed = 0;
     }

    
    
    //Limit the intensity if no effect is applied, cause the power consumption is to high
    if(DmxMacro <= 10 and DmxInt >= 200){
        DmxInt= 180;    
    }
    
    //Set Shutter
    if(DmxShutter <=0){
      //Shutter Off
      //Set Brightness
      FastLED.setBrightness(DmxInt); 
    }else if(DmxShutter >= 1 && DmxShutter <= 255){
      //Shutter On
       if(currentMillis - previousMillisShutter > DmxShutter*2) {
          // save the last time you blinked the LED 
          previousMillisShutter = currentMillis;  
          
          if(shutter_bright == 0){
            FastLED.setBrightness(0);
           shutter_bright = 1;
          }else{
            FastLED.setBrightness(DmxInt);
          shutter_bright = 0;
          }
       }
    }
    
    //Speed
    if(DmxSpeed <=230){
    //Speed Mode
    Speed = 5000 - (DmxSpeed*21.7);
    
    }else if(DmxSpeed >= 230 && DmxSpeed <= 245){
    //Sound to Light mode, louder = faster
       Speed = map(analogRead(A0), 512, 1023, 0, 2000);  
       if(Speed <= 1){
         Speed = 1;
       }
    }else if (DmxSpeed >= 246 && DmxSpeed <= 255){
    //Sound to Light beat mode 
        SoundSensorValue = digitalRead(8);

        if(SoundSensorValue == HIGH){
           previousMillis = currentMillis;  
           
        }
        
        if(currentMillis - previousMillis > 10000){
        Speed = 10000;
        }else{
        Speed = currentMillis - previousMillis; 
        }        
        
    }
    
    
    //Macro Selection
    if(DmxMacro <= 10){
      //Normal RGB Mode all Leds 
     for(int i = 0; i <= NUM_LEDS; i = i + 1) {
        // Turn all led on with the choosen color, then show the leds
        leds[i].setRGB( DmxRed, DmxGreen, DmxBlue);
     }
     // Show the leds
     FastLED.show();
      
    }else if(DmxMacro >= 11 && DmxMacro <= 30){
      //Chase Mode move a single led 

     
         if(currentMillis - previousMillisMacro > Speed) {
          // save the last time you blinked the LED 
          previousMillisMacro = currentMillis;  
          
           
        // Turn our current led on to the set color, then show the leds
        leds[i].setRGB( DmxRed, DmxGreen, DmxBlue);
        
        // Show the leds (only one of which is set to color, from above)
        FastLED.show(); 
  
        // Turn our current led back to black for the next loop around
        leds[i].setRGB( 0, 0, 0);
        i = i + 1;
         }
     
      if ( i > NUM_LEDS ){
        i =0; 
      }
      
    }else if(DmxMacro >= 31 && DmxMacro <= 60){
     //Chase Mode move a single random led 
         if(currentMillis - previousMillisMacro > Speed) {
          // save the last time you blinked the LED 
          previousMillisMacro = currentMillis;  
          
         macro_random_led = random(NUM_LEDS); 
        // Turn our current led on to white, then show the leds
        leds[macro_random_led].setRGB( DmxRed, DmxGreen, DmxBlue);
        
        // Show the leds (only one of which is set to white, from above)
        FastLED.show(); 
  
        // Turn our current led back to black for the next loop around
        leds[macro_random_led].setRGB( 0, 0, 0);
        i = i + 1;
         }
     
      if ( i > NUM_LEDS ){
        i =0; 
      }
    
    }else if(DmxMacro >= 61 && DmxMacro <= 90){
     //Chase Mode move 3x random leds 
         if(currentMillis - previousMillisMacro > Speed) {
          // save the last time you blinked the LED 
          previousMillisMacro = currentMillis;  
          
         macro_random_led = random(NUM_LEDS); 
        // Turn our current led on to white, then show the leds
        leds[macro_random_led].setRGB( DmxRed, DmxGreen, DmxBlue);
        if(macro_random_led+3 >= NUM_LEDS){
          //Out of Range
        }else{
          leds[macro_random_led+3].setRGB( DmxRed, DmxGreen, DmxBlue);
        }
   
        
        if(macro_random_led+6 >= NUM_LEDS){
          //Out of Range
        }else{
          leds[macro_random_led+6].setRGB( DmxRed, DmxGreen, DmxBlue);
        }
        
        // Show the leds (only one of which is set to white, from above)
         
        /*
        // Turn our current led back to black for the next loop around
        leds[macro_random_led].setRGB( 0, 0, 0);
        
        if(macro_random_led+3 >= NUM_LEDS){
          //Out of Range
        }else{
        leds[macro_random_led+4].setRGB( 0, 0, 0);
        }
        
        if(macro_random_led+6 >= NUM_LEDS){
          //Out of Range
        }else{
        leds[macro_random_led+6].setRGB( 0, 0, 0);
        }
        i = i + 1;
         */
         

       }//Speed close
       FastLED.show();
       fadeToBlackBy( leds, NUM_LEDS, 20);
      if ( i > NUM_LEDS ){
        i =0; 
      }
    
    }else if(DmxMacro >= 91 && DmxMacro <= 120){
    //Matrix Mode One
    
      if(currentMillis - previousMillisMacro > Speed) {
        // save the last time you blinked the LED 
        previousMillisMacro = currentMillis;  
        int i = 0;
        
        if(toggle_one == 0){
        //Iterate over Matrix  by row      
        for(int y = 0; y <= colums; y++){
          i = pgm_read_byte(&(octaMatrix[x][y]));  
          if(i == 0){
            //No led in Matrix
          }else{
            if(i == 256){
              i = 0; //Map to 0
            }else{
              //i = octaMatrix[x][y]; //Set Led from Matrix
            }
            // Turn our current led on to the set color, then show the leds
            
            if(i> 252){
            
            }else{
            
            leds[i].setRGB( DmxRed, DmxGreen, DmxBlue);
            }
          }
        }
        x = x+1;
        
        
        if (x >=rows){
          x =0;
          toggle_one = 1;
        }
        
        }else{
        //Iterate over Matrix  by row      
        for(int x = 0; x < rows; x++){
          i = pgm_read_byte(&(octaMatrix[x][y]));  
          if(i == 0){
            //No led in Matrix
          }else{
            if(i == 256){
              i = 0; //Map to 0
            }else{
              //i = octaMatrix[x][y]; //Set Led from Matrix
            }
            // Turn our current led on to the set color, then show the leds
            if(i >252){
            }else{
            leds[i].setRGB( DmxRed, DmxGreen, DmxBlue);
          }
          }
        }
        y = y+1; 
          
        if (y >=colums){
          y =0;
          toggle_one = 0;
        }  
          
          
        }
      
        
        // Show the leds (only one of which is set to color, from above)
        FastLED.show(); 
      
        
      }//close for speed
      fadeToBlackBy( leds, NUM_LEDS, 100);
      
      
    }else if(DmxMacro >= 121 && DmxMacro <= 150){
      //Matrix Mode Two
    
      if(currentMillis - previousMillisMacro > Speed) {
        // save the last time you blinked the LED 
        previousMillisMacro = currentMillis;  
        
        volume_x = map(analogRead(A0), 512, 1023, 0, rows-1);
        volume_y = map(analogRead(A0), 512, 1023, 0, colums-1);

        if(volume_x <=0){
          volume_x = 0;
        }
  		  		
        if(volume_y <=0){
          volume_y = 0;
        }
  
        int i = 0;
  		int size_x = random(16)+volume_x; 
  		int size_y = random(16)+volume_y; 
        if(size_x >= rows){
          size_x = rows-1;
        }
  		if(size_y >= colums){
          size_y = colums-1;
        }
          //Iterate over Matrix  by row      
        for(int x = volume_x; x < size_x; x++){
          for(int y = volume_y; y < size_y; y++){
            if(y >= colums-1){
             y = colums-1;
            }
            
            if(y <= 0){
             y = 0;
            }
            
            if(x >= rows-1){ 
              x = rows-1;
            }
            
            if(x <= 0){ 
              x = 0;
            }
               i = pgm_read_byte(&(octaMatrix[x][y]));         
              if(i == 0){
                //No led in Matrix
              }else{
                if(i == 256){
                  i = 0; //Map to 0
                }else{
                  //i = octaMatrix[x][y]; //Set Led from Matrix
                }
                // Turn our current led on to the set color, then show the leds
                if(i >= 252){
                }else{
                leds[i].setRGB( DmxRed, DmxGreen, DmxBlue);
                //Serial.println(i);
              }
              }
            
          }//close for 2
        }//close for 1
        
        
        // Show the leds (only one of which is set to color, from above)
        FastLED.show(); 
      }//close speed
      fadeToBlackBy( leds, NUM_LEDS, 50);
    
    }else if(DmxMacro >= 151 && DmxMacro <= 180){
      // a colored dot sweeping back and forth, with fading trails
      fadeToBlackBy( leds, NUM_LEDS, 20);
      int pos = beatsin16( 13, 0, NUM_LEDS-1 );
      leds[pos] += CRGB( DmxRed, DmxGreen, DmxBlue);
      
      // send the 'leds' array out to the actual LED strip
      FastLED.show();  
       // insert a delay to keep the framerate modest
      FastLED.delay(1000/FRAMES_PER_SECOND);
     
   }else if(DmxMacro >= 181 && DmxMacro <= 255){
      // random colored speckles that blink in and fade smoothly
      fadeToBlackBy( leds, NUM_LEDS, 10);
      int pos = random16(NUM_LEDS);
      leds[pos] += CHSV( gHue + random8(64), 200, 255);
      
      // send the 'leds' array out to the actual LED strip
      FastLED.show();  
       // insert a delay to keep the framerate modest
      FastLED.delay(1000/FRAMES_PER_SECOND);
     
   }//close for macro



  } else {
    // Switch status led off, when no data was received since 5 seconds or more.
    FastLED.setBrightness(20);
    digitalWrite(13, HIGH);
    leds[0].setRGB( 255, 0, 0);
    FastLED.show();
      
    delay(200);
    digitalWrite(13, LOW);
    
    leds[0].setRGB( 0, 0, 0);
    FastLED.show();
    delay(200);
  } // if
  
  
  
  //Delay for stability
  delay(25);

}
/* LED "Color Organ" for Adafruit Trinket and NeoPixel LEDs.
 *  reference from:
https://learn.adafruit.com/gemma-powered-neopixel-led-sound-reactive-drums/circuit-diagram
Hardware requirements:
 - Adafruit Trinket or Gemma mini microcontroller (ATTiny85).
 - Adafruit Electret Microphone Amplifier (ID: 1063)
 - Several Neopixels, you can mix and match
   o Adafruit Flora RGB Smart Pixels (ID: 1260)
   o Adafruit NeoPixel Digital LED strip (ID: 1138)
   o Adafruit Neopixel Ring (ID: 1463)

Software requirements:
 - Adafruit NeoPixel library

Connections:
 - 5 V to mic amp +
 - GND to mic amp -
 - Analog pinto microphone output (configurable below)
 - Digital pin to LED data input (configurable below)

Written by Adafruit Industries.  Distributed under the BSD license.
This paragraph must be included in any redistribution.
*/
#include <Adafruit_NeoPixel.h>

#define NUM_PIX  8  // Number of pixels you are using
#define LED_PIN    3 
#define DC_OFFSET  0  // DC offset in mic signal - if unusure, leave 0
#define NOISE     50  // Noise/hum/interference in mic signal
#define SAMPLES   60  // Length of buffer for dynamic level adjustment
#define TOP       (NUM_PIX +1) // Allow dot to go slightly off scale


// these are my mics AND MOTORS
#define MIC_F A0
#define MIC_B A1
//front middle and back
#define VIBE_F A2
#define VIBE_M A3
#define VIBE_B A4

#define LED_PIN 6
#define NUM_PIX 8
// Comment out the next line if you do not want brightness control or have a Gemma
//#define POT_PIN    3  // if defined, a potentiometer is on GPIO #3 (A3, Trinket only) 
//adafruit led code from https://learn.adafruit.com/gemma-powered-neopixel-led-sound-reactive-drums/circuit-diagram
byte
  peak      = 0,      // Used for falling dot
  dotCount  = 0,      // Frame counter for delaying dot-falling speed
  volCount  = 0;      // Frame counter for storing past volume data
  
int
  vol[SAMPLES],       // Collection of prior volume samples
  lvl       = 10,     // Current "dampened" audio level
  minLvlAvg = 0,      // For dynamic adjustment of graph low & high
  maxLvlAvg = 512;

Adafruit_NeoPixel  strip = Adafruit_NeoPixel(NUM_PIX, LED_PIN, NEO_GRB + NEO_KHZ800);

//need to be on analog pins
int MicF_Val;        // the analog reading from the first mic
int MicB_Val;       // the analog reading from the second mic

// the intensity of our buzz
int Buzzing1; 
int Buzzing2; 
int Buzzing3;
const int ledPin = 11;// the PWM pin of the LED

//initial vibrations intensity
int vibeIntensity = 0;

int sig = 0;
 
void setup(void) {
  Serial.begin(9600);  
  pinMode(VIBE_F, OUTPUT);
  pinMode(VIBE_M, OUTPUT);
  pinMode(VIBE_B, OUTPUT);
    //memset(vol, 0, sizeof(vol));
  memset(vol,0,sizeof(int)*SAMPLES);
  strip.begin();
}
 
void loop(void) {
  
GoodVibes();
  delay(20);
Led();
}

void Led(){
    uint8_t  i;
  uint16_t minLvl, maxLvl;
  int      n, height;
  n   = analogRead(MIC_B);                 // Raw reading from mic 
  n   = abs(n - 512 - DC_OFFSET);            // Center on zero
  n   = (n <= NOISE) ? 0 : (n - NOISE);      // Remove noise/hum
  lvl = ((lvl * 7) + n) >> 3;    // "Dampened" reading (else looks twitchy)
  
  // Calculate bar height based on dynamic min/max levels (fixed point):
  height = TOP * (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg);

  if(height < 0L)       height = 0;      // Clip output
  else if(height > TOP) height = TOP;
  if(height > peak)     peak   = height; // Keep 'peak' dot at top

// if POT_PIN is defined, we have a potentiometer on GPIO #3 on a Trinket 
//    (Gemma doesn't have this pin)
  uint8_t bright = 255;   
#ifdef POT_PIN            
   bright = analogRead(POT_PIN);  // Read pin (0-255) (adjust potentiometer 
                                  //   to give 0 to Vcc volts
#endif
  strip.setBrightness(bright);    // Set LED brightness (if POT_PIN at top
                                  //  define commented out, will be full)
  // Color pixels based on rainbow gradient
  for(i=0; i<NUM_PIX; i++) {  
    if(i >= height)               
       strip.setPixelColor(i,   0,   0, 0);
    else 
       strip.setPixelColor(i,Wheel(map(i,0,strip.numPixels()-1,30,150)));
    } 

   strip.show(); // Update strip

  vol[volCount] = n;                      // Save sample for dynamic leveling
  if(++volCount >= SAMPLES) volCount = 0; // Advance/rollover sample counter

  // Get volume range of prior frames
  minLvl = maxLvl = vol[0];
  for(i=1; i<SAMPLES; i++) {
    if(vol[i] < minLvl)      minLvl = vol[i];
    else if(vol[i] > maxLvl) maxLvl = vol[i];
  }
  // minLvl and maxLvl indicate the volume range over prior frames, used
  // for vertically scaling the output graph (so it looks interesting
  // regardless of volume level).  If they're too close together though
  // (e.g. at very low volume levels) the graph becomes super coarse
  // and 'jumpy'...so keep some minimum distance between them (this
  // also lets the graph go to zero when no sound is playing):
  if((maxLvl - minLvl) < TOP) maxLvl = minLvl + TOP;
  minLvlAvg = (minLvlAvg * 63 + minLvl) >> 6; // Dampen min/max levels
  maxLvlAvg = (maxLvlAvg * 63 + maxLvl) >> 6; // (fake rolling average)
}

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  }

// my function that maps my motors
void GoodVibes(){
    MicF_Val = analogRead(MIC_F);
    MicB_Val = analogRead(MIC_B);
    //I WANT TO COLLECT THE MEDIAN OF THESE TWO VALUES
    // MicM_Val = analogRead((MIC_B + MIC_F)/2)

  // we'll need to change the range from the analog reading (0-1023) down to the range
  // used by analogWrite (0-255) with map!
  Buzzing1 = map(MicF_Val, 0, 1023, 0, 255);
  Buzzing2 = map(MicF_Val && MicB_Val, 0, 1023, 0, 255);
  Buzzing3 = map(MicB_Val, 0, 1023, 0, 255);
  
  // Vibrations increase the louder the sound
  analogWrite(VIBE_F, Buzzing1);
  Serial.println(VIBE_M);
  analogWrite(VIBE_M, Buzzing2);
  analogWrite(VIBE_B, Buzzing3);
  }

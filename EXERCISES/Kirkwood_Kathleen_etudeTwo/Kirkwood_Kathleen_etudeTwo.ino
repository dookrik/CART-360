/**********************ETUDE 2 CART 360 2017*******************************
   WELCOME!
   THE PURPOSE OF THIS EXERCISE IS TO DESIGN A VERY SIMPLE KEYBOARD (5 KEYS)
   WHICH ALLOWS YOU TO PLAY LIVE, RECORD, AND PLAYBACK.
   THERE WILL BE A BUTTON WHICH WHEN PRESSED WILL TAKE THE USER TO THE NEXT MODE:
   THERE ARE 5 POSSIBLE MODES
   0 ==  reset    ==   led off == also resets
   1 ==  live     ==   led BLUE
   2 ==  record   ==   led RED
   3 ==  play     ==   led GREEN
   4 ==  loopMODE ==   led PURPLE

   ADDITIONALLY - WHEN THE USER CHANGES MODE,
   THE RGB LED WILL CHANGE COLOR (ACCORDING TO THE SPECIFICATIONS)

   PLEASE FOLLOW THE INSTRUCTIONS IN THE COMMENTS:
   DO NOT ADD ANY MORE FUNCTION DEFINITIONS
   ONLY IMPLEMENT THE FUNCTION DEFINITIONS SUPPLIED
   THERE IS NO NEED TO ADD ANY NEW VARIABLES OR CONSTANTS
   PROVIDE COMMENTS FOR ANY OF THE CODE IMPLEMENTED
   GOOD LUCK!

   Kirkwood Kathleen october 5 2017
*/
/**** CONSTANTS ********************************************************/

#define BUTTON_MODE_PIN 2 // Button to change the mode

// constants for RGB LED
#define LED_PIN_R 9 // R 5 PIN
#define LED_PIN_G 6 // G 9 PIN
#define LED_PIN_B 5 // B 6 PIN

// constant for note in (button-resistor ladder on breadboard)
# define NOTE_IN_PIN A0

//constant for max number of notes in array
#define MAX_NOTES 16

// a constant for duration
const int duration = 200;

// constant for pin to output for buzzer
#define BUZZER_PIN 3 // PWM

/******** VARIABLES *****************************************************/
// counter for how many notes we have
int countNotes = 0;
// array to hold the notes played (for record/play mode)
int notes [MAX_NOTES];
int mode = 0; // start at off

/*************************************************************************/


/**********************SETUP() DO NOT CHANGE*******************************/
// Declare pin mode for the single digital input
void setup()
{
  pinMode(BUTTON_MODE_PIN, INPUT);
}

/**********************LOOP() DO NOT CHANGE *******************************
   INSTRUCTIONS:
   There is NO NEED to change the loop - it establishes the flow of the program
   We call here 3 functions repeatedly:
   1: chooseMode(): this function will determine the mode that your program is in
   based on if the button (linked to the BUTTON_MODE_PIN) was pressed
   2: setRGB(): will set the color of the RGB LED based on the value of the mode variable
   3: selectMode(): will determine which function to call based on the value of the mode variable

**************************************************************************/
void loop()
{
  chooseMode();
  setRGB();
  selectMode();
}
/******************CHOOSEMODE(): IMPLEMENT *********************************
   INSTRUCTIONS:
   Read the value from the Button (linked to the BUTTON_MODE_PIN) and
   if is being pressed then change the mode variable.
   REMEMBER:
   mode == 0 is reset
   mode == 1 is live
   mode == 2 is record
   mode == 3 is play
   mode == 4 is loopMode
   Every time the user presses the button, the program will go to the next mode,
   once it reaches 4, it should go back to mode == 0.
   (i.e. if mode ==2 and we press, then mode ==3) ...
**************************************************************************/
void chooseMode() {

  boolean buttonState = false;
  boolean prevState = true;
  //changes mode only changes int mode=0 allowing it to browse through modes
  //on button push and when released
  if (digitalRead(BUTTON_MODE_PIN) == HIGH)
  {
    delay(200);
    if (buttonState != prevState) {

      buttonState = !buttonState;
      prevState = buttonState;
    }
  } else {
    prevState = !buttonState;
  }
  // if button pressed increment goes through modes until it reaches for where this statement becomes false and it changes back to mode 0.
  if (buttonState) {
    if (mode < 4) {
      //increment mode
      mode++;
    } else {
      //resets the mode
      mode = 0;
    }
  }
}

/******************SETRGB(): IMPLEMENT *********************************
   INSTRUCTIONS:
   Depending on the value of the mode variable:
   if the mode is 0 - RGB LED IS OFF
   if the mode is 1 - RGB LED IS BLUE
   if mode is 2 - RGB LED IS RED
   if mode is 3 - RGB LED IS GREEN
   if mode is 4 - RGB LED iS PURPLE
   YOU MUST USE A SWITCH CASE CONSTRUCT (NOT A SERIES OF IF / ELSE STATEMENTS
**************************************************************************/
void setRGB()
{
   /* So this code inputs color values to the LED and when the button is pressed*/
  // when modes switches do this so go through the color signals then break out to the next as mode changes
  switch (mode) {
    //mode 0 reset, Off (all LEDs off):
    case 0: {
        analogWrite(LED_PIN_R, LOW);
        analogWrite(LED_PIN_G, LOW);
        analogWrite(LED_PIN_B, LOW); break;
      }
    // Blue, live (turn just the blue LED on):
    case 1: {
        analogWrite(LED_PIN_R, LOW);
        analogWrite(LED_PIN_G, LOW);
        analogWrite(LED_PIN_B, HIGH); break;
      }
    // Red, record (turn just the red LED on):
    case 2: {
        analogWrite(LED_PIN_R, HIGH);
        analogWrite(LED_PIN_G, LOW);
        analogWrite(LED_PIN_B, LOW);
        break;
      }
    // Green, play (turn just the green LED on):
    case 3: {
        analogWrite(LED_PIN_R, LOW);
        analogWrite(LED_PIN_G, HIGH);
        analogWrite(LED_PIN_B, LOW); break;
      }
    // Purple, loop (turn red and blue on):
    case 4: {
        analogWrite(LED_PIN_R, HIGH);
        analogWrite(LED_PIN_G, LOW);
        analogWrite(LED_PIN_B, HIGH); break;
      }
  }
}

/**********************SELECTMODE() DO NOT CHANGE *******************************
   INSTRUCTIONS:
   There is NO NEED to change this function - it selects WHICH function should run based on the mode variable
   There are 4 possibilities
   1: reset(): this function will reset any arrays etc, and will do nothing else
   2: live(): this function will play the corresponding notes
   to the user pressing the respective buttons.
   NOTE:: the notes played are NOT stored
   3: record(): this function will play the corresponding notes
   to the user pressing the respective buttons
   AND will STORE up to 16 consecutive notes in an array.
   4: play(): this function will playback any notes stored in the array that were recorded
   in the previous mode
   5: loopMode(): this function will playback any notes stored in the array that were recorded,
   BUT unlike the previous mode, you can choose in which sequence the notes are played.
   REQUIRED: only play notes from the array (no live stuff)

******************************************************************************/

void selectMode()
{
  if (mode == 0) {
    reset();
  }
  else if (mode == 1) {
    live();
  }
  else if (mode == 2) {
    record();
  }

  else if (mode == 3) {
    play();
  }

  else if (mode == 4) {
    loopMode();
  }
}
/******************RESET(): IMPLEMENT **************************************
   INSTRUCTIONS:
   this function should ensure that any notes recorded are no longer available
**************************************************************************/
void reset()
{
  noTone(BUZZER_PIN);
  //this goes through the whole array in every element i reseting all the notes to 0, resets the array.
  for (int i = 0; i < MAX_NOTES; i++) {
    notes[i] = 0;

  }
  //restart the counter set it to 0
  countNotes = 0;
}
/******************LIVE(): IMPLEMENT **************************************
   INSTRUCTIONS:
   this function will play the corresponding notes
   to the user pressing the respective buttons
   NOTE:: the notes played are NOT stored
   SO: you need read in the input from the analog input (linked to the button-resistor ladder combo)
   THEN - output the note to the buzzer using the tone() function
**************************************************************************/
void live()
{
  // read what notes I play and output them to buzzeR with a small delay between notes
  tone(BUZZER_PIN, analogRead(NOTE_IN_PIN), duration);
}
/******************RECORD(): IMPLEMENT **********************************
   INSTRUCTIONS:
   this function will play the corresponding notes
   to the user pressing the respective buttons
   AND will STORE up to 16 consecutive notes in an array.
   SO:you need read in the input from the analog input (linked to the button-resistor ladder combo)
   AND - output the note to the buzzer using the tone() function
   THEN store that note in the array  - BE CAREFUL - you can only allow for up to MAX_NOTES to be stored
**************************************************************************/
void record()
{
  //shrinks the task for the code as well as avoiding double inputs in the array if the button was held for longer than usual
  boolean buttonState = false;
  boolean prevState = true;
  int frequency = analogRead(NOTE_IN_PIN);

  tone(BUZZER_PIN, frequency, duration);

  //evaluate if the frequency is greater than 0
  if (frequency > 0) {
    //delaying the looping process to prevent duplicated input
    delay(duration);
    if (buttonState != prevState) {
      buttonState = !buttonState;
      prevState = buttonState;
    }
  } else {
    prevState = !buttonState;
  }
  //if there is a frequency produced when button state is true then record notes  while it is smaller then maxnote
  if (buttonState == true ) {
    if (countNotes < MAX_NOTES) {
      notes[countNotes] =  frequency;
      countNotes++;
    }
  }
}
/******************PLAY(): IMPLEMENT ************************************
   INSTRUCTIONS:
   this function will playback any notes stored in the array that were recorded
   in the previous mode
   SO: you need to go through the array of values (be careful - the user may not have put in MAX_NOTES)
   READ each value IN ORDER
   AND output each note to the buzzer using the tone() function
   ALSO: as long as we are in this mode, the notes are played over and over again
   BE CAREFUL: make sure you allow for the user to get to another mode from the mode button...
**************************************************************************/
void play(){
  /* replays notes from my array in function recors and from there it would increment
    to the maximumm amount of  note or until button press, when note add turns out to be false*/
  //  it will play the notes added
  // while browing your saved index of lenght of MAX_NOTES and if its on mode play then repeat sounds from array index.

  //under the condition where the notes accumulated are less that my array lenght, play notes from array and continue playing them just as long as they don't exceed maxnotes
  // user can play notes ranging from 0 to 16
 

  if (countNotes < MAX_NOTES) {
    delay(duration);

    tone(BUZZER_PIN, notes[countNotes], duration);
    countNotes++;

  } else {
    // avoids reaching MAX_NOTES so it'll start the array.
    countNotes = 0;
  }
}
/******************LOOPMODE(): IMPLEMENT *********************************
   INSTRUCTIONS:
   this function will playback any notes stored in the array that were recorded
   in the previous mode
   SO: you need to go through the array of values (be careful - the user may not have put in MAX_NOTES)
   READ values IN ANY ORDERING (You MUST use the array - but you can determine your own sequence)
   AND output each note to the buzzer using the tone() function
   ALSO: as long as we are in this mode, the notes are played over and over again
   BE CAREFUL: make sure you allow for the user to get to another mode from the mode button...
**************************************************************************/
void loopMode()
{
  // only play notes from 0-16 in a random order, the delay is set so that the playback of the notes does come out as white noise
  delay(duration);
  // it takes the notes recorded and randomizes the array as an output buzz.
  int randomNum = random(0, MAX_NOTES);
  //plays that sweet sound,
  tone(BUZZER_PIN, notes[randomNum]);

}
/* We use ladder resistors to create different frequencies so the more amperage is present in the circuit leading up
  the speaker the higher the frequency and the lower the frequency the less amperage is presen. This is all cause by the
  strenght of the resistor so depending on the flow of for example if a 300 ohm resitor is placed the note with pitch higher 
  than one passing through 10k ohms which will produce a lower frequency*/
/**************************************************************************/





/**********************ETUDE 4 CART 360 2017*******************************
   1/ As in Etude 3, in Etude 4 you will still Play & Record Notes and capture the associated Note Duration.
   Additionally, in the record() , Pauses between Notes are also captured - you need to implement.
   2/In Playback Mode, you will NOW have the option to Store the Pattern of Notes you just recorded by pressing the Store Button -
   3/ You will be able to store up to 16 Patterns (Hardcoded Limit).
   4/ Loop Mode has been Deprecated and has been replaced by the new Retrieve Mode ( retrieveMode() ).
   In Retrieve Mode, you will be able to Select a Stored Pattern and Retrieve it
   - the Stored Pattern Number will be displayed on the 7-Segment Display.
   To Play the Stored Pattern, you will Press the Store Button (Confirm Selection).
   The Potentiometer, Shift Register and 7-Segment Display work in concert to support your intention:
   In Retrieve Mode, the Potentiometer selects Pattern ID -> Sends Pattern ID to Shift Register -> Shift Register to 7-Segment Display.
*/
// my different library tabs
#include "Constants.h"
#include "InterruptHandler.h"
#include "ShiftRegister.h"
#include "TestPattern.h"
#include "Timer.h"

/******** VARIABLES *****************************************************/
// counter for how many notes we have
int countNotes = 0;
// array to hold the notes played (for record/play mode)
int notes [MAX_NOTES];
int mode = 0; //

/******** VARIABLES FOR ETUDE 3 *****************************************************/
// NEW FOR ETUDE 3: array to hold durations
// will be a parallel array to the notes array -
// meaning that at index 0 of notes[] is stored the 1st frequency, and at index 0 of
// the durations array  is stored the 1st duration for THAT note....

int durations [MAX_NOTES];
// added needs to calculate the time from one note pressed to the next
int durations_In_Between[MAX_NOTES];

// boolean to determine if a note button was being pressed:
boolean activeNoteButton = false;

// keep track of the active frequency
int activeFrequency = 0;

// var to hold the note pressed
int testNote;

// var to hold a reference to the timer object used for finding the note durations
// this one updates teh timer and starts it up
Timer t1;

// ** NEW :: this timer object YOU will need to use  for finding pauses BETWEEN notes
Timer t_between;

/**********************SETUP() DO NOT CHANGE*******************************/
// Declare pin mode for the single digital input
void setup()
{
  Serial.begin(9600);

  // INTERRUPT HANDLER
  setupInterruptHandler();

  // SHIFT REGISTER
  setupShiftRegister();
}

/**********************LOOP() *******************************
   INSTRUCTIONS:
   The loop establishes the flow of the program:
   If the mode Button has been pressed (modeChangeInterruptRequest boolean == true) then:
   1: chooseMode():  &&&
   2. YOU need to ensure that the  7-SEGMENT DISPLAY is off.
   Then  - regardless of if the button is pressed:
   3: setRGB(): will set the color of the RGB LED based on the value of the mode variable
   4: selectMode(): will determine which function to call based on the value of the mode variable

**************************************************************************/
void loop()
{
  //
  if (modeChangeInterruptRequest)
  {
    chooseMode();
    delay(DURATION * 2);

    modeChangeInterruptRequest = ISFALSE;
    /* TO DO ::: TURN OFF THE 7-SEGMENT DISPLAY */
  }
  setRGB();
  selectMode();
}

/******************CHOOSEMODE() -NOTHING TO CHANGE *********************************
   INSTRUCTIONS:
   Read the value from the Button (linked to the BUTTON_MODE_PIN) and
   if is being pressed then change the mode variable.
   REMEMBER:
   mode == 0 is reset
   mode == 1 is live
   mode == 2 is record
   mode == 3 is play
   mode == 4 is retrieve mode
   Every time the user presses the button, the program will go to the next mode,
   once it reaches 4, it should go back to mode == 0.
   (i.e. if mode ==2 and we press, then mode ==3) ...
**************************************************************************/
void chooseMode() {
  if (mode < 4) mode++;
  else mode = 0;
}


/**********************SELECTMODE() - NOTHING TO CHANGE *******************************
   INSTRUCTIONS:
   There is NO NEED to change this function - it selects WHICH function should run based on the mode variable
   There are 4 possibilities
   1: reset(): this function will reset any arrays etc, and will do nothing else
   2: live(): this function will play the corresponding notes
   to the user pressing the respective buttons.
   NOTE:: the notes played are NOT stored
   3: recordWithDuration(): this function will play the corresponding notes
   to the user pressing the respective buttons
   AND will STORE up to 16 consecutive notes in an array.
   4: playWithDuration(): this function will playback any notes stored in the array that were recorded
   in the previous mode AND allow the user to store the pattern.
   5: retrieve():

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
    recordWithDuration();
  }
  else if (mode == 3) {
    playWithDuration();
  }
  else if (mode == 4) {
    retreiveMode();
  }
}
/******************RESET() **************************************
   INSTRUCTIONS:
   this function should ensure that any notes recorded are no longer available
   ALSO: to IMPLEMENT: turn off the SEVEN SEGMENT DISPLAY
**************************************************************************/
void reset() {
  countNotes = 0;
  noTone(BUZZER_PIN);
  storeOnce = ISFALSE;
  /* TO DO:: TURN OFF THE 7-SEGMENT DISPLAY */

}

/******************LIVE()- NOTHING TO DO **************************************
   INSTRUCTIONS:
   this function will play the corresponding notes
   to the user pressing the respective buttons
   NOTE:: the notes played are NOT stored
   SO: you need read in the input from the analog input (linked to the button-resistor ladder combo)
   ALSO - get the value of the photo cell and combine it with the note frequency
   THEN - output the note to the buzzer using the tone() function, as 3rd argument
   make the duration value == to 100
**************************************************************************/
void live() {
  if (analogRead(NOTE_IN_PIN) > 8) {
    tone(BUZZER_PIN, analogRead(NOTE_IN_PIN), 100);
  }
  // make sure that in record mode we cannot save a pattern
  if (storePatternInterruptRequest == ISTRUE)
  {
    storePatternInterruptRequest = ISFALSE;
  }
}

/******************RECORDWITHDURATION(): IMPLEMENT NEW ETUDE 4 ************
   INSTRUCTIONS:
   YOU need to implement the new functionality for capturing the pauses between notes ...
   HINT: you will need to use the second timer and at some point call the  addToArrayInBetween() function which will
   update the necessary data structures.
   THERE IS NO INDICATION WHERE OR HOW YOU SHOULD DO THIS IN THIS FUNCTION

**************************************************************************/
void recordWithDuration()
{
  //adjusts what exactly?
  int theAdjuster = 8;
  // do I reset the testnotes? then once active I can implement the second timer until another note is pressed and then repeats.
  // check we have not stored more than 16 notes
  if (countNotes < MAX_NOTES) {
    // read in the value from button press
    testNote = analogRead(NOTE_IN_PIN);

    /*** STATE A::: *******************************************
        IF the testNote is > min AND the timer has not reached 5 secs:
        This means we are pressing on a note button
    */
    //max press is 5 sec, if note is less than adjuster and timepassed hasnt exceeded 5 secs then restart timer
    if (testNote > theAdjuster && (t1.getTimePassed() < MAX_PRESS_TIME)) {
      /*** STATE AA::: *******************************************
          IF the boolean is false it means we have just started to press the button
          YOU now need to implement the function below which will set up all things
          required to have a new timer
      */
      // this also repeats it
      if (activeNoteButton == ISFALSE) {

        t1.startUpTimer();
        activeNoteButton = ISTRUE;
      }
      /*** STATE AA::: *******************************************
           IF the boolean is true it means we are continuing to press the button
      */
      else {
        // update the timer (for durations)
        t1.updateTimer();
        activeFrequency = testNote;
        playCurrentNote();
      }
    }
    //implement the 2nd timer and the code
    //durations_In_Between[MAX_NOTES];

    /* what I want is something to measure the time passed in between the intervals,
        IF the boolean is true it means we have just stopped pressing the button so implement timer 2 make it count

// SO  if the note is less than 
      if (testNote < theAdjuster && ( t_between.updateTimer() > MAX_PRESS_TIME )) {

        if (activeNoteButton == ISTRUE) {

          t_between.startUpTimer();
          activeNoteButton = ISFALSE;
        }

      else {
          // update the timer (for durations)
          t_between.getTimePassed();
         
          activeFrequency = testNote;
          playCurrentNote();
        }
      }
    */

    /*** STATE B::: *******************************************
       IF the testNote is > min AND the timer HAS reached 5 secs:
       then turn OFF the buzzer.
    */
    else if (testNote > theAdjuster && (t1.getTimePassed() > MAX_PRESS_TIME)) {

      noTone(BUZZER_PIN);
      // go to this function as well
        t_between.getTimePassed();
    }
    /*** STATE C::: *******************************************
       IF the testNote is < min AND the boolean is true
       this means we have STOPPED pressing the button, update the arrays
    */
    else if ((testNote <= theAdjuster && activeNoteButton == ISTRUE) ) {
      /*** FUNCTION TO IMPLEMENT ***/
      updateArraysWithNoteAndTimings();
    }
  }
  // make sure that in record mode we cannot save a pattern
  if (storePatternInterruptRequest == ISTRUE)
  {
    storePatternInterruptRequest = ISFALSE;
  }
}


/******************PLAYCURRENTNOTE(): *********************************
   INSTRUCTIONS:
   this function will play the current note being pressed in RecordDuration mode
**************************************************************************/
void playCurrentNote() {
  tone(BUZZER_PIN, activeFrequency, 100);

}
/******************UPDATEARRAYSWITHNOTEANDTIMINGS():SMALL CHANGE ************
   INSTRUCTIONS:
   this function handles the case where the note is no longer being pressed:
   you need to update the arrays
   reset the necessary booleans
   reset any counters
   //TO DO :: perhaps we want to start up our other timer ????
**************************************************************************/
void updateArraysWithNoteAndTimings() {
  notes[countNotes] = activeFrequency;
  durations[countNotes] = t1.getTimePassed();
  countNotes++;
  activeNoteButton = ISFALSE;
  t1.setTimePassed(0);

}
/******************addToArrayInBetween(): IMPLEMENT *********************************
   INSTRUCTIONS:
   this function will handle the case that adds the current pause into the necessary array
   You may also need to reset some variables....
  /*************************************************************************/
void addToArrayInBetween() {
  // IMPLEMENT
}

/******************PLAYWITHDURATION(): ADAPTED FOR ETUDE 4 *******************
   INSTRUCTIONS:
   this function goes through the notes array and play back the note and
   its corresponding duration value.
   TO IMPLEMENT:: add the ability to store the current sequence playing within the if statement
**************************************************************************/
void playWithDuration() {
  int i = 0;

  while (storePatternInterruptRequest == ISFALSE && modeChangeInterruptRequest == ISFALSE ) {
    if (countNotes > 0) {
      tone(BUZZER_PIN, notes[i], durations[i]);
      delay(durations[i]);
      // ensure that you have values in this array ...
      delay(durations_In_Between[i]);

      if (i < countNotes - 1) i++;
      else {
        i = 0;
        delay(DURATION);
      }
    }

  }

  if (countNotes > 0 && storePatternInterruptRequest == ISTRUE && storeOnce == ISFALSE)
  {
    // TODO:: add pattern to our list
    //HINT use the TestPattern files..... there is a specific function that one can do to do this...
    //keep
    storeOnce = ISTRUE;

    /* TO DO::: FLASH THE LED ON AND OFF for DURATION (once) */

  }
  delay(DURATION * 2);
  storePatternInterruptReset();
}
/******************HELPER STATE 0 - PARTIALLY IMPLEMENT *******************
      INSTRUCTIONS: while neither button is pressed then:
      1/ get pot value
      2/if we have do NOT have a valid index (i.e. we selected "3" but there are only 3 patterns not 4) then
      make sure the seven segment display blinks.
      3/ otherwise, make sure that the shift register sends the correct number to the seven segment display


**************************************************************************/
void selectThePatternWithPot_Stage_0_(int & shiftRef)
{
  while (storePatternInterruptRequest == ISFALSE && modeChangeInterruptRequest == ISFALSE) {
    //1::YOU NEED TO IMPLEMENT THIS FUNCTION IN SHIFTREGISTER.h
    shiftRef = getAnalog();

    if (isValidIndex(shiftRef) == ISFALSE) {
      //2:: IMPLEMENT (again you will use functions that you implemented in SHIFTREGISTER.h)
    }
    // is valid index
    else {
      //3::IMPLEMENT (again you will use functions that you implemented in SHIFTREGISTER.h)
    }
  }
}
/******************HELPER STATE 1 -PARTIALLY IMPLEMENT *********************************
      INSTRUCTIONS:
  While the modebutton is not pressed: if we have a valid index
  then:
  1/ use the TestPattern functions to retrieve the notes,
  durations and pauses for that pattern.
  2/  Then play the current pattern
**************************************************************************/
void retrieveThePatternAccordingToSelection_Stage_1(int & shiftRef)
{
  while (modeChangeInterruptRequest == ISFALSE) {
    if (isValidIndex(shiftRef) == ISTRUE) {
      //1: TO DO :::RETRIEVE THE NOTES, PAUSES AND DURATIONS
      //HINT::: you will need to return a reference from the appropriate testPattern function
      // the FIRST one is given to you - the others will follow the same idea
      const int * noteArray = retrievePatternNotes(shiftRef);
      // you do::
      // const int * durationsArray = ....
      // const int * pausesArray =  ...

      //2: TODO: play the current pattern:
      //HINT: getNumberNotesOfPattern(shiftRef) -> will give you the number of notes in the pattern
    }
  }
}
/******************RETRIEVEMODE(): IMPLEMENT *********************************
   INSTRUCTIONS:
   1/ while there are no patterns and the mode button is not pressed then blink the
   seven segment display for DURATION
   2/ otherwise select the pattern and prehaps retrieve it


**************************************************************************/
void retreiveMode()
{
  noTone(BUZZER_PIN);
  static int shift = -1;

  while (patternListIsEmpty() == ISTRUE && modeChangeInterruptRequest == ISFALSE) {
    //1:: Implement - again you will use the functions you implemented in shiftRegister.h
  }

  // not empty case::
  //(PLEASE IMPLMENT THIS FUNCTION)
  selectThePatternWithPot_Stage_0_(shift);
  // we will come here because the store button was pressed  -> so we go to next stage - (store that pattern)
  delay(DURATION * 2);
  storePatternInterruptReset();
  // store and play the pattern until the mode button is pressed (PLEASE IMPLMENT THIS FUNCTION)
  retrieveThePatternAccordingToSelection_Stage_1(shift);
}

/******************SETRGB() - NOTHING TO DO*********************************
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
  switch (mode)
  {
    case 0:
      {
        analogWrite(LED_PIN_R, 0);   // Turn on the LED -R
        analogWrite(LED_PIN_G, 0);   // Turn on the LED -G
        analogWrite(LED_PIN_B, 0);   // Turn on the LED -B
        break;
      }
    case 1:
      {
        analogWrite(LED_PIN_R, 0);   // Turn on the LED -R
        analogWrite(LED_PIN_G, 0);   // Turn on the LED -G
        analogWrite(LED_PIN_B, 255);   // Turn on the LED -B
        break;
      }
    case 2:
      {
        analogWrite(LED_PIN_R, 255);   // Turn on the LED -R
        analogWrite(LED_PIN_G, 0);   // Turn on the LED -G
        analogWrite(LED_PIN_B, 0);   // Turn on the LED -B
        break;
      }
    case 3:
      {
        analogWrite(LED_PIN_R, 0);   // Turn on the LED -R
        analogWrite(LED_PIN_G, 255);   // Turn on the LED -G
        analogWrite(LED_PIN_B, 0);   // Turn on the LED -B
        break;
      }

    case 4:
      {
        analogWrite(LED_PIN_R, 125);   // Turn on the LED -R
        analogWrite(LED_PIN_G, 0);   // Turn on the LED -G
        analogWrite(LED_PIN_B, 255);   // Turn on the LED -B
        break;
      }
  }
}

/**************************************************************************
  Anwser the  questions IN THIS SPACE....
  1.Please explain in your own words,
  what and howaShift Register works -is this how
  it is usedthe context of this Etude?


  The shift register allows us to externally iterate through arrays of saved patterns in
  our case an array of notes with pauses.It is syncronous with our clock and allows
  the storage/ transmital of the equivalent of 8 bits of data(micro pulses)
  because it is synced with the clock, the two communicate in the following: the clock pin receives
  the micro pulses and the shift register
  stores these as bits of binary data in boolean type on or off depending on themicro pulse signals.
  Allows us to store things based on
  a clockwork or syncroninity. In this context as stated above, allows us to iterate through its
  stored bits (our saved patterns) useful because it works well with timed clocks.


  2.The outcome for this Etude allows the user in Mode 3
  to Store a Pattern and then in Mode 4 to select a Pattern
  by using the Potentiometer, then they click on the Store
  Button and that Pattern is Retrieved and Played. At this
  point, if the user wants to select another Stored Pattern
  they need to use the Mode Button to cycle through mode 1 -> mode 4
  in order to do so. Please explain how you would implement the functionality
  to allow the user to Select and Retrieve Patterns WITHIN the same Mode (without having to re-cycle through)

  Maybe we could add a 3rd button thats connected to a pin mode, which when pressed would
  We could try to attach it to the shift register so it can go back and forth? Make it into a potentiomometer...?
  decrement through that array of saved patterns? maybe have a true false statement to allow for a quick shift of back and forth iterating.(?)

  .3.In this Etude,we do not use the Photocell to modulate the Note
  Frequency while it is being pressed. Please explain briefly how one
  could STORE a modulated Note over time (i.e. how would one store the
  changes in frequency over a specific time interval)?NB:youneed not consider
  the specific code implementation provided for this Etude.

  would be possible if you had access to an analog pin but stored as rough averages inside the shift register.
  BBD something that allows for a more fluid communication


  4.Please provide 2 ideas
  (150words each) for how you would suggest to extend this instrument we have been
  developing throughout the semester.

  1. For the next project maybe we can create an installation out of the peices. By creating something larger, implementing
  this code into a piano so big that you can step on it. Have it installed somewhere where people can interract
  with it and can perform duets together. It could also take a collaborative approach where output from one
  user becomes input for the next. Like some sort of sound game or telephone effect.

  2. Maybe there could be a different approach to the sound and the appearance of the instrument itself.
  Maybe next time it can function only on photocells instead of buttons. It could in this way utilise different
  input methods or maybe make it make vibrations instead or make servos dance and jiggle.
  Could be different types of sensors that enable for a neat sound texture.
  Maybe something that can amplify the volume?




  /**************************************************************************/



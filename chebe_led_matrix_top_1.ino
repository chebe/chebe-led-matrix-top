/*
  chebe's LED-matrix top
  http://chebe.dreamwidth.org/tag/led+matrix+top
  by chebe, 30th April 2010
  license:  Creative Commons Attribution-NonCommercial 3.0
  
  Version 1;  MxN LED matrix with push button (set-up as 7x7)
              Controls LEDs; on/off
              Push button; cycles through patterns
              
  Uses a Lilypad Arduino (main microcontroller) with ATmega328 
  
  To turn an LED on electricity has to flow in the correct
  direction. Assumption; the + side of the LEDs is connected
  to the 'rows', and the - side to the 'columns'.
  Therefore;  on == row HIGH, column LOW
              off == row = LOW, column = HIGH
  HIGH == 1, LOW == 0
*/

// uncomment following line to enable DEBUG mode
//#define DEBUG  1

// -- CHANGE THESE VALUES IF USING DIFFERENT CIRCUIT LAYOUT --
//NxN matrix
int NUM_COLS = 7; // num of columns
int NUM_ROWS = 7; // num of rows
int MIN_DELAY = 1;
int MAX_DELAY = 10;
int ledDelay = MIN_DELAY; 
//for blinking/constant effects, 2 = constant, 15 = chasing
//thread less conductive, now 1 = constant, 10 = chasing

// -- LEDS --
// Make sure you have as many rowNs and colNs as
// defined in NUM_LEDS/NUM_COLS/NUM_ROWS
// And make adjustments to setup(), allOn(), allOff()
int row0 = 18;     //analog pin4
int row1 = 17;     //analog pin3
int row2 = 16;     //analog pin2
int row3 = 15;     //analog pin1
int row4 = 14;     //analog pin0
int row5 = 13;     //digital pin13
int row6 = 12;     //digital pin12
int column0 = 11;  //digital pin11
int column1 = 10;  //digital pin10
int column2 = 9;   //digital pin9
int column3 = 8;   //digital pin8
int column4 = 7;   //digital pin7
int column5 = 6;   //digital pin6
int column6 = 5;   //digital pin5 

int ledPosArray[] = {row0, row1, row2, row3, row4, row5, row6};
int ledNegArray[] = {column0, column1, column2, column3, column4, column5, column6};

// -- SWITCH --
// push button connected to digital pin 4
int switchPin = 4;
//constant, for number of patterns available
int NUM_PATTERNS = 2;
 
// -- END OF NEEDED CHANGES IF USING DIFFERENT CIRCUIT LAYOUT --


// a variable to keep track of when switch is pressed
int switchValue;
// variable to keep track of switch's previous value
int oldValue;
// variable to keep track of how many times the switch was pressed
int numOfPresses = 0;


// The setup() method runs once, when the sketch starts
void setup()
{
  // initialize the digital pin as an input    
  pinMode(switchPin, INPUT);
  // set the default (unpressed) state of switchPin to HIGH
  digitalWrite(switchPin, HIGH);
  
  // initialize the digital pins as output
  pinMode(row0, OUTPUT); 
  pinMode(row1, OUTPUT); 
  pinMode(row2, OUTPUT); 
  pinMode(row3, OUTPUT); 
  pinMode(row4, OUTPUT); 
  pinMode(row5, OUTPUT); 
  pinMode(row6, OUTPUT);
  pinMode(column0, OUTPUT); 
  pinMode(column1, OUTPUT); 
  pinMode(column2, OUTPUT); 
  pinMode(column3, OUTPUT); 
  pinMode(column4, OUTPUT); 
  pinMode(column5, OUTPUT);
  pinMode(column6, OUTPUT);
  
  // turn all LEDs off to begin
  allOff();
  
  // if DEBUG mode start Serial communication
  #ifdef DEBUG
  Serial.begin(9600);
  #endif
}


// the loop() method runs over and over again,
// as long as the Arduino has power
void loop()                     
{
  // store current switchValue as oldValue
  oldValue = switchValue;
  // get new switchValue / check to see if the switch is pressed
  switchValue = digitalRead(switchPin);
  
  // button pressed when change from HIGH to LOW is detected
  if((switchValue == LOW) && (oldValue == HIGH))
  {
    //pressed
    incrementPressCount();
  }
  else if((switchValue == HIGH) && (oldValue == HIGH))
  {
    //held down for more than delay (0.1 seconds)
    //ignore?
  }
  
  // now do something depending on num of presses
  if(numOfPresses == 0)
  {
    // pattern 0; all LEDs off
    allOff();
  }
  else if(numOfPresses == 1)
  {
    // pattern 1; all LEDs on
    allOff();
    allOn();
  }
  else if(numOfPresses == 2)
  {
    // pattern 2; random single LED on, twinkle
    allOff();
    on(random(7),random(7));
    delay(100);
  }
}

// -- HELPER FUNCTIONS --

// Turn all LEDs off
// Rows switched to low
// Columns switched to high
// Power flowing in wrong direction
void allOff()
{
  digitalWrite(row0, LOW);
  digitalWrite(row1, LOW);
  digitalWrite(row2, LOW);
  digitalWrite(row3, LOW);
  digitalWrite(row4, LOW);
  digitalWrite(row5, LOW);
  digitalWrite(row6, LOW);
  digitalWrite(column0, HIGH);
  digitalWrite(column1, HIGH);
  digitalWrite(column2, HIGH); 
  digitalWrite(column3, HIGH);
  digitalWrite(column4, HIGH);
  digitalWrite(column5, HIGH);
  digitalWrite(column6, HIGH);
}

// Turn all LEDs on
// Rows switched to high
// Columns switched to low
// Power flowing in correct direction
void allOn()
{
  digitalWrite(row0, HIGH);
  digitalWrite(row1, HIGH);
  digitalWrite(row2, HIGH);
  digitalWrite(row3, HIGH);
  digitalWrite(row4, HIGH);
  digitalWrite(row5, HIGH);
  digitalWrite(row6, HIGH);
  digitalWrite(column0, LOW);
  digitalWrite(column1, LOW);
  digitalWrite(column2, LOW); 
  digitalWrite(column3, LOW);
  digitalWrite(column4, LOW);
  digitalWrite(column5, LOW);
  digitalWrite(column6, LOW);
}

// Turn on one specified LED
void on(int row, int column)
{
  #ifdef DEBUG
  Serial.print(row);
  Serial.print(" | ");
  Serial.println(column);
  #endif
  row = rangeCheckRows(row);
  column = rangeCheckCols(column);
  digitalWrite(ledPosArray[row], HIGH);
  digitalWrite(ledNegArray[column], LOW);
}

// Turn off one specified LED
void off(int row, int column)
{
  row = rangeCheckRows(row);
  column = rangeCheckCols(column);
  digitalWrite(ledPosArray[row], LOW);
  digitalWrite(ledNegArray[column], HIGH);
}

// Makes sure turning on/off LED
// that actually exists
// if greater than num of LEDs,
//  cycle back to 0
// if less than 0,
//  cylce around to max LED
int rangeCheckRows(int place)
{
  if(place >= NUM_ROWS)
  {
    place = 0;
  }
  else if(place < 0)
  {
    place = NUM_ROWS -1;
  }
  return place;
}

// Makes sure turning on/off LED
// that actually exists
// if greater than num of LEDs,
//  cycle back to 0
// if less than 0,
//  cylce around to max LED
int rangeCheckCols(int place)
{
  if(place >= NUM_COLS)
  {
    place = 0;
  }
  else if(place < 0)
  {
    place = NUM_COLS -1;
  }
  return place;
}

// Increment push-button press count
// When reaches max number of patterns
// cycles around to start from 0 again
void incrementPressCount()
{
  if(numOfPresses < NUM_PATTERNS)
  {
    numOfPresses = numOfPresses + 1;
  }
  else
  {
    numOfPresses = 0;
  }
  #ifdef DEBUG
  Serial.print("> ");
  Serial.println(numOfPresses);
  #endif
}


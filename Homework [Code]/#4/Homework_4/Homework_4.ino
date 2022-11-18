// Move through a 4 digit 7-segment display and increment/decrement digits using a joystick with the possibility to lock in values

// universal none value
#define NONE -1

// states
#define FIRST_STATE 0
#define SECOND_STATE 1

// switch presses
#define LONG_PRESS 1
#define SHORT_PRESS 0

// joystick movement directions
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

// joystick pins : digital pin connected to switch output and analog pins connected to X, Y output
const int pinSW = 3,
          pinY = A4,
          pinX = A5;

// shift register pins
const int latchPin = 11,
          clockPin = 10,
          dataPin = 12;

// display pins
const int segD1 = 7,
          segD2 = 6,
          segD3 = 5,
          segD4 = 4;
          
const int regSize = 8,
          displayCount = 4,
          encodingsNumber = 16;

int initDisplay = 0,
    initDisplayValue = 0,
    currDisplay = 0,
    currState = FIRST_STATE;

// joystick movement x axis and y axis initial, current and change values
int initialXValue = 510,
    initialYValue = 510,
    xValue = 0,
    yValue = 0,
    xChange = 0,
    yChange = 0;

// variables corresponding to the current state of the blinking segment, current and last state of the switch
byte blinkState = HIGH,
     swState = LOW,
     lastSwState = LOW;

bool joyMoved = false;

int joystickMove = NONE,
    lastJoystickMove = NONE;

// minimum and maximum thresholds for joystick movement coordinates
int minThreshold = 250,
    maxThreshold = 750;

int switchPress = NONE;

// true if the short press delay for switch press has passed, false otherwise
bool passedShortDelay = false;

const int blinkInterval = 300,
          debounceDelayLong = 3000,
          debounceDelay = 25,
          muxDelay = 1;

unsigned long long lastBlinkTime = 0,
                   lastDebounceTime = 0;

// bit position of DP segment
const int bitPosDP = 0;

const int displays[displayCount] = {
  segD1, segD2, segD3, segD4
};

const byte byteEncodings[encodingsNumber] = {
//A B C D E F G DP 
  B11111100, // 0 
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
  B11101110, // A
  B00111110, // b
  B10011100, // C
  B01111010, // d
  B10011110, // E
  B10001110  // F
};

int displayValues[displayCount];

void setup() {
  pinMode(pinSW, INPUT_PULLUP);

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  for (int i = 0; i < displayCount; i++) {
    pinMode(displays[i], OUTPUT);
    digitalWrite(displays[i], HIGH);
    displayValues[i] = initDisplayValue;
  }
}


void loop() {
  parseCurrentState();
  showcaseDisplayValues();
}

void parseCurrentState() {
  if (currState == FIRST_STATE) {
      blinkDP();
  }
  parseSwitchPress();
  parseJoystickMovement();
}

// based on the pressing of the switch or the absence of it, has no impact, changes the state or resets
void parseSwitchPress() {
  switchPress = getSwitchPress();

  if(switchPress == SHORT_PRESS) {
    currState = !currState;
  }

  if(currState == FIRST_STATE && switchPress == LONG_PRESS) {
    passedShortDelay = false;
    reset();
  }
}

// based on pressing history, returns corresponding type of press or NONE if absent
int getSwitchPress() {
   int reading = digitalRead(pinSW);

    if (reading != lastSwState) {
      lastDebounceTime = millis();
    }

    lastSwState = reading;

    if (millis() - lastDebounceTime >= debounceDelay) {
      if (reading != swState) {
        swState = reading;

        if (swState == LOW) {
          passedShortDelay = true;
        }
      }

      if (swState == LOW) {
        if (millis() - lastDebounceTime >= debounceDelayLong) {
          return LONG_PRESS;
        } 
      } 
      else {
        if (passedShortDelay) {
          passedShortDelay = false;
          return SHORT_PRESS;
        }
      }
    }

    return NONE;
}

// based on the joystick movement and current state, resets or not the current segment or its state
void parseJoystickMovement() {
  joystickMove = getJoystickMove();

  if (joystickMove != lastJoystickMove) {
    if (currState == FIRST_STATE) {
      if (joystickMove == LEFT) {
          currDisplay += 1;
          if (currDisplay > displayCount - 1) {
            currDisplay = 0;
          }
      }
      if (joystickMove == RIGHT) {
          currDisplay -= 1;
          if (currDisplay < 0) {
            currDisplay = displayCount - 1;
          }
      } 
    }
    else if (currState == SECOND_STATE) {
      if (joystickMove == UP) {
          displayValues[currDisplay] += 1;

          if (displayValues[currDisplay] > encodingsNumber - 1) {
            displayValues[currDisplay] = 0;
          }
        }  
        else if (joystickMove == DOWN) {
          displayValues[currDisplay] -= 1;

          if (displayValues[currDisplay] < 0) {
            displayValues[currDisplay] = encodingsNumber - 1;
          }
        }
    }
  }

  lastJoystickMove = joystickMove;
}

// based on joystick coordinates, returns the corresponding direction of movement (UP, DOWN, LEFT, RIGHT) or NONE
int getJoystickMove() {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  xChange = abs(initialXValue - xValue);
  yChange = abs(initialYValue - yValue);

  if (!joyMoved) {
    if (yChange >= xChange) {
      if (yValue < minThreshold) {
        joyMoved = true;
        return DOWN;
      }
      if (yValue > maxThreshold) {
        joyMoved = true;
        return UP;
      }
    } 
    else {
      if (xValue < minThreshold) {
        joyMoved = true;
        return LEFT;
      }
      if (xValue > maxThreshold) {
        joyMoved = true;
        return RIGHT;
      }
    }
  }

  if (xValue >= minThreshold && xValue <= maxThreshold && yValue >= minThreshold && yValue <= maxThreshold) {
    joyMoved = false;
  }

  return NONE;
}

void blinkDP() {
  if (millis() - lastBlinkTime >= blinkInterval) {
      blinkState = !blinkState;
      lastBlinkTime = millis();
  }
}

// displays the current configuration
void showcaseDisplayValues() {
  for (int i = 0; i < displayCount; ++i) {
    byte encodingValue = byteEncodings[displayValues[i]];

    if (i == currDisplay && (blinkState || currState == SECOND_STATE) ) {
      bitSet(encodingValue, bitPosDP);        
    }
    
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, encodingValue);
    digitalWrite(latchPin, HIGH);
    
    digitalWrite(displays[i], LOW);
    delay(muxDelay);
    digitalWrite(displays[i], HIGH);
  }
}

// reset to the initial configuration
void reset() {
  for (int i = 0; i < displayCount; i++) {
    displayValues[i] = initDisplayValue;
  }
  currDisplay = initDisplay;
  currState = FIRST_STATE;
}

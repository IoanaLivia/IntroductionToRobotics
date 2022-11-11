// Draw on the 7-segment display using the joystick by selecting each desired segment to be and remain light up with the possibility to reset and start again

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

bool commonAnode = false; 

// joystick pins : digital pin connected to switch output and analog pins connected to X, Y output
const int pinSW = 2,
          pinX = A1,
          pinY = A0;

// segments pins
const int pinA = 4,
          pinB = 5,
          pinC = 6,
          pinD = 7,
          pinE = 8,
          pinF = 9,
          pinG = 10,
          pinDP = 11;

// variables corresponding to the current state of the blinking segment, current and last state of the switch
byte blinkState = HIGH,
     swState = LOW,
     lastSwState = LOW;

// joystick movement x axis and y axis initial, current and change values
int initialXValue = 510,
    initialYValue = 510,
    xValue = 0,
    yValue = 0,
    xChange = 0,
    yChange = 0;

// current state of the program, starts in first state
int currState = FIRST_STATE;

// boolean variable that is false if joystick is in its initial position and true if it moved in one of the defined above directions
bool joyMoved = false;

// minimum and maximum thresholds for joystick movement coordinates
int minThreshold = 250,
    maxThreshold = 750;

// current and last joystick move based on direction of movement or NONE if it has not moved
int joystickMove = NONE,
    lastJoystickMove = NONE;

// true if the short press delay has passed, false otherwise
bool passedShortDelay = false;

// delays for long press and debounce
const int debounceDelayLong = 3000,
          debounceDelay = 25;

// current press of the switch or NONE if it has not been pressed
int switchPress = NONE;

// last time for segment blink in first state and last time for debounce
unsigned long lastBlinkTime = 0,
              lastDebounceTime = 0;

// interval at which current segment is blinking in first state
int blinkInterval = 300;

// number of possible directions of joystick movement
const int noOfDirections = 4;

int currSeg = 7;

const int segSize = 8;

// array that stores, for each segment, the corresponding pin.
int segments[segSize] = { 
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

// array that stores for each segment its current state, set or unset.
byte segStates[segSize];

// columns correspond to possible directions (UP, DOWN, LEFT, RIGHT). Rows correspond to a-g segments in alphabetical order and the last row corresponds to dp.
byte adjacencyMatrix[segSize][noOfDirections] = {
  {0,   6,   5,   1},
  {0,   6,   5,   1},
  {6,   3,   4,   7},
  {6,   3,   4,   2},
  {6,   3,   4,   2},
  {0,   6,   5,   1},
  {0,   3,   6,   6},
  {7,   7,   2,   7},
};

void setup() {
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
    segStates[i] = LOW;
  }

  pinMode(pinSW, INPUT_PULLUP);
}


void loop() {
  parseCurrentState();
  displaySegments();
}

void parseCurrentState() {
  if (currState == FIRST_STATE) {
      blinkSegment();
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
int getSwitchPress(){
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

  if (currState == FIRST_STATE) {
     if(joystickMove != lastJoystickMove && joystickMove != NONE){
        currSeg = adjacencyMatrix[currSeg][joystickMove];
      }
  } 
  else {
    if(joystickMove != lastJoystickMove && (joystickMove == UP || joystickMove == DOWN)){
        segStates[currSeg] = !segStates[currSeg];
    }
  }

  lastJoystickMove = joystickMove;
}

// based on joystick coordinates, returns the corresponding direction of movement (UP, DOWN, LEFT, RIGHT) or NONE
int getJoystickMove(){
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

void blinkSegment(){
  if (millis() - lastBlinkTime >= blinkInterval) {
      blinkState = !blinkState;
      lastBlinkTime = millis();
  }
}

// displays all the segments depending on their current state, considering the necessary changes in case of a common anode
void displaySegments(){
  for (int i = 0; i < segSize; i++) {
    if(currSeg == i && currState != SECOND_STATE){
      digitalWrite(segments[i], blinkState);
    }
    else {
      if (!commonAnode) {
        digitalWrite(segments[i], segStates[i]);
      }
      else {
        digitalWrite(segments[i], !segStates[i]);
      }
    }
  }
}

// reset to the initial configuration : all segment states are LOW, the current segment corresponds to dp and the current state is the first state
void reset() {
  for (int i = 0; i < segSize; i++) {
    segStates[i] = LOW;
  }
  currSeg = segSize - 1;
  currState = FIRST_STATE;
}
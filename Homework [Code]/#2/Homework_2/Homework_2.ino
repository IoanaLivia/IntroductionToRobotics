// Crosswalk traffic lights: pushing the button sets the process in motion (waiting, crossing, fast crossing, back to default state). Pressing the button again will set the same process in motion.

// Constants for the pins that are being used
const int buttonPin = 3,
          buzzerPin = 4;

const int ledPinCarsRed = 5,
          ledPinCarsYellow = 6,
          ledPinCarsGreen = 7;

const int ledPinPeopleRed = 12,
          ledPinPeopleGreen = 13;

const int buzzToneCrossing = 700,
          buzzToneFastCross = 1000;

const int buzzerIntervalCrossing = 600,
          buzzerIntervalFastCross = 300,
          blinkIntervalFastCross = 300;

const int debounceDelay = 50;

// Variable corresponding to the current state : 1 - default, 2 - waiting, 3 - crossing, 4 - fast crossing
int stateNumber = 1;

int countdownToInitiateWaiting = 8000,
    durationWaiting = 3000,
    durationCrossing = 8000,
    durationFastCross = 4000;

// Boolean variable: true - countdown (8 seconds) is over and the waiting state can be intiated, false - countdown is not over
bool waitingStateInitiated = false;

unsigned long lastDebounceTime = 0,
              lastStateChange = 0,
              lastBlinkTime = 0,
              lastBuzzedTime = 0;

// Variables corresponding for the changeable states of the entities during the process
byte buttonState = LOW,
     lastButtonState = LOW;

byte ledStatePeopleGreen = LOW,
     ledStatePeopleRed = LOW;

byte ledStateCarsGreen = LOW,
     ledStateCarsYellow = LOW,
     ledStateCarsRed = LOW;

byte buzzerState = HIGH,
     blinkState = HIGH;

void setup() {
  pinMode(buttonPin,INPUT_PULLUP);

  pinMode(ledPinPeopleGreen,OUTPUT);
  pinMode(ledPinPeopleRed,OUTPUT);

  pinMode(ledPinCarsGreen,OUTPUT);
  pinMode(ledPinCarsYellow,OUTPUT);
  pinMode(ledPinCarsRed,OUTPUT);

  Serial.begin(9600);
}

void loop() {
  setState(stateNumber);
}

void setState(int stateNumber){
  if (stateNumber == 1) {
    setDefaultState();
  }
  else if (stateNumber == 2) {
    setWaitingState();
  }
  else if (stateNumber == 3) {
    setCrossingState();
  }
  else {
    setFastCrossingState();
  }
}

void setDefaultState(){
  setLedStates(LOW, HIGH, HIGH, LOW, LOW);
  writeLedStateValuesToPins();

  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        transitionToState(2);
      }
    }
  }

  lastButtonState = reading;
}

void setWaitingState() {
  if ((millis() - lastStateChange) == countdownToInitiateWaiting) {
    waitingStateInitiated = true;
    lastStateChange = millis();

    Serial.println("Entered waiting state. (2)");
    Serial.println(lastStateChange);
  }

  if (waitingStateInitiated) {
    setLedStates(LOW, HIGH, LOW, HIGH, LOW);
    writeLedStateValuesToPins();

    if (millis() - lastStateChange == durationWaiting) {
      transitionToState(3);
    }
  }
}

void setCrossingState() {
  if (millis() - lastStateChange < durationCrossing) {
      setLedStates(HIGH, LOW, LOW, LOW, HIGH);
      setBuzzerState(buzzerIntervalCrossing);

      writeLedStateValuesToPins();
      assignStateToBuzzer(buzzToneCrossing);
  }
  else {
    transitionToState(4);
  }
}

void setFastCrossingState() {
  if (millis() - lastStateChange < durationFastCross) {
    blinkLed(ledPinPeopleGreen);

    setBuzzerState(buzzerIntervalFastCross);
    assignStateToBuzzer(buzzToneFastCross);
  } 
  else {
    transitionToState(1);
  }
}

void transitionToState(const int nextState) {
  stateNumber = nextState;

  if (nextState == 2) {
    lastStateChange = millis();
    Serial.println("Exited default state. (1)");
    Serial.println("1 -> 2");
    Serial.println(lastStateChange);
  }
  else if (nextState == 3) {
    lastBuzzedTime = millis();
    lastStateChange = millis();
    waitingStateInitiated = false;

    Serial.println("Exited waiting state. (2)");
    Serial.println("2 -> 3");
    Serial.println(lastStateChange);
  }
  else if (nextState == 4) {
    buzzerState = LOW;
    blinkState = HIGH;
    lastStateChange = millis();
    lastBuzzedTime = millis();
    lastBlinkTime = millis();

    Serial.println("Exited crossing state. (3)");
    Serial.println("3 -> 4");
    Serial.println(lastStateChange);
  }
  else {
    resetBuzzer();
    blinkState = HIGH;
    lastButtonState = LOW;
    lastStateChange = millis();

    Serial.println("Exited fast crossing state. (4)");
    Serial.println("4 -> 1");
    Serial.println(lastStateChange);
  }
}

void setLedStates(const int peopleGreen, const int peopleRed, const int carsGreen, const int carsYellow, const int carsRed) {
  ledStatePeopleGreen = peopleGreen;
  ledStatePeopleRed = peopleRed;

  ledStateCarsGreen = carsGreen;
  ledStateCarsYellow = carsYellow;
  ledStateCarsRed = carsRed;
}

void writeLedStateValuesToPins() {
  digitalWrite(ledPinPeopleGreen, ledStatePeopleGreen);
  digitalWrite(ledPinPeopleRed, ledStatePeopleRed);

  digitalWrite(ledPinCarsGreen, ledStateCarsGreen);
  digitalWrite(ledPinCarsYellow, ledStateCarsYellow);
  digitalWrite(ledPinCarsRed, ledStateCarsRed);
}

void blinkLed(const int ledPin) {
  if (millis() - lastBlinkTime >= blinkIntervalFastCross) {
      blinkState = !blinkState;
      lastBlinkTime = millis();
  }

  digitalWrite(ledPin, blinkState);
}

void setBuzzerState(int buzzerInterval) {
  if (millis() - lastBuzzedTime >= buzzerInterval) {
      buzzerState = !buzzerState;
      lastBuzzedTime = millis();
  }
}

void assignStateToBuzzer(int buzzerTone) {
  if (buzzerState == HIGH) {
      tone(buzzerPin, buzzerTone);
  } 
  else {
      noTone(buzzerPin);
  }
}

void resetBuzzer() {
  buzzerState = LOW;
  noTone(buzzerPin);
}

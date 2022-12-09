// Matrix Menu & Demo for Game

#include <LiquidCrystal.h>
#include "LedControl.h"
#include <EEPROM.h>

// universal none value
#define NONE -1

// states
#define WELCOME 0
#define MENU 1
#define SETTINGS 2
#define START_GAME 3
#define HIGHSCORE 4
#define ABOUT 6
#define HOW_TO 5
#define END_GAME 7
#define ENTER_NAME 8
#define SET_BRIGHTNESS 9
#define SET_SOUND 10
#define SET_DIFFICULTY 11
#define SET_MATRIX_BRIGHTNESS 12
#define SET_LCD_BRIGHTNESS 13

// joystick movement directions
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

// switch presses
#define LONG_PRESS 1
#define SHORT_PRESS 0

const byte matrixSize = 8;

// pins
const int pinSW = 2,
          pinY = A0,
          pinX = A1;

const byte dinPin = 12,
      clockPin = 11,
      loadPin = 10;

const byte buzzerPin = 3;

const byte rs = 9,
           en = 8,
           d4 = 7,
           d5 = 6,
           d6 = 5,
           d7 = 4;

// customized bitmaps for matrix image
const byte welcomeBitmap[matrixSize][matrixSize] = {
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 1, 1, 0, 1, 0, 1},
  {1, 0, 1, 0, 0, 0, 0, 1},
  {1, 0, 1, 1, 1, 1, 0, 1},
  {1, 0, 0, 0, 0, 1, 0, 1},
  {1, 0, 1, 1, 1, 1, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
};

const byte howToBitmap[matrixSize][matrixSize] = {
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 1, 0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0, 1, 0, 0},
  {0, 0, 0, 0, 0, 1, 0, 0},
  {0, 0, 0, 1, 1, 1, 0, 0},
  {0, 0, 0, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 0, 0, 0, 0},
};

const byte highscoreBitmap[matrixSize][matrixSize] = {
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 0},
};  

// customized chars
const byte charDownArrow[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b11111,
	0b01110,
	0b00100
};

const byte charUpArrow[8] = {
	0b00100,
	0b01110,
	0b11111,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000
};

const byte charCup[8] = {
	0b11111,
	0b11111,
	0b11111,
	0b01110,
	0b00100,
	0b00100,
	0b00100,
	0b11111
};

const byte charHeart[8] = {
	0b00000,
	0b01010,
	0b11111,
	0b11111,
	0b01110,
	0b00100,
	0b00000,
	0b00000
};

const byte charClock[8] = {
	0b00000,
	0b01110,
	0b10011,
	0b10101,
	0b10001,
	0b01110,
	0b00000,
	0b00000
};

const byte charStar[8] = {
  B00000,
  B00100,
  B10101,
  B01110,
  B01110,
  B10101,
  B00100,
  B00000
};

// control of the matrix's leds and lcd display

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); 

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


// memory starting addresses for EEPROM

const int matrixBrightnessAddress = 0,
          soundAddress = 2,
          currNameStartingAddress = 20,
          currDiffAddress = 4,
          highscoreStartingAddress = 30,
          ledBrightnessAddress = 100;

int address;

int lifes = 3,
    currScore = -1,
    noOfMinutes = 0,
    noOfSeconds = 0;

int currLevel = 1, 
    diffIndex = 1, 
    diffOptions = 3,
    aboutIndex = 1,
    aboutOptions = 6,
    settingsIndex = 1,
    settingsOptions = 5,
    howToIndex = 1,
    howToOptions = 8,
    soundIndex = 1,
    soundOptions = 3,
    letterPos = 0,
    letterIndex = 0,
    brightnessOptionIndex = 1;

unsigned long gameStartTime = 0;

byte swState = LOW,
     lastSwState = LOW;

int currState = WELCOME,
    menuOptions = 6,
    menuIndex = 1,
    startingLevel = 0;

int switchPress = NONE,
    joystickMove = NONE,
    lastJoystickMove = NONE;

bool passedShortDelay = false,
     canScrollDown = true, 
     canScrollUp = false,
     joyMoved = false;

unsigned long lastMessageTime = -1,
              lastDebounceTime = 0;

bool inLevel = true;

unsigned long lastBlinkTime = 0;

// variables for current position of food and its state : collected / not collected
int foodCol = 0, foodRow = 0;
bool collectedFood = false;

// joystick movement x axis and y axis initial, current and change values
int initialXValue = 510,
    initialYValue = 510,
    xValue = 0,
    yValue = 0,
    xChange = 0,
    yChange = 0;

// minimum and maximum thresholds for joystick movement coordinates
int minThreshold = 250,
    maxThreshold = 750;

const int blinkInterval = 300,
          debounceDelayLong = 3000,
          debounceDelay = 100,
          muxDelay = 1;

// custom char arrays for displaying text on the lcd

const char digits[10] = {'0','1','2','3','4','5','6','7','8','9'};

const char difficulty[][16] = {
  {'E','A','S','Y',' ',' '},
  {'M','E','D','I','U','M'},
  {'H','A','R','D',' ',' '},
};

const char beginMatrix[][16] = {
  {'O', 'n', 'e', ' ', 'o', 'f', ' ', 't', 'h', 'e', ' ', 'b', 'e', 's', 't', ' '},
  {'g', 'a', 'm', 'e', 's', ' ', 'i', 's', ' ', 'b', 'a', 'c', 'k', '!', ' ', ' '},
  {'I', ' ', 'h', 'o', 'p', 'e', ' ', 'y', 'o', 'u', ' ', 'a', 'r', 'e', ' ', ' '},
  {'r', 'e', 'a', 'd', 'y', ' ', 'f', 'o', 'r', ' ', 'i', 't', ' ', '.', '.', '.'},
  {'W', 'e', 'l', 'c', 'o', 'm', 'e', ' ', 't', 'o',  ' ', 'S', 'n', 'a', 'k', 'e'},
  {' ', ' ', ' ', ' ', 'R', 'e', 'd', 'e', 'e', 'm', 'e', 'd', ' ', ' ', ' ', ' '},
};

const char levelMatrix[][16] = {
  {'L', 'e', 'v', 'e', 'l', ':', '1', ' ', ' ', ' ', ' ', 'E', 'A', 'S', 'Y'},
  {'L', 'e', 'v', 'e', 'l', ':', '2', ' ', ' ', ' ', ' ', 'E', 'A', 'S', 'Y'},
  {'L', 'e', 'v', 'e', 'l', ':', '3', ' ', ' ', 'M', 'E', 'D', 'I', 'U', 'M'},
};

// current position of the player
int currRow = 4,
    currCol = 4;

int matrixBrightness = 2;

const int highscores = 5,
          nameSize = 3;

char highscoreNames[highscores][nameSize];
int highscoreValues[highscores];
char currName[nameSize];
char display[nameSize];


// initialize with true to reinitialize EEPROM
bool initEEPROM = false;

// highscore variables for placement achieved and iteration
int placeHighscore = -1,
    highscoreIndex = highscores - 1;

//const byte lcdBacklightPin = 6;

void setup() {
  pinMode(pinSW, INPUT_PULLUP);
  pinMode(lcdBacklightPin, OUTPUT);

  matrixBrightness = 2;
  EEPROM.put(matrixBrightnessAddress, matrixBrightness);

  lc.shutdown(0, false);
  lc.setIntensity(0, matrixBrightness);
  lc.clearDisplay(0);

  EEPROM.put(soundAddress, LOW);

  lcd.begin(16, 2);

  // custom chars
  lcd.createChar(0, charDownArrow);
  lcd.createChar(1, charUpArrow);
  lcd.createChar(2, charHeart);
  lcd.createChar(3, charClock);
  lcd.createChar(4, charCup);
  lcd.createChar(5, charStar);

  // initializing EEPROM memory
  if (initEEPROM) {
    // current name
    address = currNameStartingAddress;

    for (int i = 0 ; i < nameSize; i++){
      EEPROM.put(address, char(' '));
      address += sizeof(char);
    }

    for (int i = 0 ; i < nameSize; i++){
      display[i] = 'a';
    }

    // highscores
    address = highscoreStartingAddress;
    
    for (int i = 0; i < highscores; i++){
      for (int j = 0; j < nameSize; j++){
        EEPROM.put(address, char('?'));
        address += sizeof(char);
      }
    }
    
    for (int i = 0; i < highscores; i++){
      EEPROM.put(address, 0);
      address += sizeof(int);
    }

    // difficulty
    EEPROM.put(currDiffAddress, 0);
    // sound
    EEPROM.put(soundAddress, LOW);

    // led brightness
    EEPROM.put(ledBrightnessAddress, 255);

  }

  address = highscoreStartingAddress;
    
  for (int i = 0; i < highscores; i++){
    for (int j = 0; j < nameSize; j++){
      EEPROM.get(address, highscoreNames[i][j]);
      address += sizeof(char);
    }
  }
    
  for (int i=0; i < highscores; i++){
    EEPROM.get(address, highscoreValues[i]);
    address += sizeof(int);
  }

  currState = WELCOME;

  analogWrite(lcdBacklightPin, 255);
}


void loop() {
 parseCurrState();
}

void parseCurrState() {
  if (currState == WELCOME) {
    enterWelcome();
  } 
  else if (currState == MENU) {
    enterMenu();
  }
  else if (currState == START_GAME) {
    enterGame();
  }
  else if (currState == SETTINGS) {
    enterSettings();
  }
  else if (currState == ABOUT) {
    enterAbout();
  }
  else if (currState == HOW_TO) {
    enterHowTo();
  }
  else if (currState == HIGHSCORE) {
    enterHighscore();
  }
  else if (currState == END_GAME) {
    endGame();
  }
  else if (currState == SET_SOUND) {
    setSound();
  }
  else if (currState == SET_DIFFICULTY) {
    setDifficulty();
  }
  else if (currState == SET_BRIGHTNESS) {
    setBrightness();
    //setMatrixBrightness();
  }
  else if (currState == ENTER_NAME) {
    enterName();
  }
  else if(currState == SET_MATRIX_BRIGHTNESS ) {
    setMatrixBrightness();
  }
  else if(currState == SET_LCD_BRIGHTNESS ) {
    setLcdBrightness();
  }
}


void assignStateToBuzzer(int buzzerTone, byte buzzerState) {
  if (buzzerState == HIGH) {
    tone(buzzerPin, buzzerTone);
  } 
  else {
    noTone(buzzerPin);
  }
}

void setBrightness() {
  static const char brightnessMatrix[][16] = {
    {'B', 'r', 'i', 'g', 'h', 't', 'n', 'e', 's', 's', ' ', ' ', ' ', ' ', ' '},
    {'M', 'a', 't', 'r', 'i', 'x', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'L', 'c', 'd', ' ', 'D', 'i', 's', 'p', 'l', 'a', 'y', ' ', ' ', ' ', ' '},
  };

  const int brightnessOptions = 3;

  scrollThrough(brightnessMatrix, brightnessOptions, brightnessOptionIndex, SETTINGS, 1, brightnessOptions - 1, 1);

  switchPress = getSwitchPress();

  if (switchPress != NONE) {
    parseBrightnessOption();
  }
} 

void enterWelcome() {
  displayMatrix(welcomeBitmap);
  displayText(beginMatrix, 6);
}

void parseBrightnessOption() {
  lcd.clear();

  if (brightnessOptionIndex == 1) {
    currState = SET_MATRIX_BRIGHTNESS;
  }
  else if (settingsIndex == 2) {
    lcd.setCursor(0,0);
    currState = SET_LCD_BRIGHTNESS;
  }

  joystickMove = getJoystickMove();
  if (joystickMove == DOWN) {
    lcd.clear();
    currState = SETTINGS;
  }
}

void enterMenu() {
  static const char menuMatrix[][16] = {
    {' ', ' ', ' ', ' ', ' ', 'M', 'e', 'n', 'u', ' ', ' ', ' ', ' ', ' ', ' '},
    {'S', 't', 'a', 'r', 't', ' ', 'G', 'a', 'm', 'e', ' ', ' ', ' ', ' ', ' '},
    {'H', 'i', 'g', 'h', 's', 'c', 'o', 'r', 'e', 's', ' ', ' ', ' ', ' ', ' '},
    {'S', 'e', 't', 't', 'i', 'n', 'g', 's', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'A', 'b', 'o', 'u', 't', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'H', 'o', 'w', ' ', 't', 'o', ' ', 'p', 'l', 'a', 'y', ' ', ' ', ' ', ' '},
  };

  static const byte startGameBitmap[matrixSize][matrixSize] = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 1, 1, 1, 1},
    {1, 1, 0, 0, 1, 1, 1, 1},
    {1, 1, 0, 0, 0, 0, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
  };

  displayMatrix(welcomeBitmap);
  scrollThrough(menuMatrix, menuOptions, menuIndex, MENU, 1, menuOptions - 1, 1);

  switchPress = getSwitchPress();

  if (switchPress != NONE) {
    parseMenuOption(menuIndex);
  }
}

void enterGame() { 
  inGameMovement();
  generateFood();
  displayLcdGame();  

  if (lifes == 0) {
    lcd.clear();
    currState = END_GAME;
  }
}

void enterSettings() {
  static const byte settingsBitmap[matrixSize][matrixSize] = {
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
  };

  static const char settingsMatrix[][16] = {
    {' ', ' ', ' ', 'S', 'e', 't', 't', 'i', 'n', 'g', 's', ' ', ' ', ' ', ' '},
    {'E', 'n', 't', 'e', 'r', ' ', 'N', 'a', 'm', 'e', ' ', ' ', ' ', ' ', ' '},
    {'B', 'r', 'i', 'g', 'h', 't', 'n', 'e', 's', 's', ' ', ' ', ' ', ' ', ' '},
    {'S', 'o', 'u', 'n', 'd', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'D', 'i', 'f', 'f', 'i', 'c', 'u', 'l', 't', 'y', ' ', ' ', ' ', ' ', ' '},
  };

  displayMatrix(settingsBitmap);
  scrollThrough(settingsMatrix, settingsOptions, settingsIndex, MENU, 1, settingsOptions - 1, 1);

  switchPress = getSwitchPress();

  if (switchPress != NONE) {
    lcd.clear();
    parseSettingsOption();
  }
}


void setSound() {
  static const char soundMatrix[][16] = {
    {'S', 'o', 'u', 'n', 'd', ' ', 'S', 'e', 't', 't', 'i', 'n', 'g', 's', ' '},
    {'O', 'N', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'O', 'F', 'F', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
  };

  static byte buzzerState = LOW;

  scrollThrough(soundMatrix, soundOptions, soundIndex, MENU, 1, soundOptions - 1, 1);

  switchPress = getSwitchPress();

  if (switchPress != NONE) {
    if (soundIndex == 1) {
      EEPROM.put(soundAddress, 1);
    }
    else
    {
      EEPROM.put(soundAddress, 0);
    }

    EEPROM.get(soundAddress, buzzerState);
    assignStateToBuzzer(700, buzzerState);
  }

}

void parseSettingsOption() {
  lcd.clear();

  if (settingsIndex == 1) {
    currState = ENTER_NAME;
  }
  else if (settingsIndex == 2) {
    lcd.setCursor(0,0);
    currState = SET_BRIGHTNESS;
  }
  else if (settingsIndex == 3) {
    currState = SET_SOUND;
  }
  else if (settingsIndex == 4) {
    currState = SET_DIFFICULTY;
  }

  joystickMove = getJoystickMove();
  if (joystickMove == DOWN) {
    lcd.clear();
    currState = SETTINGS;
  }
}

void enterAbout() {
  static const byte aboutBitmap[matrixSize][matrixSize] = {
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
  };

  static const char aboutMatrix[][16] = {
    {' ', ' ', ' ', ' ', ' ', 'A', 'b', 'o', 'u', 't', ' ', ' ', ' ', ' ', ' '},
    {'S', 'n', 'a', 'k', 'e', ' ', 'R', 'e', 'd', 'e', 'e', 'm', 'e', 'd', ' '},
    {'A', 'u', 't', 'h', 'o', 'r', ' ', 'I', 'o', 'a', 'n', 'a', ' ', ' ', ' '},
    {'L', 'i', 'v', 'i', 'a', ' ', 'P', 'o', 'p', 'e', 's', 'c', 'u', ' ', ' '},
    {'G', 'i', 't', 'h', 'u', 'b', ' ', 'A', 'c', 'c', 'o', 'u', 'n', 't', ' '},
    {'I', 'o', 'a', 'n', 'a', 'L', 'i', 'v', 'i', 'a', ' ', ' ', ' ', ' ', ' '},
  };

  displayMatrix(aboutBitmap);
  scrollThrough(aboutMatrix, aboutOptions, aboutIndex, MENU, 1, aboutOptions - 1, 1);
}

void scrollThrough(const char matrix[][16], const int options, int &index, const int returnToState, const int lowerBoundIndex, const int upperBoundIndex, const int upperBoundCursorRow) {
  if (upperBoundCursorRow == 0) {
    lcd.setCursor(0,0);
    lcd.print(matrix[index]);
    lcd.setCursor(0,1);
    lcd.print("Press to choose");
  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print(matrix[0]);
    lcd.setCursor(0,1);
    lcd.print(matrix[index]);
  }

  displayArrows();

  joystickMove = getJoystickMove();

  if (joystickMove == LEFT) {
    lcd.clear();
    index = max (index - 1, lowerBoundIndex);
  }  
  else if (joystickMove == RIGHT) {
    lcd.clear();
    index  = min (index + 1, upperBoundIndex);
  }
  else if (joystickMove == DOWN) {
    lcd.clear();
    currState = returnToState;
  }

  canScrollUp = !(index == lowerBoundIndex);
  canScrollDown = !(index == upperBoundIndex);
}

void enterHowTo() {
  static const byte howToBitmap[matrixSize][matrixSize] = {
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
  };

  static const char howToMatrix[][16] = {
    {' ', ' ', 'H', 'o', 'w', ' ', 't', 'o', ' ', 'p', 'l', 'a', 'y', ' ', ' '},
    {'C', 'o', 'l', 'l', 'e', 'c', 't', ' ', 'f', 'o', 'o', 'd', '!', ' ', ' '},
    {'U', 's', 'e', ' ', 'j', 'o', 'y', 's', 't', 'i', 'c', 'k', ' ', 't', 'o'},
    {'m', 'o', 'v', 'e', ' ', 'W', 'A', 'S', 'D', '.', ' ', ' ', ' ', ' ', ' '},
    {'D', 'o', 'n', ' ', 't', ' ', 't', 'o', 'u', 'c', 'h', ' ', 't', 'h', 'e'},
    {'e', 'd', 'g', 'e', 's', '!', ' ', 'Y', 'o', 'u', ' ', 'w', 'i', 'l', 'l'},
    {'l', 'o', 's', 'e', ' ', 'a', ' ', 'l', 'i', 'f', 'e', '!', '!', '!', ' '},
    {'H', 'a', 'v', 'e', ' ', 'S', 's', 'o', 'm', 'e', ' ', 'F', 'u', 'n', '!'},
  };

  displayMatrix(howToBitmap);
  scrollThrough(howToMatrix, howToOptions, howToIndex, MENU, 1, howToOptions - 1, 1);
}

void enterHighscore() {
  displayMatrix(highscoreBitmap);

  lcd.setCursor(0,0);
  lcd.print("Highscores");
  lcd.setCursor(0,1);

  for(int j = 0; j < nameSize; ++j) {
    lcd.print(highscoreNames[highscoreIndex][j]);
  }

  lcd.print(" ");
  lcd.print(highscoreValues[highscoreIndex]);
  displayArrows();

  joystickMove = getJoystickMove();

  if (joystickMove == LEFT) {
    highscoreIndex = max(highscoreIndex - 1, 0);
  }  
  else if (joystickMove == RIGHT) {
    highscoreIndex = min(highscoreIndex + 1, highscores - 1);
  }
  else if (joystickMove == DOWN) {
    lcd.clear();
    currState = MENU;
  }

  canScrollUp = !(highscoreIndex == 0);
  canScrollDown = !(highscoreIndex == highscores - 1);
}

void endGame() {
  static long called = -1;

  if (called == -1) {
    called = millis();
  }

  if (millis() - called > 3000 && called != -1) {
    displaySecondEndScreen();
    
    int press = getSwitchPress();
    if (press != NONE) {
      lcd.clear();
      lc.setLed(0, currRow, currCol, 0);
      lc.setLed(0, foodRow , foodCol, 0);
      currCol = 0;
      currRow = 0;
      currState = MENU;
      called = -1;
      currScore = 0;
      gameStartTime = 0;
      lifes = 3;
      noOfMinutes = 0;
      noOfSeconds = 0;
      foodCol = 0;
      foodRow = 0;
    }
  }
  else
  { 
    displayFirstEndScreen();
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

// returns current joystick move 
int getJoystickMove() {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  xChange = abs(initialXValue - xValue);
  yChange = abs(initialYValue - yValue);

  if (!joyMoved) {     // || (currState == START_GAME
    if (yChange >= xChange) {
      if (yValue < minThreshold) {
        joyMoved = true;
        return UP;
      }
      if (yValue > maxThreshold) {
        joyMoved = true;
        return DOWN;
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

void displayFirstEndScreen() {
  static long called = millis(); 
  address = currNameStartingAddress;

  lcd.setCursor(0,0);
  lcd.print("Congrats!   ");

  for (int i = 0 ; i < nameSize; i++){
    EEPROM.get(address, currName[i]);
    lcd.print(currName[i]);
    address += sizeof(char);
  }

  lcd.setCursor(0,1);
  lcd.print("Level: ");
  displayNumber(currLevel, 1, 9);
  lcd.print("!");
}

void displaySecondEndScreen() {
  static const byte sadBitmap[matrixSize][matrixSize] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
  };

  lcd.setCursor(0,0);
  lcd.print("Your score      ");
  displayNumber(currScore, 0, 12);

  if (placeHighscore == -1) {
    for (int i = highscores - 1; i >= 0; i--){
      if (currScore > highscoreValues[i] && currScore != 0) {
        placeHighscore = i;
        break;
      }
    }

      if(placeHighscore != -1) {
        for(int i = 0; i < placeHighscore; ++i) {
          for(int j = 0; j < nameSize; j++) {
            highscoreNames[i][j] = highscoreNames[i + 1][j];
          }
          highscoreValues[i] =  highscoreValues[i + 1];
        }

        highscoreValues[placeHighscore] = currScore;
        for(int j = 0; j < nameSize; j++) {
          highscoreNames[placeHighscore][j] = currName[j];
        }

        saveHighscores();
    }
  }

  if(placeHighscore != -1){
    lcd.setCursor(0,1);
    displayMatrix(highscoreBitmap);
    lcd.print("New in Top 5!!!");
  }
  else
  { 
    lcd.setCursor(0,1);
    displayMatrix(sadBitmap);
    lcd.print("...");
    lcd.setCursor(0,1);
    lcd.print("Not a highscore...");
  }

}

void displayNumber(const int number, const int cursorRow, int cursorColumn) {
  lcd.setCursor(cursorColumn, cursorRow);
  if (number >= 100) {
   lcd.print(digits[(int)number / 100]);
  }
  else
  {
    lcd.print(" ");
  }
  if (number >= 10) {
    lcd.print(digits[(number / 10) % 10]);
  }
  else
  {
    lcd.print(" ");
  }
  cursorColumn += 2;
  lcd.print(digits[number % 10]);
  lcd.setCursor(cursorColumn + 1, cursorRow);
}

// universal display matrix function based on bitmap
void displayMatrix(byte matrix[][8]) {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrix[row][col]);
    }
  }
}

// display arrows based on the current values of canScrollUp and canScrollDown
void displayArrows() {
  // up arrow
  lcd.setCursor(15,0);
  if (canScrollUp) {
    lcd.write(byte(1));
  }
  else
  {
    lcd.print(" ");
  }

  //down arrow
  lcd.setCursor(15,1);
  if (canScrollDown) {
    lcd.write(byte(0));
  }
  else
  {
    lcd.print(" ");
  }
}

// displays all lcd game required information plus timer
void displayLcdGame() {
  static long lastScoreChange = 0;
  static int changeInterval = 1000;

  displayLife();
  displayScore();
  displayTimer();
  displayLevel();
  displayDifficulty();
}

void displayScore() {
  lcd.setCursor(4,0);
  lcd.write(byte(5));
  lcd.setCursor(6,0);
  if (currScore < 10) {
    lcd.print(digits[currScore]);
  }
  else {
    lcd.print(digits[currScore / 10]);
    lcd.print(digits[currScore % 10]);
  }
}

void displayLife() {
  lcd.setCursor(0,0);
  lcd.write(byte(2));
  lcd.setCursor(2,0);
  lcd.print(digits[lifes]);
}

void displayTimer() {
  lcd.setCursor(10,0);
  lcd.write(byte(3));
  lcd.setCursor(11,0);

  noOfMinutes = (millis() - gameStartTime) / 60000;
  if (noOfMinutes < 10) {
    lcd.print("0");
    lcd.setCursor(12,0);
  }

  lcd.print(noOfMinutes);
  lcd.print(":");
  lcd.setCursor(14,0);

  noOfSeconds = (millis() - gameStartTime) / 1000 -  60 * noOfMinutes;

  if(noOfSeconds >= 60) {
    noOfMinutes++;
    noOfSeconds -= 60;
  }

  if (noOfSeconds < 10) {
    lcd.print("0");
  }
  lcd.print(noOfSeconds);
}

void displayLevel() {
  lcd.setCursor(0,1);
  lcd.print("Level ");
  lcd.print(digits[currLevel]);
  lcd.setCursor(8,1); //
  lcd.print("  "); //
  lcd.setCursor(10,1);
  lcd.print(difficulty[0]);
}

void displayDifficulty() {
  EEPROM.get(currDiffAddress, diffIndex);

  lcd.setCursor(10,1);
  lcd.print(difficulty[diffIndex]);
}

// universal function to display text letter by letter at scrollInterval on two rows (used for welcome text)
void displayText(const char matrix[][16], const int noOfLines) {
  static int scrollInterval = 175;
  static int lineIndex = 0;
  static int columnIndex = 0;
  static int cursorRow = 0;

  if (lastMessageTime == -1 && millis() != 0) {
    lcd.setCursor(0, cursorRow);
    lastMessageTime = millis();
  }

  if (millis() - lastMessageTime >= scrollInterval && columnIndex < 17) {
    lcd.print(matrix[lineIndex][columnIndex]);
    lastMessageTime = millis();
    columnIndex += 1;
  }
  
  if (columnIndex == 17) {
    lastMessageTime = -1;
    columnIndex = 0;
    lineIndex += 1;
    if(lineIndex == noOfLines) {
      lcd.clear();
      currState = MENU;
    }
    if(cursorRow == 1) {
      lcd.clear();
      cursorRow = 0;
    }
    else {
      cursorRow = 1;
    }
  }
}

void generateFood() {
  static bool expiredFoodTime = false;
  static long generatedFood = 0;
  static long foodOnScreenInterval = 0;
  static int intervalMultiplier = 10000;

  foodOnScreenInterval = (diffOptions - EEPROM.read(currDiffAddress)) * intervalMultiplier;

  if (inLevel) {
    if (collectedFood || currScore == -1 || expiredFoodTime) {
      collectedFood = false;
      if (currScore == -1) currScore = 0;
      generatedFood = millis();
      expiredFoodTime = false;
      foodCol = rand() % 8, foodRow = rand() % 8;
      if(foodCol == currCol && foodRow == currRow) { // avoid same position
        foodCol--;
      }
    }

    if(millis() - generatedFood >= foodOnScreenInterval) {
      expiredFoodTime = true;
      lc.setLed(0, foodCol, foodRow, 0);
    }

    blinkLed(foodCol, foodRow, 1000);
  }
  
}

void inGameMovement() {
  static int move = NONE;

  move = getJoystickMove();
  
  if (move != NONE) {
    lc.setLed(0, currCol, currRow, 0);

    if (move == LEFT) {
      if (currCol ==  0) {
        lifes--;
      }
      currCol = max(currCol - 1, 0);
    }
    if (move == RIGHT) {
      if (currCol ==  matrixSize - 1) {
        lifes--;
      }
      currCol = min(currCol + 1, matrixSize - 1);
    }
    if (move == UP) {
      if (currRow == matrixSize - 1) {
        lifes--;
      }
      currRow = min(currRow + 1, matrixSize - 1);
    }
    if (move == DOWN) {
      if (currRow == 0) {
        lifes--;
      }
      currRow = max(currRow - 1, 0);
    }

    
    if(currCol == foodCol && currRow == foodRow) {
      collectedFood = true;
      lc.setLed(0, foodCol, foodRow, 0);
      currScore += 1;
    }
    else
    {
      collectedFood = false;
    }
  }

  lc.setLed(0, currCol, currRow, 1);
}

void parseMenuOption(const int menuIndex) {
    lcd.clear();

    for (int row = 0; row < matrixSize; row++) {
      for (int col = 0; col < matrixSize; col++) {
        lc.setLed(0, row, col, 0);
      }
    }

    if (menuIndex == 1) {
      currState = START_GAME;
      gameStartTime = millis();
      lastJoystickMove = NONE;
      placeHighscore = -1;
      enterGame();
    }
    if (menuIndex == 2) {
      readHighscores();
      address = highscoreStartingAddress;
      currState = HIGHSCORE;
      enterHighscore();
    }
    if (menuIndex == 3) {
      currState = SETTINGS;
      enterSettings();
    }
    if (menuIndex == 4) {
      currState = ABOUT;
      enterAbout();
    }
    if (menuIndex == 5) {
      currState = HOW_TO;
      enterHowTo();
    }
}

void blinkLed (const int randomCol, const int randomRow, const int blinkInterval) {
  static byte blinkState = 1;

  if (millis() - lastBlinkTime >= blinkInterval) {
    blinkState = !blinkState;
    lc.setLed(0, randomCol, randomRow, blinkState);
    lastBlinkTime = millis();
  }
}

void assignStateToBuzzer(int buzzerTone, byte buzzerState, int buzzerPin) {
  if (buzzerState == HIGH) {
    tone(buzzerPin, buzzerTone);
  } 
  else {
    noTone(buzzerPin);
  }
}

void setDifficulty() {
  scrollThrough(difficulty, diffOptions, diffIndex, SETTINGS, 0, diffOptions - 1, 0);

  switchPress = getSwitchPress();

  if (switchPress != NONE) {
    EEPROM.put(currDiffAddress, diffIndex);
  }
}

void setMatrixBrightness() {
  static int matrixBrightness = 2;

  lc.setIntensity(0, matrixBrightness);
  lcd.setCursor(0,0);
  lcd.print("Brightness:");
  lcd.setCursor(0,1);
  lcd.print(matrixBrightness);
  displayArrows();

  joystickMove = getJoystickMove();

  if (joystickMove == LEFT) {
    lcd.clear();
    matrixBrightness = max(matrixBrightness - 1, 0);
    EEPROM.put(matrixBrightnessAddress, matrixBrightness);
  }  
  else if (joystickMove == RIGHT) {
    lcd.clear();
    matrixBrightness = min(matrixBrightness + 1, 15);
    EEPROM.put(matrixBrightnessAddress, matrixBrightness);
  }
  else if (joystickMove == DOWN) {
    lcd.clear();
    currState = SET_BRIGHTNESS;
  }

  canScrollUp = !(matrixBrightness == 15);
  canScrollDown = !(matrixBrightness == 0);
}

void setLcdBrightness() {
  static int ledBrightness = 255;

  EEPROM.get(ledBrightnessAddress, ledBrightness);

  lcd.setCursor(0,0);
  lcd.print("Brightness:");
  lcd.setCursor(0,1);
  lcd.print(ledBrightness);
  displayArrows();

  analogWrite(lcdBacklightPin, ledBrightness);

  joystickMove = getJoystickMove();

  if (joystickMove == LEFT) {
    lcd.clear();
    ledBrightness = min(ledBrightness + 10, 255);
    EEPROM.put(ledBrightnessAddress, ledBrightness);
  }  
  else if (joystickMove == RIGHT) {
    lcd.clear();
    ledBrightness = max(ledBrightness - 10, 0);
    EEPROM.put(ledBrightnessAddress, ledBrightness);
  }
  else if (joystickMove == DOWN) {
    lcd.clear();
    EEPROM.put(ledBrightnessAddress, ledBrightness);
    currState = SET_BRIGHTNESS;
  }

  canScrollUp = !(ledBrightness == 255);
  canScrollDown = !(ledBrightness == 0);
}

void enterName() {
  static char letters[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', ' '};

  joystickMove = getJoystickMove();

  lcd.setCursor(0, 1);
  lcd.print("Press to save.");
  lcd.setCursor(letterPos, 0);
  lcd.print(letters[letterIndex]);
  lcd.setCursor(9,0);
  lcd.print("Max:3");
  displayArrows();


  if (joystickMove == LEFT) {
    letterIndex = max(letterIndex - 1, 0);
    display[letterPos] = letters[letterIndex];
  }  
  else if (joystickMove == RIGHT) {
    letterIndex = min(letterIndex + 1, 26);
    display[letterPos] = letters[letterIndex];
  }
  else if (joystickMove == UP) {
    display[letterPos] = letters[letterIndex];
    letterPos += 1;
    letterIndex = 0;
    if (letterPos == nameSize) {
      letterPos = 0;
    }
  }
  else if (joystickMove == DOWN) {
    lcd.clear();
    letterPos = 0;
    letterIndex = 0;
    
    for (int i = 0 ; i < nameSize; i++){
      display[i] = 'a';
    }

    currState = SETTINGS;
  }

  canScrollUp = !(letterIndex == 0);
  canScrollDown = !(letterIndex == 26);

  switchPress = getSwitchPress();

  if (switchPress != NONE) {
    saveName();
  }

}

void saveName() {
  address = currNameStartingAddress;

  for (int i = 0 ; i < nameSize; i++){
    EEPROM.put(address, display[i]);
    address += sizeof(char);
  }
}

void saveHighscores() {
  address = highscoreStartingAddress;
    
  for (int i = 0; i < highscores; i++){
    for (int j = 0; j < nameSize; j++){
      EEPROM.put(address, highscoreNames[i][j]);
      address += sizeof(char);
    }
  }
  
  for (int i = 0; i < highscores; i++){
    EEPROM.put(address, highscoreValues[i]);
    address += sizeof(int);
  }
}

void readHighscores() {
  address = highscoreStartingAddress;
    
  for (int i = 0; i < highscores; i++){
    for (int j = 0; j < nameSize; j++){
      EEPROM.get(address, highscoreNames[i][j]);
      address += sizeof(char);
    }
  }
  
  for (int i = 0; i < highscores; i++){
    EEPROM.get(address, highscoreValues[i]);
    address += sizeof(int);
  }
}

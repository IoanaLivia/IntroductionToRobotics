// Controls RGB led intensity of each color by mapping analog input values (from 3 potentiometers, each corresponding to a different color) to led intensity output values

// Maximum and minimum values of led intensity and potentiomer analog input
const int minLedIntensity = 0,
          maxLedIntensity = 255,
          minAnalogValue = 0,
          maxAnalogValue = 1023;

// In order to correctly set the values of the led pins, we will set commonCathode to true or false, depending of RGB led type (common cathode or common anode)
bool commonCathode = true;

// 'resPin' refers to a pin connected to an entity that controls 'resistance' values : in this case, it refers to a potentiometer, would also maintain logic for a slider etc
const int resPinRed = A0,
          resPinGreen = A1,
          resPinBlue = A2;

const int ledPinRed = 11,
          ledPinGreen = 10,
          ledPinBlue = 9;

// Analog input values from potentiometer, each will range between minAnalogValue and maxAnalogValue
int resValueRed = 0,
    resValueGreen = 0,
    resValueBlue = 0;
  
// Output values for led intensity, each will range between minLedIntensity and maxLedIntensity (in case of common cathode, reverse order otherwise)
int ledValueRed = 0,
    ledValueGreen = 0,
    ledValueBlue = 0;

void setup() {
  pinMode(resPinRed, INPUT);
  pinMode(resPinGreen, INPUT);
  pinMode(resPinBlue, INPUT);

  pinMode(ledPinRed, OUTPUT);
  pinMode(ledPinGreen, OUTPUT);
  pinMode(ledPinBlue, OUTPUT);
}

void loop() {
  setResValues();
  setLedValues();
  assignLedValuesToLedPins();
}

void setResValues() {
  resValueRed = analogRead(resPinRed);
  resValueGreen = analogRead(resPinGreen);
  resValueBlue = analogRead(resPinBlue);
}

void setLedValues() {
  // Analog input values will be mapped to led intensity values
  ledValueRed = map(resValueRed, minAnalogValue, maxAnalogValue, minLedIntensity, maxLedIntensity);
  ledValueGreen = map(resValueGreen, minAnalogValue, maxAnalogValue, minLedIntensity, maxLedIntensity);
  ledValueBlue = map(resValueBlue, minAnalogValue, maxAnalogValue, minLedIntensity, maxLedIntensity);

  // In case of a common anode, 0 (minLedIntensity) will be the maximum and 255 (maxLedIntensity) the minimum brightness
  if (!commonCathode) {
    ledValueRed = maxLedIntensity - ledValueRed;
    ledValueGreen = maxLedIntensity - ledValueGreen;
    ledValueBlue = maxLedIntensity - ledValueBlue;
  }
}

void assignLedValuesToLedPins() {
  analogWrite(ledPinRed, ledValueRed);
  analogWrite(ledPinGreen, ledValueGreen);
  analogWrite(ledPinBlue, ledValueBlue);
}
#include <Wire.h>


#define PIN_MODULE1_RELAIS1 41
#define PIN_MODULE1_RELAIS2 42
#define PIN_MODULE1_RELAIS3 43
#define PIN_MODULE1_RELAIS4 44
static const int MODULE1[] = { PIN_MODULE1_RELAIS1, PIN_MODULE1_RELAIS2, PIN_MODULE1_RELAIS3, PIN_MODULE1_RELAIS4 };

#define PIN_MODULE2_RELAIS1 45
#define PIN_MODULE2_RELAIS2 46
#define PIN_MODULE2_RELAIS3 47
#define PIN_MODULE2_RELAIS4 48
static const int MODULE2[] = { PIN_MODULE2_RELAIS1, PIN_MODULE2_RELAIS2, PIN_MODULE2_RELAIS3, PIN_MODULE2_RELAIS4 };

#define PIN_MODULE3_RELAIS1 49
#define PIN_MODULE3_RELAIS2 50
#define PIN_MODULE3_RELAIS3 51
#define PIN_MODULE3_RELAIS4 52
static const int MODULE3[] = { PIN_MODULE3_RELAIS1, PIN_MODULE3_RELAIS2, PIN_MODULE3_RELAIS3, PIN_MODULE3_RELAIS4 };

#define PIN_MODULE4_RELAIS1 53
#define PIN_MODULE4_RELAIS2 54
#define PIN_MODULE4_RELAIS3 55
#define PIN_MODULE4_RELAIS4 56
static const int MODULE4[] = { PIN_MODULE4_RELAIS1, PIN_MODULE4_RELAIS2, PIN_MODULE4_RELAIS3, PIN_MODULE4_RELAIS4 };

#define SERIAL_PORT_A0 78
#define SERIAL_PORT_A1 79
#define SERIAL_PORT_A2 80
#define SERIAL_PORT_A3 81

static const int V_IN = 5;
static const int ANALOG_SCALE = 1024;
static const int RESISTOR_REFERENCE = 1000;
static const int FRACTION_RESULT_FACTOR = 1;

static const int DELAY = 10000; // 10 seconds
void setup() {
  // put your setup code here, to run once:
  setPinsAsOutput(MODULE1, 4);
  setPinsAsOutput(MODULE2, 4);
  setPinsAsOutput(MODULE3, 4);
  setPinsAsOutput(MODULE4, 4);

  enableAnalogInput();
}
void setPinsAsOutput(const int pins[], int length) {  
  for (int i = 0; i < length; i++) {
    pinMode(pins[i], OUTPUT);          // sets the digital pin as output
  }
}

void enableAnalogInput() {
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  enableModule(MODULE1);
  float resistanceOhm1 = calculateOhm(SERIAL_PORT_A0);
  disableModule(MODULE1);

  enableModule(MODULE2);
  float resistanceOhm2 = calculateOhm(SERIAL_PORT_A1);
  disableModule(MODULE2);
  
  enableModule(MODULE3);
  float resistanceOhm3 = calculateOhm(SERIAL_PORT_A2);
  disableModule(MODULE3);

  enableModule(MODULE4);
  float resistanceOhm4 = calculateOhm(SERIAL_PORT_A3);
  disableModule(MODULE4);
  
  Serial.println(resistanceOhm1);
  Serial.println(resistanceOhm2);
  Serial.println(resistanceOhm3);
  Serial.println(resistanceOhm4);
  delay(DELAY);
}

void enableModule(const int modulePins[4]) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(modulePins[i], HIGH);       // sets the digital pin on
  }
}

void disableModule(const int modulePins[4]) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(modulePins[i], LOW);       // sets the digital pin off
  }
}

float calculateOhm(const int analogPin) {
  int rawValue = analogRead(analogPin);
  if (rawValue) {
    float buffer = rawValue * V_IN;
    int vOut = buffer / ANALOG_SCALE;
    buffer = (V_IN / vOut) - FRACTION_RESULT_FACTOR;
    return (RESISTOR_REFERENCE * buffer);
  }
  return 0.0;
}


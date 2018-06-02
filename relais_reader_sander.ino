#define FEED12V_SWITCH        22
#define LOW_HIGH_COIL_SWITCH  23

#define LOW_HIGH_CONTACT_SWITCH1 24
#define LOW_HIGH_CONTACT_SWITCH2 25
#define LOW_HIGH_CONTACT_SWITCH3 26

#define RED_LED     36
#define ORANGE_LED  38
#define GREEN_LED   40

#define START_BUTTON 42

#define SERIAL_PORT_A0 54
#define SERIAL_PORT_A1 55
#define SERIAL_PORT_A2 56
#define SERIAL_PORT_A3 81

static const int V_IN = 5;
static const int ANALOG_SCALE = 1024;
static const int RESISTOR_REFERENCE = 10;
static const int FRACTION_RESULT_FACTOR = 1;

static const int DELAY = 10000; // 10 seconds
void setup() {
  // Document point 1
  // put your setup code here, to run once:
  int pins[] = {FEED12V_SWITCH, LOW_HIGH_COIL_SWITCH, LOW_HIGH_CONTACT_SWITCH1, LOW_HIGH_CONTACT_SWITCH2, LOW_HIGH_CONTACT_SWITCH3};
  setPinsAsOutput(pins, 5);

  enableAnalogInput();
//  enable display
  // set start led on
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
  if (!readStartButton(START_BUTTON)) {
      return;
  }
// Document point 2
  // oranje led aan

// Document point 3
// Document point 3.a
// measure relais 18/19 via A0
  float resistanceOhm18And19 = calculateOhm(SERIAL_PORT_A0);
// measure relais 16/17 via A1 
  float resistanceOhm16And17 = calculateOhm(SERIAL_PORT_A1);
// measure relais 12/13 via A2
  float resistanceOhm12And13 = calculateOhm(SERIAL_PORT_A2);

// Document point 3.b
// relais 1 HIGH pin 22
  digitalWrite(FEED12V_SWITCH, HIGH); // K1
// Document point 3.c
// measure relais 19/20 via A0
  float resistanceOhm19And20 = calculateOhm(SERIAL_PORT_A0);
// measure relais 14/15 via A1 
  float resistanceOhm14And15 = calculateOhm(SERIAL_PORT_A1);
// Document point 3.d  
  digitalWrite(FEED12V_SWITCH, LOW); // K1

// Document point 3.e
  pinHighAndWait(LOW_HIGH_CONTACT_SWITCH1);
  pinHighAndWait(LOW_HIGH_CONTACT_SWITCH2);
  pinHighAndWait(LOW_HIGH_CONTACT_SWITCH3);
// Document point 3.f
// measure relais 68/69 via A0
  float resistanceOhm68And69 = calculateOhm(SERIAL_PORT_A0);
// measure relais 66/67 via A1 
  float resistanceOhm66And67 = calculateOhm(SERIAL_PORT_A1);
// measure relais 62/63 via A2
  float resistanceOhm62And63 = calculateOhm(SERIAL_PORT_A2);

  // Document point 3.g
  pinHighAndWait(LOW_HIGH_COIL_SWITCH);
  pinHighAndWait(FEED12V_SWITCH);

  // Document point 3.h
// measure relais 69/70 via A0
  float resistanceOhm69And70 = calculateOhm(SERIAL_PORT_A0);
// measure relais 64/65 via A1 
  float resistanceOhm64And65 = calculateOhm(SERIAL_PORT_A1);

  // Document point 3.h
  pinLowAndWait(FEED12V_SWITCH);
  pinLowAndWait(LOW_HIGH_COIL_SWITCH);
  pinLowAndWait(LOW_HIGH_CONTACT_SWITCH1); 
  pinLowAndWait(LOW_HIGH_CONTACT_SWITCH2);
  pinLowAndWait(LOW_HIGH_CONTACT_SWITCH3);

  // calculate tresshold

  // show green led if success

  // show failed contacts on display
  // show failed enable red led
  // tresshold 10 ohm
  
//  Serial.println(resistanceOhm1);
//  Serial.println(resistanceOhm2);
//  Serial.println(resistanceOhm3);
//  Serial.println(resistanceOhm4);
  delay(DELAY);
}

bool readStartButton(const int startButtonPin) {
  return true;
}


void pinHighAndWait(const int pin) {
  digitalWrite(pin, HIGH);       // sets the digital pin on
  delay(100);
}

void pinLowAndWait(const int pin) {
  digitalWrite(pin, LOW);       // sets the digital pin off
  delay(100);
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


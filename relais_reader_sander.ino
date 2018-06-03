#include "U8glib.h"

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

#define V_IN  5
#define ANALOG_SCALE 1024
#define RESISTOR_REFERENCE 10
#define FRACTION_RESULT_FACTOR 1
#define MARGIN 10

#define DELAY 10000 // 10 seconds

U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_DEV_1); // Dev 0, Fast I2C / TWI


void setup() {
  // Document point 1
  // enable relais pins
  int pins[] = {FEED12V_SWITCH, LOW_HIGH_COIL_SWITCH, LOW_HIGH_CONTACT_SWITCH1, LOW_HIGH_CONTACT_SWITCH2, LOW_HIGH_CONTACT_SWITCH3};
  setPinsAsOutput(pins, 5);

  // enable leds pins
  pinMode(ORANGE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // enable button pins
  pinMode(START_BUTTON, INPUT);

  // enable analog input
  Serial.begin(9600);

  // turn on the green led to indicate the system is ready
  pinHighAndWait(GREEN_LED);
  draw("Druk start");
}
void setPinsAsOutput(const int pins[], int length) {
  for (int i = 0; i < length; i++) {
    pinMode(pins[i], OUTPUT);          // sets the digital pin as output
    digitalWrite(pins[i], HIGH);       // sets the digital pin on
  }
}

void loop() {
  Serial.println("start");
  if (!readStartButton(START_BUTTON)) {
      Serial.println("Start button not pressed");
      return;
  }
  pinLowAndWait(RED_LED);
  draw("Meting loopt");
// Document point 2
  // oranje led aan
  pinHighAndWait(ORANGE_LED);
// Document point 3
// Document point 3.a
// measure relais 18/19 via A0
  float resistanceOhm18And19 = calculateOhm(SERIAL_PORT_A0);
// measure relais 16/17 via A1
  float resistanceOhm16And17 = calculateOhm(SERIAL_PORT_A1);
// measure relais 12/13 via A2
  float resistanceOhm12And13 = calculateOhm(SERIAL_PORT_A2);
  delay(1000);

// Document point 3.b
// relais 1 HIGH pin 22
  relaisOnAndWait(FEED12V_SWITCH); // K1
  delay(1000);

// Document point 3.c
// measure relais 19/20 via A0
  float resistanceOhm19And20 = calculateOhm(SERIAL_PORT_A0);
// measure relais 14/15 via A1
  float resistanceOhm14And15 = calculateOhm(SERIAL_PORT_A1);
  delay(1000);

// Document point 3.d
  relaisOffAndWait(FEED12V_SWITCH); // K1

// Document point 3.e
  relaisOnAndWait(LOW_HIGH_CONTACT_SWITCH1);
  relaisOnAndWait(LOW_HIGH_CONTACT_SWITCH2);
  relaisOnAndWait(LOW_HIGH_CONTACT_SWITCH3);
  delay(1000);

// Document point 3.f
// measure relais 68/69 via A0
  float resistanceOhm68And69 = calculateOhm(SERIAL_PORT_A0);
// measure relais 66/67 via A1
  float resistanceOhm66And67 = calculateOhm(SERIAL_PORT_A1);
// measure relais 62/63 via A2
  float resistanceOhm62And63 = calculateOhm(SERIAL_PORT_A2);
  delay(1000);

  // Document point 3.g
  relaisOnAndWait(LOW_HIGH_COIL_SWITCH);
  relaisOnAndWait(FEED12V_SWITCH);
  delay(1000);

  // Document point 3.h
// measure relais 69/70 via A0
  float resistanceOhm69And70 = calculateOhm(SERIAL_PORT_A0);
// measure relais 64/65 via A1
  float resistanceOhm64And65 = calculateOhm(SERIAL_PORT_A1);
  delay(1000);

  // Document point 3.h
  relaisOffAndWait(FEED12V_SWITCH);
  relaisOffAndWait(LOW_HIGH_COIL_SWITCH);
  relaisOffAndWait(LOW_HIGH_CONTACT_SWITCH1);
  relaisOffAndWait(LOW_HIGH_CONTACT_SWITCH2);
  relaisOffAndWait(LOW_HIGH_CONTACT_SWITCH3);

  pinLowAndWait(ORANGE_LED);

  float result = resistanceOhm12And13 + resistanceOhm14And15 + resistanceOhm16And17 + resistanceOhm18And19 + resistanceOhm19And20
    + resistanceOhm62And63 + resistanceOhm64And65 + resistanceOhm66And67 + resistanceOhm68And69 + resistanceOhm69And70;

  if (result < MARGIN) {
    // show green led if success
    pinHighAndWait(GREEN_LED);
    draw("Klaar!");
  }
  else {
    pinHighAndWait(RED_LED);
    // calculate tresshold
    // tresshold 10 ohm
    String str = "";
    if (resistanceOhm12And13 > MARGIN) {
      str += "12/13";
    }
    if (resistanceOhm16And17 > MARGIN) {
      if (str != "")
        str += " ";
      str += "16/17";
    }
    if (resistanceOhm18And19 > MARGIN) {
      if (str != "")
        str += " ";
      str += "18/19";
    }
    if (resistanceOhm14And15 > MARGIN) {
      if (str != "")
        str += " ";
      str += "14/15";
    }
    if (resistanceOhm19And20 > MARGIN) {
      if (str != "")
        str += " ";
      str += "19/20";
    }
    if (resistanceOhm62And63 > MARGIN) {
      if (str != "")
        str += " ";
      str += "62/63";
    }
    if (resistanceOhm66And67 > MARGIN) {
      if (str != "")
        str += " ";
      str += "66/67";
    }
    if (resistanceOhm68And69 > MARGIN) {
      if (str != "")
        str += " ";
      str += "68/69";
    }
    if (resistanceOhm64And65 > MARGIN) {
      if (str != "")
        str += " ";
      str += "64/65";
    }
    if (resistanceOhm69And70 > MARGIN) {
        if (str != "")
          str += " ";
        str += "69/70";
    }

    drawTwoLines("Slecht contact", str);
  }
  Serial.println("Done");
  delay(2000);
  Serial.println("end");
}

bool readStartButton(const int startButtonPin) {
  return digitalRead(startButtonPin) == HIGH;
}

void drawTwoLines(String line1, String line2) {
  u8g.firstPage();
  u8g.setFont(u8g_font_helvR08); // font instellen.
  do {
    u8g.drawStr(10, 10, line1.c_str()); // print tekst.
    if (line2.length() > 17) {
      u8g.drawStr(10, 30, line2.substring(0, 16).c_str()); // print tekst. 
      u8g.drawStr(10, 50, line2.substring(17).c_str()); // print tekst. 
    } else {
      u8g.drawStr(10, 30, line2.c_str()); // print tekst.
    }
  } while(u8g.nextPage());
}

void draw(String text) {
  u8g.firstPage();
  u8g.setFont(u8g_font_unifont); // font instellen.
  do {
    u8g.drawStr(10, 32, text.c_str()); // print tekst.
  } while(u8g.nextPage());
}

void relaisOnAndWait(const int pin) {
  pinLowAndWait(pin);
}

void pinHighAndWait(const int pin) {
  digitalWrite(pin, HIGH);       // sets the digital pin on
  delay(500);
}

void relaisOffAndWait(const int pin) {
  pinHighAndWait(pin);
}

void pinLowAndWait(const int pin) {
  digitalWrite(pin, LOW);       // sets the digital pin off
  delay(500);
}

float calculateOhm(const int analogPin) {
  int rawValue = analogRead(analogPin);
  Serial.println(rawValue);
  if (rawValue) {
    float buffer = rawValue * V_IN;
    int vOut = buffer / ANALOG_SCALE;
    buffer = (V_IN / vOut) - FRACTION_RESULT_FACTOR;
    return (RESISTOR_REFERENCE * buffer);
  }
  return 0.0;
}

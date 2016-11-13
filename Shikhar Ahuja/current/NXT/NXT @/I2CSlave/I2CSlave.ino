#include <Wire.h>
#define I2CDebug
volatile uint8_t in[17];
volatile uint8_t out2[17];

const int pingPin = 7; //Left
const int pingPin2 = 8; //Middle
const int pingPin3 = 9; //Right
const int pingPin4 = 10; //Left Back

uint8_t SensorVersion[9] = "1.01    ";
uint8_t SensorName[9] = "arduino ";
uint8_t SensorType[9] = "micro   ";

volatile float ping1 = 0, ping2 = 0, ping3 = 0, ping4 = 0;

void setup() {
  Serial.begin(9600);

  Wire.begin(0x08 >> 1);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  delay(2000);
  digitalWrite(2, LOW);
}

void loop() {
  float duration;

  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
  ping1 = duration / 29 / 2;

  pinMode(pingPin2, OUTPUT);
  digitalWrite(pingPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin2, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin2, LOW);

  pinMode(pingPin2, INPUT);
  duration = pulseIn(pingPin2, HIGH);
  ping2 = duration / 29 / 2;

  pinMode(pingPin3, OUTPUT);
  digitalWrite(pingPin3, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin3, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin3, LOW);

  pinMode(pingPin3, INPUT);
  duration = pulseIn(pingPin3, HIGH);

  ping3 = duration / 29 / 2;

  pinMode(pingPin4, OUTPUT);
  digitalWrite(pingPin4, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin4, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin4, LOW);

  pinMode(pingPin4, INPUT);
  duration = pulseIn(pingPin4, HIGH);

  ping4 = duration / 29 / 2;

#ifdef PINGDebug
  Serial.print("Sensor 1: ");
  Serial.print(ping1);
  Serial.print("cm");
  Serial.println();

  Serial.print("Sensor 2: ");
  Serial.print(ping2);
  Serial.print("cm");
  Serial.println();

  Serial.print("Sensor 3: ");
  Serial.print(ping3);
  Serial.print("cm");
  Serial.println();

  Serial.print("Sensor 4: ");
  Serial.print(ping4);
  Serial.print("cm");
  Serial.println();
#endif

#ifdef I2CDebug
  Serial.println("Command: " + in[0]);
//  Serial.print("HEX VALUES: ");
//  Serial.print(out2[0], HEX);
//  Serial.print(" ");
//  Serial.print(out2[1], HEX);
//  Serial.print(" ");
//  Serial.print(out2[2], HEX);
//  Serial.print(" ");
//  Serial.print(out2[3], HEX);
//  Serial.println();
#endif
  delay(100);
}

void receiveEvent(int bytes) {
  for (int i = 0; Wire.available() > 0; i++) {
    in[i] = Wire.read();
  }
}

void requestEvent() {
  byte* ptr;
  uint8_t out[17];
  switch (in[0]) {
    case 0x00:
      Wire.write(SensorVersion, 8);
      return;
      break;
    case 0x08:
      Wire.write(SensorName, 8);
      return;
      break;
    case 0x10:
      Wire.write(SensorType, 8);
      return;
      break;
    case 0x42:
      out[0] = 0x22;
      out[1] = 0x23;
      out[2] = 0x24;
      out[3] = 0x25;
      break;
    case 0x43:
      ptr = (byte*)&ping1;
      out[0] = ptr[0];
      out[1] = ptr[1];
      out[2] = ptr[2];
      out[3] = ptr[3];
      break;
    case 0x44:
      ptr = (byte*)&ping2;
      out[0] = ptr[0];
      out[1] = ptr[1];
      out[2] = ptr[2];
      out[3] = ptr[3];
      break;
    case 0x45:
      ptr = (byte*)&ping3;
      out[0] = ptr[0];
      out[1] = ptr[1];
      out[2] = ptr[2];
      out[3] = ptr[3];
      break;
    case 0x46:
      ptr = (byte*)&ping4;
      out[0] = ptr[0];
      out[1] = ptr[1];
      out[2] = ptr[2];
      out[3] = ptr[3];
      break;
  }
  Wire.write(out, 4);
}


// Experiment 9 (Final): Anti-Disconnect Bluetooth Control with State Tracking & Coast-Stop
// Target Board: ESP32 30-Pin NodeMCU

#include "BluetoothSerial.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

BluetoothSerial SerialBT;

// --- DRIVER 1 (LEFT SIDE) ---
const int L_PWMA = 15;
const int L_AIN1 = 2;  // Front-Left Direction 1
const int L_AIN2 = 4;  // Front-Left Direction 2

const int L_PWMB = 22;
const int L_BIN1 = 19; // Rear-Left Direction 1
const int L_BIN2 = 21; // Rear-Left Direction 2

// --- DRIVER 2 (RIGHT SIDE) ---
const int R_PWMA = 25;
const int R_AIN1 = 26; // Front-Right Direction 1
const int R_AIN2 = 27; // Front-Right Direction 2

const int R_PWMB = 14;
const int R_BIN1 = 12; // Rear-Right Direction 1
const int R_BIN2 = 13; // Rear-Right Direction 2

const int TARGET_SPEED = 240;

// Remembers what the car is currently doing so holding a button doesn't re-run the ramp loop
char currentState = 'S';

void rampUpMotors() {
  for (int spd = 110; spd <= TARGET_SPEED; spd += 40) {
    analogWrite(L_PWMA, spd);
    analogWrite(L_PWMB, spd);
    analogWrite(R_PWMA, spd);
    analogWrite(R_PWMB, spd);
    delay(15);
  }
  analogWrite(L_PWMA, TARGET_SPEED);
  analogWrite(L_PWMB, TARGET_SPEED);
  analogWrite(R_PWMA, TARGET_SPEED);
  analogWrite(R_PWMB, TARGET_SPEED);
}

// COAST STOP: Set direction pins LOW first so motors coast smoothly without a Back-EMF spike
void stopCar() {
  digitalWrite(L_AIN1, LOW); digitalWrite(L_AIN2, LOW);
  digitalWrite(L_BIN1, LOW); digitalWrite(L_BIN2, LOW);
  digitalWrite(R_AIN1, LOW); digitalWrite(R_AIN2, LOW);
  digitalWrite(R_BIN1, LOW); digitalWrite(R_BIN2, LOW);
  delay(10); // Let H-bridges enter High-Impedance Coast mode first
  analogWrite(L_PWMA, 0);
  analogWrite(L_PWMB, 0);
  analogWrite(R_PWMA, 0);
  analogWrite(R_PWMB, 0);
}

void moveForward() {
  stopCar();
  delay(15);
  digitalWrite(L_AIN1, HIGH); digitalWrite(L_AIN2, LOW);
  digitalWrite(L_BIN1, HIGH); digitalWrite(L_BIN2, LOW);
  digitalWrite(R_AIN1, HIGH); digitalWrite(R_AIN2, LOW);
  digitalWrite(R_BIN1, HIGH); digitalWrite(R_BIN2, LOW);
  rampUpMotors();
}

void moveBackward() {
  stopCar();
  delay(15);
  digitalWrite(L_AIN1, LOW); digitalWrite(L_AIN2, HIGH);
  digitalWrite(L_BIN1, LOW); digitalWrite(L_BIN2, HIGH);
  digitalWrite(R_AIN1, LOW); digitalWrite(R_AIN2, HIGH);
  digitalWrite(R_BIN1, LOW); digitalWrite(R_BIN2, HIGH);
  rampUpMotors();
}

void turnLeft() {
  stopCar();
  delay(15);
  digitalWrite(L_AIN1, LOW);  digitalWrite(L_AIN2, HIGH);
  digitalWrite(L_BIN1, LOW);  digitalWrite(L_BIN2, HIGH);
  digitalWrite(R_AIN1, HIGH); digitalWrite(R_AIN2, LOW);
  digitalWrite(R_BIN1, HIGH); digitalWrite(R_BIN2, LOW);
  rampUpMotors();
}

void turnRight() {
  stopCar();
  delay(15);
  digitalWrite(L_AIN1, HIGH); digitalWrite(L_AIN2, LOW);
  digitalWrite(L_BIN1, HIGH); digitalWrite(L_BIN2, LOW);
  digitalWrite(R_AIN1, LOW);  digitalWrite(R_AIN2, HIGH);
  digitalWrite(R_BIN1, LOW);  digitalWrite(R_BIN2, HIGH);
  rampUpMotors();
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  Serial.begin(115200);

  int pins[] = {L_PWMA, L_AIN1, L_AIN2, L_PWMB, L_BIN1, L_BIN2,
                R_PWMA, R_AIN1, R_AIN2, R_PWMB, R_BIN1, R_BIN2};
  for (int i = 0; i < 12; i++) {
    pinMode(pins[i], OUTPUT);
  }

  stopCar();

  SerialBT.begin("ESP32_Smart_Car");
  Serial.println("Bluetooth Ready!");
}

void loop() {
  if (SerialBT.available()) {
    char cmd = SerialBT.read();

    // Convert lowercase to uppercase
    if (cmd >= 'a' && cmd <= 'z') {
      cmd = cmd - 32;
    }

    // Ignore newline/space characters or repeated commands while a button is held down
    if (cmd != 'F' && cmd != 'B' && cmd != 'L' && cmd != 'R' && cmd != 'S') {
      return;
    }
    if (cmd == currentState) {
      return; // Already doing this action! Do not re-trigger the motors.
    }

    currentState = cmd;

    if (cmd == 'F') {
      moveForward();
    }
    else if (cmd == 'B') {
      moveBackward();
    }
    else if (cmd == 'L') {
      turnLeft();
    }
    else if (cmd == 'R') {
      turnRight();
    }
    else if (cmd == 'S') {
      stopCar();
    }
  }
}
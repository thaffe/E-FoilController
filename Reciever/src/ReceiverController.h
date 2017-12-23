#include <Arduino.h>
#include <Servo.h>
// #include <SoftwareSerial.h> // RX, TX

class ReceiverController
{
public:
  ReceiverController();
  void setup();
  void loop();

  void updatePowerStatus();
  void updateMotorOuput();
  int getMotorStep();

  void readBluetooth();
  void sendBatteryStatus();

private:
  // PIN SETUP
  const int motorOutputPin = 12;
  const int motorPowerPin = 9;
  const int watercoolerPowerPin = 7;
  const int voltagePin = 5;

  const int bluetoothTxPin = 2;
  const int bluetoothRxPin = 3;
  // SoftwareSerial BTSerial = SoftwareSerial(2, 3); // RX, TX

  // MOTOR PARAMETERS
  const int minMotorOutput = 1500;
  const int maxMotorOutput = 1861;
  const int timeFromMinToMax = 1000;
  const int killMotorMultiplyer = 2;
  const int motorRangeOutput = maxMotorOutput - minMotorOutput;
  volatile int motorOutput = minMotorOutput;
  volatile bool motorPowerStatus = false;
  Servo motorPowerServo;

  // BATTERY PARAMETERS
  const int maxVoltage = 620;
  const int minVoltage = 860;
  const int sendBatteryStatusEvery = 10000;
  unsigned int lastSentBatteryStatus = 0;

  // BT PARAMETERS
  volatile uint8_t lastBtRead = 0;
  int btReadingTimeout = 250;
  int btPowerTimeout = 2000;
  unsigned long timeLastBtReading = 0; //Set to timeout in begining
  unsigned long timeSinceLastBtReading = btPowerTimeout;

  unsigned long lastChange = 0;
  unsigned int timeout = 5;
  void debug();
};

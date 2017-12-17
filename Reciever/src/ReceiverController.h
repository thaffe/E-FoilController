#include <Arduino.h>
#include <Servo.h>
#include <SoftwareSerial.h> // RX, TX

class ReceiverController
{
  public:
    ReceiverController();
    void setup();
    void loop();

    void readBluetooth();
    void updatePowerStatus();
    void updateMotorOuput();

    int getMotorStep();

  private:
    // PIN SETUP
    const int motorOutputPin = 9;
    const int motorPowerPin = 8;
    const int watercoolerPowerPin = 7;
    
    const int bluetoothTxPin = 10;
    const int bluetoothRxPin = 11;
    SoftwareSerial BTSerial = SoftwareSerial(bluetoothRxPin, bluetoothTxPin); // RX, TX

    // MOTOR PARAMETERS
    const int minMotorOutput = 0;
    const int maxMotorOutput = 174;
    const int timeFromMinToMax = 1000;
    const int motorRangeOutput = maxMotorOutput - minMotorOutput;
    volatile int motorOutput = minMotorOutput;
    volatile bool motorPowerStatus = false;
    Servo motorPowerServo;

    // BT PARAMETERS
    volatile uint8_t lastBtRead = 0;
    int btReadingTimeout = 250;
    int btPowerTimeout = 2000;
    int timeLastBtReading = -btPowerTimeout; //Set to timeout in begining
    int timeSinceLastBtReading = btPowerTimeout;

    unsigned long lastChange = 0;
    unsigned int timeout = 5;
    void debug();
};

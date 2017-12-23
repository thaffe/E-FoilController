#include <Arduino.h>
#include <SoftwareSerial.h>

#define AVG_READ_COUNT 20

class TransmitterController
{
public:
  TransmitterController();
  void setup();
  void loop();

  void updateThrottleReading();
  float getAverage();

private:
  // PINS
  const int deadMansButtonPin = 4;
  const int throttlePin = 1; //Analog
  const int bluetoothTxPin = 10;
  const int bluetoothRxPin = 11;

  // PARAMETERS
  const float throttleMin = 100;
  const float throttleMax = 770;
  const float throttleRange = throttleMax - throttleMin;
  const int averageReadsCount = 20; // Number of reads to average over
  const int updateEveryMs = 10;     // Update throttle reading ever given ms
  const int readingsPrSend = 10;    // Send throttle reading over bluetooth after given throttle reads (throttle sent is averaged over readings)

  // DONT TOUCH
  int readPointer = 0;
  int readCount = 0;
  float lastReadings[AVG_READ_COUNT];
  uint8_t throttleReading = 0;

  // SoftwareSerial BTSerial = SoftwareSerial(bluetoothRxPin, bluetoothTxPin);
};

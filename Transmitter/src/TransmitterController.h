#include <Arduino.h>
#include <SoftwareSerial.h>

class TransmitterController
{
public:
    TransmitterController();
    void setup();
    void loop();

private:
    const int deadMansButtonPin = 4;
    const int throttlePin = 1; //Analog
    const int bluetoothTxPin = 10;
    const int bluetoothRxPin = 11;

    uint8_t throttleReading = 0;
    void SendThrottleReading(uint8_t reading);
    SoftwareSerial BTSerial = SoftwareSerial(bluetoothTxPin, bluetoothRxPin);
};

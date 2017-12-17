#include "TransmitterController.h"
TransmitterController transmitter;

void setup() {
  Serial.begin(9600);
  transmitter.setup();
}

void loop() {
  transmitter.loop();
}

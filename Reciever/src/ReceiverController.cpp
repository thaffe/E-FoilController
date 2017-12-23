#include "ReceiverController.h"

ReceiverController::ReceiverController() {}

void ReceiverController::setup()
{
    // Set all IO
    pinMode(motorPowerPin, OUTPUT);
    digitalWrite(motorPowerPin, LOW);

    pinMode(watercoolerPowerPin, OUTPUT);
    digitalWrite(watercoolerPowerPin, LOW);

    motorPowerServo.attach(motorOutputPin);
    motorPowerServo.write(minMotorOutput);

    //Start Bluetooth communication
    // BTSerial.begin(9600);
    Serial.begin(9600);
    delay(btPowerTimeout * 2);
}

void ReceiverController::loop()
{

    readBluetooth();
    updatePowerStatus();
    updateMotorOuput();
    // sendBatteryStatus();

    //debug();
    lastChange = millis();
    delay(timeout);
}

void ReceiverController::debug()
{
    Serial.print("Speed: ");
    Serial.print(motorOutput);
    Serial.print(" :: ");

    Serial.print("Power: ");
    Serial.print(motorPowerStatus);
    Serial.print(" :: ");

    Serial.print("lastBtRead: ");
    Serial.print(lastBtRead);

    Serial.println("");
}

void ReceiverController::readBluetooth()
{
    //Serial.println(BTSerial.available());
    if (Serial.available() > 0)
    {
        lastBtRead = Serial.read();

        // Fush other
        while (Serial.available() > 0)
            Serial.read();

        timeLastBtReading = millis();
        timeSinceLastBtReading = 0;
    }
    else
    {
        timeSinceLastBtReading = millis() - timeLastBtReading;
    }

    if (timeSinceLastBtReading > btReadingTimeout)
        lastBtRead = 0;
}

void ReceiverController::updatePowerStatus()
{
    motorPowerStatus = timeSinceLastBtReading < btPowerTimeout;

    digitalWrite(motorPowerPin, motorPowerStatus ? HIGH : LOW);
    digitalWrite(watercoolerPowerPin, motorPowerStatus ? HIGH : LOW);
}

void ReceiverController::updateMotorOuput()
{
    int step = getMotorStep();
    int newOutput = (int)(lastBtRead / 255.0 * motorRangeOutput + minMotorOutput);
    if (!motorPowerStatus)
    {
        motorOutput = minMotorOutput;
    }
    else if (newOutput > motorOutput)
    {
        motorOutput = min(min(motorOutput + step, newOutput), maxMotorOutput);
    }
    else if (newOutput < motorOutput)
    {
        motorOutput = max(max(motorOutput - step, newOutput), minMotorOutput);
    }
    // Serial.println(motorOutput);
    motorPowerServo.write(motorOutput);
}

int ReceiverController::getMotorStep()
{

    float step = ceilf(1.0 * (millis() - lastChange) / timeFromMinToMax * motorRangeOutput);
    // IF 0 speed then no dead mans button or gas released then double stepsize
    if (lastBtRead == 0)
        return step * killMotorMultiplyer;

    return step;
}

void ReceiverController::sendBatteryStatus()
{

    if (lastSentBatteryStatus < millis())
    {
        lastSentBatteryStatus = millis() + sendBatteryStatusEvery;
        float percent = (analogRead(voltagePin) - minVoltage) / (maxVoltage - minVoltage);
        uint8_t value = max(0, min(256, percent * 256));
        Serial.write(value);
    }
}
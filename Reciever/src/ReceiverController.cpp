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
    BTSerial.begin(9600);
    Serial.begin(9600);
}

void ReceiverController::loop()
{
    readBluetooth();
    updatePowerStatus();
    updateMotorOuput();

    //  debug();
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
    if (BTSerial.available() > 0)
    {
        lastBtRead = BTSerial.read();
        // Fush other
        while (BTSerial.available() > 0)
            BTSerial.read();
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
        return step * 2;

    return step;
}
#include <Arduino.h>
#include <NimBLEDevice.h>

#define MOTOR_PIN 2

// UUIDs must match your MAUI app
#define SERVICE_UUID      "00001111-0000-1000-8000-00805F9B34FB"
#define WRITE_CHAR_UUID   "00004444-0000-1000-8000-00805F9B34FB"

NimBLECharacteristic* pWriteCharacteristic;

class WriteCallbacks : public NimBLECharacteristicCallbacks
{
    void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) override
    {
        std::string value = pCharacteristic->getValue();

        Serial.print("Received: ");
        Serial.println(value.c_str());

        if (value == "VIBRATE")
        {
            digitalWrite(MOTOR_PIN, HIGH);

            delay(500);

            digitalWrite(MOTOR_PIN, LOW);
        }
    }
};

void setup()
{
    Serial.begin(115200);

    pinMode(MOTOR_PIN, OUTPUT);
    digitalWrite(MOTOR_PIN, LOW);

    NimBLEDevice::init("XIAOBLE");

    NimBLEServer* pServer =
        NimBLEDevice::createServer();

    NimBLEService* pService =
        pServer->createService(SERVICE_UUID);

    pWriteCharacteristic =
        pService->createCharacteristic(
            WRITE_CHAR_UUID,
            NIMBLE_PROPERTY::WRITE
        );

    pWriteCharacteristic->setCallbacks(
        new WriteCallbacks());

    pService->start();

    NimBLEAdvertising* advertising =
        NimBLEDevice::getAdvertising();

    advertising->addServiceUUID(SERVICE_UUID);

    advertising->start();

    Serial.println("XIAOBLE advertising...");
}

void loop()
{
    delay(100);
}
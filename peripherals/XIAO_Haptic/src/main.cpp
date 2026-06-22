#include <Arduino.h>
#include <bluefruit.h>
#include <Wire.h>
#include <Adafruit_DRV2605.h>

Adafruit_DRV2605 drv;

#define SERVICE_UUID "00001111-0000-1000-8000-00805F9B34FB"
#define WRITE_UUID   "00004444-0000-1000-8000-00805F9B34FB"

#define RED_LED LED_RED
// BLE Service
BLEService hapticService(SERVICE_UUID);

// BLE Characteristic
BLECharacteristic writeCharacteristic(WRITE_UUID);

void write_callback(uint16_t conn_hdl,
                    BLECharacteristic* chr,
                    uint8_t* data,
                    uint16_t len)
{
    String command = "";

    for (int i = 0; i < len; i++)
    {
        command += (char)data[i];
    }

    Serial.print("Received: ");
    Serial.println(command);

    if (command == "VIBRATE")
    {
        for (int i = 0; i < 3; i++)
        {
            digitalWrite(LED_RED, LOW);   // ON
            delay(200);

            digitalWrite(LED_RED, HIGH);  // OFF
            delay(200);
        }
    }
}

void setup()
{
    Serial.begin(115200);

    Wire.begin();

    drv.begin();

    drv.selectLibrary(1);

    drv.setMode(DRV2605_MODE_INTTRIG);

    Bluefruit.begin();

    Bluefruit.setTxPower(4);

    Bluefruit.setName("XIAOBLE");

    hapticService.begin();

    writeCharacteristic.setProperties(CHR_PROPS_WRITE);

    writeCharacteristic.setPermission(SECMODE_OPEN, SECMODE_OPEN);

    writeCharacteristic.setFixedLen(20);

    writeCharacteristic.setWriteCallback(write_callback);

    writeCharacteristic.begin();

    Bluefruit.Advertising.addService(hapticService);

    Bluefruit.ScanResponse.addName();

    Bluefruit.Advertising.restartOnDisconnect(true);

    Bluefruit.Advertising.setInterval(32, 244);

    Bluefruit.Advertising.setFastTimeout(30);

    Bluefruit.Advertising.start(0);

    Serial.println("XIAO BLE Ready");
}

void loop()
{
}
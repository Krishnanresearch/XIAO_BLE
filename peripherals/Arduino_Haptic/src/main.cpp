#include <Arduino.h>
#include <ArduinoBLE.h>


bool CONNECTED = false;

BLEService appService(
  "00001111-0000-1000-8000-00805F9B34FB");

BLEStringCharacteristic writeChar(
  "00002222-0000-1000-8000-00805F9B34FB",
  BLEWrite,
  50);

BLEStringCharacteristic notifyChar(
  "00003333-0000-1000-8000-00805F9B34FB",
  BLENotify,
  50);
void blePeripheralConnectHandler(BLEDevice central) {
  // central connected event handler
  CONNECTED = true;
 Serial.print("Connected to central: ");
 Serial.println(central.address());

}

void blePeripheralDisconnectHandler(BLEDevice central) {
  // central disconnected event handler
   CONNECTED = false;
 Serial.print("Disconnected from central: ");
 Serial.println(central.address());
}

void setup() {
   Serial.begin(9600);

  while (!Serial);

  Serial.println("Start of the code");

  if (!BLE.begin())
  {
    while (1);
  }

  BLE.setLocalName("ArduinoBLE");

  BLE.setAdvertisedService(appService);

  appService.addCharacteristic(writeChar);
  appService.addCharacteristic(notifyChar);

  BLE.addService(appService);
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  BLE.advertise();
}

void loop() {

  BLE.poll();
  
    while (CONNECTED)
    {
      if (writeChar.written())
      {
        String command = writeChar.value();

        Serial.println(command);

        if (command == "START")
        {
          delay(2000);

          String result = "DONE";

          notifyChar.writeValue(result);
        }
      }
    }// put your main code here, to run repeatedly:
}


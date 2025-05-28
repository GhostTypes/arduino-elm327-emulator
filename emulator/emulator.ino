#include <ArduinoBLE.h>
#include "Arduino_LED_Matrix.h"
#include <map>

// Emulate a VGate Linker MC+
BLEService obdService("0000FFE0-0000-1000-8000-00805F9B34FB");
BLECharacteristic rwCharacteristic("0000FFE1-0000-1000-8000-00805F9B34FB", BLERead | BLEWriteWithoutResponse | BLENotify, 64);

// Multi-line responses
const String dtcResponse = "43 02 00 36 00 54\r\n43 00"; // P0036, P0054
const String vinResponse = "REPLACE_ME"; // Log your vin with a real ELM327 and store the response here.

// Pid request/response map
const std::map<String, String> pidResponses = {
  {"0100", "41 00 BE 3F F8 13\r\n41 00 80 00 00 01"}, // PIDs supported (01 to 20)
  {"0101", "41 01 82 07 6D 25"}, // Monitor status since DTCs cleared
  {"0105", "41 05 7B"},       // Engine coolant temperature
  {"010C", "41 0C 1A F8"},     // Engine RPM
  {"010D", "41 0D 5A"},       // Vehicle speed
  {"010F", "41 0F 2A"}        // Intake air temperature
};

ArduinoLEDMatrix matrix;

void setup() {
  matrix.begin();
  matrix.loadFrame(LEDMATRIX_BOOTLOADER_ON);
  Serial.begin(9600);
  while (!Serial);

  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    showError();
    while (1);
  }

  // Set the BLE device name
  BLE.setLocalName("ELM327 Emulator");
  BLE.setAdvertisedService(obdService);

  // Add characteristic to the service
  obdService.addCharacteristic(rwCharacteristic);

  // Add service to BLE
  BLE.addService(obdService);

  // Start advertising
  BLE.advertise(); 
  showLoadAnim();

  Serial.println("Advertising...");
}

// Show "loading" animation when waiting for connection
void showLoadAnim() {
  matrix.loadSequence(LEDMATRIX_ANIMATION_LOAD);
  matrix.play(true);
}

// Show "processing" animation for command interaction
void showProcAnim() {
  matrix.loadSequence(LEDMATRIX_ANIMATION_BOUNCING_BALL);
  matrix.play(true);
}

// Show bluetooth "icon" when in idle state
void showIdle() {
  matrix.loadFrame(LEDMATRIX_CHIP);
}

// Show error "icon"
void showError() {
  matrix.loadFrame(LEDMATRIX_EMOJI_SAD);
}

bool dtcsActive = true;
void sendDTCs() {
  if (!dtcsActive) {
    Serial.println("Simulating empty DTC response");
    sendResponse("43 00");
    return;
  }
  Serial.println("Simulating DTC codes");
  sendResponse(dtcResponse);
}

void clearDTCs() {
  dtcsActive = false;
  delay(100); // simulate some kind of "work"
  Serial.println("DTCs cleared.");
  sendResponse("OK");
}

void simulateProtocolMatching() {
  Serial.println("Simulating auto protocol matching");
  sendRaw("SEARCHING");
  delay(200);
  sendRaw(".");
  delay(200);
  sendRaw(".");
  delay(200);
  sendRaw(".");
  delay(200);
  sendResponse("0");
}

void failProtocolMatch() {
  sendResponse("UNABLE TO CONNECT");
}

void sendRaw(String cmd) {
  rwCharacteristic.writeValue(cmd.c_str());
}

void sendResponse(String cmd) {
  rwCharacteristic.writeValue(cmd.c_str());
  sendResponseComplete();
  Serial.print("Sent response: ");
  Serial.println(cmd);
}

void sendResponseComplete() {
  rwCharacteristic.writeValue("\r");
  rwCharacteristic.writeValue(">");
  showIdle(); // stop proc animation
}

void processATcommand(String cmd) {
  String response = "";
  if (cmd == "ATZ") {
    delay(250); // simulate reset delay
    dtcsActive = true; // reset DTC status
    response = "ELM327 v2.2";
  } else if (cmd == "ATSP0") {
    simulateProtocolMatching();
    return;
  } else {
    response = "OK";
  }
  sendResponse(response);
}

void processSTcommand(String cmd) {
  String response = "OK";
  sendResponse(response);
}

void sendVIN() {
  Serial.println("Simulating VIN response");
  sendRaw(vinResponse);
  delay(10);
  sendResponseComplete();
}

void processPid(String pid) {
  auto it = pidResponses.find(pid); // Search for the PID in the map

  if (it != pidResponses.end()) {
    sendResponse(it->second); // Send the corresponding response if PID is known
  } else {
    sendResponse("NO DATA"); // Send "NO DATA" for unknown PIDs
  }
}


void loop() {
  // Listen for BLE central devices to connect
  BLEDevice central = BLE.central();

  // If a central device connects
  if (central) {
    Serial.print("Device connected: ");
    Serial.println(central.address());
    showIdle();

    while (central.connected()) {
      // Poll the BLE stack
      BLE.poll();

      // Handle written characteristics from central
      if (rwCharacteristic.written()) {
        showProcAnim();
        // Retrieve the value written to the characteristic
        String cmd = "";

        int length = rwCharacteristic.valueLength();
        for (int i = 0; i < length; i++) {
          cmd += (char) rwCharacteristic.value()[i];
        }

        // Remove any leading/trailing whitespace and terminators
        cmd.trim();

        Serial.print("Received command: ");
        Serial.println(cmd);

        // AT (ELM327) Commands
        if (cmd.indexOf("AT") == 0) {
          processATcommand(cmd);
          continue;
        }

        // ST Commands (ELM327 Successor)
        if (cmd.indexOf("ST") == 0) {
          processSTcommand(cmd);
          continue;
        }

        // Generic PID Request
        if (pidResponses.find(cmd) != pidResponses.end()) {
          processPid(cmd);
          continue;
        }

        // DTC Request
        if (cmd == "03") {
          sendDTCs();
          continue;
        }

        // Clear DTC codes
        if (cmd == "04") {
          clearDTCs();
          continue;
        }

        // VIN Request
        if (cmd == "0902") {
          sendVIN();
          continue;
        }

        // Unknown command
        sendResponse("?");
      }
    }

    Serial.print("Device disconnected: ");
    Serial.println(central.address());
    showLoadAnim();
    // reset states on disconnect
    dtcsActive = true;

  }
}  
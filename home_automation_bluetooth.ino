#include <AceButton.h>
using namespace ace_button;
#include "BluetoothSerial.h" 

// Define the GPIO pins connected to relays and switches
#define RelayPin1 23  // D23
#define RelayPin2 22  // D22

#define SwitchPin1 13  // D13
#define SwitchPin2 12  // D12

// Define initial relay states (1 means off for LOW-level triggered)
int toggleState_1 = 1;
int toggleState_2 = 1;

BluetoothSerial SerialBT;

// Initialize button configurations
ButtonConfig config1;
AceButton button1(&config1);
ButtonConfig config2;
AceButton button2(&config2);

// Function prototypes for button event handling
void button1Handler(AceButton*, uint8_t, uint8_t);
void button2Handler(AceButton*, uint8_t, uint8_t);

// Function prototypes for relay control
void BluetoothHandle();

void setup() {
    Serial.begin(9600);

    // Initialize Bluetooth
    SerialBT.begin("ESP32_BT");
    Serial.println("Bluetooth started. Pair with ESP32_BT.");

    // Set up relay pins
    pinMode(RelayPin1, OUTPUT);
    pinMode(RelayPin2, OUTPUT);
    digitalWrite(RelayPin1, toggleState_1); // Set initial state for Relay 1
    digitalWrite(RelayPin2, toggleState_2); // Set initial state for Relay 2

    // Initialize buttons
    config1.setEventHandler(button1Handler);
    config2.setEventHandler(button2Handler);
    button1.init(SwitchPin1);
    button2.init(SwitchPin2);
}

void loop() {
    // Check Bluetooth availability and handle incoming data
    if (SerialBT.available()) {
        BluetoothHandle();
    }

    // Check button events
    button1.check();
    button2.check();
}

// Handle Bluetooth data to control relays
void BluetoothHandle() {
    char bt_data = SerialBT.read();
    
    switch (bt_data) {
        case 'A':
            // Turn on Relay 1
            toggleState_1 = 0;
            digitalWrite(RelayPin1, LOW);
            break;
        case 'a':
            // Turn off Relay 1
            toggleState_1 = 1;
            digitalWrite(RelayPin1, HIGH);
            break;
        case 'B':
            // Turn on Relay 2
            toggleState_2 = 0;
            digitalWrite(RelayPin2, LOW);
            break;
        case 'b':
            // Turn off Relay 2
            toggleState_2 = 1;
            digitalWrite(RelayPin2, HIGH);
            break;
        default:
            // Unrecognized data, ignore
            break;
    }
}

// Button event handlers
void button1Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
    if (eventType == AceButton::kEventPressed) {
        // Toggle Relay 1 on button press
        toggleState_1 = !toggleState_1;
        digitalWrite(RelayPin1, toggleState_1 ? HIGH : LOW);
    }
}

void button2Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
    if (eventType == AceButton::kEventPressed) {
        // Toggle Relay 2 on button press
        toggleState_2 = !toggleState_2;
        digitalWrite(RelayPin2, toggleState_2 ? HIGH : LOW);
    }
}

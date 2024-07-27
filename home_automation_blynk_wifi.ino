#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <AceButton.h>
using namespace ace_button;

BlynkTimer timer;

// Define the GPIO pins connected to relays
#define RelayPin1 23  // D23
#define RelayPin2 22  // D22

// Define virtual pins for Blynk buttons
#define VPIN_BUTTON_1 V1
#define VPIN_BUTTON_2 V2

// Define relay toggle states
int toggleState_1 = HIGH; // Initial state for relay 1 (HIGH means off for low-level triggered relays)
int toggleState_2 = HIGH; // Initial state for relay 2 (HIGH means off for low-level triggered relays)

// Define button pins and AceButton objects
#define SwitchPin1 13  // D13
#define SwitchPin2 12  // D12
ButtonConfig config1;
AceButton button1(&config1);
ButtonConfig config2;
AceButton button2(&config2);

// WiFi and Blynk credentials
#define BLYNK_TEMPLATE_ID "TMPL3cvL0wvd4"
#define BLYNK_TEMPLATE_NAME "ESP32 Home Automation AllInOne"
#define BLYNK_AUTH_TOKEN "-tAvhufnChh-QXBtKJssOU0zFEXRhFmX"
#define WIFI_SSID "Muthukumar" // Your WiFi network name
#define WIFI_PASS "9791138148" // Your WiFi network password

// Blynk write functions to control the relays via the Blynk app
BLYNK_WRITE(VPIN_BUTTON_1) {
    toggleState_1 = param.asInt();
    digitalWrite(RelayPin1, !toggleState_1); // Invert state for low-level triggered relays
}

BLYNK_WRITE(VPIN_BUTTON_2) {
    toggleState_2 = param.asInt();
    digitalWrite(RelayPin2, !toggleState_2); // Invert state for low-level triggered relays
}

// Blynk connection setup and synchronization
BLYNK_CONNECTED() {
    Blynk.syncVirtual(VPIN_BUTTON_1);
    Blynk.syncVirtual(VPIN_BUTTON_2);
}

// Button event handlers for physical buttons
void button1Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
    switch (eventType) {
        case AceButton::kEventPressed:
            digitalWrite(RelayPin1, LOW); // Relay on (LOW) for low-level triggered relay
            toggleState_1 = LOW;
            Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
            break;
        case AceButton::kEventReleased:
            digitalWrite(RelayPin1, HIGH); // Relay off (HIGH) for low-level triggered relay
            toggleState_1 = HIGH;
            Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
            break;
    }
}

void button2Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
    switch (eventType) {
        case AceButton::kEventPressed:
            digitalWrite(RelayPin2, LOW); // Relay on (LOW) for low-level triggered relay
            toggleState_2 = LOW;
            Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
            break;
        case AceButton::kEventReleased:
            digitalWrite(RelayPin2, HIGH); // Relay off (HIGH) for low-level triggered relay
            toggleState_2 = HIGH;
            Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
            break;
    }
}

void setup() {
    Serial.begin(9600);

    // Initialize relay pins as outputs
    pinMode(RelayPin1, OUTPUT);
    pinMode(RelayPin2, OUTPUT);
    
    // Set initial states (off)
    digitalWrite(RelayPin1, HIGH); // Relays are low-level triggered, so HIGH turns them off
    digitalWrite(RelayPin2, HIGH); // Relays are low-level triggered, so HIGH turns them off

    // Initialize buttons with AceButton
    config1.setEventHandler(button1Handler);
    config2.setEventHandler(button2Handler);
    button1.init(SwitchPin1);
    button2.init(SwitchPin2);

    // Connect to WiFi and configure Blynk
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Blynk.config(BLYNK_AUTH_TOKEN);
}

void loop() {
    // Run Blynk and AceButton check
    Blynk.run();
    button1.check();
    button2.check();
}

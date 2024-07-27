#include <WiFi.h>
#include <Espalexa.h>

Espalexa espalexa;

#define RELAY_PIN1 23 // D23
#define RELAY_PIN2 22 // D22

// WiFi settings
#define WIFI_SSID "Muthukumar"
#define WIFI_PASSWORD "9791138148"

// Device names for Alexa
String device1Name = "Fan";
String device2Name = "Light";

// Alexa control functions
void controlDevice1(uint8_t brightness) {
    digitalWrite(RELAY_PIN1, brightness == 255 ? LOW : HIGH);
}

void controlDevice2(uint8_t brightness) {
    digitalWrite(RELAY_PIN2, brightness == 255 ? LOW : HIGH);
}

void setup() {
    Serial.begin(9600);
    
    // Set relay pins as outputs and initialize them as off (HIGH)
    pinMode(RELAY_PIN1, OUTPUT);
    pinMode(RELAY_PIN2, OUTPUT);
    digitalWrite(RELAY_PIN1, HIGH);
    digitalWrite(RELAY_PIN2, HIGH);
    
    // Connect to WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" Connected!");

    // Set up Espalexa
    espalexa.addDevice(device1Name, controlDevice1);
    espalexa.addDevice(device2Name, controlDevice2);
    espalexa.begin();
    Serial.println("Alexa ready");
}

void loop() {
    // Handle Alexa commands
    espalexa.loop();
}

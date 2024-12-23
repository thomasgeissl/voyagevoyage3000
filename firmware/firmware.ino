#include <Wire.h>
#include <SPI.h>
#include <AceButton.h>
#include "./config.h"
using namespace ace_button;

#include <esp_now.h>
#include <WiFi.h>
#include <esp_now_midi.h>
esp_now_midi ESP_NOW_MIDI;
void customOnDataSent(const uint8_t* mac_addr, esp_now_send_status_t status) {
  Serial.print("Custom Callback - Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Failure");
}

AceButton _buttons[NUMBER_OF_SENSORS];

void handleEvent(AceButton*, uint8_t, uint8_t);

void setup() {
  Serial.begin(115200);
  Wire.setClock(400000);  // Set I2C to 400 kHz

  WiFi.mode(WIFI_STA);
  ESP_NOW_MIDI.setup(broadcastAddress, customOnDataSent);

  for (auto i = 0; i < NUMBER_OF_SENSORS; i++) {
    pinMode(sensorPins[i], INPUT_PULLUP);
    _buttons[i].init(sensorPins[i], HIGH, i);
  }
  ButtonConfig* buttonConfig = ButtonConfig::getSystemButtonConfig();
  buttonConfig->setEventHandler(handleEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureClick);
}

void loop() {
  auto timestamp = millis();
  for (auto i = 0; i < NUMBER_OF_SENSORS; i++) {
    _buttons[i].check();
  }
}


// The event handler for the button.
void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  uint8_t id = button->getId();
  Serial.println(id);

  switch (eventType) {
    case AceButton::kEventPressed:
      {
        Serial.println("button pressed");
        // TODO: map id to value and send control change
        // esp_err_t result = ESP_NOW_MIDI.sendControlChange(trackNotes[id], 127, MIDI_CHANNEL);
        break;
      }
    case AceButton::kEventReleased:
      {
        break;
      }
  }
}

#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include "config.h"
#include "audio_io.h"
#include "lvgl_ui.h"
#include "wake_word.h"

// Buffers
int16_t micBuffer[512];
size_t bytesRead;

// WebSocket client
WebSocketsClient webSocket;
bool wakeTriggered = false;

// Forward declarations
void playTTS(const uint8_t* audioData, size_t len);

// === WebSocket Events ===
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("❌ WebSocket disconnected");
      break;
    case WStype_CONNECTED:
      Serial.println("✅ WebSocket connected");
      break;
    case WStype_TEXT:
      Serial.printf("💬 Server reply: %s\n", payload);
      updateAssistantReply((char*)payload);
      break;
    case WStype_BIN:
      Serial.printf("🔊 Received TTS audio (%d bytes)\n", length);
      playTTS(payload, length);
      break;
  }
}

void setup() {
  Serial.begin(115200);

  // Wi-Fi connect
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.println(WiFi.localIP());

  // Init audio
  setupI2SMic();
  setupI2SSpeaker();

  // Init display
  setupLVGL();
  createDashboard();

  // WebSocket connect
  webSocket.begin(SERVER_HOST, SERVER_PORT, "/ws");
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
}

void loop() {
  // Handle WebSocket events
  webSocket.loop();

  // Read audio
  i2s_read(I2S_NUM_0, (void*)micBuffer, sizeof(micBuffer), &bytesRead, portMAX_DELAY);

  // Wake-word detection
  if (!wakeTriggered && detectWakeWord(micBuffer, bytesRead / 2)) {
    Serial.println("Wake word detected: JARVIS");
    wakeTriggered = true;
    webSocket.sendTXT("{\"event\":\"wake\"}");
  }

  // If wake triggered, stream audio to server
  if (wakeTriggered) {
    webSocket.sendBIN((uint8_t*)micBuffer, bytesRead);
  }

  // UI tick
  lv_timer_handler();
  delay(5);
}

// === Play TTS ===
void playTTS(const uint8_t* audioData, size_t len) {
  size_t bytesWritten;
  i2s_write(I2S_NUM_1, audioData, len, &bytesWritten, portMAX_DELAY);
}

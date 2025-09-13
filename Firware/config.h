#ifndef CONFIG_H
#define CONFIG_H

// ---------- WiFi ----------
const char* WIFI_SSID = "Room-303";
const char* WIFI_PASSWORD = "Jakkala1327";

// ---------- Server ----------
const char* SERVER_HOST = "192.168.1.100";   // Your PC/server running app.js
const int SERVER_PORT = 3000;                // Port of Node.js server
const char* SERVER_PATH = "/ws";             // WebSocket path

// ---------- Audio (INMP441 Mic via I2S) ----------
#define I2S_MIC_SERIAL_CLOCK  32   // SCK
#define I2S_MIC_LEFT_RIGHT    25   // WS/LRCLK
#define I2S_MIC_SERIAL_DATA   33   // SD
#define I2S_MIC_CHANNEL       I2S_CHANNEL_FMT_ONLY_LEFT

// ---------- Audio Output (MAX98357A DAC) ----------
#define I2S_SPEAKER_BCLK   26
#define I2S_SPEAKER_LRC    27
#define I2S_SPEAKER_DIN    14

// ---------- Display ----------
#define TFT_CS   5
#define TFT_DC   2
#define TFT_RST  4
#define TFT_MOSI 23
#define TFT_CLK  18
#define TFT_MISO 19

#endif

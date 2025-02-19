// 16-bit Adafruit_GFX-compatible framebuffer for RP2350 HSTX

#include <Adafruit_dvhstx.h>

// If your board definition has PIN_CKP and related defines, DVHSTX_PINOUT_DEFAULT is available
DVHSTX16 display(DVHSTX_PINOUT_DEFAULT, DVHSTX_RESOLUTION_320x240);
// If you get the message "error: 'DVHSTX_PINOUT_DEFAULTx' was not declared" then you need to give
// the pins numbers explicitly, like the example below. The order is: {CKP, D0P, D1P, D2P}
// DVHSTX16 display({12, 14, 16, 18}, DVHSTX_RESOLUTION_320x240);

void setup() {
  Serial.begin(115200);
  //while(!Serial);
  if (!display.begin()) { // Blink LED if insufficient RAM
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;) digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }
  Serial.println("display initialized");
}

void loop() {
  // Draw random lines
  display.drawLine(random(display.width()), random(display.height()), random(display.width()), random(display.height()), random(65536));
  sleep_ms(1);
}

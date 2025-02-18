// 16-bit Adafruit_GFX-compatible framebuffer for RP2350 HSTX

#include <Adafruit_dvhstx.h>

// DVHSTX16 display(DVHSTX_RESOLUTION_320x180);
DVHSTX16 display(DVHSTX_PINOUT_METRO_RP2350, DVHSTX_RESOLUTION_320x240);
//DVIGFX8 display(320, 240, dvi_timing_640x480p_60hz, VREG_VOLTAGE_1_20, pimoroni_demo_hdmi_cfg);

void setup() {
  Serial.begin(115200);
  // while(!Serial);
  if (!display.begin()) { // Blink LED if insufficient RAM
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;) digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }
  Serial.println("display initialized");
}

int i, j=1;
void loop() {
  // Draw random lines
  display.drawLine(random(display.width()), random(display.height()), random(display.width()), random(display.height()), random(65536));
i++;
if (i % j == 0) {
j *= 10;
Serial.printf("%d\n", i);
}
}

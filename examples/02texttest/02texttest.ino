// 16-bit Adafruit_GFX-compatible framebuffer for RP2350 HSTX

#include <Adafruit_dvhstx.h>

DVHSTXText3 display(DVHSTX_PINOUT_METRO_RP2350);

void setup() {
  Serial.begin(115200);
  while(!Serial);
Serial.printf("Your lucky number is %d\n", random(91));
  if (!display.begin()) { // Blink LED if insufficient RAM
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;) digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }
  Serial.println("display initialized");
}

int cx, cy, i;
void loop() {
  const static uint8_t colors[] = {
      TextColor::TEXT_RED, TextColor::TEXT_GREEN, TextColor::TEXT_BLUE, TextColor::TEXT_YELLOW, TextColor::TEXT_MAGENTA, TextColor::TEXT_CYAN, TextColor::TEXT_WHITE,
  };
  // Draw random lines
  uint8_t attr = colors[random(std::size(colors))];
  uint8_t ch = random(95) + 32;
  display.writePixel(random(display.width()), random(display.height()), (attr << 8) | ch);

if (i++ == 100) {
  i = 0;
  if ((cx += 1) == display.width()) {
    cx = 0;
    cy += 1;
    if(cy == display.height()) { cy = 0; }
  }
  display.set_cursor(cx, cy);
}
  sleep_ms(1);
}

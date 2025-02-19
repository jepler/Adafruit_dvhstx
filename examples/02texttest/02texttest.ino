// Text mode demo
// With apologies to Stanley Kubrick et al
#include <Adafruit_dvhstx.h>

// If your board definition has PIN_CKP and related defines,
// DVHSTX_PINOUT_DEFAULT is available
DVHSTXText3 display(DVHSTX_PINOUT_DEFAULT);
// If you get the message "error: 'DVHSTX_PINOUT_DEFAULTx' was not declared"
// then you need to give the pins numbers explicitly, like the example below.
// The order is: {CKP, D0P, D1P, D2P}.
//
// DVHSTXText3 display({12, 14, 16, 18});

void setup() {
  Serial.begin(115200);
  if (!display.begin()) { // Blink LED if insufficient RAM
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;)
      digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }
  display.show_cursor();
  display.print("display initialized\n\n\n\n\n");
}

const char message[] = "All work and no play makes Jack a dull boy\n";

int cx, cy, i;
void loop() {
  const static TextColor colors[] = {
      TextColor::TEXT_RED,    TextColor::TEXT_GREEN,   TextColor::TEXT_BLUE,
      TextColor::TEXT_YELLOW, TextColor::TEXT_MAGENTA, TextColor::TEXT_CYAN,
      TextColor::TEXT_WHITE,
  };

  if (i == 0) {
    auto attr = colors[random(std::size(colors))];
    for (int j = random(91 - sizeof(message)); j; j--)
      display.write(' ');
    display.set_color(attr);
  }

  int ch = message[i++];
  if (ch) {
    display.write(ch);
  } else
    i = 0;

  sleep_ms(32 + random(32));
}

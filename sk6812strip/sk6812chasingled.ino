
/*
*sk6812 grbw
esp32 dev module
bufferconverter 5v
scantime change to uint16 for greater values
status working maar leds sk6812 te ver uiteen
*/


#include <Adafruit_NeoPixel.h>

#define PIN 16
#define NUM_LEDS 11 
#define NR_OF_CHASING_LEDS 5 // <-- DEFINE YOUR CHASE SIZE HERE (e.g., 3, 5, 7)
#define scantime 300

// Use NEO_GRBW for SK6812 RGBW strips
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800); 

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  Serial.println("Chasing RED");
  // Pass R=255, G=0, B=0 to the function
  chasingVariableSize(strip.Color(255, 0, 0), scantime); 

  // Serial.println("Chasing GREEN");
  // // Pass R=0, G=255, B=0 to the function
  // chasingVariableSize(strip.Color(0, 255, 0), scantime); 
  
  // Serial.println("Chasing BLUE");
  // // Pass R=0, G=0, B=255 to the function
  // chasingVariableSize(strip.Color(0, 0, 255),scantime); 

  // // To create a true, bright white on an RGBW strip, 
  // // simply pass R=255, G=255, B=255. The Adafruit library 
  // // intelligently uses the dedicated White LED channel (W) 
  // // for this.
  // Serial.println("Chasing TRUE WHITE");
  // chasingVariableSize(strip.Color(255, 255, 255),scantime);
}


// --- Variable Chasing Function (remains unchanged) ---
void chasingVariableSize(uint32_t brightColor, uint16_t wait) {
  // Extract RGB components from the bright color to create a dim version
  // We divide the brightness by a factor (e.g., 4) for the outer LEDs
  uint8_t r = (brightColor >> 16) & 0xFF;
  uint8_t g = (brightColor >>  8) & 0xFF;
  uint8_t b = (brightColor >>  0) & 0xFF;
  
  uint32_t dimColor = strip.Color(r/4, g/4, b/4); 

  for (int i = 0; i <= strip.numPixels() - NR_OF_CHASING_LEDS; i++) {
    strip.clear(); 
    for(int j = 0; j < NR_OF_CHASING_LEDS; j++) {
      int currentPixel = i + j;
      if (j == 0 || j == (NR_OF_CHASING_LEDS - 1)) {
        strip.setPixelColor(currentPixel, dimColor); 
      } else {
        strip.setPixelColor(currentPixel, brightColor); 
      }
    }
    strip.show();
    delay(wait);
  }

  for (int i = strip.numPixels() - NR_OF_CHASING_LEDS; i >= 0; i--) {
    strip.clear();
    for(int j = 0; j < NR_OF_CHASING_LEDS; j++) {
      int currentPixel = i + j;
      if (j == 0 || j == (NR_OF_CHASING_LEDS - 1)) {
        strip.setPixelColor(currentPixel, dimColor);
      } else {
        strip.setPixelColor(currentPixel, brightColor);
      }
    }
    strip.show();
    delay(wait);
  }
}
// Existing colorWipe and theaterChase functions can be added below if needed.


/*
* ws1812 144 leds grb (Adjusted for non-blocking millis timing)
* esp32 dev module
* * Features:
* 1. Non-blocking animation using elapsedMillis.
* 2. Variable size chasing segment (NR_OF_CHASING_LEDS).
* 3. Proportional dimmed tail (20% of length, 20% brightness) if chase segment > 10.
*/

#include <Adafruit_NeoPixel.h>
#include <elapsedMillis.h> // Required for non-blocking timing

#define PIN 16 //gpio data to levelconverter to strip
#define NUM_LEDS 50 //total leds in strip
#define NR_OF_CHASING_LEDS 20 // <-- Define the length of the chasing segment
#define scantime 100         // The step interval in milliseconds (how fast it moves)

// --- DEFINITIONS FOR DIMMED TAIL ---
#define MIN_CHASE_FOR_TAIL 10  // Minimum chase length to enable the variable tail
#define TAIL_SIZE_PERCENT 20   // Percentage of NR_OF_CHASING_LEDS used for the tail
#define DIM_PERCENT 20         // Brightness percentage of the tail compared to the main color

// Use NEO_GRB for typical WS2812/SK6812 RGB strips
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800); 

// --- GLOBAL VARIABLES FOR ELAPSEDMILLIS AND CHASE STATE ---
elapsedMillis timeSinceLastStep; // Tracks time since the last animation step
int chasePosition = 0;            // Current starting index of the chasing segment
bool directionForward = true;     // Direction of the chase (true = forward, false = reverse)

// --- HELPER FUNCTION TO DIM A COLOR ---
uint32_t dimColor(uint32_t color, int dimPercentage) {
  // Extract RGB components
  uint8_t r = (color >> 16) & 0xFF;
  uint8_t g = (color >>  8) & 0xFF;
  uint8_t b = (color >>  0) & 0xFF;

  // Calculate the factor (e.g., 20% dim is 0.20 factor)
  float factor = (float)dimPercentage / 100.0f; 

  // Apply the factor and return the new color
  return strip.Color(
    (uint8_t)(r * factor), 
    (uint8_t)(g * factor), 
    (uint8_t)(b * factor)
  );
}

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  // Initialize the chase state
  chasePosition = 0; 
  directionForward = true;
  timeSinceLastStep = 0;
}

void loop() {
  // The non-blocking function is called repeatedly, but only updates 
  // the LEDs when the 'scantime' interval has passed.
  chasingWithVariableTailNonBlocking(strip.Color(255, 0, 0), scantime); 
  
  // Other non-blocking code can go here (e.g., sensor readings, WiFi checks)
}

// --- NON-BLOCKING ANIMATION FUNCTION ---
void chasingWithVariableTailNonBlocking(uint32_t brightColor, uint16_t interval) {
  
  // Check if the required time interval has passed
  if (timeSinceLastStep >= interval) {
    // Reset timer and proceed with the animation step
    timeSinceLastStep = 0; 

    // 1. Calculate the tail size and dimmed color
    bool useVariableTail = (NR_OF_CHASING_LEDS > MIN_CHASE_FOR_TAIL);
    int tailSize = 0;
    if (useVariableTail) {
      // Calculate the size of the tail (20% of the chase segment length)
      tailSize = (int)((float)NR_OF_CHASING_LEDS * ((float)TAIL_SIZE_PERCENT / 100.0f));
      // Ensure minimum and maximum practical tail sizes
      if (tailSize == 0) tailSize = 1; 
      if (tailSize > NR_OF_CHASING_LEDS / 2) tailSize = NR_OF_CHASING_LEDS / 2;
    }
    
    // *** CORRECTION: Renamed variable to avoid conflict with the dimColor function ***
    uint32_t fadedColor = dimColor(brightColor, DIM_PERCENT); 
    
    // 2. Clear the strip before drawing the next frame
    strip.clear(); 
    
    // 3. Draw the chasing segment at the current chasePosition
    for(int j = 0; j < NR_OF_CHASING_LEDS; j++) {
      int currentPixel = chasePosition + j;

      if (currentPixel < strip.numPixels()) { // Safety check
        
        // Determine the color based on the position (j) within the chasing segment
        // Pixels in the leading tail (j < tailSize) OR trailing tail (j >= end of bright body)
        if (useVariableTail && (j < tailSize || j >= (NR_OF_CHASING_LEDS - tailSize))) {
          strip.setPixelColor(currentPixel, fadedColor); 
        } else {
          // Main bright body
          strip.setPixelColor(currentPixel, brightColor); 
        }
      }
    }
    
    // 4. Send the data to the LED strip
    strip.show();

    // 5. Calculate the next chasePosition and handle direction reversal
    int maxStart = strip.numPixels() - NR_OF_CHASING_LEDS;

    if (directionForward) {
      chasePosition++;
      if (chasePosition > maxStart) {
        directionForward = false;
        chasePosition = maxStart; // Start reversing from the max position
      }
    } else {
      chasePosition--;
      if (chasePosition < 0) {
        directionForward = true;
        chasePosition = 0; // Start moving forward from the min position
      }
    }
  }
}

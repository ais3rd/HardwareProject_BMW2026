#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Screen dimensions
const int screenWidth = 128;
const int screenHeight = 64;

// Hardware Pin Definitions
const int resetPin = -1;  // Set to -1 because standard 4-pin OLEDs manage reset internally
const int sdaPin = 23;
const int sclPin = 18;

const int rxPin = 34;     // Receives incoming data from Arduino Uno
const int txPin = -1;     // Set to -1 because we are only receiving data, not transmitting

auto& unoSerial = Serial1;

// Set display driver object
Adafruit_SSD1306 display(screenWidth, screenHeight, &Wire, resetPin);

void setup() {
  // Start screen wire connection
  Wire.begin(sdaPin, sclPin);
  
  // Start communication with Arduino Uno
  unoSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  
  // Turn on OLED display at address 0x3C
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void loop() {
  // if unoSerial has new data then
  if (unoSerial.available() > 0) {
    // Read text until end of line and convert to number
    float measuredDistance = unoSerial.readStringUntil('\n').toFloat();

    // Refresh the screen 
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    
    // Draw distance number in large text
    display.setTextSize(3);
    display.setCursor(10, 15);
    display.print(measuredDistance, 1);
    
    // Draw "cm" unit in small text
    display.setTextSize(1);
    display.print(" cm");
    
    // Update screen to show changes
    display.display();
  }
}

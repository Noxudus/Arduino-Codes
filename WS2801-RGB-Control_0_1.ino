/*
  Version: 0.1
  
  Using my Arduino Mega 2560 to Control an RGB-LED-Strip [WS2801-5050FC32-CV]
    -> WS2801
    -> Extern Power-Source
    -> Using Analog- or Digital-Input for manipulation
      ->> http://arduino.cc/en/Tutorial/SimpleAudioFrequencyMeter

      255 200 150 100 050 000 000 000 000 000 000 000 000 000 000
      000 000 000 050 100 150 200 255 200 150 100 050 000 000 000 
      000 000 000 000 000 000 000 000 000 000 050 100 150 200 255
*/
#include <SPI.h>
#include <FTRGBLED.h>
#include <AudioFrequencyMeter.h>
#include <Adafruit_WS2801.h>

AudioFrequencyMeter meter;

// rechts         oben           links          unten
// [1.Strip = 11] [2.Strip = 16] [3.Strip = 11]
// [1.Strip = 11] [2.Strip = 20] [3.Strip = 11] [4.Strip = 20]
const int NUM_LEDS = 62;  //37

// Pins the RGBLED module are connected to (Green=5V (ext) / Red = CLK / Yellow = Si / Blue = GND)
const int PIN_CKI = 6;
const int PIN_SDI = 7;

// Colour values for each channel { Red, Green, Blue } with range 0-255.
const FTLEDColour ON_COLOUR = { 100, 40, 0 };

// Alternatively defined in FTREGLED.h - LED_WHITE, LED_BLACK, LED_RED, LED_GREEN, LED_YELLOW, LED_VIOLET.
const FTLEDColour OFF_COLOUR = LED_BLACK;

RGBLEDChain leds(NUM_LEDS, PIN_CKI, PIN_SDI);

// Which LED is currently selected and "on"?
byte on_led = 0;


void setup() {
  /****************************AudioFreq************************************/
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("started");

  meter.setBandwidth(70.00, 1500);  // Ignore frequency out of this range
  meter.begin(A0, 45000);           // Initialize A0 at sample rate of 45 kHz
  /****************************AudioFreq************************************/
  leds.begin();
}


void loop() {
  /*************AudioFreq**********************/
  float frequency = getAudioFreq();
  /*************AudioFreq**********************/

  // Set all LEDs to OFF_COLOUR (doesn't do anything until we call update)
  for (int i = 0; i < NUM_LEDS; i++) {
    leds.setLED(i, OFF_COLOUR);
  }
  // Set the one LED to on_colour
  leds.setLED(on_led, ON_COLOUR);

  // Update the LEDs
  leds.update();

  delay(250);

  // Increment on_led for the next run of the loop
  on_led++;
  if (on_led == NUM_LEDS)
    on_led = 0;
}

float getAudioFreq() {
  float frq = meter.getFrequency();
  Serial.print(frq);
  Serial.println(" Hz");
  return frq;
}
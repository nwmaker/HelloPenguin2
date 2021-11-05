/*
 * penguin rainbow
 * 
 * paletteCrossFade
 * 
 */
#include "FastLED.h"                                          // FastLED library.

// Fixed definitions cannot change on the fly.
#define LED_PIN     A1 // A1 for Penguin and others, A6 for startrek                                             // Data pin to connect to the strip.
#define COLOR_ORDER GRB                                       // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE    WS2812B                                       // Using APA102, WS2812, WS2801. Don't forget to modify LEDS.addLeds to suit.
#define NUM_LEDS    36                                           // Number of LED's.

#define BRIGHTNESS 64

#define UPDATES_PER_SECOND 100

struct CRGB leds[NUM_LEDS];                  

CRGBPalette16 currentPalette( CRGB::Black);

CRGBPalette16 targetPalette( PartyColors_p );

void setup() {

  delay(3000);
  
  LEDS.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 
  FastLED.setBrightness(BRIGHTNESS);
  
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);               // FastLED Power management set at 5V, 500mA.
}

void loop()
{
  ChangePalettePeriodically();

  // Crossfade current palette slowly toward the target palette
  //
  // Each time that nblendPaletteTowardPalette is called, small changes
  // are made to currentPalette to bring it closer to matching targetPalette.
  // You can control how many changes are made in each call:
  //   - the default of 24 is a good balance
  //   - meaningful values are 1-48.  1=veeeeeeeery slow, 48=quickest
  //   - "0" means do not change the currentPalette at all; freeze
  
  uint8_t maxChanges = 24; 
  nblendPaletteTowardPalette( currentPalette, targetPalette, maxChanges);


  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
  FillLEDsFromPaletteColors( startIndex);

  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = 255;
  
  for( int i = 0; i < NUM_LEDS; i++) {
    
    leds[i] = ColorFromPalette( currentPalette, colorIndex + sin8(i*16), brightness);
    colorIndex += 3;
  }
}


void ChangePalettePeriodically()
{
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;
  
  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    CRGB p = CHSV( HUE_PURPLE, 255, 255);
    CRGB g = CHSV( HUE_GREEN, 255, 255);
    CRGB b = CRGB::Black;
    CRGB w = CRGB::White;
    CRGB r = CRGB::Red;
    CRGB bb = CRGB::Blue;
    if( secondHand ==  0)  { targetPalette = RainbowColors_p; }
    if( secondHand == 10)  { targetPalette = CRGBPalette16( g,g,r,r, p,p,bb,bb, r,r,g,g, p,p,r,r); }
    if( secondHand == 20)  { targetPalette = CRGBPalette16( r,g,bb,r, g,bb,r,g, bb,r,g,bb, r,g,bb,w); }
    if( secondHand == 30)  { targetPalette = LavaColors_p; }
    if( secondHand == 40)  { targetPalette = CloudColors_p; }
    if( secondHand == 50)  { targetPalette = PartyColors_p; }
  }
}

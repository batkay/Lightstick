#include <Adafruit_NeoPixel.h>

// setup for the button and led ring
const int buttonPin = 2;
const int ledPin = 3;

const int numLed = 16;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLed, ledPin, NEO_GRB);


int buttonState = LOW;      // the current reading from the input pin
int lastButtonState = LOW;  // the previous reading from the input pin

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers


enum State {
  OFF,
  SOLID_BLUE

};
State current = OFF;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);

  strip.begin();
  strip.show();
  strip.setBrightness(200);
}

void loop() {
  //--------- First part, does button debounce and sets the current mode in FSM-----------------

  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      current = nextState(current);
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;



  //------------- Second part, displays pattern based on the current state---------------

  switch (current) {
    case OFF:
      // loops through every pixel and sets it's color to 0, 0, 0 (off) before showing
      for (int i = 0; i < numLed; ++i) {
        strip.setPixelColor(strip.color(0, 0, 0));
      }
      strip.show();
      break;

    case SOLID_BLUE:
      for (int i = 0; i < numLed; ++i) {
        strip.setPixelColor(strip.color(0, 0, 0));
      }
      strip.show();
      break;

    default:
      // turn off in undefined state
      for (int i = 0; i < numLed; ++i) {
        strip.setPixelColor(strip.color(0, 0, 0));
      }
      strip.show();
      break;
  }
}

State nextState(State current) {
  switch (current) {
    case OFF:
      return SOLID_BLUE;
    case SOLID_BLUE:
      return OFF;
    default:
      return OFF;
  }
}
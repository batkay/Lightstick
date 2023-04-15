#include <Adafruit_NeoPixel.h>

// setup for the button and led ring
const int buttonPin = 2;
const int ledPin = 3;

const int numLed = 12;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLed, ledPin, NEO_GRB);


int buttonState = LOW;      // the current reading from the input pin
int lastButtonState = LOW;  // the previous reading from the input pin

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

unsigned long cycleTime = 100;
unsigned long lastChange = millis();

bool flashOn = true;
int snakeCount = 0;
int snakeLength = 4;

enum State {
  OFF,
  SOLID_BLUE,
  FLASH,
  SNAKE
};
State current = OFF;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
  strip.begin();
  strip.show();
  strip.setBrightness(200);
}

void loop() {
  //--------- First part, does button debounce and sets the current mode in FSM-----------------

  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);
  //Serial.println(reading);

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
      if (buttonState == LOW) {
        current = nextState(current);
        Serial.println(current);
      }
      
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;



  //------------- Second part, displays pattern based on the current state---------------

  switch (current) {
    case OFF:
      // strip.show with no color set should turn it off
      for (int i = 0; i < numLed; ++i) {
        strip.setPixelColor(i, 0);
      }
      strip.show();
      break;

    case SOLID_BLUE:
      for (int i = 0; i < numLed; ++i) {
        strip.setPixelColor(i, strip.Color(200, 0, 0));
      }
      strip.show();
      break;
    case FLASH:
    // Delta timings, every few seconds it'll turn the color to purpleish
      if (millis() - lastChange > cycleTime) {
        lastChange = millis();

        if (flashOn) {
          for (int i = 0; i < numLed; ++i) {
            strip.setPixelColor(i, strip.Color(188, 66, 245));
          }
          strip.show();
        } else {
          for (int i = 0; i < numLed; ++i) {
            strip.setPixelColor(i, 0);
          }
          strip.show();
        }
        flashOn = !flashOn;
        Serial.println(flashOn);
      }
      break;
    case SNAKE:
      // more delta timings
      if (millis() - lastChange > cycleTime) {
        lastChange = millis();
        // changes colors in snake range to green
        for (int i = 0; i < snakeLength; ++i) {
          strip.setPixelColor((snakeCount + i) % numLed, strip.Color(50, 168, 82));
        }
        // rest of the pixels are set to off
        for (int j = (snakeCount + snakeLength) % numLed; j != snakeCount; j = (j + 1) % numLed) {
          strip.setPixelColor(j, 0);
        }
        strip.show();

        snakeCount = (snakeCount + 1) % numLed;
      }
      break;
    default:
      // turn off in undefined state
      strip.show();
      break;
  }
}

State nextState(State current) {
  switch (current) {
    case OFF:
      return SOLID_BLUE;
    case SOLID_BLUE:
      return FLASH;
    case FLASH:
      return SNAKE;
    case SNAKE:
      return OFF;
    default : return OFF;
  }
}
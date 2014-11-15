/* The RGB fade code was borrowed from this great website
http://www.techhelpblog.com/2013/10/22/arduino-code-smooth-fading-rgb-leds-pwm/
*/

int whiteLed1 = 5;  // white led pin 5
int whiteLed2 = 6;  // white led pin 6
int candleLed = 14;  // Candle led pin 14

// RGB LED
String led_colour;
int RED_PIN = 11; // Red led set to pin 11
int GRN_PIN = 10; // Green led set to pin 10
int BLU_PIN = 9; // Blue led set to pin 9
#define COMMON_ANODE

// ------ Fade code start ------
byte RED, GREEN, BLUE; 
byte RED_A = 0;
byte GREEN_A = 0; 
byte BLUE_A = 0;
int led_delay = 0;
byte colour_count = 1;  //Count the colours out
#define colour_count_max 7  //Set this to the max number of colours defined
#define colour_delay 4000  //Define the delay between changing colours in ms
#define time_at_colour 500  //Time to stay on a colour in ms

//Some Time values
unsigned long TIME_LED = 0;
unsigned long TIME_COLOUR = 0;

//Define Colours here.
//Blue
#define C1_R 0
#define C1_G 127
#define C1_B 255
//Red
#define C2_R 255
#define C2_G 0
#define C2_B 0
//White
#define C3_R 255
#define C3_G 255
#define C3_B 255
//Orange
#define C4_R 255
#define C4_G 127
#define C4_B 0
//Light Blue
#define C5_R 0
#define C5_G 255
#define C5_B 255
//Purple
#define C6_R 175
#define C6_G 0
#define C6_B 175
//Yellow
#define C7_R 255
#define C7_G 250
#define C7_B 0

// ------ Fade code end ------

// buttons code
const int button = 12;
int buttonPushCounter = 1;    // counts the button pushes
int buttonState = 0;    // tracks the button state
int lastButtonState = 0;    // last state of the button

// debounce code
int buttonStateDebounce;  // the current reading from the input pin
int lastButtonStateDebounce = LOW;
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;  // the debounce time; increase if the output flickers


int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by
long waitUntilpulse1 = 0;  // Millis wait times
long waitUntilpulse2 = 20;  // Millis wait times

long waitUntilpolice1 = 0;  // Millis wait times
long waitUntilpolice2 = 200;  // Millis wait times

long waitUntilstrobe1 = 0;  // Millis wait times
long waitUntilstrobe2 = 20;  // Millis wait times

void setup() {
  pinMode(whiteLed1, OUTPUT);  // Set LED as output
  pinMode(whiteLed2, OUTPUT);  // Set LED as output
  pinMode(candleLed, OUTPUT);  // Set LED as output
  
  //Assign initial values
  RED = C1_R;
  GREEN = C1_G;
  BLUE = C1_B;
  //Get the led_delay speed
  led_delay = (colour_delay - time_at_colour) / 255;
  
  pinMode(RED_PIN, OUTPUT); // Set red led pin to output
  pinMode(GRN_PIN, OUTPUT); // Set green led pin to output
  pinMode(BLU_PIN, OUTPUT); // Set blue led pin to output
  
  pinMode(button, INPUT);  // Set button input
  digitalWrite(button, HIGH);  // Set internal pull up resistor
  
  Serial.begin(9600);  // Start serial
  
  int splashScreenDelay = 250;
  // Splash screen of sorts
  setColor(255, 255, 255);  // white
  delay(splashScreenDelay);
  setColor(0, 0, 0); // off
  delay(splashScreenDelay);
  digitalWrite(candleLed, HIGH);
  delay(splashScreenDelay);
  digitalWrite(candleLed, LOW);
  delay(splashScreenDelay);
  digitalWrite(whiteLed1, HIGH);
  delay(splashScreenDelay);
  digitalWrite(whiteLed1, LOW);
  delay(splashScreenDelay);
  digitalWrite(whiteLed2, HIGH);
  delay(splashScreenDelay);
  digitalWrite(whiteLed2, LOW);

}

void loop() {
  
// ------Start of button code ------
  int reading = digitalRead(button);
  buttonState = digitalRead(button);

  if (reading != lastButtonStateDebounce) {
    lastDebounceTime = millis();
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading != buttonStateDebounce) {
      buttonStateDebounce = reading;
      
        if (buttonState != lastButtonState) {
          
          if (buttonState == LOW) {
            buttonPushCounter++;
            digitalWrite(candleLed, LOW); // Turn LED off
            setColor(0, 0, 0);  // off
            analogWrite(whiteLed1, LOW); // Turn LED off
            analogWrite(whiteLed2, LOW); // Turn LED off
            
            if (buttonPushCounter == 14) {
              buttonPushCounter = 1;}
          }
          else {
          }
        }
          lastButtonState = buttonState;
    }
  }
  lastButtonStateDebounce = reading;
// ------ End of button code ------

  
  if(buttonPushCounter == 1) {
    // Off mode
    digitalWrite(candleLed, LOW);
    setColor(0, 0, 0);  // off
    analogWrite(whiteLed1, LOW);
    analogWrite(whiteLed2, LOW);
  }

  if(buttonPushCounter == 2) {
    setColor(255, 255, 0);  // yellow
  }
  
  if(buttonPushCounter == 3) {
    setColor(255, 127, 0);  // orange
  }
  
  if(buttonPushCounter == 4) {
    setColor(255, 0, 0);  // red
  }
  
  if(buttonPushCounter == 5) {
    setColor(175, 0, 175);  // purple
  }
  
  if(buttonPushCounter == 6) {
    setColor(0, 255, 255);  // aqua
  }
  
  if(buttonPushCounter == 7) {
    setColor(0, 127, 255);  // blue
  }
  
  if(buttonPushCounter == 8) {
    setColor(0, 255, 0);  // green
  }
  
  if(buttonPushCounter == 9) {
    // Police flashing mode
    if (millis() >= waitUntilpolice1) {
      setColor(255, 0, 0);  // red
      waitUntilpolice1 += 400;
    }
    if (millis() >= waitUntilpolice2) {
      setColor(0, 127, 255);  // blue
      waitUntilpolice2 += 400;
    }
  }
  
  if(buttonPushCounter == 10) {
    // RGB Colour fade mode
    if(millis() - TIME_LED >= led_delay){
    TIME_LED = millis();

    //Run the LED Function to check and adjust the values
    LED();
  }

  if(millis() - TIME_COLOUR >= colour_delay){
    TIME_COLOUR = millis();

    //Run the Colour Change function
    COLOUR();
  }

}

  if(buttonPushCounter == 11) {
    // Pulsing white LED mode
    if (millis() >= waitUntilpulse1) {
      analogWrite(whiteLed1, brightness);
      analogWrite(whiteLed2, brightness);
      brightness = brightness + fadeAmount;
      waitUntilpulse1 += 40;
    }
    if (brightness == 0 || brightness == 255) {
      fadeAmount = -fadeAmount ; 
    }
    if (millis() >= waitUntilpulse2) {
      analogWrite(whiteLed1, brightness);
      analogWrite(whiteLed2, brightness);
      brightness = brightness + fadeAmount;
      waitUntilpulse2 += 40;
    }
    if (brightness == 0 || brightness == 255) {
      fadeAmount = -fadeAmount ; 
    }   
  }
  
  if(buttonPushCounter == 12) {
    // Strobe mode
    if (millis() >= waitUntilstrobe1) {
      digitalWrite(whiteLed1, LOW);
      digitalWrite(whiteLed2, LOW);
      waitUntilstrobe1 += 40;
    }
    if (millis() >= waitUntilstrobe2) {
      digitalWrite(whiteLed1, HIGH);
      digitalWrite(whiteLed2, HIGH);
      waitUntilstrobe2 += 40;
    }
  }
  
  if(buttonPushCounter == 13) {
    // Candle mode
    digitalWrite(candleLed, HIGH);
  }
  
  
  
}
  
// Set RGB colours
void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(RED_PIN, red);
  analogWrite(GRN_PIN, green);
  analogWrite(BLU_PIN, blue);  
}

void LED()
{
  //Check Values and adjust "Active" Value
  if(RED != RED_A){
    if(RED_A > RED) RED_A = RED_A - 1;
    if(RED_A < RED) RED_A++;
  }
  if(GREEN != GREEN_A){
    if(GREEN_A > GREEN) GREEN_A = GREEN_A - 1;
    if(GREEN_A < GREEN) GREEN_A++;
  }
  if(BLUE != BLUE_A){
    if(BLUE_A > BLUE) BLUE_A = BLUE_A - 1;
    if(BLUE_A < BLUE) BLUE_A++;
  }

  //Assign modified values to the pwm outputs for each colour led
  analogWrite(RED_PIN, RED_A);
  analogWrite(GRN_PIN, GREEN_A);
  analogWrite(BLU_PIN, BLUE_A);

}

void COLOUR()
{
  //Increment the colour by one or go back to 1 if maxed out
  if(colour_count < colour_count_max) colour_count++;
  else colour_count = 1;

  if(colour_count == 1){
    RED = C1_R;
    GREEN = C1_G;
    BLUE = C1_B;
  } else if(colour_count == 2){
    RED = C2_R;
    GREEN = C2_G;
    BLUE = C2_B;
  } else if(colour_count == 3){
    RED = C3_R;
    GREEN = C3_G;
    BLUE = C3_B;
  } else if(colour_count == 4){
    RED = C4_R;
    GREEN = C4_G;
    BLUE = C4_B;
  } else if(colour_count == 5){
    RED = C5_R;
    GREEN = C5_G;
    BLUE = C5_B;
  } else if(colour_count == 6){
    RED = C6_R;
    GREEN = C6_G;
    BLUE = C6_B;
  } else if(colour_count == 7){
    RED = C7_R;
    GREEN = C7_G;
    BLUE = C7_B;
  }
}

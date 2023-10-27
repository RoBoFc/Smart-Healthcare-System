#define USE_ARDUINO_INTERRUPTS true  // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>   // Includes the PulseSensorPlayground Library.
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 64     // OLED display height, in pixels
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


const int PulseWire = A1;     // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED = LED_BUILTIN;  // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;          // Determine which Signal to "count as a beat" and which to ignore.
                              // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                              // Otherwise leave the default "550" value.

PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  display.clearDisplay();

  // Configure the PulseSensor object, by assigning our variables to it.
  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED);  //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);
  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.
  }

  display.setCursor(0, 0);
  display.print("Setting Things...");
  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop() {
  int myBPM;
  // put your main code here, to run repeatedly:
  if (pulseSensor.sawStartOfBeat()) {              // Constantly test to see if "a beat happened".
    myBPM = pulseSensor.getBeatsPerMinute();       // Calls function on our pulseSensor object that returns BPM as an "int"// "myBPM" hold this BPM value now.
    Serial.println("♥️  A HeartBeat Happened ! ");  // If test is "true", print a message "a heartbeat happened".
    Serial.print("BPM: ");                         // Print phrase "BPM: "
    Serial.println(myBPM);                         // Print the value inside of myBPM.
  }
  display.setCursor(5, 0);
  display.print("BPM: -> ");
  display.print(myBPM);
  display.display();

  delay(20);
}

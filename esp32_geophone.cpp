#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_NeoPixel.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;


// ESP32 MAC Address
// 3C:E9:0E:85:75:06
#define NUM_LEDS 30               // Number of LEDs in the strip
#define DATA_PIN 13               // Pin connected to the data input of the LED strip
#define BRIGHTNESS 100            // Brightness level (0-255)
#define PHOTOSENSOR_THRESHOLD 40 // Threshold value for photoresistor to turn off LEDs
#define GEOPHONE_PIN 33           // Pin connected to the geophone sensor


Adafruit_NeoPixel strip(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);
int sensorVal;
const int ANALOG_READ_PIN = 34;
const int resolution = 8;


void setup() {
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show();

  SerialBT.begin("Fall-detection2");
  while (!SerialBT)

    delay(10);


  Serial.begin(115200);
}

void loop() {

  // LED strip control
  analogReadResolution(resolution);
  sensorVal = analogRead(ANALOG_READ_PIN);
  Serial.print("Photoresistor value: ");
  Serial.println(sensorVal);
  int warmWhiteValue = map(sensorVal, 0, 1023, 0, 255);
  if (sensorVal > PHOTOSENSOR_THRESHOLD) {
    // Turn off the LEDs if the photoresistor reading is below the threshold
    strip.fill(strip.Color(0, 0, 0)); // Set color to black (off)
  } else {
    // Set the LED color to green
    strip.fill(strip.Color(250, 120, 25)); // Set color to green (R, G, B)
  }

  // Geophone control
  int geophoneVal = analogRead(GEOPHONE_PIN);
  Serial.print("Geophone Sensor Value: ");
  Serial.println(geophoneVal);
  delay(300);

  # change threshold for fall detection
  if(geophoneVal>15){
    SerialBT.print("Fall detected");
    Serial.print("BT Fall detected");


  }

  // Display geophone value through BLE notification

  strip.show();
  delay(100);
}
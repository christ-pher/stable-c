#include <Adafruit_LSM6DS3TRC.h>
#include <Adafruit_LIS3MDL.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_AHRS.h>
#include <Adafruit_NeoPixel.h>

Adafruit_LSM6DS3TRC lsm6ds3trc; // Accel. & Gyro
Adafruit_LIS3MDL lis3mdl; // Magnetometer
Adafruit_Madgwick filter; // Filter
Adafruit_NeoPixel onePixel; // Status light

unsigned long timestamp;

float prop_thrust_values[4] = {50, 50, 50, 50};

// ----- Drone Motor Thrust Logic -----

void correct_forward_roll(float roll_amt) {
    // Increase back prop thrust by (roll_amt/2)%
    prop_thrust_values[2] += (roll_amt/2);
    prop_thrust_values[3] += (roll_amt/2);

    // Decrease front prop thrust by (roll_amt/2)%
    prop_thrust_values[0] += ((roll_amt/2) * -1);
    prop_thrust_values[1] += ((roll_amt/2) * -1);
}

void correct_backward_roll(float roll_amt) {
    // Increase front prop thrust by (roll_amt/2)%
    prop_thrust_values[0] += ((roll_amt/2) * -1);
    prop_thrust_values[1] += ((roll_amt/2) * -1);

    // Decrease back prop thrust by (roll_amt/2)%
    prop_thrust_values[2] += (roll_amt/2);
    prop_thrust_values[3] += (roll_amt/2);
}

void correct_left_pitch(float pitch_amt) {
    // Increase left prop thrust by (pitch_amt/2)%
    prop_thrust_values[0] += ((pitch_amt/2) * -1);
    prop_thrust_values[2] += ((pitch_amt/2) * -1);

    // Decrease right prop thrust by (pitch_amt/2)%
    prop_thrust_values[1] += (pitch_amt/2);
    prop_thrust_values[3] += (pitch_amt/2);
}

void correct_right_pitch(float pitch_amt) {
    // Increase right prop thrust by (pitch_amt/2)%
    prop_thrust_values[1] += (pitch_amt/2);
    prop_thrust_values[3] += (pitch_amt/2);

    // Decrease left prop thrust by (pitch_amt/2)%
    prop_thrust_values[0] += ((pitch_amt/2) * -1);
    prop_thrust_values[2] += ((pitch_amt/2) * -1);
}

void handle_roll(float roll_amt) {
    if (roll_amt >= 0) {
        correct_backward_roll(roll_amt);
    } else {
        correct_forward_roll(roll_amt);
    }
}

void handle_pitch(float pitch_amt) {
    if (pitch_amt >= 0) {
        correct_left_pitch(pitch_amt);
    } else {
        correct_right_pitch(pitch_amt);
    }
}

void reset_thrust() {
    for (int i=0; i<4; i++) {
        prop_thrust_values[i] = 50.0;
    }
}

void set_status_light() {
  int r=0, g=255, b=0;
  for (int i=0; i<4; i++) {
    if (prop_thrust_values[i] >= 60 || prop_thrust_values[i] <= 40) {
      r=255; g=0; b=0;
      break;
    }
  }
  onePixel.setPixelColor(0, r, g, b);
  onePixel.show();
}

// ----- System Setup -----

void setup() {
  onePixel = Adafruit_NeoPixel(1, 8, NEO_GRB + NEO_KHZ800);

  onePixel.begin();             // Start the NeoPixel object
  onePixel.clear();             // Set NeoPixel color to black (0,0,0)
  onePixel.setBrightness(20);   // Affects all subsequent settings
  onePixel.show();              // Update the pixel state

  // Initialize LSM6DS sensor
  if (!lsm6ds3trc.begin_I2C()) {
    onePixel.setPixelColor(0, 200, 100, 0);
    onePixel.show();
    while (1);
  }

  // Initialize LIS3MDL sensor
  if (!lis3mdl.begin_I2C()) {
    onePixel.setPixelColor(0, 200, 100, 0);
    onePixel.show();
    while (1);
  }

  filter.begin(10);  // Sample rate (Hz)

  timestamp = millis();
}

// ----- Event Loop -----

void loop() {
  // Get current timestamp
  unsigned long now = millis();
  float deltaTime = (now - timestamp) / 1000.0;  // Time difference in seconds
  timestamp = now;

  // Sensor events
  sensors_event_t accel, gyro, temp, mag;
  lsm6ds3trc.getEvent(&accel, &gyro, &temp);
  lis3mdl.getEvent(&mag);

  // Update the filter with sensor data
  filter.update(gyro.gyro.x, gyro.gyro.y, gyro.gyro.z, 
                accel.acceleration.x, accel.acceleration.y, accel.acceleration.z, 
                mag.magnetic.x, mag.magnetic.y, mag.magnetic.z);

  // Get roll, pitch, and yaw in degrees
  float roll = filter.getRoll();
  float pitch = filter.getPitch();
  float heading = filter.getYaw();

  // Adjust thrust
  handle_roll(roll);
  handle_pitch(pitch);

  // Update stability indicator light
  set_status_light();

  delay(10);

  reset_thrust();
}

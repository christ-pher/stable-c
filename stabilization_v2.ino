#include <Adafruit_LSM6DS3TRC.h>
#include <Adafruit_LIS3MDL.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_AHRS.h>

Adafruit_NeoPixel onePixel(1, 8, NEO_GRB + NEO_KHZ800); // LED
Adafruit_LSM6DS3TRC lsm6ds3trc;                         // Accel. & Gyro
Adafruit_LIS3MDL lis3mdl;                               // Magnetometer
Adafruit_NXPSensorFusion filter;                        // Filter

constexpr float DEFAULT_THRUST = 50.0f;
constexpr float THRUST_ADJUSTMENT_FACTOR = 0.5f;

float prop_thrust_values[4] = {DEFAULT_THRUST};

// ----- Drone Motor Thrust Logic -----
inline void adjust_thrust(int index, float value) {
    prop_thrust_values[index] += value;
}

void correct_roll(float roll_amt, bool forward) {
    float adjustment = roll_amt * THRUST_ADJUSTMENT_FACTOR;
    if (forward) adjustment *= -1;

    adjust_thrust(0, -adjustment);
    adjust_thrust(1, -adjustment);
    adjust_thrust(2, adjustment);
    adjust_thrust(3, adjustment);
}

void correct_pitch(float pitch_amt, bool left) {
    float adjustment = pitch_amt * THRUST_ADJUSTMENT_FACTOR;
    if (!left) adjustment *= -1;

    adjust_thrust(0, -adjustment);
    adjust_thrust(2, -adjustment);
    adjust_thrust(1, adjustment);
    adjust_thrust(3, adjustment);
}

inline void handle_roll(float roll_amt) {
    correct_roll(roll_amt, roll_amt < 0);
}

inline void handle_pitch(float pitch_amt) {
    correct_pitch(pitch_amt, pitch_amt >= 0);
}

void reset_thrust() {
    for (float &thrust : prop_thrust_values) {
        thrust = DEFAULT_THRUST;
    }
}

void set_status_light() {
    int r = 0, g = 255, b = 0;
    for (float thrust : prop_thrust_values) {
        if (thrust >= 60.0f || thrust <= 40.0f) {
            r = 255; g = 0; b = 0;
            break;
        }
    }
    onePixel.setPixelColor(0, r, g, b);
    onePixel.show();
}

// ----- System Setup -----
void setup() {
    onePixel.begin();             // Start the NeoPixel object
    onePixel.clear();             // Set NeoPixel color to black (0,0,0)
    onePixel.setBrightness(20);   // Affects all subsequent settings
    onePixel.show();              // Update the pixel state

    if (!lsm6ds3trc.begin_I2C()) {
        onePixel.setPixelColor(0, 200, 100, 0);
        onePixel.show();
        while (true);
    }

    if (!lis3mdl.begin_I2C()) {
        onePixel.setPixelColor(0, 200, 100, 0);
        onePixel.show();
        while (true);
    }

    filter.begin(10); // Sample rate (Hz)
}

// ----- Event Loop -----
void loop() {
    sensors_event_t accel, gyro, temp, mag;
    lsm6ds3trc.getEvent(&accel, &gyro, &temp);
    lis3mdl.getEvent(&mag);

    filter.update(gyro.gyro.x, gyro.gyro.y, gyro.gyro.z,
                  accel.acceleration.x, accel.acceleration.y, accel.acceleration.z,
                  mag.magnetic.x, mag.magnetic.y, mag.magnetic.z);

    float roll = filter.getRoll();
    float pitch = filter.getPitch();

    handle_roll(roll);
    handle_pitch(pitch);

    set_status_light();

    delay(10);
    reset_thrust();
}

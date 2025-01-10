//  File:  demoAccelerationExpAve.ino
//
//  Demonstrate use of a exponential averaging to smooth data from the
//  LSM6DSS accelerometer on an Adafruit Feather nRF52840 Sense
//
//  Adapted from feather_sense_sensor_demo.ino from Adafruit
//  Gerald Recktenwald, gerry@pdx.edu,  2021-11-03

#include <Adafruit_LSM6DS33.h>    // Library for support of LSM6DS33 chip
Adafruit_LSM6DS33 accelerometer;  // Create an accelerometer object

// -------------------------------------------------------------------------------
void setup() 
{
  Serial.begin(115200);
  accelerometer.begin_I2C();        //  Start the I2C interface to the sensors
}

// -------------------------------------------------------------------------------
void loop() 
{
  float alfa=0.3;           //  Exponential averaging parameter:  0 < alfa <= 1.0
  float ax, ay, az, atot;   //  Acceleration components and magnitude

  // -- Get new values of ax, ay, az.  Supply pointers to getAveragedAcceleration
  //    to allow that function to change the contents of ax, ay, az.  nave is
  //    passed by value because getAveragedAcceleration doesn't need to change it.
  getExpAveAcceleration(alfa, &ax, &ay, &az);

  // -- Compute magnitude of acceleration from components
  atot = sqrt(ax*ax + ay*ay + az*az);

  Serial.print(ax,3);
  Serial.print("  ");  Serial.print(ay,3);
  Serial.print("  ");  Serial.print(az,3);
  Serial.print("  ");  Serial.println(atot,3);

  delay(25);
}

// -------------------------------------------------------------------------------
//  Read the LSM6DS33 accelerometer and return the exponential average of the
//  X, Y and Z components of acceleration.  alfa is the exponential smoothing
//  parameter applied to each component.  0 < alfa <= 1.0
//
void getExpAveAcceleration(float alfa, float *axAve, float *ayAve, float *azAve) {

  float ax,ay,az;                                //  Store latest readings
  sensors_event_t accel, gyro, temp;             //  Data structs for sensor data

  // -- Static variables are retained between function calls.  For exponential
  //    averaging, we only need to retain the average value from previous
  //    call to the getExpAveAcceleration function.
  static float axOld=0.0, ayOld=0.0, azOld=0.0;

  // -- get the new data
  accelerometer.getEvent(&accel, &gyro, &temp);  //  get the data
  ax = accel.acceleration.x;   //  Store latest value 
  ay = accel.acceleration.y; 
  az = accel.acceleration.z; 

  // -- Compute the exponential averages of each component
  *axAve = alfa*ax + (1.0-alfa)*axOld;
  *ayAve = alfa*ay + (1.0-alfa)*ayOld;
  *azAve = alfa*az + (1.0-alfa)*azOld;

  // -- Save current readings.  These will be the old readings on the
  //    next time that this function is called
  axOld = *axAve;
  ayOld = *ayAve;
  azOld = *azAve;
}

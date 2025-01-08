// [0] Roll X-Axis
// [1] Pitch Y-Axis
// [2] Yaw Z-Axis

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

float telemetry_data[3] = {0.0, 0.0, 0.0};
float prop_thrust_values[4] = {50.0, 50.0, 50.0, 50.0};

void correct_forward_roll(float roll_amt) {
    // Increase front prop thrust by (roll_amt/2)%
    prop_thrust_values[0] = prop_thrust_values[0] + ((roll_amt/2) * -1);
    prop_thrust_values[1] = prop_thrust_values[1] + ((roll_amt/2) * -1);

    // Decrease back prop thrust by (roll_amt/2)%
    prop_thrust_values[2] = prop_thrust_values[2] + (roll_amt/2);
    prop_thrust_values[3] = prop_thrust_values[3] + (roll_amt/2);
}

void correct_backward_roll(float roll_amt) {
    // Increase back prop thrust by (roll_amt/2)%
    prop_thrust_values[2] = prop_thrust_values[2] + (roll_amt/2);
    prop_thrust_values[3] = prop_thrust_values[3] + (roll_amt/2);

    // Decrease front prop thrust by (roll_amt/2)%
    prop_thrust_values[0] = prop_thrust_values[0] + ((roll_amt/2) * -1);
    prop_thrust_values[1] = prop_thrust_values[1] + ((roll_amt/2) * -1);
}

void correct_left_pitch(float pitch_amt) {
    // Increase left prop thrust by (pitch_amt/2)%
    prop_thrust_values[0] = prop_thrust_values[0] + (pitch_amt/2);
    prop_thrust_values[2] = prop_thrust_values[2] + (pitch_amt/2);

    // Decrease right prop thrust by (pitch_amt/2)%
    prop_thrust_values[1] = prop_thrust_values[1] + ((pitch_amt/2) * -1);
    prop_thrust_values[3] = prop_thrust_values[3] + ((pitch_amt/2) * -1);
}

void correct_right_pitch(float pitch_amt) {
    // Increase right prop thrust by (pitch_amt/2)%
    prop_thrust_values[1] = prop_thrust_values[1] + ((pitch_amt/2) * -1);
    prop_thrust_values[3] = prop_thrust_values[3] + ((pitch_amt/2) * -1);

    // Decrease left prop thrust by (pitch_amt/2)%
    prop_thrust_values[0] = prop_thrust_values[0] + (pitch_amt/2);
    prop_thrust_values[2] = prop_thrust_values[2] + (pitch_amt/2);
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

void log_thrust() {
    system("clear"); // "cls" on Windows
    printf("X: %.3f Y: %.3f Z: %.3f\n\n", telemetry_data[0], telemetry_data[1], telemetry_data[2]);
    printf("%.3f %.3f\n%.3f %.3f\n", prop_thrust_values[0], prop_thrust_values[1], prop_thrust_values[2], prop_thrust_values[3]);
}

float random_telemetry_data() { // TEMPORARY FOR TESTING
    float random = (float)rand() / RAND_MAX;
    return -10.0f + random * 20.0f;
}

void read_sensor_data() {  // TEMPORARY FOR TESTING
    for (int i = 0; i < 3; i++) {
        telemetry_data[i] = random_telemetry_data();
    }
}

int main() {
    while (1) {
        read_sensor_data();
        handle_roll(telemetry_data[0]);
        handle_pitch(telemetry_data[1]);
        // TO-DO: apply_thrust();
        log_thrust();

        usleep(15625); // 64 tick
        // getchar(); // Waits for ENTER (for dev)

        reset_thrust();
    }

    return 0;
}

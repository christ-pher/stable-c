#include <stdio.h>
#include <windows.h>

int max_thrust = 100;
int min_thrust = 80;
int poll = 10;
int prop_speeds[4];

void draw_drone() {
    system("cls");
    printf("%3d     %3d\n   \\___/\n   /   \\\n%3d     %3d",
        prop_speeds[0],
        prop_speeds[1],
        prop_speeds[2],
        prop_speeds[3]
    );
}

void init_props() {
    prop_speeds[0] = 100;
    prop_speeds[1] = 100;
    prop_speeds[2] = 100;
    prop_speeds[3] = 100;
}

void change_prop_speed(int prop, int delta) {
    if (delta == 0) { // UP
        if (prop_speeds[prop] >= max_thrust) { return; }
        prop_speeds[prop] = prop_speeds[prop] + 1;
    } else { // DOWN
        if (prop_speeds[prop] <= min_thrust) { return; }
        prop_speeds[prop] = prop_speeds[prop] - 2;
    }
}

void handle_controls() {
    for (int x = 0; x < 4; x++) {
        change_prop_speed(x, 0);
    }

    if (GetAsyncKeyState(VK_UP) & 0x8000) {
        change_prop_speed(0, 1);
        change_prop_speed(1, 1);
    }

    if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
        change_prop_speed(2, 1);
        change_prop_speed(3, 1);
    }

    if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
        change_prop_speed(0, 1);
        change_prop_speed(2, 1);
    }

    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
        change_prop_speed(1, 1);
        change_prop_speed(3, 1);
    }
}

int main() {
    init_props();
    while (1) {

        handle_controls(); // Check input & apply thrust changes

        draw_drone(); // Re draw terminal display

        Sleep(poll); // event loop delay

        // Exit on ESC key press
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            break;
        }
    }

    return 0;
}

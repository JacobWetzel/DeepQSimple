#include <Windows.h>
#include <iostream>

int main() {
    Sleep(60000);
    for(int i = 0; i < 2000; i++) {
        // Simulate pressing the 'l' key
        keybd_event('L', 0, 0, 0);
        keybd_event('L', 0, KEYEVENTF_KEYUP, 0);

        // Sleep for 100ms to avoid overwhelming the system
        Sleep(10);  // Adjust the delay if needed
    }
    return 0;
}

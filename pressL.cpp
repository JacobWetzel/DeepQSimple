#include <Windows.h>
#include <iostream>

int main() {
    Sleep(5000);
    while(true) {
        if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
            //std::cout << "Up arrow key pressed!\n";
            break;
        }
        // Simulate pressing the 'l' key
        keybd_event('L', 0, 0, 0);
        keybd_event('L', 0, KEYEVENTF_KEYUP, 0);

        // Sleep for 100ms to avoid overwhelming the system
        Sleep(75);  // Adjust the delay if needed
    }
    return 0;
}

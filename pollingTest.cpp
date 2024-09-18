#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <windows.h>
using namespace std;
void readLastCharacters(const std::string& filename, std::size_t numChars) {
    std::streampos lastPosition = 0;
    std::string lastChars;

    while (true) {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);

        if (file.is_open()) {
            // Get the current file size
            std::streampos fileSize = file.tellg();

            // Check if the file has been truncated or rotated
            if (fileSize < lastPosition) {
                lastPosition = 0;
            }

            // Proceed if the file has grown or changed
            if (fileSize != lastPosition) {
                // Determine how many characters to read
                std::size_t charsToRead = static_cast<std::size_t>(fileSize) < numChars
                                          ? static_cast<std::size_t>(fileSize)
                                          : numChars;

                // Seek to the position to start reading
                file.seekg(-static_cast<std::streamoff>(charsToRead), std::ios::end);

                // Read the characters into a buffer
                std::string buffer(charsToRead, '\0');
                file.read(&buffer[0], charsToRead);

                // If the last characters have changed, print them
                if (buffer != lastChars) {
                    lastChars = buffer;
                    for(int i = 20; i < 30; i++){
                        if(lastChars.substr(i, 6) == "setpos"){
                            cout << i << endl;
                            break;
                        }
                    }
                    std::cout << lastChars << std::endl;
                }

                // Update the last known position
                lastPosition = fileSize;
            }

            file.close();
        } else {
            std::cerr << "Unable to open file: " << filename << std::endl;
        }

        // Wait before polling again
        Sleep(2);
    }
}

int main() {
    // Replace "yourfile.txt" with the path to your file
    // Adjust the number 100 to the desired number of characters
    std::string filename = R"(C:\Program Files (x86)\Steam\steamapps\common\Counter-Strike Global Offensive\game\csgo\console.log)";

    readLastCharacters(filename, 100);
    return 0;
}
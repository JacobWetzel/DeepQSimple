#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <iomanip>
#include <windows.h>
#include <chrono>
#include <cerrno>  // for errno
#include <cstring> // for strerror
#include <vector>
#include <random>
#include <algorithm>
#include <filesystem>
#include <sstream>
#include "calculateInputs.h"
#include <mutex>
#include <condition_variable>
#include <windows.h>
#include <mmsystem.h>

using namespace std;

std::mutex mtx;
std::condition_variable cv;
std::string lastLine;
bool dataReady = false;

auto lRead = std::chrono::high_resolution_clock::now();

auto pressL(){

    // Simulate pressing the 'l' key
    keybd_event('L', 0, 0, 0);
    keybd_event('L', 0, KEYEVENTF_KEYUP, 0);
    auto retval = std::chrono::high_resolution_clock::now();
    // Sleep for 100ms to avoid overwhelming the system
    //Sleep(25);  // Adjust the delay if needed
    return retval;

}


//true for successful read, false for bad read
bool updateValues(string str, vector<double>& posAng){
    //auto start = std::chrono::high_resolution_clock::now();

    if(str.size() <= 21){return false;}

    if(str.substr(15, 6) != "setpos"){
        return false;
    }

    int vectorPos = 0;
    for(int i = 22; i < str.size(); i++){
        double val = 0;
        double decVal = 0;
        bool isNeg = false;
        while(str[i] != '.'){
            if(str[i] == '-'){isNeg = true;}
            else{
                val = (val * 10) + (str[i] - '0');
            }
            i++;
        }
        i++;
        while(str[i] != ' ' && i < str.size() && str[i] != ';'){
            if(str[i] >= '0' && str[i] <= '9'){
                decVal = (decVal * 10) + (str[i] - '0');
            }
            i++;
        }
        if(isNeg){val = val * -1; decVal = decVal * -1;}
        posAng[vectorPos] = (val + (decVal / 1000000.0));
        vectorPos++;
        if(str[i] == ';'){
            int b= 0;
            while(str[i] != ' '){
                i++;
                if(str[i] == ' '){
                    if(b == 0){
                        b++;
                        str[i] = '.';
                        continue;
                    }
                    else{
                        i++; break;
                    }
                }
            }
            isNeg = false;
            double yawVal = 0.0;
            double yawDec = 0.0;
            while(str[i] != ' ' && str[i] != '.'){
                if(str[i] == '-'){isNeg = true;}
                else{
                    yawVal = (yawVal * 10) + (str[i] - '0');
                }
                i++;
            }
            i++;
            while(str[i] != ' '){
                yawDec = (yawDec * 10) + (str[i] - '0');
                i++;
            }
            if(isNeg){yawVal = yawVal * -1; yawDec = yawDec * -1;}
            posAng[vectorPos] = (yawVal + (yawDec/1000000.0));
            break;
        }
    }
    //auto end = std::chrono::high_resolution_clock::now();
    //auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    //cout << duration << endl;

    return true;
}


void writeDoublesToFile(const std::vector<double>& data, const std::string& filename) {
    // Open the file in write mode
    std::ofstream outfile(filename);
    vector<std::ostringstream> oss(data.size());
    // Check if the file was successfully opened
    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }
    string flushString = "";
    int i = 0;
    // Write each double to the file on a new line
    for (const auto& value : data) {
        oss[i] << std::fixed << std::setprecision(2) << value;
        //outfile << std::fixed << std::setprecision(6) << value << std::endl;
        i++;
    }
    for(int i = 0; i < oss.size(); i++){
        flushString += oss[i].str() + '\n';
    }
    outfile << flushString;
    // Close the file
    outfile.close();
}




std::string getLastLine(const std::string& filename) {


    std::ifstream file(filename, std::ios::ate); // Open for reading at the end of the file

    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return ""; // Return empty string if file can't be opened
    }

    const int64_t file_size = file.tellg(); // Get the size of the file
    const int64_t read_size = 97; // Number of characters to read

    // Calculate the position to start reading from
    int64_t start_pos = file_size - read_size;
    if (start_pos < 0) start_pos = 0; // Ensure start position is not negative

    file.seekg(start_pos); // Move the position to start_pos

    std::string data;
    data.resize(read_size); // Resize the string to hold the data that will be read
    file.read(&data[0], file_size - start_pos); // Read the data into the string

    file.close(); // Close the file

    for(int i = 0; i < data.size(); i++){
        if(static_cast<int>(static_cast<unsigned char>(data[i])) == 10){
            string retval = data.substr(i + 1, data.size()-(i+1));
            return retval;
        }
    }




    return data;
}

void lc(string logFilePath){
        std::ifstream file(logFilePath, std::ios::binary); // Open for reading at the end of the file
        file.seekg(0, std::ios::end); 
        auto st = chrono::high_resolution_clock::now();
        auto endVal = chrono::high_resolution_clock::now();
        auto interval = (long long)(std::chrono::duration_cast<std::chrono::milliseconds>(endVal - st).count());
        cout << interval << endl;
    auto previousFileSize = filesystem::file_size(logFilePath);
    string l = "";
    while (true) {

            auto currentFileSize = filesystem::file_size(logFilePath);
            //cout << currentFileSize << " " << previousFileSize << endl;
            if (currentFileSize != previousFileSize) {
                endVal = chrono::high_resolution_clock::now();
                interval = (long long)(std::chrono::duration_cast<std::chrono::milliseconds>(endVal - st).count());
                st = chrono::high_resolution_clock::now();
                cout << interval << endl;
                previousFileSize = currentFileSize;  // Update the file size to compare in next iteration
                
                
                const int64_t file_size = file.tellg();//currentFileSize / 8;//file.tellg(); // Get the size of the file
                const int64_t read_size = 97; // Number of characters to read

                // Calculate the position to start reading from
                int64_t start_pos = file_size - read_size;
                if (start_pos < 0) start_pos = 0; // Ensure start position is not negative

                file.seekg(start_pos); // Move the position to start_pos

                std::string data;
                data.resize(read_size); // Resize the string to hold the data that will be read
                file.read(&data[0], file_size - start_pos); // Read the data into the string

                
                //l = getLastLine(logFilePath);
                
                for(int i = 0; i < data.size(); i++){
                    if(static_cast<int>(static_cast<unsigned char>(data[i])) == 10){
                        l = data.substr(i + 1, data.size()-(i+1));
                        lastLine = l;
                        cout << l << endl;
                        break;
                    }
                }


            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Wait for 1 second before checking again
}

int is_file_empty(const std::string& file_path) {
    std::ifstream file(file_path, std::ios::binary | std::ios::ate);  // Open in binary mode and set position at the end
    if (!file) {
        std::cerr << "Could not open the file: " << file_path << std::endl;
        return false;
    }

    std::streamsize size = file.tellg();  // Get file size by position
    //cout << size << endl;
    return (int)size;  // Returns true if file is empty
}

string prevLine = "";
string monitorFileUpdates(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    std::string line;
    string curLine = prevLine;
    bool pressed = false;
    //std::streampos lastPos = file.end;
    file.seekg(0, std::ios::end); 
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return "";
    }
    std::streampos lastPos = file.tellg();
    int i = 0;
    auto t = chrono::high_resolution_clock::now();
    pressL();
    
    while(curLine == prevLine && chrono::duration_cast<std::chrono::milliseconds>(chrono::high_resolution_clock::now() - t).count() < 5000){
        i = 0;
        //cout << chrono::duration_cast<std::chrono::milliseconds>(chrono::high_resolution_clock::now() - t).count() << endl;;
        file.seekg(lastPos);
        if (std::getline(file, line)) {
            curLine = line;
            lastPos = file.tellg();
            //t = pressL();
        } else if (file.eof()) {
            //cout << "Here?\n";
            file.clear();  // Clear EOF to allow further reading if the file is updated
            file.seekg(lastPos);  // Stay at the last known good position
            if(!pressed){
                //t = pressL();
                pressed = true;
            }
            // Consider notifying the main thread that EOF was reached
            // Optionally, you could also use a separate flag or condition to indicate EOF state
        }   
        Sleep(10);
    }
    if(curLine != prevLine){    
        prevLine = curLine;
        lastPos = file.tellg();
        file.close();
        //cout << "her?\n";
        return prevLine;
    }

        
    
    //cout << "here!\n";
    return prevLine;
    file.close();
}

void readLastCharacters(const std::string& filename, std::size_t numChars) {
    std::streampos lastPosition = 0;
    std::string lastChars;
    //std::ifstream file(filePath, std::ios::binary);
    std::string line;
    std::string curLine = "";
    std::string prevLine = "";
    auto t = chrono::high_resolution_clock::now();
    //std::ifstream file(filename, std::ios::binary | std::ios::ate);

    // Open the file once before the polling loop
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }
    bool isT = false;
    while (true) {
        // Clear any EOF or fail flags
        file.clear();

        // Get the current file size
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();

        // Check if the file has been truncated or rotated
        if (fileSize < lastPosition) {
            lastPosition = 0;
            file.seekg(0, std::ios::beg);
        }

        // Proceed if the file has grown or changed

        if (fileSize != lastPosition) {
            isT = true;
            // Determine how many characters to read
            std::size_t charsToRead = static_cast<std::size_t>(fileSize) < numChars
                                    ? static_cast<std::size_t>(fileSize)
                                    : numChars;

            // Seek to the position to start reading
            file.seekg(-static_cast<std::streamoff>(charsToRead), std::ios::end);

            // Read the characters into a buffer
            std::string buffer(charsToRead, '\0');
            file.read(&buffer[0], charsToRead);

            // If the last characters have changed, process them
            if (buffer != lastChars) {
                lastChars = buffer;
                //std::cout << lastChars << std::endl;
                for (int i = 20; i < 30; i++) {
                    if (lastChars.substr(i, 6) == "setpos") {
                        mtx.lock();
                        lastLine = lastChars.substr(i, lastChars.size() - i);
                        //std::cout << lastLine << std::endl;
                        lRead = t;
                        mtx.unlock();
                        break;
                    }
                }
                //std::cout << lastChars << std::endl;
            }
            // Update the last known position
            lastPosition = fileSize;

        }
        if(isT){
            isT = false;
            auto prev = t;
            t = pressL();
            //cout << "timeBetweenReads  " << (long long)(std::chrono::duration_cast<std::chrono::milliseconds>(t - prev).count()) << endl;;
        }
        // Wait before polling again
        else{
            Sleep(5);
        }
    }

    // Close the file when done (this code is unreachable due to the infinite loop)
    file.close();
}


int main() {
    timeBeginPeriod(1);
    Sleep(5000);
    std::string filename = R"(C:\Program Files (x86)\Steam\steamapps\common\Counter-Strike Global Offensive\game\csgo\console.log)";
    //std::thread fileMonitor(monitorFileUpdates, filename);  // Start the monitor in a new thread
    //std::thread fileMonitor(readLastCharacters, filename, 100);
    
    //std::thread fileMonitor(readLastLine, filename);  // Start the monitor in a new thread
    //std::thread fileMonitor(lc, filename);
    //fileMonitor.detach();
    vector<double> calculatedInputs;
    bool isTr = false;
    int fSize;
    auto prevT = chrono::high_resolution_clock::now();
    auto st = chrono::high_resolution_clock::now();
    long long interval = 0;
    bool newCi = true;
    vector<double> posAng(4, 0.0);
    vector<double> prevPosAng(4, 0.0);
    string ll = "";
    CalculateInputs *ci = NULL;//new CalculateInputs(posAng);
    ci = new CalculateInputs(posAng);
    auto endVal = chrono::high_resolution_clock::now();
    int itr = 0;
    while (true) {

            //cout << (long long)(std::chrono::duration_cast<std::chrono::milliseconds>(curTime - st).count()) << endl;
            //lastLine = read_Last_Line(file);
            // Process the new line
            auto pre = chrono::high_resolution_clock::now();
            fSize = is_file_empty("nnValues.txt");
            while(fSize > 1){
                Sleep(10);
                fSize = is_file_empty("nnValues.txt");
            }
            if(fSize == 1 || ci == NULL){
                newCi = true;
                //ci = new CalculateInputs(posAng);
                printf("delete/new \n");
            }
            //cout << "here\n";
            ll = monitorFileUpdates(filename);
            cout << "LL" << " " << ll << endl;

            auto curTime = chrono::high_resolution_clock::now();
            prevT = chrono::high_resolution_clock::now();

            //std::cout << "Processing new line: " << ll << std::endl;
            // Reset the flag and release the lock
            try{isTr = updateValues(ll, posAng);}
            catch(exception e){cout << "read exception\n";}
            
            
                if(isTr){
                    //posAng[3] += 180;
                    for (double& num : posAng) {
                        num = std::round(num * 100.0) / 100.0;
                    }
                    if(posAng[0] != prevPosAng[0] || posAng[1] != prevPosAng[1] || posAng[2] != prevPosAng[2] || posAng[3] != prevPosAng[3]){
                        if(abs(posAng[0]) > 5000  || abs(posAng[1]) > 2000 || abs(posAng[2]) > 1000){
                            continue;
                        }
                        //endVal = chrono::high_resolution_clock::now();

                        interval = 100000.0;//(long long)(std::chrono::duration_cast<std::chrono::microseconds>(st - prevT).count());

                        //printf("%ld int \n", interval);
                        //std::cout << std::fixed << std::setprecision(6) << posAng[0] << " " << posAng[1] << " " << posAng[2] << " " << posAng[3] << std::endl;
                        //printf("%lf ang \n", posAng[3]);
                        cout << posAng[3] << endl;
                        //st = chrono::high_resolution_clock::now();
                        //interval = max(interval, (long long)25);
                        if(newCi){
                            delete(ci);
                            newCi = false;
                            ci = new CalculateInputs(posAng);
                        }
                        calculatedInputs = ci->GetNNInputs(posAng, interval);

                        //interval = 0;


                        //interval = 0;
                        //prevPosAng = posAng;
                        //auto post = chrono::high_resolution_clock::now();
                        //cout << "rt " << (long long)(std::chrono::duration_cast<std::chrono::microseconds>(post - pre).count()) << endl;;
                    }
                    prevPosAng = posAng;
                    writeDoublesToFile(calculatedInputs, "nnValues.txt");
                }
            
        Sleep(5);
    }

    return 0;
}



//sv_cheats 1;sv_enablebunnyhopping 1;sv_maxvelocity 7000;sv_staminamax 0;sv_staminalandcost 0;sv_staminajumpcost 0;sv_accelerate_use_weapon_speed 0;sv_staminarecoveryrate 0;sv_autobunnyhopping 1;sv_airaccelerate 2000;mp_warmup_offline_enabled 1;mp_warmuptime 99999;cl_allow_multi_input_binds 1;mp_warmup_start;
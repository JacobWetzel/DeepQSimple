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

using namespace std;

std::mutex mtx;
std::condition_variable cv;
std::string lastLine;
bool dataReady = false;

//true for successful read, false for bad read
bool updateValues(string str, long long interval, vector<double>& posAng){
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
        oss[i] << std::fixed << std::setprecision(6) << value;
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



void monitorFileUpdates(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    std::string line;
    
    //std::streampos lastPos = file.end;
    file.seekg(0, std::ios::end); 
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return;
    }
    std::streampos lastPos = file.tellg();
    int i = 0;
    while (true) {
        try{
            file.seekg(lastPos);

            if (std::getline(file, line)) {
                lastLine = line;
                dataReady = true;
                //file.seekg(0, std::ios::end);
                lastPos = file.tellg();
            } else if (file.eof()) {
                file.clear();  // Clear EOF to allow further reading if the file is updated
                file.seekg(lastPos);  // Stay at the last known good position
                // Consider notifying the main thread that EOF was reached
                // Optionally, you could also use a separate flag or condition to indicate EOF state
            }
            lastPos = file.tellg();


        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        catch(exception e){
            cout << "exception" << endl;
        }
    }
    file.close();
}



int main() {
    std::string filename = R"(C:\Program Files (x86)\Steam\steamapps\common\Counter-Strike Global Offensive\game\csgo\console.log)";
    std::thread fileMonitor(monitorFileUpdates, filename);  // Start the monitor in a new thread
    //std::thread fileMonitor(readLastLine, filename);  // Start the monitor in a new thread
    //std::thread fileMonitor(lc, filename); 
    fileMonitor.detach();
    CalculateInputs ci = CalculateInputs();
    bool isTr = false;
    long long interval = 0;
    vector<double> posAng(4, 0.0);
    vector<double> prevPosAng(4, 0.0);
    string ll = "";

    auto st = chrono::high_resolution_clock::now();
    auto endVal = chrono::high_resolution_clock::now();
    int itr = 0;
    while (true) {

            if((long long)(std::chrono::duration_cast<std::chrono::milliseconds>(endVal - st).count()) > 400){
                ci = CalculateInputs();
            }
            //lastLine = read_Last_Line(file);
            // Process the new line
            ll = lastLine;
            //std::cout << "Processing new line: " << ll << std::endl;
            // Reset the flag and release the lock
            try{isTr = updateValues(ll, interval, posAng);}
            catch(exception e){cout << "read exception\n";}
            
            try{
                if(isTr){
                    //posAng[3] += 180;
                    if(posAng[0] != prevPosAng[0] || posAng[1] != prevPosAng[1] || posAng[2] != prevPosAng[2] || posAng[3] != prevPosAng[3]){
                        if(abs(posAng[0]) > 5000  || abs(posAng[1]) > 2000 || abs(posAng[2]) > 1000){
                            cout << ll << " " << posAng[0] << " " << posAng[1] << " " << posAng[2] << endl;
                            while(true){
                                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                            }
                        }
                        endVal = chrono::high_resolution_clock::now();
                        interval = (long long)(std::chrono::duration_cast<std::chrono::milliseconds>(endVal - st).count());
                        st = chrono::high_resolution_clock::now();
                        interval = interval == 0 ? 80 : interval;
                        interval = max(interval, (long long)75);
                        vector<double> calculatedInputs = ci.GetNNInputs(posAng, interval);
                        printf("%ld\n", interval);
                        std::cout << std::fixed << std::setprecision(6) << posAng[0] << " " << posAng[1] << " " << posAng[2] << " " << posAng[3] << std::endl;
                        //interval = 0;


                        writeDoublesToFile(calculatedInputs, "nnValues.txt");
                        //interval = 0;
                        prevPosAng = posAng;
                    }

                }
            }catch(exception e){
                cout << "compute exception" << endl;
            }

    }

    return 0;
}



//sv_cheats 1;sv_enablebunnyhopping 1;sv_maxvelocity 7000;sv_staminamax 0;sv_staminalandcost 0;sv_staminajumpcost 0;sv_accelerate_use_weapon_speed 0;sv_staminarecoveryrate 0;sv_autobunnyhopping 1;sv_airaccelerate 2000;mp_warmup_offline_enabled 1;mp_warmuptime 99999;cl_allow_multi_input_binds 1;mp_warmup_start;
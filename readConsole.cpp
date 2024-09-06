#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <iomanip>
#include <chrono>
#include <cerrno>  // for errno
#include <cstring> // for strerror
#include <vector>
#include <random>
#include <algorithm>
#include <sstream>
#include "calculateInputs.h"


using namespace std;

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

std::string read_last_line(std::ifstream& file) {
    std::string last_line;
    char ch;

    // Start by checking if the file is not empty
    file.seekg(0, std::ios::end);  // Go to the end of the file
    if (file.tellg() == 0) {
        //cout << "here0?\n";
        return "";  // File is empty, return empty string
    }

    file.seekg(-1, std::ios::end);  // Go to the last character of the file

    // Handle the case where the last character is a newline
    file.get(ch);
    while(ch == '\n') {
        // If the last character is a newline, skip it and go backwards
        //cout << "here1?\n";
        file.seekg(-2, std::ios::cur);
        file.get(ch);
    }

    // Now read backwards until we find a newline or reach the beginning
    while (file.tellg() > 0) {
        file.get(ch);
        //cout << ch;
        if (ch == '\n') {
            break;  // We found the newline, move on to read the last line
        }

        // Move backwards by 2 positions (1 for current, 1 for previous character)
        file.seekg(-2, std::ios::cur);
    }

    // Now read the last line (we may be at the first line)
    std::getline(file, last_line);
    return last_line;
}
std::string read_Last_Line(std::ifstream& file) {
    std::string last_line;
    char ch;

    // Check if the file is not empty
    file.seekg(0, std::ios::end);  // Go to the end of the file
    if (file.tellg() == 0) {
        printf("FILE EMPTY");
        return "";  // File is empty, return empty string
    }

    // Move one character back from the end
    file.seekg(-1, std::ios::end);
    printf("i");
    // Skip any trailing newlines (in case there are multiple)
    file.get(ch);
    while (ch == '\n'){// && file.tellg() > 1) {
        // Move backwards by 2 characters to skip the newline and check the next
        
        file.seekg(-2, std::ios::cur);
        file.get(ch);
    }
    printf("j");
    // Now we should be right after the last valid character of the last line
    while (file.tellg() > 0) {
        file.get(ch);
        
        if (ch == '\n') {
            break;  // We found the previous newline, stop here
        }
        // Move back by 2 characters (one for the current, one for the previous)
        file.seekg(-2, std::ios::cur);
    }
    printf("k");
    // Now read the last line
    std::getline(file, last_line);

    return last_line;
}

int main() {
    std::string filename = R"(C:\Program Files (x86)\Steam\steamapps\common\Counter-Strike Global Offensive\game\csgo\console.log)";
    //std::ifstream file;
    std::string lastLine;
    CalculateInputs ci = CalculateInputs();
    long long interval = 0;
    vector<double> posAng(4, 0.0);
    vector<double> prevPosAng(4, 0.0);
    //setpos str starts at char 15
    //file.open(filename, std::ios::in);
    std::ifstream file(filename, std::ios::in);
    file.seekg(0, std::ios::end);
    auto st = chrono::high_resolution_clock::now();
    auto endVal = chrono::high_resolution_clock::now();
    while (true) {
        auto start = chrono::high_resolution_clock::now();
        if (file.is_open()) {
            //std::string line;
            /*while (std::getline(file, line)) {
                lastLine = line;
            }*/
            //file.close();
            //lastLine = "08/24 20:34:18 setpos -63.172424 50.632206 127.871246;setang 0.790020 -0.501600 0.000000";
            //cout << "here\n";
            
            /*if (file.peek() != EOF) {
                std::getline(file, line);  // Read the next line
                if (!line.empty()) {
                    //std::cout << "Most recent line: " << line << std::endl;
                    lastLine = line;
            }*/
            lastLine = read_Last_Line(file);
            //std::cout << "Most recent line: " << lastLine << std::endl;
                if(updateValues(lastLine, interval, posAng)){
                    if(posAng[0] != prevPosAng[0] || posAng[1] != prevPosAng[1] || posAng[2] != prevPosAng[2] || posAng[3] != prevPosAng[3]){
                        //auto start = chrono::high_resolution_clock::now();
                        endVal = chrono::high_resolution_clock::now();
                        interval = (long long)(std::chrono::duration_cast<std::chrono::milliseconds>(endVal - st).count());
                        vector<double> calculatedInputs = ci.GetNNInputs(posAng, interval);
                        printf("%ld\n", interval);
                        std::cout << std::fixed << std::setprecision(6) << posAng[0] << " " << posAng[1] << " " << posAng[2] << " " << posAng[3] << std::endl;
                        interval = 0;
                        st = chrono::high_resolution_clock::now();
                        //auto end = std::chrono::high_resolution_clock::now();
                        //auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                        //cout << duration << endl;

                        writeDoublesToFile(calculatedInputs, "nnValues.txt");
                        //interval = 0;
                        prevPosAng = posAng;
                    }
                    
                    //auto start = chrono::high_resolution_clock::now();

                    /*for(int i = 0; i < calculatedInputs.size(); i++){
                        cout << calculatedInputs[i] << " ";
                    }cout << endl << endl;*/
                }


            //else {
                //std::cerr << "Unable to open file! Error: " << strerror(errno) << std::endl;
            //}
        }
        //file.clear(); 
        // Sleep for a short duration before checking again
        //std::this_thread::sleep_for(std::chrono::milliseconds(5));
        //interval += 5;
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        //cout << (1000000.0 / double(duration)) << " " << interval << endl;
        
        //interval += 33;
    }

    return 0;
}

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <iomanip>
#include <chrono>
#include <cerrno>  // for errno
#include <cstring> // for strerror
#include <vector>
using namespace std;

//true for successful read, false for bad read
bool updateValues(string str, long long interval, vector<double>& posAng){
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
            i+=17;
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

    return true;
}

int main() {
    std::string filename = R"(C:\Program Files (x86)\Steam\steamapps\common\Counter-Strike Global Offensive\game\csgo\console.log)";
    std::ifstream file;
    std::string lastLine;
    long long interval = 0;
    vector<double> posAng(4, 0.0);
    //setpos str starts at char 15
    while (true) {
        file.open(filename, std::ios::in);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                lastLine = line;
            }
            file.close();
            //lastLine = "08/24 20:34:18 setpos -63.172424 50.632206 127.871246;setang 0.790020 -0.501600 0.000000";

            if(updateValues(lastLine, interval, posAng)){interval = 0;
                std::cout << std::fixed << std::setprecision(6) << posAng[0] << " " << posAng[1] << " " << posAng[2] << " " << posAng[3] << std::endl;
            }

        } //else {
            //std::cerr << "Unable to open file! Error: " << strerror(errno) << std::endl;
        //}

        // Sleep for a short duration before checking again
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
        interval += 33;
    }

    return 0;
}

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>
#include <string>
#include <synchapi.h>


using namespace std;


int getRandomNumber() {
    std::random_device rd;

    std::mt19937 gen(rd());

    std::uniform_int_distribution<> distrib(0, 6);

    return distrib(gen);
}

int getRandomNumber2() {
    std::random_device rd;

    std::mt19937 gen(rd());

    std::uniform_int_distribution<> distrib(0, 3);

    return distrib(gen);
}

string cordsToString(vector<double> nums){
    int a = nums[0];
    int b = nums[1];
    string retval = "";
    retval += to_string(a);
    retval += " ";
    retval += to_string(b);
    retval += " ";
    retval += "32";
    return retval;


}


void addWideSkinnyLoc(vector<vector<double>>& LocVec, int row, int col){
    LocVec.push_back({(double(row + 1)), (double(col + 3))});
}
void addWideRectLoc(vector<vector<double>>& LocVec, int row, int col){
    LocVec.push_back({(double(row + 2)), (double(col + 3))});
}
void addLongSkinnyLoc(vector<vector<double>>& LocVec, int row, int col){
    LocVec.push_back({(double(row + 3)), (double(col + 1))});
}
void addLongRectLoc(vector<vector<double>>& LocVec, int row, int col){
    LocVec.push_back({(double(row + 3)), (double(col + 2))});
}
void addBigSqLoc(vector<vector<double>>& LocVec, int row, int col){
    LocVec.push_back({(double(row + 3)), (double(col + 3))});
}
void addsmallSqLoc(vector<vector<double>>& LocVec, int row, int col){
    LocVec.push_back({(double(row + 1)), (double(col + 1))});
}
void addSqLoc(vector<vector<double>>& LocVec, int row, int col){
    LocVec.push_back({(double(row + 2)), (double(col + 2))});
}

string wsl(int x, int y){
    string retval = "{{";
    retval += to_string(x);
    retval += ".0, ";
    retval += to_string(y);
    retval += ".0, 32.0}, {";
    retval += "64.0, 192.0, 64.0}}";
    return retval;
}
string longRectStr(int x, int y){
    string retval = "{{";
    retval += to_string(x);
    retval += ".0, ";
    retval += to_string(y);
    retval += ".0, 32.0}, {";
    retval += "192.0, 128.0, 64.0}}";
        return retval;

}
string longRectSkinStr(int x, int y){
    string retval = "{{";
    retval += to_string(x);
    retval += ".0, ";
    retval += to_string(y);
    retval += ".0, 32.0}, {";
    retval += "192.0, 64.0, 64.0}}";
        return retval;

}
string sqStr(int x, int y){
    string retval = "{{";
    retval += to_string(x);
    retval += ".0, ";
    retval += to_string(y);
    retval += ".0, 32.0}, {";
    retval += "128.0, 128.0, 64.0}}";
        return retval;

}
string wideRectStr(int x, int y){
    string retval = "{{";
    retval += to_string(x);
    retval += ".0, ";
    retval += to_string(y);
    retval += ".0, 32.0}, {";
    retval += "128.0, 192.0, 64.0}}";
        return retval;

}
string bigSqStr(int x, int y){
    string retval = "{{";
    retval += to_string(x);
    retval += ".0, ";
    retval += to_string(y);
    retval += ".0, 32.0}, {";
    retval += "192.0, 192.0, 64.0}}";
        return retval;

}
string smSqStr(int x, int y){
    string retval = "{{";
    retval += to_string(x);
    retval += ".0, ";
    retval += to_string(y);
    retval += ".0, 32.0}, {";
    retval += "64.0, 64.0, 64.0}}";
        return retval;

}


int main(){

    //int wideSkinny, wideRect, longSkinny, longRect, bigSq, smallSq, sq = 200;
    //wideSkinny = wideRect = longSkinny = longRect = bigSq = smallSq = sq;

    // 0 -> wideSkinny, 4 => wideRect, 2=>longSKinny, 3=>longRect, 4=>bigSq, 5=> smallSq, 6=>sq

    vector<int> blocks(7, 200);


    vector<vector<double>> wideSkinnyLocs, wideRectLocs, longSkinnyLocs, longRectLocs, bigSqLocs, smallSqLocs, sq;


    vector<vector<bool>> grid(2000, vector<bool>(2000, false));

    bool cont = true;
    int tlcnt = 0;
    for(int j = 0; j < 330; j += 9){ // to player left
        for(int i = 0; i < 330; i+=3){ //player spawn face
            int rNum = getRandomNumber();
            if(blocks[rNum] == 0){
                int cnt = 0;
                while(blocks[rNum] == 0){
                    rNum = (rNum + 1) % 7;
                    cnt++;
                    if(cnt > 8){
                        cout << "HERE " << tlcnt << endl;
                        Sleep(1000);
                        cont = false;
                        break;
                    }
                }
            }
            if(!cont){break;}
            blocks[rNum]--;
            tlcnt++;
            if(rNum == 0){
                addWideSkinnyLoc(wideSkinnyLocs, i, j);
                i+=2;
                i += getRandomNumber2();
            }
            else if(rNum == 4){
                addWideRectLoc(wideRectLocs, i, j);
                i += 4;
                i += getRandomNumber2();
            }
            else if(rNum == 2){
                addLongSkinnyLoc(longSkinnyLocs, i, j);
                i += 6;
                i += getRandomNumber2();
            }
            else if(rNum == 1){
                addLongRectLoc(longRectLocs, i, j);
                i += 6;
                i += getRandomNumber2();
            }
            else if(rNum == 5){
                addBigSqLoc(bigSqLocs, i, j);
                i += 6;
                i += getRandomNumber2();
            }
            else if(rNum == 6){
                addsmallSqLoc(smallSqLocs, i, j);
                i += 2;
                i += getRandomNumber2();
            }
            else{
                addSqLoc(sq, i, j);
                i += 4;
                i += getRandomNumber2();
            }
        }
    }

    vector<vector<vector<double>>> allLocs;
    allLocs = {wideSkinnyLocs, longRectLocs, longSkinnyLocs, sq, wideRectLocs, bigSqLocs, smallSqLocs};
    vector<vector<double>> transCords;
    for(int i = 0; i < allLocs.size(); i++){
        for(int j = 0; j < allLocs[i].size(); j++){
            transCords.push_back({allLocs[i][j][0] * 32, allLocs[i][j][1] * 32});
        }
    }

    for(int i = 0; i < transCords.size(); i++){
        cout << transCords[i][0] << " " << transCords[i][1] << " " << transCords.size() << " " << i << endl;
    }
        cout << wideSkinnyLocs.size() << " | " << longRectLocs.size() << " " << longSkinnyLocs.size() << " " << sq.size() << " " << wideRectLocs.size() << " " << bigSqLocs.size() << " " << smallSqLocs.size() << endl;

    
    string filePath = "bhop_scattered2.txt";

    // Open the file for reading
    std::ifstream inputFile(filePath);

    if (!inputFile) {
        std::cerr << "Error: File could not be opened for reading!" << std::endl;
        return 1;
    }

    std::cout << "Reading from the file..." << std::endl;

    // Read and display the contents of the file
    //std::string line;
    //while (std::getline(inputFile, line)) {
    //    std::cout << line << std::endl;
    //}

    std::string fileContents((std::istreambuf_iterator<char>(inputFile)),
                             std::istreambuf_iterator<char>());

    // Close the input file
    inputFile.close();

    //cout << fileContents << endl;
    cout << fileContents.size() << endl;
    size_t i = 80000;
    for(i; i < fileContents.size() - 30; i++){
        if(fileContents.substr(i, 19) == "\"nodeID\" \"int\" \"89\""){
            break;
        }
        if(i % 1000000 == 0){
            cout << "million\n";
        }
    }
    int placeCount = 0;
    string finStr = "";
    for(i; i < fileContents.size(); i++){
        if(fileContents.substr(i, 20) == "\"origin\" \"vector3\" \""){
            i += 20;
            string tmpString = fileContents.substr(0, i);
            int j = i + 1;
            while(fileContents[j] != '\"'){
                j++;
            }
            string restOfStr = fileContents.substr(j, (fileContents.size() - j));
            string addStr = cordsToString(transCords[placeCount]);
            placeCount++;
            tmpString += addStr;
            tmpString += restOfStr;
            fileContents = tmpString;

            if(placeCount == 1400){
                cout << "here\n";
                break;
            }

        }
    }

    string cordsForProgram = "";

    //for(int m = 0; m < transCords)

    ofstream outputFile("newScatter.txt");
    outputFile << fileContents;

//{wideSkinnyLocs, longRectLocs, longSkinnyLocs, sq, wideRectLocs, bigSqLocs, smallSqLocs};

    string cString = "";
    for(int l = 0; l < transCords.size(); l++){
        if(l < 200){
            cordsForProgram += wsl(transCords[l][0], transCords[l][1]);
            cordsForProgram += "\n";
        }else if(l < 400){
            cordsForProgram += longRectStr(transCords[l][0], transCords[l][1]);
            cordsForProgram += "\n";
        }else if(l < 600){
            cordsForProgram += longRectSkinStr(transCords[l][0], transCords[l][1]);
            cordsForProgram += "\n";
        }else if(l < 800){
            cordsForProgram += sqStr(transCords[l][0], transCords[l][1]);
            cordsForProgram += "\n";
        }else if(l < 1000){
            cordsForProgram += wideRectStr(transCords[l][0], transCords[l][1]);
            cordsForProgram += "\n";
        }else if(l < 1200){
            cordsForProgram += bigSqStr(transCords[l][0], transCords[l][1]);
            cordsForProgram += "\n";
        }else{
            cordsForProgram += smSqStr(transCords[l][0], transCords[l][1]);
            cordsForProgram += "\n";
        }
    }

    ofstream ofil("newblockCords.txt");
    ofil << cordsForProgram;


    return 0;
}
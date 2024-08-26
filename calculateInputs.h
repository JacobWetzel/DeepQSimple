#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

class CalculateInputs{
public:
    CalculateInputs();
    vector<vector<vector<double>>> allBlocks;
    vector<double>  GetNNInputs(vector<double> playerPos, double dt);
    double prevXPos;
    double prevYPos;
    double prevZPos;
    double prevXSpeed;
    double prevYSpeed;
    double prevZSpeed;
    double prevAng;
};
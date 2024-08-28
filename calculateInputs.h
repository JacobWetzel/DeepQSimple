#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

class CalculateInputs{
public:
    CalculateInputs();
    vector<vector<vector<double>>> blocksOriginAndDimensions;
    vector<vector<vector<double>>> allBlocks;
    vector<double>  GetNNInputs(vector<double> playerPos, double dt);
    vector<vector<vector<double>>> calculateBlockkPositions(vector<double> blocks);
    double prevXPos;
    double prevYPos;
    double prevZPos;
    double prevXSpeed;
    double prevYSpeed;
    double prevZSpeed;
    double prevAng;
};
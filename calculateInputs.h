#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

class CalculateInputs{
public:
    CalculateInputs();
    vector<vector<vector<double>>> bd;
    vector<vector<vector<double>>> allBlocks;
    vector<vector<vector<double>>> blockFaces0;
    vector<vector<vector<double>>> blockFaces2;
    vector<vector<vector<double>>> blockFaces3;
    vector<vector<vector<double>>> blockFaces4;
    vector<vector<vector<vector<double>>>> fourRadFaces;

    vector<double>  GetNNInputs(vector<double> playerPos, double dt);
    vector<vector<vector<double>>> calculateBlockPositions(vector<vector<vector<double>>> blocks, int face);
    vector<double> calculateRadialDistances(vector<double>& playerPos, vector<double>& distFromFaces);
    vector<vector<vector<double>>> pruneFacesByZValue(double zVal);
    double prevXPos;
    double prevYPos;
    double prevZPos;
    double prevXSpeed;
    double prevYSpeed;
    double prevZSpeed;
    double prevAng;
};
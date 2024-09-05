#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <queue>
#include <Windows.h>

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
    vector<vector<vector<double>>> fourRadFaces;

    vector<double>  GetNNInputs(vector<double>& playerPos, double dt);
    vector<vector<vector<double>>> calculateBlockPositions(int face);
    void calculateRadialDistances(vector<double>& playerPos, vector<double>& distFromFaces);
    vector<vector<vector<double>>> pruneFacesByZValue(double zVal);
    pair<double, double> calculateMB(vector<vector<double>>& face, vector<double>& playerPos);
    double findDist(vector<vector<double>>& face, vector<double>& playerPos, double ang);

    
    double prevXPos;
    double prevYPos;
    double prevZPos;
    double prevXSpeed;
    double prevYSpeed;
    double prevZSpeed;
    double prevAng;
};
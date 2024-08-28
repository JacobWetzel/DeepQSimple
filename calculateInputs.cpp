#include "calculateInputs.h"

using namespace std;

CalculateInputs::CalculateInputs(){
    //x is from start looking to end, y is left right, z is up down (as defined by the map, x y z arbitrary)
    blocksOriginAndDimensions = {{{-320, 160, 32}, {1024, 1088, 64}},{{},{}}, {{},{}}, {{},{}}, {{},{}}, {{},{}}, };
    prevYPos = prevXPos = prevZPos = prevXSpeed = prevYSpeed = prevZSpeed = prevAng = 0;
}

/*
*   
*   Need to return <player x Pos>, <player y Pos>, <player z Pos>, <player ang>, <player x velo>, <player y velo>, <player z velo>,
*   <player ang change>, <player x accel>, <player y accel>, <player z accel>, <10 closest blocks>
*
*/

vector<vector<vector<double>>> CalculateInputs::calculateBlockkPositions(vector<double> blocks){

}


vector<double> CalculateInputs::GetNNInputs(vector<double> playerPos, double dt){
    int numBlocksToFind = 2;
    //vector<vector<vector<double>>> distances(numBlocksToFind, vector<vector<double>>(4, vector<double>(3)));
    vector<int> dists(numBlocksToFind);
    vector<double> retval(numBlocksToFind * 3);
    priority_queue<pair<double, int>> closest;
    double xSpeed, ySpeed, zSpeed, xAccel, yAccel, zAccel, angChange;


    //calculate the numbBlocksToFind closest blocks, store their x, y, z in double vector of doubles

    for(int i = 0; i < allBlocks.size(); i++){

        double smallestDist = 1000000000.0;
        for(int j = 0; j < allBlocks[i].size(); j++){  
            smallestDist = min(smallestDist, abs(playerPos[0] - allBlocks[i][j][0]) + abs(playerPos[1] - allBlocks[i][j][1]) + abs(playerPos[2] - allBlocks[i][j][2]));
        }
        if(closest.size() < numBlocksToFind){
            closest.push({smallestDist, i});
        }else{
            if(closest.top().first >= smallestDist){
                closest.pop();
                closest.push({smallestDist, i});
            }
        }
    }

    int itr = numBlocksToFind - 1;
    while(closest.size() > 0){
        dists[itr] = closest.top().second;
        //distances[itr] = allBlocks[closest.top().second];
        closest.pop();
        itr--;
    }

    

    // calculating the speed, theta change, and accel since previous timestep

    xSpeed = (playerPos[0] - prevXPos) / dt;
    ySpeed = (playerPos[1] - prevYPos) / dt;
    zSpeed = (playerPos[2] - prevZPos) / dt;
    xAccel = (xSpeed - prevXSpeed) / dt;
    yAccel = (ySpeed - prevYSpeed) / dt;
    zAccel = (zSpeed - prevZSpeed) / dt;
    angChange = (playerPos[3] - prevAng);


    //update variables to have up to date prev-values
    prevXPos = playerPos[0];
    prevYPos = playerPos[1];
    prevZPos = playerPos[2];
    prevAng = playerPos[3];
    prevXSpeed = xSpeed;
    prevYSpeed = ySpeed;
    prevZSpeed = zSpeed;

    retval = {playerPos[0], playerPos[1], playerPos[2], playerPos[3], xSpeed, ySpeed, zSpeed, angChange, xAccel, yAccel, zAccel};

    for(int i = 0; i < dists.size(); i++){
        for(int j = 0; j < allBlocks[dists[i]].size(); j++){
            for(int k = 0; k < 4; k++){
                retval.push_back(allBlocks[dists[i]][k][0]);
                retval.push_back(allBlocks[dists[i]][k][1]);
                retval.push_back(allBlocks[dists[i]][k][2]);
            }
        }
    }

    return retval;


}
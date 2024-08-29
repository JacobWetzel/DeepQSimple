#include "calculateInputs.h"

using namespace std;

CalculateInputs::CalculateInputs(){
    //x is from start looking to end, y is left right, z is up down (as defined by the map, x y z arbitrary)
    bd = {{{-320, 160, 32}, {1024, 1088, 64}},{{},{}}, {{},{}}, {{},{}}, {{},{}}, {{},{}}, };
    prevYPos = prevXPos = prevZPos = prevXSpeed = prevYSpeed = prevZSpeed = prevAng = 0;
    allBlocks = calculateBlockPositions(bd, 1);
    blockFaces0 = calculateBlockPositions(bd, 0);
    blockFaces2 = calculateBlockPositions(bd, 2);
    blockFaces3 = calculateBlockPositions(bd, 3);
    blockFaces4 = calculateBlockPositions(bd, 4);
    fourRadFaces = {blockFaces0, blockFaces2, blockFaces3, blockFaces4};
}

/*
*   
*   Need to return <player x Pos>, <player y Pos>, <player z Pos>, <player ang>, <player x velo>, <player y velo>, <player z velo>,
*   <player ang change>, <player x accel>, <player y accel>, <player z accel>, <10 closest blocks>
*
*/




/*  6 faces to calculate positions for:
*   face 0 -> face facing player when spawned
*   face 1 -> face facing upward
*   face 2 -> face facing player when player turns right
*   face 3 -> face facing players when player turns left
*   face 4 -> face facing player when turning 180 degrees
*   face 5 -> face facing player when player looks up (unused in this case)
*/
vector<vector<vector<double>>> CalculateInputs::calculateBlockPositions(vector<vector<vector<double>>> blocks, int face){
//  assumes positive x is vector from start to end
//  assumes positive z is vector from player to sky
//  assumes positive y is vector from player left to player right (mult by -1 if not the case)
    vector<vector<vector<double>>> retval;
    if(face == 0){
        for(int i = 0; i < bd.size(); i++){
            vector<double> vertex1, vertex2, vertex3, vertex4;
            vertex1 = {bd[i][0][0] - (bd[i][1][0] / 2), bd[i][0][1] - (bd[i][0][1] / 2), bd[i][0][2] - (bd[i][1][2] / 2)};
            vertex2 = {bd[i][0][0] - (bd[i][1][0] / 2), bd[i][0][1] + (bd[i][0][1] / 2), bd[i][0][2] - (bd[i][1][2] / 2)};
            vertex3 = {bd[i][0][0] - (bd[i][1][0] / 2), bd[i][0][1] - (bd[i][0][1] / 2), bd[i][0][2] + (bd[i][1][2] / 2)};
            vertex4 = {bd[i][0][0] - (bd[i][1][0] / 2), bd[i][0][1] + (bd[i][0][1] / 2), bd[i][0][2] + (bd[i][1][2] / 2)};
            retval.push_back({vertex1, vertex2, vertex3, vertex4});
        }
    }else if(face == 1){

        for(int i = 0; i < bd.size(); i++){
            vector<double> vertex1, vertex2, vertex3, vertex4;
            vertex1 = {bd[i][0][0] - (bd[i][1][0] / 2), bd[i][0][1] - (bd[i][0][1] / 2), bd[i][0][2] + (bd[i][1][2] / 2)};
            vertex2 = {bd[i][0][0] - (bd[i][1][0] / 2), bd[i][0][1] + (bd[i][0][1] / 2), bd[i][0][2] + (bd[i][1][2] / 2)};
            vertex3 = {bd[i][0][0] + (bd[i][1][0] / 2), bd[i][0][1] - (bd[i][0][1] / 2), bd[i][0][2] + (bd[i][1][2] / 2)};
            vertex4 = {bd[i][0][0] + (bd[i][1][0] / 2), bd[i][0][1] + (bd[i][0][1] / 2), bd[i][0][2] + (bd[i][1][2] / 2)};
            retval.push_back({vertex1, vertex2, vertex3, vertex4});
        }

        
    }else if(face == 2){
        for(int i = 0; i < bd.size(); i++){
            vector<double> vertex1, vertex2, vertex3, vertex4;
            vertex1 = {bd[i][0][0] + (bd[i][1][0] / 2), bd[i][0][1] - (bd[i][0][1] / 2), bd[i][0][2] - (bd[i][1][2] / 2)};
            vertex2 = {bd[i][0][0] - (bd[i][1][0] / 2), bd[i][0][1] - (bd[i][0][1] / 2), bd[i][0][2] - (bd[i][1][2] / 2)};
            vertex3 = {bd[i][0][0] + (bd[i][1][0] / 2), bd[i][0][1] - (bd[i][0][1] / 2), bd[i][0][2] + (bd[i][1][2] / 2)};
            vertex4 = {bd[i][0][0] - (bd[i][1][0] / 2), bd[i][0][1] - (bd[i][0][1] / 2), bd[i][0][2] + (bd[i][1][2] / 2)};
            retval.push_back({vertex1, vertex2, vertex3, vertex4});
        }
    }else if(face == 3){
        for(int i = 0; i < bd.size(); i++){
            vector<double> vertex1, vertex2, vertex3, vertex4;
            vertex1 = {bd[i][0][0] - (bd[i][1][0] / 2), bd[i][0][1] + (bd[i][0][1] / 2), bd[i][0][2] - (bd[i][1][2] / 2)};
            vertex2 = {bd[i][0][0] + (bd[i][1][0] / 2), bd[i][0][1] + (bd[i][0][1] / 2), bd[i][0][2] - (bd[i][1][2] / 2)};
            vertex3 = {bd[i][0][0] - (bd[i][1][0] / 2), bd[i][0][1] + (bd[i][0][1] / 2), bd[i][0][2] + (bd[i][1][2] / 2)};
            vertex4 = {bd[i][0][0] + (bd[i][1][0] / 2), bd[i][0][1] + (bd[i][0][1] / 2), bd[i][0][2] + (bd[i][1][2] / 2)};
            retval.push_back({vertex1, vertex2, vertex3, vertex4});
        }
    }else if(face ==4 ){
        for(int i = 0; i < bd.size(); i++){
            vector<double> vertex1, vertex2, vertex3, vertex4;
            vertex1 = {bd[i][0][0] + (bd[i][1][0] / 2), bd[i][0][1] + (bd[i][0][1] / 2), bd[i][0][2] - (bd[i][1][2] / 2)};
            vertex2 = {bd[i][0][0] + (bd[i][1][0] / 2), bd[i][0][1] - (bd[i][0][1] / 2), bd[i][0][2] - (bd[i][1][2] / 2)};
            vertex3 = {bd[i][0][0] + (bd[i][1][0] / 2), bd[i][0][1] + (bd[i][0][1] / 2), bd[i][0][2] + (bd[i][1][2] / 2)};
            vertex4 = {bd[i][0][0] + (bd[i][1][0] / 2), bd[i][0][1] - (bd[i][0][1] / 2), bd[i][0][2] + (bd[i][1][2] / 2)};
            retval.push_back({vertex1, vertex2, vertex3, vertex4});
        }
    }/*else if(face == 5){
        unused for now
    }*/

    return retval;

}


vector<double> CalculateInputs::GetNNInputs(vector<double> playerPos, double dt){
    int numBlocksToFind = 2;
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

    vector<double> distFromFaces(10);
    
    calculateRadialDistances(playerPos, distFromFaces);

    return retval;


}

vector<vector<vector<double>>> CalculateInputs::pruneFacesByZValue(double zVal){
    vector<vector<vector<double>>> retval; //valid faces
    
    for(int i = 0; i < this->fourRadFaces.size(); i++){
        for(int j = 0; j < fourRadFaces[i].size(); j++){
            if(zVal > fourRadFaces[i][j][0][2] && zVal < fourRadFaces[i][j][2][2]){
                retval.push_back(fourRadFaces[i][j]);
            }
        }
    }
    return retval;

}


pair<double, double> CalculateInputs::calculateMB(vector<vector<double>> face){
    double m;
    double b;
    m = (face[0][0] - face[1][0]) / (face[0][1] - face[1][1]);
    b = face[0][1] - (face[0][0] * m);
    return {m, b};
}


double CalculateInputs::findDist(vector<vector<double>> face, vector<double>& playerPos, double ang){
    pair<double, double> mb = calculateMB(face);

    double angSlope = tan(ang - playerPos[3]);
    if(ang == mb.first){return 1000000000.0;}

    double xIntersect = mb.second / ((mb.first) - angSlope);
    double yIntersect = angSlope * xIntersect;

    double distance = sqrt(pow(yIntersect, 2) + pow(xIntersect, 2));

    double xLarge = max(face[0][1], face[1][1]);
    double yLarge = max(face[0][0], face[1][0]);
    double xMin = min(face[0][1], face[1][1]);
    double yMin = min(face[0][0], face[1][0]);

    if(xIntersect <= xLarge && xIntersect >= xMin && yIntersect <= yLarge && yIntersect >= yMin){
        return distance;
    }
    return 1000000000.0;
}

void CalculateInputs::calculateRadialDistances(vector<double>& playerPos, vector<double>& distFromFaces){
    
    vector<vector<vector<double>>> validFaces = pruneFacesByZValue(playerPos[2]);
    double curAng = playerPos[3];
    for(int i = 0; i < 10; i++){
        double curDist = 1000000000.0;
        for(int j = 0 ; j < validFaces.size(); j++){
            double testDist = findDist(validFaces[j], playerPos, curAng);
            curDist = testDist < curDist ? testDist : curDist;
        }
        distFromFaces[i] = curDist;
        curAng += 36;
    }

}



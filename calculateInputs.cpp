#include "calculateInputs.h"

using namespace std;

CalculateInputs::CalculateInputs(){
    //x is from start looking to end, y is left right, z is up down (as defined by the map, x y z arbitrary)

    //INVERTED THE Y VALUES ON ONLY THE 4 WALLS
    bd = {{{-320, 160, 32}, {1024, 1088, 64}}, {{-1184, -128, 64}, {64, 1792, 640}}, {{2144, -1024, 64}, {6720, 128, 640}},
    {{2112, 736, 64}, {6528, 64, 640}}, 
    {{5344, -96, 0}, {64, 1856, 768}},
    {{2368, -512, 128}, {64, 384, 576}},
    {{3180, -151.962, 80}, {64, 384, 608}},

    {{352, -96, 32},{192, 192, 64}},
    {{512, 160, 32},{128, 192, 64}},
    {{704, 128, 32},{128, 128, 64}},
    {{864, 352, 32},{64, 64, 64}},
    {{864, -128, 32},{192, 128, 64}},
    {{1152, 128, 32},{128, 128, 64}},
    {{1312, 352, 32},{64, 64, 64}},
    {{1440, -128, 32},{192, 192, 64}},
    {{1568, 160, 32},{64, 64, 64}},
    {{1632, 544, 32},{64, 64, 64}},
    {{1792, 256, 32},{128, 128, 64}},
    {{1856, -192, 32},{128, 128, 64}},
    {{1920, 512, 32},{128, 128, 64}},
    {{2112, 160, 32},{256, 192, 64}},
    {{2144, 608, 32},{64, 64, 64}},
    {{2400, 288, 32},{64, 64, 64}},
    {{2592, 640, 32},{64, 128, 64}},
    {{2848, 192, 32},{320, 256, 64}},
    {{2880, -256, 32},{128, 128, 64}},
    {{3200, 640, 32},{256, 128, 64}},
    {{3264, -224, 32},{256, 192, 64}},
    {{3488, 128, 32},{192, 256, 64}},
    {{2680, 608, 32},{64, 64, 64}},
    {{4480, 0, 32},{1344, 1472, 64}}
    };
    
    
     //, {{2500, 500, 32}, {5000, 100, 200}}};
    //bd = {{{-320, 160, 32}, {1024, 1088, 64}}, {{352, -92, 32},{192, 192, 64}}, {{512, 160, 32},{128, 192, 64}}, {{704, 128, 32},{128, 128, 64}}, {{864, -128, 32},{}}, {{},{}}, };

    prevYPos = prevXPos = prevZPos = prevXSpeed = prevYSpeed = prevZSpeed = prevAng = 0;
    allBlocks = calculateBlockPositions(1);
    blockFaces0 = calculateBlockPositions(0);
    blockFaces2 = calculateBlockPositions(2);
    blockFaces3 = calculateBlockPositions(3);
    blockFaces4 = calculateBlockPositions(4);
    for(int i = 0; i < blockFaces0.size(); i++){
        fourRadFaces.push_back(blockFaces0[i]);
    }
    for(int i = 0; i < blockFaces2.size(); i++){
        fourRadFaces.push_back(blockFaces2[i]);
    }
    for(int i = 0; i < blockFaces3.size(); i++){
        fourRadFaces.push_back(blockFaces3[i]);
    }
    for(int i = 0; i < blockFaces4.size(); i++){
        fourRadFaces.push_back(blockFaces4[i]);
    }
    //fourRadFaces = {blockFaces0, blockFaces2, blockFaces3, blockFaces4};
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
vector<vector<vector<double>>> CalculateInputs::calculateBlockPositions(int face){
//  assumes positive x is vector from start to end
//  assumes positive z is vector from player to sky
//  assumes positive y is vector from player left to player right (mult by -1 if not the case)
    vector<vector<vector<double>>> retval;
    if(face == 0){
        for(int i = 0; i < bd.size(); i++){
            vector<double> vertex1, vertex2, vertex3, vertex4;
            vertex1 = {bd[i][0][0] - (bd[i][1][0] / 2), bd[i][0][1] - (bd[i][1][1] / 2), bd[i][0][2] - (bd[i][1][2] / 2)};
            vertex2 = {bd[i][0][0] - (bd[i][1][0] / 2), bd[i][0][1] + (bd[i][1][1] / 2), bd[i][0][2] - (bd[i][1][2] / 2)};
            vertex3 = {bd[i][0][0] - (bd[i][1][0] / 2), bd[i][0][1] - (bd[i][1][1] / 2), bd[i][0][2] + (bd[i][1][2] / 2)};
            vertex4 = {bd[i][0][0] - (bd[i][1][0] / 2), bd[i][0][1] + (bd[i][1][1] / 2), bd[i][0][2] + (bd[i][1][2] / 2)};
            retval.push_back({vertex1, vertex2, vertex3, vertex4});
        }
    }else if(face == 1){

        for(int i = 0; i < bd.size(); i++){
            vector<double> vertex1, vertex2, vertex3, vertex4;
            vertex1 = {bd[i][0][0] - (bd[i][1][0] / 2), bd[i][0][1] - (bd[i][1][1] / 2), bd[i][0][2] + (bd[i][1][2] / 2)};
            vertex2 = {bd[i][0][0] - (bd[i][1][0] / 2), bd[i][0][1] + (bd[i][1][1] / 2), bd[i][0][2] + (bd[i][1][2] / 2)};
            vertex3 = {bd[i][0][0] + (bd[i][1][0] / 2), bd[i][0][1] - (bd[i][1][1] / 2), bd[i][0][2] + (bd[i][1][2] / 2)};
            vertex4 = {bd[i][0][0] + (bd[i][1][0] / 2), bd[i][0][1] + (bd[i][1][1] / 2), bd[i][0][2] + (bd[i][1][2] / 2)};
            retval.push_back({vertex1, vertex2, vertex3, vertex4});
        }

        
    }else if(face == 2){
        for(int i = 0; i < bd.size(); i++){
            vector<double> vertex1, vertex2, vertex3, vertex4;
            vertex1 = {bd[i][0][0] + (bd[i][1][0] / 2), bd[i][0][1] - (bd[i][1][1] / 2), bd[i][0][2] - (bd[i][1][2] / 2)};
            vertex2 = {bd[i][0][0] - (bd[i][1][0] / 2), bd[i][0][1] - (bd[i][1][1] / 2), bd[i][0][2] - (bd[i][1][2] / 2)};
            vertex3 = {bd[i][0][0] + (bd[i][1][0] / 2), bd[i][0][1] - (bd[i][1][1] / 2), bd[i][0][2] + (bd[i][1][2] / 2)};
            vertex4 = {bd[i][0][0] - (bd[i][1][0] / 2), bd[i][0][1] - (bd[i][1][1] / 2), bd[i][0][2] + (bd[i][1][2] / 2)};
            retval.push_back({vertex1, vertex2, vertex3, vertex4});
        }
    }else if(face == 3){
        for(int i = 0; i < bd.size(); i++){
            vector<double> vertex1, vertex2, vertex3, vertex4;
            vertex1 = {bd[i][0][0] - (bd[i][1][0] / 2), bd[i][0][1] + (bd[i][1][1] / 2), bd[i][0][2] - (bd[i][1][2] / 2)};
            vertex2 = {bd[i][0][0] + (bd[i][1][0] / 2), bd[i][0][1] + (bd[i][1][1] / 2), bd[i][0][2] - (bd[i][1][2] / 2)};
            vertex3 = {bd[i][0][0] - (bd[i][1][0] / 2), bd[i][0][1] + (bd[i][1][1] / 2), bd[i][0][2] + (bd[i][1][2] / 2)};
            vertex4 = {bd[i][0][0] + (bd[i][1][0] / 2), bd[i][0][1] + (bd[i][1][1] / 2), bd[i][0][2] + (bd[i][1][2] / 2)};
            retval.push_back({vertex1, vertex2, vertex3, vertex4});
        }
    }else if(face ==4 ){
        for(int i = 0; i < bd.size(); i++){
            vector<double> vertex1, vertex2, vertex3, vertex4;
            vertex1 = {bd[i][0][0] + (bd[i][1][0] / 2), bd[i][0][1] + (bd[i][1][1] / 2), bd[i][0][2] - (bd[i][1][2] / 2)};
            vertex2 = {bd[i][0][0] + (bd[i][1][0] / 2), bd[i][0][1] - (bd[i][1][1] / 2), bd[i][0][2] - (bd[i][1][2] / 2)};
            vertex3 = {bd[i][0][0] + (bd[i][1][0] / 2), bd[i][0][1] + (bd[i][1][1] / 2), bd[i][0][2] + (bd[i][1][2] / 2)};
            vertex4 = {bd[i][0][0] + (bd[i][1][0] / 2), bd[i][0][1] - (bd[i][1][1] / 2), bd[i][0][2] + (bd[i][1][2] / 2)};
            retval.push_back({vertex1, vertex2, vertex3, vertex4});
        }
    }/*else if(face == 5){
        unused for now
    }*/

    return retval;

}


vector<double> CalculateInputs::GetNNInputs(vector<double>& playerPos, double dt){
    int numBlocksToFind = 2;
    vector<int> dists(numBlocksToFind);
    vector<double> retval(numBlocksToFind * 3);
    priority_queue<pair<double, int>> closest;
    double xSpeed, ySpeed, zSpeed, xAccel, yAccel, zAccel, angChange, smallestDist;


    //calculate the numbBlocksToFind closest blocks, store their x, y, z in double vector of doubles
    for(int i = 0; i < allBlocks.size(); i++){

        smallestDist = 1000000000.0;
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

    for(int i = 0; i < distFromFaces.size(); i++){
        retval.push_back(distFromFaces[i]);
    }

    return retval;


}

vector<vector<vector<double>>> CalculateInputs::pruneFacesByZValue(double zVal){
    vector<vector<vector<double>>> retval; //valid faces
    

    for(int j = 0; j < fourRadFaces.size(); j++){
        if(zVal > fourRadFaces[j][0][2] && zVal < fourRadFaces[j][2][2]){
            retval.push_back(fourRadFaces[j]);
        }
    }

    return retval;

}

//y in line formula is x recieved value
pair<double, double> CalculateInputs::calculateMB(vector<vector<double>>& face, vector<double>& playerPos){
    double m;
    double b;
    m = (face[0][0] - face[1][0]) / (face[0][1] - face[1][1]);
    if(m == INFINITY || m == -INFINITY){
        b = m == INFINITY ? m : -INFINITY;
    }
    else if(m == 0){
        b = face[0][0] - playerPos[0];
    }
    else{
        b = (face[0][0] - playerPos[0]) - ((face[0][1] - (playerPos[1] * -1)) * m);
    }
    return {m, b};
}


double CalculateInputs::findDist(vector<vector<double>>& face, vector<double>& playerPos, double ang){
    pair<double, double> mb = calculateMB(face, playerPos);
    double xIntersect, yIntersect;
    double angSlope = tan((3.14159 / 180.0) * ang);
    //angSlope = angSlope * (180.0 / 3.14159);
    if(angSlope == mb.first){return INFINITY;}

    if(mb.first == INFINITY || mb.first == -INFINITY){
        xIntersect = face[0][1] - (-1 * playerPos[1]);
        yIntersect = angSlope * (xIntersect);
    }
    else if(mb.first == 0){
        //cout << "Here\n";
        yIntersect = face[0][0] - (playerPos[0]);
        xIntersect = yIntersect / angSlope;
        // << yIntersect << endl <<  endl;
    }else{
        xIntersect = mb.second / ((mb.first) - angSlope);
        yIntersect = angSlope * xIntersect;
    }

    double distance = sqrt(pow(yIntersect, 2) + pow(xIntersect, 2));

    double xLarge = max(face[0][1] - (playerPos[1] * -1), face[1][1] - (playerPos[1] * -1));
    //cout << face[0][1] << " " << face[1][1] << endl;
    double yLarge = max(face[0][0] - playerPos[0], face[1][0] - playerPos[0]);
    double xMin = min(face[0][1] - (playerPos[1] * -1), face[1][1] - (playerPos[1] * -1));
    double yMin = min(face[0][0] - playerPos[0], face[1][0] - playerPos[0]);

    //cout << "xint, yint, xlarge, xmin, ylarge, ymin " << xIntersect << " " << yIntersect << " " << xLarge << " " << xMin << " " << yLarge << " " << yMin << endl;
    //cout << "m tan(phi) " << mb.first << " " << angSlope << endl;
    if(xIntersect <= xLarge && xIntersect >= xMin && yIntersect <= yLarge && yIntersect >= yMin){
        double tAng = ang;
        
       // cout << endl << endl;
        //cout << ang << endl;
        //cout << xIntersect << " " << yIntersect << endl;

        if(tAng >= 0 && tAng <= 90){
            if(xIntersect < 0 || yIntersect < 0){return INFINITY;}
        }else if(tAng > 90 && tAng <= 180){
            if(xIntersect > 0 || yIntersect < 0){return INFINITY;}
        }else if(tAng > 180 && tAng <= 270){
            if(xIntersect > 0 || yIntersect > 0){return INFINITY;}
        }else{
            if(xIntersect < 0 || yIntersect > 0){return INFINITY;}
        }
        return distance;
    }
    return INFINITY;
}

void CalculateInputs::calculateRadialDistances(vector<double>& playerPos, vector<double>& distFromFaces){
    
    vector<vector<vector<double>>> validFaces = pruneFacesByZValue(playerPos[2]);


    double normPlayerAng = playerPos[3];
    if(normPlayerAng >= -180 && normPlayerAng <= -90){
        normPlayerAng += 450;
    }else{
        normPlayerAng += 90;
    }
    //need to edit how to add angle, starts from -180 -> +180 in game
    double curDist = INFINITY;
    double testDist;
    for(int i = 0; i < 10; i++){
        curDist = INFINITY;
        for(int j = 0; j < validFaces.size(); j++){
            testDist = findDist(validFaces[j], playerPos, normPlayerAng);
            curDist = testDist < curDist ? testDist : curDist;
        }
        distFromFaces[i] = curDist;
        normPlayerAng += 36;
        normPlayerAng = fmod(normPlayerAng, 360.0);
    }
    //Sleep(1000000);

}



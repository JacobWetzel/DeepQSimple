#include "calculateInputs.h"

using namespace std;

CalculateInputs::CalculateInputs(vector<double>& posAng){
    //x is from start looking to end, y is left right, z is up down (as defined by the map, x y z arbitrary)

    //INVERTED THE Y VALUES ON ONLY THE 4 WALLS (THEY ARE NOT AS OF 11/14/24)
     /*bd = {{{-1184.0, 128.0, 64.0}, {64.0, 1792.0, 640.0}}, 
    {{2144.0, 1024.0, 64.0}, {6720.0, 128.0, 640.0}},
    {{2112.0, -736.0, 64.0}, {6528.0, 64.0, 640.0}}, 
    {{5344.0, 96.0, 0.0}, {64.0, 1856.0, 768.0}},
    {{2368.0, 512.0, 128.0}, {64.0, 384.0, 576.0}},
    {{3180.0, 151.962, 80.0}, {64.0, 384.0, 608.0}},

    {{-320.0, 160.0, 32.0}, {1024.0, 1088.0, 64.0}},
    {{352.0, -96.0, 32.0},{192.0, 192.0, 64.0}},
    {{337.875, 160.0, 32.0}, {92.25, 192.0, 64.0}},
    {{512.0, 160.0, 32.0},{128.0, 192.0, 64.0}},
    {{596.79, -103.881, 32}, {128, 79.76, 64.0}},
    {{704.0, 128.0, 32.0},{128.0, 128.0, 64.0}},
    {{832.0, 320.0, 32.0},{128.0, 128.0, 64.0}},
    {{864.0, -128.0, 32.0},{192.0, 128.0, 64.0}},
    {{1120.0, 320.0, 32.0}, {192.0, 128.0, 64.0}},
    {{1152.0, 128.0, 32.0},{128.0, 128.0, 64.0}},
    {{1312.0, 352.0, 32.0},{64.0, 64.0, 64.0}},
    {{1440.0, -128.0, 32.0},{192.0, 192.0, 64.0}},
    {{1536.0, 224.0, 32.0},{128.0, 192.0, 64.0}},
    {{1632.0, 544.0, 32.0},{64.0, 64.0, 64.0}},
    {{1792.0, 256.0, 32.0},{128.0, 128.0, 64.0}},
    {{1856.0, -192.0, 32.0},{128.0, 128.0, 64.0}},
    {{1920.0, 512.0, 32.0},{128.0, 128.0, 64.0}},
    {{2112.0, 160.0, 32.0},{256.0, 192.0, 64.0}},
    {{2144.0, 608.0, 32.0},{64.0, 64.0, 64.0}},
    {{2400.0, 288.0, 32.0},{64.0, 64.0, 64.0}},
    {{2592.0, 640.0, 32.0},{64.0, 128.0, 64.0}},
    {{2848.0, 192.0, 32.0},{320.0, 256.0, 64.0}},
    {{2880.0, -256.0, 32.0},{128.0, 128.0, 64.0}},
    {{3200.0, 640.0, 32.0},{256.0, 128.0, 64.0}},
    {{3264.0, -224.0, 32.0},{256.0, 192.0, 64.0}},
    {{3488.0, 128.0, 32.0},{192.0, 256.0, 64.0}},
    {{3680.0, 608.0, 32.0},{64.0, 64.0, 64.0}},
    {{4480.0, 0.0, 32.0},{1344.0, 1472.0, 64.0}}
    };*/

    // {{, , 32.0}, {192.0, 64.0, 64.0}}, (long skinny rect)
    // {{, , 32.0}, {128.0, 128.0, 64.0}}, (square)
    // {{, , 32.0}, {128.0, 192.0, 64.0}}, (wide rect)
    // {{, , 32.0}, {64.0, 64.0, 64.0}}, (cube)
    // {{, , 32.0}, {192.0, 128.0, 64.0}}, (long rect)
    // {{, , 32.0}, {64.0, 192.0, 64.0}}, (wide skinny rect)
    // {{, , 32.0}, {192.0, 192.0, 64.0}}, (big square)

    bd = {
        {{-1184.0, 0.0, 64.0}, {64.0, 1792.0, 640.0}},  
        {{2144.0, 896.0, 64.0}, {6720.0, 128.0, 640.0}},
        {{2112.0, -864.0, 64.0}, {6528.0, 64.0, 640.0}},
        {{5344.0, -32.0, 0.0}, {64.0, 1856.0, 768.0}},  
        {{2368.0, 384.0, 128.0}, {64.0, 384.0, 576.0}},
        {{3180.0, 23.96, 80.0}, {64.0, 384.0, 608.0}},

        {{-320.0, 32.0, 32.0}, {1024.0, 1088.0, 64.0}},
        {{288.279, -753.053, 32.0}, {64.0, 192.0, 64.0}},
        {{320.0, -527.27, 32.0}, {128.0, 158.75, 64.0}},
        {{320.0, 256.0, 32.0}, {128.0, 128.0, 64.0}},
        {{352.0, -224.0, 32.0}, {192.0, 192.0, 64.0}},
        {{352.0, 473.21, 32.0}, {192.0, 128.0, 64.0}}, 
        {{337.875, 32.0, 32.0}, {92.25, 192.0, 64.0}},
        {{480.0, -805.388, 32.0}, {192.0, 192.0, 64.0}},
        {{512.0, 32.0, 32.0}, {128.0, 192.0, 64.0}},
        {{576.0, -506.814, 32.0}, {128.0, 266.37, 64.0}},
        {{596.79, -231.881, 32.0}, {128, 79.76, 64.0}}, 
        {{608.0, 288.0, 32.0}, {192.0, 64.0, 64.0}},
        {{704.0, 0.0, 32.0}, {128.0, 128.0, 64.0}},
        {{832.0, 192.0, 32.0}, {128.0, 128.0, 64.0}},
        {{864.0, -256.0, 32.0}, {192.0, 128.0, 64.0}},
        {{1120.0, 192.0, 32.0}, {192.0, 128.0, 64.0}},
        {{1152.0, 0.0, 32.0}, {128.0, 128.0, 64.0}},
        {{1312.0, 224.0, 32.0}, {64.0, 64.0, 64.0}},
        {{1440.0, -256.0, 32.0}, {192.0, 192.0, 64.0}},
        {{1536.0, 96.0, 32.0}, {128.0, 192.0, 64.0}},
        {{1632.0, 416.0, 32.0}, {64.0, 64.0, 64.0}},
        {{1792.0, 128.0, 32.0}, {128.0, 128.0, 64.0}},
        {{1856.0, -320.0, 32.0}, {128.0, 128.0, 64.0}},
        {{1920.0, 384.0, 32.0}, {128.0, 128.0, 64.0}},
        {{2112.0, 32.0, 32.0}, {256.0, 192.0, 64.0}},
        {{2144.0, 480.0, 32.0}, {64.0, 64.0, 64.0}},
        {{2400.0, 160.0, 32.0}, {64.0, 64.0, 64.0}},
        {{2592.0, 512.0, 32.0}, {64.0, 128.0, 64.0}},
        {{2848.0, 64.0, 32.0}, {320.0, 256.0, 64.0}},
        {{2880.0, -384.0, 32.0}, {128.0, 128.0, 64.0}},
        {{3200.0, 512.0, 32.0}, {256.0, 128.0, 64.0}},
        {{3264.0, -352.0, 32.0}, {256.0, 192.0, 64.0}},
        {{3488.0, 0.0, 32.0}, {192.0, 256.0, 64.0}},
        {{3680.0, 480.0, 32.0}, {64.0, 64.0, 64.0}},
        {{4480.0, -128.0, 32.0}, {1344.0, 1472.0, 64.0}}
    };


    
    // {{{xi, yi, zi}, {xj, yj, zj}, {xk, yk, zk}, {xu, yu, zu}}, . . . }
    
     //, {{2500, 500, 32}, {5000, 100, 200}}};
    //bd = {{{-320, 160, 32}, {1024, 1088, 64}}, {{352, -92, 32},{192, 192, 64}}, {{512, 160, 32},{128, 192, 64}}, {{704, 128, 32},{128, 128, 64}}, {{864, -128, 32},{}}, {{},{}}, };
    prevXPos = posAng[0]; prevYPos = posAng[1]; prevZPos = posAng[2]; prevAng = posAng[3];
    prevXSpeed = prevYSpeed = prevZSpeed = prevAng = 0;
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
    sortedTopFaces = allBlocks;
    sort(sortedTopFaces.begin(), sortedTopFaces.end(), [](vector<vector<double>>& a, vector<vector<double>>& b){
        return a[0][0] < b[0][0];
    });
    furthestXPos = 24;
    curBlock = curBlockStart(posAng);
    cout << "curBlock = " << curBlock << endl;
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
    int numBlocksToFind = 6;
    vector<int> dists(numBlocksToFind);
    vector<double> retval;//(numBlocksToFind * 3);
    priority_queue<pair<double, int>> closest;
    double xSpeed, ySpeed, zSpeed, xAccel, yAccel, zAccel, angChange, smallestDist;
    


    //calculate the numbBlocksToFind closest blocks, store their x, y, z in double vector of doubles


    int overBlock = isOnBlock(playerPos);

    vector<double> firstBlockReport(5, 0.0);

    if(overBlock != -1){
        firstBlockReport[0] = bd[overBlock][0][0] - playerPos[0];
        firstBlockReport[1] = bd[overBlock][0][1] - playerPos[1];
        firstBlockReport[2] = bd[overBlock][0][2] - playerPos[2];
        firstBlockReport[3] = bd[overBlock][1][0];
        firstBlockReport[4] = bd[overBlock][1][1];
    }

    for(int i = 0; i < allBlocks.size(); i++){
        if(playerPos[0] <= allBlocks[i][0][0]) {
            smallestDist = 1000000000.0;
            //for(int j = 0; j < allBlocks[i].size(); j++){ 
            smallestDist = min(smallestDist, abs(playerPos[0] - bd[i][0][0]) + abs(playerPos[1] - bd[i][0][1]) + abs(playerPos[2] - bd[i][0][2]));
            //}
            if(closest.size() < numBlocksToFind){
                closest.push({smallestDist, i});
            }else{
                if(closest.top().first >= smallestDist){
                    closest.pop();
                    closest.push({smallestDist, i});
                }
            }
        }
    }
    priority_queue<pair<double, int>> topsLeftQ;

    if(closest.size() < numBlocksToFind){
        int topsLeft = numBlocksToFind - closest.size();
        for(int i = 6; i < allBlocks.size(); i++){
            if(playerPos[0] > allBlocks[i][0][0] && overBlock != i) {
                smallestDist = 1000000000.0;
                //for(int j = 0; j < allBlocks[i].size(); j++){ 
                smallestDist = min(smallestDist, abs(playerPos[0] - bd[i][0][0]) + abs(playerPos[1] - bd[i][0][1]) + abs(playerPos[2] - bd[i][0][2]));
                //}
                if(topsLeftQ.size() < topsLeft){
                    topsLeftQ.push({smallestDist, i});
                }else{
                    if(topsLeftQ.top().first >= smallestDist){
                        topsLeftQ.pop();
                        topsLeftQ.push({smallestDist, i});
                    }
                }
            }
        }
    }

    int itr = closest.size() - 1;
    while(closest.size() > 0){
        dists[itr] = closest.top().second;
        closest.pop();
        itr--;
    }
    itr = numBlocksToFind-1;
    while(topsLeftQ.size() > 0){
        dists[itr] = topsLeftQ.top().second;
        topsLeftQ.pop();
        itr--;
    }
    

    // calculating the speed, theta change, and accel since previous timestep
    dt = .05;
    xSpeed = (playerPos[0] - prevXPos) / (dt);
    ySpeed = ((playerPos[1] - prevYPos) / ((dt)));
    zSpeed = ((playerPos[2] - prevZPos) / (dt));
    xAccel = (xSpeed - prevXSpeed) / ((dt));
    yAccel = (ySpeed - prevYSpeed) / ((dt));
    zAccel = (zSpeed - prevZSpeed) / ((dt));
    angChange = (playerPos[3] - prevAng);

    //cout << "xspeed " << xSpeed << endl;
    //printf("%lf x\n", xSpeed);
    //update variables to have up to date prev-values
    prevXPos = playerPos[0];
    prevYPos = playerPos[1];
    prevZPos = playerPos[2];
    prevAng = playerPos[3];
    prevXSpeed = xSpeed;
    prevYSpeed = ySpeed;
    prevZSpeed = zSpeed;

    retval = {playerPos[0], playerPos[1], playerPos[2], playerPos[3], xSpeed, ySpeed, zSpeed, angChange, xAccel, yAccel, zAccel};
    retval.push_back(firstBlockReport[0]);
    retval.push_back(firstBlockReport[1]);
    retval.push_back(firstBlockReport[2]);
    retval.push_back(firstBlockReport[3]);
    retval.push_back(firstBlockReport[4]);

    for(int i = 0; i < dists.size(); i++){
        retval.push_back(playerPos[0] - bd[dists[i]][0][0]);
        retval.push_back(playerPos[1] - bd[dists[i]][0][1]);
        retval.push_back(playerPos[2] - bd[dists[i]][0][2]);
        retval.push_back(bd[dists[i]][1][0]);
        retval.push_back(bd[dists[i]][1][1]);

        /*
        for(int j = 0; j < allBlocks[dists[i]].size(); j++){
        for(int k = 0; k < 4; k++){
        retval.push_back(allBlocks[dists[i]][k][0]);
        retval.push_back(allBlocks[dists[i]][k][1]);
        retval.push_back(allBlocks[dists[i]][k][2]);
        }
        }*/
    }

    vector<double> distFromFaces(10);
    
    calculateRadialDistances(playerPos, distFromFaces);

    for(int i = 0; i < distFromFaces.size(); i++){
        retval.push_back(distFromFaces[i]);
    }

    if(zAccel > 0 && playerPos[0] > furthestXPos && playerPos[0] > 192){
        for(int i = 4; i < sortedTopFaces.size(); i++){
            if(isInBlock(playerPos, i) && i > curBlock){
                //cout << "in block i: " << i << " " << sortedTopFaces[i][0][0] << " " << sortedTopFaces[i][0][1] << " " << sortedTopFaces[i][3][0] << " " << sortedTopFaces[i][3][1] << endl;
                curBlock = i;
                furthestXPos == playerPos[0];
                retval.push_back(1);
                return retval;
            }
        }
    }

    retval.push_back(0);
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
    m = (face[0][0] - face[1][0]) / (-1 * (face[0][1] - face[1][1]));
    if(m == INFINITY || m == -INFINITY){
        b = m == INFINITY ? m : -INFINITY;
    }
    else if(m == 0){
        b = face[0][0] - playerPos[0];
    }
    else{
        b = (face[0][0] - playerPos[0]) - (((face[0][1] * -1) - (playerPos[1] * -1)) * m);
        //b = (face[0][0] - playerPos[0]) - ((face[0][1] - (playerPos[1])) * m);   
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
        xIntersect = (face[0][1] * -1) - (-1 * playerPos[1]);
        //xIntersect = face[0][1] - (playerPos[1]);
        
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

    double xLarge = max((face[0][1] * -1) - (playerPos[1] * -1), (face[1][1] * -1) - (playerPos[1] * -1));
    //double xLarge = max((face[0][1]) - (playerPos[1]), face[1][1] - (playerPos[1]));
    
    //cout << face[0][1] << " " << face[1][1] << endl;
    double yLarge = max(face[0][0] - playerPos[0], face[1][0] - playerPos[0]);
    
    double xMin = min((face[0][1] * -1) - (playerPos[1] * -1), (face[1][1] * -1) - (playerPos[1] * -1));
    //double xMin = min(face[0][1] - (playerPos[1]), face[1][1] - (playerPos[1]));
    
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

bool CalculateInputs::isInBlock(vector<double>& playerPos, int blockIndex){
    //cout << playerPos[0] << " " << sortedTopFaces[blockIndex][0][0] << " " << sortedTopFaces[blockIndex][2][0] << " " << " " << playerPos[1] << " " << sortedTopFaces[blockIndex][0][1] << " " << sortedTopFaces[blockIndex][1][1] << endl << endl;
    if(playerPos[0] + 13 >= sortedTopFaces[blockIndex][0][0] && playerPos[0] - 13 <= sortedTopFaces[blockIndex][2][0] && (playerPos[1] + 13) >= (sortedTopFaces[blockIndex][0][1]) && (playerPos[1] - 13 <= (sortedTopFaces[blockIndex][1][1]))){
        cout << "valid in block\n";
        return true;
    }
    return false;
}

int CalculateInputs::curBlockStart(vector<double>& playerPos){
    for(int i = 0; i < sortedTopFaces.size(); i++){
        if(playerPos[0] + 13 >= sortedTopFaces[i][0][0] && playerPos[0] - 13 <= sortedTopFaces[i][2][0] && (playerPos[1] + 13) >= (sortedTopFaces[i][0][1]) && (playerPos[1] - 13 <= (sortedTopFaces[i][1][1]))){
            return i;
        }
    }
    return -1;
}

int CalculateInputs::isOnBlock(vector<double>& playerPos){
    for(int i = 6; i < allBlocks.size(); i++){
        if(playerPos[0] + 13 >= allBlocks[i][0][0] && playerPos[0] - 13 <= allBlocks[i][2][0] && (playerPos[1] + 13) >= (allBlocks[i][0][1]) && (playerPos[1] - 13 <= (allBlocks[i][1][1]))){
            return i;
        }
    }
    return -1;
}


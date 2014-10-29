//
//  main.cpp
//  merge
//
//  Created by Tobias Hofmann on 15.04.14.
//  Copyright (c) 2014 Tobias Hofmann. All rights reserved.
//

#include <iostream>

#include <stdio.h>
#include <stdlib.h>


#include "matrix.h"

using namespace std;

int main(int argc, const char * argv[])
{
    srand ((unsigned int) time(NULL));
    
    Matrix myMatrix(4);
    myMatrix.insertRandomTileAtRandomSpot();
    
    int use_ai = 1;
    myMatrix.addNewTile = true;
    
    while(true) {
        cout << string(10, '\n');
        if (myMatrix.movePossible()) {
            //
        } else {
            if(myMatrix.freeSpotAvailable()) {
                //
            } else {
                break;
            }
        }
        
        if (myMatrix.addNewTile) myMatrix.insertRandomTileAtRandomSpot();
        
        myMatrix.print();
        
        //get movement
        switch (use_ai) {
            {
            case 0:
                system("stty raw");
                char input = 0;
                bool finished = false;
                while(!finished) {
                    input = getchar();
                    switch(input) {
                        case 97:
                            myMatrix.mergeLeft();
                            finished = true;
                            break;
                        case 119:
                            myMatrix.mergeUp();
                            finished = true;
                            break;
                        case 100:
                            myMatrix.mergeRight();
                            finished = true;
                            break;
                        case 115:
                            myMatrix.mergeDown();
                            finished = true;
                            break;
                    }
                }
                break;
                system("stty cooked");
            }
            {case 1:
                double randval = (double) rand()/RAND_MAX;
                if(randval < 0.33) {
                    myMatrix.mergeLeft();
                } else if (randval < 0.67) {
                    myMatrix.mergeRight();
                } else {
                    myMatrix.mergeDown();
                }
                break;}
            {case 2:
                int maxScore = -1;
                int move = -1;
                
                for(int i=0;i<4;i++) {
                    Matrix tmpMatrix(myMatrix);
                    switch (i) {
                        case 0:
                            tmpMatrix.mergeLeft();
                            break;
                        case 1:
                            tmpMatrix.mergeUp();
                            break;
                        case 2:
                            tmpMatrix.mergeRight();
                            break;
                        case 3:
                            tmpMatrix.mergeDown();
                            break;
                    }
                    if (tmpMatrix.score > maxScore) {
                        move = i;
                        maxScore = tmpMatrix.score;
                    }
                }
                switch (move) {
                    case 0:
                        myMatrix.mergeLeft();
                        break;
                    case 1:
                        myMatrix.mergeUp();
                        break;
                    case 2:
                        myMatrix.mergeRight();
                        break;
                    case 3:
                        myMatrix.mergeDown();
                        break;
                }
                break;}
        }
    }
    std::cout << "No free spot available" << std::endl;
    std::cout << "You are dead. Score = " << myMatrix.score << endl;
    return 0;
}
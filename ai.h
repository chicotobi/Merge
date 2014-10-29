//
//  ai.h
//  merge
//
//  Created by Tobias Hofmann on 28.06.14.
//  Copyright (c) 2014 Tobias Hofmann. All rights reserved.
//


#ifndef __merge__ai__
#define __merge__ai__

#include <cmath>
#include "Matrix.h"

class ai {
public:
    Matrix aiBaseMatrix;
    virtual Direction giveDirection() = 0;
    ai(Matrix& other) : aiBaseMatrix(other) {};
};

class userDirection : public ai {
public:
    userDirection(Matrix& other) : ai(other) {};
    Direction giveDirection() {
        system("stty raw");
        char input = 0;
        bool finished = false;
        while(!finished) {
            input = getchar();
            switch(input) {
                case 97:
                    return lefta;
                case 119:
                    return up;
                case 100:
                    return righta;
                case 115:
                    return down;
            }
        }
        system("stty cooked");
        return lefta;
    }
};

class randomDirection : public ai {
public:
    randomDirection(Matrix& other) : ai(other) {};
    Direction giveDirection() {
        double randval = (double) rand()/RAND_MAX;
        if ( aiBaseMatrix.directionPossible(lefta) || aiBaseMatrix.directionPossible(down) || aiBaseMatrix.directionPossible(righta) ) {
            if(randval < 0.4) {
                return lefta;
            } else if (randval < 0.6) {
                return righta;
            } else {
                return down;
            }
        } else {
            return up;
        }
    }
};

class scoreBased : public ai {
public:
    scoreBased(Matrix& other) : ai(other) {};
    Direction giveDirection() {
        int maxScore = -1;
        Direction maxDir = lefta;
        for(int dirInt = lefta; dirInt != 4; dirInt++) {
            Direction tmpDir = static_cast<Direction>(dirInt);
            Matrix tmpMatrix(aiBaseMatrix);
            if (tmpMatrix.directionPossible(tmpDir)) {
                tmpMatrix.merge(tmpDir);
                if (tmpMatrix.getScore() > maxScore ) {
                    maxDir = tmpDir;
                    maxScore = tmpMatrix.getScore();
                }
            }
        }
        return maxDir;
    }
};

class scoreBased2 : public ai {
public:
    scoreBased2(Matrix& other) : ai(other) {};
    Direction giveDirection() {
        int maxScore = -1;
        Direction maxDir = lefta;
        for(int dirInt = lefta; dirInt != 4; dirInt++) {
            Direction tmpDir = static_cast<Direction>(dirInt);
            Matrix tmpMatrix(aiBaseMatrix);
            int tmpScore = 0;
            cout << endl << "Direction " << tmpDir << ": ";
            if (tmpMatrix.directionPossible(tmpDir)) {
                tmpMatrix.merge(tmpDir);
                for(auto til = tmpMatrix.tiles.begin() ; til != tmpMatrix.tiles.end() ; til++) {
                    tmpScore += til->second.val * ( pow(til->first.x,4) + pow(til->first.y,2) );
                }
                cout << "Algo score is " << tmpScore;
                if (tmpScore > maxScore) {
                    maxDir = tmpDir;
                    maxScore = tmpScore;
                }
            } else {
                cout << "Not possible. ";
            }
            
        }
        return maxDir;
    }
};

class scoreBased3 : public ai {
public:
    scoreBased3(Matrix& other) : ai(other) {};
    Direction giveDirection() {
        int maxScore = -1;
        Direction maxDir = lefta;
        for(int dirInt = 0; dirInt != 4; dirInt++) {
            Direction tmpDir = static_cast<Direction>(dirInt);
            Matrix tmpMatrix(aiBaseMatrix);
            int tmpScore = 1;
            cout << endl << "Direction " << tmpDir << ": ";
            if (tmpMatrix.directionPossible(tmpDir)) {
                tmpMatrix.merge(tmpDir);
                for(int i=0;i<tmpMatrix.getDim();i++) {
                    for(int j=0;j<tmpMatrix.getDim()-1;j++) {
                        int val1 = tmpMatrix.getValAtPos(Position(i,j));
                        val1 = (val1 == 0 ? 1 : val1);
                        int val2 = tmpMatrix.getValAtPos(Position(i,j+1));
                        val2 = (val2 == 0 ? 1 : val2);
                        tmpScore *= double(val2)/val1;
                    }
                }
                for(int i=0;i<tmpMatrix.getDim()-1;i++) {
                    for(int j=0;j<tmpMatrix.getDim();j++) {
                        int val1 = tmpMatrix.getValAtPos(Position(i,j));
                        val1 = (val1 == 0 ? 1 : val1);
                        int val2 = tmpMatrix.getValAtPos(Position(i+1,j));
                        val2 = (val2 == 0 ? 1 : val2);
                        tmpScore *= double(val2)/val1;
                    }
                }
                cout << "Algo score is " << tmpScore;
                if (tmpScore > maxScore) {
                    maxDir = tmpDir;
                    maxScore = tmpScore;
                }
            } else {
                cout << "Not possible. ";
            }
            
        }
        return maxDir;
    }
};

class futureBased : public ai {
public:
    futureBased(Matrix& other) : ai(other) {};
    Direction giveDirection() {
        int maxScore = -1;
        Direction maxDir = lefta;
        for(int dirInt = 0; dirInt != 4; dirInt++) {
            Direction tmpDir = static_cast<Direction>(dirInt);
            Matrix tmpMatrix(aiBaseMatrix);
            Matrix tmpMatrix2(0);
            int tmpScore = 1;
            cout << endl << "Direction " << tmpDir << ": ";
            if (tmpMatrix.directionPossible(tmpDir)) {
                tmpMatrix.merge(tmpDir);
                for(int i=0;i<tmpMatrix.getDim();i++) {
                    for(int j=0;j<tmpMatrix.getDim();j++) {
                        if(tmpMatrix.getValAtPos(Position(i,j))==0) {
                            tmpMatrix2 = tmpMatrix;
                            tmpMatrix2.tiles.insert(std::pair<Position,Tile>(Position(i,j),Tile(2)));
                            if(tmpMatrix2.anyDirectionPossible()) tmpScore++;
                        }
                    }
                }
                for(int i=0;i<tmpMatrix.getDim();i++) {
                    for(int j=0;j<tmpMatrix.getDim();j++) {
                        if(tmpMatrix.getValAtPos(Position(i,j))==0) {
                            tmpMatrix2 = tmpMatrix;
                            tmpMatrix2.tiles.insert(std::pair<Position,Tile>(Position(i,j),Tile(4)));
                            if(tmpMatrix2.anyDirectionPossible()) tmpScore++;
                        }
                    }
                }
                cout << "Algo score is " << tmpScore;
                if (tmpScore > maxScore) {
                    maxDir = tmpDir;
                    maxScore = tmpScore;
                }
            } else {
                cout << "Not possible. ";
            }
            
        }
        return maxDir;
    }
};

class futureBased2 : public ai {
public:
    futureBased2(Matrix& other) : ai(other) {};
    Direction giveDirection() {
        int maxScore = -1;
        Direction maxDir = lefta;
        for(int dirInt = 0; dirInt != 4; dirInt++) {
            Direction tmpDir = static_cast<Direction>(dirInt);
            Matrix tmpMatrix(aiBaseMatrix);
            Matrix tmpMatrix2(0);
            int tmpScore = 0;
            cout << endl << "Direction " << tmpDir << ": ";
            if (tmpMatrix.directionPossible(tmpDir)) {
                tmpMatrix.merge(tmpDir);
                for(int i=0;i<tmpMatrix.getDim();i++) {
                    for(int j=0;j<tmpMatrix.getDim();j++) {
                        if(tmpMatrix.getValAtPos(Position(i,j))==0) {
                            tmpMatrix2 = tmpMatrix;
                            tmpMatrix2.tiles.insert(std::pair<Position,Tile>(Position(i,j),Tile(2)));
                            tmpScore += giveScore(tmpMatrix2);
                        }
                    }
                }
                for(int i=0;i<tmpMatrix.getDim();i++) {
                    for(int j=0;j<tmpMatrix.getDim();j++) {
                        if(tmpMatrix.getValAtPos(Position(i,j))==0) {
                            tmpMatrix2 = tmpMatrix;
                            tmpMatrix2.tiles.insert(std::pair<Position,Tile>(Position(i,j),Tile(4)));
                            tmpScore += giveScore(tmpMatrix2);
                        }
                    }
                }
                cout << "Algo score is " << tmpScore;
                if (tmpScore > maxScore) {
                    maxDir = tmpDir;
                    maxScore = tmpScore;
                }
            } else {
                cout << "Not possible. ";
            }
            
        }
        return maxDir;
    }
    int giveScore(const Matrix & mat) {
        int maxScore = -1;
        Direction maxDir = lefta;
        for(int dirInt = 0; dirInt != 4; dirInt++) {
            Direction tmpDir = static_cast<Direction>(dirInt);
            Matrix tmpMatrix(mat);
            Matrix tmpMatrix2(0);
            int tmpScore = 1;
            if (tmpMatrix.directionPossible(tmpDir)) {
                tmpMatrix.merge(tmpDir);
                for(int i=0;i<tmpMatrix.getDim();i++) {
                    for(int j=0;j<tmpMatrix.getDim();j++) {
                        if(tmpMatrix.getValAtPos(Position(i,j))==0) {
                            tmpMatrix2 = tmpMatrix;
                            tmpMatrix2.tiles.insert(std::pair<Position,Tile>(Position(i,j),Tile(2)));
                            if(tmpMatrix2.anyDirectionPossible()) tmpScore++;
                        }
                    }
                }
                for(int i=0;i<tmpMatrix.getDim();i++) {
                    for(int j=0;j<tmpMatrix.getDim();j++) {
                        if(tmpMatrix.getValAtPos(Position(i,j))==0) {
                            tmpMatrix2 = tmpMatrix;
                            tmpMatrix2.tiles.insert(std::pair<Position,Tile>(Position(i,j),Tile(4)));
                            if(tmpMatrix2.anyDirectionPossible()) tmpScore++;
                        }
                    }
                }
                if (tmpScore > maxScore) {
                    maxDir = tmpDir;
                    maxScore = tmpScore;
                }
            }
        }
        return maxScore;
    }

};


#endif


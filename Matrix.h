//
//  matrix.h
//  merge
//
//  Created by Tobias Hofmann on 15.04.14.
//  Copyright (c) 2014 Tobias Hofmann. All rights reserved.
//

#ifndef __merge__matrix__
#define __merge__matrix__

#include <iostream>
#include <vector>
#include <map>

#include "Position.h"
#include "Tile.h"

using namespace std;

class Matrix {
    
public:
    //Members
    int dim;
    int score;
    bool addNewTile;
    std::map<Position,Tile> tiles;
    
public:
    //Constructors
    Matrix(int dim_) : dim(dim_), score(0) {
        tiles = std::map<Position,Tile>();
    };
    Matrix(const Matrix& other) {
        dim = other.dim;
        score = other.score;
        addNewTile = other.addNewTile;
        tiles = other.tiles;
    }
    
    //Methods
    bool insertRandomTileAtRandomSpot() {
        int x,y;
        while(1) {
            x = rand() % dim;
            y = rand() % dim;
            bool free = true;
            for(auto it = tiles.begin(); it != tiles.end(); ++it) {
                if(it->first.x == x && it->first.y == y) {
                    free = false;
                    break;
                }
            }
            if(free) break;
        }
        Position newPos(x, y);
        int val = getRandVal();
        Tile newTile(val);
        tiles.insert(std::pair<Position,Tile>(newPos,newTile));
        return true;
    }
    
    void mergeLeft() {
        addNewTile = false;
        int k;
        int tmpVal;
        //First slide all tiles to the left
        for(int i=0;i<dim;i++) {
            for(int j=0;j<dim;j++) {
                auto it = tiles.find(Position(i,j));
                if(it != tiles.end()) {
                    k = j;
                    tmpVal = it->second.val;
                    while (true) {
                        auto it2 = tiles.find(Position(i,k-1));
                        if(it2 != tiles.end() || k<=0) break;
                        addNewTile = true;
                        tiles.erase(Position(i,k));
                        k--;
                        tiles.insert(std::pair<Position,Tile>(Position(i,k),tmpVal));
                    }
                }
            }
        }
        //Now merge pairs and mark them with "merged"
        for(int i=0;i<dim;i++) {
            for(int j=0;j<dim;j++) {
                auto it = tiles.find(Position(i,j));
                auto it2 = tiles.find(Position(i,j+1));
                if( it != tiles.end() && it2 != tiles.end() ) {
                    if(it->second.val == it2->second.val) {
                        it->second.val *= 2;
                        score += it->second.val;
                        tiles.erase(Position(i,j+1));
                    }
                }
            }
        }
        //Now slide all again
        for(int i=0;i<dim;i++) {
            for(int j=0;j<dim;j++) {
                auto it = tiles.find(Position(i,j));
                if(it != tiles.end()) {
                    k = j;
                    tmpVal = it->second.val;
                    while (true) {
                        auto it2 = tiles.find(Position(i,k-1));
                        if(it2 != tiles.end() || k<=0) break;
                        addNewTile = true;
                        tiles.erase(Position(i,k));
                        k--;
                        tiles.insert(std::pair<Position,Tile>(Position(i,k),tmpVal));
                    }
                }
            }
        }
    }
    
    bool movePossible() {
        Matrix nMatrix(*this);
        nMatrix.addNewTile = false;
        mergeUp();
        mergeDown();
        mergeLeft();
        mergeRight();
        return nMatrix.addNewTile;
    }
    
    void mergeUp() {
        rotateLeft();
        mergeLeft();
        rotateLeft();
        rotateLeft();
        rotateLeft();
    }
    
    void mergeRight() {
        rotateLeft();
        rotateLeft();
        mergeLeft();
        rotateLeft();
        rotateLeft();
    }
    
    void mergeDown() {
        rotateLeft();
        rotateLeft();
        rotateLeft();
        mergeLeft();
        rotateLeft();
    }
    
    void rotateLeft() {
        std::map<Position,Tile> nTiles = std::map<Position,Tile>();
        for(auto tile = tiles.begin(); tile != tiles.end(); tile++ ) {
            int tmpVal = tile->second.val;
            Position newPos(dim - 1 - tile->first.y , tile->first.x );
            nTiles.insert(std::pair<Position,Tile>(newPos,tmpVal));
        }
        tiles.clear();
        tiles = nTiles;
    }
    
    int getRandVal() {
        if(rand()>RAND_MAX * .9) {
            return 4;
        } else {
            return 2;
        }
    }
    
    bool freeSpotAvailable() {
        int numberOfSpotsAvailable = dim*dim;
        for(int i=0;i<dim;i++) {
            for(int j=0;j<dim;j++) {
                auto it = tiles.find(Position(i,j));
                if(it != tiles.end()) dim--;
            }
        }
        return ( numberOfSpotsAvailable > 0 );
    }
    
    void print() {
        cout << "-----------------" << endl;
        for(int i=0;i<dim;i++) {
            cout << "|";
            for(int j=0;j<dim;j++) {
                Position pos(i,j);
                std::map<Position,Tile>::iterator it = tiles.find(pos);
                if(it != tiles.end()) {
                    int val = it->second.val;
                    if (val<10) {
                        std::cout << " ";
                    }
                    if (val<100) {
                        std::cout << " ";
                    }
                    std::cout << it->second.val;
                } else {
                    std::cout << "   ";
                }
                std::cout << "|";
            }
            cout << endl;
        }
        cout << "-----------------" << endl;
    }
    
};

#endif /* defined(__merge__matrix__) */

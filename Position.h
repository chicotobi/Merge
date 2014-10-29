//
//  Position.h
//  merge
//
//  Created by Tobias Hofmann on 15.04.14.
//  Copyright (c) 2014 Tobias Hofmann. All rights reserved.
//

#ifndef __merge__Position__
#define __merge__Position__

#include <iostream>

class Position {
public:
    int x;
    int y;
public:
    
    Position() {};
    
    Position(int x_, int y_) {
        x = x_;
        y = y_;
    }
    
    bool operator <(const Position& other) const {
        if (this->x < other.x) return true;
        if (this->x == other.x && this->y < other.y) return true;
        return false;
    }
};

#endif /* defined(__merge__Position__) */

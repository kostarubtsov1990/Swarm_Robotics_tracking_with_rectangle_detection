//
// Created by kostarubtsov1990 on 27/05/18.
//

#include "Line.h"

Line::Line(const Point &p1, const Point &p2) : p1(p1), p2(p2) {}

const Point &Line::getP1() const {
    return p1;
}

const Point &Line::getP2() const {
    return p2;
}

//
// Created by kostarubtsov1990 on 01/06/18.
//

#ifndef SWARM_ROBOTICS_TRACKING_WITH_RECTANGLE_DETECTION_COMPERAOTRS_H
#define SWARM_ROBOTICS_TRACKING_WITH_RECTANGLE_DETECTION_COMPERAOTRS_H


#include "Line.h"

class Comperators {
public:
    struct LineCompareByX {
        inline bool operator() (const Line& l1, const Line& l2)
        {
            return l1.getP1().x < l2.getP1().x;
        }
    };

    struct LineCompareByY {
        inline bool operator() (const Line& l1, const Line& l2)
        {
            return l1.getP1().y < l2.getP1().y;
        }
    };

    struct cmpByPointLocation {
        bool operator()(const Point& a, const Point& b) const {
            return (a.x < b.x) || ((a.x == b.x) && (a.y < b.y));
        }
    };

};


#endif //SWARM_ROBOTICS_TRACKING_WITH_RECTANGLE_DETECTION_COMPERAOTRS_H

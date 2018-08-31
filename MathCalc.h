//
// Created by kostarubtsov1990 on 28/05/18.
//

#ifndef SWARM_ROBOTICS_TRACKING_WITH_RECTANGLE_DETECTION_MATHCALC_H
#define SWARM_ROBOTICS_TRACKING_WITH_RECTANGLE_DETECTION_MATHCALC_H


#include "Line.h"

class MathCalc {
public:
    /**
     * Helper function to find a cosine of angle between vectors
     * from pt0->pt1 and pt0->pt2
     */
    static double calcAngle(Point pt1, Point pt2, Point pt0);
    static int calcDist(Point p1, Point p2);
    static int yExtractionFromLineEquation(Line line, double x);
    static int xExtractionFromLineEquation(Line line, double y);
    static Point calcLineIntersectionPoint(Line l1, Line l2);


};


#endif //SWARM_ROBOTICS_TRACKING_WITH_RECTANGLE_DETECTION_MATHCALC_H

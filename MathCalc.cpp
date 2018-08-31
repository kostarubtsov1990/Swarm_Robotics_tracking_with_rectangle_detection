//
// Created by kostarubtsov1990 on 28/05/18.
//

#include "MathCalc.h"

double MathCalc::calcAngle(Point pt1, Point pt2, Point pt0) {
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);}

int MathCalc::calcDist(Point p1, Point p2) {
    double x = p1.x - p2.x; //calculating number to square in next step
    double y = p1.y - p2.y;
    double dist;

    dist = pow(x, 2) + pow(y, 2);       //calculating Euclidean distance
    dist = sqrt(dist);

    return (int)dist;
}

int MathCalc::yExtractionFromLineEquation(Line line, double x) {
    return (int)(((line.getP2().y - line.getP1().y)*(x - line.getP1().x))/(line.getP2().x - line.getP1().x) +
                 line.getP1().y);
}

int MathCalc::xExtractionFromLineEquation(Line line, double y) {
    return (int)(((line.getP2().x - line.getP1().x)*(y - line.getP1().y))/(line.getP2().y - line.getP1().y) +
                 line.getP1().x);
}

Point MathCalc::calcLineIntersectionPoint(Line l1, Line l2) {
    // Line l1 represented as a1x + b1y = c1
    double a1 = l1.getP2().y - l1.getP1().y;
    double b1 = l1.getP1().x - l1.getP2().x;
    double c1 = a1*(l1.getP1().x) + b1*(l1.getP1().y);

    // Line l2 represented as a2x + b2y = c2
    double a2 = l2.getP2().y - l2.getP1().y;
    double b2 = l2.getP1().x - l2.getP2().x;
    double c2 = a2*(l2.getP1().x) + b2*(l2.getP1().y);

    double determinant = a1*b2 - a2*b1;

    if (determinant == 0)
    {
        // The lines are parallel. This is simplified
        // by returning a pair of INFINITY
        return Point(INFINITY, INFINITY);
    }
    else
    {
        double x = (b2*c1 - b1*c2)/determinant;
        double y = (a1*c2 - a2*c1)/determinant;
        return Point(x, y);
    }
}

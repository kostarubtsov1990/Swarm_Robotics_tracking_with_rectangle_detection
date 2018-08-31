//
// Created by kostarubtsov1990 on 27/05/18.
//

#ifndef SWARM_ROBOTICS_TRACKING_WITH_RECTANGLE_DETECTION_LINE_H
#define SWARM_ROBOTICS_TRACKING_WITH_RECTANGLE_DETECTION_LINE_H

#include <opencv/cv.h>

using namespace cv;


class Line {
private:
    Point p1, p2;
public:
    Line(const Point &p1, const Point &p2);
    const Point &getP1() const;
    const Point &getP2() const;

};


#endif //SWARM_ROBOTICS_TRACKING_WITH_RECTANGLE_DETECTION_LINE_H

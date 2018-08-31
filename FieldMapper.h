//
// Created by kostarubtsov1990 on 25/05/18.
//

#ifndef SWARM_ROBOTICS_TRACKING_WITH_RECTANGLE_DETECTION_FIELDMAPPER_H
#define SWARM_ROBOTICS_TRACKING_WITH_RECTANGLE_DETECTION_FIELDMAPPER_H

#include <opencv/cv.h>
#include <map>
#include "Line.h"
#include "Comperators.h"

#define LOWER_FRAME_OFFSET 25
#define UPPER_FRAME_OFFSET 125
#define PIXEL_MAPPING_OFFSET 3

using namespace cv;
using namespace std;

//This class is responsible for mapping all the image pixels to appropriate coordinates
class FieldMapper {
private:
    //This multiplier is necessary for situation where the camera is placed in angle
    const double horizontalOffsetMultiplier = 0.97;
    Line upperFrame, lowerFrame, leftFrame, rightFrame;
    //basically it is the angle between two intersecting frames
    double horizFrameToVertFrameAngle;
    //size of each cell in the field
    int segmentSize;
    //all the lines with segmentSize offset one from another are gathered in these DS
    vector<Line> horizontalLines;
    vector<Line> verticalLines;
    //map that holds pixel to grid location
    map<Point, Point, Comperators::cmpByPointLocation> pixelsToLocation;
    //This function will map pixels by searching its location on the grid
    void PixelMapping(int x, int y);
    Point CalcPixelLocationOffset(int x, int y);

public:

    //ctor'
    //This class will need the frame lines and the segment size to perform the mapping process
    FieldMapper(const Line &upperFrame, const Line &lowerFrame, const Line &leftFrame, const Line &rightFrame,
                double horizFrameToVertFrameAngle, int segmentSize);
    //This function will take pixel coordinates as an arguments and map to the corresponding segment
    void fieldMap(int rows, int cols);
    //show the mapping on the GUI
    void toggleView(string windowName, Mat frame);
    map<Point, Point, Comperators::cmpByPointLocation>& getMap();


};


#endif //SWARM_ROBOTICS_TRACKING_WITH_RECTANGLE_DETECTION_FIELDMAPPER_H

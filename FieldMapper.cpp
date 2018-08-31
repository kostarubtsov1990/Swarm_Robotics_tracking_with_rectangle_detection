//
// Created by kostarubtsov1990 on 25/05/18.
//

#include <opencv2/imgproc.hpp>
#include <opencv/cv.hpp>
#include <iostream>
#include "FieldMapper.h"
#include "MathCalc.h"
#include "Comperators.h"


FieldMapper::FieldMapper(const Line &upperFrame, const Line &lowerFrame, const Line &leftFrame, const Line &rightFrame,
                         double horizFrameToVertFrameAngle, int segmentSize) : upperFrame(upperFrame),
                                                                                  lowerFrame(lowerFrame),
                                                                                  leftFrame(leftFrame),
                                                                                  rightFrame(rightFrame),
                                                                                  horizFrameToVertFrameAngle(
                                                                                          horizFrameToVertFrameAngle),
                                                                                  segmentSize(segmentSize) {
    verticalLines.push_back(leftFrame);
    verticalLines.push_back(rightFrame);
    horizontalLines.push_back(upperFrame);
    horizontalLines.push_back(lowerFrame);
}

void FieldMapper::fieldMap(int rows, int cols) {

    //starting from the center of the horizontal frames
    Point lowerFrameCenter((lowerFrame.getP2().x - lowerFrame.getP1().x)/2 + LOWER_FRAME_OFFSET, lowerFrame.getP1().y);
    Point upperFrameCenter((upperFrame.getP2().x - upperFrame.getP1().x)/2 + UPPER_FRAME_OFFSET, upperFrame.getP1().y);

    //the offset ratio between two vertical neighbor lines intersecting with the upper frame
    //is correlated to their offset that intersecting with the lower frame
    double lowerToUpperFrameRatio =
    MathCalc::calcDist(upperFrame.getP1(), upperFrame.getP2())/
            (double)MathCalc::calcDist(lowerFrame.getP1(), lowerFrame.getP2());

    //calculate the first two center vertical lines
    Line leftLine(Point(lowerFrameCenter.x - segmentSize/2,
                        MathCalc::yExtractionFromLineEquation(lowerFrame,lowerFrameCenter.x - segmentSize/2)),
                  Point(upperFrameCenter.x - (int)(segmentSize/2*lowerToUpperFrameRatio),
                        MathCalc::yExtractionFromLineEquation(upperFrame,upperFrameCenter.x -
                                segmentSize/2*lowerToUpperFrameRatio)));

    Line rightLine(Point(lowerFrameCenter.x + segmentSize/2,
                        MathCalc::yExtractionFromLineEquation(lowerFrame,lowerFrameCenter.x + segmentSize/2)),
                  Point(upperFrameCenter.x + (int)(segmentSize/2*lowerToUpperFrameRatio),
                        MathCalc::yExtractionFromLineEquation(upperFrame,upperFrameCenter.x +
                                segmentSize/2*lowerToUpperFrameRatio)));

    verticalLines.push_back(leftLine);
    verticalLines.push_back(rightLine);

    Line* pToCurrentMostLeftLine = &leftLine;
    Line* pToCurrentMostRightLine = &rightLine;

    //creation process of the vertical lines starting from the center of horizontal frames
    while (pToCurrentMostLeftLine->getP1().x - leftFrame.getP2().x > segmentSize*1.35 &&
            rightFrame.getP2().x - pToCurrentMostRightLine->getP1().x > segmentSize*1.35) {
        //create both parallel lines from the left and the right to the current left and right pointers respectively
        Line currentMostLeftLine(Point(pToCurrentMostLeftLine->getP1().x - segmentSize,
                            MathCalc::yExtractionFromLineEquation(lowerFrame,pToCurrentMostLeftLine->getP1().x - segmentSize)),
                      Point(pToCurrentMostLeftLine->getP2().x - (int)(segmentSize*lowerToUpperFrameRatio),
                            MathCalc::yExtractionFromLineEquation(upperFrame,pToCurrentMostLeftLine->getP2().x -
                                    segmentSize*lowerToUpperFrameRatio)));

        Line currentMostRightLine(Point(pToCurrentMostRightLine->getP1().x + segmentSize,
                             MathCalc::yExtractionFromLineEquation(lowerFrame,pToCurrentMostRightLine->getP1().x + segmentSize)),
                       Point(pToCurrentMostRightLine->getP2().x + (int)(segmentSize*lowerToUpperFrameRatio),
                             MathCalc::yExtractionFromLineEquation(upperFrame,pToCurrentMostRightLine->getP2().x +
                                     segmentSize*lowerToUpperFrameRatio)));

        verticalLines.push_back(currentMostLeftLine);
        verticalLines.push_back(currentMostRightLine);
        //update the pointers to point to the most recently created lines
        pToCurrentMostLeftLine = verticalLines.end().base() - 2;
        pToCurrentMostRightLine = verticalLines.end().base() - 1;

    }

    Line* pToCurrentMostUpperLine = &lowerFrame;
    //creation process of the horizontal lines starting from the bottom
    while(pToCurrentMostUpperLine->getP1().y - upperFrame.getP1().y > segmentSize*1.35) {
        Line currentMostUpperLine(Point(MathCalc::xExtractionFromLineEquation(leftFrame,
                                                                              pToCurrentMostUpperLine->getP1().y - segmentSize),
                                        pToCurrentMostUpperLine->getP1().y - segmentSize),
                                 Point(MathCalc::xExtractionFromLineEquation(rightFrame,
                                                                             pToCurrentMostUpperLine->getP1().y - segmentSize),
                                       pToCurrentMostUpperLine->getP1().y - segmentSize));

        horizontalLines.push_back(currentMostUpperLine);

        pToCurrentMostUpperLine = horizontalLines.end().base() - 1;


    }

    sort(verticalLines.begin(), verticalLines.end(), Comperators::LineCompareByX());
    sort(horizontalLines.begin(), horizontalLines.end(), Comperators::LineCompareByY());

    //pixel mapping process, may take some time (depending on the video resolution)
    //you should turn off this process in case of debugging the detection properties only
    for(int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            PixelMapping(j, i);
        }
    }

}

void FieldMapper::toggleView(string windowName, Mat frame) {

    vector<Line>::iterator iter = horizontalLines.begin();
    while (iter != horizontalLines.end()) {
        line(frame, iter.base()->getP1(), iter.base()->getP2(), Scalar(255.0, 255.0, 255.0));
        iter++;
    }

    iter = verticalLines.begin();
    while (iter != verticalLines.end()) {
        line(frame, iter.base()->getP1(), iter.base()->getP2(), Scalar(255.0, 255.0, 255.0));
        iter++;
    }

    imshow(windowName, frame);
}

void FieldMapper::PixelMapping(int x, int y) {
    cout<< "location: x: " << x << "y: " << y << endl;
    //CalcPixelLocationOffset(x, y);
    vector<Line>::iterator horLinesIter = horizontalLines.begin();
    Line* upperLine = horLinesIter.base();
    horLinesIter++;
    Line* lowerLine = horLinesIter.base();

    int rowsLocation = (int)horizontalLines.size() - 1;

    while (horLinesIter != horizontalLines.end() - 1) {
        vector<Line>::iterator vertLinesIter = verticalLines.begin();
        Line* leftLine = vertLinesIter.base();
        vertLinesIter++;
        Line* rightLine = vertLinesIter.base();

        int columnsLocation = 1;

        while(vertLinesIter != verticalLines.end() - 1) {
            Point upperLeftIntersectionPoint = MathCalc::calcLineIntersectionPoint(*upperLine, *leftLine);
            Point lowerLeftIntersectionPoint = MathCalc::calcLineIntersectionPoint(*lowerLine, *leftLine);
            Point upperRightIntersectionPoint = MathCalc::calcLineIntersectionPoint(*upperLine, *rightLine);
            Point lowerRightIntersectionPoint = MathCalc::calcLineIntersectionPoint(*lowerLine, *rightLine);

            if (x >= upperLeftIntersectionPoint.x - PIXEL_MAPPING_OFFSET &&
                    y >= upperLeftIntersectionPoint.y - PIXEL_MAPPING_OFFSET &&
                    x >= lowerLeftIntersectionPoint.x - PIXEL_MAPPING_OFFSET &&
                    y <= lowerLeftIntersectionPoint.y + PIXEL_MAPPING_OFFSET &&
                    x <= upperRightIntersectionPoint.x + PIXEL_MAPPING_OFFSET &&
                    y >= upperRightIntersectionPoint.y - PIXEL_MAPPING_OFFSET &&
                    x <= lowerRightIntersectionPoint.x + PIXEL_MAPPING_OFFSET &&
                    y <= lowerRightIntersectionPoint.y + PIXEL_MAPPING_OFFSET) {

                pixelsToLocation[Point(x, y)] = Point(columnsLocation, rowsLocation);
                return;

            }
            leftLine = rightLine;
            vertLinesIter++;
            rightLine = vertLinesIter.base();

            columnsLocation++;
        }

        upperLine = lowerLine;
        horLinesIter++;
        lowerLine = horLinesIter.base();

        rowsLocation--;
    }
}

map<Point, Point, Comperators::cmpByPointLocation>& FieldMapper::getMap() {
    return pixelsToLocation;
}

Point FieldMapper::CalcPixelLocationOffset(int x, int y) {
    Point lowerFrameCenter((lowerFrame.getP2().x - lowerFrame.getP1().x)/2, lowerFrame.getP1().y);
    double distance = MathCalc::calcDist(Point(x, y), lowerFrameCenter);
    double angle = MathCalc::calcAngle(Point(x, y), lowerFrame.getP1(), lowerFrameCenter);

}


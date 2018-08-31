//
// Created by kostarubtsov1990 on 01/06/18.
//

#ifndef SWARM_ROBOTICS_TRACKING_WITH_RECTANGLE_DETECTION_BLOB_H
#define SWARM_ROBOTICS_TRACKING_WITH_RECTANGLE_DETECTION_BLOB_H


#include <opencv2/core/types.hpp>
using namespace std;
using namespace cv;

class Blob {
public:
    Blob(const vector<Point> &currentContour);

private:
    vector<Point> currentContour;
    Point centerPosition;
    double blobDiagonalSize;
    int id;

public:

    Blob & operator=(const Blob & rhs);
    vector<Point> getContours () const;
    Point getCenterPosition() const ;
    double getDiagonalSize() const ;
    int getId() const ;
    void setId(int id);
    void SaveBlobToFile();
};


#endif //SWARM_ROBOTICS_TRACKING_WITH_RECTANGLE_DETECTION_BLOB_H

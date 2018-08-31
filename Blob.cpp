//
// Created by kostarubtsov1990 on 01/06/18.
//

#include <opencv2/imgproc.hpp>
#include <fstream>
#include "Blob.h"


Blob::Blob(const vector<Point> &currentContour) : currentContour(currentContour) {

    Moments contourMoments = moments(currentContour);

    centerPosition.x = (int)(contourMoments.m10/ contourMoments.m00);
    centerPosition.y = (int)(contourMoments.m01/ contourMoments.m00);
    Rect currentBoundingRect = cv::boundingRect(currentContour);
    blobDiagonalSize = sqrt(pow(currentBoundingRect.width, 2) + pow(currentBoundingRect.height, 2));
}

void Blob::setId(int id) {
    this->id = id;
}

Blob & Blob::operator=(const Blob & rhs)
{
    if(this == &rhs)
        return *this;
    currentContour = rhs.getContours();
    centerPosition = rhs.getCenterPosition();
    blobDiagonalSize = rhs.getDiagonalSize();
    return *this;
}

vector<Point> Blob::getContours() const {
    return currentContour;
}

Point Blob::getCenterPosition() const {
    return centerPosition;
}

double Blob::getDiagonalSize() const {
    return blobDiagonalSize;
}

int Blob::getId() const {
    return id;
}

void Blob::SaveBlobToFile() {
    ofstream myfile;
    string fileName = "robot" + id;
    fileName += ".txt";
    myfile.open (fileName);
    myfile << "Writing this to a file.\n";
    myfile.close();

}

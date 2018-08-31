//
// Created by kostarubtsov1990 on 30/06/18.
//

#ifndef SWARM_ROBOTICS_TRACKING_WITH_RECTANGLE_DETECTION_BLOBTOFILE_H
#define SWARM_ROBOTICS_TRACKING_WITH_RECTANGLE_DETECTION_BLOBTOFILE_H



#include <map>
#include <cv.h>
using namespace std;
using namespace cv;

class BlobToFile {
private:
    map<string, ofstream*> fileNameToFileStream;
public:
    void SaveBlobLocationToFile(int blobId, Point location);
    void CloseAllFileStreams();

};


#endif //SWARM_ROBOTICS_TRACKING_WITH_RECTANGLE_DETECTION_BLOBTOFILE_H

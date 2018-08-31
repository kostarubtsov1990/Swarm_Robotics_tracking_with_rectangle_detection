#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>
#include <iostream>
#include "FieldMapper.h"
#include "MathCalc.h"
#include "Blob.h"
#include "BlobToFile.h"

//primary detection settings with high impact on performance
//consider moving to xml file
#define PATH_NAME "Video 5.wmv"
#define DEFAULT_CAMERA 0
#define CANNY_THRESHOLD_1 175
#define CANNY_THRESHOLD_2 350
#define BLOB_SIZE_THRESHOLD 300
#define BLOB_DIST_THRESHOLD 1.5
#define TIME_INTERVAL 1

const cv::Scalar SCALAR_BLACK = cv::Scalar(0.0, 0.0, 0.0);
const cv::Scalar SCALAR_WHITE = cv::Scalar(255.0, 255.0, 255.0);

void drawAndShowContours(cv::Size imageSize, std::vector<std::vector<cv::Point> > contours, std::string strImageName);
void CallBackFunc(int event, int x, int y, int flags, void* userdata);
FieldMapper* FieldMapperCreationFlow ();
void matchCurrentFrameBlobsToExistingBlobs(std::vector<Blob> &trackedBlobs, std::vector<Blob> &currentFrameBlobs);
void DrawBlobs(Mat& frame, vector<Blob> trackedBlobs);

using namespace std;
using namespace cv;

int blobCurrentNum = 1;
BlobToFile* pBlobToFile;
FieldMapper* fieldMapper;
/**
 * Helper function to display text in the center of a contour
 */
void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour)
{
    int fontface = cv::FONT_HERSHEY_SIMPLEX;
    double scale = 0.4;
    int thickness = 1;
    int baseline = 0;

    cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
    cv::Rect r = cv::boundingRect(contour);

    cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
    cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255,255,255), CV_FILLED);
    cv::putText(im, label, pt, fontface, scale, CV_RGB(0,0,0), thickness, 8);
}

int main()
{

    VideoCapture capVideo;

    Mat imgFrame;
    Mat imgGrayFrame;

    vector<Blob> trackedBlobs;

    capVideo.open(PATH_NAME);
    //this command should open the default camera instead of uploading an existing video
    //comment the previous command before uncommenting this one
    //capVideo.open(DEFAULT_CAMERA);

    if (!capVideo.isOpened()) {
        std::cout << "error reading video file" << std::endl << std::endl;
        getchar();
        return (0);
    }

    if (capVideo.get(CV_CAP_PROP_FRAME_COUNT) < 2) {
        std::cout << "error: video file must have at least two frames";
        getchar();
        return (0);
    }


    capVideo.read(imgFrame);
    cv::imshow("FieldMapper Configuration", imgFrame);


    int rows = imgFrame.rows;
    int cols = imgFrame.cols;

    //create FieldMapper Object taking data from user using opencv image showing GUI
    fieldMapper = FieldMapperCreationFlow();
    //initialize the mapping DS within the FieldMapper Object
    fieldMapper->fieldMap(rows, cols);
    //show results
    fieldMapper->toggleView("FieldMapper Configuration", imgFrame);


    BlobToFile blobToFile;
    pBlobToFile = &blobToFile;



    /*char chCheckForEscKey = 0;

    bool blnFirstFrame = true;

    int frameCount = 2;*/

    while (capVideo.isOpened() /*&& chCheckForEscKey != 27*/) {


        capVideo.read(imgFrame);
        // Convert to grayscale
        cv::cvtColor(imgFrame, imgGrayFrame, CV_BGR2GRAY);
        //imgGrayFrame = imgFrame.clone();

        // Use Canny instead of threshold to catch squares with gradient shading
        cv::Mat bw;
        cv::Canny(imgGrayFrame, bw, CANNY_THRESHOLD_1, CANNY_THRESHOLD_2);

        // Find contours
        std::vector<std::vector<cv::Point> > contours;
        cv::findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
        /*std::vector<std::vector<cv::Point> > convexHulls(contours.size());

        for (unsigned int i = 0; i < contours.size(); i++) {
            cv::convexHull(contours[i], convexHulls[i]);
        }*/

        //show all the contours found
        //you can set this off to increase performance
        drawAndShowContours(bw.size(), contours, "imgContours");

        std::vector<cv::Point> approx;
        cv::Mat dst = imgFrame.clone();

        vector<Blob> currentFrameBlobs;
        //go through all the contours and filter them by desired features
        for (int i = 0; i < contours.size(); i++) {
            // Approximate contour with accuracy proportional
            // to the contour perimeter
            cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true) * 0.02, true);

            // Skip small or non-convex objects
            if (std::fabs(cv::contourArea(contours[i])) < BLOB_SIZE_THRESHOLD
               /* || !cv::isContourConvex(approx)*/)
                continue;


            //CODE THAT RESPONSIBLE FOR DETECTING DIFFERENT KIND OF SHAPES

            //if (approx.size() == 4) {
                // Number of vertices of polygonal curve.
                int vtc = approx.size();

                // Get the cosines of all corners
                std::vector<double> cos;
                for (int j = 2; j < vtc + 1; j++)
                    cos.push_back(MathCalc::calcAngle(approx[j % vtc], approx[j - 2], approx[j - 1]));

                // Sort ascending the cosine values
                std::sort(cos.begin(), cos.end());

                // Get the lowest and the highest cosine
                double mincos = cos.front();
                double maxcos = cos.back();

                // Use the degrees obtained above and the number of vertices
                // to determine the shape of the contour
                //if (vtc >= 2 && vtc <= 7/*mincos >= -0.5 && maxcos <= 1*/)
                    currentFrameBlobs.push_back(Blob(contours[i]));
                    //setLabel(dst, "RECT", contours[i]);
            //}
        }
        matchCurrentFrameBlobsToExistingBlobs(trackedBlobs, currentFrameBlobs);
        DrawBlobs(dst, trackedBlobs);
        cv::imshow("dst", dst);
        //set the callback function for any mouse event
        setMouseCallback("dst", CallBackFunc, NULL);
        //this function will pause the process after each frame of the video
        //values that are higher than 0 increasing the time interval between each frame
        waitKey(TIME_INTERVAL);
    }

    return 0;
}

/*
 * This function will take the current iteration blobs and compare them to the global tracked blobs,
 * and then it will update the tracked blobs parameters to the most current parameters
 */
void matchCurrentFrameBlobsToExistingBlobs(std::vector<Blob> &trackedBlobs, std::vector<Blob> &currentFrameBlobs) {

    /*for (auto &trackedBlob : trackedBlobs) {

        trackedBlob.blnCurrentMatchFoundOrNewBlob = false;

        trackedBlob.predictNextPosition();
    }*/

    //for each current frame blob check whether it is close enough to one of the tracked blobs
    for (auto &currentFrameBlob : currentFrameBlobs) {

        int indexOfLeastDistance = 0;
        double dblLeastDistance = DBL_MAX;

        for (unsigned int i = 0; i < trackedBlobs.size(); i++) {
            //if (trackedBlobs[i].blnStillBeingTracked == true) {
                double dblDistance = MathCalc::calcDist(currentFrameBlob.getCenterPosition(),
                                                        trackedBlobs[i].getCenterPosition());

                if (dblDistance < dblLeastDistance) {
                    dblLeastDistance = dblDistance;
                    indexOfLeastDistance = i;
                }
            //}
        }

        //check if the distance that found above is bounded by current blob diagonal size multiply by threshold
        if (dblLeastDistance < currentFrameBlob.getDiagonalSize() * BLOB_DIST_THRESHOLD) {
            //update the tracked blob to the current blob information
            trackedBlobs[indexOfLeastDistance] = currentFrameBlob;
            //save its location to a file
            pBlobToFile->SaveBlobLocationToFile(trackedBlobs[indexOfLeastDistance].getId(),
                                                fieldMapper->getMap()
                                                [trackedBlobs[indexOfLeastDistance].getCenterPosition()]);
        }
            //in that case the blob that had been found in the current frame is a new one
        else {
            currentFrameBlob.setId(blobCurrentNum);
            trackedBlobs.push_back(currentFrameBlob);
            pBlobToFile->SaveBlobLocationToFile(currentFrameBlob.getId(),
                                                fieldMapper->getMap()
                                                [currentFrameBlob.getCenterPosition()]);
            blobCurrentNum++;
        }

    }

    /*for (auto &trackedBlob : trackedBlobs) {

        if (trackedBlob.blnCurrentMatchFoundOrNewBlob == false) {
            trackedBlob.intNumOfConsecutiveFramesWithoutAMatch++;
        }

        if (trackedBlob.intNumOfConsecutiveFramesWithoutAMatch >= 1000) {
            trackedBlob.blnStillBeingTracked = false;
        }

    }*/

}

void drawAndShowContours(cv::Size imageSize, std::vector<std::vector<cv::Point> > contours, std::string strImageName) {
    cv::Mat image(imageSize, CV_8UC3, SCALAR_BLACK);
    cv::drawContours(image, contours, -1, SCALAR_WHITE, -1);
    cv::imshow(strImageName, image);
}

void DrawBlobs(Mat& frame, vector<Blob> trackedBlobs) {
    vector<Blob>::iterator iter = trackedBlobs.begin();
    vector<std::vector<cv::Point> > allContours;
    while (iter != trackedBlobs.end()) {
        allContours.push_back(iter.base()->getContours());
        iter++;
    }
    drawContours(frame, allContours, -1, SCALAR_WHITE, -1);

    iter = trackedBlobs.begin();
    while (iter != trackedBlobs.end()){
        vector<Point> currentBlobContour = iter.base()->getContours();
        setLabel(frame, to_string(iter.base()->getId()), currentBlobContour);
        iter++;
    }
}
/*
 * This function will be executed when mouse click event occurs
 * It will set the userdata variable (that will be casted to Point object) to the pixel location that
 * the cursor pointed to on click event
 */
void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    if  ( event == EVENT_LBUTTONDOWN )
    {
        cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
        ((Point*)userdata)->x = x;
        ((Point*)userdata)->y = y;
    }
    else if  ( event == EVENT_RBUTTONDOWN )
    {
        cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    }
    else if  ( event == EVENT_MBUTTONDOWN )
    {
        cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    }
}
/*
 * This function will launch the procedure of creating FieldMapper object passing required arguments to its c'tor
 */
FieldMapper* FieldMapperCreationFlow () {
    Point upperLeft, lowerLeft, upperRight, lowerRight, startDetectObject, endDetectObject;
    FieldMapper* fieldMapper;

    setMouseCallback("FieldMapper Configuration", CallBackFunc, &upperLeft);
    cout << "Please click on the upperLeft corner of the desired field" << endl;
    waitKey(0);
    setMouseCallback("FieldMapper Configuration", CallBackFunc, &lowerLeft);
    cout << "Please click on the lowerLeft corner of the desired field" << endl;
    waitKey(0);
    setMouseCallback("FieldMapper Configuration", CallBackFunc, &upperRight);
    cout << "Please click on the upperRight corner of the desired field" << endl;
    waitKey(0);
    setMouseCallback("FieldMapper Configuration", CallBackFunc, &lowerRight);
    cout << "Please click on the lowerRight corner of the desired field" << endl;
    waitKey(0);
    setMouseCallback("FieldMapper Configuration", CallBackFunc, &startDetectObject);
    cout << "Please click on the starting point of the desired object to be tracked";
    waitKey(0);
    setMouseCallback("FieldMapper Configuration", CallBackFunc, &endDetectObject);
    cout << "Please click on the starting point of the desired object to be tracked";
    waitKey(0);

    Line upperFrame(upperLeft, upperRight), lowerFrame(lowerLeft, lowerRight),
            leftFrame (upperLeft, lowerLeft), rightFrame(upperRight, lowerRight);
    double horizFrameToVertFrameAngle = MathCalc::calcAngle(upperLeft, lowerRight, lowerLeft);
    int segmentSize = MathCalc::calcDist(startDetectObject, endDetectObject);
    fieldMapper = new FieldMapper(upperFrame, lowerFrame, leftFrame, rightFrame,
                                  horizFrameToVertFrameAngle, segmentSize);

    return fieldMapper;
}
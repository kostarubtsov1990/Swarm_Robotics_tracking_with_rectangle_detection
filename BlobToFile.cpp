//
// Created by kostarubtsov1990 on 30/06/18.
//

#include <fstream>
#include "BlobToFile.h"

void BlobToFile::SaveBlobLocationToFile(int blobId, Point location) {
    string fileName = "blob" + to_string(blobId) + ".txt";
    ofstream* file = NULL;

    if (fileNameToFileStream.find(fileName) == fileNameToFileStream.end()) {
        file = new ofstream();
        file->open(fileName);
        fileNameToFileStream[fileName] = file;
    }

    if (file == NULL) {
        file = fileNameToFileStream[fileName];
    }

    (*file) << "(" << to_string(location.x) << "," << to_string(location.y) + ")\n";
    (*file).flush();
}

void BlobToFile::CloseAllFileStreams() {
    map<string, ofstream*>::iterator iter = fileNameToFileStream.begin();
    while (iter != fileNameToFileStream.end()) {
        iter->second->close();
        delete iter->second;
    }
}



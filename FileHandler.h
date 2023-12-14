// methods.h
#pragma once

#include <vector>
#include <QStandardItemModel>
#include <string>
#include <QLabel>
#include "Data.h"

using namespace std;

/**
 * @brief The FileHandler class provides methods to read a csv file, to show flights and to export the current flights as csv-fromat.
 *
 */

class FileHandler {
public:





    int readFileToVec(string path);
    void showFlights(vector<flights> Flights);
    void exportFileToCSV();


};



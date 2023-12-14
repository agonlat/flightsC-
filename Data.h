#pragma once
#include <vector>
#include <QStandardItemModel>
#include <QColor>
#include <map>

/**
 * @brief Data.h is a header file which provides the neccesary data structures for storing the flights.
 */

using namespace std;

/**
 * @brief This struct holds the neccesary attributes of a flight.
 */
struct flights {
    string fnr;
    string knr;
    string reg;
    string typ;
    string ha0;
    string lsk;
    string stt;
    string itt;
    string pos;
    string ter;
    string mad;
    string saa;
};


extern vector<flights> allFlights;
extern vector<flights> filteredFlights;
extern vector<flights> currentFlights;
extern vector<flights> currentFlightsCOPY;
extern int rows;
extern QStandardItemModel *model;
extern std::string path;
extern QColor color1;
extern QColor color2;
extern std::map<std::string, int> airlineCounter;
extern int changedFlights;
extern int allFlightsCount;

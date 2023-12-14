#include "FileHandler.h"
#include <fstream>
#include <sstream>
#include "mainwindow.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/c_local_time_adjustor.hpp"
#include <sstream>
#include "Validator.h"
#include <QMessageBox>
#include "TimeHandler.h"
#include "Data.h"
#include <QDesktopServices>
#include "utilitymethods.h"

using namespace std;
using namespace boost::gregorian;
using namespace boost::posix_time;






int rows = 0;
int c=0;

vector<flights> allFlights;
vector<flights> currentFlights;
vector<flights> currentFlightsCOPY;
vector<flights> filteredFlights;
int allFlightsCount;
std::map<std::string, int> airlineCounter;


/**
 * @brief Reads a file from a given path and stores the data in allFlights.
 *
 * It checks if the CSV file contains 12 columns. If not, it does not crash and calls `exc()` with an error message.
 *
 * @param path The file path as a string.
 * @return 1 If the file was read successfully.
 * @return 0 If the file could not be opened.
 * @return -2 If the file has not 12 columns
 */
int FileHandler::readFileToVec(string path) {
    TimeHandler *handler = new TimeHandler;
    int rows = 0;
    ifstream file(path);

    if (!file.is_open()) {
        return 0;
    }

    string line;
    bool hasIncorrectFormat = false;  // Flag to check incorrect format

    while (getline(file, line)) {
        rows++;
        allFlightsCount++;

        istringstream iss(line);
        string token;
        int cols = 0;
        flights fl;
        vector<string> tokens;

        while (getline(iss, token, ',')) {
            tokens.push_back(token);
        }




        if (tokens.size() != 12)
            return -2;
        else {

            string a = UtilityMethods::getAirline(tokens.at(0));
            airlineCounter[a]++;

            fl.fnr = tokens.at(0);
            fl.knr = tokens.at(1);
            fl.reg = tokens.at(2);
            fl.typ = tokens.at(3);
            fl.ha0 = tokens.at(4);
            fl.lsk = tokens.at(5);
            if (tokens.at(6) != "")
                fl.stt = handler->refreshDate() + " "+ handler->getLocalTime(tokens.at(6));


            if (tokens.at(7) != "")
                fl.itt = handler->refreshDate() + " " +handler->getLocalTime(tokens.at(7));


            fl.pos = tokens.at(8);
            fl.ter = tokens.at(9);
            fl.mad = tokens.at(10);
            fl.saa = tokens.at(11);
            // Add the flight only if format is correct
            if (rows == 1)
                continue;
            else
                allFlights.push_back(fl);
        }



    }

    return 1; // Return 1 if all lines have the correct format


}



/**
 * @brief Shows all Flights in the tableView. It takes a vector of flights and refreshes the table accordingly.
 * @param Flights Is the vector of a struct "flights" to be shown.
 */
void FileHandler::showFlights(vector<flights> Flights) {

    for (int i = 0; i < Flights.size(); i++) {
        const flights& rowData = Flights[i];
        /**
         * @brief You need QStandardItems in order to add it to the QTableView
         */
        QStandardItem *item0 = new QStandardItem(QString::fromStdString(rowData.fnr));
        QStandardItem *item1 = new QStandardItem(QString::fromStdString(rowData.knr));
        QStandardItem *item2 = new QStandardItem(QString::fromStdString(rowData.reg));
        QStandardItem *item3 = new QStandardItem(QString::fromStdString(rowData.typ));
        QStandardItem *item4 = new QStandardItem(QString::fromStdString(rowData.ha0));
        QStandardItem *item5 = new QStandardItem(QString::fromStdString(rowData.lsk));
        QStandardItem *item6 = new QStandardItem(QString::fromStdString(rowData.stt));
        QStandardItem *item7 = new QStandardItem(QString::fromStdString(rowData.itt));
        QStandardItem *item8 = new QStandardItem(QString::fromStdString(rowData.pos));
        QStandardItem *item9 = new QStandardItem(QString::fromStdString(rowData.ter));
        QStandardItem *item10 = new QStandardItem(QString::fromStdString(rowData.mad));
        QStandardItem *item11 = new QStandardItem(QString::fromStdString(rowData.saa));

        /*
         * Simply call setItem on model and put the items as the last argument.
         * First arg is row, second arg is column.
         */


        model->setItem(i, 0, item0); // First column (column 0)
        model->setItem(i, 1, item1); // Second column (column 1)
        model->setItem(i, 2, item2); // Third column (column 2)
        model->setItem(i, 3, item3); // Fourth column (column 3)
        model->setItem(i, 4, item4); // Fifth column (column 4)
        model->setItem(i, 5, item5); // Sixth column (column 5)
        model->setItem(i, 6, item6); // Seventh column (column 6)
        model->setItem(i, 7, item7); // Eighth column (column 7)
        model->setItem(i, 8, item8); // Ninth column (column 8)
        model->setItem(i, 9, item9); // Tenth column (column 9)
        model->setItem(i, 10, item10); // Eleventh column (column 10)
        model->setItem(i, 11, item11); // Twelfth column (column 11)
    }


}

void FileHandler::exportFileToCSV() {
    if (!currentFlights.empty()) {

        ofstream file("output.csv");
        file << "fnr,knr,reg,typ,ha0,lsk,stt,itt,pos,ter,mad,saa" << endl;
        for (int i = 0; i < currentFlights.size(); i++) {
            const flights& flight = currentFlights[i];
            file << flight.fnr << "," << flight.knr << "," << flight.reg << "," <<
                flight.typ << "," << flight.ha0 << "," << flight.lsk << "," << flight.stt << "," <<
                flight.itt << "," << flight.pos << "," << flight.ter << "," << flight.mad << "," <<
                flight.saa << endl;
        }
        file.close();

        //simply open the generated csv file in order to let the user save the file elsewhere.
        QUrl fileUrl = QUrl::fromLocalFile("output.csv");
        QDesktopServices::openUrl(fileUrl);
    }
    else {
        UtilityMethods::exc("Bitte zuerst Datei öffnen!");
    }
}












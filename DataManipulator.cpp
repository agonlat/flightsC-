#include "DataManipulator.h"
#include "FileHandler.h"
#include <boost/algorithm/string.hpp>
#include "TimeHandler.h"
#include "utilitymethods.h"
#include "Data.h"
/**
 * @brief Sets the new ITT (intern time) for a specific row. So you do not have to call showFlights to refresh the table.
 * @param itt The new ITT as a string.
 * @param row The row, in which the new ITT has to be refreshed, as an int.
 */



void DataManipulator::setITT(string itt, int row) {
    allFlights[row].itt = itt;

}

/**
 * @brief Sets the new positon of the airplane.
 * @param pos The new position as a string
 * @param row The row, in which the new POS has to be refreshed, as an int.
 */

void DataManipulator::setPOS(string pos, int row) {
    allFlights[row].pos = pos;

}

/**
 * @brief Sets the new terminal of the airplane.
 * @param ter The new terminal as a string.
 * @param row The row, in which the new TER has to be refreshed, as an int.
 */
void DataManipulator::setTER(string ter, int row) {
    allFlights[row].ter = ter;

}

/**
 * @brief setSTA Sets the new status of the airplane.
 * @param sta The status as a string.
 * @param row The row, in which the new SAA has to be refreshed, as an int.
 */

void DataManipulator::setSTA(string sta, int row) {
    allFlights[row].saa = sta;

}

/**
 * @brief Sets label information based on flight details.
 *
 * This function trims the input string and iterates over all flights. If the fnr or knr of a flight matches
 * the input, it constructs a string with various flight details and sets this as the text of the label.
 *
 * @param l The QLabel to be updated.
 * @param input The input string used to match fnr or knr of flights.
 */

void DataManipulator::setLabelInfo(QLabel&l, string& input) {

    boost::algorithm::trim_right(input);

    /*
     * Search the flight with the help of a for loop and if the flight is found,
     * then set the text of the label (the label shows the information of the selected flight)
     * according to the found flight.
     */

    for (int i = 0; i < allFlights.size(); i++) {
        string f = allFlights[i].fnr;
        string k = allFlights[i].knr;
        boost::algorithm::trim_right(f);
        boost::algorithm::trim_right(k);

        f = UtilityMethods::removeExtraSpaces(f);
        if (f== input ||  k== input) {
            string a = allFlights[i].fnr + " "+
                       allFlights[i].knr +" "+
                       allFlights[i].itt +" "+
                       allFlights[i].pos +" "+
                       allFlights[i].ter +" "+
                       allFlights[i].saa;

            l.setText(QString::fromStdString(a));
            return;
        }
    }

}

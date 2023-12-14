#include "Validator.h"
#include "FileHandler.h"
#include "TimeHandler.h"
#include "utilitymethods.h"
#include <set>
#include <sstream>
#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/local_time_adjustor.hpp"
#include <regex>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <sstream>
#include "Data.h"



/**
 * @brief Checks a given number as a string for its validity. The input is a flightNO or ControlNO.
 *        This function uses a regex pattern to match the input.
 *        The input should be like the following examples, in order to be accepted: LH 2309, LH 204 or LH 9851F.
 *        So two chars at the beginning, one space and three or four digits and an optional char at the end.
 *        Be careful! This function calls exc("...") if something went wrong, so it calls an Error Dialog (a small frame
 *        containg some information, what went wrong).
 * @param const input input is the string to be checked
 * @return 0 Returns 0 when there occurs and error or a flight is not found.
 * @return 1 Returns 1 if everything went okay.
 */

int Validator::checkNR(const std::string input) {
    std::regex patternFlight("^(?:[A-Z]{2} [0-9]{3,4}[A-Z]?|[0-9]{6}|[A-Z]{3}[0-9]{4}|[A-Z]{1}[0-9] [0-9]{3}|[A-Z]{1}[0-9] [0-9]{4})$");
    std::string trimmedInput = boost::trim_copy(input);
    // Check if input is empty
    if (input.empty()) {
        return -1;
    }

    // Check if the input doesn't match the pattern
    if (!regex_match(input, patternFlight)) {
        return -2;
    }

    // Check in a for-each loop if the flight exists.
    for (const flights& f : allFlights) {



        string fr = f.fnr;
        string kr = f.knr;

        boost::trim_right(fr);
        boost::trim_right(kr);
        boost::trim_left(fr);
        boost::trim_left(kr);
        fr = UtilityMethods::removeExtraSpaces(fr);

        // If flight found, return 1.
        if (fr == input || kr == input) {

            return 1;  // Found a matching flight
        }
    }

    // If the loop completes without finding a match, return 0.
    return 0;  // Flight not found
}



/**
 * @brief Checks if two strings tim1 and tim2 (in real: new time ITT and old time ITT.
 * @param tim1 new ITT
 * @param tim2 old ITT
 * @return 0 If new tim1 is not valid or at errors
 * @return 1 If new itt is valid and GREATER than the old itt.
 */




int Validator::checkITT(string tim1, string tim2) {

    TimeHandler *handler = new TimeHandler;

    boost::algorithm::trim_right(tim1);
    boost::algorithm::trim_right(tim2);

    std::regex pattern(R"(\b[0-9]{2}\.[0-9]{2}\.(?:[0-9]{4}|[0-9]{2})\s(?:[01]?[0-9]|2[0-3]):[0-5][0-9]:[0-5][0-9]\b)");

    boost::posix_time::time_duration time1;
    boost::posix_time::time_duration time2;
    if (!tim1.empty() && !regex_match(tim1, pattern)) {
        //  exc("Itt im falschen Format");
        return -2;  // Return -2 to indicate an error
    }



    // Check if the input string is empty
    if (tim1.empty()) {
        return 1;  // Return 1 to indicate that it's empty (no error)
    }

    // Now try the time conversion
    try {

        if (tim2=="")
            return 1;

        /*
         * Convert the two times in the format hh:mm:ss to a posix_time in order to
         *        check if the new time is smaller!
         *        Append .000 at the end to match the format of posix_time. 000 at the end actually
         *        represent milliseconds.
         */
        time1 = boost::posix_time::duration_from_string(handler->getLocalTime(tim1)+".000");
        time2 = boost::posix_time::duration_from_string(handler->getLocalTime(tim2)+".000");

        std::istringstream ss1(tim1);
        std::istringstream ss2(tim2);
        string day1 = tim1.substr(0, 2);
        string month1 = tim1.substr(3, 2);
        string year1 = tim1.substr(6, 4);

        string day2 = tim2.substr(0, 2);
        string month2 = tim2.substr(3, 2);
        string year2 = tim2.substr(6, 4);

        int d1 = stoi(day1);
        int m1 = stoi(month1);
        int y1 = stoi(year1);

        int d2 = stoi(day2);
        int m2 = stoi(month2);
        int y2 = stoi(year2);
        if (y1 < y2)
            return 0;
        if (y1 > y2)
            return 1;
        if (y1 == y2 && m1 < m2)
            return 0;
        if (y1 == y2 && m1 > m2)
            return 1;
        if (y1 == y2 && m1 == m2) {
            if (d1 < d2)
                return 0;
            if (d1 > d2)
                return 1;
        }

        // If the date part is equal, compare the time
        cout << time1 <<endl;
        cout << time2 <<endl;
        if (time1 < time2)
            return 0;
        if (time1 > time2)
            return 1;

        return 0;

        /*
         * Catch the bad_lexical_cast exception caused by duration_from_string
         */
    } catch (const boost::bad_lexical_cast&) {
        // Handle any conversion error here
        //  exc("Fehler bei der Zeitkonvertierung");

        return 0;  // Return 0 to indicate an error
    }
}

/**
 * @brief Checks if input (in real:Position) is valid.
 * @param input The input as a string.
 * @return 1 If input is empty.
 * @return 1 If input is non-empty and valid.
 * @return 0 If input is invalid.
 */

int Validator::checkPOS(string input) {
    std::regex pattern("^[0-9]{3}[ABSNWE]?$");


    if (input.empty())
        return 1;

    /*
     * If the input is not empty AND the input doesn't match the regex pattern, then
     * call exc and indicate an error. Else return 1
     */

    if (!input.empty() && !regex_match(input,pattern)) {
        // exc("Position entspricht nicht dem Standardmuster!");
        return -2;

    }

    return 1;

}




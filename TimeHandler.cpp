#include "TimeHandler.h"
#include "FileHandler.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <regex>
#include "Data.h"
using namespace std;
using namespace boost::gregorian;
using namespace boost::posix_time;

/**
 * @brief Converts a local time in the format DD.MM.YYYY HH:MM:SS to UTC time.
 *
 * This function takes a local time string as input, checks if it's not empty, and then extracts and converts the date and time components to UTC.
 * The input string is first trimmed on the right. Then, the year, day, and month are extracted from the date part of the string.
 * The hour, minute, and second are extracted from the time part of the string.
 * A local time is created with these values, and a time zone for Germany (CET+01:00:00) is applied to it.
 * The local time is then converted to UTC and returned as a string.
 * If any part of the date or time is invalid, an error message is shown with exc() and the function returns an empty string.
 *
 * @param input The input local time string in the format DD.MM.YYYY HH:MM:SS.
 * @return The converted UTC time string if the input matches the expected format and all date and time components are valid; otherwise, an empty string.
 */

std::string TimeHandler::getUTC(const std::string& input) {
    std::string trimmedInput = boost::trim_copy(input);

    if (trimmedInput.empty()) {
        return "";
    }

    std::istringstream ss(trimmedInput);
    std::tm timeStruct = {};

    ss >> std::get_time(&timeStruct, "%d.%m.%Y %H:%M:%S");

    if (ss.fail()) {
        return "";
    }

    std::time_t localTime = std::mktime(&timeStruct);

    // Convert to UTC using UTC offset
    std::time_t utcTime = localTime; // Assuming your time is in UTC+01:00

    std::tm* utcTimeStruct = std::gmtime(&utcTime);

    std::stringstream formattedUTC;
    formattedUTC << std::put_time(utcTimeStruct, "%Y-%m-%dT%H:%M:%S+01:00");

    return formattedUTC.str();
}

/**
 * @brief Return the input as a local time in the format hh:mm:ss. The input is in the format
 *        dd:mm:yyyy hh:mm:ss.
 * @param input The input date and time as a string.
 * @return string Returns the local time as a string
 * @return input Returns the input if it cannot find a space in the input.
 */

string TimeHandler::getLocalTime(string input) {

    int spacePos = input.find(' ');
    if (spacePos != std::string::npos && spacePos < input.length()) {
        // Extract the part of the string after the first space

        return input.substr(spacePos + 1);

    }

    return "";
}

/**
 * @brief Extracts the date of a given string in the format dd:mm:yyyy hh:mm:ss and returns it
 * @param input The actual input as a string
 * @return string Returns the date extracted from the input.
 * @return string Return the input if it is empty, so it returns an empty string, which does not affect the
 *         calculations.
 */

string TimeHandler::getDate(string input) {
    boost::algorithm::trim_right(input);
    std::regex pattern(R"(\b[0-9]{2}\.[0-9]{2}\.(?:[0-9]{4}|[0-9]{2})\s(?:[01]?[0-9]|2[0-3]):[0-5][0-9]:[0-5][0-9]\b)");


    /**
     * If the input is not empty and the input matches a date pattern, then
     * extract the day, month and year from the string with substr and form a
     * new date.
     * Then return the date.
     */

    if (!input.empty() && regex_match(input, pattern)) {
        string d = input.substr(0,2);
        string m = input.substr(3,2);
        string y = input.substr(6,4);
        string date = d + "." + m + "." + y;
        return date;
    }


    return "";

}

/**
 * @brief Converts the input date to a specific format.
 *
 * This function takes a date string as input, splits it into components, and then formats it into a specific format.
 * The format is "day.month.year". If the input string is empty, the function returns the current date in the specified format.
 *
 * @param input The input date string.
 * @return The formatted date string.
 */
string TimeHandler::getDateAsFormat() {

    // Get the current date
    date currentDate = day_clock::local_day();

    // Format the date
    std::string dt = boost::lexical_cast<std::string>(currentDate.day()) + "." +
                     boost::lexical_cast<std::string>(currentDate.month()) + "." +
                     boost::lexical_cast<std::string>(currentDate.year());



    return dt;
}

/**
 * @brief This function returns an actual date and the old time using boost date.
 *        So it only refreshes the date, the time is the same as it was.
 * @param input This is the input as format dd.mm.yyyy hh.mm.ss.a
 * @return string Returns the current date
 */
std::string TimeHandler::refreshDate() {
    // Get the current date
    date currentDate = day_clock::local_day();

    // Format day, month, and year with leading zeros if necessary
    std::string dayStr = boost::lexical_cast<std::string>(currentDate.day());
    std::string monthStr = boost::lexical_cast<std::string>(currentDate.month().as_number());
    std::string yearStr = boost::lexical_cast<std::string>(currentDate.year());

    if (dayStr.size() == 1) dayStr = "0" + dayStr;
    if (monthStr.size() == 1) monthStr = "0" + monthStr;

    // Concatenate date components
    std::string dt = dayStr + "." + monthStr + "." + yearStr;

    return dt;
}


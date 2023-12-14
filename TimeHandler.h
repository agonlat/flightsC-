#pragma once
#include <string>

using namespace std;

/**
 * @brief The TimeHandler class provides various methods for time operations.
 *
 * It includes functionalities such as retrieving local time, extracting the date from a time string
 * in the format dd.mm.yyyy hh:mm:ss, obtaining the date with the month represented as a name,
 * retrieving the UTC time of a local time, and getting the current date.
 */

class TimeHandler {
public:
    string getLocalTime(string input);
    string getDate(string input);
    string getDateAsFormat();
    string getUTC(const std::string& input);
    string refreshDate();
};



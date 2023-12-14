#ifndef UTILITYMETHODS_H
#define UTILITYMETHODS_H
#include <string>
#include <map>

/**
 * @brief The UtilityMethods class provides convenient helper methods, which are needed for other operations.
 */

class UtilityMethods
{
public:
    static std::string removeExtraSpaces(std::string s);
    static void exc(std::string info);
    static std::string getAirline(std::string str);
    static std::string findAirlineName_and_mostFlights(const std::map<std::string, int>& inputMap);
};

#endif // UTILITYMETHODS_H

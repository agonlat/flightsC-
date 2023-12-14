#include "utilitymethods.h"
#include <string>
#include <QMessageBox>
#include <QString>
#include "Data.h"
/**
 * @brief Removes extra spaces from the given input string.
 *
 * This method eliminates extra spaces within the input string. It loops through the characters and skips whitespace characters according to certain conditions.
 * At the end only one space is permitted and left within the string. If we have LH  2309 and call this method on this string,
 * it returns LH 2309, so one space less.
 * @param s The input string containing spaces to be removed.
 * @return The string with extra spaces removed.
 */
std::string UtilityMethods::removeExtraSpaces(std::string s) {
    std::string nstr;
        //loop through the characters of the input string
        for(int i=0; i<s.length();  ){
            //check if character is white space
            if(s[i] == ' '){
                /*
              *do not include the white space, if-
              *it is at the trailing or leading position
            */
                if(i==0 || i==s.length()-1){
                    i++;
                    continue;
                }

                /*
              *if space is inbetween then skip it-
              *except the last occurrence
            */
                while(s[i+1] == ' ')
                    i++;
            }

            //concatenate the character to the new string
            nstr += s[i++];
        }

        return nstr;
    }

 /**
 * @brief Opens a QMessageBox that shows information that is given as argument. It has an error style.
 * @param info This is the information that has to be displayed
 */

    void UtilityMethods::exc(std::string info) {
        QMessageBox box;
        box.setWindowTitle("Error");
        box.setIcon(QMessageBox::Critical);
        box.setText(QString::fromStdString(info));
        box.exec();
    }

    string UtilityMethods::getAirline(std::string str) {
        string s;
        for (char& c : str) {
            if (isalpha(c)) {
                s = s + c;
            }
        }

        return s;
    }
    /**
     * @brief Finds the airline which operates the most flights in the table of all flights and returns it in
     *        the format <[airlineName]: [flights]>
     * @param inputMap
     * @return
     */

    std::string UtilityMethods::findAirlineName_and_mostFlights(const std::map<std::string, int>& inputMap) {
        int maxFlightCount = -1; // Initialize with a smaller value
        std::string maxAirlineCode;

        for (const auto& entry : inputMap) {
            if (entry.second > maxFlightCount) {
                maxFlightCount = entry.second;
                maxAirlineCode = entry.first;
            }
        }

        return maxAirlineCode + ": " + std::to_string(maxFlightCount);
    }





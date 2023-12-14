#pragma once
#include <string>

using namespace std;

/**
 * @brief The Validator class provides methods to check various strings, if they are valid, such as flight number or control number,
 *        intern time and the position.
 */

class Validator {
public:
    static int checkNR(string input);
    static int checkITT(string tim1, string tim2);
    static int checkPOS(string input);

};


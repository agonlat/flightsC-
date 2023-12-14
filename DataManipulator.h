#pragma once
#include <string>
#include <QLabel>
using namespace std;

/**
 * @brief The DataManipulator class provies convenient methods to manipulate data in the current flights table.
 *        You are able to set a new intern time, new terminal, new status, new position and you can provide a label with the flight information.
 *        Specifically, the label is located in the "Flug ändern" section, so in the Dialog, where you can change a flight.
 *        And the Dialog is implemented in mainwindow.cpp.
 */

class DataManipulator {
public:
    void setITT(string itt, int row);
    void setPOS(string pos, int row);
    void setLabelInfo(QLabel&l, string& input);
    void setTER(string ter, int row);
    void setSTA(string sta, int row);
};



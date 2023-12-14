#include "mainwindow.h"

#include <QApplication>
#include <QFont>
#include <QColor>
#include <QIcon>

int main(int argc, char *argv[])
{
    /*
     * Here start the main program. The QApplication class manages the GUI application's control flow and main settings.
     * MainWindow is the frame, which is visible for the user.
     *
     */

    QApplication a(argc, argv);
    MainWindow w;


    QFont font;
    QIcon icon(":/Munich_airport_logo.svg.png");

    /*
     * Now we make some adjustments to the font variable, regarding to the design.
     */
    font.setFamily("Simplified Arabic");
    font.setBold(true);
    font.setPointSize(14);

    /*
     * The following adjustments are for the MainWindow variable @var v, specifically for the GUI window.
     */

    w.setStyleSheet("QLabel {color:white;}");
    w.setFont(font);
    w.setWindowTitle("Flughafen Anzeigetafel");
    w.setWindowIcon(icon);
    /*
     * Finally, show the window
    */
    w.show();

    return a.exec();
}

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QComboBox>
#include <QListWidgetItem>
#include <QSpinBox>
#include <QStandardItemModel>
#include "FileHandler.h"
#include <iostream>
#include <QProcess>
#include <QDir>
#include <fstream>
#include <QVBoxLayout>
#include <QDialog>
#include <QPushButton>
#include <QFileDialog>
#include <QUrl>
#include <QColorDialog>
#include <QDesktopServices>
#include <boost/algorithm/string.hpp>
#include <QMessageBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <regex>
#include "Validator.h"
#include "TimeHandler.h"
#include "DataManipulator.h"
#include "customitemdelegate.h"
#include <QToolTip>
#include "utilitymethods.h"
#include "Data.h"
#include <QSortFilterProxyModel>

string path;
bool del = false;
bool combo = false;

QStandardItemModel *model = nullptr;
DataManipulator * data_manipulator = new DataManipulator;
QColor color1;
QColor color2;
int changedFlights;
bool utc = false;
bool local = false;
int S;
bool all = false;

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    CustomItemDelegate* itemDelegate = new CustomItemDelegate;
    //Apply the painting of the rows to the table
    ui->tableView->setItemDelegate(itemDelegate);
    color1.setRgb(2,55,185);
    color2.setRgb(1,126,236);

    //Make some adjustments, like removing the grid of the table and prevent editing in the cells
    ui->tableView->setShowGrid(false);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //set the model with 12 cols and rows like allFlights has.
    //The model is like a layer you put to a QTableView, so we can display some header information
    //and specify the columns, rows ...
    model = new QStandardItemModel(allFlights.size(),12);
    model->setHorizontalHeaderLabels(
            {"FNR", "KNR", "REG", "TYP","HA0","LSK", "STT", "ITT", "POS", "TER", "MAD", "SAA" });


    ui->tableView->setModel(model);
    // Attach the model to the view




   // ui->tableView->setModel(model);
    // ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    for (int i = 0; i <12; i++)
        ui->tableView->setColumnWidth(i, 123);



    ui->tableView->update();
    ui->tableView->show();



}

MainWindow::~MainWindow()
{
    delete ui;
}


/**
 * @brief This slot performs some operations to open a file dialog and then it calls readFileToVec to store the data to
 *        a vector. So it basically retrieves the path of the file and calls readFileToVec.
 */

void MainWindow::on_actionDatei_einlesen_triggered()
{
    FileHandler *fileHandler = new FileHandler;
    utc = false;
    local = false;


    //  make some adjustments, add icons ...
    QDialog dialog;
    dialog.setMinimumWidth(150);
    dialog.setWindowTitle("Datei einlesen");
    dialog.setWindowIcon(QIcon(":/csv.png"));
    QPushButton buttonDatei("");
    buttonDatei.setIcon(QIcon(":/csv.png"));
    QVBoxLayout layout (&dialog);


    //If the button buttonDatei is clicked, then the following operations are done.
    QObject::connect(&buttonDatei, &QPushButton::clicked, this, [this,  &dialog] () {
        FileHandler * fileHandler = new FileHandler;
        QString path2 = QFileDialog::getOpenFileName(this, tr("CSV-Datei öffnen"), "", tr("Dateien (*.csv)"));
        path = path2.toStdString();

        int check;

        if (path != "") {
            allFlights.clear();
            currentFlights.clear();
            model->clear();
            ui->tableView->setModel(model);
            model->setHorizontalHeaderLabels(
                    {"FNR", "KNR", "REG", "TYP","HA0","LSK", "STT", "ITT", "POS", "TER", "MAD", "SAA" });
            for (int i = 0; i <12; i++)
                ui->tableView->setColumnWidth(i, 123);

            check =  fileHandler->readFileToVec(path);
            if (check == 1) {

                utc = false;
                local = false;
                all = true;
                currentFlights = allFlights;
                currentFlightsCOPY = currentFlights;


                fileHandler->showFlights(currentFlights);
                QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel;
                proxyModel->setSourceModel(model);

                // Set the proxy model to the table view
                ui->tableView->setModel(proxyModel);

                // Enable filtering on the table view
                ui->tableView->setSortingEnabled(true); // Optionally, enable sorting

                dialog.close();
            } else if (check == -1){
                UtilityMethods::exc("Datei enthält falsche Formattierung!");

            } else {
                 UtilityMethods::exc("Datei konnte nicht geöffnet werden!");
            }
        }
        else
            dialog.close();

    });

    layout.addWidget(&buttonDatei);
    dialog.exec();

}

/**
 * @brief This slot export the actual table as a csv file in the same format as the uploaded table.
 */
void MainWindow::on_actionExportieren_triggered()
{
    FileHandler * fileHandler = new FileHandler;
    fileHandler->exportFileToCSV();

}


/**
 * @brief This slot opens a dialog with some checkboxes and a text field. In this slot operations are performed
 *        such as filtering the flights, showing utc time, showing local time or showing all flights.
 *        The checkboxes and the text field are connected to some event, which are expressed as lambda expressions
 */


void MainWindow::on_actionAnzeige_triggered()
{
    //Create the elements QDialog (contains all checkboxes and the text field), QCheckBox and QLineEdit

    QDialog dialog;
    dialog.setWindowTitle("Anzeige");
    dialog.setWindowIcon(QIcon(":/monitor.png"));
    QVBoxLayout layout (&dialog);

    QCheckBox alleFlüge("Alle Flüge anzeigen");
    QCheckBox lokaleZeit("Lokale Zeit anzeigen");
    QCheckBox utcZeit("UTC anzeigen");

    QLabel label("Filtern nach Flug");
    QLineEdit input;

    //perform some design operations; does not affec the functionality
    layout.addWidget(&alleFlüge);
    layout.addWidget(&lokaleZeit);
    layout.addWidget(&utcZeit);
    layout.addSpacing(10);
    layout.addWidget(&label);
    layout.addWidget(&input);


    //If alleFlüge checkbox is clicked, perform the following operations
    QObject::connect(&alleFlüge, &QCheckBox::stateChanged, this, [ this,&alleFlüge, &input, &utcZeit, &lokaleZeit]() {
    FileHandler * fileHandler = new FileHandler;
        if (currentFlights.empty()) {
            alleFlüge.setChecked(false);
             UtilityMethods::exc("Datensatz laden!");
            return;
        }

        //if the checkbox is checked then show currentFlights, because currentFlights is first set to allFlights!
        if (all == false && alleFlüge.isChecked()) {
            input.setText("");
            utcZeit.setChecked(false);
            lokaleZeit.setChecked(false);
            local = false;
            utc = false;
            currentFlights = allFlights;  // Set currentFlights to allFlights
            fileHandler->showFlights(currentFlights);
        }
    });



//if lokaleZeit checkbox is clicked then perform the following operations
    QObject::connect(&lokaleZeit, &QCheckBox::stateChanged, this, [this, &lokaleZeit, &utcZeit, &input]() {
        S = currentFlights.size();

        if (currentFlights.empty()) {
            lokaleZeit.setChecked(false);
             UtilityMethods::exc("Datensatz laden!");
            return;
        }

        /* If lokaleZeit ist checked then set utcZeit checkbox to false, because local time and utc are not allowed to be checked both
         * Then go with a for loop in the currentFlightsCOPY vector which is a copy of the currentFlights vector but with local times.
         * So the copy is assigned before utc was clicked. So it contains the local Times.
         */



        if (lokaleZeit.isChecked() && !local) {

            local = true;
            utc = false;
            utcZeit.setChecked(false);
            for (int i = 0; i < S; i++) {


                currentFlights[i].stt = currentFlightsCOPY[i].stt;
                currentFlights[i].itt = currentFlightsCOPY[i].itt;

                model->setItem(i, 6, new QStandardItem(QString::fromStdString(currentFlights[i].stt)));
                model->setItem(i, 7, new QStandardItem(QString::fromStdString(currentFlights[i].itt)));

            }


        }


        for (int i = 0; i <12; i++)
            ui->tableView->setColumnWidth(i, 123);



    });

    // Connect the stateChanged signal of utcZeit to a lambda function


    QObject::connect(&utcZeit, &QCheckBox::stateChanged, this, [this, &utcZeit, &lokaleZeit]() {
        TimeHandler * handler = new TimeHandler;
        S = currentFlights.size();
        if (currentFlights.empty()) {
            utcZeit.setChecked(false);
             UtilityMethods::exc("Datensatz laden!");
            return;
        }

        if (utcZeit.isChecked() && !utc) {


            utc = true;
            lokaleZeit.setChecked(false);


            // Ensure the loop iterates properly through your data
            for (int i = 0; i < S; i++) {

                    cout << currentFlights[i].stt;
                    currentFlights[i].stt = handler->getUTC(currentFlights[i].stt);
                    model->setItem(i, 6, new QStandardItem(QString::fromStdString(currentFlights[i].stt)));




                    currentFlights[i].itt = handler->getUTC(currentFlights[i].itt);

                    model->setItem(i, 7, new QStandardItem(QString::fromStdString(currentFlights[i].itt)));

            }

            // Adjust column widths
            ui->tableView->setColumnWidth(4, 80);
            ui->tableView->setColumnWidth(5, 80);
            ui->tableView->setColumnWidth(6, 163);
            ui->tableView->setColumnWidth(7, 163);

            // For debugging purposes, log after the conversion

        }

        ui->tableView->update();
    });



    // Connect the textChanged signal of input to a lambda function
    QObject::connect(&input, &QLineEdit::textChanged, this, [&input, &alleFlüge, this]() {
FileHandler *fileHandler = new FileHandler;
        regex reg("[A-Z]{2}[A-Z]?|[A-Z][0-9]");
        QString searchString = input.text();
        searchString = searchString.toUpper();
        string m = searchString.toStdString().substr(0, 2);
        string m2 = searchString.toStdString().substr(0, 3);
        if (regex_match(m, reg) || regex_match(m2, reg)) {
            std::vector<flights> filteredFlights;

            for (const flights& flight : allFlights) {
                std::string flightNumber = flight.fnr;
                std::transform(flightNumber.begin(), flightNumber.end(), flightNumber.begin(), ::toupper);

                if (flightNumber.compare(0, 2, searchString.toStdString()) == 0 || flightNumber.compare(0, 3, searchString.toStdString()) == 0) {
                    filteredFlights.push_back(flight);
                    S++;
                }
            }

            if (filteredFlights.empty()) {
                 UtilityMethods::exc("Airline nicht gefunden!");
            } else {
                // Updating model and table
                model->clear();
                ui->tableView->setModel(model);
                model->setHorizontalHeaderLabels({"FNR", "KNR", "REG", "TYP", "HA0", "LSK", "STT", "ITT", "POS", "TER", "MAD", "SAA"});
                for (int i = 0; i < 12; i++)
                    ui->tableView->setColumnWidth(i, 123);

                // Update flights data, ensuring sizes match and clearing the table if needed
                currentFlights = filteredFlights;
                if (filteredFlights.size() < model->rowCount()) {
                    model->removeRows(0, model->rowCount());
                }
                fileHandler->showFlights(filteredFlights);

                // Reset the time display flags
                utc = false;
                local = false;
                all = false;
                alleFlüge.setChecked(false);
            }
        }
    });





    dialog.exec();




}

/**
 * @brief This slot shows the window for changing a specific flight. It has some text fields for the input and a button to
 * confirm the update.
 */
void MainWindow::on_actionFlug_ndern_triggered()
{
    QDialog dialog;
    dialog.setWindowIcon(QIcon(":/edit.png"));
    dialog.setWindowTitle("Flug ändern");
    QGridLayout layout(&dialog);
//Creating the labels
    QLabel NR("Flugnummer oder Kontrollnummer");
    QLabel ITT("Neue ITT");
    QLabel POS("Neue Position");
    QLabel TER("Neuer Terminal");
    QLabel SAA("Neue SAA");
    QLabel info("");



//Creating the text fields
    QLineEdit NR_EDIT;
    QLineEdit ITT_EDIT;
    QLineEdit POS_EDIT;
    QComboBox TER_EDIT;
    QComboBox SAA_EDIT;

    QString tooltipText =
                          "Airline:\n"
                          "- 3-stelliges Kürzel\n"
                          "- Bei Bedarf Leerzeichen an 3. Stelle\n"
                          "- Großbuchstaben oder Zahlen\n"
                          "Streckennummer:\n"
                          "- 4-stellige Nummer\n"
                          "- Nur numerisch\n"
                          "Flugfunktion:\n"
                          "- Entweder ein Großbuchstabe oder ein Leerzeichen\n"
                          "Muss immer vorhanden sein\n"
                          "Beispiele: LH 2309, SRR6188, LH 2075, TG 924\n"
                          "Aufbau der Kontrollnummer:";
                          "6-stellige, eindeutige ID eines Fluges. Muss immer vorhanden sein"
                          ;



    NR_EDIT.setToolTip(tooltipText);

    tooltipText = "Interne Zeit für Onblock bzw. Offblock Format DD.MM.YYYY hh24:mm:ss\n"
                  "Kann leer sein!";





    ITT_EDIT.setToolTip(tooltipText);

    tooltipText = "4-stellige Abstellposition am Vorfeld\n"
                  "Leerzeichen rechts\n"
                  "Großbuchstaben und Zahlen\n"
                  "Kann leer sein\n"
                  "Beispiele: 145, 545E";

    POS_EDIT.setToolTip(tooltipText);


    ITT_EDIT.installEventFilter(&ITT_EDIT);
    NR_EDIT.installEventFilter(&NR_EDIT);
    POS_EDIT.installEventFilter(&POS_EDIT);

    QStringList values = {
        "01", "03", "05", "15", "20", "22", "23", "25", "27", "30",
        "35", "39", "43", "46", "48", "49", "51", "53", "56", "57",
        "60", "61", "63", "64", "65", "66", "67", "72", "80", "86",
        "88", "94", "96", "97", "98", "99"
    };

    // Add specific values to the combo box
    SAA_EDIT.addItems(values);
    TER_EDIT.addItem("1");
    TER_EDIT.addItem("2");





    QPushButton ändern("Ändern");
    QPushButton abbrechen("Abbrechen");
    ändern.setEnabled(false);


//adding the text fields and the button to layout of the window, in this case it is a gridlayout
    layout.addWidget(&NR, 0,0);
    layout.addWidget(&NR_EDIT,0,1);

    layout.addWidget(&ITT,1,0);
    layout.addWidget(&ITT_EDIT,1,1);

    layout.addWidget(&POS,2,0);
    layout.addWidget(&POS_EDIT,2,1);

    layout.addWidget(&TER,3,0);
    layout.addWidget(&TER_EDIT,3,1);

    layout.addWidget(&SAA,4,0);
    layout.addWidget(&SAA_EDIT,4,1);

    layout.setRowStretch(5, 1);

// Add the INFO button to the next row
    layout.addWidget(&info, 6, 0, 1, 2);
    layout.addWidget(&abbrechen, 7,0);
    layout.addWidget(&ändern, 7, 1);

        QObject::connect(&abbrechen, &QPushButton::clicked, this, [&dialog]() {
        QMessageBox box;
        box.setWindowTitle("Information");
        box.setIcon(QMessageBox::Question);
        box.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);

        if (allFlights.empty() ) {

            box.setText("Wollen Sie wirklich abbrechen?");

            int ret = box.exec();

            if (ret == QMessageBox::Ok) {

                dialog.close();
            } else if (ret == QMessageBox::Cancel) {
                box.close();
            }


        } else {
            box.setText("Wollen Sie wirklich abbrechen? Die Änderungen gehen verloren!");

            int ret = box.exec();

            if (ret == QMessageBox::Ok) {

                dialog.close();
            } else if (ret == QMessageBox::Cancel) {
                box.close();
            }
        }


    });





QObject::connect(&NR_EDIT, &QLineEdit::editingFinished, this, [&info, &NR_EDIT, &TER_EDIT, &SAA_EDIT]() {
        if (allFlights.empty()&& !NR_EDIT.text().isEmpty()) {
            UtilityMethods::exc("Datensatz laden!");
            return;
        }

        string input = NR_EDIT.text().toStdString();


            int check = Validator::checkNR(NR_EDIT.text().toStdString());

            if (check == 1) {

                string text = NR_EDIT.text().toStdString();
                data_manipulator->setLabelInfo(info, text);

                for (flights& f : allFlights) {

                    string NR = NR_EDIT.text().toStdString();
                    string fr = f.fnr;
                    string kr = f.knr;
                    boost::algorithm::trim_right(fr);
                    boost::algorithm::trim_right(kr);

                    if (fr == NR || kr == NR) {

                        TER_EDIT.setCurrentText(QString::fromStdString(f.ter));
                        SAA_EDIT.setCurrentText(QString::fromStdString(f.saa));
                        return;
                    }
                }

            } else if (check == 0) {
                UtilityMethods::exc("Flug nicht gefunden!");
            } else if (check == -2) {
                UtilityMethods::exc("Flugnummer/Kontrollnummer ist im falschen Format!");
            }

});


//the same as previous is for all text fields. If the user want to write something into the fields, but the data set
//is empty, then the same error occurs.
    QObject::connect(&ITT_EDIT, &QLineEdit::textChanged, this, [&info, &ITT_EDIT, &NR_EDIT, &TER_EDIT, &SAA_EDIT, &ändern, &abbrechen]() {
        string input = NR_EDIT.text().toStdString();

        if (allFlights.empty() && !ITT_EDIT.text().isEmpty()) {
            ITT_EDIT.setText("");
             UtilityMethods::exc("Datensatz laden!");
        } else {
            int check = Validator::checkNR(NR_EDIT.text().toStdString());

            if (check == 1) {
                ändern.setEnabled(true);

                string text = NR_EDIT.text().toStdString();
                data_manipulator->setLabelInfo(info, text);

                for (flights& f : allFlights) {

                    string NR = NR_EDIT.text().toStdString();
                    string fr = f.fnr;
                    string kr = f.knr;
                    boost::algorithm::trim_right(fr);
                    boost::algorithm::trim_right(kr);

                    if (fr == NR || kr == NR) {

                        TER_EDIT.setCurrentText(QString::fromStdString(f.ter));
                        SAA_EDIT.setCurrentText(QString::fromStdString(f.saa));
                        return;
                    }
                }

            } else if (check == 0) {
                 UtilityMethods::exc("Flug nicht gefunden!");
            } else if (check == -2) {
                 UtilityMethods::exc("Flugnummer/Kontrollnummer ist im falschen Format!");
            }
        }
    });


QObject::connect(&ITT_EDIT, &QLineEdit::editingFinished, this, [&info, &ITT_EDIT, &ändern, &abbrechen]() {
    // Check if the QLineEdit text is empty after editing is finished
    if (ITT_EDIT.text().isEmpty()) {
        ändern.setEnabled(false);

    }
});


/** @brief Similar blocks of code are found in POS_EDIT connection, TER_EDIT connection and STA_EDIT conn.
 * So the logic is that NR_EDIT is mandatory, so the flight number, but if the user typed in the flight no.
 * but only wants  to change some of the values, not all, then every changed text field, should call the method
 * to set the label with the current flight information. It is a design concept.
 * For instance, I filled in the flight no, then when I only want to change the STA (Status) then when I start typing in
 * STA_EDIT the program checks if the flight no exists and if it is correct, so after every change in a block it is checked,
 * for security reasons, to not write a wrong no.
 *
 */
    QObject::connect(&POS_EDIT, &QLineEdit::textChanged, this, [&info, &POS_EDIT, &NR_EDIT, &SAA_EDIT, &TER_EDIT,&ändern, &abbrechen]() {
        string input = NR_EDIT.text().toStdString();


        int check = Validator::checkNR(NR_EDIT.text().toStdString());
        if (allFlights.empty()&& !POS_EDIT.text().isEmpty()) {
            POS_EDIT.setText("");
             UtilityMethods::exc("Datensatz laden!");
            return;
        }
        else if (!allFlights.empty()&& !NR_EDIT.text().isEmpty() && check) {
            ändern.setEnabled(true);

            string text = NR_EDIT.text().toStdString();
            data_manipulator->setLabelInfo(info, text);
            for (flights& f : allFlights) {
                string NR = NR_EDIT.text().toStdString();
                string fr = f.fnr;
                string kr = f.knr;
                boost::algorithm::trim_right(fr);
                boost::algorithm::trim_right(kr);

                if (fr == NR || kr == NR) {

                    TER_EDIT.setCurrentText(QString::fromStdString(f.ter));
                    SAA_EDIT.setCurrentText(QString::fromStdString(f.saa));
                    return;
                }

        }
        }

    });

QObject::connect(&POS_EDIT, &QLineEdit::editingFinished, this, [&info, &POS_EDIT, &ändern, &abbrechen]() {
    // Check if the QLineEdit text is empty after editing is finished
    if (POS_EDIT.text().isEmpty()) {
        ändern.setEnabled(false);

    }

});

    connect(&TER_EDIT, QOverload<int>::of(&QComboBox::currentIndexChanged), [&](int /*index*/) {
        ändern.setEnabled(true);

    });

    connect(&SAA_EDIT, QOverload<int>::of(&QComboBox::currentIndexChanged),[&](int /*index*/) {
        ändern.setEnabled(true);

    });



//When button "bestätigen" is clicked, the program performs the final checkings, like checking NR_EDIT, or ITT_EDIT and so on...
//Then it checks if a field is not empty and if so, it sets the new specific value.

    QObject::connect(&ändern, &QPushButton::clicked, this, [this,&NR_EDIT, &ITT_EDIT, &POS_EDIT, &TER_EDIT, &SAA_EDIT, &dialog]() {



        if (allFlights.empty() ) {
             UtilityMethods::exc("Datensatz laden!");
            return;
        }

        QMessageBox box;

        box.setIcon(QMessageBox::Question);
        box.setText("Wollen Sie die Daten wirklich ändern?");
         box.setWindowTitle("Information");
        box.setStandardButtons(QMessageBox::Cancel |QMessageBox::Ok);

        int ret = box.exec();

        if (ret == QMessageBox::Cancel) {

            box.close();
            return;
        }
        else {

            box.close();

        }








        int row = -1;

        // Get values from QLineEdit fields
        string NR = NR_EDIT.text().toStdString();
        string ITT = ITT_EDIT.text().toStdString();
        string POS = POS_EDIT.text().toStdString();
        string TER = TER_EDIT.currentText().toStdString();
        string STA = SAA_EDIT.currentText().toStdString();

        // Trim whitespace from the values
        boost::algorithm::trim_right(NR);
        boost::algorithm::trim_right(ITT);
        boost::algorithm::trim_right(POS);
        boost::algorithm::trim_right(TER);
        boost::algorithm::trim_right(STA);

        // Check if NR is empty (mandatory)
        if (NR.empty()) {
            UtilityMethods::exc("Flugnummer/Kontrollnummer darf nicht leer sein!");
            return;
        }

        if (NR != "" && ITT == "" && POS == "" && TER == "" && STA == "") {
            QMessageBox box;
            box.setWindowTitle("Information");
            box.setText("Wollen Sie etwas ändern? Daten sind leer!");
            box.setIcon(QMessageBox::Question);
            box.exec();
            return;
        }


        if (Validator::checkNR(NR)) {
            for (flights& f : allFlights) {
                row++;
                string fr = f.fnr;
                string kr = f.knr;
                boost::algorithm::trim_right(fr);
                boost::algorithm::trim_right(kr);

                if (fr == NR || kr == NR) {
                    int ittCheck = Validator::checkITT(ITT, f.itt);
                    int posCheck = Validator::checkPOS(POS);


                    if (ittCheck == 1 && posCheck == 1) {
                        if (!ITT.empty()) {
                            // Set the ITT
                            string s = ITT;
                            data_manipulator->setITT(s, row);
                            model->setItem(row, 7, new QStandardItem(QString::fromStdString(s)));
                        }

                        if (!POS.empty()) {
                            // Set the POS
                            string s = POS;
                            data_manipulator->setPOS(s, row);
                            model->setItem(row, 8, new QStandardItem(QString::fromStdString(s)));
                        }

                        if (!TER.empty()) {
                            // Set the TER
                            string s = TER;
                            data_manipulator->setTER(s, row);
                            model->setItem(row, 9, new QStandardItem(QString::fromStdString(s)));
                        }

                        if (!STA.empty()) {
                            // Set the STA
                            string s = STA;
                            data_manipulator->setSTA(s, row);
                            model->setItem(row, 11, new QStandardItem(QString::fromStdString(s)));
                        }

                        // Show a success message
                        QMessageBox message;
                        message.setText("Flug erfolgreich geändert!");
                        message.setWindowTitle("Erfolg");
                        message.setIcon(QMessageBox::Information);
                        message.exec();
                        currentFlights = allFlights;
                        changedFlights++;
                        return;  // Exit if everything is successful
                    } else {
                        // Handle errors

                        if (ittCheck == 0) {
                             UtilityMethods::exc("Neue Zeit darf nicht kleiner oder gleich sein!");
                        } else if (ittCheck == -2) {
                             UtilityMethods::exc("Neue Zeit ist im falschen Format!");
                        } else if (posCheck == -2) {
                             UtilityMethods::exc("POS entspricht nicht dem Standardmuster!");
                        }


                        return;  // Exit after handling error
                    }
                }
            }

            // Show an error message if no flights match
             UtilityMethods::exc("Flug nicht gefunden!");
            return;
        }
    });

    dialog.exec();

}







void MainWindow::on_actionAnzeige_2_triggered()
{


    QDialog dialog;
    dialog.setWindowTitle("Farbeinstellungen");
    dialog.setWindowIcon(QIcon(":/colour.png"));
    QPushButton farbe1("Erste Farbe wählen");
    QPushButton farbe2("Zweite Farbe wählen");
        QVBoxLayout *layout =  new QVBoxLayout();
    dialog.setLayout(layout);
        layout->addWidget(&farbe1);
    layout->addWidget(&farbe2);


    QObject::connect(&farbe1, &QPushButton::clicked, this, [this]() {
         color1 = QColorDialog::getColor(Qt::white, this, "Bitte erste Farbe wählen");
    });

    QObject::connect(&farbe2, &QPushButton::clicked, this, [this]() {
        color2 = QColorDialog::getColor(Qt::white, this, "Bitte zweite Farbe wählen");
    });

    dialog.setMinimumWidth(150);

    dialog.exec();

}


void MainWindow::on_actionStatistiken_triggered()
{
    QDialog dialog;
    dialog.setWindowIcon(QIcon(":/analytics.png"));
    dialog.setWindowTitle("Statistiken");

    QVBoxLayout * layout = new QVBoxLayout();
    dialog.setLayout(layout);


    QLabel lab1("Anzahl aller Flüge");
    QLabel lab2;
    QLabel lab3("Anzahl der Änderungen");
    QLabel lab4;
    QLabel lab5("Airline mit meisten Flügen");
    QLabel lab6;

    string all = std::to_string(allFlightsCount);
    string changed =  std::to_string(changedFlights);
    string airline = UtilityMethods::findAirlineName_and_mostFlights(airlineCounter);


    lab2.setText(QString::fromStdString(all));
    lab4.setText(QString::fromStdString(changed));
    if (currentFlights.empty())
    lab6.setText(QString::fromStdString(""));
    else
      lab6.setText(QString::fromStdString(airline));

    layout->addWidget(&lab1);
    layout->addWidget(&lab2);
    layout->addWidget(&lab3);
    layout->addWidget(&lab4);
    layout->addWidget(&lab5);
    layout->addWidget(&lab6);

    dialog.exec();

}


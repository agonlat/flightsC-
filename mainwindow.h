#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTableView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief The MainWindow class operates as the class where nearly all main operations are executed. In this class
 *        there also the slots defined and the connection methods, which connect a signal with a slot.
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Ui::MainWindow *ui;



private slots:
    void on_actionDatei_einlesen_triggered();

    void on_actionExportieren_triggered();

    void on_actionAnzeige_triggered();

    void on_actionFlug_ndern_triggered();

    void on_actionAnzeige_2_triggered();

    void on_actionStatistiken_triggered();

private:

};
#endif // MAINWINDOW_H

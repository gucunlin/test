#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QSettings> // Added
#include <QString>   // Added
#include <QProcess>  // Added for later use

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots: // Added for button clicks
    void onModuleButton1Clicked();
    void onModuleButton2Clicked();

private:
    void loadConfig(); // Method to load configuration

    // UI Elements
    QLabel *titleLabel;
    QPushButton *moduleButton1;
    QPushButton *moduleButton2;

    // Layouts
    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;
    QWidget *centralWidget;

    // Configuration
    QString module1ExePath;
    QString module2ExePath;

    QProcess *process; // For launching executables
};
#endif // MAINWINDOW_H

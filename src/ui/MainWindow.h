#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QToolBar>
#include <QAction>
#include <QStatusBar>
#include "core/Board.h"
#include "TaskDialog.h"
#include "DeveloperDialog.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private slots:
    void createTask();
    void addDeveloper();
    void showAbout();

private:
    QTabWidget* tabWidget;
    Board* scrumBoard;
    QToolBar* toolBar;
    
    void setupUI();
    void createToolBar();
    void createSampleData();
};

#endif // MAINWINDOW_H
#include "MainWindow.h"
#include <QApplication>
#include <QStyle>
#include <QFile>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setupUI();
    createToolBar();
    createSampleData();
    
    setWindowTitle("Scrum Board");
    setMinimumSize(1000, 700);
}

void MainWindow::setupUI() {
    tabWidget = new QTabWidget(this);
    scrumBoard = new Board();
    
    tabWidget->addTab(scrumBoard, "Scrum �����");
    
    setCentralWidget(tabWidget);
    
    // �������� ������
    QFile styleFile(":/styles.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        qApp->setStyleSheet(styleSheet);
    }
    
    statusBar()->showMessage("������");
}

void MainWindow::createToolBar() {
    toolBar = addToolBar("��������");
    
    QAction* addTaskAction = new QAction("�������� ������", this);
    QAction* addDeveloperAction = new QAction("�������� ������������", this);
    QAction* aboutAction = new QAction("� ���������", this);
    
    toolBar->addAction(addTaskAction);
    toolBar->addAction(addDeveloperAction);
    toolBar->addSeparator();
    toolBar->addAction(aboutAction);
    
    connect(addTaskAction, &QAction::triggered, this, &MainWindow::createTask);
    connect(addDeveloperAction, &QAction::triggered, this, &MainWindow::addDeveloper);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
}

void MainWindow::createTask() {
    TaskDialog dialog(scrumBoard->getDevelopers(), this);
    if (dialog.exec() == QDialog::Accepted) {
        Task* task = dialog.getTask();
        if (task) {
            scrumBoard->addTask(task);
            statusBar()->showMessage("������ ���������: " + task->getTitle());
        }
    }
}

void MainWindow::addDeveloper() {
    DeveloperDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Developer* developer = dialog.getDeveloper();
        if (developer) {
            scrumBoard->addDeveloper(developer);
            statusBar()->showMessage("����������� ��������: " + developer->getName());
        }
    }
}

void MainWindow::showAbout() {
    QMessageBox::about(this, "� ���������", 
        "Scrum Board v1.0\n\n"
        "������� ���������� �������� �� ����������� Scrum.\n"
        "�����������:\n"
        "- �������� � ���������� ��������\n"
        "- ���������� ������������\n"
        "- ������������ �������� �����\n"
        "- ���������� workflow ����� �������");
}

void MainWindow::createSampleData() {
    // ������� �������������
    Developer* dev1 = new Developer("���� ������", "�����������");
    Developer* dev2 = new Developer("����� ��������", "�����������");
    Developer* dev3 = new Developer("������� ������", "��������");
    
    scrumBoard->addDeveloper(dev1);
    scrumBoard->addDeveloper(dev2);
    scrumBoard->addDeveloper(dev3);
    
    // ������� ������
    Task* task1 = new Task("����������� �����������", "�������������� ����� ����������� ����������", dev1->getId());
    Task* task2 = new Task("�������� �����", "������� unit-����� ��� �������� �������", dev2->getId());
    Task* task3 = new Task("������� ����������", "�������� �������� � ����������", dev3->getId());
    Task* task4 = new Task("Code review", "��������� ��� �� ������������ ����������");
    
    scrumBoard->addTask(task1);
    scrumBoard->addTask(task2);
    scrumBoard->addTask(task3);
    scrumBoard->addTask(task4);
}
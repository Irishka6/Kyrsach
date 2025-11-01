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
    
    tabWidget->addTab(scrumBoard, "Scrum Доска");
    
    setCentralWidget(tabWidget);
    
    // Загрузка стилей
    QFile styleFile(":/styles.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        qApp->setStyleSheet(styleSheet);
    }
    
    statusBar()->showMessage("Готово");
}

void MainWindow::createToolBar() {
    toolBar = addToolBar("Основные");
    
    QAction* addTaskAction = new QAction("Добавить задачу", this);
    QAction* addDeveloperAction = new QAction("Добавить разработчика", this);
    QAction* aboutAction = new QAction("О программе", this);
    
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
            statusBar()->showMessage("Задача добавлена: " + task->getTitle());
        }
    }
}

void MainWindow::addDeveloper() {
    DeveloperDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Developer* developer = dialog.getDeveloper();
        if (developer) {
            scrumBoard->addDeveloper(developer);
            statusBar()->showMessage("Разработчик добавлен: " + developer->getName());
        }
    }
}

void MainWindow::showAbout() {
    QMessageBox::about(this, "О программе", 
        "Scrum Board v1.0\n\n"
        "Система управления задачами по методологии Scrum.\n"
        "Возможности:\n"
        "- Создание и управление задачами\n"
        "- Назначение исполнителей\n"
        "- Визуализация статусов задач\n"
        "- Управление workflow через стрелки");
}

void MainWindow::createSampleData() {
    // Создаем разработчиков
    Developer* dev1 = new Developer("Иван Петров", "Разработчик");
    Developer* dev2 = new Developer("Мария Сидорова", "Тестировщик");
    Developer* dev3 = new Developer("Алексей Козлов", "Аналитик");
    
    scrumBoard->addDeveloper(dev1);
    scrumBoard->addDeveloper(dev2);
    scrumBoard->addDeveloper(dev3);
    
    // Создаем задачи
    Task* task1 = new Task("Разработать архитектуру", "Спроектировать общую архитектуру приложения", dev1->getId());
    Task* task2 = new Task("Написать тесты", "Создать unit-тесты для основных модулей", dev2->getId());
    Task* task3 = new Task("Собрать требования", "Провести интервью с заказчиком", dev3->getId());
    Task* task4 = new Task("Code review", "Проверить код на соответствие стандартам");
    
    scrumBoard->addTask(task1);
    scrumBoard->addTask(task2);
    scrumBoard->addTask(task3);
    scrumBoard->addTask(task4);
}
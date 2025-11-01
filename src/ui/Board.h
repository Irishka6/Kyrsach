#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QList>
#include <QMap>
#include "Task.h"
#include "Developer.h"

class Board : public QWidget {
    Q_OBJECT

public:
    explicit Board(QWidget* parent = nullptr);
    void addTask(Task* task);
    void addDeveloper(Developer* developer);
    void updateArrows();
    const QMap<int, Developer*>& getDevelopers() const { return developers; } // Добавленный метод

private slots:
    void toggleArrow(int arrowIndex);

private:
    QHBoxLayout* mainLayout;
    QList<QVBoxLayout*> columnLayouts;
    QList<QLabel*> columnHeaders;
    QList<QPushButton*> arrows;
    QMap<TaskStatus, QList<Task*>> tasksByStatus;
    QMap<int, Developer*> developers;
    
    void setupColumns();
    void setupArrows();
    void refreshColumns();
    QString getStatusText(TaskStatus status) const;
};

#endif // BOARD_H
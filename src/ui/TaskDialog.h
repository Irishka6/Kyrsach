#ifndef TASKDIALOG_H
#define TASKDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QFormLayout>
#include "../core/Task.h"
#include "../core/Developer.h"

class TaskDialog : public QDialog {
    Q_OBJECT

public:
    explicit TaskDialog(const QMap<int, Developer*>& developers, QWidget* parent = nullptr);
    Task* getTask() const;
    
private slots:
    void createTask();

private:
    QLineEdit* titleEdit;
    QTextEdit* descriptionEdit;
    QComboBox* developerCombo;
    QPushButton* createButton;
    QPushButton* cancelButton;
    Task* createdTask;
    
    void setupUI(const QMap<int, Developer*>& developers);
};

#endif // TASKDIALOG_H
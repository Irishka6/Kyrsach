#ifndef DEVELOPERDIALOG_H
#define DEVELOPERDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QFormLayout>
#include "../core/Developer.h"

class DeveloperDialog : public QDialog {
    Q_OBJECT

public:
    explicit DeveloperDialog(QWidget* parent = nullptr);
    Developer* getDeveloper() const;
    
private slots:
    void createDeveloper();

private:
    QLineEdit* nameEdit;
    QComboBox* roleCombo;
    QPushButton* createButton;
    QPushButton* cancelButton;
    Developer* createdDeveloper;
    
    void setupUI();
};

#endif // DEVELOPERDIALOG_H
#include "TaskDialog.h"
#include <QLabel>
#include <QMessageBox>

TaskDialog::TaskDialog(const QMap<int, Developer*>& developers, QWidget* parent) 
    : QDialog(parent), createdTask(nullptr) {
    setupUI(developers);
}

void TaskDialog::setupUI(const QMap<int, Developer*>& developers) {
    setWindowTitle("������� ������");
    setModal(true);
    setMinimumWidth(400);
    
    QFormLayout* formLayout = new QFormLayout(this);
    
    titleEdit = new QLineEdit(this);
    descriptionEdit = new QTextEdit(this);
    descriptionEdit->setMaximumHeight(100);
    
    developerCombo = new QComboBox(this);
    developerCombo->addItem("�� ��������", -1);
    
    for (Developer* dev : developers) {
        developerCombo->addItem(dev->getName() + " - " + dev->getRole(), dev->getId());
    }
    
    formLayout->addRow("��������:", titleEdit);
    formLayout->addRow("��������:", descriptionEdit);
    formLayout->addRow("�����������:", developerCombo);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    createButton = new QPushButton("�������", this);
    cancelButton = new QPushButton("������", this);
    
    buttonLayout->addWidget(createButton);
    buttonLayout->addWidget(cancelButton);
    
    formLayout->addRow(buttonLayout);
    
    connect(createButton, &QPushButton::clicked, this, &TaskDialog::createTask);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void TaskDialog::createTask() {
    QString title = titleEdit->text().trimmed();
    if (title.isEmpty()) {
        QMessageBox::warning(this, "������", "������� �������� ������");
        return;
    }
    
    QString description = descriptionEdit->toPlainText();
    int developerId = developerCombo->currentData().toInt();
    
    createdTask = new Task(title, description, developerId);
    accept();
}

Task* TaskDialog::getTask() const {
    return createdTask;
}
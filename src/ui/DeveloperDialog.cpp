#include "DeveloperDialog.h"
#include <QLabel>
#include <QMessageBox>

DeveloperDialog::DeveloperDialog(QWidget* parent) 
    : QDialog(parent), createdDeveloper(nullptr) {
    setupUI();
}

void DeveloperDialog::setupUI() {
    setWindowTitle("�������� ������������");
    setModal(true);
    
    QFormLayout* formLayout = new QFormLayout(this);
    
    nameEdit = new QLineEdit(this);
    
    roleCombo = new QComboBox(this);
    roleCombo->addItem("�����������");
    roleCombo->addItem("�����������");
    roleCombo->addItem("��������");
    roleCombo->addItem("��������");
    roleCombo->addItem("��������");
    
    formLayout->addRow("���:", nameEdit);
    formLayout->addRow("����:", roleCombo);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    createButton = new QPushButton("��������", this);
    cancelButton = new QPushButton("������", this);
    
    buttonLayout->addWidget(createButton);
    buttonLayout->addWidget(cancelButton);
    
    formLayout->addRow(buttonLayout);
    
    connect(createButton, &QPushButton::clicked, this, &DeveloperDialog::createDeveloper);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void DeveloperDialog::createDeveloper() {
    QString name = nameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "������", "������� ��� ������������");
        return;
    }
    
    QString role = roleCombo->currentText();
    createdDeveloper = new Developer(name, role);
    accept();
}

Developer* DeveloperDialog::getDeveloper() const {
    return createdDeveloper;
}
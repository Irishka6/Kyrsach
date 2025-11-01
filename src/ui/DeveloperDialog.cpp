#include "DeveloperDialog.h"
#include <QLabel>
#include <QMessageBox>

DeveloperDialog::DeveloperDialog(QWidget* parent) 
    : QDialog(parent), createdDeveloper(nullptr) {
    setupUI();
}

void DeveloperDialog::setupUI() {
    setWindowTitle("Добавить разработчика");
    setModal(true);
    
    QFormLayout* formLayout = new QFormLayout(this);
    
    nameEdit = new QLineEdit(this);
    
    roleCombo = new QComboBox(this);
    roleCombo->addItem("Разработчик");
    roleCombo->addItem("Тестировщик");
    roleCombo->addItem("Аналитик");
    roleCombo->addItem("Дизайнер");
    roleCombo->addItem("Менеджер");
    
    formLayout->addRow("Имя:", nameEdit);
    formLayout->addRow("Роль:", roleCombo);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    createButton = new QPushButton("Добавить", this);
    cancelButton = new QPushButton("Отмена", this);
    
    buttonLayout->addWidget(createButton);
    buttonLayout->addWidget(cancelButton);
    
    formLayout->addRow(buttonLayout);
    
    connect(createButton, &QPushButton::clicked, this, &DeveloperDialog::createDeveloper);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void DeveloperDialog::createDeveloper() {
    QString name = nameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите имя разработчика");
        return;
    }
    
    QString role = roleCombo->currentText();
    createdDeveloper = new Developer(name, role);
    accept();
}

Developer* DeveloperDialog::getDeveloper() const {
    return createdDeveloper;
}
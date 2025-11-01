#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // ������� ������� ����
    QMainWindow window;
    QWidget* centralWidget = new QWidget();
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    
    // �������� ������� �� ������� ��� � �����������
    QStringList columnNames = {"���������", "� ������", "�����������", "���������"};
    
    for (int i = 0; i < 4; i++) {
        // ������� �������
        QVBoxLayout* columnLayout = new QVBoxLayout();
        
        // ��������� �������
        QLabel* header = new QLabel(columnNames[i]);
        header->setAlignment(Qt::AlignCenter);
        header->setStyleSheet("QLabel { background-color: #e0e0e0; padding: 15px; font-weight: bold; border: 2px solid #ccc; border-radius: 5px; font-size: 14px; }");
        
        // ������� �������
        QLabel* content = new QLabel("������ �������� �����");
        content->setAlignment(Qt::AlignCenter);
        content->setStyleSheet("QLabel { padding: 20px; color: #666; }");
        
        columnLayout->addWidget(header);
        columnLayout->addWidget(content);
        columnLayout->addStretch();
        
        QFrame* columnFrame = new QFrame();
        columnFrame->setLayout(columnLayout);
        columnFrame->setStyleSheet("QFrame { background-color: white; border: 2px solid #ccc; border-radius: 8px; margin: 5px; }");
        columnFrame->setMinimumWidth(200);
        columnFrame->setMinimumHeight(400);
        
        mainLayout->addWidget(columnFrame);
        
        // ��������� ������� ����� ���������
        if (i < 3) {
            QPushButton* arrow = new QPushButton("?");
            arrow->setProperty("direction", "right");
            arrow->setStyleSheet("QPushButton { font-size: 24px; padding: 10px 15px; background-color: #4CAF50; color: white; border: none; border-radius: 5px; } QPushButton:hover { background-color: #45a049; }");
            
            // ���������� ����� ��� ����� ����������� �������
            QObject::connect(arrow, &QPushButton::clicked, [arrow]() {
                if (arrow->property("direction").toString() == "right") {
                    arrow->setText("?");
                    arrow->setProperty("direction", "left");
                } else {
                    arrow->setText("?");
                    arrow->setProperty("direction", "right");
                }
            });
            
            mainLayout->addWidget(arrow);
        }
    }
    
    window.setCentralWidget(centralWidget);
    window.setWindowTitle("Scrum Board - ������ �����");
    window.resize(1000, 600);
    window.show();
    
    return app.exec();
}
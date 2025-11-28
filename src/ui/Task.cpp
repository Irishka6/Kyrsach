#include "Task.h"

Task::Task(int id, const std::string& taskText, sf::Font& font, float x, float y) 
    : id(id), currentSection(0), isMoving(false), developerName(""), description(taskText) {
    
    shape.setSize(sf::Vector2f(400, 85));
    shape.setFillColor(sf::Color(200, 220, 240));
    shape.setPosition(x, y);
    
    text.setString(taskText);
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color(50, 50, 80));
    
    // Текст разработчика
    developerText.setFont(font);
    developerText.setCharacterSize(16);
    developerText.setFillColor(sf::Color(100, 100, 130));
    developerText.setString("");
    
    // Инициализация смещения текста
    textOffset = sf::Vector2f(0, 0);
    
    updateTextPosition();
}

void Task::setDeveloperName(const std::string& name, sf::Font& font) {
    developerName = name;
    developerText.setFont(font);
    developerText.setCharacterSize(16);
    developerText.setFillColor(sf::Color(80, 80, 120));
    developerText.setStyle(sf::Text::Bold);
    developerText.setString(developerName);
    
    // Обновляем позицию
    sf::FloatRect taskBounds = shape.getGlobalBounds();
    sf::FloatRect textBounds = developerText.getLocalBounds();
    
    developerText.setPosition(
        taskBounds.left + taskBounds.width - textBounds.width - 10,  // Справа с отступом
        taskBounds.top + taskBounds.height - textBounds.height - 5   // Снизу с отступом
    );
}

void Task::updateTextPosition() {
    sf::FloatRect taskBounds = shape.getGlobalBounds();
    sf::FloatRect textBounds = text.getLocalBounds();
    
    // Центрируем основной текст задачи
    float textX = taskBounds.left + (taskBounds.width - textBounds.width) / 2;
    float textY = taskBounds.top + (taskBounds.height - textBounds.height) / 2 - 10;
    
    text.setPosition(textX, textY);
    
    // Сохраняем смещение для корректного перемещения
    textOffset = sf::Vector2f(
        (taskBounds.width - textBounds.width) / 2,
        (taskBounds.height - textBounds.height) / 2 - 10
    );
    
    // Обновляем позицию текста разработчика
    if (!developerName.empty()) {
        sf::FloatRect devTextBounds = developerText.getLocalBounds();
        developerText.setPosition(
            taskBounds.left + taskBounds.width - devTextBounds.width - 10,
            taskBounds.top + taskBounds.height - devTextBounds.height - 5
        );
    }
}

void Task::setPosition(float x, float y) {
    shape.setPosition(x, y);
    
    // Обновляем позицию текста с сохранением относительного смещения
    sf::FloatRect taskBounds = shape.getGlobalBounds();
    text.setPosition(
        taskBounds.left + textOffset.x,
        taskBounds.top + textOffset.y
    );
    
    // Обновляем позицию текста разработчика
    if (!developerName.empty()) {
        sf::FloatRect devTextBounds = developerText.getLocalBounds();
        developerText.setPosition(
            taskBounds.left + taskBounds.width - devTextBounds.width - 10,
            taskBounds.top + taskBounds.height - devTextBounds.height - 5
        );
    }
}
#include "Task.h"

Task::Task(int taskId, const std::string& taskName, sf::Font& font, float x, float y) 
    : id(taskId), isMoving(false), currentSection(0) {
    
    // Инициализация формы
    shape.setSize(sf::Vector2f(350, 85));
    shape.setFillColor(sf::Color(200, 220, 240));
    shape.setOutlineColor(sf::Color(150, 170, 190));
    shape.setOutlineThickness(2);
    shape.setPosition(x, y);
    
    // Инициализация текста
    text.setString(taskName);
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color(50, 50, 80));
    text.setPosition(x + 10, y + 10);
}

void Task::setPosition(float x, float y) {
    shape.setPosition(x, y);
    text.setPosition(x + 10, y + 10);
}
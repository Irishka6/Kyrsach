#include "Task.h"

Task::Task(const std::string& taskText, sf::Font& font, float x, float y) 
    : description(taskText), currentSection(0), isMoving(false) {
    
    shape.setSize(sf::Vector2f(340, 80));
    shape.setFillColor(sf::Color(255, 255, 255, 250)); // Белый
    shape.setOutlineColor(sf::Color(200, 210, 220));   // Светло-серая обводка
    shape.setOutlineThickness(2);
    shape.setPosition(x, y);
    
    text.setString(taskText);
    text.setFont(font);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color(40, 40, 100)); // Темно-синий текст
    
    updateTextPosition();
}

void Task::updateTextPosition() {
    sf::FloatRect textBounds = text.getLocalBounds();
    sf::Vector2f shapePos = shape.getPosition();
    text.setPosition(shapePos.x + (340 - textBounds.width) / 2, shapePos.y + 30);
}

void Task::setPosition(float x, float y) {
    shape.setPosition(x, y);
    updateTextPosition();
}
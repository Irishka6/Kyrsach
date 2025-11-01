#include "Task.h"

Task::Task(const std::string& taskText, sf::Font& font, float x, float y) 
    : description(taskText), currentSection(0), isMoving(false) {
    
    shape.setSize(sf::Vector2f(280, 70)); // Увеличиваем размер задач
    shape.setFillColor(sf::Color(255, 255, 255, 240));
    shape.setOutlineColor(sf::Color(100, 149, 237));
    shape.setOutlineThickness(2);
    shape.setPosition(x, y);
    
    text.setString(taskText);
    text.setFont(font);
    text.setCharacterSize(16); // Увеличиваем шрифт
    text.setFillColor(sf::Color(25, 25, 112));
    
    updateTextPosition();
}

void Task::updateTextPosition() {
    sf::FloatRect textBounds = text.getLocalBounds();
    sf::Vector2f shapePos = shape.getPosition();
    text.setPosition(shapePos.x + (280 - textBounds.width) / 2, shapePos.y + 25);
}

void Task::setPosition(float x, float y) {
    shape.setPosition(x, y);
    updateTextPosition();
}
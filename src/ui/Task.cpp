#include "Task.h"

Task::Task(const std::string& taskText, sf::Font& font, float x, float y) 
    : description(taskText), currentSection(0), isMoving(false) {
    
    shape.setSize(sf::Vector2f(380, 80));
    shape.setFillColor(sf::Color(255, 255, 255, 250));
    shape.setOutlineColor(sf::Color(200, 210, 220));
    shape.setOutlineThickness(2);
    shape.setPosition(x, y);
    
    text.setString(taskText);
    text.setFont(font);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color(40, 40, 100));
    
    updateTextPosition();
}

void Task::updateTextPosition() {
    sf::FloatRect textBounds = text.getLocalBounds();
    sf::Vector2f shapePos = shape.getPosition();
    
    float textX = shapePos.x + (shape.getSize().x - textBounds.width) / 2;
    float textY = shapePos.y + (shape.getSize().y - textBounds.height) / 2 - 5;
    
    text.setPosition(textX, textY);
}

void Task::setPosition(float x, float y) {
    shape.setPosition(x, y);
    updateTextPosition();
}
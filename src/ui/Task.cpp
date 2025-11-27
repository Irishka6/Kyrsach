#include "Task.h"

Task::Task(int taskId, const std::string& taskName, sf::Font& font, float x, float y) 
    : id(taskId), isMoving(false), currentSection(0), textOffset(10, 10) {  // ДОБАВЬТЕ textOffset ЗДЕСЬ
    
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
    
    // Центрирование текста и сохранение смещения
    sf::FloatRect textBounds = text.getLocalBounds();
    sf::FloatRect shapeBounds = shape.getGlobalBounds();
    
    // Расчет смещения для центрирования текста
    textOffset.x = (shapeBounds.width - textBounds.width) / 2;
    textOffset.y = (shapeBounds.height - textBounds.height) / 2 - 5;
    
    text.setPosition(x + textOffset.x, y + textOffset.y);
}

void Task::setPosition(float x, float y) {
    shape.setPosition(x, y);
    
    // Обновляем позицию текста с сохраненным смещением
    text.setPosition(x + textOffset.x, y + textOffset.y);
}

void Task::updateTextPosition() {
    // Метод для обновления позиции текста при изменении размера задачи
    sf::FloatRect textBounds = text.getLocalBounds();
    sf::FloatRect shapeBounds = shape.getGlobalBounds();
    
    textOffset.x = (shapeBounds.width - textBounds.width) / 2;
    textOffset.y = (shapeBounds.height - textBounds.height) / 2 - 5;
    
    text.setPosition(shapeBounds.left + textOffset.x, shapeBounds.top + textOffset.y);
}
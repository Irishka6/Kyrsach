#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Task.h"
#include "Arrow.h"

class ScrumBoard {
private:
    std::vector<Task> tasks;
    std::vector<Arrow> arrows;
    
    // Колонки Scrum доски
    sf::RectangleShape todoColumn;
    sf::RectangleShape inProgressColumn;
    sf::RectangleShape doneColumn;
    
    // Заголовки колонок
    sf::Text todoText;
    sf::Text inProgressText;
    sf::Text doneText;
    
    // Шрифт
    sf::Font font;
    
    // Параметры прокрутки для каждой колонки
    float todoScrollOffset;
    float inProgressScrollOffset;
    float doneScrollOffset;
    float maxScrollOffset;
    
    // Переменные для перетаскивания
    bool isDragging;
    sf::Vector2f dragStartPosition;
    float dragStartScrollOffset;
    int draggedColumn; // -1: нет, 0: todo, 1: inProgress, 2: done
    
public:
    ScrumBoard();
    bool initialize();
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    
private:
    void setupColumn(sf::RectangleShape& column, float x, sf::Color color);
    void setupText(sf::Text& text, const std::string& content, float x);
    void updateTaskPositions();
    float calculateMaxScrollOffset(const std::vector<Task>& tasksInColumn);
};
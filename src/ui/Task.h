#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Task {
public:
    sf::RectangleShape shape;
    sf::Text text;
    int currentSection;
    bool isMoving;
    std::string description;
    
    Task(const std::string& taskText, sf::Font& font, float x, float y);
    void updateTextPosition();
    void setPosition(float x, float y);
};
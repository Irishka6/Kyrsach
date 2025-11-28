#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Task {
public:
    int id;
    sf::RectangleShape shape;
    sf::Text text;
    sf::Text developerText;  
    int currentSection;
    bool isMoving;
    std::string description;
    sf::Vector2f textOffset;  
    std::string developerName;  // Имя разработчика
    
    Task(int id, const std::string& taskText, sf::Font& font, float x, float y);
    void updateTextPosition();
    void setPosition(float x, float y);
    void setDeveloperName(const std::string& name, sf::Font& font);  
};
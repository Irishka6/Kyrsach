#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Task.h"
#include "Arrow.h"
#include "WaveAnimation.h"

class ScrumBoard {
public:
    std::vector<std::string> sectionNames;
    std::vector<sf::RectangleShape> sections;
    std::vector<sf::Text> sectionTexts;
    std::vector<std::vector<Task>> tasks;
    std::vector<Arrow> arrows;
    WaveAnimation waveAnimation;
    sf::Font font;
    sf::Text titleText;
    
    ScrumBoard();
    bool initialize();
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void addTask(const std::string& taskName, int section);
    
private:
    void createSections();
    void createArrows();
    void createSampleTasks();
    void createTitle();
};
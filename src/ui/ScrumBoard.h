#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Task.h"

class ScrumBoard {
public:
    std::vector<std::string> sectionNames;
    std::vector<sf::RectangleShape> sections;
    std::vector<sf::Text> sectionTexts;
    std::vector<std::vector<Task>> tasks;
    sf::Font font;
    sf::Text titleText;
    
    // Для прокрутки
    std::vector<float> scrollOffsets;
    std::vector<bool> isDraggingScroll;
    std::vector<sf::Vector2f> dragStartPositions;
    int draggingTaskSection;
    int draggingTaskIndex;
    
    // Для верхней панели и компаний
    sf::RectangleShape topPanel;
    sf::RectangleShape companyButton;
    sf::Text companyButtonText;
    bool showCompanyWindow;
    sf::RectangleShape companyWindow;
    sf::RectangleShape dividerLine; // ДОБАВЬТЕ ЭТУ СТРОКУ
    std::vector<std::string> companies;
    std::vector<sf::RectangleShape> companyRects;
    std::vector<sf::Text> companyTexts;
    
    ScrumBoard();
    bool initialize();
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void addTask(const std::string& taskName, int section);
    void updateTaskPositions();
    
private:
    void createSections();
    void createSampleTasks();
    void createTitle();
    void createTopPanel();
    void createCompanyWindow();
};
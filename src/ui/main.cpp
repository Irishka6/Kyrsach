#include <SFML/Graphics.hpp>
#include "ScrumBoard.h"

int main() {
    setlocale(LC_ALL, "Russian");
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Scrum Board - Управление задачами", sf::Style::Default);
    
    ScrumBoard scrumBoard;
    if (!scrumBoard.initialize()) {
        return -1;
    }
    
    // Идеальный фон для голубых разделов - очень светлый серо-бежевый
    sf::RectangleShape background(sf::Vector2f(1920, 1080));
    background.setFillColor(sf::Color(245, 242, 240)); // #F5F2F0
    
    sf::Clock clock;
    
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            
            scrumBoard.handleEvent(event, window);
        }
        
        scrumBoard.update(deltaTime);
        
        // Рисуем идеальный фон
        window.draw(background);
        
        // Потом все остальное
        scrumBoard.draw(window);
        window.display();
    }
    
    return 0;
}
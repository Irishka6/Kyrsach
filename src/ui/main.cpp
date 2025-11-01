#include <SFML/Graphics.hpp>
#include "ScrumBoard.h"

int main() {
    setlocale(LC_ALL, "Russian");
    // Создаем окно во весь экран или большое окно
    sf::RenderWindow window(sf::VideoMode(1600, 900), "Scrum Board - Управление задачами", sf::Style::Default);
    
    ScrumBoard scrumBoard;
    if (!scrumBoard.initialize()) {
        return -1;
    }
    
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
        
        window.clear(sf::Color(240, 248, 255)); // Нежно-голубой фон
        scrumBoard.draw(window);
        window.display();
    }
    
    return 0;
}
#include <SFML/Graphics.hpp>
#include "ScrumBoard.h"

int main() {
    // Создание главного окна приложения
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Scrum Board - Task Management", sf::Style::Default);
    
    // Создание и инициализация доски Scrum
    ScrumBoard scrumBoard;
    if (!scrumBoard.initialize()) {
        return -1;
    }
    
    // Создание фона окна
    sf::RectangleShape background(sf::Vector2f(1920, 1080));
    background.setFillColor(sf::Color(245, 242, 240));
    
    // Создание часов для измерения времени между кадрами
    sf::Clock clock;
    
    // Главный цикл приложения
    while (window.isOpen()) {
        // Измерение времени, прошедшего с последнего кадра
        float deltaTime = clock.restart().asSeconds();
        
        // Обработка всех событий окна
        sf::Event event;
        while (window.pollEvent(event)) {
            // Закрытие окна при нажатии на крестик
            if (event.type == sf::Event::Closed)
                window.close();
            
            // Передача события на обработку в доску Scrum
            scrumBoard.handleEvent(event, window);
        }
        
        // Обновление состояния доски Scrum
        scrumBoard.update(deltaTime);
        
        // Отрисовка кадра
        window.draw(background);
        scrumBoard.draw(window);
        window.display();
    }
    
    return 0;
}
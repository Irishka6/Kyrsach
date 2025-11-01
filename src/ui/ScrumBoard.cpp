#include "ScrumBoard.h"
#include <iostream>

ScrumBoard::ScrumBoard() {
    sectionNames = {"Назначено", "В процессе", "Блокировано", "Готово"};
    tasks.resize(4);
}

bool ScrumBoard::initialize() {
    const char* fontPaths[] = {
        "Roboto_Condensed-Regular.ttf",
        "Roboto-Regular.ttf", 
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/tahoma.ttf",
        nullptr
    };
    
    bool fontLoaded = false;
    for (int i = 0; fontPaths[i]; i++) {
        if (font.loadFromFile(fontPaths[i])) {
            fontLoaded = true;
            std::cout << "Шрифт загружен: " << fontPaths[i] << std::endl;
            break;
        }
    }
    
    if (!fontLoaded) {
        std::cout << "Не удалось загрузить шрифт!" << std::endl;
        return false;
    }
    
    createTitle();
    createSections();
    createArrows();
    createSampleTasks();
    waveAnimation.initialize(450, 450); // Новые координаты для волн
    
    return true;
}

void ScrumBoard::createTitle() {
    titleText.setString("Scrum Board - Управление задачами");
    titleText.setFont(font);
    titleText.setCharacterSize(32);
    titleText.setFillColor(sf::Color(25, 25, 112));
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(550, 20);
}

void ScrumBoard::createSections() {
    // Новые нежные цвета - убрали красный и сделали все в пастельных тонах
    sf::Color sectionColors[] = {
        sf::Color(173, 216, 230), // Нежно-голубой
        sf::Color(221, 160, 221), // Светло-фиолетовый (заменяем красный)
        sf::Color(255, 218, 185), // Персиковый (заменяем зеленый)  
        sf::Color(152, 251, 152)  // Светло-зеленый
    };
    
    sf::Color outlineColors[] = {
        sf::Color(70, 130, 180),   // Стальной синий
        sf::Color(186, 85, 211),   // Средний фиолетовый
        sf::Color(210, 180, 140),  // Бронзовый
        sf::Color(60, 179, 113)    // Морской зеленый
    };
    
    for (int i = 0; i < 4; i++) {
        // Увеличиваем размеры разделов для большого окна
        sf::RectangleShape section;
        section.setSize(sf::Vector2f(320, 650));
        section.setFillColor(sectionColors[i]);
        section.setOutlineColor(outlineColors[i]);
        section.setOutlineThickness(4);
        section.setPosition(80 + i * 360, 100); // Новые позиции
        sections.push_back(section);
        
        // Текст названия раздела
        sf::Text text;
        text.setString(sectionNames[i]);
        text.setFont(font);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color(25, 25, 112));
        text.setStyle(sf::Text::Bold);
        
        // Центрируем текст
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition(80 + i * 360 + (320 - textBounds.width) / 2, 65);
        sectionTexts.push_back(text);
    }
}

void ScrumBoard::createArrows() {
    // Новые позиции стрелок для большого окна
    for (int i = 0; i < 3; i++) {
        arrows.push_back(Arrow(420 + i * 360, 450, true)); // Исправленные координаты
    }
}

void ScrumBoard::createSampleTasks() {
    // Очищаем старые задачи
    for (int i = 0; i < 4; i++) {
        tasks[i].clear();
    }
    
    // Задачи для первого раздела "Назначено"
    addTask("Создать дизайн интерфейса", 0);
    addTask("Написать основной код", 0);
    addTask("Протестировать функционал", 0);
    addTask("Добавить анимации", 0);
    
    // Задачи для второго раздела "В процессе" 
    addTask("Реализовать перетаскивание", 1);
    addTask("Настроить цвета", 1);
    
    // Задачи для третьего раздела "Блокировано"
    addTask("Исправить баги", 2);
    addTask("Оптимизировать код", 2);
    
    // Задачи для четвертого раздела "Готово"
    addTask("Создать проект", 3);
    addTask("Настроить сборку", 3);
}

void ScrumBoard::addTask(const std::string& taskName, int section) {
    if (section >= 0 && section < 4) {
        float x = 100 + section * 360;
        float y = 120 + tasks[section].size() * 80; // Больше расстояние между задачами
        Task newTask(taskName, font, x, y);
        tasks[section].push_back(newTask);
    }
}

void ScrumBoard::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
        
        for (auto& arrow : arrows) {
            if (arrow.bounds.contains(mousePos)) {
                arrow.toggle();
                
                if (&arrow == &arrows[0]) {
                    waveAnimation.start();
                }
            }
        }
        
        for (int i = 0; i < 4; i++) {
            for (auto& task : tasks[i]) {
                if (task.shape.getGlobalBounds().contains(mousePos)) {
                    task.isMoving = true;
                }
            }
        }
    }
    
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        for (int i = 0; i < 4; i++) {
            for (auto& task : tasks[i]) {
                task.isMoving = false;
            }
        }
    }
    
    if (event.type == sf::Event::MouseMoved) {
        for (int i = 0; i < 4; i++) {
            for (auto& task : tasks[i]) {
                if (task.isMoving) {
                    task.setPosition(event.mouseMove.x - 105, event.mouseMove.y - 30);
                }
            }
        }
    }
}

void ScrumBoard::update(float deltaTime) {
    waveAnimation.update(deltaTime);
}

void ScrumBoard::draw(sf::RenderWindow& window) {
    window.draw(titleText);
    
    for (const auto& section : sections) {
        window.draw(section);
    }
    
    for (const auto& text : sectionTexts) {
        window.draw(text);
    }
    
    for (const auto& arrow : arrows) {
        window.draw(arrow.shape);
    }
    
    for (int i = 0; i < 4; i++) {
        for (const auto& task : tasks[i]) {
            window.draw(task.shape);
            window.draw(task.text);
        }
    }
    
    waveAnimation.draw(window);
}
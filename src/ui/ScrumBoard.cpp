#include "ScrumBoard.h"
#include <iostream>

ScrumBoard::ScrumBoard() {
    sectionNames = {"Назначено", "В процессе", "Блокировано", "Готово"};
    tasks.resize(4);
    scrollOffsets.resize(4, 0.0f);
    isDraggingScroll.resize(4, false);
    dragStartPositions.resize(4, sf::Vector2f(0, 0));
    draggingTaskSection = -1;
}

bool ScrumBoard::initialize() {
    if (!font.loadFromFile("Roboto-Regular.ttf")) {
        std::cout << "Не удалось загрузить шрифт Roboto-Regular.ttf!" << std::endl;
        return false;
    }
    
    std::cout << "Шрифт Roboto-Regular.ttf загружен успешно!" << std::endl;
    
    createTitle();
    createSections();
    createSampleTasks();
    
    return true;
}

void ScrumBoard::createTitle() {
    titleText.setString("Scrum Board - Управление задачами");
    titleText.setFont(font);
    titleText.setCharacterSize(36);
    titleText.setFillColor(sf::Color(50, 50, 100));
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(600, 30);
}

void ScrumBoard::createSections() {
    sf::Color sectionColors[] = {
        sf::Color(180, 210, 235),
        sf::Color(160, 195, 225),
        sf::Color(140, 180, 215),
        sf::Color(120, 165, 205)
    };
    
    float sectionWidth = 420.0f;
    float spacing = 40.0f;
    float startX = 80.0f;
    
    for (int i = 0; i < 4; i++) {
        sf::RectangleShape section;
        section.setSize(sf::Vector2f(sectionWidth, 800));
        section.setFillColor(sectionColors[i]);
        section.setOutlineThickness(0);
        section.setPosition(startX + i * (sectionWidth + spacing), 120);
        sections.push_back(section);
        
        sf::Text text;
        text.setString(sectionNames[i]);
        text.setFont(font);
        text.setCharacterSize(28);
        text.setFillColor(sf::Color(50, 50, 80));
        text.setStyle(sf::Text::Bold);
        
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition(startX + i * (sectionWidth + spacing) + (sectionWidth - textBounds.width) / 2, 80);
        sectionTexts.push_back(text);
    }
}

void ScrumBoard::createSampleTasks() {
    for (int i = 0; i < 4; i++) {
        tasks[i].clear();
    }
    
    // Назначено
    addTask("Создать дизайн интерфейса", 0);
    addTask("Написать основной код", 0);
    addTask("Протестировать функционал", 0);
    addTask("Добавить анимации", 0);
    addTask("Оптимизировать производительность", 0);
    addTask("Разработать API", 0);
    
    // В процессе
    addTask("Реализовать перетаскивание", 1);
    addTask("Настроить цвета", 1);
    addTask("Добавить новые функции", 1);
    
    // Блокировано
    addTask("Исправить критические баги", 2);
    addTask("Обновить документацию", 2);
    
    // Готово
    addTask("Создать проект", 3);
    addTask("Настроить сборку", 3);
    addTask("Протестировать на разных устройствах", 3);
    
    updateTaskPositions();
}

void ScrumBoard::addTask(const std::string& taskName, int section) {
    if (section >= 0 && section < 4) {
        float sectionWidth = 420.0f;
        float spacing = 40.0f;
        float startX = 80.0f;
        float taskWidth = 380.0f;
        
        float x = startX + section * (sectionWidth + spacing) + (sectionWidth - taskWidth) / 2;
        float y = 150 + tasks[section].size() * 90 - scrollOffsets[section];
        
        Task newTask(taskName, font, x, y);
        newTask.currentSection = section;
        tasks[section].push_back(newTask);
    }
}

void ScrumBoard::updateTaskPositions() {
    float sectionWidth = 420.0f;
    float spacing = 40.0f;
    float startX = 80.0f;
    float taskWidth = 380.0f;
    
    for (int section = 0; section < 4; section++) {
        float startY = 150.0f;
        for (size_t i = 0; i < tasks[section].size(); i++) {
            float x = startX + section * (sectionWidth + spacing) + (sectionWidth - taskWidth) / 2;
            float y = startY + i * 90 - scrollOffsets[section];
            tasks[section][i].setPosition(x, y);
        }
    }
}

void ScrumBoard::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    // Прокрутка колесиком мыши
    if (event.type == sf::Event::MouseWheelScrolled) {
        float mouseX = event.mouseWheelScroll.x;
        
        for (int i = 0; i < 4; i++) {
            if (sections[i].getGlobalBounds().contains(mouseX, event.mouseWheelScroll.y)) {
                float scrollDelta = -event.mouseWheelScroll.delta * 30.0f;
                float maxScroll = std::max(0.0f, (tasks[i].size() * 90.0f) - 700.0f);
                
                scrollOffsets[i] += scrollDelta;
                scrollOffsets[i] = std::max(0.0f, std::min(scrollOffsets[i], maxScroll));
                updateTaskPositions();
                break;
            }
        }
    }
    
    // Нажатие кнопки мыши
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            
            // Проверяем клик по задачам
            for (int i = 0; i < 4; i++) {
                for (auto& task : tasks[i]) {
                    if (task.shape.getGlobalBounds().contains(mousePos)) {
                        task.isMoving = true;
                        draggingTaskSection = i;
                        return;
                    }
                }
            }
            
            // Проверяем прокрутку
            for (int i = 0; i < 4; i++) {
                if (sections[i].getGlobalBounds().contains(mousePos)) {
                    isDraggingScroll[i] = true;
                    dragStartPositions[i] = mousePos;
                    break;
                }
            }
        }
    }
    
    // Отпускание кнопки мыши
    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            // Обрабатываем перетаскивание задач
            for (int i = 0; i < 4; i++) {
                for (auto& task : tasks[i]) {
                    if (task.isMoving) {
                        task.isMoving = false;
                        
                        // Находим новую секцию для задачи
                        sf::FloatRect taskBounds = task.shape.getGlobalBounds();
                        sf::Vector2f taskCenter(
                            taskBounds.left + taskBounds.width / 2,
                            taskBounds.top + taskBounds.height / 2
                        );
                        
                        for (int newSection = 0; newSection < 4; newSection++) {
                            if (sections[newSection].getGlobalBounds().contains(taskCenter)) {
                                if (newSection != i) {
                                    // Перемещаем задачу в новую секцию
                                    Task movedTask = task;
                                    movedTask.currentSection = newSection;
                                    movedTask.isMoving = false;
                                    tasks[newSection].push_back(movedTask);
                                    
                                    // Удаляем из старой секции
                                    for (auto it = tasks[i].begin(); it != tasks[i].end(); ++it) {
                                        if (it->isMoving) {
                                            tasks[i].erase(it);
                                            break;
                                        }
                                    }
                                    
                                    // Сбрасываем прокрутку
                                    scrollOffsets[i] = 0.0f;
                                    scrollOffsets[newSection] = 0.0f;
                                    
                                    updateTaskPositions();
                                    return;
                                } else {
                                    // Если вернули в ту же секцию
                                    updateTaskPositions();
                                }
                            }
                        }
                    }
                }
                isDraggingScroll[i] = false;
            }
        }
    }
    
    // Движение мыши
    if (event.type == sf::Event::MouseMoved) {
        // Перетаскивание задач
        for (int i = 0; i < 4; i++) {
            for (auto& task : tasks[i]) {
                if (task.isMoving) {
                    task.setPosition(event.mouseMove.x - 190, event.mouseMove.y - 40);
                }
            }
        }
        
        // Перетаскивание для прокрутки
        for (int i = 0; i < 4; i++) {
            if (isDraggingScroll[i]) {
                sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
                float deltaY = dragStartPositions[i].y - mousePos.y;
                float maxScroll = std::max(0.0f, (tasks[i].size() * 90.0f) - 700.0f);
                
                scrollOffsets[i] += deltaY;
                scrollOffsets[i] = std::max(0.0f, std::min(scrollOffsets[i], maxScroll));
                dragStartPositions[i] = mousePos;
                updateTaskPositions();
            }
        }
    }
}

void ScrumBoard::update(float deltaTime) {
    // Ничего не обновляем
}

void ScrumBoard::draw(sf::RenderWindow& window) {
    window.draw(titleText);
    
    // Рисуем секции
    for (const auto& section : sections) {
        window.draw(section);
    }
    
    // Рисуем заголовки секций
    for (const auto& text : sectionTexts) {
        window.draw(text);
    }
    
    // Рисуем задачи с учетом видимой области
    for (int i = 0; i < 4; i++) {
        sf::FloatRect sectionBounds = sections[i].getGlobalBounds();
        
        for (const auto& task : tasks[i]) {
            sf::FloatRect taskBounds = task.shape.getGlobalBounds();
            
            if (taskBounds.top + taskBounds.height >= sectionBounds.top && 
                taskBounds.top <= sectionBounds.top + sectionBounds.height) {
                window.draw(task.shape);
                window.draw(task.text);
            }
        }
    }
}
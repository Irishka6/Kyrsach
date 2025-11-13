#include "ScrumBoard.h"
#include <iostream>

const float WINDOW_WIDTH = 1920.0f;
const float WINDOW_HEIGHT = 1080.0f;
const float TASK_WIDTH = 380.0f;
const float TASK_HEIGHT = 80.0f;

ScrumBoard::ScrumBoard() {
    sectionNames = {"Назначено", "В процессе", "Блокировано", "Готово"};
    tasks.resize(4);
    scrollOffsets.resize(4, 0.0f);
    isDraggingScroll.resize(4, false);
    dragStartPositions.resize(4, sf::Vector2f(0, 0));
    draggingTaskSection = -1;
    draggingTaskIndex = -1;
    showCompanyWindow = false;
    
    // Компании как в изображении
    companies = {"Компания A", "Компания B", "Компания T", "Компания D"};
}

bool ScrumBoard::initialize() {
    if (!font.loadFromFile("ofont.ru_Pastry Chef.ttf")) {
        std::cout << "Не удалось загрузить шрифт!" << std::endl;
        if (!font.loadFromFile("arial.ttf")) {
            return false;
        }
    }
    
    createTitle();
    createTopPanel();
    createSections();
    createSampleTasks();
    createCompanyWindow();
    
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

void ScrumBoard::createTopPanel() {
    // Верхняя панель - ДЛИННЫЙ ПРЯМОУГОЛЬНИК цветом как у 4 блока (Готово)
    topPanel.setSize(sf::Vector2f(1820, 60));
    topPanel.setFillColor(sf::Color(120, 165, 205)); // ЦВЕТ КАК У 4 БЛОКА (Готово)
    topPanel.setOutlineThickness(0);
    topPanel.setPosition(50, 80);

    // Кнопка "Компании" - СВЕТЛЫЙ цвет
    companyButton.setSize(sf::Vector2f(200, 40));
    companyButton.setFillColor(sf::Color(180, 210, 235));
    companyButton.setOutlineThickness(0);
    companyButton.setPosition(100, 90);

    companyButtonText.setString("Компании");
    companyButtonText.setFont(font);
    companyButtonText.setCharacterSize(24);
    companyButtonText.setFillColor(sf::Color(50, 50, 80));
    companyButtonText.setStyle(sf::Text::Bold);
    
    sf::FloatRect textBounds = companyButtonText.getLocalBounds();
    companyButtonText.setPosition(
        100 + (200 - textBounds.width) / 2,
        90 + (40 - textBounds.height) / 2 - 5
    );
}

void ScrumBoard::createCompanyWindow() {
    // Окно компаний - над блоками
    companyWindow.setSize(sf::Vector2f(1820, 120)); // Увеличиваем высоту для разделительной линии
    companyWindow.setFillColor(sf::Color(180, 210, 235)); // ЦВЕТ КАК У 1 БЛОКА (Назначено)
    companyWindow.setOutlineColor(sf::Color(160, 190, 220));
    companyWindow.setOutlineThickness(2);
    companyWindow.setPosition(50, 100); // НАД БЛОКАМИ
    
    // Разделительная линия цвета как у текста
    dividerLine.setSize(sf::Vector2f(1800, 2));
    dividerLine.setFillColor(sf::Color(50, 50, 80)); // ЦВЕТ КАК У ТЕКСТА
    dividerLine.setPosition(60, 160);
    
    // Создаем 4 компании в ряд - выровнены по краям длинной линии
    float startX = 60; // НАЧАЛО длинной линии
    float companyWidth = 430.0f;
    
    for (size_t i = 0; i < companies.size(); i++) {
        sf::RectangleShape companyRect;
        companyRect.setSize(sf::Vector2f(430, 50));
        companyRect.setFillColor(sf::Color(180, 210, 235)); // ЦВЕТ КАК У 1 БЛОКА
        companyRect.setOutlineColor(sf::Color(160, 190, 220));
        companyRect.setOutlineThickness(1);
        companyRect.setPosition(startX + i * companyWidth, 110); // ВЫРОВНЕНЫ ПО НАЧАЛУ ЛИНИИ
        companyRects.push_back(companyRect);
        
        sf::Text companyText;
        companyText.setString(companies[i]);
        companyText.setFont(font);
        companyText.setCharacterSize(20);
        companyText.setFillColor(sf::Color(50, 50, 80));
        companyText.setStyle(sf::Text::Bold);
        
        // Центрируем текст в прямоугольнике компании
        sf::FloatRect textBounds = companyText.getLocalBounds();
        companyText.setPosition(
            startX + i * companyWidth + (430 - textBounds.width) / 2,
            110 + (50 - textBounds.height) / 2 - 5
        );
        companyTexts.push_back(companyText);
    }
}

void ScrumBoard::createSections() {
    sf::Color sectionColors[] = {
        sf::Color(180, 210, 235),  // Назначено
        sf::Color(160, 195, 225),  // В процессе  
        sf::Color(140, 180, 215),  // Блокировано
        sf::Color(120, 165, 205)   // Готово
    };
    
    // Выравниваем блоки по началу и концу длинной линии (50 - 1820+50=1870)
    float sectionWidth = 430.0f; // Немного шире чтобы выровнять
    float spacing = 40.0f;
    float startX = 60.0f; // НАЧАЛО длинной линии
    
    for (int i = 0; i < 4; i++) {
        sf::RectangleShape section;
        section.setSize(sf::Vector2f(sectionWidth, 700));
        section.setFillColor(sectionColors[i]);
        section.setOutlineThickness(0);
        section.setPosition(startX + i * (sectionWidth + spacing), 240); // Опускаем ниже окна компаний
        sections.push_back(section);
        
        sf::Text text;
        text.setString(sectionNames[i]);
        text.setFont(font);
        text.setCharacterSize(32);
        text.setFillColor(sf::Color(50, 50, 80));
        text.setStyle(sf::Text::Bold);
        
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition(startX + i * (sectionWidth + spacing) + (sectionWidth - textBounds.width) / 2, 200);
        sectionTexts.push_back(text);
    }
}

void ScrumBoard::createSampleTasks() {
    for (int i = 0; i < 4; i++) {
        tasks[i].clear();
    }
    
    // Назначено
    addTask("Студент аудий интерфейса", 0);
    addTask("Написать скорбые код", 0);
    addTask("Протестировать функцию", 0);
    addTask("Особый описание", 0);
    addTask("Оптимизировать преобразователь", 0);
    
    // В процессе
    addTask("Реализуйте переговорные", 1);
    addTask("Настроить адель", 1); // Исправлено на "адель"
    addTask("сладить новые функции", 1); // Исправлено на "сладить"
    
    // Блокировано
    addTask("Контройть критические доли", 2);
    addTask("Обработ. аргументацию", 2); // Исправлено на "Обработ."
    
    // Готово
    addTask("Проект", 3);
    addTask("Настроить сборку", 3);
    addTask("Протестировать по разряду устройства", 3);
    
    updateTaskPositions();
}

void ScrumBoard::addTask(const std::string& taskName, int section) {
    if (section >= 0 && section < 4) {
        float sectionWidth = 430.0f;
        float spacing = 40.0f;
        float startX = 60.0f;
        float taskWidth = 380.0f;
        
        float x = startX + section * (sectionWidth + spacing) + (sectionWidth - taskWidth) / 2;
        float y = 270 + tasks[section].size() * 90 - scrollOffsets[section];
        
        Task newTask(taskName, font, x, y);
        newTask.currentSection = section;
        tasks[section].push_back(newTask);
    }
}

void ScrumBoard::updateTaskPositions() {
    float sectionWidth = 430.0f;
    float spacing = 40.0f;
    float startX = 60.0f;
    float taskWidth = 380.0f;
    
    for (int section = 0; section < 4; section++) {
        float startY = 270.0f;
        for (size_t i = 0; i < tasks[section].size(); i++) {
            float x = startX + section * (sectionWidth + spacing) + (sectionWidth - taskWidth) / 2;
            float y = startY + i * 90 - scrollOffsets[section];
            tasks[section][i].setPosition(x, y);
        }
    }
}

void ScrumBoard::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            
            if (companyButton.getGlobalBounds().contains(mousePos)) {
                showCompanyWindow = !showCompanyWindow;
                return;
            }
            
            if (showCompanyWindow) {
                for (size_t i = 0; i < companyRects.size(); i++) {
                    if (companyRects[i].getGlobalBounds().contains(mousePos)) {
                        std::cout << "Выбрана компания: " << companies[i] << std::endl;
                        showCompanyWindow = false;
                        return;
                    }
                }
                if (!companyWindow.getGlobalBounds().contains(mousePos)) {
                    showCompanyWindow = false;
                }
            }
        }
    }
    
    if (event.type == sf::Event::MouseWheelScrolled) {
        float mouseX = event.mouseWheelScroll.x;
        
        for (int i = 0; i < 4; i++) {
            if (sections[i].getGlobalBounds().contains(mouseX, event.mouseWheelScroll.y)) {
                float scrollDelta = -event.mouseWheelScroll.delta * 30.0f;
                float maxScroll = std::max(0.0f, (tasks[i].size() * 90.0f) - 650.0f);
                
                scrollOffsets[i] += scrollDelta;
                scrollOffsets[i] = std::max(0.0f, std::min(scrollOffsets[i], maxScroll));
                updateTaskPositions();
                break;
            }
        }
    }
    
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            
            for (int i = 0; i < 4; i++) {
                for (size_t j = 0; j < tasks[i].size(); j++) {
                    if (tasks[i][j].shape.getGlobalBounds().contains(mousePos)) {
                        tasks[i][j].isMoving = true;
                        draggingTaskSection = i;
                        draggingTaskIndex = j;
                        return;
                    }
                }
            }
            
            for (int i = 0; i < 4; i++) {
                if (sections[i].getGlobalBounds().contains(mousePos)) {
                    isDraggingScroll[i] = true;
                    dragStartPositions[i] = mousePos;
                    break;
                }
            }
        }
    }
    
    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            if (draggingTaskSection != -1 && draggingTaskIndex != -1) {
                Task& draggedTask = tasks[draggingTaskSection][draggingTaskIndex];
                draggedTask.isMoving = false;
                
                sf::FloatRect taskBounds = draggedTask.shape.getGlobalBounds();
                sf::Vector2f taskCenter(
                    taskBounds.left + taskBounds.width / 2,
                    taskBounds.top + taskBounds.height / 2
                );
                
                for (int newSection = 0; newSection < 4; newSection++) {
                    if (sections[newSection].getGlobalBounds().contains(taskCenter)) {
                        if (newSection != draggingTaskSection) {
                            Task movedTask = draggedTask;
                            movedTask.currentSection = newSection;
                            movedTask.isMoving = false;
                            tasks[newSection].push_back(movedTask);
                            
                            tasks[draggingTaskSection].erase(tasks[draggingTaskSection].begin() + draggingTaskIndex);
                            
                            scrollOffsets[draggingTaskSection] = 0.0f;
                            scrollOffsets[newSection] = 0.0f;
                            
                            updateTaskPositions();
                            break;
                        } else {
                            updateTaskPositions();
                        }
                    }
                }
                
                draggingTaskSection = -1;
                draggingTaskIndex = -1;
            }
            
            for (int i = 0; i < 4; i++) {
                isDraggingScroll[i] = false;
            }
        }
    }
    
    if (event.type == sf::Event::MouseMoved) {
        if (draggingTaskSection != -1 && draggingTaskIndex != -1) {
            Task& draggedTask = tasks[draggingTaskSection][draggingTaskIndex];
            if (draggedTask.isMoving) {
                draggedTask.setPosition(event.mouseMove.x - 190, event.mouseMove.y - 40);
            }
        }
        
        for (int i = 0; i < 4; i++) {
            if (isDraggingScroll[i]) {
                sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
                float deltaY = dragStartPositions[i].y - mousePos.y;
                float maxScroll = std::max(0.0f, (tasks[i].size() * 90.0f) - 650.0f);
                
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
    
    window.draw(topPanel);
    window.draw(companyButton);
    window.draw(companyButtonText);
    
    // Рисуем секции
    for (const auto& section : sections) {
        window.draw(section);
    }
    
    // Рисуем заголовки секций
    for (const auto& text : sectionTexts) {
        window.draw(text);
    }
    
    // Рисуем окно компаний если оно открыто
    if (showCompanyWindow) {
        window.draw(companyWindow);
        window.draw(dividerLine);
        
        for (const auto& rect : companyRects) {
            window.draw(rect);
        }
        for (const auto& text : companyTexts) {
            window.draw(text);
        }
    }
    
    // Рисуем задачи
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
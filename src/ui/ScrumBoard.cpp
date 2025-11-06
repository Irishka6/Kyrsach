#include "ScrumBoard.h"
#include <iostream>

ScrumBoard::ScrumBoard() 
    : todoScrollOffset(0.0f), inProgressScrollOffset(0.0f), doneScrollOffset(0.0f),
      maxScrollOffset(0.0f), isDragging(false), draggedColumn(-1) {
}

bool ScrumBoard::initialize() {
    // Загрузка шрифта
    if (!font.loadFromFile("Roboto-Regular.ttf")) {
        std::cerr << "Ошибка загрузки шрифта!" << std::endl;
        return false;
    }
    
    // Настройка колонок
    setupColumn(todoColumn, 100.0f, sf::Color(173, 216, 230)); // Светло-голубой
    setupColumn(inProgressColumn, 700.0f, sf::Color(255, 255, 224)); // Светло-желтый
    setupColumn(doneColumn, 1300.0f, sf::Color(144, 238, 144)); // Светло-зеленый
    
    // Настройка текста
    setupText(todoText, "TO DO", 100.0f);
    setupText(inProgressText, "IN PROGRESS", 700.0f);
    setupText(doneText, "DONE", 1300.0f);
    
    // Создание тестовых задач
    tasks.clear();
    
    // Задачи для TO DO
    for (int i = 0; i < 8; ++i) {
        Task task;
        task.initialize("Задача " + std::to_string(i+1), "Описание задачи " + std::to_string(i+1), font);
        task.setStatus(Task::TO_DO);
        tasks.push_back(task);
    }
    
    // Задачи для IN PROGRESS
    for (int i = 0; i < 6; ++i) {
        Task task;
        task.initialize("В работе " + std::to_string(i+1), "Описание в работе " + std::to_string(i+1), font);
        task.setStatus(Task::IN_PROGRESS);
        tasks.push_back(task);
    }
    
    // Задачи для DONE
    for (int i = 0; i < 10; ++i) {
        Task task;
        task.initialize("Выполнено " + std::to_string(i+1), "Описание выполненной " + std::to_string(i+1), font);
        task.setStatus(Task::DONE);
        tasks.push_back(task);
    }
    
    updateTaskPositions();
    
    return true;
}

void ScrumBoard::setupColumn(sf::RectangleShape& column, float x, sf::Color color) {
    column.setSize(sf::Vector2f(500.0f, 800.0f));
    column.setPosition(x, 150.0f);
    column.setFillColor(color);
    column.setOutlineThickness(2.0f);
    column.setOutlineColor(sf::Color(200, 200, 200));
}

void ScrumBoard::setupText(sf::Text& text, const std::string& content, float x) {
    text.setFont(font);
    text.setString(content);
    text.setCharacterSize(36);
    text.setFillColor(sf::Color::Black);
    text.setStyle(sf::Text::Bold);
    
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition(x + 250.0f - textBounds.width / 2.0f, 80.0f);
}

void ScrumBoard::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    // Обработка прокрутки колесиком мыши
    if (event.type == sf::Event::MouseWheelScrolled) {
        float mouseX = event.mouseWheelScroll.x;
        float scrollDelta = -event.mouseWheelScroll.delta * 20.0f; // Чувствительность прокрутки
        
        // Определяем, над какой колонкой находится курсор
        if (todoColumn.getGlobalBounds().contains(mouseX, event.mouseWheelScroll.y)) {
            todoScrollOffset += scrollDelta;
            todoScrollOffset = std::max(0.0f, std::min(todoScrollOffset, calculateMaxScrollOffset(tasks)));
        }
        else if (inProgressColumn.getGlobalBounds().contains(mouseX, event.mouseWheelScroll.y)) {
            inProgressScrollOffset += scrollDelta;
            inProgressScrollOffset = std::max(0.0f, std::min(inProgressScrollOffset, calculateMaxScrollOffset(tasks)));
        }
        else if (doneColumn.getGlobalBounds().contains(mouseX, event.mouseWheelScroll.y)) {
            doneScrollOffset += scrollDelta;
            doneScrollOffset = std::max(0.0f, std::min(doneScrollOffset, calculateMaxScrollOffset(tasks)));
        }
        
        updateTaskPositions();
    }
    
    // Обработка перетаскивания для прокрутки
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            
            // Проверяем, находится ли курсор в пределах какой-либо колонки
            if (todoColumn.getGlobalBounds().contains(mousePos)) {
                isDragging = true;
                draggedColumn = 0;
                dragStartPosition = mousePos;
                dragStartScrollOffset = todoScrollOffset;
            }
            else if (inProgressColumn.getGlobalBounds().contains(mousePos)) {
                isDragging = true;
                draggedColumn = 1;
                dragStartPosition = mousePos;
                dragStartScrollOffset = inProgressScrollOffset;
            }
            else if (doneColumn.getGlobalBounds().contains(mousePos)) {
                isDragging = true;
                draggedColumn = 2;
                dragStartPosition = mousePos;
                dragStartScrollOffset = doneScrollOffset;
            }
        }
    }
    
    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            isDragging = false;
            draggedColumn = -1;
        }
    }
    
    if (event.type == sf::Event::MouseMoved) {
        if (isDragging) {
            sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
            float deltaY = mousePos.y - dragStartPosition.y;
            
            switch (draggedColumn) {
                case 0: // TO DO
                    todoScrollOffset = dragStartScrollOffset - deltaY;
                    todoScrollOffset = std::max(0.0f, std::min(todoScrollOffset, calculateMaxScrollOffset(tasks)));
                    break;
                case 1: // IN PROGRESS
                    inProgressScrollOffset = dragStartScrollOffset - deltaY;
                    inProgressScrollOffset = std::max(0.0f, std::min(inProgressScrollOffset, calculateMaxScrollOffset(tasks)));
                    break;
                case 2: // DONE
                    doneScrollOffset = dragStartScrollOffset - deltaY;
                    doneScrollOffset = std::max(0.0f, std::min(doneScrollOffset, calculateMaxScrollOffset(tasks)));
                    break;
            }
            
            updateTaskPositions();
        }
    }
    
    // Передача событий задачам (для перетаскивания между колонками)
    for (auto& task : tasks) {
        task.handleEvent(event, window);
    }
}

void ScrumBoard::update(float deltaTime) {
    for (auto& task : tasks) {
        task.update(deltaTime);
    }
}

void ScrumBoard::draw(sf::RenderWindow& window) {
    // Рисуем колонки
    window.draw(todoColumn);
    window.draw(inProgressColumn);
    window.draw(doneColumn);
    
    // Рисуем заголовки
    window.draw(todoText);
    window.draw(inProgressText);
    window.draw(doneText);
    
    // Создаем область отсечения для каждой колонки
    sf::FloatRect todoBounds = todoColumn.getGlobalBounds();
    sf::FloatRect inProgressBounds = inProgressColumn.getGlobalBounds();
    sf::FloatRect doneBounds = doneColumn.getGlobalBounds();
    
    // Рисуем задачи для TO DO с областью отсечения
    sf::View originalView = window.getView();
    
    sf::View todoView(sf::FloatRect(todoBounds.left, todoBounds.top, todoBounds.width, todoBounds.height));
    todoView.setViewport(sf::FloatRect(
        todoBounds.left / 1920.0f, 
        todoBounds.top / 1080.0f,
        todoBounds.width / 1920.0f, 
        todoBounds.height / 1080.0f
    ));
    window.setView(todoView);
    
    for (const auto& task : tasks) {
        if (task.getStatus() == Task::TO_DO) {
            task.draw(window);
        }
    }
    
    // Рисуем задачи для IN PROGRESS с областью отсечения
    sf::View inProgressView(sf::FloatRect(inProgressBounds.left, inProgressBounds.top, inProgressBounds.width, inProgressBounds.height));
    inProgressView.setViewport(sf::FloatRect(
        inProgressBounds.left / 1920.0f, 
        inProgressBounds.top / 1080.0f,
        inProgressBounds.width / 1920.0f, 
        inProgressBounds.height / 1080.0f
    ));
    window.setView(inProgressView);
    
    for (const auto& task : tasks) {
        if (task.getStatus() == Task::IN_PROGRESS) {
            task.draw(window);
        }
    }
    
    // Рисуем задачи для DONE с областью отсечения
    sf::View doneView(sf::FloatRect(doneBounds.left, doneBounds.top, doneBounds.width, doneBounds.height));
    doneView.setViewport(sf::FloatRect(
        doneBounds.left / 1920.0f, 
        doneBounds.top / 1080.0f,
        doneBounds.width / 1920.0f, 
        doneBounds.height / 1080.0f
    ));
    window.setView(doneView);
    
    for (const auto& task : tasks) {
        if (task.getStatus() == Task::DONE) {
            task.draw(window);
        }
    }
    
    // Восстанавливаем оригинальный вид
    window.setView(originalView);
    
    // Рисуем стрелки поверх всего
    for (const auto& arrow : arrows) {
        arrow.draw(window);
    }
}

void ScrumBoard::updateTaskPositions() {
    float startY = 180.0f; // Начальная позиция Y для первой задачи
    float taskSpacing = 20.0f; // Расстояние между задачами
    float taskHeight = 120.0f; // Высота одной задачи
    
    // Обновляем позиции для TO DO
    float currentY = startY - todoScrollOffset;
    for (auto& task : tasks) {
        if (task.getStatus() == Task::TO_DO) {
            task.setPosition(sf::Vector2f(120.0f, currentY));
            currentY += taskHeight + taskSpacing;
        }
    }
    
    // Обновляем позиции для IN PROGRESS
    currentY = startY - inProgressScrollOffset;
    for (auto& task : tasks) {
        if (task.getStatus() == Task::IN_PROGRESS) {
            task.setPosition(sf::Vector2f(720.0f, currentY));
            currentY += taskHeight + taskSpacing;
        }
    }
    
    // Обновляем позиции для DONE
    currentY = startY - doneScrollOffset;
    for (auto& task : tasks) {
        if (task.getStatus() == Task::DONE) {
            task.setPosition(sf::Vector2f(1320.0f, currentY));
            currentY += taskHeight + taskSpacing;
        }
    }
}

float ScrumBoard::calculateMaxScrollOffset(const std::vector<Task>& tasksInColumn) {
    float startY = 180.0f;
    float taskSpacing = 20.0f;
    float taskHeight = 120.0f;
    float columnHeight = 800.0f;
    
    // Подсчитываем количество задач в каждой колонке
    int todoCount = 0, inProgressCount = 0, doneCount = 0;
    
    for (const auto& task : tasks) {
        switch (task.getStatus()) {
            case Task::TO_DO: todoCount++; break;
            case Task::IN_PROGRESS: inProgressCount++; break;
            case Task::DONE: doneCount++; break;
        }
    }
    
    // Вычисляем максимальную высоту контента для каждой колонки
    float todoContentHeight = todoCount * (taskHeight + taskSpacing) - taskSpacing;
    float inProgressContentHeight = inProgressCount * (taskHeight + taskSpacing) - taskSpacing;
    float doneContentHeight = doneCount * (taskHeight + taskSpacing) - taskSpacing;
    
    // Максимальное смещение - разница между высотой контента и высотой колонки
    float maxTodoScroll = std::max(0.0f, todoContentHeight - columnHeight + 50.0f);
    float maxInProgressScroll = std::max(0.0f, inProgressContentHeight - columnHeight + 50.0f);
    float maxDoneScroll = std::max(0.0f, doneContentHeight - columnHeight + 50.0f);
    
    // Возвращаем максимальное значение для безопасности
    return std::max(maxTodoScroll, std::max(maxInProgressScroll, maxDoneScroll));
}
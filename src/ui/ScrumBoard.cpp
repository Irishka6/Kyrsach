#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class Task {
public:
    sf::RectangleShape shape;
    sf::Text text;
    int currentSection;
    bool isMoving;
    std::string description;
    
    Task(const std::string& taskText, sf::Font& font, float x, float y) 
        : description(taskText), currentSection(0), isMoving(false) {
        
        shape.setSize(sf::Vector2f(380, 80));
        shape.setFillColor(sf::Color(255, 255, 255));
        shape.setOutlineColor(sf::Color(200, 210, 220));
        shape.setOutlineThickness(2);
        shape.setPosition(x, y);
        
        text.setString(taskText);
        text.setFont(font);
        text.setCharacterSize(22);
        text.setFillColor(sf::Color(40, 40, 100));
        
        updateTextPosition();
    }
    
    void updateTextPosition() {
        sf::FloatRect textBounds = text.getLocalBounds();
        sf::Vector2f shapePos = shape.getPosition();
        
        float textX = shapePos.x + (380 - textBounds.width) / 2;
        float textY = shapePos.y + (80 - textBounds.height) / 2;
        
        text.setPosition(textX, textY);
    }
    
    void setPosition(float x, float y) {
        shape.setPosition(x, y);
        updateTextPosition();
    }
};

class ScrumBoard {
private:
    const float WINDOW_WIDTH = 1920.0f;
    const float WINDOW_HEIGHT = 1080.0f;
    
    std::vector<std::string> sectionNames;
    std::vector<sf::RectangleShape> sections;
    std::vector<sf::Text> sectionTexts;
    std::vector<std::vector<Task>> tasks;
    sf::Font font;
    sf::Text titleText;
    
    std::vector<float> scrollOffsets;
    std::vector<bool> isDraggingScroll;
    std::vector<sf::Vector2f> dragStartPositions;
    int draggingTaskSection;
    int draggingTaskIndex;
    
    sf::RectangleShape topPanel;
    sf::RectangleShape companyButton;
    sf::Text companyButtonText;
    bool showCompanyWindow;
    sf::RectangleShape companyWindow;
    std::vector<sf::RectangleShape> dividerLines;
    std::vector<std::string> companies;
    std::vector<sf::RectangleShape> companyRects;
    std::vector<sf::Text> companyTexts;

public:
    ScrumBoard() {
        sectionNames = {"Назначено", "В процессе", "Блокировано", "Готово"};
        tasks.resize(4);
        scrollOffsets.resize(4, 0.0f);
        isDraggingScroll.resize(4, false);
        dragStartPositions.resize(4, sf::Vector2f(0, 0));
        draggingTaskSection = -1;
        draggingTaskIndex = -1;
        showCompanyWindow = false;
        
        companies = {"Компания A", "Компания Б", "Компания С", "Компания Д"};
    }

    bool initialize() {
        if (!font.loadFromFile("ofont.ru_Pastry Chef.ttf")) {
            std::cout << "Не удалось загрузить шрифт!" << std::endl;
            return false;
        }
        
        createTitle();
        createTopPanel();
        createSections();
        createSampleTasks();
        createCompanyWindow();
        
        return true;
    }

    void createTitle() {
        titleText.setString("Scrum Board - Управление задачами");
        titleText.setFont(font);
        titleText.setCharacterSize(36);
        titleText.setFillColor(sf::Color(50, 50, 100));
        titleText.setStyle(sf::Text::Bold);
        titleText.setPosition(600, 30);
    }

    void createTopPanel() {
        topPanel.setSize(sf::Vector2f(1820, 60));
        topPanel.setFillColor(sf::Color(120, 165, 205));
        topPanel.setOutlineThickness(0);
        topPanel.setPosition(50, 80);

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

    void createCompanyWindow() {
        // Компактное окно компаний
        float companyHeight = 55.0f;
        float spacing = 0.0f; 
        float padding = 15.0f;
        
        // Рассчитываем размер окна
        float windowWidth = 380.0f;
        float windowHeight = companyHeight * companies.size() + padding * 2;
        
        float startX = (WINDOW_WIDTH - windowWidth) / 2;
        float startY = (WINDOW_HEIGHT - windowHeight) / 2;
        
        companyWindow.setSize(sf::Vector2f(windowWidth, windowHeight));
        companyWindow.setFillColor(sf::Color(180, 210, 235));
        companyWindow.setOutlineColor(sf::Color(160, 190, 220));
        companyWindow.setOutlineThickness(3);
        companyWindow.setPosition(startX, startY);
        
        companyRects.clear();
        companyTexts.clear();
        dividerLines.clear();
        
        float companyWidth = windowWidth - padding * 2;
        
        for (size_t i = 0; i < companies.size(); i++) {
            // Прямоугольник компании
            sf::RectangleShape companyRect;
            companyRect.setSize(sf::Vector2f(companyWidth, companyHeight));
            companyRect.setFillColor(sf::Color(180, 210, 235));
            companyRect.setOutlineColor(sf::Color(160, 190, 220));
            companyRect.setOutlineThickness(1);
            companyRect.setPosition(startX + padding, startY + padding + i * companyHeight);
            companyRects.push_back(companyRect);
            
            // Текст компании
            sf::Text companyText;
            companyText.setString(companies[i]);
            companyText.setFont(font);
            companyText.setCharacterSize(24);
            companyText.setFillColor(sf::Color(50, 50, 80));
            companyText.setStyle(sf::Text::Bold);
            
            sf::FloatRect textBounds = companyText.getLocalBounds();
            companyText.setPosition(
                startX + padding + (companyWidth - textBounds.width) / 2,
                startY + padding + i * companyHeight + (companyHeight - textBounds.height) / 2 - 3
            );
            companyTexts.push_back(companyText);
            
            // Разделительная линия между элементами
            if (i < companies.size() - 1) {
                sf::RectangleShape dividerLine;
                dividerLine.setSize(sf::Vector2f(companyWidth, 4));
                dividerLine.setFillColor(sf::Color(100, 130, 160));
                dividerLine.setPosition(
                    startX + padding,
                    startY + padding + (i + 1) * companyHeight - 2
                );
                dividerLines.push_back(dividerLine);
            }
        }
    }

    void createSections() {
        sf::Color sectionColors[] = {
            sf::Color(180, 210, 235),
            sf::Color(160, 195, 225),
            sf::Color(140, 180, 215),
            sf::Color(120, 165, 205)
        };
        
        float totalWidth = 1820.0f;
        float sectionWidth = (totalWidth - 120.0f) / 4.0f;
        float spacing = 40.0f;
        float startX = 50.0f;
        
        for (int i = 0; i < 4; i++) {
            sf::RectangleShape section;
            section.setSize(sf::Vector2f(sectionWidth, 700));
            section.setFillColor(sectionColors[i]);
            section.setOutlineThickness(0);
            section.setPosition(startX + i * (sectionWidth + spacing), 240);
            sections.push_back(section);
            
            sf::Text text;
            text.setString(sectionNames[i]);
            text.setFont(font);
            text.setCharacterSize(32);
            text.setFillColor(sf::Color(50, 50, 80));
            text.setStyle(sf::Text::Bold);
            
            sf::FloatRect textBounds = text.getLocalBounds();
            text.setPosition(
                startX + i * (sectionWidth + spacing) + (sectionWidth - textBounds.width) / 2, 
                200
            );
            sectionTexts.push_back(text);
        }
    }

    void createSampleTasks() {
        for (int i = 0; i < 4; i++) {
            tasks[i].clear();
        }
        
        addTask("Получение презентаци", 0);
        addTask("Просмотр презентации", 0);
        addTask("Выбор темы", 0);
        addTask("Особые описания", 0);
        addTask("Оптимизирование", 0);
        
        addTask("Прослушивание лекции", 1);
        addTask("Начало написания кода", 1);
        addTask("Блок 1 текста готов", 1);
        
        addTask("Контроль кода", 2);
        addTask("Обработка", 2);
        
        addTask("Проект", 3);
        addTask("Инструменты сборки", 3);
        addTask("Перетестировать по порядку устройства", 3);
        
        updateTaskPositions();
    }

    void addTask(const std::string& taskName, int section) {
        if (section >= 0 && section < 4) {
            float totalWidth = 1820.0f;
            float sectionWidth = (totalWidth - 120.0f) / 4.0f;
            float spacing = 40.0f;
            float startX = 50.0f;
            float taskWidth = sectionWidth - 30.0f;
            
            float x = startX + section * (sectionWidth + spacing) + (sectionWidth - taskWidth) / 2;
            float y = 270 + tasks[section].size() * 90 - scrollOffsets[section];
            
            Task newTask(taskName, font, x, y);
            newTask.currentSection = section;
            newTask.shape.setSize(sf::Vector2f(taskWidth, 85));
            tasks[section].push_back(newTask);
        }
    }

    void updateTaskPositions() {
        float totalWidth = 1820.0f;
        float sectionWidth = (totalWidth - 120.0f) / 4.0f;
        float spacing = 40.0f;
        float startX = 50.0f;
        float taskWidth = sectionWidth - 30.0f;
        
        for (int section = 0; section < 4; section++) {
            float startY = 270.0f;
            for (size_t i = 0; i < tasks[section].size(); i++) {
                float x = startX + section * (sectionWidth + spacing) + (sectionWidth - taskWidth) / 2;
                float y = startY + i * 95 - scrollOffsets[section];
                tasks[section][i].setPosition(x, y);
                tasks[section][i].shape.setSize(sf::Vector2f(taskWidth, 85));
            }
        }
    }

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) {
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
                    // Закрываем окно при клике вне его области
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

    void update(float deltaTime) {}

    void draw(sf::RenderWindow& window) {
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
        
        // Рисуем окно компаний
        if (showCompanyWindow) {
            // Затемняем фон под окном
            sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
            overlay.setFillColor(sf::Color(0, 0, 0, 150));
            window.draw(overlay);
            
            window.draw(companyWindow);
            
            // Рисуем разделительные линии
            for (const auto& line : dividerLines) {
                window.draw(line);
            }
            
            // Рисуем прямоугольники компаний
            for (const auto& rect : companyRects) {
                window.draw(rect);
            }
            
            // Рисуем текст компаний
            for (const auto& text : companyTexts) {
                window.draw(text);
            }
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Scrum Board - Управление задачами", sf::Style::Default);
    
    ScrumBoard scrumBoard;
    if (!scrumBoard.initialize()) {
        return -1;
    }
    
    sf::RectangleShape background(sf::Vector2f(1920, 1080));
    background.setFillColor(sf::Color(245, 242, 240));
    
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
        
        window.draw(background);
        scrumBoard.draw(window);
        window.display();
    }
    
    return 0;
}
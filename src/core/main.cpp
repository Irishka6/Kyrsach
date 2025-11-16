int main(){
    setlocale(LC_ALL, "Russian");
        std::vector<Tasks> tasks = getTasksFromJson("tasks.json");
    
    // Если файл пустой, создаем начальные задачи
    if (tasks.empty()) {
        std::cout << "Файл пустой, создаем начальные задачи..." << std::endl;
        std::vector<std::vector<std::string>> tasks_data = {
            {"1", "Implement login feature"}, 
            {"0", "bay kola"}
        };
        
        for (int i = 0; i < 2; i++){
            Task task(i+1, tasks_data[i][1], std::stoi(tasks_data[i][0]));
            tasks.push_back(task);
        }
    }
    std::cout << "\n=== ТЕКУЩИЙ СПИСОК ЗАДАЧ ===" << std::endl;
    for (const auto& task : tasks) {
        std::cout << "ID: " << task.getId() 
                  << " | Название: " << task.getTitle() 
                  << " | Статус: " << task.getStatus() 
                  << " | Создана: " << task.getFormattedCreationDate() << std::endl;
    }
    
    // 3. ВНОСИМ ИЗМЕНЕНИЯ
    std::cout << "\n=== ВНЕСЕНИЕ ИЗМЕНЕНИЙ ===" << std::endl;
    if (!tasks.empty()) {
        // Меняем статус первой задачи
        tasks[0].changeStatus(2); // Например, статус 2 = "В работе"
        std::cout << "Изменен статус задачи: " << tasks[0].getTitle() << std::endl;
        
        // Меняем название второй задачи
        tasks[1].setTitle("Bay cuctus");
        std::cout << "Изменено название задачи: " << tasks[1].getTitle() << std::endl;
    }
    
    // 4. СОХРАНЯЕМ ИЗМЕНЕНИЯ ОБРАТНО В JSON ФАЙЛ
    std::cout << "\n=== СОХРАНЕНИЕ ИЗМЕНЕНИЙ ===" << std::endl;
    saveTasksToJson(tasks);
    
    // 5. ПРОВЕРЯЕМ, ЧТО ИЗМЕНЕНИЯ СОХРАНИЛИСЬ
    std::cout << "\n=== ПРОВЕРКА СОХРАНЕННЫХ ДАННЫХ ===" << std::endl;
    std::vector<Task> updatedTasks = getTasksFromJson();
    for (const auto& task : updatedTasks) {
        std::cout << "ID: " << task.getId() 
                  << " | Название: " << task.getTitle() 
                  << " | Статус: " << task.getStatus() << std::endl;
    }
    
    return 0;
}
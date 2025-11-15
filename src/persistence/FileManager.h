#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "../core/Board.h"
#include <string>
#include <fstream>
#include <memory>

// Если используется nlohmann/json, раскомментируйте следующую строку:
// #include <nlohmann/json.hpp>

class FileManager {
public:
    // Сохранение и загрузка в бинарном формате
    bool saveBoard(const Board& board, const std::string& filename);
    bool loadBoard(Board& board, const std::string& filename);
    
    // Экспорт и импорт в JSON формате
    bool exportToJSON(const Board& board, const std::string& filename);
    bool importFromJSON(Board& board, const std::string& filename);
    
    // Вспомогательные методы для проверки файлов
    bool fileExists(const std::string& filename) const;
    bool isFileReadable(const std::string& filename) const;
    bool isFileWritable(const std::string& filename) const;

private:
    // Вспомогательные методы для сериализации
    std::string serializeTask(const Task& task) const;
    std::string serializeDeveloper(const Developer& developer) const;
    
    Task deserializeTask(const std::string& data) const;
    Developer deserializeDeveloper(const std::string& data) const;
    
    // Временная реализация JSON сериализации (можно заменить nlohmann/json)
    std::string taskToJSON(const Task& task) const;
    std::string developerToJSON(const Developer& developer) const;
    
    // Обработка ошибок
    void logError(const std::string& error) const;
    void logInfo(const std::string& info) const;
};

#endif // FILEMANAGER_H
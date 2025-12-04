// core/tests.cpp - Упрощенные тесты без Google Test
#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <fstream>
#include "Tasks.h"
#include "Developer.h"
#include "Project.h"

using namespace std;

// Простая замена макросов Google Test
#define TEST_CASE(name) class Test_##name { public: void Run(); }; void Test_##name::Run()
#define EXPECT_EQ(a, b) assert((a) == (b))
#define EXPECT_TRUE(a) assert((a))
#define EXPECT_FALSE(a) assert(!(a))
#define EXPECT_NE(a, b) assert((a) != (b))
#define EXPECT_THROW(statement, exception) \
    try { statement; assert(false && "Expected exception"); } \
    catch(const exception&) {} \
    catch(...) { assert(false && "Wrong exception type"); }
#define EXPECT_NO_THROW(statement) \
    try { statement; } catch(...) { assert(false && "Unexpected exception"); }

// ===================== ТЕСТЫ ДЛЯ КЛАССА TASKS =====================
TEST_CASE(TasksTest_ConstructorAndGetters) {
    Tasks task(1, "Test Task", 0);
    
    EXPECT_EQ(task.getId(), 1);
    EXPECT_EQ(task.getTitle(), "Test Task");
    EXPECT_EQ(task.getStatus(), 0);
    EXPECT_EQ(task.getCreatorId(), 0);
}

TEST_CASE(TasksTest_SetTitle) {
    Tasks task(1, "Original", 0);
    task.setTitle("Modified");
    
    EXPECT_EQ(task.getTitle(), "Modified");
}

TEST_CASE(TasksTest_ChangeStatus) {
    Tasks task(1, "Task", 0);
    task.changeStatus(2);
    
    EXPECT_EQ(task.getStatus(), 2);
}

TEST_CASE(TasksTest_SetCreatorId) {
    Tasks task(1, "Task", 0);
    task.setCreatorId(5);
    
    EXPECT_EQ(task.getCreatorId(), 5);
}

TEST_CASE(TasksTest_SetProject) {
    Tasks task(1, "Task", 0);
    task.setProject("Test Project");
    
    EXPECT_EQ(task.getProjekt(), "Test Project");
}

// ===================== ТЕСТЫ ДЛЯ КЛАССА DEVELOPER =====================
TEST_CASE(DeveloperTest_ConstructorAndGetters) {
    Developer dev(1, "testuser", "password123");
    
    EXPECT_EQ(dev.getId(), 1);
    EXPECT_EQ(dev.getLogin(), "testuser");
    EXPECT_EQ(dev.getPassword(), "password123");
    EXPECT_TRUE(dev.getProjectIds().empty());
}

TEST_CASE(DeveloperTest_ValidateLogin) {
    Developer dev(1, "test", "pass");
    
    EXPECT_TRUE(dev.validateLogin("validuser"));
    EXPECT_FALSE(dev.validateLogin(""));
    EXPECT_FALSE(dev.validateLogin("ab"));
    EXPECT_TRUE(dev.validateLogin("abc"));
}

TEST_CASE(DeveloperTest_ValidatePassword) {
    Developer dev(1, "test", "pass");
    
    EXPECT_TRUE(dev.validatePassword("pass"));
    EXPECT_FALSE(dev.validatePassword(""));
    EXPECT_FALSE(dev.validatePassword("123"));
    EXPECT_TRUE(dev.validatePassword("1234"));
}

TEST_CASE(DeveloperTest_CheckCredentials) {
    Developer dev(1, "admin", "admin123");
    
    EXPECT_TRUE(dev.checkCredentials("admin", "admin123"));
    EXPECT_FALSE(dev.checkCredentials("admin", "wrong"));
    EXPECT_FALSE(dev.checkCredentials("wrong", "admin123"));
}

TEST_CASE(DeveloperTest_AddAndRemoveProject) {
    Developer dev(1, "user", "pass");
    
    dev.addProject(100);
    dev.addProject(200);
    dev.addProject(100); // Дубликат
    
    auto projects = dev.getProjectIds();
    EXPECT_EQ(projects.size(), 2);
    EXPECT_EQ(projects[0], 100);
    EXPECT_EQ(projects[1], 200);
    
    dev.removeProject(100);
    projects = dev.getProjectIds();
    EXPECT_EQ(projects.size(), 1);
    EXPECT_EQ(projects[0], 200);
}

// ===================== ТЕСТЫ ДЛЯ КЛАССА PROJECT =====================
TEST_CASE(ProjectTest_ConstructorAndGetters) {
    Project project(1, "Test Project", "2024-12-31", 100);
    
    EXPECT_EQ(project.getId(), 1);
    EXPECT_EQ(project.getName(), "Test Project");
    EXPECT_EQ(project.getDeadline(), "2024-12-31");
    EXPECT_EQ(project.getCreatorId(), 100);
    
    auto devs = project.getDeveloperIds();
    EXPECT_EQ(devs.size(), 1);
    EXPECT_EQ(devs[0], 100);
}

TEST_CASE(ProjectTest_AddTask) {
    Project project(1, "Project", "2024-12-31", 1);
    
    Tasks task1(1, "Task 1", 0);
    task1.setCreatorId(1);
    project.addTask(task1);
    
    Tasks task2(2, "Task 2", 1);
    task2.setCreatorId(1);
    project.addTask(task2);
    
    auto tasks = project.getTasks();
    EXPECT_EQ(tasks.size(), 2);
    EXPECT_EQ(tasks[0].getTitle(), "Task 1");
    EXPECT_EQ(tasks[1].getTitle(), "Task 2");
}

TEST_CASE(ProjectTest_UpdateTask) {
    Project project(1, "Project", "2024-12-31", 1);
    
    Tasks task(1, "Original", 0);
    task.setCreatorId(1);
    project.addTask(task);
    
    Tasks updatedTask(1, "Updated", 2);
    updatedTask.setCreatorId(1);
    project.addTask(updatedTask);
    
    auto tasks = project.getTasks();
    EXPECT_EQ(tasks.size(), 1);
    EXPECT_EQ(tasks[0].getTitle(), "Updated");
    EXPECT_EQ(tasks[0].getStatus(), 2);
}

TEST_CASE(ProjectTest_RemoveTask) {
    Project project(1, "Project", "2024-12-31", 1);
    
    Tasks task1(1, "Task 1", 0);
    task1.setCreatorId(1);
    Tasks task2(2, "Task 2", 0);
    task2.setCreatorId(1);
    
    project.addTask(task1);
    project.addTask(task2);
    
    EXPECT_EQ(project.getTasks().size(), 2);
    
    project.removeTask(1);
    
    auto tasks = project.getTasks();
    EXPECT_EQ(tasks.size(), 1);
    EXPECT_EQ(tasks[0].getId(), 2);
}

TEST_CASE(ProjectTest_AddAndRemoveDeveloper) {
    Project project(1, "Project", "2024-12-31", 100);
    
    project.addDeveloper(200);
    project.addDeveloper(300);
    project.addDeveloper(200);
    
    auto devs = project.getDeveloperIds();
    EXPECT_EQ(devs.size(), 3);
    EXPECT_TRUE(project.hasDeveloper(100));
    EXPECT_TRUE(project.hasDeveloper(200));
    EXPECT_TRUE(project.hasDeveloper(300));
    EXPECT_FALSE(project.hasDeveloper(400));
    
    EXPECT_TRUE(project.isCreator(100));
    EXPECT_FALSE(project.isCreator(200));
    
    project.removeDeveloper(200);
    devs = project.getDeveloperIds();
    EXPECT_EQ(devs.size(), 2);
    EXPECT_FALSE(project.hasDeveloper(200));
}

TEST_CASE(ProjectTest_FindTaskById) {
    Project project(1, "Project", "2024-12-31", 1);
    
    Tasks task1(1, "Task 1", 0);
    task1.setCreatorId(1);
    Tasks task2(2, "Task 2", 1);
    task2.setCreatorId(1);
    
    project.addTask(task1);
    project.addTask(task2);
    
    Tasks* found = project.findTaskById(2);
    EXPECT_NE(found, nullptr);
    EXPECT_EQ(found->getTitle(), "Task 2");
    
    Tasks* notFound = project.findTaskById(999);
    EXPECT_EQ(notFound, nullptr);
}

// ===================== ТЕСТЫ ИСКЛЮЧЕНИЙ =====================
TEST_CASE(ExceptionTest_DeveloperInvalidLogin) {
    EXPECT_THROW(Developer(1, "", "password"), std::invalid_argument);
}

TEST_CASE(ExceptionTest_DeveloperInvalidPassword) {
    EXPECT_THROW(Developer(1, "user", ""), std::invalid_argument);
}

TEST_CASE(ExceptionTest_ProjectInvalidName) {
    EXPECT_THROW(Project(1, "", "2024-12-31", 1), std::invalid_argument);
}

// ===================== ГЛАВНАЯ ФУНКЦИЯ =====================
void runAllTests() {
    cout << "Запуск тестов..." << endl;
    cout << "==================" << endl;
    
    Test_TasksTest_ConstructorAndGetters().Run();
    cout << "? TasksTest_ConstructorAndGetters passed" << endl;
    
    Test_TasksTest_SetTitle().Run();
    cout << "? TasksTest_SetTitle passed" << endl;
    
    Test_TasksTest_ChangeStatus().Run();
    cout << "? TasksTest_ChangeStatus passed" << endl;
    
    Test_TasksTest_SetCreatorId().Run();
    cout << "? TasksTest_SetCreatorId passed" << endl;
    
    Test_TasksTest_SetProject().Run();
    cout << "? TasksTest_SetProject passed" << endl;
    
    Test_DeveloperTest_ConstructorAndGetters().Run();
    cout << "? DeveloperTest_ConstructorAndGetters passed" << endl;
    
    Test_DeveloperTest_ValidateLogin().Run();
    cout << "? DeveloperTest_ValidateLogin passed" << endl;
    
    Test_DeveloperTest_ValidatePassword().Run();
    cout << "? DeveloperTest_ValidatePassword passed" << endl;
    
    Test_DeveloperTest_CheckCredentials().Run();
    cout << "? DeveloperTest_CheckCredentials passed" << endl;
    
    Test_DeveloperTest_AddAndRemoveProject().Run();
    cout << "? DeveloperTest_AddAndRemoveProject passed" << endl;
    
    Test_ProjectTest_ConstructorAndGetters().Run();
    cout << "? ProjectTest_ConstructorAndGetters passed" << endl;
    
    Test_ProjectTest_AddTask().Run();
    cout << "? ProjectTest_AddTask passed" << endl;
    
    Test_ProjectTest_UpdateTask().Run();
    cout << "? ProjectTest_UpdateTask passed" << endl;
    
    Test_ProjectTest_RemoveTask().Run();
    cout << "? ProjectTest_RemoveTask passed" << endl;
    
    Test_ProjectTest_AddAndRemoveDeveloper().Run();
    cout << "? ProjectTest_AddAndRemoveDeveloper passed" << endl;
    
    Test_ProjectTest_FindTaskById().Run();
    cout << "? ProjectTest_FindTaskById passed" << endl;
    
    Test_ExceptionTest_DeveloperInvalidLogin().Run();
    cout << "? ExceptionTest_DeveloperInvalidLogin passed" << endl;
    
    Test_ExceptionTest_DeveloperInvalidPassword().Run();
    cout << "? ExceptionTest_DeveloperInvalidPassword passed" << endl;
    
    Test_ExceptionTest_ProjectInvalidName().Run();
    cout << "? ExceptionTest_ProjectInvalidName passed" << endl;
    
    cout << "==================" << endl;
    cout << "Все тесты прошли успешно!" << endl;
}

int main() {
    setlocale(LC_ALL, "Russian");
    try {
        runAllTests();
        return 0;
    } catch (const std::exception& e) {
        cerr << "Ошибка в тестах: " << e.what() << endl;
        return 1;
    } catch (...) {
        cerr << "Неизвестная ошибка в тестах" << endl;
        return 1;
    }
}
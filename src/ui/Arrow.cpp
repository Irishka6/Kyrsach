#include "Arrow.h"

Arrow::Arrow(float x, float y, bool right) : pointsRight(right) {
    shape.setPointCount(7);
    
    updateShape();
    shape.setFillColor(sf::Color(70, 130, 180, 200));
    shape.setPosition(x, y);
    bounds = sf::FloatRect(x - 10, y - 20, 50, 40); // Увеличиваем область клика
}

void Arrow::updateShape() {
    if (pointsRight) {
        shape.setPoint(0, sf::Vector2f(0, 0));
        shape.setPoint(1, sf::Vector2f(30, 0));
        shape.setPoint(2, sf::Vector2f(30, 12));
        shape.setPoint(3, sf::Vector2f(45, 0));
        shape.setPoint(4, sf::Vector2f(30, -12));
        shape.setPoint(5, sf::Vector2f(30, 0));
        shape.setPoint(6, sf::Vector2f(0, 0));
    } else {
        shape.setPoint(0, sf::Vector2f(45, 0));
        shape.setPoint(1, sf::Vector2f(15, 0));
        shape.setPoint(2, sf::Vector2f(15, 12));
        shape.setPoint(3, sf::Vector2f(0, 0));
        shape.setPoint(4, sf::Vector2f(15, -12));
        shape.setPoint(5, sf::Vector2f(15, 0));
        shape.setPoint(6, sf::Vector2f(45, 0));
    }
}

void Arrow::toggle() {
    pointsRight = !pointsRight;
    updateShape();
}
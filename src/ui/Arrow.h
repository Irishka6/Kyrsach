#pragma once
#include <SFML/Graphics.hpp>

class Arrow {
public:
    sf::ConvexShape shape;
    bool pointsRight;
    sf::FloatRect bounds;
    
    Arrow(float x, float y, bool right);
    void toggle();
    void updateShape();
};
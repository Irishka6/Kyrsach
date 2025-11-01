#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

class WaveAnimation {
public:
    std::vector<sf::CircleShape> waves;
    bool isActive;
    float waveTime;
    
    WaveAnimation();
    void initialize(float centerX, float centerY);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void start();
    void stop();
};
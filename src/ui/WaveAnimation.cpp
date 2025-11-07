#include "WaveAnimation.h"

WaveAnimation::WaveAnimation() : isActive(false), waveTime(0) {
    // Создаем 6 волн разного размера и цвета
    sf::Color waveColors[] = {
        sf::Color(135, 206, 250, 150),
        sf::Color(173, 216, 230, 120),
        sf::Color(176, 224, 230, 100),
        sf::Color(175, 238, 238, 80),
        sf::Color(240, 255, 255, 60),
        sf::Color(224, 255, 255, 40)
    };
    
    for (int i = 0; i < 6; i++) {
        sf::CircleShape wave(30 + i * 12);
        wave.setFillColor(sf::Color::Transparent);
        wave.setOutlineColor(waveColors[i]);
        wave.setOutlineThickness(2);
        waves.push_back(wave);
    }
}

void WaveAnimation::initialize(float centerX, float centerY) {
    for (size_t i = 0; i < waves.size(); i++) {
        sf::FloatRect bounds = waves[i].getLocalBounds();
        waves[i].setPosition(centerX - bounds.width / 2, centerY - bounds.height / 2);
    }
}

void WaveAnimation::update(float deltaTime) {
    if (!isActive) return;
    
    waveTime += deltaTime;
    for (size_t i = 0; i < waves.size(); i++) {
        float scale = 1.0f + 0.15f * std::sin(waveTime * 2.0f + i * 0.8f);
        waves[i].setScale(scale, scale);
        
        sf::Color currentColor = waves[i].getOutlineColor();
        float alpha = 150 + 105 * std::sin(waveTime * 1.5f + i * 0.6f);
        currentColor.a = std::max(40, std::min(255, static_cast<int>(alpha)));
        waves[i].setOutlineColor(currentColor);
    }
    
    if (waveTime > 4.0f) {
        stop();
    }
}

void WaveAnimation::draw(sf::RenderWindow& window) {
    if (!isActive) return;
    
    for (const auto& wave : waves) {
        window.draw(wave);
    }
}

void WaveAnimation::start() {
    isActive = true;
    waveTime = 0;
    for (auto& wave : waves) {
        wave.setScale(1, 1);
        sf::Color color = wave.getOutlineColor();
        color.a = 150;
        wave.setOutlineColor(color);
    }
}

void WaveAnimation::stop() {
    isActive = false;
    waveTime = 0;
}
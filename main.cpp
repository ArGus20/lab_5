#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <cmath>
#include <algorithm>

const float DEG_TO_RAD = 3.1415926535f / 180.f;

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1024, 768 }), "SFML Space Ship Simulation");
    window.setFramerateLimit(60);

    sf::Font font;
    font.openFromFile("../resources/fonts/ArialMT.ttf");

    sf::Text infoText(font, "", 18);
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition({ 20.f, 20.f });

    sf::Vector2f position(512.f, 384.f);
    sf::Vector2f velocity(0.f, 0.f);
    float angle = 0.f;
    float enginePower = 200.f;
    const float rotationSpeed = 180.f;
    bool isEngineOn = false;

    sf::ConvexShape ship(3);
    ship.setPoint(0, { 0.f, -15.f });
    ship.setPoint(1, { -10.f, 15.f });
    ship.setPoint(2, { 10.f, 15.f });
    ship.setFillColor(sf::Color::Cyan);

    sf::ConvexShape flame(3);
    flame.setPoint(0, { -6.f, 15.f });
    flame.setPoint(1, { 6.f, 15.f });
    flame.setPoint(2, { 0.f, 25.f });
    flame.setFillColor(sf::Color::Red);

    std::vector<sf::Vector2f> trajectory;
    sf::VertexArray trajectoryLine(sf::PrimitiveType::LineStrip);

    sf::Clock clock;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        if (dt > 0.1f) dt = 0.1f;

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        isEngineOn = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            angle -= rotationSpeed * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            angle += rotationSpeed * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            isEngineOn = true;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
            enginePower += 1000.f * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) {
            enginePower -= 1000.f * dt;
        }
        enginePower = std::clamp(enginePower, 50.f, 1000.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
        {
            position = { 512.f, 384.f };
            velocity = { 0.f, 0.f };
            angle = 0.f;
            trajectory.clear();
        }

        float rad = angle * DEG_TO_RAD;
        sf::Vector2f acceleration(0.f, 0.f);

        if (isEngineOn)
        {
            acceleration.x = enginePower * std::sin(rad);
            acceleration.y = -enginePower * std::cos(rad);
        }

        velocity += acceleration * dt;
        position += velocity * dt;

        if (position.x < 0.f) position.x += 1024.f;
        if (position.x > 1024.f) position.x -= 1024.f;
        if (position.y < 0.f) position.y += 768.f;
        if (position.y > 768.f) position.y -= 768.f;

        trajectory.push_back(position);
        if (trajectory.size() > 600) {
            trajectory.erase(trajectory.begin());
        }

        ship.setPosition(position);
        ship.setRotation(sf::degrees(angle));

        flame.setPosition(position);
        flame.setRotation(sf::degrees(angle));

        trajectoryLine.clear();
        for (const auto& pt : trajectory) {
            trajectoryLine.append(sf::Vertex{ pt, sf::Color(100, 100, 100) });
        }

        float speedMagnitude = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

        infoText.setString(
            "Power: " + std::to_string((int)enginePower) + "\n" +
            "Speed: " + std::to_string((int)speedMagnitude)
        );

        window.clear(sf::Color::Black);
        window.draw(trajectoryLine);
        if (isEngineOn) {
            window.draw(flame);
        }
        window.draw(ship);
        window.draw(infoText);
        window.display();
    }

    return 0;
}
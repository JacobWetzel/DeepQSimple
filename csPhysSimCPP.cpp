#include <C://Users//16105//Desktop//SFMLProj//Graphics.hpp>

int main()
{
    // Set up the window
    const int screenWidth = 800;
    const int screenHeight = 600;
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Move the Square");

    // Define square properties
    sf::RectangleShape square(sf::Vector2f(50.f, 50.f));
    square.setFillColor(sf::Color::Red);
    square.setPosition((screenWidth - square.getSize().x) / 2, (screenHeight - square.getSize().y) / 2);

    // Movement speed (pixels per second)
    const float moveSpeed = 200.f;

    // Clock for delta time
    sf::Clock clock;

    // Main game loop
    while (window.isOpen())
    {
        // Calculate delta time
        sf::Time deltaTime = clock.restart();

        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Movement logic
        sf::Vector2f movement(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            movement.y -= moveSpeed * deltaTime.asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            movement.y += moveSpeed * deltaTime.asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            movement.x -= moveSpeed * deltaTime.asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            movement.x += moveSpeed * deltaTime.asSeconds();

        // Update square position
        square.move(movement);

        // Keep the square within the window bounds
        sf::Vector2f pos = square.getPosition();
        if (pos.x < 0)
            square.setPosition(0, pos.y);
        if (pos.x > screenWidth - square.getSize().x)
            square.setPosition(screenWidth - square.getSize().x, pos.y);
        if (pos.y < 0)
            square.setPosition(pos.x, 0);
        if (pos.y > screenHeight - square.getSize().y)
            square.setPosition(pos.x, screenHeight - square.getSize().y);

        // Clear the window
        window.clear(sf::Color::Black);

        // Draw everything
        window.draw(square);

        // Display the window
        window.display();
    }

    return 0;
}

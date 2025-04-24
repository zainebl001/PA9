#ifndef GAME_HPP
#define GAME_HPP

#include "GameBoard.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

class Game
{
private:
    sf::RenderWindow window;
    GameBoard board;
    sf::Clock clock;
    float dropTimer = 0.0f;
    sf::Font font;

    enum class State
    {
        Menu, Playing, GameOver
    } state = State::Menu;

    bool rotatePressed = false;
    bool hardDropPressed = false;

public:
    Game() : window(sf::VideoMode(720, 720), "TETRIS")
    {
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
        {
            std::cerr << "Failed to load font.\n";
        }
    }

    void run()
    {
        while (window.isOpen())
        {
            handleEvents();
            update();
            render();
        }
    }

private:
    void handleEvents()
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
                handleKeyPress(event.key.code);

            if (event.type == sf::Event::KeyReleased)
                handleKeyRelease(event.key.code);
        }
    }

    void handleKeyPress(sf::Keyboard::Key key)
    {
        if (state == State::Menu && key == sf::Keyboard::Enter)
        {
            restart();
            state = State::Playing;
        }

        if (state == State::GameOver && key == sf::Keyboard::R)
            restart();

        if (state == State::Playing)
        {
            switch (key)
            {
            case sf::Keyboard::Left:  board.moveLeft(); break;
            case sf::Keyboard::Right: board.moveRight(); break;
            case sf::Keyboard::Down:  board.moveDown(); dropTimer = 0.0f; break;
            case sf::Keyboard::Up:
                if (!rotatePressed)
                {
                    board.rotate();
                    rotatePressed = true;
                }
                break;
            case sf::Keyboard::Space:
                if (!hardDropPressed)
                {
                    board.hardDrop();
                    hardDropPressed = true;
                    dropTimer = 0.0f;
                }
                break;
            default:
                break;
            }
        }
    }

    void handleKeyRelease(sf::Keyboard::Key key)
    {
        if (key == sf::Keyboard::Up) rotatePressed = false;
        if (key == sf::Keyboard::Space) hardDropPressed = false;
    }

    void update()
    {
        if (state != State::Playing) return;

        float delta = clock.restart().asSeconds();
        dropTimer += delta;

        if (dropTimer >= board.getDropInterval())
        {
            board.moveDown();
            dropTimer = 0.0f;
        }

        if (board.isGameOver())
        {
            state = State::GameOver;
        }
    }

    void render()
    {
        window.clear(sf::Color(90, 90, 90));
        board.draw(window);
        drawSidebar();
        drawStateText();
        window.display();
    }

    void drawSidebar()
    {
        sf::Vector2f sidebarOrigin(GRID_OFFSET_X + BOARD_WIDTH * GRID_SIZE + BORDER_THICKNESS + 20, GRID_OFFSET_Y);

        auto drawPanel = [&](const std::string& label, const std::string& value, float yOffset)
            {
                sf::RectangleShape panel(sf::Vector2f(160, 60));
                panel.setPosition(sidebarOrigin.x, sidebarOrigin.y + yOffset);
                panel.setFillColor(sf::Color::Black);
                panel.setOutlineThickness(1);
                panel.setOutlineColor(sf::Color::White);
                window.draw(panel);

                drawText(label, panel.getPosition().x + 10, panel.getPosition().y + 5);
                drawText(value, panel.getPosition().x + 10, panel.getPosition().y + 30);
            };

        drawPanel("SCORE", std::to_string(board.getScore()), 0);
        drawPanel("LEVEL", std::to_string(board.getLevel()), 70);
        drawPanel("LINES", std::to_string(board.getLinesCleared()), 140);

        // Smaller next piece panel
        sf::RectangleShape nextPanel(sf::Vector2f(160, 100));
        nextPanel.setPosition(sidebarOrigin.x, sidebarOrigin.y + 210);
        nextPanel.setFillColor(sf::Color::Black);
        nextPanel.setOutlineThickness(1);
        nextPanel.setOutlineColor(sf::Color::White);
        window.draw(nextPanel);

        drawText("NEXT", nextPanel.getPosition().x + 10, nextPanel.getPosition().y + 5);
        float nextCenterX = nextPanel.getPosition().x + nextPanel.getSize().x / 2.f;
        float nextCenterY = nextPanel.getPosition().y + nextPanel.getSize().y / 2.f;

        board.drawNextPiece(window, nextCenterX, nextCenterY + 6.0f);
    }

    void drawStateText()
    {
        if (state == State::Menu)
        {
            drawText("TETRIS", GRID_OFFSET_X + 30, GRID_OFFSET_Y + 180, 32);
            drawText("Press ENTER to Start", GRID_OFFSET_X + 20, GRID_OFFSET_Y + 250, 20);
        }
        else if (state == State::GameOver)
        {
            drawText("GAME OVER", GRID_OFFSET_X + 30, GRID_OFFSET_Y + 180, 32, sf::Color::Red);
            drawText("Press R to Restart", GRID_OFFSET_X + 20, GRID_OFFSET_Y + 250, 20, sf::Color::Red);
        }
    }

    void drawText(const std::string& str, float x, float y, int size = 18, sf::Color color = sf::Color::White)
    {
        sf::Text text;
        text.setFont(font);
        text.setString(str);
        text.setCharacterSize(size);
        text.setFillColor(color);
        text.setPosition(x, y);
        window.draw(text);
    }

    void restart()
    {
        board.reset();
        dropTimer = 0.0f;
        state = State::Playing;
    }
};

#endif
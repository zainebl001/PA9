#ifndef GAME_BOARD_HPP
#define GAME_BOARD_HPP

#include "Shape.hpp"
#include "PieceFactory.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>

class GameBoard
{
private:
    std::vector<std::vector<sf::Color>> board;
    std::unique_ptr<Shape> currentPiece;
    std::unique_ptr<Shape> nextPiece;
    PieceFactory factory;
    int score = 0;
    int level = 1;
    int totalLinesCleared = 0;
    bool gameOver = false;

public:
    GameBoard() { reset(); }

    void reset()
    {
        board.assign(BOARD_HEIGHT, std::vector<sf::Color>(BOARD_WIDTH, sf::Color::Black));
        score = 0;
        level = 1;
        totalLinesCleared = 0;
        gameOver = false;

        currentPiece = factory.createRandomPiece();
        nextPiece = factory.createRandomPiece();
    }

    bool moveLeft() { return tryMove(&Shape::moveLeft, &Shape::moveRight); }
    bool moveRight() { return tryMove(&Shape::moveRight, &Shape::moveLeft); }

    bool rotate()
    {
        if (gameOver || !currentPiece) return false;
        currentPiece->rotate();
        if (!currentPiece->isValid(board))
        {
            for (int i = 0; i < 3; i++) currentPiece->rotate(); // undo
            return false;
        }
        return true;
    }

    bool moveDown()
    {
        if (gameOver || !currentPiece) return false;

        currentPiece->moveDown();

        if (!currentPiece->isValid(board))
        {
            currentPiece->moveUp();
            lockPiece();
            return false;
        }
        return true;
    }

    void hardDrop()
    {
        while (moveDown()) {}
    }

    void lockPiece()
    {
        for (const auto& pos : currentPiece->getPositions())
        {
            if (pos.y < 0 || pos.y >= BOARD_HEIGHT || pos.x < 0 || pos.x >= BOARD_WIDTH)
            {
                gameOver = true;
                return;
            }
            board[pos.y][pos.x] = currentPiece->getColor();
        }
        checkLines();
        currentPiece = std::move(nextPiece);
        nextPiece = factory.createRandomPiece();
        if (!currentPiece || !currentPiece->isValid(board))
        {
            gameOver = true;
        }
    }

    void checkLines()
    {
        int cleared = 0;

        for (int y = BOARD_HEIGHT - 1; y >= 0; y--)
        {
            if (std::all_of(board[y].begin(), board[y].end(), [](const sf::Color& c) { return c != sf::Color::Black; }))
            {
                cleared++;
                for (int row = y; row > 0; --row)
                    board[row] = board[row - 1];
                board[0].assign(BOARD_WIDTH, sf::Color::Black);
                ++y;
            }
        }

        if (cleared > 0)
        {
            static const int points[] = { 0, 40, 100, 300, 1200 };
            score += points[cleared] * level;
            totalLinesCleared += cleared;
            level = 1 + totalLinesCleared / 10;
        }
    }

    void draw(sf::RenderWindow& window) const
    {
        // Fill the inside of the playfield with black before drawing blocks
        sf::RectangleShape playAreaBG(sf::Vector2f(BOARD_WIDTH * GRID_SIZE, BOARD_HEIGHT * GRID_SIZE));
        playAreaBG.setPosition(GRID_OFFSET_X, GRID_OFFSET_Y);
        playAreaBG.setFillColor(sf::Color::Black);
        window.draw(playAreaBG);

        // Draw board blocks
        sf::RectangleShape block(sf::Vector2f(GRID_SIZE - 1, GRID_SIZE - 1));
        block.setOutlineThickness(1.f);
        block.setOutlineColor(sf::Color::Black);
        for (int y = 0; y < BOARD_HEIGHT; ++y)
        {
            for (int x = 0; x < BOARD_WIDTH; ++x)
            {
                if (board[y][x] != sf::Color::Black)
                {
                    float px = x * GRID_SIZE + GRID_OFFSET_X;
                    float py = y * GRID_SIZE + GRID_OFFSET_Y;

                    // Outer
                    sf::RectangleShape outer(sf::Vector2f(GRID_SIZE - 1, GRID_SIZE - 1));
                    outer.setPosition(px, py);
                    outer.setFillColor(sf::Color(204, 255, 153));
                    window.draw(outer);

                    // Inner
                    sf::RectangleShape inner(sf::Vector2f(GRID_SIZE / 2, GRID_SIZE / 2));
                    inner.setPosition(px + GRID_SIZE / 4, py + GRID_SIZE / 4);
                    inner.setFillColor(sf::Color::Black);
                    window.draw(inner);
                }
            }
        }


        // Draw white border
        sf::RectangleShape border(sf::Vector2f(BOARD_WIDTH * GRID_SIZE, BOARD_HEIGHT * GRID_SIZE));
        border.setPosition(GRID_OFFSET_X, GRID_OFFSET_Y);
        border.setOutlineColor(sf::Color::White);
        border.setOutlineThickness(BORDER_THICKNESS);
        border.setFillColor(sf::Color::Transparent);
        window.draw(border);

        if (currentPiece) currentPiece->draw(window);
    }

    void drawNextPiece(sf::RenderWindow& window, float centerX, float centerY) const
    {
        if (!nextPiece) return;

        const auto& layout = nextPiece->getLayout();

        float avgX = 0, avgY = 0;
        for (const auto& p : layout)
        {
            avgX += static_cast<float>(p.x);
            avgY += static_cast<float>(p.y);
        }
        avgX /= layout.size();
        avgY /= layout.size();

        // Convert block center to pixel offset
        float offsetX = centerX - (avgX * GRID_SIZE + GRID_SIZE / 2.f);
        float offsetY = centerY - (avgY * GRID_SIZE + GRID_SIZE / 2.f);

        sf::Color color = nextPiece->getColor();
        for (const auto& p : layout)
        {
            float x = offsetX + p.x * GRID_SIZE;
            float y = offsetY + p.y * GRID_SIZE;

            sf::RectangleShape outer(sf::Vector2f(GRID_SIZE - 1, GRID_SIZE - 1));
            outer.setPosition(x, y);
            outer.setFillColor(color);
            window.draw(outer);

            sf::RectangleShape inner(sf::Vector2f(GRID_SIZE / 2, GRID_SIZE / 2));
            inner.setFillColor(sf::Color::Black);
            inner.setPosition(x + GRID_SIZE / 4, y + GRID_SIZE / 4);
            window.draw(inner);
        }
    }

    int getScore() const { return score; }
    int getLevel() const { return level; }
    int getLinesCleared() const { return totalLinesCleared; }
    bool isGameOver() const { return gameOver; }

    float getDropInterval() const
    {
        return 1.0f / (level * 0.5f + 0.5f);
    }

private:
    bool tryMove(void (Shape::* move)(), void (Shape::* undo)())
    {
        if (gameOver || !currentPiece) return false;

        (currentPiece.get()->*move)();

        if (!currentPiece->isValid(board))
        {
            (currentPiece.get()->*undo)();
            return false;
        }
        return true;
    }
};

#endif
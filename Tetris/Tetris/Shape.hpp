#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <SFML/Graphics.hpp>
#include <vector>

const int GRID_SIZE = 30;
const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 20;
const int GRID_OFFSET_X = 60;
const int GRID_OFFSET_Y = 50;
const float BORDER_THICKNESS = GRID_SIZE / 4.0f;

struct Position
{
    int x, y;
    Position(int x = 0, int y = 0) : x(x), y(y) {}
    Position operator+(const Position& other) const
    {
        return Position(x + other.x, y + other.y);
    }
};

inline bool operator==(const Position& a, const Position& b)
{
    return a.x == b.x && a.y == b.y;
}

class Shape
{
protected:
    sf::Color color;
    std::vector<Position> layout;
    Position position;

public:
    Shape(sf::Color c, std::vector<Position> blocks) : color(c), layout(blocks), position(BOARD_WIDTH / 2, 1) {}

    virtual ~Shape() = default;

    void moveLeft() { position.x--; }
    void moveRight() { position.x++; }
    void moveDown() { position.y++; }
    void moveUp() { position.y--; }

    virtual void rotate() = 0;

    std::vector<Position> getPositions() const
    {
        std::vector<Position> positions;
        for (const auto& rel : layout) positions.push_back(position + rel);
        return positions;
    }

    void setPosition(Position p) { position = p; }
    Position getPosition() const { return position; }
    sf::Color getColor() const { return color; }

    bool isValid(const std::vector<std::vector<sf::Color>>& board) const
    {
        for (const auto& pos : getPositions())
        {
            if (pos.x < 0 || pos.x >= BOARD_WIDTH || pos.y >= BOARD_HEIGHT)
                return false;
            if (pos.y >= 0 && board[pos.y][pos.x] != sf::Color::Black)
                return false;
        }
        return true;
    }

    void draw(sf::RenderWindow& window) const
    {
        for (const auto& pos : getPositions())
        {
            float x = pos.x * GRID_SIZE + GRID_OFFSET_X;
            float y = pos.y * GRID_SIZE + GRID_OFFSET_Y;

            // Outer block
            sf::RectangleShape outer(sf::Vector2f(GRID_SIZE - 1, GRID_SIZE - 1));
            outer.setPosition(x, y);
            outer.setFillColor(color);
            window.draw(outer);

            // Inner block (simulate pixel grid)
            sf::RectangleShape inner(sf::Vector2f(GRID_SIZE / 2, GRID_SIZE / 2));
            inner.setPosition(x + GRID_SIZE / 4, y + GRID_SIZE / 4);
            inner.setFillColor(sf::Color::Black); // inner square (like punchout)
            window.draw(inner);
        }
    }

    const std::vector<Position>& getLayout() const
    {
        return layout;
    }

};

#endif
#ifndef GAME_PIECES_HPP
#define GAME_PIECES_HPP

#include "Shape.hpp"

class IPIECE : public Shape
{
public:
    IPIECE() : Shape(sf::Color::Blue,
        { {-1, 0}, {0, 0}, {1, 0}, {2, 0} }) {}
    void rotate() override { for (auto& p : layout) std::swap(p.x, p.y), p.x *= -1; }
};

class TPIECE : public Shape
{
public:
    TPIECE() : Shape(sf::Color(128, 0, 128),
        { {-1, 0}, {0, 0}, {1, 0}, {0, 1} }) {}
    void rotate() override { for (auto& p : layout) std::swap(p.x, p.y), p.x *= -1; }
};

class OPIECE : public Shape
{
public:
    OPIECE() : Shape(sf::Color::Yellow,
        { {0, 0}, {1, 0}, {0, 1}, {1, 1} }) {}
    void rotate() override {} // no rotation needed
};

class LPIECE : public Shape
{
public:
    LPIECE() : Shape(sf::Color::Red,
        { {-1, 0}, {0, 0}, {1, 0}, {-1, 1} }) {}
    void rotate() override { for (auto& p : layout) std::swap(p.x, p.y), p.x *= -1; }
};

class JPIECE : public Shape
{
public:
    JPIECE() : Shape(sf::Color::Green,
        { {-1, 0}, {0, 0}, {1, 0}, {1, 1} }) {}
    void rotate() override { for (auto& p : layout) std::swap(p.x, p.y), p.x *= -1; }
};

class ZPIECE : public Shape
{
public:
    ZPIECE() : Shape(sf::Color(255, 165, 0),
        { {-1, 0}, {0, 0}, {0, 1}, {1, 1} }) {}
    void rotate() override { for (auto& p : layout) std::swap(p.x, p.y), p.x *= -1; }
};

class SPIECE : public Shape
{
public:
    SPIECE() : Shape(sf::Color::Magenta,
        { {0, 0}, {1, 0}, {-1, 1}, {0, 1} }) {}
    void rotate() override { for (auto& p : layout) std::swap(p.x, p.y), p.x *= -1; }
};

#endif
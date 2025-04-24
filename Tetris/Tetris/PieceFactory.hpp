#ifndef PIECE_FACTORY_HPP
#define PIECE_FACTORY_HPP

#include "GamePieces.hpp"
#include <memory>
#include <random>

class PieceFactory
{
private:
    std::mt19937 rng{ std::random_device{}() };
    std::uniform_int_distribution<int> dist{ 0, 6 };

public:
    std::unique_ptr<Shape> createRandomPiece()
    {
        switch (dist(rng))
        {
        case 0:
            return std::make_unique<IPIECE>();
        case 1:
            return std::make_unique<JPIECE>();
        case 2:
            return std::make_unique<LPIECE>();
        case 3:
            return std::make_unique<OPIECE>();
        case 4:
            return std::make_unique<SPIECE>();
        case 5:
            return std::make_unique<TPIECE>();
        case 6:
            return std::make_unique<ZPIECE>();
        default:
            return nullptr;
        }
    }
};

#endif
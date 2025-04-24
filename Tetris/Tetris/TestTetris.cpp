#include "GameBoard.hpp"
#include "GamePieces.hpp"
#include <cassert>
#include <iostream>

void testPieceMovement()
{
    GameBoard board;
    board.reset();
    int left = 0;
    while (board.moveLeft()) ++left;
    assert(left > 0 && "Should be able to move left");

    board.reset();
    int right = 0;
    while (board.moveRight()) ++right;
    assert(right > 0 && "Should be able to move right");
}

void testPieceRotation()
{
    TPIECE t;
    auto before = t.getPositions();
    t.rotate();
    auto after = t.getPositions();
    assert(before != after && "T piece should rotate");
}

void testLineClearing()
{
    GameBoard board;
    for (int x = 0; x < 10; ++x)
        board.hardDrop();

    assert(board.getLinesCleared() >= 1);
}

void testPieceSpawn()
{
    GameBoard board;
    auto initialScore = board.getScore();
    auto initialLines = board.getLinesCleared();

    assert(initialScore == 0 && "Initial score should be 0");
    assert(initialLines == 0 && "Initial lines cleared should be 0");
    assert(!board.isGameOver() && "Game should not be over at start");
}

void testGameOver()
{
    GameBoard board;
    for (int i = 0; i < 200; ++i)
        board.hardDrop();

    assert(board.isGameOver() && "Game should be over after repeated drops");
}

//int main()
//{
//    testPieceMovement();
//    testPieceRotation();
//    testLineClearing();
//    testPieceSpawn();
//    testGameOver();
//    std::cout << "All tests passed!\n";
//}
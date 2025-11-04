/*****************************************************************************
 * Exercise:    Shape
 * Date:        3/11/2025
 * Developer:   Tal Hindi
 * Status:      Finished
 *****************************************************************************/

#include "shape.hpp"

#include <iostream>

using namespace ilrd;

void Shape::Draw()
{
    std::cout << "Drawing generic shape\n";
}

void Shape::Move()
{
    std::cout << "Moving generic shape\n";
}

void Line::Draw()
{
    std::cout << "Drawing a line\n";
}

void Line::Move()
{
    std::cout << "Moving a line\n";
}

void Rectangle::Draw()
{
    std::cout << "Drawing a rectangle\n";
}

void Rectangle::Move()
{
    std::cout << "Moving a rectangle\n";
}

void Circle::Draw()
{
    std::cout << "Drawing a circle\n";
}

void Circle::Move()
{
    std::cout << "Moving a circle\n";
}

void Square::Draw()
{
    std::cout << "Drawing a square\n";
}

void Square::Move()
{
    std::cout << "Moving a square\n";
}



/*****************************************************************************
 * Exercise:    Shape
 * Date:        3/11/2025
 * Developer:   Tal Hindi
 * Status:      Finished
 *****************************************************************************/


#include "shape.hpp"
#include <iostream>

using namespace ilrd;

int main()
{
    Shape* shapes[4];

    shapes[0] = new Rectangle();
    shapes[1] = new Line();
    shapes[2] = new Circle();
    shapes[3] = new Square();

    for (int i = 0; i < 4; ++i)
    {
        shapes[i]->Draw();
        shapes[i]->Move();
    }

    return 0;
}

/*****************************************************************************
 * Exercise:    Shape
 * Date:        3/11/2025
 * Developer:   Tal Hindi
 * Status:      Finished
 *****************************************************************************/

#ifndef __ILRD_SHAPE__
#define __ILRD_SHAPE__

namespace ilrd
{

class Shape
{
public:
    virtual void Draw();
    virtual void Move();
};

class Line: public Shape
{
    void Draw();
    void Move();
};

class Rectangle: public Shape
{
    void Draw();
    void Move();
};

class Circle: public Shape
{
    void Draw();
    void Move();
};

class Square: public Shape
{
    void Draw();
    void Move();
};

} // namespace ilrd

#endif //__ILRD_SHAPE__
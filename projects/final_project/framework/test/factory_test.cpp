/*****************************************************************************
 Exercise:    Factory Design Pattern - Test
 Date:        04/12/2025
 Developer:   Tal Hindi
 Reviewer:    Avi Tobar
 Status:      Approved
 *****************************************************************************/

#include <iostream>   // std::cout
#include <string>     // std::string

#include "factory.hpp"

using namespace ilrd;

class Shape
{
public:
    virtual ~Shape() noexcept = default;
    virtual void Draw() const = 0;
};

class Circle : public Shape
{
public:
    explicit Circle(double radius) : m_radius(radius) {}

    void Draw() const override
    {
        std::cout << "Drawing Circle with radius: " << m_radius << std::endl;
    }
private:
    double m_radius;
};

class Rectangle : public Shape
{
public:
    Rectangle(double height) : m_height(height) {}

    void Draw() const override
    {
        std::cout << "Drawing Rectangle "  << m_height << std::endl;
    }
private:
    double m_height;
};

/*****************************************************************************
 * Test class with no constructor arguments
 *****************************************************************************/
class DefaultShape : public Shape
{
public:
    DefaultShape() = default;

    void Draw() const override
    {
        std::cout << "Drawing DefaultShape (unit square)" << std::endl;
    }
};

/*****************************************************************************
 * Test Functions
 *****************************************************************************/
void TestBasicFactory()
{
    std::cout << "=== Test: Basic Factory with single argument ===" << std::endl;
    
    Factory<Shape, std::string, double> circleFactory;
    
    circleFactory.Add("circle", [](double radius) {
        return std::make_shared<Circle>(radius);
    });
    
    auto circle = circleFactory.Create("circle", 5.0);
    circle->Draw();
    
    std::cout << "PASSED" << std::endl << std::endl;
}

void TestMultipleArgs()
{
    std::cout << "=== Test: Factory with multiple arguments ===" << std::endl;
    
    Factory<Shape, std::string, double> rectFactory;
    
    rectFactory.Add("rectangle", [](double w) {
        return std::make_shared<Rectangle>(w);
    });
    
    auto rect = rectFactory.Create("rectangle", 4.0);
    rect->Draw();
    
    std::cout << "PASSED" << std::endl << std::endl;
}

void TestNoArgs()
{
    std::cout << "=== Test: Factory with no arguments ===" << std::endl;
    
    Factory<Shape, std::string> defaultFactory;
    
    defaultFactory.Add("default", []() {
        return std::make_shared<DefaultShape>();
    });
    
    auto shape = defaultFactory.Create("default");
    shape->Draw();
    
    std::cout << "PASSED" << std::endl << std::endl;
}

void TestKeyNotFound()
{
    std::cout << "=== Test: Exception on key not found ===" << std::endl;
    
    Factory<Shape, std::string, double> factory;
    
    try
    {
        factory.Create("nonexistent", 1.0);
        std::cout << "FAILED - Expected exception" << std::endl;
    }
    catch (const std::out_of_range& e)
    {
        std::cout << "Caught expected exception: " << e.what() << std::endl;
        std::cout << "PASSED" << std::endl;
    }
    
    std::cout << std::endl;
}

void TestOverwriteKey()
{
    std::cout << "=== Test: Overwriting existing key ===" << std::endl;
    
    Factory<Shape, std::string, double> factory;
    
    factory.Add("shape", [](double r) {
        return std::make_shared<Circle>(r);
    });
    
    auto shape1 = factory.Create("shape", 2.0);
    shape1->Draw();
    
    // Overwrite with different creator
    factory.Add("shape", [](double side) {
        return std::make_shared<Rectangle>(side);
    });
    
    auto shape2 = factory.Create("shape", 2.0);
    shape2->Draw();
    
    std::cout << "PASSED" << std::endl << std::endl;
}

void TestIntegerKey()
{
    std::cout << "=== Test: Factory with integer key ===" << std::endl;
    
    Factory<Shape, int, double> factory;
    
    factory.Add(1, [](double r) { return std::make_shared<Circle>(r); });
    factory.Add(2, [](double s) { return std::make_shared<Rectangle>(s); });
    
    auto circle = factory.Create(1, 3.0);
    auto rect = factory.Create(2, 4.0);
    
    circle->Draw();
    rect->Draw();
    
    std::cout << "PASSED" << std::endl << std::endl;
}


int main()
{
 
    TestBasicFactory();
    TestMultipleArgs();
    TestNoArgs();
    TestKeyNotFound();
    TestOverwriteKey();
    TestIntegerKey();

    return 0;
}
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

class curve {
public:

int x_center;
int y_center;
double x_freq;
double y_freq;
double x;
double y;
double r;
sf::RenderWindow& window;

curve(int x_center, int y_center, double x_freq, double y_freq, double r, sf::RenderWindow& win)
    : x_center(x_center), y_center(y_center), x_freq(x_freq), y_freq(y_freq), r(r), window(win) {
    x = x_center + r;
    y = y_center;
}

void updatePos(double t) {
    x = x_center + r * std::cos(t * x_freq);
    y = y_center + r * std::sin(t * y_freq);
}

void show() {
    sf::CircleShape point(3.f); // Create a small circle representing the point
    point.setFillColor(sf::Color::Red); // Set the color of the point

    // Set the position of the point at (x, y)
    point.setPosition(static_cast<float>(x), static_cast<float>(y));

    // Draw the point on the window
    window.draw(point);
}
};


int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Lissajous Curves!");
    float t = 0;
    int cols = 10;
    int rows = 10;
    
    std::vector<curve> curves;
    for(int i = 0; i < cols; i++) {
        for(int j = 0; j < rows; j++) {
            int x_center = i * (window.getSize().x) / cols;
            int y_center = j * (window.getSize().y) / rows;
            curve my_curve(x_center, y_center, i, j, 20, window);
            curves.push_back(my_curve);
        }
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        for (curve curve: curves) {
        curve.updatePos(t);
        curve.show();

        }
        // window.clear();
        // window.draw(shape);
        window.display();
        t = t + 0.001;
    }

    return 0;
}
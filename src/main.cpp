#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <iostream>

class curve {
public:

int x_center;
int y_center;
double x_freq;
double y_freq;
double x;
double y;
double r;
int max_trail = 300;
std::vector<sf::CircleShape> pt_trail;
sf::RenderWindow& window;

curve(int x_center, int y_center, double x_freq, double y_freq, double r, sf::RenderWindow& win)
    : x_center(x_center), y_center(y_center), x_freq(x_freq), y_freq(y_freq), r(r), window(win) {
    x = x_center;
    y = y_center;
}

void updatePos(double t) {
    x = x_center + r * std::cos(t * x_freq) + r;
    y = y_center + r * std::sin(t * y_freq) + r;


    sf::CircleShape point(1.f); // Create a small circle representing the point
    int r_clr = x / window.getSize().x * 255;
    int b_clr = y / window.getSize().y * 255;
    sf::Color clr(r_clr, 100, b_clr);
    point.setFillColor(clr); // Set the color of the point

    // Set the position of the point at (x, y)
    point.setPosition(static_cast<float>(x), static_cast<float>(y));

    pt_trail.push_back(point);

    if (pt_trail.size() > max_trail) {
        pt_trail.erase(pt_trail.begin());
    }

}

void show() {

    for (int i = 0; i < pt_trail.size(); i++){
        sf::Color color = pt_trail.at(i).getFillColor();
        color.a = static_cast<int>(255 * (i + (max_trail - pt_trail.size()))/max_trail); 
        pt_trail.at(i).setFillColor(color);
        window.draw(pt_trail.at(i));
    }
    
}
};


int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Lissajous Curves!");
    float t = 0;
    int cols = 10;
    int rows = 10;
    int r = window.getSize().x / (cols * 4);
    
    std::vector<curve> curves;
    for(int i = 0; i < cols; i++) {
        for(int j = 0; j < rows; j++) {
            int x_center = i * (window.getSize().x) / cols;
            int y_center = j * (window.getSize().y) / rows;
            curve my_curve(x_center, y_center, i, j, r, window);
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

        window.clear();

        for (curve& curve: curves) {
            curve.updatePos(t);
            curve.show();
        }
        window.display();
        t = t + 0.01;
    }

    return 0;
}
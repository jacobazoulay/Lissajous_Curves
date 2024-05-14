#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <iostream>
#include <queue>
#include <algorithm>

int WIDTH = 800;
int HEIGHT = 800;

class Curve
{
public:
    int x_center;
    int y_center;
    int x_freq;
    int y_freq;
    float x;
    float y;
    float r;
    int max_trail = 100;
    sf::RenderWindow &window;
    std::queue<sf::Vector2f> pts;

    Curve(int x_center, int y_center, int x_freq, int y_freq, float r, sf::RenderWindow &win)
        : x_center(x_center), y_center(y_center), x_freq(x_freq), y_freq(y_freq), r(r), window(win)
    {
        x = x_center;
        y = y_center;
        if (x_freq == 0 ^ y_freq == 0)
        {
            this->x_freq = std::max(x_freq, y_freq);
            this->y_freq = this->x_freq;
        }
    }

    void updatePos(double t)
    {
        if (t == 0)
        {
            while (!pts.empty())
            {
                pts.pop();
            }
        }
        x = x_center + r * std::cos(t * x_freq - 3.14159 / 2) + 2 * r;
        y = y_center + r * std::sin(t * y_freq - 3.14159 / 2) + 2 * r;

        sf::Vector2f pt = {x, y};

        pts.push(pt);
        if (pts.size() > max_trail)
        {
            pts.pop();
        }
    }

    void drawCurve()
    {
        std::queue<sf::Vector2f> copyQueue = pts;
        sf::VertexArray shape(sf::LineStrip);
        while (!copyQueue.empty())
        {
            sf::Vector2f pt = copyQueue.front();
            int r_clr = (pt.x / WIDTH) * 255;
            int b_clr = (pt.y / HEIGHT) * 255;
            int a_clr = 255 * (max_trail - copyQueue.size()) / max_trail;
            sf::Color clr(r_clr, 100, b_clr, a_clr);
            shape.append(sf::Vertex(pt, clr));
            copyQueue.pop();
        }

        window.draw(shape);
    }

    void drawTracers()
    {
        if (x_center == 0 and y_center != 0)
        {
            sf::Vector2f pt_start = {0.0, (float)y};
            sf::Vector2f pt_end = {(float)WIDTH, (float)y};
            sf::VertexArray h_line(sf::LineStrip);
            sf::Color clr(255, 255, 255, 100);
            h_line.append(sf::Vertex(pt_end, clr));
            h_line.append(sf::Vertex(pt_start, clr));
            window.draw(h_line);
        }

        if (y_center == 0 and x_center != 0)
        {
            sf::Vector2f pt_start = {(float)x, 0.0};
            sf::Vector2f pt_end = {(float)x, (float)HEIGHT};
            sf::VertexArray v_line(sf::LineStrip);
            sf::Color clr(255, 255, 255, 100);
            v_line.append(sf::Vertex(pt_end, clr));
            v_line.append(sf::Vertex(pt_start, clr));
            window.draw(v_line);
        }

        sf::CircleShape head(2.f);
        head.setPosition(x - 2, y - 2);
        window.draw(head);
    }
};

class LissajousAnimator
{
private:
    sf::RenderWindow window;
    std::vector<Curve> curves;
    sf::Clock clock;
    int  cols = 5;
    int rows = 5;
    float t = 0;
    const int max_frame_rt = 30;

    void createCurves()
    {
        float r = WIDTH / (cols * 4);
        for (int i = 0; i < cols; i++)
        {
            for (int j = 0; j < rows; j++)
            {
                if (j == 0 && i == 0)
                {
                    continue;
                }
                int x_center = i * (WIDTH) / cols;
                int y_center = j * (HEIGHT) / rows;
                Curve my_curve(x_center, y_center, i, j, r, window);
                curves.push_back(my_curve);
            }
        }
    }

    void checkWindowClose() 
    {   std::cout << "here" << std::endl;
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    void step() 
    {
        for (Curve &curve : curves)
        {
            curve.updatePos(t);
            curve.drawCurve();
        }
        for (Curve &curve : curves)
        {
            curve.drawTracers();
        }

        t = t + (2 * 3.1415 / curves.at(0).max_trail);
        if (t > 3.1415 * 2)
        {
            t = 0;
        }

    }

    void enforceFrameRate() 
    {
        sf::Time elapsed = clock.getElapsedTime();
        float delta_time = 1.0 / max_frame_rt - elapsed.asSeconds();
        sf::sleep(sf::seconds(std::max(0.f, delta_time)));

    }

    void drawLoop()
    {
        while (window.isOpen())
        {
            clock.restart();
            window.clear();
            checkWindowClose();
            step();
            enforceFrameRate();
            window.display();
        }
    }

public:
    LissajousAnimator()
    {
        
    }

    void run()
    {   
        window.create(sf::VideoMode(WIDTH, HEIGHT), "Lissajous Curves!");
        createCurves();
        drawLoop();

    }
};

int main()
{
    LissajousAnimator animator;
    animator.run();
    return 0;
}
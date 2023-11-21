#include <SFML/Graphics.hpp>
#include "obj.h"
#include <Vector>
#include <SFML/System/Vector2.hpp>
#include <math.h>
#include <iostream>
#include "vectFunctions.h"
#include <string>
#include <chrono>

using std::cout;

#define SCREEN_WIDTH 680
#define SCREEN_HEIGHT 480

#define ROT_SPEED 3.0f;

int mymax(int a, int b, int c) {
    int tmp_max = a;
    if (tmp_max < b) tmp_max = b;
    if (tmp_max < c) tmp_max = c;
    return tmp_max;
}
double get_time()
{
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() / 1e6;
}

void drawFrame(sf::RenderWindow *rw,obj object, vector<light> ls) {
    float shade=0;


    //vector<Vector2f> points_to_draw = object.to_draw_vert();
    //sf::Vertex line[2];
    //sf::CircleShape circle;
    //for (int pid = 0; pid < points_to_draw.size(); pid++) {
    //    //cout << points_to_draw[pid].x<<" " << points_to_draw[pid].y << "\n";
    //    circle = sf::CircleShape(5);
    //    circle.setFillColor(sf::Color(0, 0, 0, 0));
    //    circle.setPosition(points_to_draw[pid]);
    //    circle.setOutlineColor(sf::Color(0, 100, 190,255));
    //    circle.setOutlineThickness(1);
    //    (*rw).draw(circle);
    //}
    sf::ConvexShape faceDraw;
    for (int face = 0; face < object.faces.size(); face++) {
        faceDraw.setPointCount(object.to_draw_face(object.faces[face]).size());
        for (int p = 0; p < object.to_draw_face(object.faces[face]).size(); p++) {
            //cout << object.to_draw_face(object.faces[face])[p].x  << " " << object.to_draw_face(object.faces[face])[p].x << '\n';
            faceDraw.setPoint(p, object.to_draw_face(object.faces[face])[p]);
        }
        /*_________________Вся магия__________________*/

        //cout << dot(object.faces[face].normal, sunlight) << "\n";
        sf::Color FaceColor;
        float dot_calc = 0;
        int r = 0, g = 0, b = 0;
        for (int light_i = 0; light_i < ls.size(); light_i++) {
            dot_calc = dot(object.faces[face].normal, ls[light_i].dir);
            if (dot_calc<0) {
                shade = -dot_calc;
            }
            else
            {
                shade = 0;
            }
            r += shade * ls[light_i].color.r;
            g += shade * ls[light_i].color.g;
            b += shade * ls[light_i].color.b;
            //cout << FaceColor.r << " | "<<  (int)ls[light_i].color.r <<" !";
        }
        int max_color = mymax(r,g,b);
        if (max_color != 0 and max_color > 255) {
            FaceColor.r = (float)r / (float)max_color * 255;
            FaceColor.g = (float)g / (float)max_color * 255;
            FaceColor.b = (float)b / (float)max_color * 255;
        }
        else {
            FaceColor.r = r;
            FaceColor.g = g;
            FaceColor.b = b;
        }
        cout << max_color<< " " << FaceColor.r << " " << FaceColor.g << " " << FaceColor.b << '\n';

        faceDraw.setFillColor(FaceColor);
        faceDraw.setOutlineColor(sf::Color(0, 100, 190));
        //faceDraw.setOutlineThickness(1);
        if(dot(object.faces[face].normal, Vector3f(0,0,1))<0)
            (*rw).draw(faceDraw);
    }

}






int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Cube makes rot rot",7U,settings);
    //sf::CircleShape shape(100.f);
    //shape.setFillColor(sf::Color::Green);

    sf::Vector3f viewDir(0.0f, 0.0f, 1.0f);

    sf::Vertex* point = new sf::Vertex(sf::Vector2f(680 / 2, 480 / 2), sf::Color::Blue);

    char path[] = "C:/Users/z2016/Desktop/tor.obj";
    inputObj CubeFromObj(path, sf::Vector2f(680 / 2, 480 / 2));

    Cube cube;
    cube.offset = Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    Vector3f rot_angles(0,0,0);

    vector<light> lights;
    //lights.push_back(light(1, 1, 1, sf::Color(255, 255, 255)));
    lights.push_back(light(-1, 0, 0, sf::Color(255, 0, 0)));
    lights.push_back(light(1, 0, 0, sf::Color(0, 0, 255)));

    double start=1, end=0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.scancode)
                {
                case sf::Keyboard::W:
                    rot_angles.x = -ROT_SPEED;
                    break;
                case sf::Keyboard::S:
                    rot_angles.x = ROT_SPEED;
                    break;
                case sf::Keyboard::A:
                    rot_angles.y = ROT_SPEED;
                    break;
                case sf::Keyboard::D:
                    rot_angles.y = -ROT_SPEED;
                    break;
                case sf::Keyboard::Q:
                    rot_angles.z = -ROT_SPEED;
                    break;
                case sf::Keyboard::E:
                    rot_angles.z = ROT_SPEED;
                    break;
                default:
                    break;
                }
            }
            if (event.type == sf::Event::KeyReleased) {
                switch (event.key.scancode)
                {
                case sf::Keyboard::W:
                    rot_angles.x = 0;
                    break;
                case sf::Keyboard::S:
                    rot_angles.x = 0;
                    break;
                case sf::Keyboard::A:
                    rot_angles.y = 0;
                    break;
                case sf::Keyboard::D:
                    rot_angles.y = 0;
                    break;
                case sf::Keyboard::Q:
                    rot_angles.z = 0;
                    break;
                case sf::Keyboard::E:
                    rot_angles.z = 0;
                    break;
                default:
                    break;
                }
            }
        }
        window.clear();
        normalize(&rot_angles);
        //rot_angles = rot_angles * ROT_SPEED;
        //sunlight= rotateVect(sunlight, 0, 0, 0.0005);
        //window.draw(shape);
        CubeFromObj.rotate(rot_angles.x*(start-end), rot_angles.y * (start - end), rot_angles.z * (start - end));

        start = get_time();
        drawFrame(&window, CubeFromObj, lights);
        end = get_time();
        window.display();
    }

    return 0;
}
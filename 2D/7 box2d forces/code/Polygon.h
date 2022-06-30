#pragma once

#include <ciso646>
#include <memory>
#include <vector>
#include <Box2D/Box2D.h>
#include <string>
#include <Box2D/b2_body.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;


class Polygon
{
public:
    b2Body* body;
    Color color;
    b2PolygonShape body_shape;
    


public:


    b2FixtureDef body_fixture;
public:

    Polygon() {};
    Polygon(b2World& physics_world, b2BodyType body_type, float angle, float x, float y, Color c);


    Polygon& operator = (const Polygon& t)
    {
        body_fixture = t.body_fixture;
        body_shape = t.body_shape;
        body = t.body;
        color = t.color;
        return *this;
    }

    //Crea fixture tipo ground
    void createfixture_ground(float width, float height, int id = 0);

    //Crea fixture de plataforma verde
    void createfixture_platform(float width, float height, int id=0);

    //Doble fixture para crear la esquina
    void createfixture_goal( float width, float height2, int id=0);

    //Crea un triangulo
    void createfixture_triangle(float width, float height);

    //Crea una cruz
    void createfixture_cross(float width, float height2, int id = 0);


    void render(RenderWindow& renderer, float scale);
   

    inline Vector2f box2d_position_to_sfml_position(const b2Vec2& box2d_position, float window_height, float scale)
    {
        return Vector2f(box2d_position.x * scale, window_height - box2d_position.y * scale);
    };
};
#pragma once

#include <ciso646>
#include <memory>
#include <vector>
#include <Box2D/Box2D.h>
#include <Box2D/b2_world_callbacks.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


using namespace sf;
using namespace std;


class Ball: public b2ContactListener
{

	b2BodyDef body_definition;
    b2CircleShape body_shape;
public:
	b2Body* body;

public:
    Ball() {};
	Ball(b2World& physics_world, b2BodyType body_type, float x, float y, float radius, Color c);
    inline Vector2f box2d_position_to_sfml_position(const b2Vec2& box2d_position, float window_height, float scale)
    {
        return Vector2f(box2d_position.x * scale, window_height - box2d_position.y * scale);
    };
    ~Ball() = default;   
    Ball& operator = (const Ball& t)
    {
        body_definition = t.body_definition;
        body_shape = t.body_shape;
        body = t.body;
        return *this;
    }

    // http://www.iforce2d.net/b2dtut/collision-callbacks
    void BeginContact(b2Contact* contact)
    {
        b2Body * b_contacted = contact->GetFixtureA()->GetBody();
        //Comprueba que id tiene el colisionado y reacciona ante la colision
        if (b_contacted->GetUserData().pointer == 1 && b_contacted->GetPosition().y < 6.1f)
        {
            b_contacted->SetLinearVelocity({ 0, 1 });
            body->SetAngularDamping(10);

        }
        else if (b_contacted->GetUserData().pointer == 2 && b_contacted->GetPosition().y < 7)
        {
            b_contacted->SetLinearVelocity({ 0, 1 });
            body->GetFixtureList()->SetRestitution(0.75);
            body->SetAngularDamping(0);
        }
        else if (b_contacted->GetUserData().pointer == 4 )
        {
           
            body->GetFixtureList()->SetRestitution(0.3f);
         
        }

    }

    void EndContact(b2Contact* contact)
    {
        b2Body* b_contacted = contact->GetFixtureA()->GetBody();
   
        if (b_contacted->GetUserData().pointer == 1 && body->GetPosition().x < b_contacted->GetPosition().x)
        {
            b_contacted->SetLinearVelocity({ 0, -1 });
        }
        else if (b_contacted->GetUserData().pointer == 2 && body->GetPosition().x < b_contacted->GetPosition().x)
        {
            b_contacted->SetLinearVelocity({ 0,-1 });
           
        }
    }
    

    
};
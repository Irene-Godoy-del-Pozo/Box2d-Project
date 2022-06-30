#include "Ball.h"

Ball::Ball(b2World& physics_world, b2BodyType body_type, float x, float y, float radius, Color c)
{
    // Se crea el body:

    body_definition.type = body_type;
    body_definition.position.Set(x, y);                          
    body = physics_world.CreateBody(&body_definition);

    // Se añande una fixture:

    body_shape.m_radius = radius;

    b2FixtureDef body_fixture;

    body_fixture.shape = &body_shape;
    body_fixture.density = 1.00f;
    body_fixture.restitution = 0.5f;
    body_fixture.friction =0.5f;

    body->CreateFixture(&body_fixture);



}



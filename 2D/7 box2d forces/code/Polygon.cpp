#include "Polygon.h"

Polygon::Polygon(b2World& physics_world, b2BodyType body_type, float angle, float x, float y, Color c)

{
    //https://box2d.org/documentation/md__d_1__git_hub_box2d_docs_dynamics.html
   
    color = c;

    //Se configura la definicion del body
    b2BodyDef body_definition;
    body_definition.type = body_type;
    body_definition.position.Set(x, y);
    body_definition.angle = angle * b2_pi;

    //Se crea el body a traves de la definicion
    body = physics_world.CreateBody(&body_definition);

}

void Polygon::createfixture_ground(float width, float height, int id)
{
    //https://stackoverflow.com/questions/64576289/undefined-reference-when-trying-to-use-box2ds-b2bodysetuserdatavoid


    body->GetUserData().pointer = id;

    b2Vec2 vertices[4];
    vertices[0].Set(0, 0);
    vertices[1].Set(width, 0);
    vertices[2].Set(width, height);
    vertices[3].Set(0, height);

    body_shape.Set(vertices, 4);


    //Creo la fixture

    body_fixture.shape = &body_shape;
    body_fixture.density = 1.00f;
    body_fixture.restitution = 0.002f;
    body_fixture.friction = 0.5f;




    body->CreateFixture(&body_fixture);

}

void Polygon::createfixture_platform(float width, float height, int id)
{
    body->GetUserData().pointer = id;
    b2Vec2 vertices[4];
    vertices[0].Set(0, 0);
    vertices[1].Set(width, 0);
    vertices[2].Set(width, height);
    vertices[3].Set(0, height);

    body_shape.Set(vertices, 4);
    body_fixture.shape = &body_shape;
    body_fixture.density = 1.00f;
    body_fixture.restitution = 0.1;
    body_fixture.friction = 20;



    body->CreateFixture(&body_fixture);
}

void Polygon::createfixture_goal(float width, float height2, int id)
{

    body->GetUserData().pointer = id;

    //Primera fixture

    body_fixture.shape = &body_shape;
    body_fixture.density = 1.00f;
    body_fixture.restitution = 0.50f;
    body_fixture.friction = 0.50f;


    body_shape.SetAsBox(width / 2, height2 / 2, b2Vec2(-0, 0), 0);


    body->CreateFixture(&body_fixture);

    // Segunda fixture
    body_fixture.shape = &body_shape;
    body_fixture.density = 1.00f;
    body_fixture.restitution = 0.50f;
    body_fixture.friction = 0.50f;


    body_shape.SetAsBox(width / 2, height2 / 2, b2Vec2(width / 2 - height2 / 2, width / 2 - height2 / 2), 0.5 * b2_pi); 

    body->CreateFixture(&body_fixture);
}

void Polygon::createfixture_triangle(float width, float height)
{


    // Se añande una fixture:
    b2Vec2 vertices[3];
    vertices[0].Set(0, 0);
    vertices[1].Set(width, 0);
    vertices[2].Set(width / 2, height);



    body_shape.Set(vertices, 3);


    body_fixture.shape = &body_shape;
    body_fixture.density = 1.00f;
    body_fixture.restitution = 0.50f;
    body_fixture.friction = 0.50f;

    body->CreateFixture(&body_fixture);
}

void Polygon::createfixture_cross(float width, float height2, int id)
{

    body->GetUserData().pointer = id;


    //Primera fixture
    body_fixture.shape = &body_shape;
    body_fixture.density = 1.00f;
    body_fixture.restitution = 0.50f;
    body_fixture.friction = 0.50f;


    body_shape.SetAsBox(width / 2, height2 / 2, b2Vec2(0, 0), 0); 


    body->CreateFixture(&body_fixture);

    // Segunda fixture
    body_fixture.shape = &body_shape;
    body_fixture.density = 1.00f;
    body_fixture.restitution = 0.50f;
    body_fixture.friction = 0.50f;


    body_shape.SetAsBox(width / 2, height2 / 2, b2Vec2(0, 0), 0.5 * b2_pi); 

    body->CreateFixture(&body_fixture);

}

void Polygon::render(RenderWindow& renderer, float scale)
{
    float window_height = (float)renderer.getSize().y;

    //Coge todas las fixtures del body y las renderiza
    for (b2Fixture* fixture = body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext())
    {

        b2PolygonShape* box2d_polygon = dynamic_cast<b2PolygonShape*>(fixture->GetShape());

        ConvexShape sfml_polygon;
        sfml_polygon.setPointCount(box2d_polygon->m_count);
        sfml_polygon.setFillColor(color);

        for (int index = 0; index < box2d_polygon->m_count; index++)
        {
            sfml_polygon.setPoint
            (
                index,
                box2d_position_to_sfml_position(b2Mul(body->GetTransform(), box2d_polygon->m_vertices[index]), window_height, scale)
            );
        }

        renderer.draw(sfml_polygon);


    }


}

//
//
// Controles:
//
//   Espacio: Usar torque.
//   Click Izquierdo: activar el palo de golf (arriba derecha).
//   A: Girar la canasta.
//   R: Reiniciar la escena
//

#include <ciso646>
#include <memory>
#include <vector>
#include <Box2D/Box2D.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Ball.h"
#include "Joint_Object.h"
#include "ParticleSystemManager.h"
#include "Polygon.h"
using namespace sf;
using namespace std;




//Objetos de tipo Polygon en la escena
const int num_polygon = 14;
Polygon objects[num_polygon];

//Jugador
Ball  player;


namespace
{

    /** En Box2D las coordenadas Y crecen hacia arriba y en SFML crecen hacia abajo desde el borde
      * superior. Esta función se encarga de convertir el sistema de coordenadas para que la escena
      * no se vea invertida.
      */
    inline Vector2f box2d_position_to_sfml_position (const b2Vec2 & box2d_position, float window_height, float scale)
    {
        return Vector2f(box2d_position.x * scale, window_height - box2d_position.y * scale);
    }

    // ------------------------------------------------------------------------------------------ //

    inline b2Vec2 sfml_position_to_box2d_position (const Vector2f & sfml_position, float window_height, float scale)
    {
        return b2Vec2(sfml_position.x / scale, (window_height - sfml_position.y) / scale);
    }

    // ------------------------------------------------------------------------------------------ //

    b2Body * create_edge (b2World & physics_world, b2BodyType body_type, float x0, float y0, float x1, float y1)
    {
        // Se crea el body:

        b2BodyDef body_definition;

        body_definition.type = body_type;
        body_definition.position.Set (0.f, 0.f);                        // Posición inicial absoluta

        b2Body * body = physics_world.CreateBody (&body_definition);

        // Se añande una fixture:

        b2EdgeShape body_shape;

        body_shape.SetTwoSided (b2Vec2(x0, y0), b2Vec2(x1, y1));        // Coordenadas locales respecto al centro del body

        b2FixtureDef body_fixture;

        body_fixture.shape = &body_shape;

        body->CreateFixture (&body_fixture);

        return body;
    }

    // ------------------------------------------------------------------------------------------ //

    void render_circle
    (
        b2Vec2              center,
        float               radius, 
        const b2Transform & transform, 
        RenderWindow      & renderer, 
        float               window_height, 
        float               scale
    )
    {
        CircleShape shape;

        radius *= scale;

        shape.setPosition  (box2d_position_to_sfml_position (b2Mul (transform, center), window_height, scale) - Vector2f(radius, radius));
        shape.setRadius    (radius);
        shape.setFillColor (Color::Magenta);

        renderer.draw (shape);
    }

    // ------------------------------------------------------------------------------------------ //

    void render (b2World & physics_world, RenderWindow & renderer, float scale)
    {
        // Se cachea el alto de la ventana en una variable local:

        float window_height = (float)renderer.getSize ().y;

        //Llamamos a la función render de cada objeto Polygon
        for (int i = 0; i < num_polygon; i++)
        {
            objects[i].render(renderer, scale);
        }
        
        //Se renderiza la pelota
        const b2Transform & body_transform = player.body->GetTransform ();

        // Se recorre la lista de fixtures del body:

        for (b2Fixture * fixture = player.body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext ())
        {
            // Se obtiene el tipo de forma de la fixture:

            b2Shape::Type shape_type = fixture->GetShape ()->GetType ();

               
            // Se crea un CircleShape a partir de los atributos de la forma de la fixture y del body:
            // En SFML el centro de un círculo no está en su position. Su position es la esquina superior izquierda
            // del cuadrado en el que está inscrito. Por eso a position se le resta el radio tanto en X como en Y.

            b2CircleShape * circle = dynamic_cast< b2CircleShape * >(fixture->GetShape ());

            render_circle  (circle->m_p, circle->m_radius, body_transform, renderer, window_height, scale);

                       
                    
                 
        }

        
    }
  
    // ------------------------------------------------------------------------------------------ //

    Ball crear_player(b2World& physics_world)
    {
        Ball pelota(physics_world, b2_dynamicBody, 2.5f, 5.3f, .2f, Color::Magenta);

        return pelota;
    }

    // ------------------------------------------------------------------------------------------ //

    void reset(b2World& physics_world, Joint_Object* golf, b2PrismaticJoint* piston)
    {
 
        //Destruimos los body b de canasta y balancin
        physics_world.DestroyBody(objects[9].body);
        physics_world.DestroyBody(objects[11].body);
        
      //Restablecemos la posicion de las plataformas verdes
        objects[3].body->SetTransform(b2Vec2(14,0),0);
        objects[4].body->SetTransform(b2Vec2(3, 4.5f), 0);

        //Creamos los joints y bodys b de la canasta y balancin
            //Balancin
        Polygon pala_balancin(physics_world, b2_dynamicBody, 0, 0.6f, 2.75f, Color::Yellow);
        pala_balancin.createfixture_ground(2.3f, .15f);

        Joint_Object balancin(physics_world, objects[0].body, pala_balancin.body, b2Vec2(1.2f, 1), b2Vec2(0, 0), true);
        balancin.setLimits(-1.25, 0.25);

            //Canasta
        Polygon corner(physics_world, b2_dynamicBody, 0, 2.5f, 4.5f, Color::Yellow);
        corner.createfixture_goal(1.25, .15f);

        Joint_Object canasta(physics_world, objects[0].body, corner.body, b2Vec2(2.87f, 2.6f), b2Vec2(-corner.body->GetLocalCenter().x, corner.body->GetLocalCenter().y), false);

        canasta.setLimits(0, 0.5f);

        canasta.setMotor(15, -3);
        
        //Restablecemos las posiciones iniciales del piston y el golf
        golf->setMotor(-20, 3.5f);
        piston->SetMotorSpeed(-5);


        //Sobrescribimos en el array los body b de canasta y balancin
        objects[9] = pala_balancin;
        objects[11] = corner;


    }

    // ------------------------------------------------------------------------------------------ //

    bool process (Window & window, float window_height, float scale, Joint_Object * golf, Joint_Object * canasta, b2PrismaticJoint * s, b2World& physics_world)
    {
        Event event;

        while (window.pollEvent (event))
        {
            switch (event.type)
            {
                case Event::KeyPressed:
                {
                    switch (event.key.code)
                    {
                        case Keyboard::R: //Resetea la escena
                        {
                          //Necesita llamarse dos veces para que no se pierda el input
                            reset(physics_world,golf,s);
                            reset(physics_world, golf, s);

                            physics_world.DestroyBody(player.body);


                            player = crear_player(physics_world);
                            break;
                        }
                        case Keyboard::Space: //Controla el piston
                        {
                            s->SetMaxMotorForce(60);
                            s->SetMotorSpeed(-s->GetMotorSpeed());
                            break;
                        }
                        case Keyboard::A:  //Controla la canasta
                        {
                       
                            canasta->joint_body->SetMotorSpeed(-canasta->joint_body->GetMotorSpeed());
                   
                            break;
                        }
                    }

                    break;
                }

                case Event::MouseButtonPressed:
                {
                    if (event.mouseButton.button == Mouse::Button::Left)  //Controla el palo de golf
                    {
                      
                        golf->joint_body->SetMaxMotorTorque(15);
                        golf->joint_body->SetMotorSpeed(-golf->joint_body->GetMotorSpeed());

                        
                    }

                    break;
                }

                case Event::Closed:
                {
                    return true;
                }
            }
        }

        return false;
    }


   
}



int main ()
{


    constexpr auto window_width = 1600u;
    constexpr auto window_height = 820u;
    RenderWindow window(VideoMode(window_width, window_height), "Box2D Forces", Style::Titlebar | Style::Close, ContextSettings(32));

    window.setVerticalSyncEnabled (true);

    // Se crea y se configura la escena física (de 8 x 6 metros):

    b2World physics_world{ b2Vec2{ 0, -10.f } };

    constexpr float left   = 0.01f;
    constexpr float right  = 16.00f;
    constexpr float top    = 8.20f;
    constexpr float bottom = 0.01f;

    create_edge (physics_world, b2_staticBody, left,  bottom,  right, bottom);
    create_edge (physics_world, b2_staticBody, left,  bottom,  left,  top   );
    create_edge (physics_world, b2_staticBody, left,  top,     right, top   );
    create_edge (physics_world, b2_staticBody, right, bottom,  right, top   );

    //Particulas
    ParticleSystemManager* m_VFXManager = new ParticleSystemManager(&window);
    m_VFXManager->AddParticleSystem(sf::Vector2f(window_width-650, window_height-50));

    //Pelota
    player = crear_player(physics_world);
    physics_world.SetContactListener(&player);

    //Suelos                                   
    
    Polygon suelo1(physics_world, b2_staticBody, 0, 0, 2.f, Color::Blue);
    suelo1.createfixture_ground(3.f, .25f);
    
    Polygon suelo2(physics_world, b2_staticBody, -0.25f, 2.83f, 2.06f, Color::Blue);
    suelo2.createfixture_ground(3.f, .25f);

    Polygon suelo3(physics_world, b2_staticBody, 0, 5, 0.f, Color::Blue);
    suelo3.createfixture_ground(9.f, .25f);

    Polygon suelo4(physics_world, b2_staticBody, 0, 5, 4.5f, Color::Blue);
    suelo4.createfixture_ground(4.f, .25f);

    Polygon suelo5(physics_world, b2_staticBody, 0.25f, 9, 4.5f, Color::Blue);
    suelo5.createfixture_ground(2.35f, .25f);

    Polygon suelo6(physics_world, b2_staticBody, 0, 10.49f, 6.1f, Color::Blue);
    suelo6.createfixture_ground(3.51f, .25f);

    //Balancin
    Polygon pala_balancin(physics_world, b2_dynamicBody, 0, 0.6f, 2.75f,  Color::Yellow);
    pala_balancin.createfixture_ground(2.3f, .15f);

    Joint_Object balancin(physics_world, suelo1.body, pala_balancin.body, b2Vec2(1.2f, 1), b2Vec2(0, 0),true);
    balancin.setLimits(-1.25f, 0.25f);

    //Canasta

    Polygon corner(physics_world, b2_dynamicBody, 0, 2.5f, 4.5f, Color::Yellow);
    corner.createfixture_goal(1.25f, .15f);

    Joint_Object canasta(physics_world, suelo1.body, corner.body, b2Vec2(2.87f, 2.6f), b2Vec2(-corner.body->GetLocalCenter().x, corner.body->GetLocalCenter().y), false);

    canasta.setLimits(0, 0.5f);

    canasta.setMotor(15, -3);

    //Piston
    Polygon pala_piston(physics_world, b2_dynamicBody, 0, 6, 0.f, Color::Yellow);
    pala_piston.createfixture_ground(2.f, .05f,4);

    b2PrismaticJointDef aa;
    aa.bodyA = suelo3.body;
    aa.bodyB = pala_piston.body;
    aa.localAxisA.Set(0, 1);
    aa.localAnchorA = b2Vec2(1.15f,0);
    aa.enableLimit = true;
    aa.lowerTranslation = 0.2f;
    aa.upperTranslation = 0.5f;
    aa.enableMotor = true;

    b2PrismaticJoint * s= dynamic_cast<b2PrismaticJoint*>(physics_world.CreateJoint(&aa)); 
    s->SetMaxMotorForce(10);
    s->SetMotorSpeed(-5);

    //Molinillo

    Polygon palas_molino(physics_world, b2_dynamicBody, 0, 5, 7, Color::Yellow);
    palas_molino.createfixture_cross(.8f, .05f);

    Joint_Object molinillo(physics_world, suelo4.body, palas_molino.body, b2Vec2(0, 3.28f), b2Vec2(0, 0), false);
    molinillo.setMotor(15, -5);

    //Golf

    Polygon pala_golf(physics_world, b2_dynamicBody, 0, 14, 7.f, Color::Yellow);
    pala_golf.createfixture_ground(1.6f, .05f);

    Joint_Object golf(physics_world, suelo6.body, pala_golf.body, b2Vec2(5.3f, 2), b2Vec2(-0.8f, 0), false);

    golf.setLimits(0, 0.6f);

    golf.setMotor(-20, 3);
   
   //Meta
    
    Polygon meta(physics_world, b2_staticBody, -0.25f, 1.4f, 6.3f, Color::Red);
    meta.createfixture_goal(1.5f, .25f, 3);

    //Plataformas verde                               
   
    Polygon plataforma1(physics_world, b2_kinematicBody, 0, 14, 0, Color::Green);
    plataforma1.createfixture_platform(2, .25f,1);
   
    Polygon plataforma2(physics_world, b2_kinematicBody, 0, 3, 4.5f, Color::Green);
    plataforma2.createfixture_platform(2, .25f, 2);

    //Añadimos al array de Polygons para poder ser renderizados con su color
    objects[0]  = suelo1;
    objects[1]  = suelo2;
    objects[2]  = suelo3;
    objects[3]  = plataforma1;
    objects[4]  = plataforma2;
    objects[5]  = suelo4;
    objects[6]  = suelo5;
    objects[7]  = suelo6;
    objects[8]  = meta;
    objects[9]  = pala_balancin;
    objects[10] = pala_golf;
    objects[11] = corner;
    objects[12] = palas_molino;
    objects[13] = pala_piston;
   


    const float physics_to_graphics_scale = 100.f;      // Escala para pasar de unidades de física a unidades de gráficos

    // Se ejecuta el bucle principal de la animación:

    
    const float target_fps  = 60.f;                     // Cuántos fotogramas por segundo se busca conseguir
    const float target_time = 1.f / target_fps;         // Duración en segundos de un fotograma a la tasa deseada

    float delta_time = target_time;                     // Previsión de la duración del fotograma actual
    bool  exit       = false;

    Clock timer;

    do
    {
        timer.restart ();

        ////////////// Input ///////////////////////////////////

        exit = process (window, window_height, physics_to_graphics_scale, &golf, &canasta, s, physics_world);

        ////////////////////////// Update/////////////////////////////

        physics_world.Step (delta_time, 8, 4);

       
        //Topes plataforma 1 

        if (objects[3].body->GetPosition().y >= 6.1f) 
        {
            objects[3].body->SetLinearVelocity({ 0, 0 });
            player.body->SetAngularDamping(0);
        }
        else if (objects[3].body->GetPosition().y <= 0)
        {
            objects[3].body->SetLinearVelocity({ 0, 0 });
        }
        //Comprobacion para que vuelva a su posicion inicial si se reinicia la ecena y se esta moviendo
        else if (player.body->GetPosition().x < objects[3].body->GetPosition().x)
        {
            objects[3].body->SetLinearVelocity({ 0, -1 });
        }
        //Tope plataforma 2 
        if (objects[4].body->GetPosition().y >= 7)
        {
            objects[4].body->SetLinearVelocity({ 0, 0 });
            player.body->SetAngularDamping(0);
        }
        else if (objects[4].body->GetPosition().y <= 4.5f)
        {
            objects[4].body->SetLinearVelocity({ 0, 0 });
  
        }
        //Comprobacion para que vuelva a su posicion inicial si se reinicia la ecena y se esta moviendo
        else if (player.body->GetPosition().x < objects[4].body->GetPosition().x)
        {
            objects[4].body->SetLinearVelocity({ 0, -1 });
        }

        //Colision con las particulas del fuego
        if (player.body->GetPosition().x > 9 && player.body->GetPosition().x < 10 && player.body->GetPosition().y < 1)
        {
            //Si no se llama dos veces se pierde el control del input
            reset(physics_world, &golf, s);
            reset(physics_world, &golf, s);

            //Destruimos y creamos un nuevo player
            physics_world.DestroyBody(player.body);
            player = crear_player(physics_world);
        }
       

        //Llamos al update de las particulas
        m_VFXManager->Update(seconds(delta_time));

       

        ///////////////////////// Render//////////////////////////////

        window.clear ();
        //renderizamos las particulas
        m_VFXManager->Draw(&window);
        //renderizamos los elementos de la escena
        render (physics_world, window, physics_to_graphics_scale);
       
    
        window.display ();

        // Si resulta necesario se detiene la ejecución unos instantes para no exceder la tasa de
        // fotogramas por segundo deseada:

        float elapsed = timer.getElapsedTime ().asSeconds ();

        if (elapsed < target_time)
        {
            sleep (seconds (target_time - elapsed));
        }

        // Se restablece la estimación de la duración del siguiente fotograma:

        delta_time = timer.getElapsedTime ().asSeconds ();
    }
    while (not exit);

    return 0;
}

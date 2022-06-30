#pragma once

#include <ciso646>
#include <memory>
#include <vector>
#include <Box2D/Box2D.h>
#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>



using namespace sf;
using namespace std;




class Joint_Object
{
	b2Body* body_a;
	b2Body* body_b;

public:
	b2RevoluteJoint* joint_body;

public: 

	Joint_Object(b2World& physics_world, b2Body* body_a, b2Body* body_b,  b2Vec2 vec_a, b2Vec2 vec_b,bool collision);

	void setLimits(float min_limit, float max_limit)
	{
		if (joint_body != NULL)
		{
			joint_body->EnableLimit(true);
			joint_body->SetLimits(min_limit * b2_pi, max_limit * b2_pi);
		}
		
	};

	void setMotor(float maxTorque, float speed)
	{
		if (joint_body != NULL)
		{
			joint_body->EnableMotor(true);
			joint_body->SetMaxMotorTorque(maxTorque);
			joint_body->SetMotorSpeed(speed);
		}
		
	};

};


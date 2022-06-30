#include "Joint_Object.h"

Joint_Object::Joint_Object(b2World& physics_world, b2Body* body_a, b2Body* body_b, b2Vec2 vec_a, b2Vec2 vec_b, bool collision)
{

    //Crea la definicion de un revolute joint y se configura
    b2RevoluteJointDef joint_def;
    joint_def.bodyA = body_a;
    joint_def.bodyB = body_b;
    joint_def.enableMotor = false;
    joint_def.collideConnected = collision;
    joint_def.localAnchorA = body_a->GetLocalCenter() + vec_a;// b2Vec2(0.15, 0.25);
    joint_def.localAnchorB = body_b->GetLocalCenter() - vec_b;// b2Vec2(-0.05, 0.25);
    joint_def.userData.pointer = 1;

    //Crea un revolute joint
    joint_body = dynamic_cast<b2RevoluteJoint*>(physics_world.CreateJoint(&joint_def));

}


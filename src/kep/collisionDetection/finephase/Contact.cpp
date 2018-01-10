#include "Contact.h"
using namespace kep;

Contact::Contact()
{
}

Contact::~Contact()
{
}

void Contact::calculateContactBasis()
{
      Vector3 contactTangent[2];

    // Check whether the Z-axis is nearer to the X or Y axis
    if (real_abs(normal.x) > real_abs(normal.y))
    {
        // Scaling factor to ensure the results are normalised
        const real s = (real)1.0f/real_sqrt(normal.z*normal.z +
            normal.x*normal.x);

        // The new X-axis is at right angles to the world Y-axis
        contactTangent[0].x = normal.z*s;
        contactTangent[0].y = 0;
        contactTangent[0].z = -normal.x*s;

        // The new Y-axis is at right angles to the new X- and Z- axes
        contactTangent[1].x = normal.y*contactTangent[0].x;
        contactTangent[1].y = normal.z*contactTangent[0].x - normal.x*contactTangent[0].z;
        contactTangent[1].z = -normal.y*contactTangent[0].x;
    }
    else
    {
        // Scaling factor to ensure the results are normalised
        const real s = (real)1.0/real_sqrt(normal.z*normal.z +
            normal.y*normal.y);

        // The new X-axis is at right angles to the world X-axis
        contactTangent[0].x = 0;
        contactTangent[0].y = -normal.z*s;
        contactTangent[0].z = normal.y*s;

        // The new Y-axis is at right angles to the new X- and Z- axes
        contactTangent[1].x = normal.y*contactTangent[0].z -
            normal.z*contactTangent[0].y;
        contactTangent[1].y = -normal.x*contactTangent[0].z;
        contactTangent[1].z = normal.x*contactTangent[0].y;
    }

    // Make a matrix from the three vectors.
    contactToWorld.setComponents(normal, contactTangent[0],contactTangent[1]);
}
void Contact::resolve(real _duration)
{
    calculateContactBasis();
    Vector3 relativeContactPosition[2] = {position - *body[0]->position, position - *body[1]->position};
    // Build a vector that shows the change in velocity in
    // world space for a unit impulse in the direction of the contact
    // normal.
    Vector3 velocity;
    ///////////////////////////////////////////
    //IMPULSE resolution///////////////////////AKA micro collision that directy works with velocity instead of force
    ///////////////////////////////////////////
    real deltaVelocity = 0;
    real velocityFromAcc = 0;//for resting contatcs

    if(body[0]->hasFiniteMass())
    {
        Vector3 deltaVelWorld = relativeContactPosition[0] % normal;
        deltaVelWorld = body[0]->inverseInertiaTensorWorld * deltaVelWorld;
        deltaVelWorld = deltaVelWorld % relativeContactPosition[0];
        deltaVelocity += deltaVelWorld * normal; //<-------------------------------------cheated will change when working with friction
        deltaVelocity += body[0]->inverseMass;
        
        ///////////////////////////
        velocity += body[0]->angularVelocity % relativeContactPosition[0];
        velocity += body[0]->velocity;
        ///////////////////////////
        velocityFromAcc += body[0]->lastFrameAcceleration * _duration * normal;//for resting contacts
        
    }
    if (body[1]->hasFiniteMass())
    {
        Vector3 deltaVelWorld = relativeContactPosition[1] % normal;
        deltaVelWorld = body[1]->inverseInertiaTensorWorld * deltaVelWorld;
        deltaVelWorld = deltaVelWorld % relativeContactPosition[1];
        deltaVelocity += deltaVelWorld * normal;
        deltaVelocity += body[1]->inverseMass;
        
        //////////////////////////
        velocity += body[1]->angularVelocity % relativeContactPosition[1];
        velocity += body[1]->velocity;
        ///////////////////////////
        velocityFromAcc -= body[1]->lastFrameAcceleration * _duration * normal;
    }
    //printf("%f\n", deltaVelocity);
    Vector3 contactVelocity = contactToWorld.transpose() * velocity;
    //contactVelocity.dump();
    const real restitution = 0.2f;//TODO: each contact should have its own  //HARDCODED
    const real velocityLimit = 0.25f;                                        //HARDCODED
    real modRestitution = restitution;
    if (real_abs(contactVelocity.x) < velocityLimit)
    {
        modRestitution = 0.0f;
    }
    
    real desiredDeltaVelocity = -contactVelocity.x - modRestitution * (contactVelocity.x - velocityFromAcc);

    //float desiredDeltaVelocity = -contactVelocity.x * (1 + restitution);
    
    //////////////////////////////////////////////////////////
    //FRICTION handling
    //////////////////////////////////////////////////////////
//     const real friction = -0.1f;
// 
     Vector3 impulseContact;
// 
//     
//     // The equivalent of a cross product in matrices is multiplication
//     // by a skew-symmetric matrix - we build the matrix for converting
//     // between linear and angular quantities.
//     real inverseMass = 0.0f;
//     Matrix3 deltaVelWorld;
//     
//     if (body[0]->hasFiniteMass())
//     {
//         inverseMass += body[0]->inverseMass;
//         Matrix3 impulseToTorque;
//         impulseToTorque.setSkewSymmetric(relativeContactPosition[0]);
//         // Build the matrix to convert contact impulse to change in velocity
//         // in world coordinates.
//         deltaVelWorld = impulseToTorque;
//         deltaVelWorld *= body[0]->inverseInertiaTensorWorld;
//         deltaVelWorld *= impulseToTorque;
//         deltaVelWorld *= -1;
//     }
//     // Check whether we need to add body 2’s data.
//     if (body[1]->hasFiniteMass())
//     {
//         // Add to the inverse mass.
//         inverseMass += body[1]->inverseMass;
//         // Set the cross product matrix.
//         Matrix3 impulseToTorque;
//         impulseToTorque.setSkewSymmetric(relativeContactPosition[1]);
//         // Calculate the velocity change matrix.
//         Matrix3 deltaVelWorld2 = impulseToTorque;
//         deltaVelWorld2 *= body[1]->inverseInertiaTensorWorld;
//         deltaVelWorld2 *= impulseToTorque;
//         deltaVelWorld2 *= -1;
//         // Add to the total delta velocity.
//         deltaVelWorld += deltaVelWorld2;
//     }
// 
//     // Do a change of basis to convert into contact coordinates.
//     Matrix3 deltaVelocityM = contactToWorld.transpose();
//     deltaVelocityM *= deltaVelWorld;
//     deltaVelocityM *= contactToWorld;
//     // Add in the linear velocity change.
//     deltaVelocityM.data[0] += inverseMass;
//     deltaVelocityM.data[4] += inverseMass;
//     deltaVelocityM.data[8] += inverseMass;
//     // Invert to get the impulse needed per unit velocity.
//     Matrix3 impulseMatrix = deltaVelocityM.inverse();
//     // Find the target velocities to kill.
//     Vector3 velKill(desiredDeltaVelocity,
//                     -contactVelocity.y,
//                     -contactVelocity.z);
//     // Find the impulse to kill target velocities.
//     impulseContact = impulseMatrix * velKill;
//     // Check for exceeding friction.
//     real planarImpulse = real_sqrt(impulseContact.y*impulseContact.y + impulseContact.z*impulseContact.z);
//     if (planarImpulse > impulseContact.x * friction)
//     {
//         // We need to use dynamic friction.
//         impulseContact.y /= planarImpulse;
//         impulseContact.z /= planarImpulse;
//         impulseContact.x = deltaVelocityM.data[0] + deltaVelocityM.data[1] * friction * impulseContact.y + deltaVelocityM.data[2] * friction * impulseContact.z;
//         impulseContact.x = desiredDeltaVelocity / impulseContact.x;
//         impulseContact.y *= friction * impulseContact.x;
//         impulseContact.z *= friction * impulseContact.x;
//      }

    /////////////////////////////////////////////////////////////
    
    
    impulseContact.x = desiredDeltaVelocity / deltaVelocity; //enough for contatcts without friction
    impulseContact.y = 0;
    impulseContact.z = 0;
    Vector3 impulse = contactToWorld * impulseContact;
    
    if(body[0]->hasFiniteMass())
    {
        Vector3 velocityChange = impulse * body[0]->inverseMass;
        Vector3 impulsiveTorque = impulse % relativeContactPosition[0];
        Vector3 rotationChange = body[0]->inverseInertiaTensor * impulsiveTorque;
        body[0]->velocity += velocityChange;
        body[0]->angularVelocity += rotationChange;
    }
    impulse *= -1;
    if(body[1]->hasFiniteMass())
    {
        Vector3 velocityChange = impulse * body[1]->inverseMass;
        Vector3 impulsiveTorque = impulse % relativeContactPosition[1];
        Vector3 rotationChange = body[1]->inverseInertiaTensor * impulsiveTorque;
        body[1]->velocity += velocityChange;
        body[1]->angularVelocity += rotationChange;
    }
    
    ////////////////////////////////////////////////////////////
    //PENETRATION resolution////////////////////////////////////AKA moves the collider back the place where the contact actually occured
    ////////////////////////////////////////////////////////////
    float linearInertia[2] = {0.0f, 0.0f};
    float angularInertia[2] = {0.0f, 0.0f};
    float totalInertia = 0.0f;
    
    float linearMove[2] = {0.0f, 0.0f};
    float angularMove[2] = {0.0f, 0.0f};
    for (unsigned i = 0; i < 2; i++)
    {
        if (body[i]->hasFiniteMass())
        {
            Matrix3 inverseInertiaTensor;
            Vector3 angularInertiaWorld = relativeContactPosition[i] % normal;
            angularInertiaWorld = body[i]->inverseInertiaTensorWorld * angularInertiaWorld;
            angularInertiaWorld = angularInertiaWorld % relativeContactPosition[i];
            angularInertia[i] = angularInertiaWorld * normal;
            linearInertia[i] = body[i]->inverseMass;
            totalInertia += linearInertia[i] + angularInertia[i];
        }
    }
    real inverseInertia = 1 / totalInertia;
    
    for (unsigned i = 0; i < 2; i++)
    {
        if (body[i]->hasFiniteMass())
        {
            linearMove[i] = penetration * linearInertia[i] * inverseInertia;
            angularMove[i] = penetration * angularInertia[i] * inverseInertia;
            *body[i]->position += normal * linearMove[i];//<------------------------------------- teleport yours truly to where collision happened
            
            Vector3 impulsiveTorque = relativeContactPosition[i] % normal;
            Vector3 impulsePerMove = body[i]->inverseInertiaTensorWorld * impulsiveTorque;
//             if(angularInertia[i] != 0.0f && (angularInertia[i] > 0.0001f || angularInertia[i] < -0.0001f))//actually not need as we only have sphere colliders, might cause jerky effects
//             {
//                 //printf("%f\n", angularInertia[i]);
//                 Vector3 rotationPerMove = impulsePerMove * (1.0f/angularInertia[i]);
//                 Vector3 rotation = rotationPerMove * angularMove[i];
//                 body[i]->orientation->rotateByVector(rotation);//-------------------------------------rotate to where collision happened
//             }
            
            
        }
    }
    

    
    //linearMove[1] = -penetration * linearInertia[1] * inverseInertia;
    
    //angularMove[1] = -penetration * angularInertia[1] * inverseInertia;
    
    
    
    //*body[1]->position += normal * linearMove[1];



    



}


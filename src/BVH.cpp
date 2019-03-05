#include "BVH.h"

#include <cmath>

BoundingSphere::BoundingSphere(const glm::vec3 &centre, double radius)
{
    BoundingSphere::centre = centre;
    BoundingSphere::radius = radius;
}

BoundingSphere::BoundingSphere(const BoundingSphere &one,
                               const BoundingSphere &two)
{
    glm::vec3 centreOffset = two.centre - one.centre;
    double distance = centreOffset.x * centreOffset.x + centreOffset.y * centreOffset.y + centreOffset.z * centreOffset.z;
    double radiusDiff = two.radius - one.radius;

    // Check if the larger sphere encloses the small one
    if(distance <= radiusDiff * radiusDiff)
    {
        if (one.radius > two.radius)
        {
            centre = one.centre;
            radius = one.radius;
        }
        else
        {
            centre = two.centre;
            radius = two.radius;
        }
    }
    else
    {
        distance = sqrt(distance);
        radius = (distance + one.radius + two.radius) * ((double)0.5);


        centre = one.centre;
        if (distance > 0)
        {
            centre += (float)((radius - one.radius) / distance) * centreOffset;
        }
    }

}

bool BoundingSphere::overlaps(const BoundingSphere &other) const
{
    glm::vec3 centreOffset = centre - other.centre;
    double distance = centreOffset.x * centreOffset.x + centreOffset.y * centreOffset.y + centreOffset.z * centreOffset.z;

    return distance < (radius + other.radius) * (radius + other.radius);
}

double BoundingSphere::getGrowth(const BoundingSphere &other) const
{
    BoundingSphere newSphere(*this, other);

    return newSphere.radius * newSphere.radius - radius * radius;
}

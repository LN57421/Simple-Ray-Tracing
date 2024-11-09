//============================================================
// STUDENT NAME: Liu Jiabao
// NUS User ID.: e1353839
// COMMENTS TO GRADER:
//
// ============================================================

#include <cmath>
#include "Sphere.h"

using namespace std;

bool Sphere::hit(const Ray &r, double tmin, double tmax,
                 SurfaceHitRecord &rec) const {
    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    //***********************************************

    // Vector oc: the vector from the ray origin to the center of the sphere
    Vector3d oc = r.origin() - center;

    // Compute the coefficients for the quadratic equation
    double a = dot(r.direction(), r.direction());  // A = d · d
    double b = 2.0 * dot(oc, r.direction());       // B = 2 * (oc · d)
    double c = dot(oc, oc) - radius * radius;      // C = (oc · oc) - r^2

    // Calculate the discriminant to check for intersection
    double discriminant = b * b - 4 * a * c;

    // If the discriminant is positive, the ray intersects the sphere
    if (discriminant > 0) {
        double sqrtD = sqrt(discriminant);

        // Calculate the first root (smallest t)
        double t = (-b - sqrtD) / (2.0 * a);
        if (t < tmax && t > tmin) {  // Check if t is within the valid range
            rec.t = t;
            rec.p = r.pointAtParam(t);  // Intersection point
            rec.normal =
                (rec.p - center) / radius;  // Normal at the intersection point
            rec.material = material;        // Assign material
            return true;
        }

        // Calculate the second root (if the first is out of range)
        t = (-b + sqrtD) / (2.0 * a);
        if (t < tmax && t > tmin) {  // Check if t is within the valid range
            rec.t = t;
            rec.p = r.pointAtParam(t);  // Intersection point
            rec.normal =
                (rec.p - center) / radius;  // Normal at the intersection point
            rec.material = material;        // Assign material
            return true;
        }
    }

    return false;  // No valid intersection found
}

bool Sphere::shadowHit(const Ray &r, double tmin, double tmax) const {
    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    //***********************************************

    // Vector oc: the vector from the ray origin to the center of the sphere
    Vector3d oc = r.origin() - center;

    // Compute the coefficients for the quadratic equation
    double a = dot(r.direction(), r.direction());
    double b = 2.0 * dot(oc, r.direction());
    double c = dot(oc, oc) - radius * radius;

    // Calculate the discriminant to check for intersection
    double discriminant = b * b - 4 * a * c;

    // If the discriminant is positive, the ray intersects the sphere
    if (discriminant > 0) {
        double sqrtD = sqrt(discriminant);

        // Calculate the first root (smallest t)
        double t = (-b - sqrtD) / (2.0 * a);
        if (t < tmax && t > tmin) {  // Check if t is within the valid range
            return true;             // Early exit if the shadow ray is blocked
        }

        // Calculate the second root (if the first is out of range)
        t = (-b + sqrtD) / (2.0 * a);
        if (t < tmax && t > tmin) {  // Check if t is within the valid range
            return true;             // Early exit if the shadow ray is blocked
        }
    }

    return false;  // No valid intersection found, no shadow
}

//============================================================
// STUDENT NAME:Liu Jiabao
// NUS User ID.: e1353839
// COMMENTS TO GRADER:
//
// ============================================================

#include "Raytrace.h"

#include <cfloat>
#include <cmath>

#include "Color.h"
#include "Light.h"
#include "Material.h"
#include "Ray.h"
#include "Scene.h"
#include "Surface.h"
#include "Vector3d.h"

// This is for avoiding the "epsilon problem" or the shadow acne problem.
static constexpr double DEFAULT_TMIN = 10e-6;

// Use this for tmax for non-shadow ray intersection test.
static constexpr double DEFAULT_TMAX = DBL_MAX;

//////////////////////////////////////////////////////////////////////////////
// Compute the outgoing mirror reflection vector.
// Input incoming vector L is pointing AWAY from surface point.
// Assume normal vector N is unit vector.
// The output reflection vector is pointing AWAY from surface point, and
// has same length as incoming vector L.
//////////////////////////////////////////////////////////////////////////////

static Vector3d mirrorReflect(const Vector3d &L, const Vector3d &N) {
    return (2.0 * dot(N, L)) * N - L;
}

//////////////////////////////////////////////////////////////////////////////
// Compute I_source * [ k_d * (N.L) + k_r * (R.V)^n ].
// Input vectors L, N and V are pointing AWAY from surface point.
// Assume all vector L, N and V are unit vectors.
//////////////////////////////////////////////////////////////////////////////

static Color computePhongLighting(const Vector3d &L, const Vector3d &N,
                                  const Vector3d &V, const Material &mat,
                                  const PointLightSource &ptLight) {
    Vector3d R = mirrorReflect(L, N);

    auto N_dot_L = (float)dot(N, L);
    if (N_dot_L < 0.0f)
        N_dot_L = 0.0f;

    auto R_dot_V = (float)dot(R, V);
    if (R_dot_V < 0.0f)
        R_dot_V = 0.0f;
    float R_dot_V_pow_n = powf(R_dot_V, (float)mat.n);

    return ptLight.I_source * (mat.k_d * N_dot_L + mat.k_r * R_dot_V_pow_n);
}

//////////////////////////////////////////////////////////////////////////////
// Traces a ray into the scene.
// reflectLevels: specifies number of levels of reflections (0 for no
// reflection). hasShadow: specifies whether to generate shadows.
//////////////////////////////////////////////////////////////////////////////

Color Raytrace::TraceRay(const Ray &ray, const Scene &scene, int reflectLevels,
                         bool hasShadow) {
    Ray uRay(ray);
    uRay.makeUnitDirection();  // Normalize ray direction.

    // Find whether and where the ray hits some surface.
    // Take the nearest hit point.

    bool hasHitSomething = false;
    double nearest_t = DEFAULT_TMAX;
    SurfaceHitRecord nearestHitRec;

    for (const auto &surface : scene.surfaces) {
        SurfaceHitRecord tempHitRec;
        bool hasHit = surface->hit(uRay, DEFAULT_TMIN, DEFAULT_TMAX, tempHitRec);

        if (hasHit && tempHitRec.t < nearest_t) {
            hasHitSomething = true;
            nearest_t = tempHitRec.t;
            nearestHitRec = tempHitRec;
        }
    }

    if (!hasHitSomething)
        return scene.backgroundColor;

    nearestHitRec.normal.makeUnitVector();
    Vector3d N = nearestHitRec.normal;  // Unit vector.
    Vector3d V = -uRay.direction();     // Unit vector.

    Color result(0.0f, 0.0f, 0.0f);  // The result will be accumulated here.

    //**********************************
    // result = nearestHitRec.material.k_d;  // REMOVE THIS LINE AFTER YOU HAVE
    // FINISHED CODE BELOW.
    //**********************************

    // Add to result the Phong lighting contributed by each point light source.
    // Compute for shadow if hasShadow is true.

    // Local illumination calculation
    for (const auto &light : scene.ptLights) {
        Vector3d L = (light.position - nearestHitRec.p)
                         .unitVector();  // Unit vector from intersection point
                                         // to light source

        // Check for shadow
        bool inShadow = false;
        if (hasShadow) {
            Vector3d shadowOrigin =
                nearestHitRec.p + N * DEFAULT_TMIN;  // Add a small offset
            Ray shadowRay(shadowOrigin, L);

            for (const auto &surface : scene.surfaces) {
                if (surface->shadowHit(
                        shadowRay, DEFAULT_TMIN,
                        (light.position - nearestHitRec.p).length())) {
                    inShadow = true;
                    break;
                }
            }
        }

        // If not in shadow, calculate the Phong lighting contribution from the
        // light source
        if (!inShadow) {
            result +=
                computePhongLighting(L, N, V, nearestHitRec.material, light);
        }
    }

    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    //***********************************************

    // Add to result the global ambient lighting.

    // Add global ambient lighting
    result += scene.amLight.I_a * nearestHitRec.material.k_a;

    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    //***********************************************

    // Add to result the reflection of the scene.

    // Recursively calculate reflected light
    if (reflectLevels > 0 &&
        nearestHitRec.material.k_rg != Color(0.0f, 0.0f, 0.0f)) {
        Vector3d R = mirrorReflect(V, N);  // Calculate reflection direction
        Vector3d reflectedOrigin =
            nearestHitRec.p + N * DEFAULT_TMIN;  // Add offset
        Ray reflectedRay(reflectedOrigin, R);

        // Recursive call to TraceRay to calculate the contribution of the
        // reflected light
        result += nearestHitRec.material.k_rg *
                  TraceRay(reflectedRay, scene, reflectLevels - 1, hasShadow);
    }

    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    //***********************************************

    return result;
}

/**
//////////////////////////////////////////////////////////////////////////////
// Compute Blinn-Phong lighting model.
// Computes I_source * [ k_d * (N.L) + k_r * (N.H)^n ].
// Here, N is the normal, L is the light direction, V is the view direction,
// and H is the half-vector between L and V.
//////////////////////////////////////////////////////////////////////////////

static Color computeBlinnPhongLighting(const Vector3d &L, const Vector3d &N,
                                       const Vector3d &V, const Material &mat,
                                       const PointLightSource &ptLight) {
    // Calculate half-vector H, which is the normalized vector halfway between L
    // and V
    Vector3d H = (L + V).unitVector();

    // Compute N.L for diffuse component
    auto N_dot_L = (float)dot(N, L);
    if (N_dot_L < 0.0f)
        N_dot_L = 0.0f;

    // Compute N.H for specular component
    auto N_dot_H = (float)dot(N, H);
    if (N_dot_H < 0.0f)
        N_dot_H = 0.0f;
    float N_dot_H_pow_n = powf(N_dot_H, (float)mat.n);

    // Return combined diffuse and specular components scaled by light intensity
    return ptLight.I_source * (mat.k_d * N_dot_L + mat.k_r * N_dot_H_pow_n);
}



// Compute the outgoing mirror reflection vector.
static bool refract(const Vector3d &I, const Vector3d &N, double eta, Vector3d &refracted) {
    double cos_i = -dot(N, I);
    double sin2_t = eta * eta * (1 - cos_i * cos_i);
    if (sin2_t > 1.0) return false;  // Total internal reflection
    double cos_t = sqrt(1.0 - sin2_t);
    refracted = eta * I + (eta * cos_i - cos_t) * N;
    return true;
}

// Compute the refracted ray.
if (nearestHitRec.material.k_t != Color(0.0f, 0.0f, 0.0f)) { 
    Vector3d refractedDirection;
    double eta = insideObject ? (material.eta / 1.0) : (1.0 / material.eta);

    if (refract(V, N, eta, refractedDirection)) {
        Ray refractedRay(nearestHitRec.p + refractedDirection * DEFAULT_TMIN, refractedDirection);
        result += nearestHitRec.material.k_t * TraceRay(refractedRay, scene, reflectLevels - 1, hasShadow);
    }
}


 */

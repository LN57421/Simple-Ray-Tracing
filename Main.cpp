//============================================================
// STUDENT NAME:Liu Jiabao
// NUS User ID.:e1353839
// COMMENTS TO GRADER:
//
// ============================================================

#include <string>

#include "Camera.h"
#include "Color.h"
#include "Image.h"
#include "Light.h"
#include "Material.h"
#include "Plane.h"
#include "Ray.h"
#include "Raytrace.h"
#include "Scene.h"
#include "Sphere.h"
#include "Surface.h"
#include "Triangle.h"
#include "Util.h"
#include "Vector3d.h"

// Constants for Scene 1.
static constexpr int imageWidth1 = 640;
static constexpr int imageHeight1 = 480;
static constexpr int reflectLevels1 = 2;  // 0 -- object does not reflect scene.
static constexpr int hasShadow1 = true;
const static constexpr std::string_view outImageFile1 = "img_spheres.png";

// Constants for Scene 2.
static constexpr int imageWidth2 = 640;
static constexpr int imageHeight2 = 480;
static constexpr int reflectLevels2 = 2;  // 0 -- object does not reflect scene.
static constexpr int hasShadow2 = true;
static constexpr std::string_view outImageFile2 = "img_scene2.png";

///////////////////////////////////////////////////////////////////////////
// Raytrace the whole image of the scene and write it to a file.
///////////////////////////////////////////////////////////////////////////

void RenderImage(const std::string &imageFilename, const Scene &scene,
                 int reflectLevels, bool hasShadow) {
    int imgWidth = scene.camera.getImageWidth();
    int imgHeight = scene.camera.getImageHeight();

    Image image(imgWidth, imgHeight);  // To store the result of ray tracing.

    double startTime = Util::GetCurrRealTime();
    double startCPUTime = Util::GetCurrCPUTime();

// Generate image, rendering in parallel on Windows and Linux.
#ifndef __APPLE__
#pragma warning(push)
#pragma warning(disable : 6993)
#pragma omp parallel for
#endif
    for (int y = 0; y < imgHeight; y++) {
        double pixelPosY = y + 0.5;

        for (int x = 0; x < imgWidth; x++) {
            double pixelPosX = x + 0.5;
            Ray ray = scene.camera.getRay(pixelPosX, pixelPosY);
            Color pixelColor =
                Raytrace::TraceRay(ray, scene, reflectLevels, hasShadow);
            pixelColor.clamp();
            image.setPixel(x, y, pixelColor);
        }
    }
#ifndef __APPLE__
#pragma warning(pop)
#endif

    double cpuTimeElapsed = Util::GetCurrCPUTime() - startCPUTime;
    double realTimeElapsed = Util::GetCurrRealTime() - startTime;
    std::cout << "CPU time taken = " << cpuTimeElapsed << "sec" << std::endl;
    std::cout << "Real time taken = " << realTimeElapsed << "sec" << std::endl;

    // Write image to file.
    if (!image.writeToFile(imageFilename))
        return;
    else
        Util::ErrorExit("File: %s could not be written.\n",
                        imageFilename.c_str());
}

// Forward declarations. These functions are defined later in the file.
void DefineScene1(Scene &scene, int imageWidth, int imageHeight);
void DefineScene2(Scene &scene, int imageWidth, int imageHeight);

void WaitForEnterKeyBeforeExit(void) {
    fflush(stdin);
    getchar();
}

// task 1 2 3
// the result of task1 had changed because of the change of task2
int main() {
    // task 1
    // Define Scene 1.
    // Scene scene1;
    // DefineScene1(scene1, imageWidth1, imageHeight1);

    // // Render Scene 1.

    // std::cout << "Render Scene 1..." << std::endl;
    // RenderImage(std::string(outImageFile1), scene1, reflectLevels1,
    // hasShadow1); std::cout << "Scene 1 completed." << std::endl;

    // // Delete Scene 1 surfaces.

    // for (auto &surface : scene1.surfaces) {
    //     delete surface;
    // }

    // task 2
    // Define Scene1
    Scene scene1;
    DefineScene1(scene1, imageWidth1, imageHeight1);

    // Render Scene 1.
    std::cout << "Generating images with different reflection levels and "
                 "shadow settings..."
              << std::endl;

    // reflectLevels = 0, hasShadow = false
    RenderImage("img_r0.png", scene1, 0, false);

    // reflectLevels = 0, hasShadow = true
    RenderImage("img_r0s.png", scene1, 0, true);

    // reflectLevels = 1, hasShadow = false
    RenderImage("img_r1.png", scene1, 1, false);

    // reflectLevels = 1, hasShadow = true
    RenderImage("img_r1s.png", scene1, 1, true);

    // reflectLevels = 2, hasShadow = false
    RenderImage("img_r2.png", scene1, 2, false);

    // reflectLevels = 2, hasShadow = true
    RenderImage("img_r2s.png", scene1, 2, true);

    std::cout << "Image generation completed." << std::endl;

    // Delete Scene 1 surfaces.
    for (auto &surface : scene1.surfaces) {
        delete surface;
    }

    // task 3
    // Define Scene 2.

    Scene scene2;
    DefineScene2(scene2, imageWidth2, imageHeight2);

    // Render Scene 2.

    std::cout << "Render Scene 2..." << std::endl;
    RenderImage(std::string(outImageFile2), scene2, reflectLevels2, hasShadow2);
    std::cout << "Scene 2 completed." << std::endl;

    // Delete Scene 2 surfaces.

    for (auto &surface : scene2.surfaces) {
        delete surface;
    }

    std::cout << "All done. Press Enter to exit." << std::endl;
    std::cin.get();
    return 0;
}

///////////////////////////////////////////////////////////////////////////
// Modelling of Scene 1.
///////////////////////////////////////////////////////////////////////////

void DefineScene1(Scene &scene, int imageWidth, int imageHeight) {
    scene.backgroundColor = Color(0.2f, 0.3f, 0.5f);

    scene.amLight.I_a = Color(1.0f, 1.0f, 1.0f) * 0.25f;

    // Define materials.

    // Light red.
    Material lightRed = Material();
    lightRed.k_d = Color(0.8f, 0.4f, 0.4f);
    lightRed.k_a = lightRed.k_d;
    lightRed.k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
    lightRed.k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
    lightRed.n = 64.0f;

    // Light green.
    Material lightGreen = Material();
    lightGreen.k_d = Color(0.4f, 0.8f, 0.4f);
    lightGreen.k_a = lightGreen.k_d;
    lightGreen.k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
    lightGreen.k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
    lightGreen.n = 64.0f;

    // Light blue.
    Material lightBlue = Material();
    lightBlue.k_d = Color(0.4f, 0.4f, 0.8f) * 0.9f;
    lightBlue.k_a = lightBlue.k_d;
    lightBlue.k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
    lightBlue.k_rg = Color(0.8f, 0.8f, 0.8f) / 2.5f;
    lightBlue.n = 64.0f;

    // Yellow.
    Material yellow = Material();
    yellow.k_d = Color(0.6f, 0.6f, 0.2f);
    yellow.k_a = yellow.k_d;
    yellow.k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
    yellow.k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
    yellow.n = 64.0f;

    // Gray.
    Material gray = Material();
    gray.k_d = Color(0.6f, 0.6f, 0.6f);
    gray.k_a = gray.k_d;
    gray.k_r = Color(0.6f, 0.6f, 0.6f);
    gray.k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
    gray.n = 128.0f;

    // Insert into scene materials vector.
    scene.materials = {lightRed, lightGreen, lightBlue, yellow, gray};

    // Define point light sources.

    scene.ptLights.resize(2);

    PointLightSource light0 = {Vector3d(100.0, 120.0, 10.0),
                               Color(1.0f, 1.0f, 1.0f) * 0.6f};
    PointLightSource light1 = {Vector3d(5.0, 80.0, 60.0),
                               Color(1.0f, 1.0f, 1.0f) * 0.6f};

    scene.ptLights = {light0, light1};

    // Define surface primitives.

    scene.surfaces.resize(15);

    auto horzPlane =
        new Plane(0.0, 1.0, 0.0, 0.0, scene.materials[2]);  // Horizontal plane.
    auto leftVertPlane = new Plane(1.0, 0.0, 0.0, 0.0,
                                   scene.materials[4]);  // Left vertical plane.
    auto rightVertPlane = new Plane(
        0.0, 0.0, 1.0, 0.0, scene.materials[4]);  // Right vertical plane.
    auto bigSphere = new Sphere(Vector3d(40.0, 20.0, 42.0), 22.0,
                                scene.materials[0]);  // Big sphere.
    auto smallSphere = new Sphere(Vector3d(75.0, 10.0, 40.0), 12.0,
                                  scene.materials[1]);  // Small sphere.

    // Cube +y face.
    auto cubePosYTri1 =
        new Triangle(Vector3d(50.0, 20.0, 90.0), Vector3d(50.0, 20.0, 70.0),
                     Vector3d(30.0, 20.0, 70.0), scene.materials[3]);
    auto cubePosYTri2 =
        new Triangle(Vector3d(50.0, 20.0, 90.0), Vector3d(30.0, 20.0, 70.0),
                     Vector3d(30.0, 20.0, 90.0), scene.materials[3]);

    // Cube +x face.
    auto cubePosXTri1 =
        new Triangle(Vector3d(50.0, 0.0, 70.0), Vector3d(50.0, 20.0, 70.0),
                     Vector3d(50.0, 20.0, 90.0), scene.materials[3]);
    auto cubePosXTri2 =
        new Triangle(Vector3d(50.0, 0.0, 70.0), Vector3d(50.0, 20.0, 90.0),
                     Vector3d(50.0, 0.0, 90.0), scene.materials[3]);

    // Cube -x face.
    auto cubeNegXTri1 =
        new Triangle(Vector3d(30.0, 0.0, 90.0), Vector3d(30.0, 20.0, 90.0),
                     Vector3d(30.0, 20.0, 70.0), scene.materials[3]);
    auto cubeNegXTri2 =
        new Triangle(Vector3d(30.0, 0.0, 90.0), Vector3d(30.0, 20.0, 70.0),
                     Vector3d(30.0, 0.0, 70.0), scene.materials[3]);

    // Cube +z face.
    auto cubePosZTri1 =
        new Triangle(Vector3d(50.0, 0.0, 90.0), Vector3d(50.0, 20.0, 90.0),
                     Vector3d(30.0, 20.0, 90.0), scene.materials[3]);
    auto cubePosZTri2 =
        new Triangle(Vector3d(50.0, 0.0, 90.0), Vector3d(30.0, 20.0, 90.0),
                     Vector3d(30.0, 0.0, 90.0), scene.materials[3]);

    // Cube -z face.
    auto cubeNegZTri1 =
        new Triangle(Vector3d(30.0, 0.0, 70.0), Vector3d(30.0, 20.0, 70.0),
                     Vector3d(50.0, 20.0, 70.0), scene.materials[3]);
    auto cubeNegZTri2 =
        new Triangle(Vector3d(30.0, 0.0, 70.0), Vector3d(50.0, 20.0, 70.0),
                     Vector3d(50.0, 0.0, 70.0), scene.materials[3]);

    scene.surfaces = {horzPlane,    leftVertPlane, rightVertPlane, bigSphere,
                      smallSphere,  cubePosXTri1,  cubePosXTri2,   cubePosYTri1,
                      cubePosYTri2, cubePosZTri1,  cubePosZTri2,   cubeNegXTri1,
                      cubeNegXTri2, cubeNegZTri1,  cubeNegZTri2};

    // Define camera.

    scene.camera =
        Camera(Vector3d(150.0, 120.0, 150.0),      // eye
               Vector3d(45.0, 22.0, 55.0),         // lookAt
               Vector3d(0.0, 1.0, 0.0),            // upVector
               (-1.0 * imageWidth) / imageHeight,  // left
               (1.0 * imageWidth) / imageHeight,   // right
               -1.0, 1.0, 3.0,                     // bottom, top, near
               imageWidth, imageHeight);           // image_width, image_height
}

///////////////////////////////////////////////////////////////////////////
// Modeling of Scene 2.
///////////////////////////////////////////////////////////////////////////
void DefineScene2(Scene &scene, int imageWidth, int imageHeight) {
    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    //***********************************************
    scene.backgroundColor = Color(0.3f, 0.3f, 0.3f);
    scene.amLight.I_a =
        Color(1.0f, 1.0f, 1.0f) * 0.2f;  // Reduce ambient light intensity

    // Define materials
    scene.materials.resize(6);

    // Light red.
    scene.materials[0].k_d = Color(0.8f, 0.4f, 0.4f);
    scene.materials[0].k_a = scene.materials[0].k_d;
    scene.materials[0].k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
    scene.materials[0].k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
    scene.materials[0].n = 64.0f;

    // Light blue.
    scene.materials[1].k_d = Color(0.4f, 0.4f, 0.8f) * 0.9f;
    scene.materials[1].k_a = scene.materials[1].k_d;
    scene.materials[1].k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
    scene.materials[1].k_rg = Color(0.8f, 0.8f, 0.8f) / 2.5f;
    scene.materials[1].n = 64.0f;

    // Dark Green.
    scene.materials[2].k_d = Color(0.15f, 0.4f, 0.15f);
    scene.materials[2].k_a = scene.materials[2].k_d;
    scene.materials[2].k_r = Color(0.8f, 0.8f, 0.8f);
    scene.materials[2].k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
    scene.materials[2].n = 64.0f;

    // Gray.
    scene.materials[3].k_d = Color(0.6f, 0.6f, 0.6f);
    scene.materials[3].k_a = scene.materials[3].k_d;
    scene.materials[3].k_r = Color(0.6f, 0.6f, 0.6f) / 1.5f;
    scene.materials[3].k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
    scene.materials[3].n = 128.0f;

    // Shiny yellow (reflective).
    scene.materials[4].k_d = Color(0.9f, 0.9f, 0.3f);
    scene.materials[4].k_a = scene.materials[4].k_d * 0.3f;
    scene.materials[4].k_r = Color(0.9f, 0.9f, 0.9f);
    scene.materials[4].k_rg = Color(0.9f, 0.9f, 0.9f) / 2.0f;
    scene.materials[4].n = 100.0f;

    // Transparent blue (refractive).
    // Adjust transparent blue sphere material
    scene.materials[5].k_d =
        Color(0.3f, 0.5f, 0.9f);  // Increase blue saturation
    scene.materials[5].k_a =
        scene.materials[5].k_d * 0.3f;  // Increase ambient reflection
    scene.materials[5].k_r =
        Color(0.2f, 0.2f, 0.3f);  // Reduce gloss, decrease reflection
    scene.materials[5].k_rg =
        Color(0.5f, 0.5f, 0.5f);   // Decrease transparency effect
    scene.materials[5].n = 50.0f;  // Adjust gloss sharpness

    // Define point light sources
    scene.ptLights.resize(3);

    scene.ptLights[0].I_source = Color(1.0f, 1.0f, 1.0f) * 0.5f;
    scene.ptLights[0].position = Vector3d(150.0, 120.0, 35.0);

    scene.ptLights[1].I_source = Color(1.0f, 1.0f, 1.0f) * 0.5f;
    scene.ptLights[1].position = Vector3d(35.0, 120.0, 150.0);

    // New light source at the top of the scene
    scene.ptLights[2].I_source = Color(0.7f, 0.7f, 0.7f);
    scene.ptLights[2].position = Vector3d(75.0, 200.0, 75.0);

    // Define geometric surfaces
    scene.surfaces.push_back(new Plane(
        0.0, 1.0, 0.0, 0.0, scene.materials[3]));  // Horizontal plane.
    scene.surfaces.push_back(new Plane(
        1.0, 0.0, 0.0, 0.0, scene.materials[0]));  // Left vertical plane.
    scene.surfaces.push_back(new Plane(
        0.0, 0.0, 1.0, 0.0, scene.materials[1]));  // Right vertical plane.

    // Add a reflective yellow sphere
    scene.surfaces.push_back(
        new Sphere(Vector3d(100.0, 20.0, 45.0), 20.0, scene.materials[4]));

    // Add a transparent blue sphere
    scene.surfaces.push_back(
        new Sphere(Vector3d(100.0, 20.0, 87.0), 15.0, scene.materials[5]));

    // Add a glossy red sphere
    scene.surfaces.push_back(
        new Sphere(Vector3d(40.0, 15.0, 85.0), 15.0, scene.materials[0]));

    // Pyramid faces (deep blue material)
    // Use deep blue material and add slight gloss to the pyramid
    scene.materials[2].k_d =
        Color(0.1f, 0.1f, 0.3f);  // Deep blue diffuse reflection
    scene.materials[2].k_a =
        scene.materials[2].k_d * 0.3f;  // Appropriate ambient reflection
    scene.materials[2].k_r = Color(0.2f, 0.2f, 0.5f);  // Add slight gloss
    scene.materials[2].n = 50.0f;                      // Set gloss sharpness

    // Define the pyramid peak
    Vector3d pyramidPeak = Vector3d(
        32.25, 30.0, 32.25);  // Increase peak height from 25.75 to 30.0

    // Define the four faces of the pyramid
    scene.surfaces.push_back(
        new Triangle(Vector3d(15.0, 0.0, 15.0), pyramidPeak,
                     Vector3d(50.0, 0.0, 15.0), scene.materials[2]));
    scene.surfaces.push_back(
        new Triangle(Vector3d(50.0, 0.0, 15.0), pyramidPeak,
                     Vector3d(50.0, 0.0, 50.0), scene.materials[2]));
    scene.surfaces.push_back(
        new Triangle(Vector3d(50.0, 0.0, 50.0), pyramidPeak,
                     Vector3d(15.0, 0.0, 50.0), scene.materials[2]));
    scene.surfaces.push_back(
        new Triangle(Vector3d(15.0, 0.0, 50.0), pyramidPeak,
                     Vector3d(15.0, 0.0, 15.0), scene.materials[2]));

    // Define the camera
    scene.camera = Camera(
        Vector3d(170.0, 120.0, 160.0),  // Move camera position to the right
        Vector3d(75.0, 27.5, 60.0),     // Adjust viewing direction
        Vector3d(0.0, 1.0, 0.0), (-1.0 * imageWidth) / imageHeight,
        (1.0 * imageWidth) / imageHeight, -1.0, 1.0, 3.0, imageWidth,
        imageHeight);
}

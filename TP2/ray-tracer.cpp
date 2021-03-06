#include <qapplication.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Viewer.h"
#include "Scene.h"
#include "Sphere.h"
#include "Material.h"
#include "PointLight.h"
#include "PeriodicPlane.h"

using namespace std;
using namespace rt;

void addBubble(Scene &scene, Point3 c, Real r, Material transp_m) {
    Material revert_m = transp_m;
    std::swap(revert_m.in_refractive_index, revert_m.out_refractive_index);
    Sphere *sphere_out = new Sphere(c, r, transp_m);
    Sphere *sphere_in = new Sphere(c, r - 0.02f, revert_m);
    scene.addObject(sphere_out);
    scene.addObject(sphere_in);
}

int main(int argc, char **argv) {
    // Read command lines arguments.
    QApplication application(argc, argv);

    // Creates a 3D scene
    Scene scene;
    /// Scene 1
//  // Light at infinity
//  Light* light0 = new PointLight( GL_LIGHT0, Point4( 0,0,1,0 ),
//                                    Color( 1.0, 1.0, 1.0 ) );
//  Light* light1 = new PointLight( GL_LIGHT1, Point4( 7,5,15,1 ),
//                                  Color( 1.0, 1.0, 1.0 ) );
//  scene.addLight( light0 );
//  scene.addLight( light1 );
//
//  // Objects
//  Sphere* sphere1 = new Sphere( Point3( 0, 0, 0), 2.0, Material::bronze() );
//  Sphere* sphere2 = new Sphere( Point3( 0, 4, 0), 1.0, Material::emerald() );
//  scene.addObject( sphere1 );
//  scene.addObject( sphere2 );

/// Scene 2
// Light at infinity
    Light *light0 = new PointLight(GL_LIGHT0, Point4(0, 0, 1, 0),
                                   Color(1.0, 1.0, 1.0));
    Light *light1 = new PointLight(GL_LIGHT1, Point4(-10, -4, 2, 1),
                                   Color(1.0, 1.0, 1.0));
    scene.addLight(light0);
    scene.addLight(light1);

// Objects
    Sphere *sphere1 = new Sphere(Point3(0, 0, 0), 2.0, Material::bronze());
    Sphere *sphere2 = new Sphere(Point3(0, 4, 0), 1.0, Material::emerald());
    Sphere *sphere3 = new Sphere(Point3(6, 6, 0), 3.0, Material::mirror());
    Sphere *sphere4 = new Sphere(Point3(0, 10, 6), 3.0, Material::rubilax());

    Sphere *sphereInBuble = new Sphere(Point3(-5, 4, -2), 1.0, Material::emerald());
    addBubble(scene, Point3(-5, 4, -1), 2.0, Material::glass());

    Sphere *sphereInBuble1 = new Sphere(Point3(5, 6, 5), 1.0, Material::emerald());
    addBubble(scene, Point3(5, 6, 6), 2.0, Material::glass());

    Sphere *sphereInBuble2 = new Sphere(Point3(-3, 4, 1), 1.0, Material::emerald());
    addBubble(scene, Point3(-3, 4, 2), 2.0, Material::glass());

    scene.addObject(sphere1);
    scene.addObject(sphere2);
    scene.addObject(sphere3);
    scene.addObject(sphere4);
    scene.addObject(sphereInBuble);
    scene.addObject(sphereInBuble1);
    scene.addObject(sphereInBuble2);

    // Un sol noir et blanc
//    PeriodicPlane *pplane = new PeriodicPlane(Point3(0, 0, 0), Vector3(5, 0, 0), Vector3(0, 5, 0),
//                                              Material::whitePlastic(), Material::blackMatter(), 0.05f);
    // Un mur de building "moderne" ?? gauche.
//    PeriodicPlane *pplane = new PeriodicPlane(Point3(-10, 0, 0), Vector3(0, 2, 0), Vector3(0, 0, 4),
//                                              Material::mirror(), Material::blackMatter(), 0.025f);

//    scene.addObject(pplane);


    // Instantiate the viewer.
    Viewer viewer;
    // Give a name
    viewer.setWindowTitle("Ray-tracer preview");

    // Sets the scene
    viewer.setScene(scene);

    // Make the viewer window visible on screen.
    viewer.show();
    // Run main loop.
    application.exec();
    return 0;
}

/**
@file Scene.h
@author JOL
*/
#pragma once
#ifndef _SCENE_H_
#define _SCENE_H_

#include <cassert>
#include <vector>
#include "GraphicalObject.h"
#include "Light.h"

/// Namespace RayTracer
namespace rt
{

  /**
  Models a scene, i.e. a collection of lights and graphical objects
  (could be a tree, but we keep a list for now for the sake of
  simplicity).

  @note Once the scene receives a new object, it owns the object and
  is thus responsible for its deallocation.
  */

  struct Scene
  {
    /// The list of lights modelled as a vector.
    std::vector<Light *> myLights;
    /// The list of objects modelled as a vector.
    std::vector<GraphicalObject *> myObjects;

    /// Default constructor. Nothing to do.
    Scene() {}

    /// Destructor. Frees objects.
    ~Scene()
    {
      for (Light *light : myLights)
        delete light;
      for (GraphicalObject *obj : myObjects)
        delete obj;
      // The vector is automatically deleted.
    }

    /// This function calls the init method of each of its objects.
    void init(Viewer &viewer)
    {
      for (GraphicalObject *obj : myObjects)
        obj->init(viewer);
      for (Light *light : myLights)
        light->init(viewer);
    }
    /// This function calls the draw method of each of its objects.
    void draw(Viewer &viewer)
    {
      for (GraphicalObject *obj : myObjects)
        obj->draw(viewer);
      for (Light *light : myLights)
        light->draw(viewer);
    }
    /// This function calls the light method of each of its lights
    void light(Viewer &viewer)
    {
      for (Light *light : myLights)
        light->light(viewer);
    }

    /// Adds a new object to the scene.
    void addObject(GraphicalObject *anObject)
    {
      myObjects.push_back(anObject);
    }

    /// Adds a new light to the scene.
    void addLight(Light *aLight)
    {
      myLights.push_back(aLight);
    }

    /// returns the closest object intersected by the given ray.
    Real
    rayIntersection(const Ray &ray,
                    GraphicalObject *&object, Point3 &p)
    {

      // On fait une boucle sur tous les objets de la scène
      //     si le rayon intersecte l'objet, alors
      //         on calcule la distance entre l'origine et le point d'intersection.
      //         si cette distance est plus courte que la précédente, l'objet courant devient l'objet intersecté.
      // si le rayon n'a pas touché d'objets, on retourne un nombre positif, sinon on retourne un nombre négatif, et object et p correspondent à l'intersection la plus proche.

      double distOprecP = std::numeric_limits<double>::infinity();
      double cpt = 0;

      for (int i = 0; i < (int)myObjects.size(); i++)
      {
        if (myObjects[i]->rayIntersection(ray, p) < 0.0f)
        {
          // const double dist = sqrt( pow(p[0] - ray.origin[0], 2.0) + pow(p[1] - ray.origin[1], 2.0) + pow(p[2] - ray.origin[2], 2.0));
          const double dist = sqrt(pow((ray.origin[0] - p[0]), 2.0) + pow((ray.origin[1] - p[1]), 2.0) + pow((ray.origin[2]) - p[2], 2.0));
          if (dist < distOprecP)
          {
            object = myObjects[i];
            distOprecP = dist;
          }
          cpt++;
        }
      }
      if (cpt == 0)
      {
        return 1.0f;
      }
      return -1.0f;
    }

  private:
    /// Copy constructor is forbidden.
    Scene(const Scene &) = delete;
    /// Assigment is forbidden.
    Scene &operator=(const Scene &) = delete;
  };
} // namespace rt

#endif // #define _SCENE_H_

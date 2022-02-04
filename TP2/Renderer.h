/**
@file Renderer.h
@author JOL
*/
#pragma once
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "Color.h"
#include "Image2D.h"
#include "Ray.h"
#include "Background.h"

/// Namespace RayTracer
namespace rt {

    inline void progressBar(std::ostream &output,
                            const double currentValue, const double maximumValue) {
        static const int PROGRESSBARWIDTH = 60;
        static int myProgressBarRotation = 0;
        static int myProgressBarCurrent = 0;
        // how wide you want the progress meter to be
        double fraction = currentValue / maximumValue;

        // part of the progressmeter that's already "full"
        int dotz = static_cast<int>(floor(fraction * PROGRESSBARWIDTH));
        if (dotz > PROGRESSBARWIDTH) dotz = PROGRESSBARWIDTH;

        // if the fullness hasn't changed skip display
        if (dotz == myProgressBarCurrent) return;
        myProgressBarCurrent = dotz;
        myProgressBarRotation++;

        // create the "meter"
        int ii = 0;
        output << "[";
        // part  that's full already
        for (; ii < dotz; ii++) output << "#";
        // remaining part (spaces)
        for (; ii < PROGRESSBARWIDTH; ii++) output << " ";
        static const char *rotation_string = "|\\-/";
        myProgressBarRotation %= 4;
        output << "] " << rotation_string[myProgressBarRotation]
               << " " << (int) (fraction * 100) << "/100\r";
        output.flush();
    }

    /// This structure takes care of rendering a scene.
    struct Renderer {

        /// The scene to render
        Scene *ptrScene;
        /// The origin of the camera in space.
        Point3 myOrigin;
        /// (myOrigin, myOrigin+myDirUL) forms a ray going through the upper-left
        /// corner pixel of the viewport, i.e. pixel (0,0)
        Vector3 myDirUL;
        /// (myOrigin, myOrigin+myDirUR) forms a ray going through the upper-right
        /// corner pixel of the viewport, i.e. pixel (width,0)
        Vector3 myDirUR;
        /// (myOrigin, myOrigin+myDirLL) forms a ray going through the lower-left
        /// corner pixel of the viewport, i.e. pixel (0,height)
        Vector3 myDirLL;
        /// (myOrigin, myOrigin+myDirLR) forms a ray going through the lower-right
        /// corner pixel of the viewport, i.e. pixel (width,height)
        Vector3 myDirLR;

        int myWidth;
        int myHeight;


        /// The Background to render
        Background *ptrBackground;

        Renderer() : ptrScene(0), ptrBackground(0) {}

        Renderer(Scene &scene, Background &background) : ptrScene(&scene), ptrBackground(&background) {}

        void setScene(rt::Scene &aScene) { ptrScene = &aScene; }

        void setViewBox(Point3 origin,
                        Vector3 dirUL, Vector3 dirUR, Vector3 dirLL, Vector3 dirLR) {
            myOrigin = origin;
            myDirUL = dirUL;
            myDirUR = dirUR;
            myDirLL = dirLL;
            myDirLR = dirLR;
        }

        void setResolution(int width, int height) {
            myWidth = width;
            myHeight = height;
        }

        /// The main rendering routine
        void render(Image2D<Color> &image, int max_depth) {
            std::cout << "Rendering into image ... might take a while." << std::endl;
            image = Image2D<Color>(myWidth, myHeight);
            for (int y = 0; y < myHeight; ++y) {
                Real ty = (Real) y / (Real) (myHeight - 1);
                progressBar(std::cout, ty, 1.0);
                Vector3 dirL = (1.0f - ty) * myDirUL + ty * myDirLL;
                Vector3 dirR = (1.0f - ty) * myDirUR + ty * myDirLR;
                dirL /= dirL.norm();
                dirR /= dirR.norm();
                for (int x = 0; x < myWidth; ++x) {
                    Real tx = (Real) x / (Real) (myWidth - 1);
                    Vector3 dir = (1.0f - tx) * dirL + tx * dirR;
                    Ray eye_ray = Ray(myOrigin, dir, max_depth);
                    Color result = trace(eye_ray);
                    image.at(x, y) = result.clamp();
                }
            }
            std::cout << "Done." << std::endl;
        }

        // Affiche les sources de lumières avant d'appeler la fonction qui
        // donne la couleur de fond.
        Color background(const Ray &ray) {
            Color result = Color(0.0, 0.0, 0.0);
            for (Light *light: ptrScene->myLights) {
                Real cos_a = light->direction(ray.origin).dot(ray.direction);
                if (cos_a > 0.99f) {
                    Real a = acos(cos_a) * 360.0 / M_PI / 8.0;
                    a = std::max(1.0f - a, 0.0f);
                    result += light->color(ray.origin) * a * a;
                }
            }
            if (ptrBackground != 0) result += ptrBackground->backgroundColor(ray);
            return result;
        }

        /// The rendering routine for one ray.
        /// @return the color for the given ray.
        Color trace(const Ray &ray) {
            assert(ptrScene != 0);
            GraphicalObject *obj_i = 0; // pointer to intersected object
            Point3 p_i;       // point of intersection

            Color c = Color(0.0, 0.0, 0.0);

            // Look for intersection in this direction.
            Real ri = ptrScene->rayIntersection(ray, obj_i, p_i);

            // Nothing was intersected
            if (ri >= 0.0f) return background(ray); // some background color

            const Material m = obj_i->getMaterial(p_i);

            // Reflexion
            if (ray.depth > 0 && m.coef_reflexion != 0) {
                Ray ray_refl = Ray(ray.origin, reflect(ray.direction, obj_i->getNormal(p_i)), ray.depth - 1);
                Color c_refl = trace(ray_refl);
                c += c_refl * m.specular * m.coef_reflexion;
            }

            // Refraction
            if (ray.depth > 0 && m.coef_refraction > 0) {
                Ray ray_refract = refractionRay(ray, p_i, obj_i->getNormal(p_i), m);
                Color c_refract = trace(ray_refract);
                c += c_refract * m.diffuse * m.coef_refraction;
            }

            if (ray.depth == 0) {
                return c + illumination(ray, obj_i, p_i);
            } else {
                return c + illumination(ray, obj_i, p_i) * m.coef_diffusion;
            }
        }

        /// Calcule le vecteur réfléchi à W selon la normale N.
        Vector3 reflect(const Vector3 &W, Vector3 N) const {
            const double tmp = W.dot(N);
            const Vector3 res = W + Vector3((-2.0 * tmp) * N[0],
                                            (-2.0 * tmp) * N[1],
                                            (-2.0 * tmp) * N[2]);
            if (W.dot(N) > 0) {
                return Vector3(-1.0 * res[0], -1.0 * res[1], -1.0 * res[2]);
            }
            return res;
        }

        /// Calcule la couleur de la lumière (donnée par light_color) dans la
        /// direction donnée par le rayon. Si aucun objet n'est traversé,
        /// retourne light_color, sinon si un des objets traversés est opaque,
        /// retourne du noir, et enfin si les objets traversés sont
        /// transparents, attenue la couleur.
        Color shadow(const Ray &ray, Color light_color) {
            Color c = light_color;
            Point3 p = ray.origin;
            Ray current_Ray = ray;

            // Tant que n'est pas noir
            while (c.max() > 0.003f) {
                //on déplace légèrement p vers L pour éviter d'intersecter l'objet initial.
                p = Point3(p[0] + 0.01 * current_Ray.direction[0],
                           p[1] + 0.01 * current_Ray.direction[1],
                           p[2] + 0.01 * current_Ray.direction[2]);
                current_Ray.origin = p;

                GraphicalObject *obj_intersect = 0;
                Point3 p_intersect;
                //  si ce rayon [p,L) intersecte un autre objet (Scene::rayIntersection)
                if (ptrScene->rayIntersection(current_Ray, obj_intersect, p_intersect) < 0.0) {
                    //on récupère le matériau m de l'objet au point p' d'intersection
                    Material m = obj_intersect->getMaterial(p_intersect);
                    //C est multiplié par la couleur diffuse et le coefficient de refraction de m
                    c = c * m.diffuse * m.coef_refraction;
                    //p <- p'
                    p = p_intersect;
                } else { break; }//sinon break
            }
            return c;
        }

        /// Calcule l'illumination de l'objet \a obj au point \a p, sachant que l'observateur est le rayon \a ray.
        Color illumination(const Ray &ray, GraphicalObject *obj, Point3 p) {
            Material m = obj->getMaterial(p);
            Color c = Color();
            const Vector3 n = obj->getNormal(p);

            for (int i = 0; i < (int) ptrScene->myLights.size(); i++) {
                const Vector3 L = ptrScene->myLights[i]->direction(p);
                double Kd = L.dot(n);

                if (Kd < 0) Kd = 0;

                // Specular
                const Vector3 V = ray.direction;
                const Vector3 W = reflect(V, obj->getNormal(p));
                const double beta = W.dot(L);

                Ray pl = Ray(p, L);
                Color b = ptrScene->myLights[i]->color(p);
                Color pp = shadow(pl, b);

                if (beta >= 0.0) {
                    const double Ks = pow(beta, obj->getMaterial(p).shinyness);
                    c = c + (pp * m.specular * Ks);
                }
                c = c + (Kd * pp * m.diffuse);
            }
            return c + m.ambient;
        }

        Ray refractionRay(const Ray &aRay, const Point3 &p, Vector3 N, const Material &m) {
            // Lois de Snell
            Real r = m.in_refractive_index / m.out_refractive_index;
            const Real cosTheta_1 = (-1.0f) * N.dot(aRay.direction);
            Real cosTheta_2;

            // Cas du rayon dans l'objet
            if (aRay.direction.dot(N) <= 0.0) { r = m.out_refractive_index / m.in_refractive_index; }


            if (cosTheta_1 > 0) {
                cosTheta_2 = r * cosTheta_1 - sqrt(1 - ((r * r) * (1 - (cosTheta_1 * cosTheta_1))));
            } else {
                cosTheta_2 = r * cosTheta_1 + sqrt(1 - ((r * r) * (1 - (cosTheta_1 * cosTheta_1))));
            }

            Vector3 vRefract = Vector3(r * aRay.direction + cosTheta_2 * N);

            // Reflexion totale
            if (1 - (r * r) * (1 - (cosTheta_1 * cosTheta_1)) < 0) {
                vRefract = reflect(aRay.direction, N);
            }
            return Ray(p + vRefract * 0.01f, vRefract, aRay.depth - 1);
        }
    };

} // namespace rt

#endif // #define _RENDERER_H_

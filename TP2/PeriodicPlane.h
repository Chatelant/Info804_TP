#ifndef TP2_PERIODICPLANE_H
#define TP2_PERIODICPLANE_H

#include "GraphicalObject.h"

namespace rt {
    struct PeriodicPlane : public GraphicalObject {

        ~PeriodicPlane() {}

        /// Creates a periodic infinite plane passing through \a c and
        /// tangent to \a u and \a v. Then \a w defines the width of the
        /// band around (0,0) and its period to put material \a band_m,
        /// otherwise \a main_m is used.
        PeriodicPlane(Point3 c, Vector3 u, Vector3 v, Material main_m, Material band_m, Real w)
                : GraphicalObject(), c(c), u(u), v(v), main_m(main_m), band_m(band_m), band_size(w) {}

        /// Etant donné un point \a p dans le plan, retourne ses coordonnées \a x et \a y selon \a u et \a v.
        void coordinates(Point3 p, Real &x, Real &y);

        // ---------------- GraphicalObject services ----------------------------
    public:

        /// This method is called by Scene::init() at the beginning of the
        /// display in the OpenGL window. May be useful for some
        /// precomputations.
        void init(Viewer & /* viewer */ ) {}

        /// This method is called by Scene::draw() at each frame to
        /// redisplay objects in the OpenGL window.
        void draw(Viewer &viewer);


        /// @return the normal vector at point \a p on the sphere (\a p
        /// should be on or close to the sphere).
        Vector3 getNormal(Point3 p);

        /// @return the material associated to this part of the object
        Material getMaterial(Point3 p);

        /// @param[in] ray the incoming ray
        /// @param[out] returns the point of intersection with the object
        /// (if any), or the closest point to it.
        ///
        /// @return either a real < 0.0 if there is an intersection, or a
        /// kind of distance to the closest point of intersection.
        Real rayIntersection(const Ray &ray, Point3 &p);

    public:
//        Le plan ainsi créé est un plan passant par le point c, avec deux vecteurs orthogonaux
//        tangents u et v qui vont définir les coordonnées du point d
//        'intersection dans le plan. Maintenant, si p est le point d'intersection entre un rayon
//        et le plan, p pourra aussi être défini à l'aide des deux coordonnées x et y selon
//        les vecteurs u et v.
//
//        Ensuite les deux matériaux servent à définir l
//        'aspect du plan infini. Si le point p a des coordonnées x ou y proche d'un entier,
//        alors on utilise le matériau pour les bandes (band_m), sinon on utilise le matériau
//        principal (main_m). Le réel w, plus petit que 1, est l'épaisseur de la bande.
        Point3 c;
        Vector3 u;
        Vector3 v;

        Material main_m;
        Material band_m;
        Real band_size;
    };

}
#endif //TP2_PERIODICPLANE_H

#include "PeriodicPlane.h"

void rt::PeriodicPlane::coordinates(Point3 p, Real &x, Real &y) {
    x = (c - p).dot(u) / u.norm();
    y = (c - p).dot(v) / v.norm();
}


void rt::PeriodicPlane::draw(Viewer & /* viewer */) {
    glBegin(GL_TRIANGLES);
    glColor4fv( main_m.ambient );
    glMaterialfv(GL_FRONT, GL_DIFFUSE, main_m.diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, main_m.specular);
    glMaterialf(GL_FRONT, GL_SHININESS, main_m.shinyness );
    glVertex3f(-1000, -1000, -10.0);
    glVertex3f(0, 1000.0, -10.0);
    glVertex3f(1000.0, 0, -10.0);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor4fv( main_m.ambient );
    glMaterialfv(GL_FRONT, GL_DIFFUSE, main_m.diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, main_m.specular);
    glMaterialf(GL_FRONT, GL_SHININESS, main_m.shinyness );
    glVertex3f(0, 1000.0, -10.0);
    glVertex3f(1000.0, 1000.0, -10.0);
    glVertex3f(1000.0, 0, -10.0);

    glEnd();
}


rt::Vector3
rt::PeriodicPlane::getNormal(Point3 p) { return p + u.cross(v); }


rt::Material
rt::PeriodicPlane::getMaterial(Point3 p) {
    Real x, y;
    coordinates(p, x, y);
    x = x - (Real)(int) x;
    y = y - (Real)(int) y;
    return (x < band_size || y < band_size ) ? band_m : main_m;
}


rt::Real
rt::PeriodicPlane::rayIntersection(const Ray &ray, Point3 &p) {
    Vector3 n = getNormal(p);
    Real gamma = (c - ray.origin).dot(n) / ray.direction.dot(n);
    if (gamma < 0) return 1.0f;
    p = c + ray.direction * gamma;
    return -1.0f;
}
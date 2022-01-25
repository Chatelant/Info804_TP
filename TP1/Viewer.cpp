/// \file Viewer.cpp
#include "Viewer.h"
#include <math.h>

using namespace std;

// Draws a tetrahedron with 4 colors.
void Viewer::draw() {
    float colorBronzeDiff[4] = {0.8, 0.6, 0.0, 1.0};
    float colorBronzeSpec[4] = {1.0, 1.0, 0.4, 1.0};
//    float colorNull[4] = {0.0, 0.0, 0.0, 1.0}; // En comm car non utilisé (warning)
    float colorSummit[4] = {0.0, 0.0, 0.0, 1.0};

    // Draws triangles given by 3 vertices.
    glBegin(GL_TRIANGLES);
    glEnable(GL_COLOR_MATERIAL); // le materiau peut changer à chaque triangle.

    glColor4fv(colorSummit);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, colorBronzeDiff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, colorBronzeSpec);
    glMaterialf(GL_FRONT, GL_SHININESS, 20.0f);
    for (int i = 0; i < (int) ptrSoup->_triangles.size(); i++) {
        const Triangle &T = ptrSoup->_triangles[i];
        Vecteur n = T.normal();
        glNormal3f(n[0], n[1], n[2]);
        colorSummit[0] = ptrSoup->_triangles.at(i)[0][0] - floor(ptrSoup->_triangles.at(i)[0][0]);
        colorSummit[1] = ptrSoup->_triangles.at(i)[0][1] - floor(ptrSoup->_triangles.at(i)[0][1]);
        colorSummit[2] = ptrSoup->_triangles.at(i)[0][2] - floor(ptrSoup->_triangles.at(i)[0][2]);
        glColor4fv(colorSummit);
        glVertex3f(ptrSoup->_triangles.at(i)[0][0], ptrSoup->_triangles.at(i)[0][1], ptrSoup->_triangles.at(i)[0][2]);
        glVertex3f(ptrSoup->_triangles.at(i)[1][0], ptrSoup->_triangles.at(i)[1][1], ptrSoup->_triangles.at(i)[1][2]);
        glVertex3f(ptrSoup->_triangles.at(i)[2][0], ptrSoup->_triangles.at(i)[2][1], ptrSoup->_triangles.at(i)[2][2]);
    }
    glDisable(GL_COLOR_MATERIAL);
    glEnd();
}


void Viewer::init() {
    // Restore previous viewer state.
    restoreStateFromFile();

    Vecteur v1 = Vecteur();
    Vecteur v2 = Vecteur();

    ptrSoup->boundingBox(v1, v2);

    qglviewer::Vec vec1 = qglviewer::Vec(v1.xyz[0], v1.xyz[1], v1.xyz[2]);
    qglviewer::Vec vec2 = qglviewer::Vec(v2.xyz[0], v2.xyz[1], v2.xyz[2]);

    camera()->setSceneBoundingBox(vec1, vec2);

// ou camera()->setSceneRadius( ... );
    camera()->showEntireScene();

    // Opens help window
    help();
}

QString Viewer::helpString() const {
    QString text("<h2>S i m p l e V i e w e r</h2>");
    text += "Use the mouse to move the camera around the object. ";
    text += "You can respectively revolve around, zoom and translate with the three mouse buttons. ";
    text += "Left and middle buttons pressed together rotate around the camera view direction axis<br><br>";
    text += "Pressing <b>Alt</b> and one of the function keys (<b>F1</b>..<b>F12</b>) defines a camera keyFrame. ";
    text += "Simply press the function key again to restore it. Several keyFrames define a ";
    text += "camera path. Paths are saved when you quit the application and restored at next start.<br><br>";
    text += "Press <b>F</b> to display the frame rate, <b>A</b> for the world axis, ";
    text += "<b>Alt+Return</b> for full screen mode and <b>Control+S</b> to save a snapshot. ";
    text += "See the <b>Keyboard</b> tab in this window for a complete shortcut list.<br><br>";
    text += "Double clicks automates single click actions: A left button double click aligns the closer axis with the camera (if close enough). ";
    text += "A middle button double click fits the zoom of the camera and the right button re-centers the scene.<br><br>";
    text += "A left button double click while holding right button pressed defines the camera <i>Revolve Around Point</i>. ";
    text += "See the <b>Mouse</b> tab and the documentation web pages for details.<br><br>";
    text += "Press <b>Escape</b> to exit the viewer.";
    return text;
}

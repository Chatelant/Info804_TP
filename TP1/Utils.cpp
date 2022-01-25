//
// Created by user on 14/01/2022.
//

#include "Utils.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>

// ----- Vecteur ----- //
Vecteur::Vecteur() {
    xyz[0] = 0.0;
    xyz[1] = 0.0;
    xyz[2] = 0.0;
}

Vecteur::Vecteur(float x, float y, float z) {
    xyz[0] = x;
    xyz[1] = y;
    xyz[2] = z;
}

Vecteur Vecteur::cross(const Vecteur &v) const {
    return Vecteur((xyz[1] * v.xyz[2] - xyz[2] * v.xyz[1]),
                   (xyz[2] * v.xyz[0] - xyz[0] * v.xyz[2]),
                   (xyz[0] * v.xyz[1] - xyz[1] * v.xyz[0]));
}

float Vecteur::operator[](int i) const { return xyz[i]; }

float &Vecteur::operator[](int i) { return xyz[i]; }

Vecteur Vecteur::inf(const Vecteur &other) const {
    float xmax, ymax, zmax;
    if (this->xyz[0] < other.xyz[0]) { xmax = this->xyz[0]; } else { xmax = other.xyz[0]; }
    if (this->xyz[1] < other.xyz[1]) { ymax = this->xyz[1]; } else { ymax = other.xyz[1]; }
    if (this->xyz[2] < other.xyz[2]) { zmax = this->xyz[2]; } else { zmax = other.xyz[2]; }
    return Vecteur(xmax, ymax, zmax);
}

Vecteur Vecteur::sup(const Vecteur &other) const {
    float xmax, ymax, zmax;
    if (this->xyz[0] > other.xyz[0]) { xmax = this->xyz[0]; } else { xmax = other.xyz[0]; }
    if (this->xyz[1] > other.xyz[1]) { ymax = this->xyz[1]; } else { ymax = other.xyz[1]; }
    if (this->xyz[2] > other.xyz[2]) { zmax = this->xyz[2]; } else { zmax = other.xyz[2]; }
    return Vecteur(xmax, ymax, zmax);
}

Vecteur Vecteur::operator-(Vecteur other) const {
    return Vecteur(xyz[0] - other.xyz[0],
                   xyz[1] - other.xyz[1],
                   xyz[2] - other.xyz[2]);
}

Vecteur Vecteur::operator+(Vecteur other) const {
    return Vecteur(xyz[0] + other.xyz[0],
                   xyz[1] + other.xyz[1],
                   xyz[2] + other.xyz[2]);
}

std::ostream &operator<<(std::ostream &out, Vecteur v) {
    out << v[0] << " " << v[1] << " " << v[2];
    return out;
}

std::istream &operator>>(std::istream &in, Vecteur &v) {
    in >> v[0] >> v[1] >> v[2];
    return in;
}

// ----- Triangle ----- //
Triangle::Triangle() {
    _xyz->push_back(Vecteur());
    _xyz->push_back(Vecteur());
    _xyz->push_back(Vecteur());
}

Triangle::Triangle(Vecteur v1, Vecteur v2, Vecteur v3) {
    _xyz->push_back(v1);
    _xyz->push_back(v2);
    _xyz->push_back(v3);
}

Vecteur Triangle::normal() const {
    Vecteur v1 = _xyz->at(1) - _xyz->at(0);
    Vecteur v2 = _xyz->at(2) - _xyz->at(0);

    // Cross
    Vecteur vres = v1.cross(v2);

    float norm = sqrt(pow(vres.xyz[0], 2.0) + pow(vres.xyz[1], 2.0) + pow(vres.xyz[2], 2.0));

    // Normalisation
    vres.xyz[0] = vres.xyz[0] / norm;
    vres.xyz[1] = vres.xyz[1] / norm;
    vres.xyz[2] = vres.xyz[2] / norm;
    return vres;
}

Vecteur Triangle::operator[](int i) const {
    return _xyz->at(i);
}

Vecteur &Triangle::operator[](int i) {
    return _xyz->at(i);
}

std::ostream &operator<<(std::ostream &out, Triangle t) {
    out << t[0] << " " << t[1] << " " << t[2];
    return out;
}

std::istream &operator>>(std::istream &in, Triangle &t) {
    in >> t[0] >> t[1] >> t[2];
    return in;
}

// ----- TriangleSoup ----- //
TriangleSoup::TriangleSoup() {}

void TriangleSoup::read(std::istream &in) {
    // Init input
    if (!in.good()) std::cerr << "TriangleSoup::read --> Erreur ouverture input" << std::endl;

    // Lecture ligne en str
    for (string line; getline(in, line);) {
        // Si la ligne n'est pas un commentaire
        if (line[0] != '#') {
            istringstream ss(line);
            Triangle tmp = Triangle();
            ss >> tmp;
            _triangles.insert(_triangles.end(), tmp);
        }
    }
}

int TriangleSoup::size() { return _triangles.size(); }

int TriangleSoup::size() const { return _triangles.size(); }

void TriangleSoup::boundingBox(Vecteur &low, Vecteur &up) const {
    low = _triangles[0][0];
    up = _triangles[0][0];
    for (int i = 0; i < (int) _triangles.size(); i++) {
        for (int j = 0; j < 3; j++) {
            low = low.inf(_triangles[i][j]);
            up = up.sup(_triangles[i][j]);
        }
    }
}

int TriangleSoup::write(std::ofstream &output) {
    if (!output.good()) return 1;

    for (int i = 0; i < (int) _triangles.size(); i++) { // triangles
        for (int j = 0; j < 3; j++) { // Vecteurs
            output << _triangles[i][j] << " ";
        }
        output << endl;
    }
    return 0;
};

// ----- Index ----- /
Index::Index(int i0, int i1, int i2) {
    idx[0] = i0;
    idx[1] = i1;
    idx[2] = i2;
}

Index::Index(int indices[]) {
    idx[0] = indices[0];
    idx[1] = indices[1];
    idx[2] = indices[2];
}

bool Index::operator<(const Index &other) const {
    return (idx[0] < other.idx[0])
           || ((idx[0] == other.idx[0])
               && ((idx[1] < other.idx[1])
                   || ((idx[1] == other.idx[1])
                       && (idx[2] < other.idx[2]))));
}

bool Index::operator==(const Index &other) const {
    return (idx[0] == other.idx[0] && idx[1] == other.idx[1] && idx[2] == other.idx[2]);
}

// ----- TriangleSoupZipper ----- //
TriangleSoupZipper::TriangleSoupZipper(const TriangleSoup &anInput,
                                       TriangleSoup &anOuput,
                                       Index size) {
    // Boite englobante
    Vecteur vInf, vSupp;
    anInput.boundingBox(_low, _up);

    // Taille
    Vecteur vSizeBox = _up - _low; // Taille boite

    _size = Vecteur(vSizeBox[0] / size.idx[0],
                    vSizeBox[1] / size.idx[1],
                    vSizeBox[2] / size.idx[2]);

//    zip(anInput, anOuput);
    smartZip(anInput, anOuput);
}

TriangleSoupZipper::TriangleSoupZipper() {
    _low = Vecteur(-5, -5, -5);
    _up = Vecteur(5, 5, 5);

    // Taille
    Vecteur vSizeBox = _up - _low; // Taille boite

    _size = Vecteur(vSizeBox[0] / 10,
                    vSizeBox[1] / 10,
                    vSizeBox[2] / 10);

    Vecteur v = Vecteur(1.2, 3.4, -1.7);
    Index x = index(v);

    std::cout << "Index obtenu  :" << x.idx[0] << " " << x.idx[1] << " " << x.idx[2] << std::endl;
    std::cout << "Index attendu : 6, 8, 3" << std::endl;

    Index i = Index(6, 8, 3);
    Vecteur a = centroid(i);

    std::cout << "Vect obtenu  :" << a << std::endl;
    std::cout << "Vect attendu : 1.5, 3.5, -1.5" << std::endl;
}

Index TriangleSoupZipper::index(const Vecteur &p) const {
    return Index(floor((p - _low)[0] / _size[0]),
                 floor((p - _low)[1] / _size[1]),
                 floor((p - _low)[2] / _size[2]));
}

Vecteur TriangleSoupZipper::centroid(const Index &idx) const {
    return Vecteur((idx[0] * _size[0] + _low[0]) + _size[0] / 2, (idx[1] * _size[1] + _low[1]) + _size[1] / 2,
                   (idx[2] * _size[2] + _low[2]) + _size[2] / 2);
}

void TriangleSoupZipper::zip(const TriangleSoup &anIput, TriangleSoup &anOutput) {
    for (int i = 0; i < anIput.size(); ++i) {
        Index idxX = index(anIput[i][0]);
        Index idxY = index(anIput[i][1]);
        Index idxZ = index(anIput[i][2]);
        if (!(idxX == idxY && idxX == idxZ && idxY == idxZ)) {
            // Ajouter le triangle
            anOutput._triangles.insert(anOutput._triangles.end(),
                                       Triangle(centroid(idxX), centroid(idxY), centroid(idxZ)));

            // Pour idxX
            if (index2data.find(idxX) == index2data.end()) {
                index2data.insert(pair<Index, CellData>(idxX, CellData()));
            }
            index2data[idxX].add(anIput[i][0]);

            // Pour idxY
            if (index2data.find(idxY) == index2data.end()) {
                index2data.insert(pair<Index, CellData>(idxY, CellData()));
            }
            index2data[idxY].add(anIput[i][1]);

            // Pour idxZ
            if (index2data.find(idxZ) == index2data.end()) {
                index2data.insert(pair<Index, CellData>(idxZ, CellData()));
            }
            index2data[idxZ].add(anIput[i][2]);
        }
    }
}

void TriangleSoupZipper::smartZip(const TriangleSoup &anIput, TriangleSoup &anOutput) {
    index2data.clear();
    zip(anIput, anOutput);

    for (int i = 0; i < anOutput.size(); i++) {
        for (int j = 0; j < 3; j++) {
            anOutput._triangles[i][j] = index2data[index(anOutput._triangles[i][j])].barycenter();
        }
    }
}

// ----- CellData ----- //
void CellData::add(const Vecteur &v) {
    acc = acc + v;
    nb++;
}

Vecteur CellData::barycenter() const {
    return Vecteur((acc[0] / nb), (acc[1] / nb), (acc[2] / nb));
}


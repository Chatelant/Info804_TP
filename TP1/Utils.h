#ifndef TP1_CHATEL_UTILS_H
#define TP1_CHATEL_UTILS_H

#include <iostream>
#include <vector>
#include <map>

using namespace std;

/// Vecteur
struct Vecteur {
    float xyz[3]; // les composantes
    Vecteur();

    Vecteur(float x, float y, float z);// constructeur
    Vecteur cross(const Vecteur &v) const;


    float operator[](int i) const;     // accesseur en lecture
    float &operator[](int i);          // accesseur en ecriture
    Vecteur operator-(Vecteur other) const;

    Vecteur operator+(Vecteur other) const;

    // Retourne le vecteur dont les composantes sont les minima des
    // composantes de soi-même et de other.
    Vecteur inf(const Vecteur &other) const;

    // Retourne le vecteur dont les composantes sont les maxima des
    // composantes de soi-même et de other.
    Vecteur sup(const Vecteur &other) const;
};

std::ostream &operator<<(std::ostream &out, Vecteur v);

std::istream &operator>>(std::istream &in, Vecteur &v);

/// Class Triangle
// Represente un triangle a partir de trois vecteurs
class Triangle {
private:
    vector <Vecteur> _xyz[3];

public:
    Triangle();

    Triangle(Vecteur v1, Vecteur v2, Vecteur v3);

    Vecteur normal() const;

    Vecteur operator[](int i) const;     // accesseur en lecture
    Vecteur &operator[](int i);          // accesseur en ecriture
};

std::ostream &operator<<(std::ostream &out, Triangle t);

std::istream &operator>>(std::istream &in, Triangle &t);

/// Class TriangleSoup
// Contient une soupe de triangles
struct TriangleSoup {
    std::vector <Triangle> _triangles;

    TriangleSoup();

    void boundingBox(Vecteur &low, Vecteur &up) const;

    void read(std::istream &in);

    int write(std::ofstream &output);

    int size();

    int size() const;

    Triangle operator[](int i) const { return _triangles.at(i); }

    Triangle &operator[](int i) { return _triangles.at(i); }
};

/// Class Index
/// Définit un index sur 3 entiers. Toutes les opérations usuelles
/// sont surchargées (accès, comparaisons, égalité).
struct Index {
    int idx[3];

    Index() {}

    Index(int i0, int i1, int i2);

    Index(int indices[]);

    int operator[](int i) const { return idx[i]; }

    int &operator[](int i) { return idx[i]; }

    bool operator<(const Index &other) const;

    bool operator==(const Index &other) const;
};

/// Struct CellData
// Structure pour calculer le barycentre d'un ensemble de points.
struct CellData {
    Vecteur acc;
    int nb;

    // Crée un accumulateur vide.
    CellData() : acc(), nb(0) {}

    // Ajoute le point v à l'accumulateur.
    void add(const Vecteur &v);

    // Retourne le barycentre de tous les points ajoutés.
    Vecteur barycenter() const;
};

/// Struct TriangleSoupZipper
// Construit le zipper avec une soupe de triangle en entrée \a
// anInput, une soupe de triangle en sortie \a anOutput, et un index \a size
// qui est le nombre de cellules de la boîte découpée selon les 3 directions.
struct TriangleSoupZipper {
    Vecteur _low;   // bas gauche
    Vecteur _up;    // haut droit
    Vecteur _size;  // taille cellule
    std::map <Index, CellData> index2data;   // Stocke pour chaque cellule son barycentre.

    TriangleSoupZipper(const TriangleSoup &anInput,
                       TriangleSoup &anOuput,
                       Index size);

    TriangleSoupZipper();

    /// @return l'index de la cellule dans laquelle tombe \a p.
    Index index(const Vecteur &p) const;

    /// @return le centroïde de la cellule d'index \a idx (son "centre").
    Vecteur centroid(const Index &idx) const;

    void zip(const TriangleSoup &anIput, TriangleSoup &anOutput);

    void smartZip(const TriangleSoup &anIput, TriangleSoup &anOutput);
};

#endif //TP1_CHATEL_UTILS_H

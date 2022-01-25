#include <qapplication.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Viewer.h"
#include "Utils.h"

using namespace std;

/// Main de developpement
int main(int argc, char **argv) {
    // Récupération des informations d'un fichier
    if (argc < 2 || argc > 3) {
        cout << "main : error : ./viewer file.tri (optional)N  (default N:20)" << endl;
        return 1;
    }

    // Instantiation soup
    TriangleSoup ts = TriangleSoup();
    TriangleSoup tsout;
    ifstream input(argv[1]);
    ts.read(input);
    input.close();

    // Instantiation TriangleSoupZipper
    if (argc == 3) {
        int n = atoi(argv[2]);
        TriangleSoupZipper tsz = TriangleSoupZipper(ts, tsout, Index(n, n, n));
        cout << "N = " << n << endl;
    } else {
        TriangleSoupZipper tsz = TriangleSoupZipper(ts, tsout, Index(20, 20, 20));
        cout << "N = " << 20 << endl;
    }

    // Read command lines arguments.
    QApplication application(argc, argv);
    // Instantiate the viewer.
//    Viewer viewer(&ts);
    Viewer viewer(&tsout);
    // Give a name
    viewer.setWindowTitle("Viewer triangle soup");
    // Make the viewer window visible on screen.
    viewer.show();
    // Run main loop.
    application.exec();
    return 0;
}


/// ----- main visualiseur ----- //
//int main(int argc, char **argv) {
//    // Récupération des informations d'un fichier
//    if (argc < 2 || argc > 2) {
//        cout << "erreur : paramètre : file.tri" << endl;
//        return 1;
//    }
//
//    // Instantiation soup
//    TriangleSoup ts = TriangleSoup();
//    ifstream input(argv[1]);
//    ts.read(input);
//    input.close();
//
//    // Read command lines arguments.
//    QApplication application(argc, argv);
//    // Instantiate the viewer.
//    Viewer viewer(&ts);
//    // Give a name
//    viewer.setWindowTitle("Viewer triangle soup");
//    // Make the viewer window visible on screen.
//    viewer.show();
//    // Run main loop.
//    application.exec();
//    return 0;
//}

/// ----- main compresseur ----- //
//int main(int argc, char **argv) {
//    // Récupération des informations d'un fichier
//    if (argc != 6) {
//        cout << "args : fileIn.tri fileOut.tri Nx Ny Nz" << endl;
//        return 1;
//    }
//
//    // Instantiation soup
//    TriangleSoup ts = TriangleSoup();
//    TriangleSoup tsout;
//    ifstream input(argv[1]);
//    ts.read(input);
//    input.close();
//
//    // Instantiation TriangleSoupZipper
//    int nx = atoi(argv[3]);
//    int ny = atoi(argv[4]);
//    int nz = atoi(argv[5]);
//    TriangleSoupZipper tsz = TriangleSoupZipper(ts, tsout, Index(nx, ny, nz));
//
//    // Affichage
//    cout << "Nb triangles entree : " << ts.size() << endl;
//    cout << "Nb triangles sortie : " << tsout.size() << endl;
//    cout << "Taux de compression : : " << (float)tsout.size() / (float)ts.size() * 100.0 << endl;
//
//    // Export TriangleSoupZipper
//    ofstream output(argv[2]);
//    tsout.write(output);
//
//    return 0;
//}
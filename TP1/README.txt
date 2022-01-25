Etudiant : Antoine Chatel
Groupe : 2
Stade : 5 (terminé)


Executables :
 - visualiseur : correspond au 1. permet de visualiser les file.tri
 - compresseur : correspond au 2. permet de compresser les triangles et les ecrire dans un fichier sortie

Code source :

Dans le main.cpp figure les 3 main() utilisés.
    - Le premier correspond au main utilisé lors du dev
    - Le second au main qui a généré l'exec visualiseur
    - Le dernier au main qui a généré le compresseur

command (exec visualiseur) :
    $ ./visualiseur file.tri

command (exec compresseur) :
    $ ./compresseur fileInput.tri fileOutput.tri Nx Ny Nz

command (main dev) :
$  make
$  ./viewer file.tri    (default N = 20)
$  ./viewer file.tri N

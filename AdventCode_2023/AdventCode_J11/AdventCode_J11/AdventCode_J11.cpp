// AdventCode_J11.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <list>

using namespace std;

int main()
{
    ifstream input;
    input.open("input.txt");
    string line;
    //map<int, vector<int>> galaxies;
    vector<pair<long long, long long>> galaxies;
    //Liste des indices des colonnes vides
    list<int> emptySpaces;
    long long index = 0;
    while (getline(input, line)) 
    {
        if (emptySpaces.size() == 0) {
            for (int i = 0; i < line.length(); i++) {
                emptySpaces.push_back(i);
            }
        }
        if (find(line.begin(), line.end(), '#') == line.end()) {
            //On double les lignes qui ne contiennent pas de galaxies
            //On pense bien à augmenter l'index (car on vient d'ajouter une deuxième ligne)

            //PART ONE
            //index+=1;

            //PART TWO
            //La taille des galaxies vides est multipliée par 1 000 000, donc on ajoute 999 999 lignes
            index += (999999);
        }
        else {
            int subIndex = 0;
            for (int i = 0; i < line.length(); i++) {
                if (line[i] == '#') {
                    galaxies.push_back({ i, index });
                    emptySpaces.erase(remove(emptySpaces.begin(), emptySpaces.end(), subIndex), emptySpaces.end());
                }
                subIndex++;
            }
        }
        index++;
    }
    input.close();

    for (int i = 0; i < galaxies.size(); i++) 
    {
        //On décale les galaxies sur chaque ligne, selon le nombre de colonnes vides à leur gauche
        //Il faut tenir compte du fait que les indices des colonnes vides sont valables lorsqu'il n'y a eu AUCUN déplacement
        //A partir du moment où on commence à décaler les valeurs, il faut ajouter à l'indice initial de la colonne vide le déplacement qu'il y a eu
        long long displacement = 0;
        for (auto& empty : emptySpaces) 
        {
            if (galaxies[i].first > empty + displacement) {
                //PART ONE
                /*galaxies[i].first += 1;
                displacement += 1;*/
                //PART TWO
                //La taille des galaxies vides est multipliée par 1 000 000, donc on décale les étoiles de 999 999 cases
                galaxies[i].first+=999999;
                displacement+=999999;
            }
        }
        cout << galaxies[i].first << " " << galaxies[i].second << endl;
    }

    //Distance entre deux étoiles : abs(xb - xa) + abs(yb - ya)
    // Cela vient du fait que la distance la plus courte serait en théorie la droite reliant A et B (de valeur sqrt((xb-xa)² + (yb-ya)²)
    // Mais on ne peut effectuer des déplacement que selon haut-bas-gauche-droite pour joindre des étoiles
    // Le chemin le plus court sera donc alors parcourir la distance séparant les coorodnnées X puis la distance séparant les coordonnées y
    //TODO : calculer la distance entre toutes les paires
    long long sum = 0;
    //Prenons 4 galaxies, A B C D
    //On a les paires uniques AB AC AD, puis BC BD et enfin CD
    //Ainsi, pour chacune des galaxies, on peut former n-i-1 paires uniques (i étant l'indice de la galaxie)
    for (int i = 0; i < galaxies.size(); i++) {
        for (int j = i + 1; j < galaxies.size(); j++) 
        {
            sum += (abs(galaxies[i].first - galaxies[j].first) + abs(galaxies[i].second - galaxies[j].second));
        }
    }
    cout << sum << endl;


}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.

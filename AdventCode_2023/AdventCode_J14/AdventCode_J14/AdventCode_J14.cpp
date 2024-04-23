// AdventCode_J14.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

bool compare_patterns(vector<string>& left, vector<string>& right) 
{
    if (left.size() != right.size()) return false;
    for (int i = 0; i < left.size(); i++) 
    {
        if (left[i].compare(right[i]) != 0) return false;
    }
    return true;
}


int main()
{
    ifstream input;
    vector<string> dish;
    input.open("input.txt");
    string line;
    while (getline(input, line)) 
    {
        dish.push_back(line);
    }
    input.close();
    int total = 0;


    vector<vector<string>> patterns;

    for (long long cycle = 0; cycle < 1000000000; cycle++)
    {

        //TODO : avoir une liste des patterns déjà parcourus => permet, dans le cas de boucles, de ne pas
        //Recalculer des résultats déjà calculés
        //Permet donc d'arrêter la boucle et de calculer quel sera le résultat à la fin de celle-ci
        bool loop = false;
        for (int i = 0; i < patterns.size(); i++) {
            if (compare_patterns(dish, patterns[i]) == true) 
            {
                loop = true;
                if (i < patterns.size() - 1)
                {
                    //On calcule le patterne résultant à la fin des cycles restants, selon le nombre de cycles restant et le nombre de séquences dans la boucle
                    //Index (dans la liste) de l'élément au DEBUT dernier cycle : index actuel + (nombre cycles RESTANTS % longueur boucle)
                    //Donc index à la FIN du dernier cycle : on doit ajouter 1 élément au nombre d'éléments restants (qui est donc 1000000001 - indice_cycle - 1)
                    int end_pattern = i + (1000000000 - cycle) % (patterns.size() - i);
                    //cout << "end pattern index : " << end_pattern << endl;
                    dish = patterns[end_pattern];
                }
                break;
            }
        }
        //On s'arrête si on trouve une boucle
        if (loop == true) {
            break;
        }

        //On ajoute le pattern avant modification à la liste des patterns
        patterns.push_back(dish);

        //NORTH
        //On décale tous les cailloux vers le nord
        //Pour chaque colonne on pousse les cailloux vers l haut, en partant de ceux le plus en haut
        for (int i = 0; i < dish[0].length(); i++)
        {
            //Représente l'indice du premier espace libre,
            //ie le premier espace sur lequel se déplacera un caillou situé plus en bas
            int first_free_space = INT_MAX;
            for (int j = 0; j < dish.size(); j++)
            {
                //Si on rencontre un caillou, et qu'on a un espace libre plus haut (ie avec un indice < à celui du caillou
                if (dish[j][i] == 'O')
                {
                    if (j > first_free_space) {
                        //On peut alors pousser le caillou sur l'espace libe
                        dish[first_free_space][i] = 'O';
                        dish[j][i] = '.';
                        //Le prochain espace libre est au moins une case plus basse que celle de l'espace libre précédent, car le caillou
                        //A réussi à atteindre la position de l'espace libre, ce qui implique donc que l'espace juste après est libre
                        first_free_space += 1;
                    }
                }
                //Si on a un nouvel espace libre, on sauvegarde sa position
                else if (dish[j][i] == '.' && j < first_free_space)
                {
                    first_free_space = j;
                }
                //Si on rencontre un rocher inamovible, les espaces libres au-dessus de lui ne pourront pas être atteints par les cailloux plus bas
                //On reprend donc notre recherche d'espace libre
                else if (dish[j][i] == '#') {
                    first_free_space = INT_MAX;
                }
            }
        }
        //WEST
        //On décale tous les cailloux vers l'ouest
        //Pour chaque ligne on pousse les cailloux vers la droite, en partant de ceux le plus à droite
        //La logique est la même que pour le cas du nord, si ce n'est qu'on parcours les lignes et non pas les colonnes
        for (auto& it : dish) 
        {
            int first_free_space = INT_MAX;
            for (int i = 0; i < dish[0].length(); i++)
            {
                //Si on rencontre un caillou, et qu'on a un espace libre plus haut (ie avec un indice < à celui du caillou
                if (it[i] == 'O')
                {
                    if (i > first_free_space) {
                        //On peut alors pousser le caillou sur l'espace libe
                        it[first_free_space] = 'O';
                        //Penser à remplacer le caillou par un espace vide 
                        it[i] = '.';
                        //Le prochain espace libre est au moins une case plus à droite que celle de l'espace libre précédent, car le caillou
                        //A réussi à atteindre la position de l'espace libre, ce qui implique donc que l'espace juste à droite est libre
                        first_free_space += 1;
                    }
                }
                //Si on a un nouvel espace libre, on sauvegarde sa position
                else if (it[i] == '.' && i < first_free_space)
                {
                    first_free_space = i;
                }
                //Si on rencontre un rocher inamovible, les espaces libres au-dessus de lui ne pourront pas être atteints par les cailloux plus bas
                //On reprend donc notre recherche d'espace libre
                else if (it[i] == '#') {
                    first_free_space = INT_MAX;
                }
            }
        }

        //SOUTH
        //On décale tous les cailloux vers le sud
        for (int i = 0; i < dish[0].length(); i++)
        {
            int first_free_space = INT_MIN;
            for (int j = dish.size() - 1; j >= 0; j--)
            {
                if (dish[j][i] == 'O')
                {
                    if (j < first_free_space) {
                        dish[first_free_space][i] = 'O';
                        dish[j][i] = '.';

                        //Le prochain espace libre est au moins une case plus haute que celle de l'espace libre précédent, car le caillou
                        //A réussi à atteindre cette position, ce qui implique donc que l'espace juste avant est libre
                        first_free_space -= 1;
                    }
                }
                else if (dish[j][i] == '.' && j > first_free_space)
                {
                    first_free_space = j;
                }
                else if (dish[j][i] == '#') {
                    first_free_space = INT_MIN;
                }
            }
        }

        //EAST
        //On décale tous les cailloux vers l'est
        //Pour chaque ligne on pousse les cailloux vers la droite, en partant de ceux le plus à droite
        //La logique est la même que pour le cas du nord, si ce n'est qu'on parcours les lignes et non pas les colonnes
        for (auto& it : dish)
        {
            int first_free_space = INT_MIN;
            for (int i = dish[0].length() - 1; i >= 0; i--)
            {
                //Si on rencontre un caillou, et qu'on a un espace libre plus haut (ie avec un indice < à celui du caillou
                if (it[i] == 'O')
                {
                    if (i < first_free_space) {
                        //On peut alors pousser le caillou sur l'espace libe
                        it[first_free_space] = 'O';
                        it[i] = '.';

                        //Le prochain espace libre est au moins une case plus à gauche que celle de l'espace libre précédent, car le caillou
                        //A réussi à atteindre la position de l'espace libre, ce qui implique donc que l'espace juste à gauche est libre
                        first_free_space -= 1;
                    }
                }
                //Si on a un nouvel espace libre, on sauvegarde sa position
                else if (it[i] == '.' && i > first_free_space)
                {
                    first_free_space = i;
                }
                //Si on rencontre un rocher inamovible, les espaces libres au-dessus de lui ne pourront pas être atteints par les cailloux plus bas
                //On reprend donc notre recherche d'espace libre
                else if (it[i] == '#') {
                    first_free_space = INT_MIN;
                }
            }
        }
    }

    for (int l = 0; l < dish.size(); l++) {
        for (int i = 0; i < dish[0].length(); i++) 
        {
            if (dish[l][i] == 'O') {
                total += (dish[0].length() - l);
            }
        }
    }

    //PART ONE
    //On compte le poids sur les piliers nord
    //for (int i = 0; i < dish[0].length(); i++) 
    //{
    //    int first_free_space = INT_MAX;
    //    for (int j = 0; j < dish.size(); j++)
    //    {
    //        if (dish[j][i] == 'O') 
    //        {
    //            if (j > first_free_space) {
    //                total += dish.size() - first_free_space;
    //                //Le prochain espace libre est au moins une case plus base que celle de l'espace libre précédent, car le caillou
    //                //A réussi à atteindre cette position, ce qui implique donc que l'espace juste après est libre
    //                first_free_space += 1;
    //            }
    //            else {
    //                total += dish.size() - j;
    //            }
    //        }
    //        else if (dish[j][i] == '.' && j < first_free_space) 
    //        {
    //            first_free_space = j;
    //        }
    //        else if (dish[j][i] == '#') {
    //            first_free_space = INT_MAX;
    //        }
    //    }
    //}
    cout << total << endl;
    
    

}

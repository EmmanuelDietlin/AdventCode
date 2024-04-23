// AdventCode_J22.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <cmath>

using namespace std;


int main()
{
    ifstream input;
    int total = 0;
    input.open("input.txt");
    string line;
    //On va stocker les différentes briques dans une multimap selon leur valeur de coordonnée z
    //Ensuite, on pourra ainsi regarder pour chaque brique les briques directement au-dessus et vérifier si enlever la brique impacte les briques au-dessus
    //La première valeur est la coordonnée du coin gauche
    //La deuxième valeur est la différence entre la coordonnée du coin droite et du coin gauche
    //LEs coordonnées sont de la forme x,y,z
    vector<pair<vector<int>, vector<int>>> bricks;
    while (getline(input, line)) 
    {
        pair<vector<int>, vector<int>> brick;
        string left, right;
        int index = line.find('~');
        left = line.substr(0, index);
        right = line.substr(index + 1);
        stringstream ss(left);
        string coord;
        while (getline(ss, coord, ',')) {
            brick.first.push_back(stoi(coord));
            brick.second.push_back(stoi(coord));
        }
        ss = stringstream(right);
        int subIndex = 0;
        while (getline(ss, coord, ',')) {
            brick.second[subIndex] = stoi(coord) - brick.second[subIndex];
            subIndex++;
        }
        bricks.push_back(brick);
    }
    input.close();

    //Le sol est à z = 0 => z est l'axe vertical
    //Comme les clés sont triées par valeur (propriétés des maps), l'ordre des éléments est donc croissant vers le haut

    //On doit également faire descendre les briques, car la snapshot à été prise à un instant t NON final
    //Donc certaines briques sont encore en l'air
    //On doit donc déterminer en premier lieu leur placement final

    //On a ici une map donnant, pour chaque case, la hauteur minimal à laquelle peut se situer une brique
    //PAr observation, les coordonnées x et y sont comprises entre 0 et 9 inclus

    //TODO
    //Il faudrait associer à chaque case un indice supplémentaire pour indiquer quelle brique couvre ce morceau
    // Cela permettrait ainsi de savoir, lorsqu'on fait descendre une brique, si elle est supportée par une seule ou plusieurs briques
    // TODO
    map<pair<int, int>, pair<int, int>> minHeight;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            //La hauteur minimale possible est 1
            minHeight[{i, j}] = { 1,0};
        }
    }
    int unique_index = 1;

    map<int, bool> safe_removal;
    //Respectement les lien entre une brique et ses enfants et une brique et ses parents
    map<int, vector<int>> tree, parents_tree;
    map<int, pair<vector<int>, vector<int>>> lookup_table;

    map<int, vector<int>> placement;

    //On trie les briques par Z croissant
    sort(bricks.begin(), bricks.end(), [](const pair<vector<int>, vector<int>>& left, const pair<vector<int>, vector<int>>& right) {return left.first[2] < right.first[2]; });
    //Les briques sont triées par ordre de y croissant, ce qui nous permet de traiter la mise en position dans le bon ordre
    for (auto& it : bricks) 
    {
        map<int, bool> supports;
        //PAr défaut, on suppose que la brique peut être retirée sans problème
        //Ce qui sera confirmé ou non par l'étude des briques au-dessus
        safe_removal[unique_index] = true;
        int index = unique_index;
        //On place tout d'abord les briques à leur placement final
        //On regarde, pour toute la zone couverte par la brique, sur quelle hauteur elle va se placer
        //La hauteur sera toujours la plus grande valeur trouvée sur cette zone
        int newHeight = 1;
        for (int i = 0; i <= it.second[0]; i++) {
            for (int j = 0; j <= it.second[1]; j++) {
                //On garde en mémoire toutes les briques supportant la nouvelle brique
                if (minHeight[{it.first[0] + i, it.first[1] + j}].first > newHeight) {
                    newHeight = minHeight[{it.first[0] + i, it.first[1] + j}].first;
                    supports.clear();
                    supports[minHeight[{it.first[0] + i, it.first[1] + j}].second] = true;
                }
                else if (minHeight[{it.first[0] + i, it.first[1] + j}].first == newHeight) {
                    supports[minHeight[{it.first[0] + i, it.first[1] + j}].second] = true;
                }
            }
        }
        //On met à jour la hauteur de la brique
        it.first[2] = newHeight;
        
        //On met ensuite la nouvelle hauteur minimale possible sur la zone, qui est donc le max de hauteur min précédente + hauteur brique + 1
        for (int i = 0; i <= it.second[0]; i++) {
            for (int j = 0; j <= it.second[1]; j++) {
                minHeight[{it.first[0] + i, it.first[1] + j}].first = newHeight + 1 + it.second[2];
                //On met à jour l'identifiant de la dernière brique posée sur ces cases, qui est donc celui de la brique actuelle
                minHeight[{it.first[0] + i, it.first[1] + j}].second = unique_index;
            }
        }
        //On pense également à mettre à jour l'index unique
        lookup_table[unique_index] = it;
        placement[unique_index] = it.first;

        unique_index++;

        //Enfin, on regarde combien de briques différentes supportent la nouvelle brique
        //Si == 1, alors on ne pourra pas retirer la brique en dessous
        //Si >= 2, alors les briques sont potentiellement retirables
        //On fait attention à exclure le cas 0, car c'est le sol (qui ne compte pas comme une brique retirable)
        if (supports.find(0) != supports.end()) {
            //cout << unique_index - 1 << endl;
            continue;
        }
        if (supports.size() == 1) {
            //On ajoute cette brique comme enfant du support
            tree[supports.begin()->first].push_back(index);
            parents_tree[index].push_back(supports.begin()->first);
            safe_removal[supports.begin()->first] = false;
        }
        else {
            for (auto& itt : supports) {
                //On ajoute cette brique comme enfant de chaque support
                tree[itt.first].push_back(index);
                parents_tree[index].push_back(itt.first);
                //Si la brique ne peut pas être retirée de manière sûre, alors on passe à la brique suivante
                //Elle ne pourra jamais être retirée de manière ^sûre
                if (safe_removal.count(itt.first) > 0 && safe_removal[itt.first] == false) continue;
                safe_removal[itt.first] = true;
            }
        }

    }
    //On compte toutes les briques que l'on peut enlever
    for (auto& it : safe_removal) {
        if (it.second == true) total++;
    }

    //sort(bricks.begin(), bricks.end(), [](const pair<vector<int>, vector<int>>& left, const pair<vector<int>, vector<int>>& right) {return left.first[2] < right.first[2]; });

    //PART TWO
    //On doit déterminer, pour chaque brique, combien de briques tombent si celle-ci venait à être désintégrée
    //Pour chaque brique, on va alors stocker sous forme d'arbre les liens entre briques (quelle brique supporte quelle brique)
    //On aura ensuite juste à parcourir l'arbre et voir, pour chaque brique que l'on supprime, si les briques au-dessus tombent
    int total_2 = 0;
    for (auto& it : tree)
    {
        //Méthode 1 => pas le bon résultat
        // 
        // 
        // TODO : 
        // trouver erreur et faire fonctionner
        // Résultat trouvé : 59683
        // Résultat attendu : 57770
        // TODO
        // Source d'erreur : on a des éléments qui apparaissent en boucle 
        // Dû au fait que l'on vérifiait si on avait des éléments en double dans la liste "next", et pas dans la liste "allconsidered" !
        // 
        // 
        //PRochaines briques où l'on doit vérifier si celles-ci tombent / sont détruites, celles qu'elle supportent tombent également
        vector<int> nextToConsider({ it.first });
        vector<int> allConsidered(nextToConsider);
        int brickTotal = 0;
        while (nextToConsider.size() > 0) 
        {
            vector<int> next;
            //Pour chacune des briques à considérer
            for (auto& itt : nextToConsider) 
            {
                //Si la brique n'est pas dans "tree", elle n'a pas de successeur
                if (tree.count(itt) == 0) continue;
                //Pour chacune des briques enfant de cette brique
                for (auto& child : tree[itt]) 
                {
                    //On regarde si tous les parents de la brique sont en train de chuter ou non
                    bool allFalling = true;
                    for (auto& parent : parents_tree[child]) 
                    {
                        //Si un des parents n'est pas dans les briques considérées en chute, alors la brique ne tombera pas, on la passe
                        if (find(allConsidered.begin(), allConsidered.end(), parent) == allConsidered.end()) {
                            allFalling = false;
                            break;
                        }
                    }
                    //Si on a au moins une brique parente ne tombant pas, on ne prend pas en compte cet enfant
                    if (allFalling == false) continue;
                    //On fait attention à ne pas avoir plusieurs fois les mêmes briques dans notre liste de briques considérées
                    if (find(allConsidered.begin(), allConsidered.end(), child) != allConsidered.end()) continue;
                    //Si tous les parents de la brique sont en chute / détruits, alors la brique sera en chute également
                    brickTotal++;
                    total_2++;
                    //On ajoute ensuite cette brique à la liste des prochaines briques en chute à considérer
                    next.push_back(child);
                    allConsidered.push_back(child);
                }
            }
            nextToConsider = vector<int>(next);
        }
        cout << lookup_table[it.first].first[0] << " " << lookup_table[it.first].first[1] << " " << lookup_table[it.first].first[2] << " : " << brickTotal << endl;


        //Méthode 2
        //Force brute
        //On applique la même méthode que pour la partie 1 mais en retirant une brique et en comparant les briques
        //for (int i = 0; i < 10; i++) {
        //    for (int j = 0; j < 10; j++) {
        //        //La hauteur minimale possible est 1
        //        minHeight[{i, j}] = { 1,0 };
        //    }
        //}
        //map<int, vector<int>> newPlacement;
        //unique_index = 1;
        //for (auto& itt : bricks)
        //{
        //    //On ne considère pas la brique que l'on veut exclure
        //    if (itt.first == lookup_table[it.first].first) {
        //        unique_index++;
        //        continue;
        //    }
        //    map<int, bool> supports;
        //    //On place tout d'abord les briques à leur placement final
        //    //On regarde, pour toute la zone couverte par la brique, sur quelle hauteur elle va se placer
        //    //La hauteur sera toujours la plus grande valeur trouvée sur cette zone
        //    int newHeight = 1;
        //    for (int i = 0; i <= itt.second[0]; i++) {
        //        for (int j = 0; j <= itt.second[1]; j++) {
        //            //On garde en mémoire toutes les briques supportant la nouvelle brique
        //            if (minHeight[{itt.first[0] + i, itt.first[1] + j}].first > newHeight) {
        //                newHeight = minHeight[{itt.first[0] + i, itt.first[1] + j}].first;
        //                supports.clear();
        //                supports[minHeight[{itt.first[0] + i, itt.first[1] + j}].second] = true;
        //            }
        //            else if (minHeight[{itt.first[0] + i, itt.first[1] + j}].first == newHeight) {
        //                supports[minHeight[{itt.first[0] + i, itt.first[1] + j}].second] = true;
        //            }
        //        }
        //    }
        //    //On met à jour la hauteur de la brique
        //    itt.first[2] = newHeight;

        //    //On met ensuite la nouvelle hauteur minimale possible sur la zone, qui est donc le max de hauteur min précédente + hauteur brique + 1
        //    for (int i = 0; i <= itt.second[0]; i++) {
        //        for (int j = 0; j <= itt.second[1]; j++) {
        //            minHeight[{itt.first[0] + i, itt.first[1] + j}].first = newHeight + 1 + itt.second[2];
        //        }
        //    }
        //    newPlacement[unique_index] = itt.first;
        //    unique_index++;
        //}

        //for (auto& itt : newPlacement) {
        //    if (placement[itt.first] != itt.second) 
        //        total_2++;
        //}



    }
    //Note : les briques qui ne supportent pas d'autres briques, une fois désintégrées, ne feront pas tomber d'autres briques
    //Elles ajouteront donc 0 au total, ce qui fait que leur non présence dans le "tree" n'a pas d'importance

    //DEBUG
   /* sort(bricks.begin(), bricks.end(), [](const pair<vector<int>, vector<int>>& left, const pair<vector<int>, vector<int>>& right) {return left.first[2] < right.first[2]; });
    int z = 1;
    vector<string> out_x, out_y;
    vector<vector<string>> out_z;
    for (int i = 0; i < 350; i++) {
        out_x.push_back("..........");
        out_y.push_back("..........");
        vector<string> cur_z;
        for (int j = 0; j < 10; j++) {
            cur_z.push_back("..........");
        }
        out_z.push_back(cur_z);
    }
    char c = 'A'; string safe;
    for (auto& it : bricks) 
    {
        
        if (it.first[2] > 2) c = '#';
        else {
            c++;
        }
        for (int i = 0; i <= it.second[0]; i++) {
            out_x[it.first[2]][it.first[0] + i] = '#';
            out_z[it.first[2]][it.first[1]][it.first[0] + i] = c;
        }
        for (int j = 0; j <= it.second[1]; j++) {
            out_y[it.first[2]][it.first[1] + j] = '#';
            out_z[it.first[2]][it.first[1]+j][it.first[0]] = c;
        }
        for (int k = 0; k <= it.second[2]; k++) {
            out_x[it.first[2] + k][it.first[0]] = '#';
            out_y[it.first[2] + k][it.first[1]] = '#';
            out_z[it.first[2]+k][it.first[1]][it.first[0]] = c;
        }
    }
    cout << "x-z" << endl;
    for (int i = 0; i < out_x.size(); i++) {
        cout << out_x[i] << endl;
    }
    cout << "y-z" << endl;
    for (int i = 0; i < out_y.size(); i++) {
        cout << out_y[i] << endl;
    }
    cout << "x - y" << endl;
    cout << safe << endl;
    for (auto& it : out_z) {
        for (int i = it.size() - 1; i >= 0; i--) {
            cout << it[i] << endl;
        }
        cout << endl;
    }*/
    //END DEBUG

    cout << "Part one : " << total << endl;
    cout << "Part two : " << total_2 << endl;

}


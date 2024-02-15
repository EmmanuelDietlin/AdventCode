// AdventCode_21.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

using namespace std;

int main()
{
    ifstream input;
    input.open("input.txt");
    string line;
    vector<string> gardenMap;
    map<pair<int, int>, int> pathways;
    vector<pair<int, int>> points;
    int index = 0;
    while (getline(input, line)) 
    {
        //PART ONE
        //On récupère également la position du centre (indiqué par S)
        /*gardenMap.push_back(line);
        if (line.find('S') != string::npos) {
            points.push_back({ line.find('S'), index });
        }
        index++;*/

        //PART TWO
        //On va ajouter deux fois la figure de chaque côté du centre
        //On prend la position du 'S', qu'on devra cependant penser à modifier pour prendre en compte le fait qu'on a ajouté des patterns sur tous les côtés
        if (line.find('S') != string::npos) {
            points.push_back({ line.find('S'), index });
        }
        line = line + line + line + line + line;
        gardenMap.push_back(line);
        index++;
    }
    input.close();

    //PART TWO
    //On agrandit la figure
    vector<string> bigGardenMap;
    for (int i = 0; i < 5; i++) {
        for (auto& it : gardenMap) {
            bigGardenMap.push_back(it);
        }
    }

    points[0].first += 2 * 131;
    points[0].second += 2 * 131;

    //DEBUG
    //points[0] = { 65,65 };

    //Le départ est déja atteint au début
    pathways[points[0]] = 0;

    //On va déjà exclure du compte tous les points non accessibles en 64 pas sans obstacles
    //C'est-à-dire tous ceux éloignés de plus de 64 cases du point de départ
    //En partant du départ, on va, pour chaque case, déterminer en combien de pas on peut l'atteindre
    //Ensuite, il nous suffira de prendre toutes les cases que l'on peut atteindre en un nomnbre PAIR de pas, <= 64
    //En effet, pour toutes les cases atteintes en un nombre impair de pas, il restera alors un nombre impair de pas à effectuer
    //Or, pour pouvoir terminer sur une case, il faut pouvoir effectuer un nombre PAIR de pas, un multiple de 2 qui est le nombre minimal de pas pour partir et revenir sur la case

    //Pour la longueur du chemin
    //On prend les points adjacents dans les 4 directions d'un point, et on calcule la longueur du chemin pour chacun
    //PART ONE
    //while (points.size() > 0) 
    //{
    //    vector<pair<int, int>> nextPoints;
    //    pair<int, int> newPoint;
    //    for (auto& point : points) {
    //        //On regarde dans les 4 directions autour du point si on a un point qui n'a pas encore été calculé
    //        //Et si le point est un jardin
    //        newPoint = pair<int, int>({ point.first - 1, point.second });
    //        if (point.first > 0 && pathways.count(newPoint) == 0 && gardenMap[newPoint.second][newPoint.first] != '#')
    //        {
    //            pathways[newPoint] = pathways[point] + 1;
    //            nextPoints.push_back(newPoint);
    //        }
    //        newPoint = pair<int, int>({ point.first + 1, point.second });
    //        if (point.first < gardenMap[0].length() - 1 && pathways.count(newPoint) == 0 && gardenMap[newPoint.second][newPoint.first] != '#')
    //        {
    //            pathways[newPoint] = pathways[point] + 1;
    //            nextPoints.push_back(newPoint);
    //        }
    //        newPoint = pair<int, int>({ point.first, point.second - 1 });
    //        if (point.second > 0 && pathways.count(newPoint) == 0 && gardenMap[newPoint.second][newPoint.first] != '#')
    //        {
    //            pathways[newPoint] = pathways[point] + 1;
    //            nextPoints.push_back(newPoint);
    //        }
    //        newPoint = pair<int, int>({ point.first, point.second + 1 });
    //        if (point.second < gardenMap.size() - 1 && pathways.count(newPoint) == 0 && gardenMap[newPoint.second][newPoint.first] != '#')
    //        {
    //            pathways[newPoint] = pathways[point] + 1;
    //            nextPoints.push_back(newPoint);
    //        }
    //    }
    //    points = vector<pair<int, int>>(nextPoints);
    //    nextPoints.clear();
    //}
    //long count = 0;
    //for (auto& it : pathways) {
    //    if (it.second <= 64 && it.second % 2 == 0) {
    //        count++;
    //    }
    //}
    //cout << count << endl;

    //PART TWO
    //Le pattern se répète (géométrie pacman) dans toutes les directions
    //Si jamais on atteint la limite à droite, on se retrouve à gauche et inversement
    //On a déjà 65 éléments de rayon dans notre figure

    while (points.size() > 0) 
    {
        vector<pair<int, int>> nextPoints;
        pair<int, int> newPoint;
        for (auto& point : points) {
            //On regarde dans les 4 directions autour du point si on a un point qui n'a pas encore été calculé
            //Et si le point est un jardin
            newPoint = pair<int, int>({ point.first - 1, point.second });
            if (point.first > 0 && pathways.count(newPoint) == 0 && bigGardenMap[newPoint.second][newPoint.first] != '#')
            {
                pathways[newPoint] = pathways[point] + 1;
                nextPoints.push_back(newPoint);
            }
            newPoint = pair<int, int>({ point.first + 1, point.second });
            if (point.first < bigGardenMap[0].length() - 1 && pathways.count(newPoint) == 0 && bigGardenMap[newPoint.second][newPoint.first] != '#')
            {
                pathways[newPoint] = pathways[point] + 1;
                nextPoints.push_back(newPoint);
            }
            newPoint = pair<int, int>({ point.first, point.second - 1 });
            if (point.second > 0 && pathways.count(newPoint) == 0 && bigGardenMap[newPoint.second][newPoint.first] != '#')
            {
                pathways[newPoint] = pathways[point] + 1;
                nextPoints.push_back(newPoint);
            }
            newPoint = pair<int, int>({ point.first, point.second + 1 });
            if (point.second < bigGardenMap.size() - 1 && pathways.count(newPoint) == 0 && bigGardenMap[newPoint.second][newPoint.first] != '#')
            {
                pathways[newPoint] = pathways[point] + 1;
                nextPoints.push_back(newPoint);
            }
        }
        points = vector<pair<int, int>>(nextPoints);
        nextPoints.clear();
    }

    //On va s'appuyer sur le travail effectué dans le jour 9
    //On va récupérer les 3 premiers termes 
    //Et ensuite extrapoler les termes suivants
    long count_65 = 0, count_196 = 0, count_327 = 0;
    for (auto& it : pathways) {
        if (it.second <= 65 && it.second % 2 != 0) {
            count_65++;
            //bigGardenMap[it.first.second][it.first.first] = 'O';
        }
        //Attention : on a un nombre PAIR de pas
        if (it.second <= 196 && it.second % 2 == 0) {
            count_196++;
        }
        if (it.second <= 327 && it.second % 2 != 0) {
            count_327++;
        }
    }

    //On créé les séquences permettant l'extrapolation
    vector<vector<long long>> sequences;
    vector<long long> initial({count_65, count_196, count_327});
    sequences.push_back(initial);
    bool allZeroes = false;
    index = 0;
    while (allZeroes == false) {
        vector<long long> v;
        allZeroes = true;
        for (int i = 1; i < sequences[index].size(); i++) {
            long long value = sequences[index][i] - sequences[index][i - 1];
            //cout << value << " ";
            if (value > 0 || value < 0) allZeroes = false;
            v.push_back(value);
        }
        //cout << endl;
        sequences.push_back(v);
        index++;
    }

    //On extrapole jusqu'à avoir le terme correspondant au nombre de pas
    //26501365 = 202300 * 131 + 65
    for (int i = 3; i <= 202300; i++) {
        sequences[index].push_back(0);
        for (int i = index - 2; i >= 0; i--) {
            sequences[i].push_back(sequences[i][sequences[i].size() - 1] + sequences[i + 1][sequences[i + 1].size() - 1]);
        }
        cout << 131*i + 65 << " steps : " << sequences[0][sequences[0].size() - 1] << endl;
    }
    

    cout << "TODO" << endl;

}


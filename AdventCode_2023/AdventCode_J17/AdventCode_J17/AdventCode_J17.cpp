// AdventCode_J17.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <string>
#include <map>
#include <vector>
#include <chrono>

using namespace std;
using namespace chrono;

char parent_to_point_direction(pair<int, int> parent, pair<int, int> point) 
{
    if (parent.first < point.first) return 'R';
    else if (parent.first > point.first) return 'L';
    else if (parent.second < point.second) return 'D';
    else if (parent.second > point.second) return 'U';
}

bool compare(pair<pair<char, pair<int, int>>, pair<int, pair<int, int>>>left, pair<pair<char, pair<int, int>>, pair<int, pair<int, int>>> right) {
    return left.second.first < right.second.first;
}

int main()
{
    ifstream input;
    input.open("input.txt");
    string line;
    vector<string> city_map;
    while (getline(input, line)) 
    {
        city_map.push_back(line);
    }
    input.close();
    //Points de départ et d'arrivée
    pair<int, int> start(0, 0), end(city_map[0].length() - 1, city_map.size() - 1);
    int total = 0;
    //ALGORITHME DE MOORE-DIJKSTRA POUR TROUVER LE PLUS COURT CHEMIN (= CHEMIN DE COUT LE PLUS FAIBLE)
    //NOTE : faire attention car on ne peut pas parcourir PLUS de trois blocs consécutifs dans une même direction
    //De même, on ne peut pas faire demi-tour
    //La chaleur du premier bloc ne compte pas si on n'y retourne pas
    //Ce vecteur contient les deux coordonnées du bloc, la dernière direction prise et la distance parcourue dans cette dernière direction
    //vector<pair<char, pair<int, int>>> blocks;
    //blocks.push_back({ 'V', start });
    //pair<char, pair<int, int>> pivot({'V', start});
    //bool ended = false;
    ////On stocke avec comme clé les coordonnées et comme valeur un couple de valeur distance et coordonnées parent
    //map<pair<int, int>, pair<int, pair<int, int>>> distances;
    //distances[start] = { 0, {0, 0} };

    ////Algorithme de Moore-Dijkstra
    //while (ended == false) 
    //{
    //    //Nouveau point à ajouter à l'ensemble S
    //    //C'est le point avec la plus faible distance au départ
    //    pair<int, int> newPoint(0, 0);
    //    int newPointDistance = INT_MAX;
    //    int newPointStraightPathLength = 1;
    //    
    //    char movement = 'R';

    //    //TODO
    //    //Revoir la manière dont on vérifie qu'il n'y a au maximum que 3 actions identiques dans une direction (donc max 4 à la suite)
    //    //Regarder, dans les relations parent - enfant, si on ne trouve bien que max 4 consécutifs
    //    //Si on atteint ce cap, on regarde une autre direction
    //    // Sinon, on peut également considérer de continuer dans la même direction
    //    // => enlever direction + longueur mouvement de pivot
    //    //TODO
    //    

    //    /*for (auto& it : blocks)
    //    {*/
    //        //pivot = it;

    //        //On prend le pivot comme point de départ, et on regarde tous les points autours SAUF les points déjà dans l'ensemble et les points derrière lui
    //        pair<int, int> point(0, 0), pivotPoint(0, 0);
    //        point.first = pivotPoint.first = get<0>(pivot);
    //        point.second = pivotPoint.second = get<1>(pivot);
    //        //On vérifie toutes les directions autour du point SAUF pour les points déjà dans l'ensemble et les points derrière nous
    //        switch (get<2>(pivot))
    //        {
    //            //Le dernier déplacement était vers la droite
    //        case 'R':
    //            //AU-DESSUS
    //            //Si l'élément n'est pas déjà dans la liste des sommets et qu'il existe (on n'est pas à la frontière en haut)
    //            if (point.second - 1 >= 0 && find_if(blocks.begin(), blocks.end(),
    //                [&](tuple<int, int, char, int>& element) {return (get<0>(element) == point.first && get<1>(element) == point.second - 1 && get<2>(element) == 'R'); }) == blocks.end()) {
    //                point.second -= 1;
    //                //Si la distance au centre n'existe pas (= infini) ou bien si celle calculée actuellement est inférieur à celle existante
    //                if (distances.find(point) == distances.end() || distances[point].first > distances[pivotPoint].first + (city_map[point.second][point.first] - '0')) {
    //                    distances[point].first = distances[pivotPoint].first + (city_map[point.second][point.first] - '0');
    //                    distances[point].second = pivotPoint;
    //                }
    //                if (distances[point].first < newPointDistance)
    //                {
    //                    newPoint = point;
    //                    newPointDistance = distances[point].first;
    //                    //On récupère la direction du chemin entre le parent et le point
    //                    movement = parent_to_point_direction(distances[point].second, point);
    //                    //Pour la distance du chemin, on va parcourir le point de parent en parent jusqu'à changer de direction
    //                    newPointStraightPathLength = 0;
    //                    char c = movement;
    //                    pair<int, int> parent = distances[point].second;
    //                    do
    //                    {
    //                        newPointStraightPathLength++;
    //                        if (parent == start) break;
    //                        c = parent_to_point_direction(distances[parent].second, parent);
    //                        parent = distances[parent].second;
    //                    } while (c == movement);
    //                    if (parent == start && c == movement) newPointStraightPathLength++;
    //                }

    //            }
    //            point.first = get<0>(pivot);
    //            point.second = get<1>(pivot);
    //            //EN-DESSOUS
    //            if (point.second + 1 < city_map.size() && find_if(blocks.begin(), blocks.end(),
    //                [&](tuple<int, int, char, int>& element) {return (get<0>(element) == point.first && get<1>(element) == point.second + 1 && get<2>(element) == 'R'); }) == blocks.end()) {
    //                point.second += 1;
    //                //Si la distance au centre n'existe pas (= infini) ou bien si celle calculée actuellement est inférieur à celle existante
    //                if (distances.find(point) == distances.end() || distances[point].first > distances[pivotPoint].first + (city_map[point.second][point.first] - '0')) {
    //                    distances[point].first = distances[pivotPoint].first + (city_map[point.second][point.first] - '0');
    //                    distances[point].second = pivotPoint;
    //                }
    //                if (distances[point].first < newPointDistance)
    //                {
    //                    newPoint = point;
    //                    newPointDistance = distances[point].first;
    //                    //On récupère la direction du chemin entre le parent et le point
    //                    movement = parent_to_point_direction(distances[point].second, point);
    //                    //Pour la distance du chemin, on va parcourir le point de parent en parent jusqu'à changer de direction
    //                    newPointStraightPathLength = 0;
    //                    char c = movement;
    //                    pair<int, int> parent = distances[point].second;
    //                    do
    //                    {
    //                        newPointStraightPathLength++;
    //                        if (parent == start) break;
    //                        c = parent_to_point_direction(distances[parent].second, parent);
    //                        parent = distances[parent].second;
    //                    } while (c == movement);
    //                    if (parent == start && c == movement) newPointStraightPathLength++;
    //                }

    //            }
    //            point.first = get<0>(pivot);
    //            point.second = get<1>(pivot);
    //            //A DROITE
    //            //On doit vérifier aussi qu'on a pas déjà un segment de distance 3
    //            if (point.first + 1 < city_map[0].length() && get<3>(pivot) < 3 && find_if(blocks.begin(), blocks.end(),
    //                [&](tuple<int, int, char, int>& element) {return (get<0>(element) == point.first + 1 && get<1>(element) == point.second && get<2>(element) == 'R'); }) == blocks.end()) {
    //                point.first += 1;
    //                //Si la distance au centre n'existe pas (= infini) ou bien si celle calculée actuellement est inférieur à celle existante
    //                if (distances.find(point) == distances.end() || distances[point].first > distances[pivotPoint].first + (city_map[point.second][point.first] - '0')) {
    //                    distances[point].first = distances[pivotPoint].first + (city_map[point.second][point.first] - '0');
    //                    distances[point].second = pivotPoint;
    //                }
    //                //On cherche le prochain point à ajouter à l'ensemble
    //                if (distances[point].first < newPointDistance)
    //                {
    //                    newPoint = point;
    //                    newPointDistance = distances[point].first;
    //                    //On récupère la direction du chemin entre le parent et le point
    //                    movement = parent_to_point_direction(distances[point].second, point);
    //                    //Pour la distance du chemin, on va parcourir le point de parent en parent jusqu'à changer de direction
    //                    newPointStraightPathLength = 0;
    //                    char c = movement;
    //                    pair<int, int> parent = distances[point].second;
    //                    do
    //                    {
    //                        newPointStraightPathLength++;
    //                        if (parent == start) break;
    //                        c = parent_to_point_direction(distances[parent].second, parent);
    //                        parent = distances[parent].second;
    //                    } while (c == movement);
    //                    if (parent == start && c == movement) newPointStraightPathLength++;
    //                }
    //            }
    //            break;
    //            //Le dernier déplacement était vers la gauche
    //        case 'L':
    //            //AU-DESSUS
    //            //Si l'élément n'est pas déjà dans la liste des sommets et qu'il existe (on n'est pas à la frontière en haut)
    //            if (point.second - 1 >= 0 && find_if(blocks.begin(), blocks.end(),
    //                [&](tuple<int, int, char, int>& element) {return (get<0>(element) == point.first && get<1>(element) == point.second - 1 && get<2>(element) == 'L'); }) == blocks.end()) {
    //                point.second -= 1;
    //                //Si la distance au centre n'existe pas (= infini) ou bien si celle calculée actuellement est inférieur à celle existante
    //                if (distances.find(point) == distances.end() || distances[point].first > distances[pivotPoint].first + (city_map[point.second][point.first] - '0')) {
    //                    distances[point].first = distances[pivotPoint].first + (city_map[point.second][point.first] - '0');
    //                    distances[point].second = pivotPoint;
    //                }
    //                if (distances[point].first < newPointDistance)
    //                {
    //                    newPoint = point;
    //                    newPointDistance = distances[point].first;
    //                    //On récupère la direction du chemin entre le parent et le point
    //                    movement = parent_to_point_direction(distances[point].second, point);
    //                    //Pour la distance du chemin, on va parcourir le point de parent en parent jusqu'à changer de direction
    //                    newPointStraightPathLength = 0;
    //                    char c = movement;
    //                    pair<int, int> parent = distances[point].second;
    //                    do
    //                    {
    //                        newPointStraightPathLength++;
    //                        if (parent == start) break;
    //                        c = parent_to_point_direction(distances[parent].second, parent);
    //                        parent = distances[parent].second;
    //                    } while (c == movement);
    //                    if (parent == start && c == movement) newPointStraightPathLength++;
    //                }

    //            }
    //            //EN-DESSOUS
    //            point.first = get<0>(pivot);
    //            point.second = get<1>(pivot);
    //            if (point.second + 1 < city_map.size() && find_if(blocks.begin(), blocks.end(),
    //                [&](tuple<int, int, char, int>& element) {return (get<0>(element) == point.first && get<1>(element) == point.second + 1 && get<2>(element) == 'L'); }) == blocks.end()) {
    //                point.second += 1;
    //                //Si la distance au centre n'existe pas (= infini) ou bien si celle calculée actuellement est inférieur à celle existante
    //                if (distances.find(point) == distances.end() || distances[point].first > distances[pivotPoint].first + (city_map[point.second][point.first] - '0')) {
    //                    distances[point].first = distances[pivotPoint].first + (city_map[point.second][point.first] - '0');
    //                    distances[point].second = pivotPoint;
    //                }
    //                if (distances[point].first < newPointDistance)
    //                {
    //                    newPoint = point;
    //                    newPointDistance = distances[point].first;
    //                    //On récupère la direction du chemin entre le parent et le point
    //                    movement = parent_to_point_direction(distances[point].second, point);
    //                    //Pour la distance du chemin, on va parcourir le point de parent en parent jusqu'à changer de direction
    //                    newPointStraightPathLength = 0;
    //                    char c = movement;
    //                    pair<int, int> parent = distances[point].second;
    //                    do
    //                    {
    //                        newPointStraightPathLength++;
    //                        if (parent == start) break;
    //                        c = parent_to_point_direction(distances[parent].second, parent);
    //                        parent = distances[parent].second;
    //                    } while (c == movement);
    //                    if (parent == start && c == movement) newPointStraightPathLength++;
    //                }
    //            }
    //            point.first = get<0>(pivot);
    //            point.second = get<1>(pivot);
    //            //A GAUCHE
    //            //On doit vérifier aussi qu'on a pas déjà un segment de distance 3
    //            if (point.first - 1 >= 0 && get<3>(pivot) < 3 && find_if(blocks.begin(), blocks.end(),
    //                [&](tuple<int, int, char, int>& element) {return (get<0>(element) == point.first - 1 && get<1>(element) == point.second && get<2>(element) == 'L'); }) == blocks.end()) {
    //                point.first -= 1;
    //                //Si la distance au centre n'existe pas (= infini) ou bien si celle calculée actuellement est inférieur à celle existante
    //                if (distances.find(point) == distances.end() || distances[point].first > distances[pivotPoint].first + (city_map[point.second][point.first] - '0')) {
    //                    distances[point].first = distances[pivotPoint].first + (city_map[point.second][point.first] - '0');
    //                    distances[point].second = pivotPoint;
    //                }
    //                if (distances[point].first < newPointDistance)
    //                {
    //                    newPoint = point;
    //                    newPointDistance = distances[point].first;
    //                    //On récupère la direction du chemin entre le parent et le point
    //                    movement = parent_to_point_direction(distances[point].second, point);
    //                    //Pour la distance du chemin, on va parcourir le point de parent en parent jusqu'à changer de direction
    //                    newPointStraightPathLength = 0;
    //                    char c = movement;
    //                    pair<int, int> parent = distances[point].second;
    //                    do
    //                    {
    //                        newPointStraightPathLength++;
    //                        if (parent == start) break;
    //                        c = parent_to_point_direction(distances[parent].second, parent);
    //                        parent = distances[parent].second;
    //                    } while (c == movement);
    //                    if (parent == start && c == movement) newPointStraightPathLength++;
    //                }

    //            }
    //            break;
    //            //Le dernier déplacement était vers le bas
    //        case 'D':

    //            //EN-DESSOUS
    //            //On doit vérifier aussi qu'on a pas déjà un segment de distance 3
    //            if (point.second + 1 < city_map.size() && get<3>(pivot) < 3 && find_if(blocks.begin(), blocks.end(),
    //                [&](tuple<int, int, char, int>& element) {return (get<0>(element) == point.first && get<1>(element) == point.second + 1 && get<2>(element) == 'D'); }) == blocks.end()) {
    //                point.second += 1;
    //                //Si la distance au centre n'existe pas (= infini) ou bien si celle calculée actuellement est inférieur à celle existante
    //                if (distances.find(point) == distances.end() || distances[point].first > distances[pivotPoint].first + (city_map[point.second][point.first] - '0')) {
    //                    distances[point].first = distances[pivotPoint].first + (city_map[point.second][point.first] - '0');
    //                    distances[point].second = pivotPoint;
    //                }
    //                if (distances[point].first < newPointDistance)
    //                {
    //                    newPoint = point;
    //                    newPointDistance = distances[point].first;
    //                    //On récupère la direction du chemin entre le parent et le point
    //                    movement = parent_to_point_direction(distances[point].second, point);
    //                    //Pour la distance du chemin, on va parcourir le point de parent en parent jusqu'à changer de direction
    //                    newPointStraightPathLength = 0;
    //                    char c = movement;
    //                    pair<int, int> parent = distances[point].second;
    //                    do
    //                    {
    //                        newPointStraightPathLength++;
    //                        if (parent == start) break;
    //                        c = parent_to_point_direction(distances[parent].second, parent);
    //                        parent = distances[parent].second;
    //                    } while (c == movement);
    //                    if (parent == start && c == movement) newPointStraightPathLength++;
    //                }

    //            }
    //            point.first = get<0>(pivot);
    //            point.second = get<1>(pivot);
    //            //A DROITE
    //            if (point.first + 1 < city_map[0].length() && find_if(blocks.begin(), blocks.end(),
    //                [&](tuple<int, int, char, int>& element) {return (get<0>(element) == point.first + 1 && get<1>(element) == point.second && get<2>(element) == 'R'); }) == blocks.end()) {
    //                point.first += 1;
    //                //Si la distance au centre n'existe pas (= infini) ou bien si celle calculée actuellement est inférieur à celle existante
    //                if (distances.find(point) == distances.end() || distances[point].first > distances[pivotPoint].first + (city_map[point.second][point.first] - '0')) {
    //                    distances[point].first = distances[pivotPoint].first + (city_map[point.second][point.first] - '0');
    //                    distances[point].second = pivotPoint;
    //                }
    //                if (distances[point].first < newPointDistance)
    //                {
    //                    newPoint = point;
    //                    newPointDistance = distances[point].first;
    //                    //On récupère la direction du chemin entre le parent et le point
    //                    movement = parent_to_point_direction(distances[point].second, point);
    //                    //Pour la distance du chemin, on va parcourir le point de parent en parent jusqu'à changer de direction
    //                    newPointStraightPathLength = 0;
    //                    char c = movement;
    //                    pair<int, int> parent = distances[point].second;
    //                    do
    //                    {
    //                        newPointStraightPathLength++;
    //                        if (parent == start) break;
    //                        c = parent_to_point_direction(distances[parent].second, parent);
    //                        parent = distances[parent].second;
    //                    } while (c == movement);
    //                    if (parent == start && c == movement) newPointStraightPathLength++;
    //                }

    //            }
    //            point.first = get<0>(pivot);
    //            point.second = get<1>(pivot);
    //            //A GAUCHE
    //            if (point.first - 1 >= 0 && find_if(blocks.begin(), blocks.end(),
    //                [&](tuple<int, int, char, int>& element) {return (get<0>(element) == point.first - 1 && get<1>(element) == point.second && get<2>(element) == 'L'); }) == blocks.end()) {
    //                point.first -= 1;
    //                //Si la distance au centre n'existe pas (= infini) ou bien si celle calculée actuellement est inférieur à celle existante
    //                if (distances.find(point) == distances.end() || distances[point].first > distances[pivotPoint].first + (city_map[point.second][point.first] - '0')) {
    //                    distances[point].first = distances[pivotPoint].first + (city_map[point.second][point.first] - '0');
    //                    distances[point].second = pivotPoint;
    //                }
    //                if (distances[point].first < newPointDistance)
    //                {
    //                    newPoint = point;
    //                    newPointDistance = distances[point].first;
    //                    //On récupère la direction du chemin entre le parent et le point
    //                    movement = parent_to_point_direction(distances[point].second, point);
    //                    //Pour la distance du chemin, on va parcourir le point de parent en parent jusqu'à changer de direction
    //                    newPointStraightPathLength = 0;
    //                    char c = movement;
    //                    pair<int, int> parent = distances[point].second;
    //                    do
    //                    {
    //                        newPointStraightPathLength++;
    //                        if (parent == start) break;
    //                        c = parent_to_point_direction(distances[parent].second, parent);
    //                        parent = distances[parent].second;
    //                    } while (c == movement);
    //                    if (parent == start && c == movement) newPointStraightPathLength++;
    //                }

    //            }
    //            break;
    //            //Le dernier déplacement était vers le haut
    //        case 'U':

    //            //AU-DESSUS
    //            //On doit vérifier aussi qu'on a pas déjà un segment de distance 3
    //            if (point.second - 1 >= 0 && get<3>(pivot) < 3 && find_if(blocks.begin(), blocks.end(),
    //                [&](tuple<int, int, char, int>& element) {return (get<0>(element) == point.first && get<1>(element) == point.second - 1 && get<2>(element) == 'U'); }) == blocks.end()) {
    //                point.second -= 1;
    //                //Si la distance au centre n'existe pas (= infini) ou bien si celle calculée actuellement est inférieur à celle existante
    //                if (distances.find(point) == distances.end() || distances[point].first > distances[pivotPoint].first + (city_map[point.second][point.first] - '0')) {
    //                    distances[point].first = distances[pivotPoint].first + (city_map[point.second][point.first] - '0');
    //                    distances[point].second = pivotPoint;
    //                }
    //                if (distances[point].first < newPointDistance)
    //                {
    //                    newPoint = point;
    //                    newPointDistance = distances[point].first;
    //                    //On récupère la direction du chemin entre le parent et le point
    //                    movement = parent_to_point_direction(distances[point].second, point);
    //                    //Pour la distance du chemin, on va parcourir le point de parent en parent jusqu'à changer de direction
    //                    newPointStraightPathLength = 0;
    //                    char c = movement;
    //                    pair<int, int> parent = distances[point].second;
    //                    do
    //                    {
    //                        newPointStraightPathLength++;
    //                        if (parent == start) break;
    //                        c = parent_to_point_direction(distances[parent].second, parent);
    //                        parent = distances[parent].second;
    //                    } while (c == movement);
    //                    if (parent == start && c == movement) newPointStraightPathLength++;
    //                }

    //            }
    //            point.first = get<0>(pivot);
    //            point.second = get<1>(pivot);
    //            //A DROITE
    //            if (point.first + 1 < city_map[0].length() && find_if(blocks.begin(), blocks.end(),
    //                [&](tuple<int, int, char, int>& element) {return (get<0>(element) == point.first + 1 && get<1>(element) == point.second && get<2>(element) == 'R'); }) == blocks.end()) {
    //                point.first += 1;
    //                //Si la distance au centre n'existe pas (= infini) ou bien si celle calculée actuellement est inférieur à celle existante
    //                if (distances.find(point) == distances.end() || distances[point].first > distances[pivotPoint].first + (city_map[point.second][point.first] - '0')) {
    //                    distances[point].first = distances[pivotPoint].first + (city_map[point.second][point.first] - '0');
    //                    distances[point].second = pivotPoint;
    //                }
    //                if (distances[point].first < newPointDistance)
    //                {
    //                    newPoint = point;
    //                    newPointDistance = distances[point].first;
    //                    //On récupère la direction du chemin entre le parent et le point
    //                    movement = parent_to_point_direction(distances[point].second, point);
    //                    //Pour la distance du chemin, on va parcourir le point de parent en parent jusqu'à changer de direction
    //                    newPointStraightPathLength = 0;
    //                    char c = movement;
    //                    pair<int, int> parent = distances[point].second;
    //                    do
    //                    {
    //                        newPointStraightPathLength++;
    //                        if (parent == start) break;
    //                        c = parent_to_point_direction(distances[parent].second, parent);
    //                        parent = distances[parent].second;
    //                    } while (c == movement);
    //                    if (parent == start && c == movement) newPointStraightPathLength++;
    //                }

    //            }
    //            point.first = get<0>(pivot);
    //            point.second = get<1>(pivot);
    //            //A GAUCHE
    //            if (point.first - 1 >= 0 && find_if(blocks.begin(), blocks.end(),
    //                [&](tuple<int, int, char, int>& element) {return (get<0>(element) == point.first - 1 && get<1>(element) == point.second && get<2>(element) == 'L'); }) == blocks.end()) {
    //                point.first -= 1;
    //                //Si la distance au centre n'existe pas (= infini) ou bien si celle calculée actuellement est inférieur à celle existante
    //                if (distances.find(point) == distances.end() || distances[point].first > distances[pivotPoint].first + (city_map[point.second][point.first] - '0')) {
    //                    distances[point].first = distances[pivotPoint].first + (city_map[point.second][point.first] - '0');
    //                    distances[point].second = pivotPoint;
    //                }
    //                if (distances[point].first < newPointDistance)
    //                {
    //                    newPoint = point;
    //                    newPointDistance = distances[point].first;
    //                    //On récupère la direction du chemin entre le parent et le point
    //                    movement = parent_to_point_direction(distances[point].second, point);
    //                    //Pour la distance du chemin, on va parcourir le point de parent en parent jusqu'à changer de direction
    //                    newPointStraightPathLength = 0;
    //                    char c = movement;
    //                    pair<int, int> parent = distances[point].second;
    //                    do
    //                    {
    //                        newPointStraightPathLength++;
    //                        if (parent == start) break;
    //                        c = parent_to_point_direction(distances[parent].second, parent);
    //                        parent = distances[parent].second;
    //                    } while (c == movement);
    //                    if (parent == start && c == movement) newPointStraightPathLength++;
    //                }

    //            }
    //            break;
    //        default:
    //            break;
    //        }
    //    //}
    //    //}
    // 
    // 
    // 
    // 
    // TODO : 
    // Déterminer le nouveau pivot A LA FIN
    // En regardant dans la liste des distances laquelle est la plus petite (pour tout élément n'appartenant pas à l'ensemble
    // TODO 
    //
    // 
    // 
    // 
    // 
    //        movement = parent_to_point_direction(distances[newPoint].second, newPoint);
    //    tuple<int, int, char, int> nextPoint({ newPoint.first, newPoint.second, movement, newPointStraightPathLength });
    //    blocks.push_back(nextPoint);
    //    pivot = nextPoint;

    //    //Finir quand le sommet de fin est ajouté à la liste des blocs
    //    if (find_if(blocks.begin(), blocks.end(),
    //        [&](tuple<int, int, char, int>& element) {return (get<0>(element) == end.first && get<1>(element) == end.second); }) != blocks.end()) {
    //        ended = true;
    //    }
    //    for (int i = 0; i < city_map.size(); i++) {
    //        for (int j = 0; j < city_map[0].length(); j++)
    //        {
    //            if (find_if(blocks.begin(), blocks.end(),
    //                [&](tuple<int, int, char, int>& element) {return (get<0>(element) == j && get<1>(element) == i); }) != blocks.end()) {
    //                /* int index = find_if(blocks.begin(), blocks.end(),
    //                        [&](tuple<int, int, char, int>& element) {return (get<0>(element) == j && get<1>(element) == i); }) - blocks.begin();*/
    //                cout << '#';
    //            }
    //            else cout << city_map[i][j];
    //        }
    //        cout << endl;
    //    }
    //    cout << endl;
    //    
    //}
    

    //DONNE LE BON RESULTAT    

    //On transforme le problème de façon à pouvoir appliquer simplement l'algo de Moore-Dijkstra
    //Chacune des cases de la map va être considérée comme étant liée à d'autres cases de la map par des chemins soit HORIZONTAUX soit VERTICAUX
    //Les poids de ces chemins correspondent aux pertes cumulées des cases sur le chemin entre les deux cases (case d'arrivée incluse et case de départ exclue)
    //Si le chemin HORIZONTAL est choisi entre deux cases, alors le prochain chemin devra être considéré parmi les chemins VERTICAUX (sans quoi on pourrait se retrouver avec des chemins de longueur > 3)
    //Ainsi, une case prise au hasard au sein de la map (en excluant les cas limites) peut relier et à donc des chemins vers 6 cases à l'horizontale (x-3,y; x-2;y, x-1,y;x+1,y;x+2,y;x+3,y)
    //et des chemins vers 6 cases à la verticale (x,y-3;x,y-2;x,y-1;x,y+1;x,y+2;x,y+3)
    //Ainsi, on a alors un graphe où chaque case est représentée par deux nodes, une verticale et une horizontale, et chacune des verticale est reliée à une horizontale et inversement
    //On stocke donc dans le tableau des blocks, en plus de la position, la direction de la node (H pour horizontale ou V pour verticale)    
    
    vector<pair<char, pair<int, int>>> blocks;
    blocks.push_back({ 'V', start });
    pair<char, pair<int, int>> pivot({ 'V', start });
    bool ended = false;
    //On stocke avec comme clé les coordonnées et comme valeur un couple de valeur distance et coordonnées parent
    map<pair<char, pair<int, int>>, pair<int, pair<int, int>>> distances;

    map<pair<char, pair<int, int>>, pair<int, pair<int, int>>> tmp;

    //ALGO TROP LENT => TROUVER UN MOYEN D'ACCELERER
    //La lenteur est sur la recherche du nouveau pivot

   /* for (int i = 0; i < city_map[0].length(); i++) {
        for (int j = 0; j < city_map.size(); j++) {
            distances[{'V', { i,j }}] = { INT_MAX, start };
            distances[{'H', { i,j }}] = { INT_MAX, start };
        }
    }*/
    distances[pivot] = { 0, {0, 0} };
    distances[{'H', start}] = {0, {0, 0}};

    tmp[pivot] = distances[pivot];
    tmp[{'H', start}] = distances[{'H', start}];

    vector<string> iterate_map(city_map);
    iterate_map[0][0] = '#';
    
    //PART ONE
    //while (ended == false) 
    //{
    //    auto start_1 = high_resolution_clock::now();
    //    //Si on est sur un pivot vertical
    //    if (pivot.first == 'V') {
    //        //On regarde toutes les cases en haut à portée
    //        for (int i = -1; i >= -3; i--) {
    //            if (pivot.second.second + i < 0) continue;
    //            //Le point suivant sera une node horizontale
    //            pair<char, pair<int, int>> point({ 'H', {pivot.second.first, pivot.second.second + i} });
    //            //Si la node est déjà dans l'ensemble, on ne la considère pas
    //            //On ne considère que les points à la frontière du pivot mais pas dans l'ensemble
    //            if (find(blocks.begin(), blocks.end(), point) != blocks.end()) continue;
    //            int dist = 0;
    //            for (int j = -1; j >= i; j--) {
    //                dist += (city_map[pivot.second.second + j][pivot.second.first] - '0');
    //            }
    //            if (distances.count(point) == 0 || dist + distances[pivot].first < distances[point].first) {
    //                distances[point].second = pivot.second;
    //                distances[point].first = distances[pivot].first + dist;
    //            }
    //        }
    //        //On regarde toutes les cases en bas à portée
    //        for (int i = 1; i <= 3; i++) {
    //            if (pivot.second.second + i >= city_map.size()) continue;
    //            //Le point suivant sera une node horizontale
    //            pair<char, pair<int, int>> point({ 'H', {pivot.second.first, pivot.second.second + i} });
    //            //Si la node est déjà dans l'ensemble, on ne la considère pas
    //            //On ne considère que les points à la frontière du pivot mais pas dans l'ensemble
    //            if (find(blocks.begin(), blocks.end(), point) != blocks.end()) continue;
    //            int dist = 0;
    //            for (int j = 1; j <= i; j++) {
    //                dist += (city_map[pivot.second.second + j][pivot.second.first] - '0');
    //            }
    //            if (distances.count(point) == 0 || dist + distances[pivot].first < distances[point].first) {
    //                distances[point].second = pivot.second;
    //                distances[point].first = distances[pivot].first + dist;
    //            }
    //        }
    //    }
    //    //Si on est sur un pivot horizontal
    //    else {
    //        //On regarde toutes les cases à gauche à portée
    //        for (int i = -1; i >= -3; i--) {
    //            if (pivot.second.first + i < 0) continue;
    //            //Le point suivant sera une node verticale
    //            pair<char, pair<int, int>> point({ 'V', {pivot.second.first + i, pivot.second.second} });
    //            //Si la node est déjà dans l'ensemble, on ne la considère pas
    //            //On ne considère que les points à la frontière du pivot mais pas dans l'ensemble
    //            if (find(blocks.begin(), blocks.end(), point) != blocks.end()) continue;
    //            int dist = 0;
    //            for (int j = -1; j >= i; j--) {
    //                dist += (city_map[pivot.second.second][pivot.second.first + j] - '0');
    //            }
    //            if (distances.count(point) == 0 || dist + distances[pivot].first < distances[point].first) {
    //                distances[point].second = pivot.second;
    //                distances[point].first = distances[pivot].first + dist;
    //            }
    //        }
    //        //On regarde toutes les cases à droite à portée
    //        for (int i = 1; i <= 3; i++) {
    //            if (pivot.second.first + i >= city_map[0].length()) continue;
    //            //Le point suivant sera une node verticale
    //            pair<char, pair<int, int>> point({ 'V', {pivot.second.first + i, pivot.second.second} });
    //            //Si la node est déjà dans l'ensemble, on ne la considère pas
    //            //On ne considère que les points à la frontière du pivot mais pas dans l'ensemble
    //            if (find(blocks.begin(), blocks.end(), point) != blocks.end()) continue;
    //            int dist = 0;
    //            for (int j = 1; j <= i; j++) {
    //                dist += (city_map[pivot.second.second][pivot.second.first + j] - '0');
    //            }
    //            if (distances.count(point) == 0 || dist + distances[pivot].first < distances[point].first) {
    //                distances[point].second = pivot.second;
    //                distances[point].first = distances[pivot].first + dist;
    //            }
    //        }
    //    }
    //    auto start_2 = high_resolution_clock::now();
    //    pair<char, pair<int, int>> newPivot;
    //    int distance = INT_MAX;

    //    distances.erase(pivot);
    //    //distances[pivot].first = INT_MAX;
    //    pivot = min_element(distances.begin(), distances.end(), compare)->first;

    //    //for (auto& it : distances) 
    //    //{
    //    //    //if (find(blocks.begin(), blocks.end(), it.first) != blocks.end()) continue;
    //    //    if (it.second.first < distance) {
    //    //        newPivot = it.first;
    //    //        distance = it.second.first;
    //    //    }
    //    //}
    //    //pivot = newPivot;
    //    blocks.push_back(pivot);
    //    auto start_3 = high_resolution_clock::now();
    //    cout << duration_cast<microseconds>(start_2 - start_1).count() << endl;
    //    cout << duration_cast<microseconds>(start_3 - start_2).count() << endl;
    //    if (pivot.second == end) break;
    //   /* iterate_map[pivot.second.second][pivot.second.first] = '#';
    //    for (auto& it : iterate_map) {
    //        cout << it << endl;
    //    }
    //    cout << endl;*/
    //}
    
    //PART TWO
    while (ended == false) 
        {
            auto start_1 = high_resolution_clock::now();
            //Si on est sur un pivot vertical
            if (pivot.first == 'V') {
                //On regarde toutes les cases en haut à portée
                for (int i = -4; i >= -10; i--) {
                    if (pivot.second.second + i < 0) continue;
                    //Le point suivant sera une node horizontale
                    pair<char, pair<int, int>> point({ 'H', {pivot.second.first, pivot.second.second + i} });
                    //Si la node est déjà dans l'ensemble, on ne la considère pas
                    //On ne considère que les points à la frontière du pivot mais pas dans l'ensemble
                    if (find(blocks.begin(), blocks.end(), point) != blocks.end()) continue;
                    int dist = 0;
                    //On ne peut tourner qu'au bout de 4 blocks consécutifs, mais il faut bien compter la perte des 3 premières cases
                    for (int j = -1; j >= i; j--) {
                        dist += (city_map[pivot.second.second + j][pivot.second.first] - '0');
                    }
                    if (distances.count(point) == 0 || dist + distances[pivot].first < distances[point].first) {
                        distances[point].second = pivot.second;
                        distances[point].first = distances[pivot].first + dist;
                        tmp[point] = distances[point];
                    }
                }
                //On regarde toutes les cases en bas à portée
                for (int i = 4; i <= 10; i++) {
                    if (pivot.second.second + i >= city_map.size()) continue;
                    //Le point suivant sera une node horizontale
                    pair<char, pair<int, int>> point({ 'H', {pivot.second.first, pivot.second.second + i} });
                    //Si la node est déjà dans l'ensemble, on ne la considère pas
                    //On ne considère que les points à la frontière du pivot mais pas dans l'ensemble
                    if (find(blocks.begin(), blocks.end(), point) != blocks.end()) continue;
                    int dist = 0;
                    //On ne peut tourner qu'au bout de 4 blocks consécutifs, mais il faut bien compter la perte des 3 premières cases
                    for (int j = 1; j <= i; j++) {
                        dist += (city_map[pivot.second.second + j][pivot.second.first] - '0');
                    }
                    if (distances.count(point) == 0 || dist + distances[pivot].first < distances[point].first) {
                        distances[point].second = pivot.second;
                        distances[point].first = distances[pivot].first + dist;
                        tmp[point] = distances[point];
                    }
                }
            }
            //Si on est sur un pivot horizontal
            else {
                //On regarde toutes les cases à gauche à portée
                for (int i = -4; i >= -10; i--) {
                    if (pivot.second.first + i < 0) continue;
                    //Le point suivant sera une node verticale
                    pair<char, pair<int, int>> point({ 'V', {pivot.second.first + i, pivot.second.second} });
                    //Si la node est déjà dans l'ensemble, on ne la considère pas
                    //On ne considère que les points à la frontière du pivot mais pas dans l'ensemble
                    if (find(blocks.begin(), blocks.end(), point) != blocks.end()) continue;
                    int dist = 0;
                    //On ne peut tourner qu'au bout de 4 blocks consécutifs, mais il faut bien compter la perte des 3 premières cases
                    for (int j = -1; j >= i; j--) {
                        dist += (city_map[pivot.second.second][pivot.second.first + j] - '0');
                    }
                    if (distances.count(point) == 0 || dist + distances[pivot].first < distances[point].first) {
                        distances[point].second = pivot.second;
                        distances[point].first = distances[pivot].first + dist;
                        tmp[point] = distances[point];
                    }
                }
                //On regarde toutes les cases à droite à portée
                for (int i = 4; i <= 10; i++) {
                    if (pivot.second.first + i >= city_map[0].length()) continue;
                    //Le point suivant sera une node verticale
                    pair<char, pair<int, int>> point({ 'V', {pivot.second.first + i, pivot.second.second} });
                    //Si la node est déjà dans l'ensemble, on ne la considère pas
                    //On ne considère que les points à la frontière du pivot mais pas dans l'ensemble
                    if (find(blocks.begin(), blocks.end(), point) != blocks.end()) continue;
                    int dist = 0;
                    //On ne peut tourner qu'au bout de 4 blocks consécutifs, mais il faut bien compter la perte des 3 premières cases
                    for (int j = 1; j <= i; j++) {
                        dist += (city_map[pivot.second.second][pivot.second.first + j] - '0');
                    }
                    if (distances.count(point) == 0 || dist + distances[pivot].first < distances[point].first) {
                        distances[point].second = pivot.second;
                        distances[point].first = distances[pivot].first + dist;
                        tmp[point] = distances[point];
                    }
                }
            }
            auto start_2 = high_resolution_clock::now();
            pair<char, pair<int, int>> newPivot;
            int distance = INT_MAX;

            distances.erase(pivot);
            //distances[pivot].first = INT_MAX;
            pivot = min_element(distances.begin(), distances.end(), compare)->first;

            //for (auto& it : distances) 
            //{
            //    //if (find(blocks.begin(), blocks.end(), it.first) != blocks.end()) continue;
            //    if (it.second.first < distance) {
            //        newPivot = it.first;
            //        distance = it.second.first;
            //    }
            //}
            //pivot = newPivot;
            blocks.push_back(pivot);
            auto start_3 = high_resolution_clock::now();
            /*cout << duration_cast<microseconds>(start_2 - start_1).count() << endl;
            cout << duration_cast<microseconds>(start_3 - start_2).count() << endl;*/
            if (pivot.second == end) break;
           /* iterate_map[pivot.second.second][pivot.second.first] = '#';
            for (auto& it : iterate_map) {
                cout << it << endl;
            }
            cout << endl;*/
        }

    
    cout << "end" << endl;
    if (distances.find({ 'V', end }) == distances.end()) {
        cout << distances[{ 'H', end }].first << endl;
    }
    else if (distances.find({ 'H', end }) == distances.end()) cout << distances[{ 'V', end }].first << endl;
    else 
    {
        cout << min(distances[{ 'H', end }].first, distances[{ 'V', end }].first) << endl;
    }
    //cout << distances[end].first << endl;
}

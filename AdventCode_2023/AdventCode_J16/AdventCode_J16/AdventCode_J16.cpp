// AdventCode_J16.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cmath>

using namespace std;

map<char, pair<int, int>> directions
{
    {'U', {0,-1}},
    {'D', {0,1}},
    {'L', {-1,0}},
    {'R', {1,0}}
};

int main()
{
    ifstream input;
    input.open("input.txt");
    int total = 0;
    string line;
    vector<string> pattern;
    while (getline(input, line)) 
    {
        pattern.push_back(line);
    }
    input.close();
    //Ce vecteur stocke la/les directions du rayon  ainsi que sa/ses positions actuelles
    //vector<pair<char, pair<int, int>>> light_directions;
    
    //Stocke les différentes cases parcourues, ainsi que la direction de la lumière lorsqu'elle arrive sur cette case
    //Cela permet à la fois de ne compter qu'une seule fois les cases où l'on serait passé plusieurs fois mais de côtés différents
    //Mais également de s'arrêter en cas de boucle, dès que l'on détecte que l'on repasse sur une même case dans la même direction que précédemment
    //map<pair<int, int>, vector<char>> energized;

    //PART ONE
    //Le rayon commence en haut à gauche vers la droite
    //On le place donc à -1.0 afin de vérifier la première case en haut à gauche lors de la première itération (case 0,0)
    //light_directions.push_back({ 'R', {-1,0} });

    //PART TWO
    //On va parcourir toutes les possibilités de commencer à gauche et à droite (vers respectivement la droite et la gauche)

    //TODO : 
    // POUR ACCELERER LE PROCESSUS
    //Utiliser la mise en cache des cases déjà parcourues afin de ne pas avoir à recalculer des patterns déjà calculés

    for (int h = 0; h < pattern.size(); h++) 
    {
        cout << h << endl;
        vector<pair<char, pair<int, int>>> light_dirs_left;
        //On commence tout à gauche
        light_dirs_left.push_back({ 'R', {-1,h} });
        vector<pair<char, pair<int, int>>> light_dirs_right;
        //On commence tout à droite
        light_dirs_right.push_back({ 'L', {pattern[0].size(),h}});
        //Stocke les différentes cases parcourues, ainsi que la direction de la lumière lorsqu'elle arrive sur cette case
        //Cela permet à la fois de ne compter qu'une seule fois les cases où l'on serait passé plusieurs fois mais de côtés différents
        //Mais également de s'arrêter en cas de boucle, dès que l'on détecte que l'on repasse sur une même case dans la même direction que précédemment
        map<pair<int, int>, vector<char>> energized;

        //Tant qu'on a encore des directions à parcourir
        while (light_dirs_left.size() > 0)
        {
            vector<pair<char, pair<int, int>>> nextPositions;
            for (auto& it : light_dirs_left)
            {
                //On calcule la nouvelle position du rayon selon sa position précédente et sa direction
                pair<int, int> newPosition = it.second;
                newPosition.first += directions[it.first].first;
                newPosition.second += directions[it.first].second;
                //On vérifie que l'on est bien dans la limite du pattern
                if (newPosition.first >= 0 && newPosition.first < pattern[0].length() && newPosition.second >= 0 && newPosition.second < pattern.size())
                {
                    //Si jamais on a déjà parcouru cette case avec la même direction d'arrivée, on se trouve alors dans un début de boucle
                    //On ne considère donc pas la case car on a déjà parcouru le chemin précédemment
                    if (energized.find(newPosition) != energized.end() && find(energized[newPosition].begin(), energized[newPosition].end(), it.first) != energized[newPosition].end()) {
                        continue;
                    }
                    energized[newPosition].push_back(it.first);
                    //cout << newPosition.first << " " << newPosition.second << endl;
                    switch (pattern[newPosition.second][newPosition.first])
                    {
                        //On continue sur notre trajectoire
                    case '.':
                        nextPositions.push_back({ it.first, newPosition });
                        break;
                        //On part à 90° selon l'angle d'arrivée sur la case
                    case '/':
                        if (it.first == 'U') {
                            nextPositions.push_back({ 'R', newPosition });
                        }
                        else if (it.first == 'D') {
                            nextPositions.push_back({ 'L', newPosition });
                        }
                        else if (it.first == 'R') {
                            nextPositions.push_back({ 'U', newPosition });
                        }
                        else {
                            nextPositions.push_back({ 'D', newPosition });
                        }
                        break;
                        //Idem
                    case '\\':
                        if (it.first == 'U') {
                            nextPositions.push_back({ 'L', newPosition });
                        }
                        else if (it.first == 'D') {
                            nextPositions.push_back({ 'R', newPosition });
                        }
                        else if (it.first == 'R') {
                            nextPositions.push_back({ 'D', newPosition });
                        }
                        else {
                            nextPositions.push_back({ 'U', newPosition });
                        }
                        break;
                        //On continue tout droit ou le rayon est séparé en 2, selon l'angle d'arrivée
                    case '|':
                        //Si on arrive d'en haut ou d'en bas, on continue sur la trajectoire
                        if (it.first == 'U' || it.first == 'D') {
                            nextPositions.push_back({ it.first, newPosition });
                        }
                        //Sinon, on splitte le rayon en deux, une partie partant en haut et une en bas
                        else {
                            nextPositions.push_back({ 'U', newPosition });
                            nextPositions.push_back({ 'D', newPosition });
                        }
                        break;
                        //Idem
                    case '-':
                        //Si on arrive de la gauche ou la droite, on continue
                        if (it.first == 'R' || it.first == 'L') {
                            nextPositions.push_back({ it.first, newPosition });
                        }
                        //Sinon, on splitte le rayon en deux, une partie partant à gauche et une à droite
                        else {
                            nextPositions.push_back({ 'L', newPosition });
                            nextPositions.push_back({ 'R', newPosition });
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
            //On récupère les prochaines directions de la lumière
            light_dirs_left = vector<pair<char, pair<int, int>>>(nextPositions);
        }
        int current = 0;
        for (auto& it : energized)
        {
            current++;
        }
        if (current > total) total = current;

        energized.clear();

        while (light_dirs_right.size() > 0)
        {
            vector<pair<char, pair<int, int>>> nextPositions;
            for (auto& it : light_dirs_right)
            {
                //On calcule la nouvelle position du rayon selon sa position précédente et sa direction
                pair<int, int> newPosition = it.second;
                newPosition.first += directions[it.first].first;
                newPosition.second += directions[it.first].second;
                //On vérifie que l'on est bien dans la limite du pattern
                if (newPosition.first >= 0 && newPosition.first < pattern[0].length() && newPosition.second >= 0 && newPosition.second < pattern.size())
                {
                    //Si jamais on a déjà parcouru cette case avec la même direction d'arrivée, on se trouve alors dans un début de boucle
                    //On ne considère donc pas la case car on a déjà parcouru le chemin précédemment
                    if (energized.find(newPosition) != energized.end() && find(energized[newPosition].begin(), energized[newPosition].end(), it.first) != energized[newPosition].end()) {
                        continue;
                    }
                    energized[newPosition].push_back(it.first);
                    //cout << newPosition.first << " " << newPosition.second << endl;
                    switch (pattern[newPosition.second][newPosition.first])
                    {
                        //On continue sur notre trajectoire
                    case '.':
                        nextPositions.push_back({ it.first, newPosition });
                        break;
                        //On part à 90° selon l'angle d'arrivée sur la case
                    case '/':
                        if (it.first == 'U') {
                            nextPositions.push_back({ 'R', newPosition });
                        }
                        else if (it.first == 'D') {
                            nextPositions.push_back({ 'L', newPosition });
                        }
                        else if (it.first == 'R') {
                            nextPositions.push_back({ 'U', newPosition });
                        }
                        else {
                            nextPositions.push_back({ 'D', newPosition });
                        }
                        break;
                        //Idem
                    case '\\':
                        if (it.first == 'U') {
                            nextPositions.push_back({ 'L', newPosition });
                        }
                        else if (it.first == 'D') {
                            nextPositions.push_back({ 'R', newPosition });
                        }
                        else if (it.first == 'R') {
                            nextPositions.push_back({ 'D', newPosition });
                        }
                        else {
                            nextPositions.push_back({ 'U', newPosition });
                        }
                        break;
                        //On continue tout droit ou le rayon est séparé en 2, selon l'angle d'arrivée
                    case '|':
                        //Si on arrive d'en haut ou d'en bas, on continue sur la trajectoire
                        if (it.first == 'U' || it.first == 'D') {
                            nextPositions.push_back({ it.first, newPosition });
                        }
                        //Sinon, on splitte le rayon en deux, une partie partant en haut et une en bas
                        else {
                            nextPositions.push_back({ 'U', newPosition });
                            nextPositions.push_back({ 'D', newPosition });
                        }
                        break;
                        //Idem
                    case '-':
                        //Si on arrive de la gauche ou la droite, on continue
                        if (it.first == 'R' || it.first == 'L') {
                            nextPositions.push_back({ it.first, newPosition });
                        }
                        //Sinon, on splitte le rayon en deux, une partie partant à gauche et une à droite
                        else {
                            nextPositions.push_back({ 'L', newPosition });
                            nextPositions.push_back({ 'R', newPosition });
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
            //On récupère les prochaines directions de la lumière
            light_dirs_right = vector<pair<char, pair<int, int>>>(nextPositions);

            current = 0;
            for (auto& it : energized)
            {
                current++;
            }
            if (current > total) total = current;
        }
    }

    

    //On parcours maintenant toutes les possibilités de commencer en haut et en bas (vers respectivement le bas et le haut)
    for (int w = 0; w < pattern[0].length(); w++)
    {
        vector<pair<char, pair<int, int>>> light_dirs_top;
        light_dirs_top.push_back({ 'D', {w,-1} });
        vector<pair<char, pair<int, int>>> light_dirs_bottom;
        light_dirs_bottom.push_back({ 'U', {w,pattern.size()}});
        //Stocke les différentes cases parcourues, ainsi que la direction de la lumière lorsqu'elle arrive sur cette case
        //Cela permet à la fois de ne compter qu'une seule fois les cases où l'on serait passé plusieurs fois mais de côtés différents
        //Mais également de s'arrêter en cas de boucle, dès que l'on détecte que l'on repasse sur une même case dans la même direction que précédemment
        map<pair<int, int>, vector<char>> energized;

        cout << w << endl;

        //Tant qu'on a encore des directions à parcourir
        while (light_dirs_top.size() > 0)
        {
            vector<pair<char, pair<int, int>>> nextPositions;
            for (auto& it : light_dirs_top)
            {
                //On calcule la nouvelle position du rayon selon sa position précédente et sa direction
                pair<int, int> newPosition = it.second;
                newPosition.first += directions[it.first].first;
                newPosition.second += directions[it.first].second;
                //On vérifie que l'on est bien dans la limite du pattern
                if (newPosition.first >= 0 && newPosition.first < pattern[0].length() && newPosition.second >= 0 && newPosition.second < pattern.size())
                {
                    //Si jamais on a déjà parcouru cette case avec la même direction d'arrivée, on se trouve alors dans un début de boucle
                    //On ne considère donc pas la case car on a déjà parcouru le chemin précédemment
                    if (energized.find(newPosition) != energized.end() && find(energized[newPosition].begin(), energized[newPosition].end(), it.first) != energized[newPosition].end()) {
                        continue;
                    }
                    energized[newPosition].push_back(it.first);
                    //cout << newPosition.first << " " << newPosition.second << endl;
                    switch (pattern[newPosition.second][newPosition.first])
                    {
                        //On continue sur notre trajectoire
                    case '.':
                        nextPositions.push_back({ it.first, newPosition });
                        break;
                        //On part à 90° selon l'angle d'arrivée sur la case
                    case '/':
                        if (it.first == 'U') {
                            nextPositions.push_back({ 'R', newPosition });
                        }
                        else if (it.first == 'D') {
                            nextPositions.push_back({ 'L', newPosition });
                        }
                        else if (it.first == 'R') {
                            nextPositions.push_back({ 'U', newPosition });
                        }
                        else {
                            nextPositions.push_back({ 'D', newPosition });
                        }
                        break;
                        //Idem
                    case '\\':
                        if (it.first == 'U') {
                            nextPositions.push_back({ 'L', newPosition });
                        }
                        else if (it.first == 'D') {
                            nextPositions.push_back({ 'R', newPosition });
                        }
                        else if (it.first == 'R') {
                            nextPositions.push_back({ 'D', newPosition });
                        }
                        else {
                            nextPositions.push_back({ 'U', newPosition });
                        }
                        break;
                        //On continue tout droit ou le rayon est séparé en 2, selon l'angle d'arrivée
                    case '|':
                        //Si on arrive d'en haut ou d'en bas, on continue sur la trajectoire
                        if (it.first == 'U' || it.first == 'D') {
                            nextPositions.push_back({ it.first, newPosition });
                        }
                        //Sinon, on splitte le rayon en deux, une partie partant en haut et une en bas
                        else {
                            nextPositions.push_back({ 'U', newPosition });
                            nextPositions.push_back({ 'D', newPosition });
                        }
                        break;
                        //Idem
                    case '-':
                        //Si on arrive de la gauche ou la droite, on continue
                        if (it.first == 'R' || it.first == 'L') {
                            nextPositions.push_back({ it.first, newPosition });
                        }
                        //Sinon, on splitte le rayon en deux, une partie partant à gauche et une à droite
                        else {
                            nextPositions.push_back({ 'L', newPosition });
                            nextPositions.push_back({ 'R', newPosition });
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
            //On récupère les prochaines directions de la lumière
            light_dirs_top = vector<pair<char, pair<int, int>>>(nextPositions);
        }
        int current = 0;
        for (auto& it : energized)
        {
            current++;
        }
        if (current > total) total = current;

        energized.clear();

        while (light_dirs_bottom.size() > 0)
        {
            vector<pair<char, pair<int, int>>> nextPositions;
            for (auto& it : light_dirs_bottom)
            {
                //On calcule la nouvelle position du rayon selon sa position précédente et sa direction
                pair<int, int> newPosition = it.second;
                newPosition.first += directions[it.first].first;
                newPosition.second += directions[it.first].second;
                //On vérifie que l'on est bien dans la limite du pattern
                if (newPosition.first >= 0 && newPosition.first < pattern[0].length() && newPosition.second >= 0 && newPosition.second < pattern.size())
                {
                    //Si jamais on a déjà parcouru cette case avec la même direction d'arrivée, on se trouve alors dans un début de boucle
                    //On ne considère donc pas la case car on a déjà parcouru le chemin précédemment
                    if (energized.find(newPosition) != energized.end() && find(energized[newPosition].begin(), energized[newPosition].end(), it.first) != energized[newPosition].end()) {
                        continue;
                    }
                    energized[newPosition].push_back(it.first);
                    //cout << newPosition.first << " " << newPosition.second << endl;
                    switch (pattern[newPosition.second][newPosition.first])
                    {
                        //On continue sur notre trajectoire
                    case '.':
                        nextPositions.push_back({ it.first, newPosition });
                        break;
                        //On part à 90° selon l'angle d'arrivée sur la case
                    case '/':
                        if (it.first == 'U') {
                            nextPositions.push_back({ 'R', newPosition });
                        }
                        else if (it.first == 'D') {
                            nextPositions.push_back({ 'L', newPosition });
                        }
                        else if (it.first == 'R') {
                            nextPositions.push_back({ 'U', newPosition });
                        }
                        else {
                            nextPositions.push_back({ 'D', newPosition });
                        }
                        break;
                        //Idem
                    case '\\':
                        if (it.first == 'U') {
                            nextPositions.push_back({ 'L', newPosition });
                        }
                        else if (it.first == 'D') {
                            nextPositions.push_back({ 'R', newPosition });
                        }
                        else if (it.first == 'R') {
                            nextPositions.push_back({ 'D', newPosition });
                        }
                        else {
                            nextPositions.push_back({ 'U', newPosition });
                        }
                        break;
                        //On continue tout droit ou le rayon est séparé en 2, selon l'angle d'arrivée
                    case '|':
                        //Si on arrive d'en haut ou d'en bas, on continue sur la trajectoire
                        if (it.first == 'U' || it.first == 'D') {
                            nextPositions.push_back({ it.first, newPosition });
                        }
                        //Sinon, on splitte le rayon en deux, une partie partant en haut et une en bas
                        else {
                            nextPositions.push_back({ 'U', newPosition });
                            nextPositions.push_back({ 'D', newPosition });
                        }
                        break;
                        //Idem
                    case '-':
                        //Si on arrive de la gauche ou la droite, on continue
                        if (it.first == 'R' || it.first == 'L') {
                            nextPositions.push_back({ it.first, newPosition });
                        }
                        //Sinon, on splitte le rayon en deux, une partie partant à gauche et une à droite
                        else {
                            nextPositions.push_back({ 'L', newPosition });
                            nextPositions.push_back({ 'R', newPosition });
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
            //On récupère les prochaines directions de la lumière
            light_dirs_bottom = vector<pair<char, pair<int, int>>>(nextPositions);

            current = 0;
            for (auto& it : energized)
            {
                current++;
            }
            if (current > total) total = current;

        }
    }
    
    cout << total << endl;
}


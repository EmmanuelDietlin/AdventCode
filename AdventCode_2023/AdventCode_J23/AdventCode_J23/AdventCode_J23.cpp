// AdventCode_J23.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

struct PathElement
{
    pair<int, int> coords;
    char direction;
    int length;
    vector<int> indexes;
};

bool compare(pair<pair<char, pair<int, int>>, pair<int, pair<int, int>>>left, pair<pair<char, pair<int, int>>, pair<int, pair<int, int>>> right) {
    return left.second.first < right.second.first;
}

int main()
{
    ifstream input;
    string line;
    vector<string> hike_map;
    input.open("input.txt");
    pair<int, int> start(0,0), end(0,0);
    int non_walkable_count = 0;
    while (getline(input, line)) 
    {
        hike_map.push_back(line);
        non_walkable_count += count(line.begin(), line.end(), '#');
        
    }
    input.close();
    start.first = hike_map[0].find('.');
    end.first = hike_map[hike_map.size() - 1].find('.');
    end.second = hike_map.size() - 1;

    //On va parcourir l'intégralité des chemins
    //A chaque embranchement, on va créer un nouveau départ partant dans la nouvelle direction disponible
    //On va calculer la distance sur tous les chemins simultanéments
    //Et garder le plus long à la fin
    //Si on a deux chemins qui se rejoignent, on garde uniquement le plus long
    //Coordonnées du dernier point à partir duquel on change de direction + nouvelle direction de déplacement et longueur actuelle du chemin
    vector<PathElement> paths;
    int longest = 0;

    //Pour améliorer, on devrait soustraire à ce nombre le nombre de cases '#' afin d'avoir une meilleure limite
    long long max_size = (hike_map.size() * hike_map[0].size() - non_walkable_count);


    //PART TWO
    //Ici, les pentes peuvent être remontées
    //
    //On doit faire attention à ne pas passer 2 fois par une même case pour chaque chemin
    //Comme on travaille sur tous les chemins en même temps, il faut trouver un moyen  d'identifier de manière unique chaque chemin ainsi que quels cases
    //parcourent ces chemins
    //Chaque chemin aura une liste d'indices associés, qui indiquent ainsi les séparations successives qui ont eu lieues.
    // A chaque séparation, on va donc associer un nouvel identifiant à la bifurcation créée.
    // Ensuite, on stocke les coins avec leurs coordonnées ainsi qu'une liste d'indices, qui sont, pour chaque chemin passant par ce coin, 
    // L'identifiant le plus à droite (ie le plus récente)
    // Si un chemin essayant de passer par le corner à dans sa liste d'id l'identifiant du corner, alors on est déjà passé par ici
    // On arrête donc le chemin en cours.
    // Par exemple : 
    // #.###
    // #...#
    // #.#.#
    // #....
    // Le corner (1,1) aura comme identifiant 1 (chemin 1 passe par lui)
    // Le corner (1,3) aura comme identifiant 2 (chemin 1,2 passe par lui)
    // Le corner (3,1) aura comme identifiant 3 (chemin 1,3) passe par lui
    // Supposons ensuite qu'on ai le chemin 1,3,4 qui remonte jusqu'à (1,3) => on est ok car 1,3,4 ne contient pas 2, ie on n'est donc pas encore passé par là par ce chemin
    // On arrive ensuite à (1,1). Ici, 1 est bien présent dans (1,3,4), donc on est déjà passé par ce corner avec ce chemin, on est en train de boucler et donc on s'arrête ici
    //

    //La solution modifiée est également fonctionnelle pour la partie 1, mais elle est trop lente pour la partie 2

    //TODO
    //PArtie 2 => algo trop lent
    // On peut ici utiliser l'algorithme de BELLMAN-FORD
    // https://ics.utc.fr/~led/co/anx_recherche_plus_long_chemin.html
    // 
    // 
    // On va définir comme node les transitions entre déplacement vertical et horizontal (Cf J17)
    // Autrement dit, pour chacune des coordonnées où un chemin peut changer de direction
    // On aura deux nodes : une verticale et une horizontale
    // Les nodes verticales ne peuvent rejoindre que des nodes horizontales, et inversement
    // La première étape est alors de parcourir la carte pour déterminer toutes les nodes 
    // => à implémenter !
    //TODO
    /*map<pair<char, pair<int, int>>, pair<int, pair<int, int>>> distances;
    int k = 0;
    distances[{'V', start}] = { 0, start };
    do {

    } while(k < )*/


    //vector<pair<char, pair<int, int>>> corners;
    //corners.push_back({ 'V', start });
    //pair<char, pair<int, int>> pivot(corners[0]);
    ////On stocke avec comme clé les coordonnées et comme valeur un couple de valeur distance et coordonnées parent
    //map<pair<char, pair<int, int>>, pair<int, pair<int, int>>> distances;
    //map<pair<char, pair<int, int>>, pair<int, pair<int, int>>> tmp;
    //distances[pivot] = { 0, {0, 0} };
    //bool ended = false;

    //while (ended == false) 
    //

    //    if (pivot.first == 'V') 
    //    {
    //        vector<pair<int, pair<int, int>>> nextNodes;
    //        int index = 1;
    //        //On regarde toutes les cases au-dessus et en-dessous
    //        while (pivot.second.second - index >= 0 && hike_map[pivot.second.second - index][pivot.second.first] != '#')
    //        {
    //            //On regarde d'abord au-desssus
    //            pair<int, int> nextPoint({ pivot.second.first, pivot.second.second - index });
    //            if (nextPoint == end) nextNodes.push_back({ index, nextPoint });
    //            //On regarde à gauche et/ou a droite de la case pour voir si c'est une intersection
    //            if ((nextPoint.first - 1 >= 0 && hike_map[nextPoint.second][nextPoint.first - 1] != '#') ||
    //                (nextPoint.first + 1 < hike_map[0].length() && hike_map[nextPoint.second][nextPoint.first + 1] != '#')) 
    //            {
    //                nextNodes.push_back({ index, nextPoint });
    //            }
    //            index++;
    //        }
    //        index = 1;
    //        while (pivot.second.second + index < hike_map.size() && hike_map[pivot.second.second + index][pivot.second.first] != '#') 
    //        {
    //            //On regarde ensuite en-dessous
    //            pair<int, int> nextPoint({ pivot.second.first, pivot.second.second + index });
    //            if (nextPoint == end) nextNodes.push_back({ index, nextPoint });
    //            //On regarde à gauche et/ou a droite de la case pour voir si c'est une intersection
    //            if ((nextPoint.first - 1 >= 0 && hike_map[nextPoint.second][nextPoint.first - 1] != '#') ||
    //                (nextPoint.first + 1 < hike_map[0].length() && hike_map[nextPoint.second][nextPoint.first + 1] != '#')) {
    //                nextNodes.push_back({ index, nextPoint });
    //            }
    //            index++;
    //        }
    //        //On met ensuite à jour la distance des corners
    //        for (auto& it : nextNodes) 
    //        {
    //            if (find_if(corners.begin(), corners.end(), [&it](const pair<char, pair<int, int>>& elem) {return elem.first == 'H' && elem.second == it.second; }) != corners.end()) {
    //                //La node est déjà dans l'ensemble, on passe à la suivante
    //                continue;
    //            }
    //            //On modifie ici l'algorithme pour avoir le chemin LE PLUS LONG
    //            //Donc la condition d'inégalité est inversée
    //            if (distances.count({ 'H',it.second }) == 0 || distances[{ 'H', it.second }].first < distances[pivot].first + it.first) {
    //                distances[{ 'H', it.second }].first = distances[pivot].first + it.first;
    //                distances[{ 'H', it.second }].second = pivot.second;
    //                tmp[{ 'H', it.second }] = distances[{ 'H', it.second }];
    //            }

    //        }

    //    }
    //    else 
    //    {
    //        vector<pair<int, pair<int, int>>> nextNodes;
    //        int index = 1;
    //        //On regarde toutes les cases à gauche et à droite
    //        while (pivot.second.first - index >= 0 && hike_map[pivot.second.second][pivot.second.first - index] != '#')
    //        {
    //            //On regarde d'abord au-desssus
    //            pair<int, int> nextPoint({ pivot.second.first - index, pivot.second.second });
    //            if (nextPoint == end) nextNodes.push_back({ index, nextPoint });
    //            //On regarde en haut et en bas de la case pour voir si c'est une intersection
    //            if ((nextPoint.second - 1 >= 0 && hike_map[nextPoint.second - 1][nextPoint.first] != '#') ||
    //                (nextPoint.second + 1 < hike_map.size() && hike_map[nextPoint.second + 1][nextPoint.first] != '#'))
    //            {
    //                nextNodes.push_back({ index, nextPoint });
    //            }
    //            index++;
    //        }
    //        index = 1;
    //        while (pivot.second.first + index < hike_map[0].length() && hike_map[pivot.second.second][pivot.second.first + index] != '#')
    //        {
    //            //On regarde ensuite en-dessous
    //            pair<int, int> nextPoint({ pivot.second.first + index, pivot.second.second });
    //            if (nextPoint == end) nextNodes.push_back({ index, nextPoint });
    //            //On regarde en haut et en bas de la case pour voir si c'est une intersection
    //            if ((nextPoint.second - 1 >= 0 && hike_map[nextPoint.second - 1][nextPoint.first] != '#') ||
    //                (nextPoint.second + 1 < hike_map.size() && hike_map[nextPoint.second + 1][nextPoint.first] != '#'))
    //            {
    //                nextNodes.push_back({ index, nextPoint });
    //            }
    //            index++;
    //        }
    //        //On met ensuite à jour la distance des corners
    //        for (auto& it : nextNodes)
    //        {
    //            if (find_if(corners.begin(), corners.end(), [&it](const pair<char, pair<int, int>>& elem) {return elem.first == 'V' && elem.second == it.second; }) != corners.end()) {
    //                //La node est déjà dans l'ensemble, on passe à la suivante
    //                continue;
    //            }
    //            //On modifie ici l'algorithme pour avoir le chemin LE PLUS LONG
    //            //Donc la condition d'inégalité est inversée
    //            if (distances.count({ 'V',it.second }) == 0 || distances[{ 'V', it.second }].first < distances[pivot].first + it.first) {
    //                distances[{ 'V', it.second }].first = distances[pivot].first + it.first;
    //                distances[{ 'V', it.second }].second = pivot.second;
    //                tmp[{ 'V', it.second }] = distances[{ 'V', it.second }];
    //            }
    //        }
    //    }

    //    pair<char, pair<int, int>> newPivot;
    //    cout << "pivot : " <<  pivot.first << " " << pivot.second.first << " " << pivot.second.second << endl;
    //    hike_map[pivot.second.second][pivot.second.first] = '0';
    //    for (auto& map_line : hike_map) {
    //        cout << map_line << endl;
    //    }
    //    //On enlève le pivot des distances, car on cherche comme prochain pivot la node de distance la plus grande PAS ENCORE dans l'ensemble
    //    distances.erase(pivot);
    //    //distances[pivot].first = INT_MAX;
    //    if (distances.size() == 0) break;
    //    pivot = max_element(distances.begin(), distances.end(), compare)->first;

    //    //On rentre les deux directions du corner afin d'éviter d'avoir un parcours de celui-ci dans les deux sens 
    //    corners.push_back({'V', pivot.second});
    //    corners.push_back({ 'H', pivot.second });
    //    if (pivot.second == end) break;
    //}

    //longest = distances[pivot].first;



    //METHODE 1 (partie 1)
    map<pair<int, int>, vector<int>> corners;
    //A partir du point de départ, on ne peut que descendre (par construction)
    PathElement startElement
    {
        start,
        'D',
        0,
        {}
    };
    paths.push_back(startElement);
    corners[start].push_back(1);
    int nextAvailablePathIndex = 2;
    
    while (paths.size() > 0) 
    {
        vector<PathElement> nextPaths;
        for (auto& path : paths) 
        {
            //Le chemin à dépassé la taille possible théorique pour un chemin (qu'on pose ici au nombre total de cases de la carte)
            //On tourne donc en rond, on arrête donc de considérer ce chemin
            if (path.length >= max_size) continue;
            pair<int, int> nextPoint(path.coords);
            switch (path.direction) {
                //Selon la direction de déplacement
            case 'D':
                nextPoint.second += 1;
                //Tant que l'on a pas d'obstacle le long de notre chemin
                while (nextPoint.second < hike_map.size() && hike_map[nextPoint.second][nextPoint.first] != '#' 
#if PART_ONE
                    && hike_map[nextPoint.second][nextPoint.first] != '^'
#endif 
                    ) {
                    //Le chemin est de 1 plus long
                    path.length++;

                    //Pour la partie 2, les pentes sont des cases normales
#if PART_ONE
                    if (hike_map[nextPoint.second][nextPoint.first] == 'v') {
                        //On est sur une pente raide, on doit alors continuer sur la pente obligatoirement
                        nextPoint.second++;
                        continue;
                    }
#endif
                    //Si on est au bout, on s'arrête directement de continuer à calculer le chemin
                    if (nextPoint == end) {
                        if (path.length > longest) longest = path.length;
                        break;
                    }

                    bool alreadyVisitedCorner = false;

                    //On regarde à gauche et à droite si on a des chemins possibles
                    //On n'est pas obligé de vérifier que l'on ne dépasse pas les bornes gauche et droite, car on a une bordure qui entoure complètement la carte
                    //On aura donc obligatoirement un '#' avant de dépasser la taillez
                    //On va tout de même s'assurer que l'on ne dépasse pas les limites
                    //On vérifie que l'on a pas un mur ou une pente raide dans le sens opposé sur le potentiel nouveau chemin

                    //Pour la partie 2, les pentes ne sont pas bloquantes
                    //On commente donc cette condition
                    if (nextPoint.first - 1 >= 0 && hike_map[nextPoint.second][nextPoint.first - 1] != '#' 
#if PART_ONE
                        && hike_map[nextPoint.second][nextPoint.first - 1] != '>'
#endif
                        ) {
                        //On a un chemin à gauche que l'on peut explorer
                        PathElement nextPath{ 
                            nextPoint,
                            'L',
                            path.length,
                            path.indexes
                        };
                        //Si ce corner est déjà enregistré, c'est qu'on est déjà passé par là.
                        //On excluera alors ce chemin
                        if (corners.count(nextPath.coords) != 0) 
                        {
                            for (auto& idx : corners[nextPath.coords]) {
                                if (find(nextPath.indexes.begin(), nextPath.indexes.end(), idx) != nextPath.indexes.end()) {
                                    alreadyVisitedCorner = true;
                                    break;
                                }
                            }
                        }
                        if (alreadyVisitedCorner == false) {
                            nextPath.indexes.push_back(nextAvailablePathIndex);
                            nextPaths.push_back(nextPath);
                            corners[nextPath.coords].push_back(nextAvailablePathIndex);
                            nextAvailablePathIndex++;
                        }
                        
                    }
                    alreadyVisitedCorner = false;
                    if (nextPoint.first + 1 < hike_map[0].length() && hike_map[nextPoint.second][nextPoint.first + 1] != '#' 
#if PART_ONE
                        && hike_map[nextPoint.second][nextPoint.first + 1] != '<'
#endif
                        ) {
                        //On a un chemin à droite que l'on peut explorer
                        PathElement nextPath{
                            nextPoint,
                            'R',
                            path.length,
                            path.indexes
                        };
                        //Si ce corner est déjà enregistré, c'est qu'on est déjà passé par là.
                        //On excluera alors ce chemin
                        if (corners.count(nextPath.coords) != 0)
                        {
                            for (auto& idx : corners[nextPath.coords]) {
                                if (find(nextPath.indexes.begin(), nextPath.indexes.end(), idx) != nextPath.indexes.end()) {
                                    alreadyVisitedCorner = true;
                                    break;
                                }
                            }
                        }
                        if (alreadyVisitedCorner == false) {
                            nextPath.indexes.push_back(nextAvailablePathIndex);
                            nextPaths.push_back(nextPath);
                            corners[nextPath.coords].push_back(nextAvailablePathIndex);
                            nextAvailablePathIndex++;
                        }
                    }
                    nextPoint.second++;
                }
                break;
            case 'U':
                nextPoint.second--;
                while (nextPoint.second >= 0 && hike_map[nextPoint.second][nextPoint.first] != '#' 
#if PART_ONE
                    && hike_map[nextPoint.second][nextPoint.first] != 'v'
#endif        
                    ) {
                    //Le chemin est de 1 plus long
                    path.length++;
#if PART_ONE
                    if (hike_map[nextPoint.second][nextPoint.first] == '^') {
                        //On est sur une pente raide, on doit alors continuer sur la pente obligatoirement
                        nextPoint.second--;
                        continue;
                    }
#endif
                    //Si on est au bout, on s'arrête directement de continuer à calculer le chemin
                    if (nextPoint == end) {
                        if (path.length > longest) longest = path.length;
                        break;
                    }

                    bool alreadyVisitedCorner = false;

                    //On regarde à gauche et à droite si on a des chemins possibles
                    //On n'est pas obligé de vérifier que l'on ne dépasse pas les bornes gauche et droite, car on a une bordure qui entoure complètement la carte
                    //On aura donc obligatoirement un '#' avant de dépasser la taillez
                    //On va tout de même s'assurer que l'on ne dépasse pas les limites
                    //On vérifie que l'on a pas un mur ou une pente raide dans le sens opposé sur le potentiel nouveau chemin
                    if (nextPoint.first - 1 >= 0 && hike_map[nextPoint.second][nextPoint.first - 1] != '#' 
#if PART_ONE
                        && hike_map[nextPoint.second][nextPoint.first - 1] != '>'
#endif
                        ) {                        
                        //On a un chemin à gauche que l'on peut explorer
                        PathElement nextPath{
                            nextPoint,
                            'L',
                            path.length,
                            path.indexes
                        };
                        //Si ce corner est déjà enregistré, c'est qu'on est déjà passé par là.
                        //On excluera alors ce chemin
                        if (corners.count(nextPath.coords) != 0)
                        {
                            for (auto& idx : corners[nextPath.coords]) {
                                if (find(nextPath.indexes.begin(), nextPath.indexes.end(), idx) != nextPath.indexes.end()) {
                                    alreadyVisitedCorner = true;
                                    break;
                                }
                            }
                        }
                        if (alreadyVisitedCorner == false) {
                            nextPath.indexes.push_back(nextAvailablePathIndex);
                            nextPaths.push_back(nextPath);
                            corners[nextPath.coords].push_back(nextAvailablePathIndex);
                            nextAvailablePathIndex++;
                        }

                    }
                    alreadyVisitedCorner = false;
                    if (nextPoint.first + 1 < hike_map[0].length() && hike_map[nextPoint.second][nextPoint.first + 1] != '#' 
#if PART_ONE
                        && hike_map[nextPoint.second][nextPoint.first + 1] != '<'
#endif
                        ) {
                        //On a un chemin à droite que l'on peut explorer
                        PathElement nextPath{
                            nextPoint,
                            'R',
                            path.length,
                            path.indexes
                        };
                        //Si ce corner est déjà enregistré, c'est qu'on est déjà passé par là.
                        //On excluera alors ce chemin
                        if (corners.count(nextPath.coords) != 0)
                        {
                            for (auto& idx : corners[nextPath.coords]) {
                                if (find(nextPath.indexes.begin(), nextPath.indexes.end(), idx) != nextPath.indexes.end()) {
                                    alreadyVisitedCorner = true;
                                    break;
                                }
                            }
                        }
                        if (alreadyVisitedCorner == false) {
                            nextPath.indexes.push_back(nextAvailablePathIndex);
                            nextPaths.push_back(nextPath);
                            corners[nextPath.coords].push_back(nextAvailablePathIndex);
                            nextAvailablePathIndex++;
                        }
                    }
                    nextPoint.second--;
                }
                break;
            case 'L':
                nextPoint.first--;
                while (nextPoint.first >= 0 && hike_map[nextPoint.second][nextPoint.first] != '#' 
#if PART_ONE
                    && hike_map[nextPoint.second][nextPoint.first] != '>'
#endif        
                    ) 
                {
                    path.length++;
#if PART_ONE
                    if (hike_map[nextPoint.second][nextPoint.first] == '<') {
                        //On est sur une pente raide, on doit alors continuer sur la pente obligatoirement
                        nextPoint.first--;
                        continue;
                    }
#endif

                    //Si on est au bout, on s'arrête directement de continuer à calculer le chemin
                    if (nextPoint == end) {
                        if (path.length > longest) longest = path.length;
                        break;
                    }

                    bool alreadyVisitedCorner = false;

                    //On regarde si on a des chemins en haut et en bas
                    if (nextPoint.second - 1 >= 0 && hike_map[nextPoint.second - 1][nextPoint.first] != '#' 
#if PART_ONE
                        && hike_map[nextPoint.second - 1][nextPoint.first] != 'v'
#endif
                        ) {
                        PathElement nextPath{
                            nextPoint,
                            'U',
                            path.length,
                            path.indexes
                        };
                        //Si ce corner est déjà enregistré, c'est qu'on est déjà passé par là.
                        //On excluera alors ce chemin
                        if (corners.count(nextPath.coords) != 0)
                        {
                            for (auto& idx : corners[nextPath.coords]) {
                                if (find(nextPath.indexes.begin(), nextPath.indexes.end(), idx) != nextPath.indexes.end()) {
                                    alreadyVisitedCorner = true;
                                    break;
                                }
                            }
                        }
                        if (alreadyVisitedCorner == false) {
                            nextPath.indexes.push_back(nextAvailablePathIndex);
                            nextPaths.push_back(nextPath);
                            corners[nextPath.coords].push_back(nextAvailablePathIndex);
                            nextAvailablePathIndex++;
                        }
                    }
                    if (nextPoint.second + 1 < hike_map.size() && hike_map[nextPoint.second + 1][nextPoint.first] != '#' 
#if PART_ONE
                        && hike_map[nextPoint.second + 1][nextPoint.first] != '^'
#endif
                        ) {
                        PathElement nextPath{
                            nextPoint,
                            'D',
                            path.length,
                            path.indexes
                        };
                        //Si ce corner est déjà enregistré, c'est qu'on est déjà passé par là.
                        //On excluera alors ce chemin
                        if (corners.count(nextPath.coords) != 0)
                        {
                            for (auto& idx : corners[nextPath.coords]) {
                                if (find(nextPath.indexes.begin(), nextPath.indexes.end(), idx) != nextPath.indexes.end()) {
                                    alreadyVisitedCorner = true;
                                    break;
                                }
                            }
                        }
                        if (alreadyVisitedCorner == false) {
                            nextPath.indexes.push_back(nextAvailablePathIndex);
                            nextPaths.push_back(nextPath);
                            corners[nextPath.coords].push_back(nextAvailablePathIndex);
                            nextAvailablePathIndex++;
                        }
                    }
                    nextPoint.first--;
                }
                break;
            case 'R':
                nextPoint.first++;
                while (nextPoint.first < hike_map[0].length() && hike_map[nextPoint.second][nextPoint.first] != '#' 
#if PART_ONE
                    && hike_map[nextPoint.second][nextPoint.first] != '<'
#endif        
                    )
                {
                    path.length++;
#if PART_ONE
                    if (hike_map[nextPoint.second][nextPoint.first] == '>') {
                        //On est sur une pente raide, on doit alors continuer sur la pente obligatoirement
                        nextPoint.first++;
                        continue;
                    }
#endif

                    if (nextPoint == end) {
                        if (path.length > longest) longest = path.length;
                        break;
                    }

                    bool alreadyVisitedCorner = false;

                    //On regarde si on a des chemins en haut et en bas
                    if (nextPoint.second - 1 >= 0 && hike_map[nextPoint.second - 1][nextPoint.first] != '#' 
#if PART_ONE 
                        && hike_map[nextPoint.second - 1][nextPoint.first] != 'v'
#endif
                        ) {
                        PathElement nextPath{
                            nextPoint,
                            'U',
                            path.length,
                            path.indexes
                        };
                        //Si ce corner est déjà enregistré, c'est qu'on est déjà passé par là.
                        //On excluera alors ce chemin
                        if (corners.count(nextPath.coords) != 0)
                        {
                            for (auto& idx : corners[nextPath.coords]) {
                                if (find(nextPath.indexes.begin(), nextPath.indexes.end(), idx) != nextPath.indexes.end()) {
                                    alreadyVisitedCorner = true;
                                    break;
                                }
                            }
                        }
                        if (alreadyVisitedCorner == false) {
                            nextPath.indexes.push_back(nextAvailablePathIndex);
                            nextPaths.push_back(nextPath);
                            corners[nextPath.coords].push_back(nextAvailablePathIndex);
                            nextAvailablePathIndex++;
                        }
                    }
                    if (nextPoint.second + 1 < hike_map.size() && hike_map[nextPoint.second + 1][nextPoint.first] != '#' 
#if PART_ONE
                        && hike_map[nextPoint.second + 1][nextPoint.first] != '^'
#endif
                        ) {
                        PathElement nextPath{
                            nextPoint,
                            'D',
                            path.length,
                            path.indexes
                        };
                        //Si ce corner est déjà enregistré, c'est qu'on est déjà passé par là.
                        //On excluera alors ce chemin
                        if (corners.count(nextPath.coords) != 0)
                        {
                            for (auto& idx : corners[nextPath.coords]) {
                                if (find(nextPath.indexes.begin(), nextPath.indexes.end(), idx) != nextPath.indexes.end()) {
                                    alreadyVisitedCorner = true;
                                    break;
                                }
                            }
                        }
                        if (alreadyVisitedCorner == false) {
                            nextPath.indexes.push_back(nextAvailablePathIndex);
                            nextPaths.push_back(nextPath);
                            corners[nextPath.coords].push_back(nextAvailablePathIndex);
                            nextAvailablePathIndex++;
                        }
                    }
                    nextPoint.first++;
                }
                break;
            }

        }
        paths.clear();
        paths = nextPaths;
    }



#if PART_ONE
    cout << "Part one : " << longest << endl;
#else
    cout << "Part two : " << longest << endl;
#endif
}




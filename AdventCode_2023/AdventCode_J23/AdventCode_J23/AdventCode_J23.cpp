// AdventCode_J23.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <list>

using namespace std;

struct PathElement
{
    pair<int, int> coords;
    char direction;
    int length;
    vector<int> indexes;
};

struct Node {
    int x, y;
    char direction;
    int length;
    int parent_x, parent_y;

    Node(int x, int y, char direction, int length, int parent_x, int parent_y)
    {
        this->x = x;
        this->y = y;
        this->direction = direction;
        this->length = length;
        this->parent_x = parent_x;
        this->parent_y = parent_y;
    }

    Node(const Node& node)
    {
        x = node.x;
        y = node.y;
        direction = node.direction;
        length = node.length;
        parent_x = node.parent_x;
        parent_y = node.parent_y;
    }

    Node() {};
};

bool compare(pair<pair<char, pair<int, int>>, pair<int, pair<int, int>>>left, pair<pair<char, pair<int, int>>, pair<int, pair<int, int>>> right) {
    return left.second.first < right.second.first;
}

int main()
{
    ifstream input;
    string line;
    vector<string> hike_map;
    input.open("test.txt");
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
    //Soit le graphe tel que les nodes soient les croisement entre différents chemins
    //ie on a une node dès qu'un chemin change de direction
    //On peut alors, dans le cas où on a une node entre DEUX chemins seulement, supprimer celle-ci
    //Et définir un arc entre les deux nodes autour d'elle avec comme longueur la somme des deux longueurs 
    //Ainsi, cela permet de réduire la taille du graphe en ne gardant que les intersections entre deux chemins ou plus
    //Ensuite, on peut effectuer une recherche DFS (Depth first search) sur chacune des nodes pour chercher la longueur de tous les chemins possibles
    //Cela est possible car on peut supposer que la réduction du graphe à ses intersections réduit considérablement sa taille
    //Note : on remarque qu'avant chaque croisement, on rencontre toujours un > < ^ v
    //TODO
    //Chercher les nodes (= croisements)
    //Entier = longueur actuelle de l'arc
    Node startNode{
        start.first,
        start.second,
        'D',
        0,
        start.first,
        start.second
    };
    vector<Node> crossroads({ startNode });
    //Note : ne pas oublier que pour la partie 2, les relations sont symétriques
    multimap<pair<int, int>, pair<pair<int, int>, int>> corners;
    while (crossroads.size() > 0)
    {
        vector<Node> next;
        for (auto& it : crossroads)
        {
            pair<int, int> nextPoint({ it.x, it.y });
            bool foundIntersection = false;
            switch (it.direction) {
            case 'D':
                nextPoint.second++;
                while (foundIntersection == false && nextPoint.second < hike_map.size() && hike_map[nextPoint.second][nextPoint.first] != '#') {
                    it.length++;
                    if (hike_map[nextPoint.second][nextPoint.first] == 'v') {
                        foundIntersection = true;
                        //On est dans une intersection
                        //On cherche à identifier toutes les directions de cette intersection
                        nextPoint.second++;
                        it.length++;
                        //On fait attention à ne pas les chemins en sens inverse, cela nous ferait alors calculer plusieurs fois 
                        //Les mêmes chemins
                        //Ce qui ajoute inutilement au temps d'exécution
                        if (nextPoint.first - 1 >= 0 && hike_map[nextPoint.second][nextPoint.first - 1] != '#' 
                            && hike_map[nextPoint.second][nextPoint.first - 1] != '>') {
                            //On a un chemin vers la gauche
                            //On se place directement sur la première case de ce chemin car
                            //Sinon on aurait tout de suite un symbole '<', ce qui perturberait l'algorithme qui
                            //Chercherait alors directement une autre intersection
                            Node nxt
                            {
                                nextPoint.first - 1,
                                nextPoint.second,
                                'L',
                                1,
                                nextPoint.first,
                                nextPoint.second
                            };
                            next.push_back(nxt);

                        }
                        if (nextPoint.first + 1 < hike_map[0].length() && hike_map[nextPoint.second][nextPoint.first + 1] != '#' 
                            && hike_map[nextPoint.second][nextPoint.first + 1] != '<') {
                            //On a un chemin vers la droite
                            Node nxt
                            {
                                nextPoint.first + 1,
                                nextPoint.second,
                                'R',
                                1,
                                nextPoint.first,
                                nextPoint.second
                            };
                            next.push_back(nxt);
                        }
                        if (nextPoint.second + 1 < hike_map.size() && hike_map[nextPoint.second + 1][nextPoint.first] != '#' 
                            && hike_map[nextPoint.second + 1][nextPoint.first] != '^') {
                            //On a un chemin en bas
                            Node nxt
                            {
                                nextPoint.first,
                                nextPoint.second + 1,
                                'D',
                                1,
                                nextPoint.first,
                                nextPoint.second
                            };
                            next.push_back(nxt);
                        }
                        //On ajoute dans la map des corners les arcs it -> intersection et intersection -> it
                        //On utilise les coordonnées des parents car on peut avoir un chemin ayant eu plusieurs changements
                        //de direction (ce qui fait que ses coordonnées ne sont pas celles du début du chemin, mais
                        //celles du dernier changement de direction
                        corners.insert({ {it.parent_x, it.parent_y}, {{nextPoint.first, nextPoint.second},it.length} });
                        corners.insert({ {nextPoint.first, nextPoint.second}, {{it.parent_x, it.parent_y},it.length} });
                    }
                    if (nextPoint == end) {
                        corners.insert({ {it.parent_x, it.parent_y}, {{nextPoint.first, nextPoint.second},it.length} });
                        corners.insert({ {nextPoint.first, nextPoint.second}, {{it.parent_x, it.parent_y},it.length} });
                    }
                    nextPoint.second++;
                }
                if (foundIntersection) continue;
                //On arrive au bout du chemin, on cherche la prochaine direction de déplacement
                if (nextPoint.first - 1 >= 0 && hike_map[nextPoint.second - 1][nextPoint.first - 1] != '#') {
                    //On a un chemin à gauche
                    Node nxt;
                    nxt.x = nextPoint.first;
                    nxt.y = nextPoint.second - 1;
                    nxt.direction = 'L';
                    nxt.length = it.length;
                    nxt.parent_x = it.parent_x;
                    nxt.parent_y = it.parent_y;
                    next.push_back(nxt);
                }
                else if (nextPoint.first + 1 < hike_map[0].length() && hike_map[nextPoint.second - 1][nextPoint.first + 1] != '#') {
                    //On a un chemin à droite
                    Node nxt;
                    nxt.x = nextPoint.first;
                    nxt.y = nextPoint.second - 1;
                    nxt.direction = 'R';
                    nxt.length = it.length;
                    nxt.parent_x = it.parent_x;
                    nxt.parent_y = it.parent_y;
                    next.push_back(nxt);
                }
                break;
            case 'L':
                nextPoint.first--;
                while (foundIntersection == false && nextPoint.first >= 0 && hike_map[nextPoint.second][nextPoint.first] != '#') {
                    it.length++;
                    if (hike_map[nextPoint.second][nextPoint.first] == '<') {
                        foundIntersection = true;
                        //On est dans une intersection
                        //On cherche à identifier toutes les directions de cette intersection
                        nextPoint.first--;
                        it.length++;
                        if (nextPoint.first - 1 >= 0 && hike_map[nextPoint.second][nextPoint.first - 1] != '#'
                            && hike_map[nextPoint.second][nextPoint.first - 1] != '>') {
                            //On a un chemin vers la gauche
                            Node nxt
                            {
                                nextPoint.first - 1,
                                nextPoint.second,
                                'L',
                                1,
                                nextPoint.first,
                                nextPoint.second
                            };
                            next.push_back(nxt);
                        }
                        if (nextPoint.second - 1 >= 0 && hike_map[nextPoint.second - 1][nextPoint.first] != '#'
                            && hike_map[nextPoint.second - 1][nextPoint.first] != 'v') {
                            //On a un chemin en haut
                            Node nxt
                            {
                                nextPoint.first,
                                nextPoint.second - 1,
                                'U',
                                1,
                                nextPoint.first,
                                nextPoint.second
                            };
                            next.push_back(nxt);
                        }
                        if (nextPoint.second + 1 < hike_map.size() && hike_map[nextPoint.second + 1][nextPoint.first] != '#'
                            && hike_map[nextPoint.second + 1][nextPoint.first] != '^') {
                            //On a un chemin en bas
                            Node nxt
                            {
                                nextPoint.first,
                                nextPoint.second + 1,
                                'D',
                                1,
                                nextPoint.first,
                                nextPoint.second
                            };
                            next.push_back(nxt);
                        }
                        //On ajoute dans la map des corners les arcs it -> intersection et intersection -> it
                        //On utilise les coordonnées des parents car on peut avoir un chemin ayant eu plusieurs changements
                        //de direction (ce qui fait que ses coordonnées ne sont pas celles du début du chemin, mais
                        //celles du dernier changement de direction
                        corners.insert({ {it.parent_x, it.parent_y}, {{nextPoint.first, nextPoint.second},it.length} });
                        corners.insert({ {nextPoint.first, nextPoint.second}, {{it.parent_x, it.parent_y},it.length} });

                    }
                    if (nextPoint == end) {
                        corners.insert({ {it.parent_x, it.parent_y}, {{nextPoint.first, nextPoint.second},it.length} });
                        corners.insert({ {nextPoint.first, nextPoint.second}, {{it.parent_x, it.parent_y},it.length} });
                    }
                    nextPoint.first--;
                }
                if (foundIntersection) continue;
                //On arrive au bout du chemin, on cherche la prochaine direction de déplacement
                if (nextPoint.second + 1 < hike_map.size() && hike_map[nextPoint.second + 1][nextPoint.first + 1] != '#') {
                    //On a un chemin en bas
                    Node nxt;
                    nxt.x = nextPoint.first + 1;
                    nxt.y = nextPoint.second;
                    nxt.direction = 'D';
                    nxt.length = it.length;
                    nxt.parent_x = it.parent_x;
                    nxt.parent_y = it.parent_y;
                    next.push_back(nxt);
                }
                else if (nextPoint.second - 1 >= 0 && hike_map[nextPoint.second - 1][nextPoint.first + 1] != '#') {
                    //On a un chemin en haut
                    Node nxt;
                    nxt.x = nextPoint.first + 1;
                    nxt.y = nextPoint.second;
                    nxt.direction = 'U';
                    nxt.length = it.length;
                    nxt.parent_x = it.parent_x;
                    nxt.parent_y = it.parent_y;
                    next.push_back(nxt);
                }
                break;
            case 'R':
                nextPoint.first++;
                while (foundIntersection == false && nextPoint.first < hike_map[0].length() && hike_map[nextPoint.second][nextPoint.first] != '#') {
                    it.length++;
                    if (hike_map[nextPoint.second][nextPoint.first] == '>') {
                        foundIntersection = true;
                        it.length++;
                        //On est dans une intersection
                        //On cherche à identifier toutes les directions de cette intersection
                        nextPoint.first++;
                        if (nextPoint.first + 1 < hike_map[0].length() && hike_map[nextPoint.second][nextPoint.first + 1] != '#'
                            && hike_map[nextPoint.second][nextPoint.first + 1] != '<') {
                            //On a un chemin vers la droite
                            Node nxt
                            {
                                nextPoint.first + 1,
                                nextPoint.second,
                                'R',
                                1,
                                nextPoint.first,
                                nextPoint.second
                            };
                            next.push_back(nxt);
                        }
                        if (nextPoint.second - 1 >= 0 && hike_map[nextPoint.second - 1][nextPoint.first] != '#'
                            && hike_map[nextPoint.second - 1][nextPoint.first] != 'v') {
                            //On a un chemin en haut
                            Node nxt
                            {
                                nextPoint.first,
                                nextPoint.second - 1,
                                'U',
                                1,
                                nextPoint.first,
                                nextPoint.second
                            };
                            next.push_back(nxt);
                        }
                        if (nextPoint.second + 1 < hike_map.size() && hike_map[nextPoint.second + 1][nextPoint.first] != '#'
                            && hike_map[nextPoint.second + 1][nextPoint.first] != '^') {
                            //On a un chemin en bas
                            Node nxt
                            {
                                nextPoint.first,
                                nextPoint.second + 1,
                                'D',
                                1,
                                nextPoint.first,
                                nextPoint.second
                            };
                            next.push_back(nxt);
                        }
                        //On ajoute dans la map des corners les arcs it -> intersection et intersection -> it
                        //On utilise les coordonnées des parents car on peut avoir un chemin ayant eu plusieurs changements
                        //de direction (ce qui fait que ses coordonnées ne sont pas celles du début du chemin, mais
                        //celles du dernier changement de direction
                        corners.insert({ {it.parent_x, it.parent_y}, {{nextPoint.first, nextPoint.second},it.length} });
                        corners.insert({ {nextPoint.first, nextPoint.second}, {{it.parent_x, it.parent_y},it.length} });

                    }
                    if (nextPoint == end) {
                        corners.insert({ {it.parent_x, it.parent_y}, {{nextPoint.first, nextPoint.second},it.length} });
                        corners.insert({ {nextPoint.first, nextPoint.second}, {{it.parent_x, it.parent_y},it.length} });
                    }
                    nextPoint.first++;
                }
                if (foundIntersection) continue;
                //On arrive au bout du chemin, on cherche la prochaine direction de déplacement
                if (nextPoint.second + 1 < hike_map.size() && hike_map[nextPoint.second + 1][nextPoint.first - 1] != '#') {
                    //On a un chemin en bas
                    Node nxt;
                    nxt.x = nextPoint.first - 1;
                    nxt.y = nextPoint.second;
                    nxt.direction = 'D';
                    nxt.length = it.length;
                    nxt.parent_x = it.parent_x;
                    nxt.parent_y = it.parent_y;
                    next.push_back(nxt);
                }
                else if (nextPoint.second - 1 >= 0 && hike_map[nextPoint.second - 1][nextPoint.first - 1] != '#') {
                    //On a un chemin en haut
                    Node nxt;
                    nxt.x = nextPoint.first - 1;
                    nxt.y = nextPoint.second;
                    nxt.direction = 'U';
                    nxt.length = it.length;
                    nxt.parent_x = it.parent_x;
                    nxt.parent_y = it.parent_y;
                    next.push_back(nxt);
                }
                break;
            case 'U':
                nextPoint.second--;
                while (foundIntersection == false && nextPoint.second >= 0 && hike_map[nextPoint.second][nextPoint.first] != '#') {
                    it.length++;
                    if (hike_map[nextPoint.second][nextPoint.first] == '^') {
                        foundIntersection = true;
                        it.length++;
                        //On est dans une intersection
                        //On cherche à identifier toutes les directions de cette intersection
                        nextPoint.second--;
                        if (nextPoint.first - 1 >= 0 && hike_map[nextPoint.second][nextPoint.first - 1] != '#'
                            && hike_map[nextPoint.second][nextPoint.first - 1] != '>') {
                            //On a un chemin vers la gauche
                            Node nxt
                            {
                                nextPoint.first - 1,
                                nextPoint.second,
                                'L',
                                1,
                                nextPoint.first,
                                nextPoint.second
                            };
                            next.push_back(nxt);

                        }
                        if (nextPoint.first + 1 < hike_map[0].length() && hike_map[nextPoint.second][nextPoint.first + 1] != '#'
                            && hike_map[nextPoint.second][nextPoint.first + 1] != '<') {
                            //On a un chemin vers la droite
                            Node nxt
                            {
                                nextPoint.first + 1,
                                nextPoint.second,
                                'R',
                                1,
                                nextPoint.first,
                                nextPoint.second
                            };
                            next.push_back(nxt);
                        }
                        if (nextPoint.second - 1 >= 0 && hike_map[nextPoint.second - 1][nextPoint.first] != '#'
                            && hike_map[nextPoint.second - 1][nextPoint.first] != 'v') {
                            //On a un chemin en haut
                            Node nxt
                            {
                                nextPoint.first,
                                nextPoint.second - 1,
                                'U',
                                1,
                                nextPoint.first,
                                nextPoint.second
                            };
                            next.push_back(nxt);
                        }
                        //On ajoute dans la map des corners les arcs it -> intersection et intersection -> it
                        //On utilise les coordonnées des parents car on peut avoir un chemin ayant eu plusieurs changements
                        //de direction (ce qui fait que ses coordonnées ne sont pas celles du début du chemin, mais
                        //celles du dernier changement de direction
                        
                        corners.insert({ {it.parent_x, it.parent_y}, {{nextPoint.first, nextPoint.second},it.length} });
                        corners.insert({ {nextPoint.first, nextPoint.second}, {{it.parent_x, it.parent_y},it.length} });

                    }
                    if (nextPoint == end) {
                        corners.insert({ {it.parent_x, it.parent_y}, {{nextPoint.first, nextPoint.second},it.length} });
                        corners.insert({ {nextPoint.first, nextPoint.second}, {{it.parent_x, it.parent_y},it.length} });
                    }
                    nextPoint.second--;
                }
                if (foundIntersection) continue;
                //On arrive au bout du chemin, on cherche la prochaine direction de déplacement
                if (nextPoint.first - 1 >= 0 && hike_map[nextPoint.second + 1][nextPoint.first - 1] != '#') {
                    //On a un chemin à gauche
                    Node nxt;
                    nxt.x = nextPoint.first;
                    nxt.y = nextPoint.second + 1;
                    nxt.direction = 'L';
                    nxt.length = it.length;
                    nxt.parent_x = it.parent_x;
                    nxt.parent_y = it.parent_y;
                    next.push_back(nxt);
                }
                else if (nextPoint.first + 1 < hike_map[0].length() && hike_map[nextPoint.second + 1][nextPoint.first + 1] != '#') {
                    //On a un chemin à droite
                    Node nxt;
                    nxt.x = nextPoint.first;
                    nxt.y = nextPoint.second + 1;
                    nxt.direction = 'R';
                    nxt.length = it.length;
                    nxt.parent_x = it.parent_x;
                    nxt.parent_y = it.parent_y;
                    next.push_back(nxt);
                }
                break;
            }
        }
        crossroads = next;
    }
    //On enlève les valeurs redondantes de la map
    auto prev_first = corners.begin()->first;
    auto prev_second = corners.begin()->second;
    auto it = corners.begin();
    for (++it; it != corners.end();) {
        if (it->first == prev_first && it->second == prev_second) {
            it = corners.erase(it);
        }
        else {
            prev_first = it->first;
            prev_second = it->second;
            ++it;
        }
    }

    //Enfin, on fait un DFS pour trouver le plus long chemin
    //on parcours tous les embranchements de notre arbre pour trouver le plus long chemin, en partant du premier noeud
    //On gardera une liste de toutes les arêtes parcourues afin de faire attention à ne pas parcourir deux fois les mêmes lors d'un chemin
    
    //Liste des segments déjà utilisés dans notre chemin
    list<pair<pair<int, int>, pair<pair<int, int>, int>>> used_elems;
    used_elems.push_back(*corners.equal_range(start).first);
    int len = corners.find(start)->second.second;
    list<int> lengths({ len });
    pair<int, int> node = used_elems.back().second.first;
    while (used_elems.size() > 0) 
    {
        auto its = corners.equal_range(node);
        auto& it = its.first;
        for (; it != its.second; it++) 
        {
            //C'est une intersection qu'on a pas encore explorée
            if (find_if(used_elems.begin(), used_elems.end(), [&](const pair<pair<int, int>, pair<pair<int, int>, int>>& elem) {
                return (elem.first == it->first && elem.second.first == it->second.first && elem.second.second == it->second.second) || 
                    (elem.first == it->second.first && elem.second.first == it->first && elem.second.second == it->second.second);
                }) == used_elems.end())
            {
                //On ajoute un nouveau segment à notre chemin
                used_elems.push_back(*it);
                lengths.push_back(lengths.back() + it->second.second);
                node = it->second.first;
                break;
            }
        }
        if (lengths.back() > len) 
            len = lengths.back();
        //Tous les segments ont déjà été utilisés ou on est arrivé à la fin
        if (it == its.second || node == end) 
        {
            //On revient alors à la node précédente pour tester les autres chemins
            //On retire alors tous les segments partant de la node qu'on retire
            auto itt = find_if(used_elems.begin(), used_elems.end(), [&](const pair<pair<int, int>, pair<pair<int, int>, int>>& elem) {return elem.first == node; });
            used_elems.erase(itt, used_elems.end());
            //La nouvelle node est la node précédente
            node = used_elems.back().first;
            lengths.pop_back();

        }
        
    }

    //TODO


    // 
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
    

#if PART_ONE
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
#endif


#if PART_ONE
    cout << "Part one : " << longest << endl;
#else
    cout << "Part two : " << longest << endl;
#endif
}




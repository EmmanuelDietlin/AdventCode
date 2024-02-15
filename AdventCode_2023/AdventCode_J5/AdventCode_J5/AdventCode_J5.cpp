// AdventCode_J5.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <tuple>
#include <algorithm>
#include <cmath>

using namespace std;

int main()
{
    ifstream input;
    input.open("input.txt");
    string line = "";
    vector<long long> seeds;
    vector<vector<vector<long long>>> maps(7);
    string num;
    std::getline(input, line);
    stringstream ss(line);
    while (ss >> num) {
        if (num.compare("seeds:") == 0) continue;
        seeds.push_back(stoll(num));
    }
    int map_index = 0;
    //Dans cette étape, on va stocker toutes les valeurs des différentes maps dans des listes, afin de pouvoir les 
    //utiliser par la suite
    while (std::getline(input, line)) 
    {
        if (line.compare("") == 0) continue;
        if (line.compare("seed-to-soil map:") == 0) {
            map_index = 0;
        }
        else if (line.compare("soil-to-fertilizer map:") == 0) {
            map_index = 1;
        }
        else if (line.compare("fertilizer-to-water map:") == 0) {
            map_index = 2;
        }
        else if (line.compare("water-to-light map:") == 0) {
            map_index = 3;
        }
        else if (line.compare("light-to-temperature map:") == 0) {
            map_index = 4;
        }
        else if (line.compare("temperature-to-humidity map:") == 0) {
            map_index = 5;
        }
        else if (line.compare("humidity-to-location map:") == 0) {
            map_index = 6;
        }
        else {
            stringstream nums(line);
            vector<long long> values;
            while (nums >> num)
            {
                values.push_back(stoll(num));
            }
            maps[map_index].push_back(values);
        }
    }
    for (int i = 0; i < 7; i++) {
        sort(maps[i].begin(), maps[i].end(), [](const vector<long long>& left, const vector<long long>& right) {return left[1] < right[1]; });
    }
    input.close();

    //PART ONE
    vector<long long> result(seeds);
    //On opère sur les maps successivement afin de trouver au final la valeur de distance correspondante
    //Pour chacune des maps (dans l'ordre)
    for (int i = 0; i < 7; i++) {
        //On regarde pour chacun des éléments de result (qui sont donc successivement : seed - soil - fertilizer - water - light - temp - humid - loc)
        //On cherche la valeur destination correspondante, trouvée via l'utilisation de la map correspondante
        for (auto& it : result) 
        {
            for (auto& itt : maps[i]) {
                //Si on trouve une correspondance dans la table, on modifie en place la valeur,
                //qui servira ainsi dans la prochaine itération de la boucle
                if (it >= itt[1] && it < itt[1] + itt[2]) {
                    it = itt[0] + (it - itt[1]);
                    break;
                }
            }
            //Si on ne trouve pas, la valeur reste la même pour la prochaine itération
        }
    }
    //On récupère enfin la distance la plus courte
    sort(result.begin(), result.end());
    cout << result[0] << endl;

    //PART TWO - idée 1
    // Fonctionne mais prend bcp trop de temsp d'exécution pour le fichier final
    //Ici, les numéros de seeds fonctionnent par paire, avec le deuxième indiquant une range
    //vector<long long> result2;
    //for (int i = 0; i < seeds.size(); i += 2) {
    //    for (int j = seeds[i]; j < seeds[i] + seeds[i + 1]; j++) {
    //        result2.push_back(j);
    //    }
    //}
    //for (int i = 0; i < 7; i++) {
    //    //On regarde pour chacun des éléments de result (qui sont donc successivement : seed - soil - fertilizer - water - light - temp - humid - loc)
    //    //On cherche la valeur destination correspondante, trouvée via l'utilisation de la map correspondante
    //    for (auto& it : result2)
    //    {
    //        for (auto& itt : maps[i]) {
    //            //Si on trouve une correspondance dans la table, on modifie en place la valeur,
    //            //qui servira ainsi dans la prochaine itération de la boucle
    //            if (it >= itt[1] && it < itt[1] + itt[2]) {
    //                it = itt[0] + (it - itt[1]);
    //                break;
    //            }
    //        }
    //        //Si on ne trouve pas, la valeur reste la même pour la prochaine itération
    //        cout << it << endl;
    //    }
    //    cout << endl;
    //}
    ////On récupère enfin la distance la plus courte
    //sort(result2.begin(), result2.end());
    //cout << result2[0];

    //PART TWO - idée 2
    //On travaille en gardant la notion de range, car le travail sur les nombres individuellement prendrait beaucoup
    //trop de ressources, du fait que les ranges peuvent dépasser le milliard
    vector<pair<long long, long long>> seeds_p;
    //On regroupe d'abord les seeds avec leur range
    for (int i = 0; i < seeds.size(); i += 2) {
        seeds_p.push_back({ seeds[i], seeds[i + 1] });
    }
    vector<vector<pair<long long, long long>>> results(8);
    results[0] = seeds_p;
    for (int i = 0; i < 7; i++) {
        //Pour chacun des éléments de results, qui sont des paires de valeur et de range
        for (auto& it : results[i])
        {
            //On stocke les différentes ranges de résultat
            vector<pair<long long, long long>> ranges;
            long long start = it.first, range = it.second;
            //cout << "starting element : " << start << " " << range << endl;
            //On regarde, pour tous les éléments du tableau, ceux dont la range contient tout ou partie
            //la range de l'élément que l'on considère
            //Note : les éléments de la map sont triés par ordre croissant
            for (auto& itt : maps[i]) {
                //cout << "map element : " << itt[0] << " " << itt[1] << " " << itt[2] << endl;
                //Si la range de notre élément démarre avant la range considérée, les éléments jusqu'au début de la range
                //n'ont pas de correspondance et donc le résultant est égal à la source, on stocke une partie de la range dans le tableau résultant
                if (start < itt[1]) {
                    long long diff = min(itt[1] - start, range);
                    if (diff <= 0) break;
                    ranges.push_back({ start, diff });
                    range -= diff;
                    start += diff;
                }
                //Si le début de notre range démarre dans la range de la map, on va extraire la partie correspondante
                //de la range et calculer sa résultante, puis stocker dans le vecteur résultant.
                if (start >= itt[1] && start <= itt[1] + itt[2]) {
                    long long diff = min(itt[2] - (start - itt[1]), range);
                    if (diff <= 0) break;
                    long long new_start = itt[0] + (start - itt[1]);
                    ranges.push_back({ new_start, diff });
                    range -= diff;
                    start += diff;
                }
            }
            //Si il reste des éléments à la fin, on stocke la range telle qu'elle.
            if (range > 0) {
                //cout << "remaining : " << start << " " << range << endl;
                ranges.push_back({ start, range });
            }
            //On stocke ensuite le tableau des ranges pour cette étape dans le vecteur global
            for (auto& it : ranges) {
                //cout << it.first << " " << it.second << endl;
                results[i + 1].push_back(it);
            }

        }
        //cout << endl;
    }
    //On trie par ordre de début de range croissant
    sort(results[7].begin(), results[7].end(), [](const pair<long long, long long>& left, const pair<long long, long long>& right) {return left.first < right.first; });
    cout << results[7][0].first;


}

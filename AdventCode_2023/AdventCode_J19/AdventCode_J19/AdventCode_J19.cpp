// AdventCode_J19.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>

using namespace std;

map<char, int> variables {
    {'x',0},
    {'m',1},
    {'a',2},
    {'s',3}
};

int main()
{
    ifstream input;
    input.open("input.txt");
    string line;
    bool workflow = true;
    //Structure workflow : flz{x<3157:A,x<3324:R,A}
    //On va transcrire la variable et le symbole de comparaison tels que :
    // x = 0, m = 1, a = 2, s = 3
    //et < = 0, > = 1
    //Donc x<3157 peut être converti en 003157
    //On peut alors avoir un vecteur de pair<string, string>, avec string = formule de comparaison et string = opération à effectuer
    //Note : pour le dernier, on aura string = ""
    map<string, vector<pair<string, string>>> workflows;
    vector<map<char, int>> values;
    long long total = 0;
    while (getline(input, line)) 
    {
        if (line.length() == 0) {
            workflow = false;
            continue;
        }
        if (workflow) 
        {
            int i = 0;
            string workflow_name = "";
            //On récupère le nom du workflow
            while (line[i] != '{') 
            {
                workflow_name += line[i];
                i++;
            }
            i++;
            //On récupère les opérations sans le nom du workflow et les accolades
            string substring = line.substr(i, line.length() - 1 - i);
            stringstream ss(substring);
            string operation;
            while (getline(ss, operation, ',')) {
                if (find(operation.begin(), operation.end(), ':') != operation.end()) 
                {
                    string left, right;
                    int index = find(operation.begin(), operation.end(), ':') - operation.begin();
                    left = operation.substr(0, index);
                    right = operation.substr(index+1);
                    workflows[workflow_name].push_back({ left, right });
                }
                else {
                    workflows[workflow_name].push_back({ "", operation });
                }
            }
        }
        else 
        {
            //PART ONE
            //On prend la ligne sans les accolades
            /*string val = line.substr(1, line.length() - 2);
            stringstream ss(val);
            string l;
            map<char, int> variables_value;
            while (getline(ss, l, ',')) 
            {
                variables_value[l[0]] = stoi(l.substr(2));
            }
            values.push_back(variables_value);*/
        }
    }
    input.close();

    //PART ONE
    //for (auto& it : values)
    //{

    //    //On commence par le workflow "in"
    //    //Pour le workflow, si on vérifie la formule de comparaison, on effectue l'opération
    //    //Sinon on continue dans le workflow
    //    string workflow = "in";
    //    int index = 0;
    //    //Tant que la pièce n'est pas acceptée ou rejetée
    //    while (workflow.compare("A") != 0 && workflow.compare("R") != 0)
    //    {
    //        if (workflows[workflow][index].first.length() == 0) {
    //            //On est à la fin du workflow actuel
    //            workflow = workflows[workflow][index].second;
    //            index = 0;
    //        }
    //        else {
    //            char var = workflows[workflow][index].first[0];
    //            char sign = workflows[workflow][index].first[1];
    //            int val = stoi(workflows[workflow][index].first.substr(2));
    //            if (sign == '<') {
    //                //Si on respecte la condition, on prend la valeur comme nouveau workflow
    //                //Et on recommence au début du nouveau workflow 
    //                //Fonctionne également pour les cas A ou R
    //                if (it[var] < val) {
    //                    workflow = workflows[workflow][index].second;
    //                    index = 0;
    //                }
    //                else {
    //                    //Sinon, on continue dans le workflow
    //                    index++;
    //                }
    //            }
    //            else {
    //                //Si on respecte la condition, on prend la valeur comme nouveau workflow
    //                //Et on recommence au début du nouveau workflow 
    //                //Fonctionne également pour les cas A ou R
    //                if (it[var] > val) {
    //                    workflow = workflows[workflow][index].second;
    //                    index = 0;
    //                }
    //                else {
    //                    //Sinon, on continue dans le workflow
    //                    index++;
    //                }
    //            }
    //        }
    //    }
    //    if (workflow.compare("A") == 0) {
    //        total += (it['x'] + it['m'] + it['a'] + it['s']);
    //    }
    //}

    //PART TWO
    //TODO
    //Pour la partie 2, on va transporter les indexs de chaque variable
    // Comme on doit étudier les possiblités pour toutes les combinaisons de variables (dont les valeurs sont comprises chacune entre 1 et 4000)
    // On va ainsi opérer des séparations selon la valeurs de chaque variable
    // Par exemple :
    // Prenons in{s<1351:px,qqz}
    // On va alors avoir une disjonction de cas : tous les s < 1351 partiront sur px, et tous les s>= 1351 continueront dans le workflow (et iront donc à qqz)
    // On travaille donc sur des ranges
    // On va alors stocker dans un vecteur des maps de {x,m,a,s} avec comme valeur des paires, correspondant aux valeurs min et max
    // On devra également y associer le workflow suivant ainsi que l'index dans le workflow
    // vector<pair<map<char, pair<int,int>>, pair<string, int>>
    //TODO

    //Pour optimiser, on va stocker les ranges déjà terminées dans un vecteur à part, pour ne plus avoirà les parcourir inutilement 
    //Dans les boucles suivantes
    vector<pair<map<char, pair<int, int>>, pair<string, int>>> ranges, endRanges;
    map<char, pair<int, int>> first = {
        {'x',{1,4000}},
        {'m',{1,4000}},
        {'a',{1,4000}},
        {'s',{1,4000}}
    };
    //On commenceau workflow "in", à l'index 0;
    ranges.push_back({ first, {"in", 0} });
    while (true) 
    {
        vector<pair<map<char, pair<int, int>>, pair<string, int>>> newRanges;
        //Dès lors qu'on n'a plus de range à calculer, on s'arrête
        if (ranges.size() == 0) {
            break;
        }
        for (auto& it : ranges) 
        {
            //Si notre range à pour workflow suivant 'R' ou 'A', on la stocke dans la liste finale et on passe à la range suivante
            //On ne met dans la liste de fin que les ranges étant acceptées ("A")
            if (it.second.first.compare("A") == 0 || it.second.first.compare("R") == 0) {
                if (it.second.first.compare("A") == 0) {
                    endRanges.push_back(it);
                }
                continue;
            }
            else {
                string workflow = it.second.first;
                int index = it.second.second;
                if (workflows[workflow][index].first.length() == 0) {
                    //On est à la fin du workflow actuel
                    //On passe donc au workflow demandé et on remet l'index à 0
                    pair<map<char, pair<int, int>>, pair<string, int>> next(it);
                    next.second.first = workflows[workflow][index].second;
                    next.second.second = 0;
                    newRanges.push_back(next);
                }
                else {
                    char var = workflows[workflow][index].first[0];
                    char sign = workflows[workflow][index].first[1];
                    int val = stoi(workflows[workflow][index].first.substr(2));
                    if (sign == '<') {
                        //On va alors regarder si la valeur de comparaison est contenue dans la range de la variable
                        pair<int, int> range = it.first[var];
                        if (range.second < val) {
                            //Notre range est entièrement inférieure à la valeur
                            // Elle respecte donc la condition
                            //On change donc son workflow et l'index de départ est remis à 0
                            pair<map<char, pair<int, int>>, pair<string, int>> next(it);
                            next.second.first = workflows[workflow][index].second;
                            next.second.second = 0;
                            newRanges.push_back(next);
                        }
                        else if (range.first > val) 
                        {
                            //Notre range est entièrement supérieure à la valeur
                            //Elle ne respecte donc pas la condition
                            //On continue donc dans le workflow à l'index suivant
                            pair<map<char, pair<int, int>>, pair<string, int>> next(it);
                            next.second.second++;
                            newRanges.push_back(next);
                        }
                        else {
                            //On a une partie de la range qui respecte la condition et une autre partie non
                            pair<map<char, pair<int, int>>, pair<string, int>> next_left(it), next_right(it);

                            //La borne haute de la partie gauche est égale à la limite - 1
                            next_left.first[var].second = val - 1;
                            //La partie gauche passe ensuite à un autre workflow
                            next_left.second.first = workflows[workflow][index].second;
                            next_left.second.second = 0;

                            //La borne basse de la partie droite est égale à la limite
                            next_right.first[var].first = val;
                            //La partie droite continue dans le même workflow
                            next_right.second.second++;

                            newRanges.push_back(next_left);
                            newRanges.push_back(next_right);
                        }
                    }
                    else {
                        //On va alors regarder si la valeur de comparaison est contenue dans la range de la variable
                        pair<int, int> range = it.first[var];
                        if (range.first > val) {
                            //Notre range est entièrement supérieure à la valeur
                            // Elle respecte donc la condition
                            //On change donc son workflow et l'index de départ est remis à 0
                            pair<map<char, pair<int, int>>, pair<string, int>> next(it);
                            next.second.first = workflows[workflow][index].second;
                            next.second.second = 0;
                            newRanges.push_back(next);
                        }
                        else if (range.second < val)
                        {
                            //Notre range est entièrement inférieure à la valeur
                            //Elle ne respecte donc pas la condition
                            //On continue donc dans le workflow à l'index suivant
                            pair<map<char, pair<int, int>>, pair<string, int>> next(it);
                            next.second.second++;
                            newRanges.push_back(next);
                        }
                        else {
                            //On a une partie de la range qui respecte la condition et une autre partie non
                            pair<map<char, pair<int, int>>, pair<string, int>> next_left(it), next_right(it);

                            //La borne haute de la partie gauche est égale à la limite
                            next_left.first[var].second = val;
                            //La partie gauche reste sur le même workflow
                            next_left.second.second++;

                            //La borne basse de la partie droite est égale à la limite + 1
                            next_right.first[var].first = val+1;
                            //La partie droite passe dans le workflow suivant
                            next_right.second.first = workflows[workflow][index].second;
                            next_right.second.second = 0;

                            newRanges.push_back(next_left);
                            newRanges.push_back(next_right);
                        }
                    }
                }
            }
        }
        ranges = newRanges;

    }
    //Compter le nombre de possibilités totales
    for (auto& it : endRanges) 
    {
        //On ajoute un à la soustraction car la soustraction nous donne n-1 éléments
        total += (long long)(it.first['x'].second - it.first['x'].first + 1) * (long long)(it.first['m'].second - it.first['m'].first + 1) *
            (long long)(it.first['a'].second - it.first['a'].first + 1) * (long long)(it.first['s'].second - it.first['s'].first + 1);
    }

    cout << total << endl;
}


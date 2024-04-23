// AdventCode_J13.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//
#define PART_TWO true;

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

int compare_columns(vector<string>& pattern, int left_index, int right_index) 
{
    int differences = 0;
    for (int i = 0; i < pattern.size(); i++) 
    {
        if (pattern[i][left_index] != pattern[i][right_index]) {
            differences++;
        }
    }
    return differences;
}

int compare_lines(vector<string>& pattern, int first, int second) 
{
    int differences = 0;
    for (int i = 0; i < pattern[first].size(); i++) 
    {
        if (pattern[first][i] != pattern[second][i]) {
            differences++;
        }
    }
    return differences;
}


int main()
{
    ifstream input;
    vector<vector<string>> patterns;
    string line;
    vector<string> pattern;
    vector<int> vertical;
    vector<int> horizontal;
    input.open("input.txt");
    while (getline(input, line)) 
    {
        if (line.length() == 0)
        {
            patterns.push_back(pattern);
            pattern.clear();
            continue;
        }
        pattern.push_back(line);
    }
    patterns.push_back(pattern);
    input.close();
    for (auto& it : patterns) 
    {
#if !PART_TWO
        bool found_symetry = false, left_symetry = false, right_symetry = false;
        //Algo : si nombre impair de lignes, commencer à partir de la ligne 1 jusqu'à n-1 si égales, sinon commencer de la ligne 2 jusqu'à n
        //Si nombre pair, commencer de 1 à n
        //Chercher si toutes les lignes sont bien identiques en symétrie au fur et à mesure du parcours
        int startIndex = 0, endIndex = 0;
        pair<int, int> leftIndexes(0, 0), rightIndexes(0, 0);
        //On parcours simultanément les lignes par le début et par la fin
        //On compare la ligne 0 avec les lignes parcourues par la fin et la ligne de fin avec les lignes parcourues par le début
        //Si on trouve à un moment une correspondance, on a alors trouvé les bornes de notre potentielle symétrie
        //On compare alors les lignes à l'intérieur par symétrie

        //Problème du parcours simultané par la gauche et la droite
        //Si jamais on tombe sur un cas où on a à la fois une correspondance pour le parcours de haut en bas et de bas en haut, un des deux sera privilégié
        //Et si ce cas est faux, on ne reviendra jamais sur l'autre cas, qui peut s'avérer être le bon
        //3 solutions :
        // - Faire deux boucles séparées, une pour chaque type de parcours (si jamais la première n'a pas donné de résultat
        // - Faire une sous boucle dès que l'on a trouvé des bornes pour vérifier si on a bien une symétrie parfaite
        // - Utiliser des variables différentes pour stocker les bornes de recherche par la droite et par la gauche

        //Je vais ici utiliser la solution 3, ie des variables différentes
        for (int i = it.size() - 1, j = 0; i > 0 && j < it.size() - 1; i--, j++) 
        {
            //On observe donc au maximum deux symétries en simultané, puisque l'on parcours les lignes dans deux sens différents en même temps
            if (left_symetry == true) 
            {
                //On calcule le décalage différement selon si on est dans le cas où c'est la ligne la plus à gauche ou la plus à droite qui est un des bords de la symétrie
                int shift = leftIndexes.second - i;
                if (leftIndexes.first + shift < leftIndexes.second - shift)
                {
                    if (it[leftIndexes.first + shift].compare(it[leftIndexes.second - shift]) != 0)
                    {
                        //On n'a pas une symétrie parfaite à l'intérieur des bornes
                        //On reprend donc notre recherche en commençant une ligne plus haut ou plus bas que la borne précédente
                        left_symetry = false;
                        i += (shift - 1);
                    }
                    //Si les colonnes sont identiques, on vérifie les suivantes
                    else {
                        continue;
                    }
                }
                //On s'arrête quand startIndex + shift >= endIndex - shift pour éviter de comparer deux fois les mêmes lignes
                else {
                    //On a trouvé notre symétrie, pour éviter de potentiels résidus on met bien la valeur de left_symetry à false pour
                    //Indiquer que la symétrie est bien par la gauche
                    right_symetry = false;
                    break;
                }
                
            }
            if (right_symetry == true)
            {
                //On calcule le décalage différement selon si on est dans le cas où c'est la ligne la plus à gauche ou la plus à droite qui est un des bords de la symétrie
                int shift = j - rightIndexes.first;
                if (rightIndexes.first + shift < rightIndexes.second - shift)
                {
                    if (it[rightIndexes.first + shift].compare(it[rightIndexes.second - shift]) != 0)
                    {
                        //On n'a pas une symétrie parfaite à l'intérieur des bornes
                        //On reprend donc notre recherche en commençant une ligne plus haut ou plus bas que la borne précédente
                        right_symetry = false;
                        j -= (shift - 1);
                    }
                    //Si les colonnes sont identiques, on vérifie les suivantes
                    else {
                        continue;
                    }
                }
                //On s'arrête quand startIndex + shift >= endIndex - shift pour éviter de comparer deux fois les mêmes lignes
                else {
                    //On a trouvé notre symétrie, pour éviter de potentiels résidus on met bien la valeur de left_symetry à false pour
                    //Indiquer que la symétrie est bien par la droite
                    left_symetry = false;
                    break;
                }

            }
            //il faut qu'on ai un nombre pair de lignes car sinon la ligne de symétrie ne pourra pas se trouver ENTRE deux lignes
            if (it[i].compare(it[0]) == 0 && (i+1)%2 == 0) 
            {
                leftIndexes.first = 0;
                leftIndexes.second = i;
                left_symetry = true;
            }
            //il faut qu'on ai un nombre pair de lignes car sinon la ligne de symétrie ne pourra pas se trouver ENTRE deux lignes
            if (it[j].compare(it[it.size() - 1]) == 0 && (it.size() - 1 - j + 1)% 2 == 0) {
                rightIndexes.first = j;
                rightIndexes.second = it.size() - 1;
                right_symetry = true;
            }
        }
        if (left_symetry == true)
        {
            //On a l'indice de la dernière ligne AVANT la ligne de symétrie, et on ajoute 1 pour avoir le nombre total de lignes (car indices commencent à 0)
            cout << ((float)leftIndexes.second - leftIndexes.first) / (float)2 << endl;
            horizontal.push_back(1 + leftIndexes.first + (leftIndexes.second - leftIndexes.first) / 2);
            std::cout << "horizontal " << leftIndexes.first << " : " << leftIndexes.second << " length : " << it.size() << endl;
        }
        if (right_symetry == true) {
            //On a l'indice de la dernière ligne AVANT la ligne de symétrie, et on ajoute 1 pour avoir le nombre total de lignes (car indices commencent à 0)
            cout << ((float)rightIndexes.second - rightIndexes.first) / (float)2 << endl;
            horizontal.push_back(1 + rightIndexes.first + (rightIndexes.second - rightIndexes.first) / 2);
            std::cout << "horizontal " << rightIndexes.first << " : " << rightIndexes.second << " length : " << it.size() << endl;
        }
        
        found_symetry = left_symetry || right_symetry;
        left_symetry = right_symetry = false;

        //Algo : si nombre impair de colonnes, commencer à partir de la colonne 1 jusqu'à n-1 si égales, sinon commencer de la colonne 2 jusqu'à n
        //Si nombre pair, commencer de 1 à n
        //Chercher si toutes les colonne sont bien identiques en symétrie au fur et à mesure du parcours        
        //Si on n'a pas trouvé de symétrie horizontale, on cherche une symétrie verticale

        //On parcours simultanément les colonnes par le début et par la fin
        //On compare la colonne 0 avec les colonnes parcourues par la fin et la colonne de fin avec les colonnes parcourues par le début
        //Si on trouve à un moment une correspondance, on a alors trouvé les bornes de notre potentielle symétrie
        //On compare alors les colonnes à l'intérieur par symétrie
        if (found_symetry == false)
        {
            for (int i = it[0].length() - 1, j = 0; i > 0 && j < it[0].length() - 1; i--, j++)
            {
                if (left_symetry == true)
                {
                    //On calcule le décalage différement selon si on est dans le cas où c'est la colonne la plus à gauche ou la plus à droite qui est un des bords de la symétrie
                    int shift = leftIndexes.second - i;
                    if (leftIndexes.first + shift < leftIndexes.second - shift)
                    {
                        if (compare_columns(it, leftIndexes.first + shift, leftIndexes.second - shift) != 0)
                        {
                            //On n'a pas une symétrie parfaite à l'intérieur des bornes
                            //On reprend donc notre recherche en commençant une colonne plus à gauche ou plus à droite que la borne précédente
                            left_symetry = false;
                            i += (shift - 1);
                        }
                        //Si les colonnes sont identiques, on vérifie les suivantes
                        else {
                            continue;
                        }
                    }
                    //On s'arrête quand startIndex + shift >= endIndex - shift pour éviter de comparer deux fois les mêmes colonnes
                    else {
                        right_symetry = false;
                        break;
                    }
                }
                if (right_symetry == true)
                {
                    //On calcule le décalage différement selon si on est dans le cas où c'est la colonne la plus à gauche ou la plus à droite qui est un des bords de la symétrie
                    int shift = j - rightIndexes.first;
                    if (rightIndexes.first + shift < rightIndexes.second - shift)
                    {
                        if (compare_columns(it, rightIndexes.first + shift, rightIndexes.second - shift) != 0)
                        {
                            //On n'a pas une symétrie parfaite à l'intérieur des bornes
                            //On reprend donc notre recherche en commençant une colonne plus à gauche ou plus à droite que la borne précédente
                            right_symetry = false;
                            j -= (shift - 1);
                        }
                        //Si les colonnes sont identiques, on vérifie les suivantes
                        else {
                            continue;
                        }
                    }
                    //On s'arrête quand startIndex + shift >= endIndex - shift pour éviter de comparer deux fois les mêmes colonnes
                    else {
                        left_symetry = false;
                        break;
                    }
                }
                //il faut qu'on ai un nombre pair de colonnes dans notre symétrie car sinon la ligne de symétrie ne pourra pas se trouver entre deux colonnes
                if (compare_columns(it, 0, i) == 0 && (i + 1) % 2 == 0)
                {
                    leftIndexes.first = 0;
                    leftIndexes.second = i;
                    left_symetry = true;
                }
                //il faut qu'on ai un nombre pair de colonnes car sinon la ligne de symétrie ne pourra pas se trouver entre deux colonnes
                if (compare_columns(it, j, it[0].length() - 1) == 0 && (it[0].length() - 1 - j + 1) % 2 == 0) {
                    rightIndexes.first = j;
                    rightIndexes.second = it[0].length() - 1;
                    right_symetry = true;
                }
            }
            if (left_symetry == true)
            {
                cout << ((float)leftIndexes.second - leftIndexes.first) / (float)2 << endl;
                //On a l'indice de la dernière colonne AVANT la ligne de symétrie, et on ajoute 1 pour avoir le nombre total de colonnes (car indices commencent à 0)
                vertical.push_back(1 + leftIndexes.first + (leftIndexes.second - leftIndexes.first) / 2);
                std::cout << "vertical " << leftIndexes.first << " : " << leftIndexes.second << " length : " << it[0].size() << endl;
            }
            if (right_symetry == true) {
                cout << ((float)rightIndexes.second - rightIndexes.first) / (float)2 << endl;
                //On a l'indice de la dernière colonne AVANT la ligne de symétrie, et on ajoute 1 pour avoir le nombre total de colonnes (car indices commencent à 0)
                vertical.push_back(1 + rightIndexes.first + (rightIndexes.second - rightIndexes.first) / 2);
                std::cout << "vertical " << rightIndexes.first << " : " << rightIndexes.second << " length : " << it[0].size() << endl;
            }
        }
#endif
#if PART_TWO
        bool found_symetry = false, left_symetry = false, right_symetry = false;
        //Algo : si nombre impair de lignes, commencer à partir de la ligne 1 jusqu'à n-1 si égales, sinon commencer de la ligne 2 jusqu'à n
        //Si nombre pair, commencer de 1 à n
        //Chercher si toutes les lignes sont bien identiques en symétrie au fur et à mesure du parcours
        int startIndex = 0, endIndex = 0;
        pair<int, int> leftIndexes(0, 0), rightIndexes(0, 0);
        int left_smudges = 0, right_smudges = 0;
        
        rightIndexes.second = it.size() - 1;
        leftIndexes.first = 0;
        //Par rapport à la partie 1, on va compter le nombre de différences dans la réflection
        //Si on trouve UNE SEULE différence, alors on pourra prendre cette réflection comme la nouvelle bonne refléction, car
        //il suffira de changer UN seul symbole pour la rendre valide
        //Si on en trouve 0, on exclu cette solution (c'est la précédente bonne solution)
        //Si on en trouve 2 ou plus, on peut s'arrêter sans aller au bout, car changer un seul élément ne rendra pas la réflection valide
        for (int i = it.size() - 1, j = 0; i > 0 && j < it.size() - 1; i--, j++)
        {
            rightIndexes.first = j;
            leftIndexes.second = i;
            left_smudges = 0;
            right_smudges = 0;
            for (int k = i, l = j; k > 0 && l < it.size() - 1; k--, l++) 
            {
                //On n'aura pas de symétrie, on peut quitter la boucle
                if (left_smudges > 1 && right_smudges > 1) break;
                int shift = leftIndexes.second - k;
                //On s'arrête de vérifier si on a plus d'une différence dans la symétrie
                if (leftIndexes.first + shift < leftIndexes.second - shift && left_smudges <= 1) 
                {
                    left_smudges += compare_lines(it, leftIndexes.first + shift, leftIndexes.second - shift);
                }
                shift = l - rightIndexes.first;
                //On s'arrête de vérifier si on a plus d'une différence dans la symétrie
                if (rightIndexes.first + shift < rightIndexes.second - shift && right_smudges <= 1) 
                {
                    right_smudges += compare_lines(it, rightIndexes.first + shift, rightIndexes.second - shift);
                }
            }
            //On fait attention à avoir une ligne de reflection entre deux lignes et non pas sur une ligne
            if (left_smudges == 1 && (leftIndexes.second - leftIndexes.first + 1) % 2 == 0) {
                left_symetry = true;
                right_symetry = false;
                break;
            }
            //On fait attention à avoir une ligne de reflection entre deux lignes et non pas sur une ligne
            if (right_smudges == 1 && (rightIndexes.second - rightIndexes.first + 1) % 2 == 0)
            {
                right_symetry = true;
                left_symetry = false;
                break;
            }

            
        }
        if (left_symetry == true)
        {
            //On a l'indice de la dernière ligne AVANT la ligne de symétrie, et on ajoute 1 pour avoir le nombre total de lignes (car indices commencent à 0)
            cout << ((float)leftIndexes.second - leftIndexes.first) / (float)2 << endl;
            horizontal.push_back(1 + leftIndexes.first + (leftIndexes.second - leftIndexes.first) / 2);
            std::cout << "horizontal " << leftIndexes.first << " : " << leftIndexes.second << " length : " << it.size() << endl;
        }
        if (right_symetry == true) {
            //On a l'indice de la dernière ligne AVANT la ligne de symétrie, et on ajoute 1 pour avoir le nombre total de lignes (car indices commencent à 0)
            cout << ((float)rightIndexes.second - rightIndexes.first) / (float)2 << endl;
            horizontal.push_back(1 + rightIndexes.first + (rightIndexes.second - rightIndexes.first) / 2);
            std::cout << "horizontal " << rightIndexes.first << " : " << rightIndexes.second << " length : " << it.size() << endl;
        }

        found_symetry = left_symetry || right_symetry;
        left_symetry = right_symetry = false;
        left_smudges = 0;
        right_smudges = 0;

        rightIndexes.second = it[0].length() - 1;
        leftIndexes.first = 0;
        
        if (found_symetry == false)
        {
            for (int i = it[0].length() - 1, j = 0; i > 0 && j < it[0].length() - 1; i--, j++)
            {
                rightIndexes.first = j;
                leftIndexes.second = i;
                left_smudges = 0;
                right_smudges = 0;
                for (int k = i, l = j; k > 0 && l < it[0].length() - 1; k--, l++)
                {
                    //On n'aura pas de symétrie, on peut quitter la boucle
                    if (left_smudges > 1 && right_smudges > 1) break;
                    int shift = leftIndexes.second - k;
                    if (leftIndexes.first + shift < leftIndexes.second - shift && left_smudges <= 1)
                    {
                        left_smudges += compare_columns(it, leftIndexes.first + shift, leftIndexes.second - shift);
                    }
                    shift = l - rightIndexes.first;
                    if (rightIndexes.first + shift < rightIndexes.second - shift && right_smudges <= 1)
                    {
                        right_smudges += compare_columns(it, rightIndexes.first + shift, rightIndexes.second - shift);
                    }
                }
                if (left_smudges == 1 && (leftIndexes.second - leftIndexes.first + 1) % 2 == 0) {
                    left_symetry = true;
                    right_symetry = false;
                    break;
                }
                if (right_smudges == 1 && (rightIndexes.second - rightIndexes.first + 1) % 2 == 0)
                {
                    right_symetry = true;
                    left_symetry = false;
                    break;
                }
            }
            if (left_symetry == true)
            {
                cout << ((float)leftIndexes.second - leftIndexes.first) / (float)2 << endl;
                //On a l'indice de la dernière colonne AVANT la ligne de symétrie, et on ajoute 1 pour avoir le nombre total de colonnes (car indices commencent à 0)
                vertical.push_back(1 + leftIndexes.first + (leftIndexes.second - leftIndexes.first) / 2);
                std::cout << "vertical " << leftIndexes.first << " : " << leftIndexes.second << " length : " << it[0].size() << endl;
            }
            if (right_symetry == true) {
                cout << ((float)rightIndexes.second - rightIndexes.first) / (float)2 << endl;
                //On a l'indice de la dernière colonne AVANT la ligne de symétrie, et on ajoute 1 pour avoir le nombre total de colonnes (car indices commencent à 0)
                vertical.push_back(1 + rightIndexes.first + (rightIndexes.second - rightIndexes.first) / 2);
                std::cout << "vertical " << rightIndexes.first << " : " << rightIndexes.second << " length : " << it[0].size() << endl;
            }
        }
#endif
    }

    int result = 0;
    int count = 0;
    for (auto& it : vertical) {
        result += it;
        count++;
    }
    for (auto& it : horizontal) {
        result += 100 * it;
        count++;
    }
    cout << result << endl;
    cout << count << endl;
}


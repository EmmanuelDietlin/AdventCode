// AdventCode_J18.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
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

map<char, pair<long, long>> directions{
    {'U',{0,-1}},
    {'D',{0,1}},
    {'L',{-1,0}},
    {'R',{1,0}}
};

int main()
{
    ifstream input;
    long long total = 0;
    input.open("input.txt");
    vector<pair<long, long>> corners;
    vector<pair<pair<char, long>, pair<long, long>>> points;
    //Coordonnées du coin HAUT GAUCHE du point actuel
    pair<long, long> point({ 0,0 });
    corners.push_back(point);
    string line;
    long long perimeter = 0;
    //Note : on stocke deux fois 0,0 car pour le dernier calcul de lacet on en aura besoin
    //Il nous est nécessaire d'adapter nore système de coordonnées
    //Les coordonnées que l'on calcule nous donnent la position d'une case
    //Or, les cases sont de surface non nulle, égale à 1.
    //La méthode des lacets suppose que l'on travaille avec une bordure d'épaisseur nulle
    //On va donc considérer une case comme étant 4 coordonnnées, 1 pour chacun de ses sommets, et travailler avec lesdites coordonnées
    //On va stocker les coordonnées des cases se trouvant le plus à l'extérieur possible de la figure
    //En reliant ces points, on aurait alors la figure englobant nos cases avec une bordure d'épaisseur nulle
    //Par exemple, pour ce morceau de figure :
    // #####
    //     # 
    //    ##
    //On utilisera respectivement la coordonnée HAUT GAUCHE, HAUT DROITE et BAS DROITE pour nos calculs suivants
    //On travaillera en utilisant le coin HAUT GAUCHE à chaque fois, et on ajoutera la quantité manquante pour le stockage dans notre vecteur
    //Comme pour déterminer le coin à garder on a besoin de connaître la direction actuelle ainsi que la direction suivante du graphe, on va d'abord tout stocker dans une liste avant de calculer nos coordonnées

    while (getline(input, line)) 
    {
        string subline;
        char direction = 'U';
        stringstream ss(line);
        while (getline(ss, subline, ' ')) {
            //PART ONE
            //Direction
            //if (subline[0] >= 'A' && subline[0] <= 'Z') {
            //    direction = subline[0];
            //}
            ////Couleur
            //else if (subline[0] == '(') {
            //    //On traitera le cas de la couleur plus tard
            //}
            //else {
            //    int length = stoi(subline);
            //    point.first += directions[direction].first * length;
            //    point.second += directions[direction].second * length;
            //    points.push_back({ { direction, length }, point });
            //}

            //PART TWO
            //On ne récupère que le chiffre en hexadécimal
            if (subline[0] == '(') {
                long length = 0;
                //LA structure de l'hexa est la suivante : (#aaaaa0)
                switch (subline[7]) {
                case '0':
                    direction = 'R';
                    break;
                case '1':
                    direction = 'D';
                    break;
                case '2':
                    direction = 'L';
                    break;
                case '3':
                    direction = 'U';
                    break;
                }
                string number = subline.substr(2, 5);
                length = stoll(number, 0, 16);
                perimeter += length;
                point.first += directions[direction].first * length;
                point.second += directions[direction].second * length;
                points.push_back({ { direction, length }, point });
            }
        }
    }
    input.close();
    //On rajoute le premier mouvement à la fin car on en aura besoin pour calculer le dernier point de coordonnée qui va relier (0,0) pour boucler la figure
    //points.push_back(points[0]);
    //char inside = 'U';

    //METHODE 1
    // On détermine de quel côté du départ se trouve l'intérieur de la figure, en utilisant le premier segment de la figure
    // On prend un point au hasard d'un côté ou de l'autre du segment de départ
    // On trace ensuite la droite entre ce point et l'infini
    // On compte combien de segments de la figure elle traverse. Si nombre impair, alors à l'intérieur
    // Si nombre pair, à l'extérieur
    // On peut ainsi déterminer de quel côté du segment est l'intérieur de la figure
    //
    //pair<long, long> first_point(1,1);
    ////Si le nombre de coupes est pair, alors on est à l'extérieur
    ////Sinon on est à l'intérieur
    //long cut_count = 0;
    ////On va ensuite prendre la droite à partir du point jusqu'au plus à gauche possible, et compter combien de segments verticaux on traverse
    ////Pour ainsi déterminer si on est à l'intérieur ou l'extérieur
    //for (auto& it : points) 
    //{
    //    //On ne va considérer que les points résultants d'un déplacement VERTICAL et dont la droite ne les traverse pas sur les extrémités
    //    //Donc la coordonnée de fin doit être > ou < à la coordonnée y du point
    //    //Et la coordonnée x doit être < à la coordonnée x du point 
    //    //Ainsi, tout segment vertical à gauche du point, dont les extrémités sont strictement plus hautes et basses que le point, sont considérées comme coupées
    //    if (it.second.first < first_point.first) 
    //    {
    //        if (it.first.first == 'D' && it.second.second > first_point.second && it.second.second - it.first.second < first_point.second) 
    //        {
    //            cut_count++;
    //        }
    //        else if (it.first.first == 'U' && it.second.second < first_point.second && it.second.second + it.first.second > first_point.second) 
    //        {
    //            cut_count++;
    //        }
    //    }
    //}

    //switch (points[0].first.first) 
    //{
    //case 'R':
    //    //Le point est en bas par rapport au premier segment, donc si il n'est pas dans l'intérieur celui-ci est vers le haut pour ce premier segment
    //    inside = (cut_count % 2 == 0) ? 'U' : 'D';
    //    break;
    //case 'U':
    //    //Le point est à droite par rapport au premier segment, donc si il n'est pas dans l'intérieur celui-ci est vers la gauche pour ce premier segment
    //    inside = (cut_count % 2 == 0) ? 'L' : 'R';
    //    break;
    //case 'L':
    //    //Le point est en bas par rapport au premier segment, donc si il n'est pas dans l'intérieur celui-ci est vers le haut pour ce premier segment
    //    inside = (cut_count % 2 == 0) ? 'U' : 'D';
    //    break;
    //case 'D':
    //    //Le point est à droite par rapport au premier segment, donc si il n'est pas dans l'intérieur celui-ci est vers la gauche pour ce premier segment
    //    inside = (cut_count % 2 == 0) ? 'L' : 'R';
    //    break;
    //}
    //
    ////On calcule maintenant les coordonnées de la frontière englobant la figure
    ////(On commence à 1 car on a déjà traité le 1er segment)
    //for (int i = 0; i < points.size() - 1; i++)
    //{        

    //    //On détermine la direction de l'intérieur par rapport à la rotation entre la direction actuelle et la direction suivante appliquée a la direction précédente de l'intérieur
    //    // Ex : si intérieur vers le bas, direction actuelle vers la droite et direction suivante vers le bas, alors intérieur vers le bas
    //    // ==> rotation de la direction de l'intérieur entre 2 mouvements de 90° dans le même sens que la rotation entre 2 mouvements
    //    //On détermine alors quelle coordonnée utiliser pour la frontière (à l'opposé de l'intérieur)
    //    switch (points[i].first.first) 
    //    {
    //    case 'U':
    //        if (points[i + 1].first.first == 'L') 
    //        {
    //            //Si on part en haut puis à gauche et que l'intérieur est à gauche, on prend le coin HAUT DROITE
    //            if (inside == 'L') 
    //            {
    //                corners.push_back({ points[i].second.first + 1, points[i].second.second });
    //                inside = 'D';
    //            }
    //            //Si on part en haut puis à gauche et que l'intérieur est à droite, on prend le coin BAS GAUCHE
    //            else 
    //            {
    //                corners.push_back({ points[i].second.first, points[i].second.second + 1 });
    //                inside = 'U';
    //            }
    //        }
    //        else 
    //        {
    //            //Si on part en haut puis à droite et que l'intérieur est à gauche, on prend le coin BAS DROITE
    //            if (inside == 'L')
    //            {
    //                corners.push_back({ points[i].second.first + 1, points[i].second.second + 1 });
    //                inside = 'U';
    //            }
    //            //Si on part en haut puis à droite et que l'intérieur est à droite, on prend le coin HAUT GAUCHE
    //            else
    //            {
    //                corners.push_back(points[i].second);
    //                inside = 'D';
    //            }
    //        }
    //        break;
    //    case 'D':
    //        if (points[i + 1].first.first == 'L')
    //        {
    //            //Si on part en bas puis à gauche et que l'intérieur est à gauche, on prend le coin BAS DROITE
    //            if (inside == 'L')
    //            {
    //                corners.push_back({ points[i].second.first + 1, points[i].second.second + 1});
    //                inside = 'U';
    //            }
    //            //Si on part en bas puis à gauche et que l'intérieur est à droite, on prend le coin HAUT GAUCHE
    //            else
    //            {
    //                corners.push_back(points[i].second);
    //                inside = 'D';
    //            }
    //        }
    //        else
    //        {
    //            //Si on part en bas puis à droite et que l'intérieur est à gauche, on prend le coin HAUT DROITE
    //            if (inside == 'L')
    //            {
    //                corners.push_back({ points[i].second.first + 1, points[i].second.second });
    //                inside = 'D';
    //            }
    //            //Si on part en bas puis à droite et que l'intérieur est à droite, on prend le coin BAS GAUCHE
    //            else
    //            {
    //                corners.push_back({ points[i].second.first, points[i].second.second + 1 });
    //                inside = 'U';
    //            }
    //        }
    //        break;
    //    case 'L':
    //        if (points[i + 1].first.first == 'D')
    //        {
    //            //Si on part à gauche puis en bas et que l'intérieur est en bas, on prend le coin HAUT GAUCHE
    //            if (inside == 'D')
    //            {
    //                corners.push_back(points[i].second);
    //                inside = 'R';
    //            }
    //            //Si on part à gauche puis en bas et que l'intérieur est à en haut, on prend le coin BAS DROITE
    //            else
    //            {
    //                corners.push_back({ points[i].second.first + 1, points[i].second.second + 1 });
    //                inside = 'L';
    //            }
    //        }
    //        else
    //        {
    //            //Si on part à gauche puis en haut et que l'intérieur est en bas, on prend le coin HAUT DROITE
    //            if (inside == 'D')
    //            {
    //                corners.push_back({ points[i].second.first + 1, points[i].second.second });
    //                inside = 'L';
    //            }
    //            //Si on part à gauche puis en haut et que l'intérieur est en haut, on prend le coin BAS GAUCHE
    //            else
    //            {
    //                corners.push_back({ points[i].second.first, points[i].second.second + 1 });
    //                inside = 'R';
    //            }
    //        }
    //        break;
    //    case 'R':
    //        if (points[i + 1].first.first == 'D')
    //        {
    //            //Si on part à droite puis en bas et que l'intérieur est en bas, on prend le coin HAUT DROITE
    //            if (inside == 'D')
    //            {
    //                corners.push_back({ points[i].second.first + 1, points[i].second.second });
    //                inside = 'L';
    //            }
    //            //Si on part à droite puis en bas et que l'intérieur est en haut, on prend le coin BAS GAUCHE
    //            else
    //            {
    //                corners.push_back({ points[i].second.first, points[i].second.second + 1 });
    //                inside = 'R';
    //            }
    //        }
    //        else
    //        {
    //            //Si on part à droite puis en haut et que l'intérieur est en bas, on prend le coin HAUT GAUCHE
    //            if (inside == 'D') {
    //            
    //                corners.push_back(points[i].second);
    //                inside = 'R';
    //            }
    //            //Si on part à droite puis en haut et que l'intérieur est en haut, on prend le coin BAS DROITE
    //            else
    //            {
    //                corners.push_back({ points[i].second.first + 1, points[i].second.second + 1 });
    //                inside = 'L';
    //            }
    //        }
    //        break;
    //    }

    //}

    ////On va appliquer la méthode des lacets
    ////http://villemin.gerard.free.fr/GeomLAV/Polygone/Lacet.htm
    //for (int i = 0; i < corners.size() - 1; i++) 
    //{
    //    
    //    //OLD
    //    ////ON a des problèmes d'overflow de long long lors des multiplications, on va donc utiliser des unsigned long long
    //    ////Et garder le signe à part, qu'on utilisera ensuite pour déterminer si on doit additionner ou soustraire le résultat
    //    //unsigned long long left = 0, right = 0;
    //    //int left_sign = 1, right_sign = 1;
    //    //if (corners[i].first < 0 && corners[i + 1].second < 0) {
    //    //    if (corners[i].first != 0 && ULLONG_MAX / (-1 * corners[i].first) < (-1 * corners[i + 1].second)) cout << "OVERFLOW" << endl;
    //    //    left = (-1 * corners[i].first) * (-1 * corners[i + 1].second);
    //    //    left_sign = 1;
    //    //}
    //    //else if (corners[i].first < 0) {
    //    //    if (corners[i].first != 0&&ULLONG_MAX / (-1 * corners[i].first) < corners[i + 1].second) cout << "OVERFLOW" << endl;
    //    //    left = (-1 * corners[i].first) * corners[i + 1].second;
    //    //    left_sign = -1;
    //    //}
    //    //else if (corners[i + 1].second < 0) {
    //    //    if (corners[i].first != 0 && ULLONG_MAX / corners[i].first < (-1 * corners[i + 1].second)) cout << "OVERFLOW" << endl;
    //    //    left = corners[i].first * (-1 * corners[i + 1].second);
    //    //    left_sign = -1;
    //    //}
    //    //else {
    //    //    if (corners[i].first != 0 && ULLONG_MAX / corners[i].first < corners[i + 1].second) cout << "OVERFLOW" << endl;
    //    //    left = corners[i].first * corners[i + 1].second;
    //    //    left_sign = 1;
    //    //}
    //    ////Même chose avec la droite du calcul
    //    //if (corners[i].second < 0 && corners[i + 1].first < 0) {
    //    //    if (corners[i].second != 0 && ULLONG_MAX / (-1 * corners[i].second) < (-1 * corners[i + 1].first)) cout << "OVERFLOW" << endl;
    //    //    right = (-1 * corners[i].second) * (-1 * corners[i + 1].first);
    //    //    right_sign = 1;
    //    //}
    //    //else if (corners[i].first < 0) {
    //    //    if (corners[i].second != 0 && ULLONG_MAX / (-1 * corners[i].second) < corners[i + 1].first) cout << "OVERFLOW" << endl;
    //    //    right = (-1 * corners[i].second) * corners[i + 1].first;
    //    //    right_sign = -1;
    //    //}
    //    //else if (corners[i + 1].second < 0) {
    //    //    if (corners[i].second != 0 && ULLONG_MAX / corners[i].second < (-1 * corners[i + 1].first)) cout << "OVERFLOW" << endl;
    //    //    right = corners[i].second * (-1 * corners[i + 1].first);
    //    //    right_sign = -1;
    //    //}
    //    //else {
    //    //    if (corners[i].second != 0 && ULLONG_MAX / corners[i].second < corners[i + 1].first) cout << "OVERFLOW" << endl;
    //    //    right = corners[i].second * corners[i + 1].first;
    //    //    right_sign = 1;
    //    //}
    //    ////On calcule la nouvelle somme en ADDITIONANT le membre de gauche
    //    //if (total_sign > 0) {
    //    //    if (total < left && left_sign < 0) {
    //    //        total_sign = -1;
    //    //        total = left - total;
    //    //    }
    //    //    else if (total >= left && left_sign < 0) {
    //    //        total = total - left;
    //    //        total_sign = 1;
    //    //    }
    //    //    else {
    //    //        total += left;
    //    //        total_sign = 1;
    //    //    }
    //    //}
    //    //else 
    //    //{
    //    //    if (total < left && left_sign > 0) {
    //    //        total_sign = 1;
    //    //        total = left - total;
    //    //    }
    //    //    else if (total >= left && left_sign > 0) {
    //    //        total = total - left;
    //    //        total_sign = -1;
    //    //    }
    //    //    else {
    //    //        total += left;
    //    //        total_sign = -1;
    //    //    }
    //    //}
    //    ////On calcule la nouvelle somme en SOUSTRAYANT le membre de droite
    //    ////On inverse donc le signe du membre de droite
    //    //right_sign *= -1;
    //    //if (total_sign > 0) {
    //    //    if (total < right && right_sign < 0) {
    //    //        total_sign = -1;
    //    //        total = right - total;
    //    //    }
    //    //    else if (total >= right && right_sign < 0) {
    //    //        total = total - right;
    //    //        total_sign = 1;
    //    //    }
    //    //    else {
    //    //        total += right;
    //    //        total_sign = 1;
    //    //    }
    //    //}
    //    //else
    //    //{
    //    //    if (total < right && right_sign > 0) {
    //    //        total_sign = 1;
    //    //        total = right - total;
    //    //    }
    //    //    else if (total >= right && right_sign > 0) {
    //    //        total = total - right;
    //    //        total_sign = -1;
    //    //    }
    //    //    else {
    //    //        total += right;
    //    //        total_sign = -1;
    //    //    }
    //    //}
    //    //
    //    ////Penser aussi à traiter le cas où le total pourrait être négatif => garder son signe !
    //    //total += (corners[i].first * corners[i + 1].second - corners[i].second * corners[i + 1].first);
    //    
    //    //On a des problèmes d'overflow de long long lors des multiplications
    //    //On va s'appuyer sur le fait qu'on travaille sur des segments entièrement horizontaux ou verticaux (par construction)
    //    //Ainsi, on aura soit deux points avec les mêmes coordonnées y ou les mêmes coordonnées x
    //    //On peut donc regrouper un des termes du calcul
    //    //On aura alors, dans notre calcul de xy' - x'y
    //    //=> si x = x', on a x(y'-y)
    //    //=> si y = y', on a y(x - x')
    //    
    //    long long left = (long long)corners[i].first * (long long)corners[i+1].second;
    //    long long right = (long long)corners[i + 1].first * (long long)corners[i].second;
    //    total += (left - right);

    //    //Si les coordonnées x sont égales
    //    //if (corners[i].first == corners[i + 1].first) 
    //    //{
    //    //    long long left = corners[i].first;
    //    //    long long right = corners[i + 1].second - corners[i].second;
    //    //    if (left != 0 && abs(LLONG_MAX / left) < abs(right)) 
    //    //        cout << "OVERFLOW" << endl;
    //    //    long long result = left * right;
    //    //    total += result;
    //    //    cout << total << endl;
    //    //}
    //    //    
    //    ////Si les coordonnées y sont égales
    //    //else {
    //    //    long long left = corners[i].second;
    //    //    long long right = corners[i].first - corners[i + 1].first;
    //    //    if (left != 0 && abs(LLONG_MAX / left) < abs(right))
    //    //        cout << "OVERFLOW" << endl;
    //    //    long long result = left * right;
    //    //    total += result;
    //    //    cout << total << endl;
    //    //}
    //}
    //On n'oublie pas de diviser le total par 2

    //METHODE 2
    //Par la méthode des lacets, on a la surface couverte par le polygone
    //Et la formule de Pick nous donne la relation entre l'aire et le nombre de points intérieurs et à la frontière d'un polygone 
    //Si celui ci est placé sur une grille de points
    //https://www.maths-et-tiques.fr/index.php/detentes/theoreme-de-pick#:~:text=Si%20on%20construit%20un%20polygone,%2B%20b%2F2%20%2D%201.
    //A = i + b/2 - 1 avec A l'aire, b le nombre de points de la frontière, i le nombre de points intérieurs
    //Donc i = A - b/2 + 1
    //Et pour avoir le nombre total de points on ajoute le périmètre (car ici l'aire correspond également au nombre total de points)
    // Donc total = i + b = A + b/2 + 1
    //On calcule donc l'aire par la méthode des lacets
    long long lace = 0;
    for (int i = 0; i < points.size() - 1; i++)
    {
        long long left = (long long)points[i].second.first * (long long)points[i + 1].second.second;
        long long right = (long long)points[i].second.second * (long long)points[i + 1].second.first;
        lace += (left - right);
    }
    lace /= 2;
    total = perimeter / 2 + lace + 1;

    cout << total << endl;
}

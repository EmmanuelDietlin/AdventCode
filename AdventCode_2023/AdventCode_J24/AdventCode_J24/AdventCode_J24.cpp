// AdventCode_J24.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <boost/multiprecision/integer.hpp>

using namespace std;
using namespace boost;
using namespace multiprecision;

struct Point3D
{
    int128_t x, y, z;
    Point3D() {
        x = 0;
        y = 0;
        z = 0;
    }
    Point3D(int128_t x, int128_t y, int128_t z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Point3D(const Point3D& point) 
    {
        x = point.x;
        y = point.y;
        z = point.z;
    }
};

Point3D operator-(const Point3D& first, const Point3D& second)
{
    Point3D result;
    result.x = first.x - second.x;
    result.y = first.y - second.y;
    result.z = first.z - second.z;
    return result;
}

Point3D operator+(const Point3D& first, const Point3D& second) 
{
    Point3D result;
    result.x = first.x + second.x;
    result.y = first.y + second.y;
    result.z = first.z + second.z;
    return result;
}

Point3D operator*(int128_t factor, const Point3D& point)
{
    Point3D result;
    result.x = factor * point.x;
    result.y = factor * point.y;
    result.z = factor * point.z;
    return result;
}

Point3D operator*(const Point3D& point, int128_t factor)
{
    Point3D result;
    result.x = factor * point.x;
    result.y = factor * point.y;
    result.z = factor * point.z;
    return result;
}

Point3D operator/(const Point3D& point, int128_t denominator) 
{
    Point3D result;
    result.x = point.x / denominator;
    result.y = point.y / denominator;
    result.z = point.z / denominator;
    return result;
}

struct hailstone 
{
    Point3D coords;
    Point3D velocity;

    hailstone(vector<int128_t>& vals)
    {
        coords.x = vals[0];
        coords.y = vals[1];
        coords.z = vals[2];
        velocity.x = vals[3];
        velocity.y = vals[4];
        velocity.z = vals[5];
    }

    hailstone() {
        coords.x = 0; coords.y = 0; coords.z = 0;
        velocity.x = 0; velocity.y = 0; velocity.z = 0;
    }

    constexpr void operator-=(const hailstone& stone) 
    {
        coords.x -= stone.coords.x;
        coords.y -= stone.coords.y;
        coords.z -= stone.coords.z;
        velocity.x -= stone.velocity.x;
        velocity.y -= stone.velocity.y;
        velocity.z -= stone.velocity.z;
    }

    constexpr void operator+=(const hailstone& stone)
    {
        coords.x += stone.coords.x;
        coords.y += stone.coords.y;
        coords.z += stone.coords.z;
        velocity.x += stone.velocity.x;
        velocity.y += stone.velocity.y;
        velocity.z += stone.velocity.z;
    }

};

hailstone operator+(const hailstone& first, const hailstone& second)
{
    hailstone res(first);
    res += second;
    return res;
}




Point3D CrossProduct(const Point3D& first, const Point3D& second) 
{
    Point3D cross;
    cross.x = first.y * second.z - first.z * second.y;
    cross.y = first.z * second.x - first.x * second.z;
    cross.z = first.x * second.y - first.y * second.x;
    return cross;
}

int128_t DotProduct(const Point3D& first, const Point3D& second)
{
    int128_t dot = 0;
    dot += (first.x * second.x);
    dot += (first.y * second.y);
    dot += (first.z * second.z);
    return dot;
}

int main()
{
    cout.precision(16);
    ifstream input;
    input.open("input.txt");
    string line;
    vector<hailstone> hailstones;
    while (getline(input, line)) 
    {
        int separator = find(line.begin(), line.end(), '@') - line.begin();
        string left = line.substr(0, separator), right = line.substr(separator + 1);
        stringstream ss(left);
        string value;
        vector<int128_t> vals;
        while (getline(ss, value, ',')) 
        {
            vals.push_back(stoll(value));
        }
        ss = stringstream(right);
        while (getline(ss, value, ','))
        {
            vals.push_back(stoll(value));
        }
        hailstones.emplace_back(vals);
    }
    input.close();
    int count = 0;
    //Taille de la zone considérée pour les collisions

    //Example
    //int xMin = 7, xMax = 27, yMin = 7, yMax = 27;
    long long xMin = 200000000000000, xMax = 400000000000000, yMin = 200000000000000, yMax = 400000000000000;
    

    //Pour déterminer si les segments s'intersectent, on va 
    //Détermine l'équation des droites qui portent ces segments
    //Puis déterminer les coordonnées du point d'intersection
    //On vérifiera ensuite si ce point est bien présent dans la limite ET que ce point appartient bien aux deux segments (car on a considéré les droites !)
    //Il faudra donc vérifier que les coordonnées de ce point rentrent dans la limite de la zone ET que ces coordonnées sont comprises dans l'intervalle de coordonnées 
    //De chacun des segments
    //for (int i = 0; i < hailstones.size(); i++) 
    //{
    //    for (int j = i + 1; j < hailstones.size(); j++) 
    //    {
    //        //Paramètres des deux droites
    //        long double a1, a2, b1, b2;
    //        a1 = (long double)hailstones[i].velocity.y / (long double)hailstones[i].velocity.x;
    //        a2 = (long double)hailstones[j].velocity.y / (long double)hailstones[j].velocity.x;
    //        b1 = (long double)hailstones[i].coords.y - a1 * (long double)hailstones[i].coords.x;
    //        b2 = (long double)hailstones[j].coords.y - a2 * (long double)hailstones[j].coords.x;

    //        if (a1 == a2 && b1 == b2) {
    //            //si on a les mêmes paramètres de droites, alors ces deux droites sont parallèles
    //            //On n'aura pas d'intersection possible
    //            //On passe donc au segment suivant
    //            continue;
    //        }
    //        float inter_x, inter_y;
    //        //Résolution d'un système à une inconnue
    //        inter_x = (b2-b1) / (a1-a2);
    //        //On trouve y en utilisant l'équation d'une des deux droites
    //        inter_y = a1 * inter_x + b1;

    //        //On vérifie ensuite si on appartien bien aux deux segments
    //        if ((hailstones[i].velocity.x < 0 && inter_x > hailstones[i].coords.x) || (hailstones[i].velocity.x > 0 && inter_x < hailstones[i].coords.x) ||
    //            (hailstones[i].velocity.y < 0 && inter_y > hailstones[i].coords.y) || (hailstones[i].velocity.y < 0 && inter_y > hailstones[i].coords.y) ||
    //            (hailstones[j].velocity.x < 0 && inter_x > hailstones[j].coords.x) || (hailstones[j].velocity.x > 0 && inter_x < hailstones[j].coords.x) ||
    //            (hailstones[j].velocity.y < 0 && inter_y > hailstones[j].coords.y) || (hailstones[j].velocity.y < 0 && inter_y > hailstones[j].coords.y)) {
    //            continue;
    //        }
    //        if (inter_x >= xMin && inter_x <= xMax && inter_y >= yMin && inter_y <= yMax) {
    //            //Si l'intersection est bien dans les limites, on peut la compter
    //            count++;
    //        }

    //        
    //    }
    //}

    //PART TWO
    // 
    // TODO 
    // Prendre en compte le fait que les multiplications peuvent dépasser la taille d'un long long
    // => utiliser des double ou alors utiliser des ints_128
    // 
    // + comprendre pourquoi besoin uniquement de 3 grêlons pour résoudre le problème
    // 
    // 
    // TODO
    // 
    //Je vais m'appuyer sur le travail présenter ici :
    //https://aidiakapi.com/blog/2024-01-20-advent-of-code-2023-day-24/
    //On n'aura besoin que de trois grêlons pour déterminer la position et le vecteur déplacement du caillou
    //On prend comme référence arbitraire le premier grêlon
    hailstone reference = hailstones[0];
    //On prend ensuite de manière arbitraire 2 autres grêlons
    hailstone first = hailstones[1], second = hailstones[2], rock;
    //On retranche à ces deux grêlons ainsi qu'au caillou la position et la vélocité de la référence
    //On se place donc dans le repère du premier grêlon, qui est alors à l'origine de ce nouveau repère
    first -= reference;
    second -= reference;
    rock -= reference;
    //Le caillou doit intersecter les trois grêlons, et donc intersecter leur trajectoire
    //LA trajectoire du premier grêlon ainsi que l'origine du repère nous donne alors un plan, dans lequel le caillou doit se trouver
    //Pour pouvoir intersecter le grêlon à l'origine ainsi que le premier grêlon
    //L'intersection du deuxième grêlon avec le plan est alors aussi le moment où ce grêlon est frappé par le caillou
    //CAr c'est le seul moment où le deuxième grêlon entre dans le plan (et donc comme le rocher est forcément dans le plan c'est le seul 
    //moment où les deux peuvent se percuter)
    //La position à un instant t d'un objet, comme sa vitesse est constante, est de la forme a*t + b, avec a le vecteur vitesse et b la position à t = 0

    //Equation de plan : ensemble des points p tels que (p - p0) . n = 0, avec p0 point appartenant au plan et n vecteur normal du plan
    //Equation de droite : ensemble de points p tels que p = l0 + l * t, avec l le vecteur unité directeur de la droite, l0 un point appartenant à la droite et t un réel (ici le temps)
    //En remplaçant p dans la première équation, on a : 
    //d = (p0 - l0) . n / (l . n)
    //Note : . représente le produit scalaire
    //Si l.n = 0, la droite et le plan sont parallèles => si (p0 - l0).n, alors la droite est contenue dans le plan; sinon il n'y a pas d'intersection
    //Si l.n != 0, il y a un unique point d'intersection
    //On calcule alors d, puis on peut alors calculer p via la seconde équation

    //Pour calculer le vecteur normal du plan, on peut calculer le produit vectoriel avec deux points appartenant à la droite de référence
    //Cela est équivalent à faire le produit vectoriel de deux vecteurs partant de l'origine du repère, car ici les coordonnées des points sont équivalentes
    //Aux paramètres des vecteurs partant de l'origine
    //On prendra comme points du plan le point de départ de la demi droite et point de départ + vecteur déplacement
    Point3D plane_first(first.coords), plane_second(first.coords), line_point(second.coords.x * -1, second.coords.y * -1, second.coords.z * -1), line_vector(second.velocity);
    plane_second.x += first.velocity.x;
    plane_second.y += first.velocity.y;
    plane_second.z += first.velocity.z;
    //Note : on ne normalise pas le vecteur normal, car il se trouve à la fois au numérateur et au dénominateur de la fraction, ce qui fait
    //Que la normalisation n'est pas nécessaire
    Point3D normal = CrossProduct(plane_first, plane_second);
    //En prenant comme point du plan l'origine, on a alors p0 - l0 = -l0
    //Instant auquel le caillou percute le grêlon B (important : on a pris pour le plan le grêlon A, donc on a la collision du grêlon B)
    int128_t tmp1 = DotProduct(line_point, normal), tmp2 = DotProduct(line_vector, normal);
    cout << "tmp1 " << tmp1 << ", tmp2 " << tmp2 << endl;
    int128_t timeB = DotProduct(line_point, normal) / DotProduct(line_vector, normal);
    cout << timeB << endl;
    //De même, en créant un plan avec cette fois le deuxième grêlon et l'origine, on opère le même raisonnement et on trouve le moment 
    //Où le premier grêlon et le caillou se percutent
    plane_first = Point3D(second.coords);
    plane_second = Point3D(second.coords);
    line_point = Point3D(first.coords.x * -1, first.coords.y * -1, first.coords.z * -1);
    line_vector = Point3D(first.velocity);
    plane_second.x += second.velocity.x;
    plane_second.y += second.velocity.y;
    plane_second.z += second.velocity.z;
    normal = CrossProduct(plane_first, plane_second);
    //Instant auquel le caillou percute le grêlon A (important : on a pris pour le plan le grêlon B, donc on a la collision du grêlon A)
    tmp1 = DotProduct(line_point, normal), tmp2 = DotProduct(line_vector, normal);
    cout << "tmp1 " << tmp1 << ", tmp2 " << tmp2 << endl;
    int128_t timeA = DotProduct(line_point, normal) / DotProduct(line_vector, normal);
    cout << timeA << endl;

    //On peut enfin déterminer la position du caillou à ces deux instants, déterminer le delta de temps ainsi que de position
    //Pour alors en déduire la vitesse
    //En enfin calculer la position du caillou au départ
    //Note : bien penser à travailler avec les coordonnées dans le repère de base ! (et non pas dans le repère de la référence arbitraire)
    Point3D rockPosA = hailstones[1].coords + timeA * hailstones[1].velocity;
    Point3D rockPosB = hailstones[2].coords + timeB * hailstones[2].velocity;
    int128_t deltaTime = timeB - timeA;
    cout << deltaTime << endl;
    Point3D deltaPos = rockPosB - rockPosA;
    Point3D velocity = deltaPos / deltaTime;
    cout << velocity.x << " " << velocity.y << " " << velocity.z << endl;
    Point3D rockPosition = rockPosA - velocity * timeA;
    int128_t res = (rockPosition.x + rockPosition.y + rockPosition.z);

    cout << "Part one : " << count << endl;
    cout << "Part two : " << res << endl;
}


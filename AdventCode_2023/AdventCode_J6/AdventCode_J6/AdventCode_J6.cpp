// AdventCode_J6.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
using namespace std;

int main()
{
    //PART ONE
    int result = 1;
    vector<int> times;
    vector<int> distances;
    ifstream input;
    input.open("input.txt");
    string line, num;
    getline(input, line);
    stringstream ss(line);
    while (ss >> num) {
        cout << num << endl;
        if (num.compare("Time:") == 0) continue;
        times.push_back(stoi(num));
    }
    getline(input, line);
    stringstream sss(line);
    while (sss >> num) {
        cout << num << endl;
        if (num.compare("Distance:") == 0) continue;
        distances.push_back(stoi(num));
    }
    input.close();
    //Pour trouver les valeurs, on effectue une étude de fonction
    //La fonction est -X² + X*Ttotal - Drecord  (obtenue par les formules Ttot = Tmaintenu + Tparcours, Dparcours = Vparcours * Tparcours,
    //Vparcours = Tmaintenu
    //Et on cherche les valeurs de X telles que la fonction est > 0 (Dparcours > Drecord donc Dparcours - Drecord > 0)
    //Le delta calculé est donc de Ttot² - 4*Drecord
    //Par étude du signe de la fonction, on observe qu'elle est croissante puis décroissante, donc que les valeurs que l'on
    //Cherche telles que f > 0 sont celles incluses entre les deux valeurs Xa et Xb obtenues par le delta
    for (int i = 0; i < times.size(); i++) 
    {
        //Borne de droite => arrondir à l'entier inférieur
        int Xa = (int)floor(((double) - 1 * times[i] - sqrt(times[i] * times[i] - 4 * distances[i])) / (-1 * 2));
        //On gère le cas où la valeur de la racine est directement entière
        if (Xa * times[i] - Xa * Xa - distances[i] < 0.0001) Xa--;
        cout << Xa << endl;
        //Borne de gauche => arrondir à l'entier supérieur
        int Xb = (int)ceil(((double) - 1 * times[i] + sqrt(times[i] * times[i] - 4 * distances[i])) / (-1 * 2));
        //On gère le cas où la valeur de la racine est directement entière
        if (Xb * times[i] - Xb * Xb - distances[i] < 0.0001) Xb++;
        cout << Xb << endl;
        result *= ((Xa - Xb) + 1);
    }
    cout << result << endl;;

    //PART TWO
    result = 1;
    input.open("input.txt");
    string time, dist;
    getline(input, line);
    ss = stringstream(line);
    while (ss >> num) {
        if (num.compare("Time:") == 0) continue;
        time += num;
    }
    cout << time << endl;;
    getline(input, line);
    ss = stringstream(line);
    while (ss >> num) {
        if (num.compare("Distance:") == 0) continue;
        dist += num;
    }
    cout << dist << endl;
    long long t = stoll(time), d = stoll(dist);
    long long Xa = (long long)floor(((double)-1 * t - sqrt(t * t - 4 * d)) / (-1 * 2));
    //On gère le cas où la valeur de la racine est directement entière
    if (Xa * t - Xa * Xa - d < 0.0001) Xa--;
    //Borne de gauche => arrondir à l'entier supérieur
    long long Xb = (long long)ceil(((double)-1 * t + sqrt(t * t - 4 * d)) / (-1 * 2));
    //On gère le cas où la valeur de la racine est directement entière
    if (Xb * t - Xb * Xb - d < 0.0001) Xb++;
    cout << ((Xa - Xb) + 1);

}


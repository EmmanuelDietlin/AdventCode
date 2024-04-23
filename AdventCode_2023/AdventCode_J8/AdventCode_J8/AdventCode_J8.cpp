// AdventCode_J8.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <string>
#include <sstream>

using namespace std;

long long PGCD(long long a, long long b) 
{
    if (b > 0) return PGCD(b, a % b);
    return a;
}

long long PPCM(long long a, long long b) 
{
    return (a * b) / PGCD(a, b);
}

int main()
{

    ifstream input;
    input.open("input.txt");
    map<string, pair<string, string>> map;
    string directions;
    getline(input, directions);
    string line;
    getline(input, line);
    while (getline(input, line)) 
    {
        stringstream ss(line);
        string key, leftNode, rightNode; 
        getline(ss, key, '=');
        getline(ss, leftNode, ',');
        getline(ss, rightNode, ',');
        key.erase(remove_if(key.begin(), key.end(), [](const char c) {return c == ' ' || c == '(' || c == ')'; }), key.end());
        leftNode.erase(remove_if(leftNode.begin(), leftNode.end(), [](const char c) {return c == ' ' || c == '(' || c == ')'; }), leftNode.end());
        rightNode.erase(remove_if(rightNode.begin(), rightNode.end(), [](const char c) {return c == ' ' || c == '(' || c == ')'; }), rightNode.end());
        map[key] = pair<string, string>(leftNode, rightNode);
    }
    input.close();
    string current = "AAA";
    int steps = 0, dir_index = 0;
    while (current.compare("ZZZ") != 0) 
    {
        //cout << current << endl;
        if (directions[dir_index] == 'L') {
            current = map[current].first;
        }
        else {
            current = map[current].second;
        }
        steps++;
        dir_index = (dir_index + 1) % directions.size();
    }
    cout << steps << endl;

    //PART TWO
    //Au lieu de faire par force brute,
    //on va chercher le nombre d'étapes nécessaires pour atteindre un objectif pour chaque départ, puis chercher
    //le PPCM (plus petit commun multiple)
    //vector<string> currents;
    vector<string> currents;
    vector<int> stepsCount;
    for (auto& it : map) {
        if (it.first[it.first.length() - 1] == 'A') {
            currents.push_back(it.first);
            stepsCount.push_back(0);
        }
    }
    for (int i = 0; i < currents.size(); i++) 
    {
        current = currents[i];
        steps = 0;
        dir_index = 0;
        while (current[current.length() - 1] != 'Z') 
        {
            if (directions[dir_index] == 'L') {
                current = map[current].first;
            }
            else {
                current = map[current].second;
            }
            steps++;
            dir_index = (dir_index + 1) % directions.size();
        }
        stepsCount[i] = steps;
        cout << steps << endl;
    }
    //RECHERCHE DU PPCM
    //TODO
    long long ppcm = stepsCount[0];
    for (int i = 1; i < stepsCount.size(); i++) {
        ppcm = PPCM(ppcm, stepsCount[i]);
    }

    cout << ppcm << endl;
}





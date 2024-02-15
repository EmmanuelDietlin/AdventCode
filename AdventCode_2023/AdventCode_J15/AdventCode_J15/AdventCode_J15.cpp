// AdventCode_J15.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <string>
#include <algorithm>
#include <cmath>
#include <vector>;

using namespace std;

int main()
{
    ifstream input;
    input.open("input.txt");
    string line;
    int value = 0, total = 0;
    getline(input, line);
    stringstream ss(line);
    string sequence;
    //Utilisation d'un vecteur de pair au lieu d'une map car l'ordre des élément est important
    vector<vector<pair<string, int>>> boxes(256);
    while (getline(ss, sequence, ','))
    {
        value = 0;
        char operation = 'A';
        int focal = 0;
        string key = "";
        for (char& it : sequence) {
            if (it == '=' || it == '-') operation = it;
            else if (it >= '0' && it <= '9') focal = (it - '0');
            else
            {
                key += it;
                value += (int)(it);
                value *= 17;
                value %= 256;
            }
            
        }
        //total += value;
        if (operation == '-') {
            if (find_if(boxes[value].begin(), boxes[value].end(), [&](pair<string, int>& value) {return value.first.compare(key) == 0; }) != boxes[value].end()) {
                boxes[value].erase(find_if(boxes[value].begin(), boxes[value].end(), [&](pair<string, int>& value) {return value.first.compare(key) == 0; }));
                //cout << "remove " << key << " from " << value << endl;
            }
        }
        else {
            if (find_if(boxes[value].begin(), boxes[value].end(), [&](pair<string, int>& value) {return value.first.compare(key) == 0; }) != boxes[value].end()) {
                int index = find_if(boxes[value].begin(), boxes[value].end(), [&](pair<string, int>& value) {return value.first.compare(key) == 0; }) - boxes[value].begin();
                boxes[value][index].second = focal;
                //cout << "change focal of lens " << key << " to " << focal << endl;

            }
            else {
                //cout << "add " << key << " to " << value << " with " << focal << endl;
                boxes[value].push_back({ key, focal });
            }
        }
    }
    input.close();


    for (int i = 0; i < boxes.size(); i++) 
    {
        //cout << i << endl;
        for (int j = 0; j < boxes[i].size(); j++) {
            total += (i + 1) * (j + 1) * boxes[i][j].second;
            //cout << boxes[i][j].first << " " << boxes[i][j].second << endl;
        }
    }

    cout << total << endl;
}


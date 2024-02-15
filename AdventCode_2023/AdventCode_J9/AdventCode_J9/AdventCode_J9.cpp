// AdventCode_J9.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <sstream>

using namespace std;

int main()
{
    ifstream input;
    input.open("input.txt");
    string line;
    int result = 0;
    while (getline(input, line)) 
    {
        vector<vector<long long>> sequences;
        stringstream ss(line);
        string value;
        vector<long long> initial;
        while (getline(ss, value, ' ')) {
            initial.push_back(stoll(value));
        }
        sequences.push_back(initial);
        bool allZeroes = false;
        int index = 0;
        while (allZeroes == false) {
            vector<long long> v;
            allZeroes = true;
            for (int i = 1; i < sequences[index].size(); i++) {
                long long value = sequences[index][i] - sequences[index][i - 1];
                //cout << value << " ";
                if (value > 0 || value < 0) allZeroes = false;
                v.push_back(value);
            }
            //cout << endl;
            sequences.push_back(v);
            index++;
        }
        //PART ONE
        /*sequences[index].push_back(0);
        for (int i = index - 2; i >= 0; i--) {
            sequences[i].push_back(sequences[i][sequences[i].size() - 1] + sequences[i + 1][sequences[i + 1].size() - 1]);
        }
        cout << sequences[0][sequences[0].size() - 1] << endl;
        result += sequences[0][sequences[0].size() - 1];
        */
        //PART TWO
        //Il aurait aussi été possible d'appliquer la même stratégie qua'à la partie 1 en utilisant des listes 
        //a la place des vecteurs et en ajoutant les éléments au début
        int tmp = 0;
        for (int i = index - 1; i >= 0; i--) 
        {
            //cout << sequences[i][0] << " " << tmp << endl;
            tmp = sequences[i][0] - tmp;
        }
        result += tmp;

    }
    input.close();
    cout << result << endl;


}


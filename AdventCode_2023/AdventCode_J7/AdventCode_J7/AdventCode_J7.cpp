// AdventCode_J7.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <cmath>
#include <map>

using namespace std;

static map<char, int> cardPoints
{
    {'A', 13},
    {'K', 12},
    {'Q', 11},
    {'J', 10},
    {'T', 9},
    {'9', 8},
    {'8', 7},
    {'7', 6},
    {'6', 5},
    {'5', 4},
    {'4', 3},
    {'3', 2},
    {'2', 1}
};

static map<char, int> cardPointsP2{
    {'A', 13},
    {'K', 12},
    {'Q', 11},
    {'J', 1},
    {'T', 10},
    {'9', 9},
    {'8', 8},
    {'7', 7},
    {'6', 6},
    {'5', 5},
    {'4', 4},
    {'3', 3},
    {'2', 2}
};

map<string, int> points;

int CompareHands(string leftHand, string rightHand, bool isJoker)
{
    //Le joker est une carte pour créer le meilleur TYPE DE MAIN
    map<char, int> left, right;
    int leftPoints = 0, rightPoints = 0;
    for (char c : leftHand) {
        left[c] += 1;
    }
    for (char c : rightHand) {
        right[c] += 1;
    }
    if (isJoker == true) 
    {
        //On calcule la meilleur main théorique, en comptant les J comme les cartes permettant de scorer le
        //meilleur type de main
        if (left.find('J') != left.end()) 
        {
            char max = 'J';
            if (left['J'] < 5) {
                for (auto& it : left) {
                    if (max == 'J' || (it.second > left[max] && it.first != 'J')) max = it.first;
                }
            }
            if (max != 'J') {
                left[max] += left['J'];
                left['J'] = 0;
            }
        }
        if (right.find('J') != right.end()) 
        {
            char max = 'J';
            if (right['J'] < 5) {
                for (auto& it : right) {
                    if (max == 'J' || (it.second > right[max] && it.first != 'J')) max = it.first;
                }
            }
            if (max != 'J') {
                right[max] += right['J'];
                right['J'] = 0;
            }
        }
    }
    
    for (auto& it : left)
    {
        leftPoints += it.second * it.second;
    }
    for (auto& it : right)
    {
        rightPoints += it.second * it.second;
    }
    
    points[leftHand] = leftPoints;
    points[rightHand] = rightPoints;
    //cout << leftHand << " " << leftPoints << " " << rightHand << " " << rightPoints << endl;
    if (leftPoints < rightPoints) return 1;
    else if (leftPoints > rightPoints) return -1;
    else
    {
        for (int i = 0; i < 5; i++)
        {
            if (isJoker == true) {
                //cout << cardPointsP2[leftHand[i]] << " " << cardPointsP2[rightHand[i]] << endl;
                if (cardPointsP2[leftHand[i]] < cardPointsP2[rightHand[i]])
                    return 1;
                else if (cardPointsP2[leftHand[i]] > cardPointsP2[rightHand[i]])
                    return -1;
            }
            else {
                if (cardPoints[leftHand[i]] < cardPoints[rightHand[i]])
                    return 1;
                else if (cardPoints[leftHand[i]] > cardPoints[rightHand[i]])
                    return -1;
            }
        }
    }
    return 1;
}

list<pair<string, int>> Merge(list<pair<string, int>> left, list<pair<string, int>> right)
{
    if (left.size() == 0) {
        return right;
    }
    if (right.size() == 0) {
        return left;
    }
    int res = CompareHands(left.front().first, right.front().first, false);
    if (res > 0) {
        list<pair<string, int>> newLeft;
        newLeft.push_back(left.front());
        left.pop_front();
        list<pair<string, int>> newRight = Merge(left, right);
        for (auto& it : newRight) {
            newLeft.push_back(it);
        }
        return newLeft;
    }
    else
    {
        list<pair<string, int>> newRight;
        newRight.push_back(right.front());
        right.pop_front();
        list<pair<string, int>> newLeft = Merge(left, right);
        for (auto& it : newLeft) {
            newRight.push_back(it);
        }
        return newRight;
    }
}

list<pair<string, int>> MergeSort(list<pair<string, int>> input) 
{
    if (input.size() <= 1) return input;
    int half = input.size() / 2;
    int index = 0;
    list<pair<string, int>> left, right;
    for (auto& it : input) {
        if (index < half) left.push_back(it);
        else right.push_back(it);
        index++;
    }
    return Merge(MergeSort(left), MergeSort(right));
}

void CombSort(vector<pair<string, int>>& input, bool IsJoker) 
{
    int interval = input.size();
    bool sw = false;
    while (interval > 1 || sw) 
    {
        interval = (int)floor(interval / 1.3);
        if (interval < 1) interval = 1;
        sw = false;
        for (int i = 0; i < input.size() - interval; i++) 
        {
            int compare = CompareHands(input[i].first, input[i + interval].first, IsJoker);
            if (compare < 0) {
                pair<string, int> tmp = input[i + interval];
                input[i + interval] = input[i];
                input[i] = tmp;
                //NE PAS OUBLIER
                sw = true;
            }
        }
    }
}

//Situation de tri => utilisation d'un algorithme de tri
int main()
{
    ifstream file;
    file.open("input.txt");
    string line;
    //Pour le tri par fusion
    //list<pair<string, int>> hands;
    //Pour le tri à peigne
    vector<pair<string, int>> hands_c, hands_p2;
    while (getline(file, line))
    {
        stringstream ss(line);
        string hand, bid;
        getline(ss, hand, ' ');
        getline(ss, bid, ' ');
        //hands.push_back({ hand, stoi(bid) });
        hands_c.push_back({ hand, stoi(bid) });

    }
    file.close();

    hands_p2 = vector<pair<string, int>>(hands_c);

    //UTILISER UN ALGO DE TRI NON RECURSIF => stack overflow du fait du nombre trop important de récursions
    //Tri récursif => tri par fusion ou tri rapide
    //list<pair<string, int>> result = MergeSort(hands);

    //Tri à peigne (amélioration du tri à bulles)
    CombSort(hands_c, false);

    //ofstream f;
    //f.open("output.txt");
    //long long res = 0;
    //for (int i = 0; i < hands_c.size(); i++) {
    //    //f << hands_c[i].first << endl;
    //    res += hands_c[i].second * (i+1);
    //}
    ////f.close();
    ///*for (auto& it : hands_c) {
    //    cout << it.first << endl;
    //    res += it.second * rank;
    //    rank++;
    //}*/
    //cout << "winnings : " << res << endl;

    //PART TWO
    ofstream f;
    f.open("output.txt");
    CombSort(hands_p2, true);
    long long res_p2 = 0;
    for (int i = 0; i < hands_p2.size(); i++) {
        f << hands_p2[i].first << endl;
        //cout << hands_p2[i].first << endl;
        res_p2 += hands_p2[i].second * (i + 1);
    }
    f.close();

    cout << "winnings p2 : " << res_p2 << endl;
}



using System;
using System.IO;
using System.Collections;
using System.Linq;



string path = @"..\..\..\..\input.txt";
List<string> rounds = new List<string>();
Dictionary<char, int> movePoints = new Dictionary<char, int>()
{
    {'A', 1},
    {'B', 2},
    {'C', 3},
};
string moves = "ABC";

using (StreamReader sr = File.OpenText(path))
{
    string s;

    while((s = sr.ReadLine()) != null)
    {
        rounds.Add(s);   
    }
}

int score = 0;
foreach (string move in rounds)
{
    char[] mv = move.Split(' ').Select(_ => char.Parse(_)  > 'C' ? (char)(char.Parse(_) - 23) : char.Parse(_)).ToArray();
    score += movePoints[mv[1]];
    if (mv[0] == mv[1])
    {
        score += 3;
    }
    else if (mv[1] - mv[0] == 1 || mv[0] - mv[1] == 2)
    {
        score += 6;
    }
}

Console.WriteLine("Part one : score = {0}", score);

score = 0;
foreach (string move in rounds)
{
    char[] mv = move.Split(' ').Select(_ => char.Parse(_)).ToArray();
    char m = 'A';
    switch (mv[1])
    {
        //Draw
        case 'Y':
            //Même symbole
            score += movePoints[mv[0]];
            score += 3;
            break;
        //Win
        case 'Z':
            //Les symboles de droite battent les symboles de gauche, on prend donc un symbole à droite de celui de l'adversaire (en bouclant aux extrémités)
            m = mv[0] + 1 > 'C' ? (char)(mv[0] - 2) : (char)(mv[0] + 1);
            score += movePoints[m];
            score += 6;
            break;
        //Lose
        default:
            //Les symboles de droite battent les symboles de gauche, on prend donc un symbole à gauche de celui de l'adversaire (en bouclant aux extrémités)
            m = mv[0] - 1 < 'A' ? (char)(mv[0] + 2) : (char)(mv[0] - 1);
            score += movePoints[m];
            break;
    }
}

Console.WriteLine("Part two : score = {0}", score);
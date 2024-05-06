using System;
using System.IO;
using System.Linq;
using System.Collections;
using System.Collections.Generic;
using static System.Net.Mime.MediaTypeNames;
using System.Net.Http.Headers;
using System.Runtime;
using System.Diagnostics.CodeAnalysis;

string path = @"..\..\..\..\test.txt";

List<string> map = new List<string>();
Dictionary<Position, int> distances = new Dictionary<Position, int>();
Position Start = new(0, 0, 'a'), End = new(0, 0, 'a');
using (StreamReader sr = File.OpenText(path))
{
    string s;
    int index = 0;
    int pos = -1;
    Position elem = new(0, 0, 'a');
    while((s = sr.ReadLine()) != null)
    {
        for (int i = 0; i < s.Length; i++)
        {
            elem.X = i;
            elem.Y = index;
            elem.V = s[i];
            if (s[i] == 'S')
            {
                elem.V = 'a';
                Start = elem;
                s = s.Replace('S', 'a');
            }
            if (s[i] == 'E')
            {
                elem.V = 'z';
                End = elem;
                s = s.Replace('E', 'z');
            }
            distances.Add(elem, 1000000);
        }
        map.Add(s);
        index++;
    }
}

#region Bellmann-Ford
//On va implémenter l'algo de Bellmann-Ford en prenant comme point de départ l'arrivée
//Cela permet pour la partie 2 d'avoir une seule itération de l'algorithme au lieu de devoir répéter pour tous les 'a' de départ, 
//Puisque l'algo donne la distance entre un sommet de départ et tous les autres
//L'algo est également moins gourmand que Moore-Dijkstra car on n'a pas besoin de vérifier si un sommet à déjà été vérifié ou non, on les vérifie tous à chaque fois
int k = 0;
bool change = true;
distances[End] = 0;
Dictionary<Position, int> prevDistances = new Dictionary<Position, int>(distances);
do
{
    change = false;
    k++;
    foreach ((var node, var dist) in distances)
    {
        Position neighbor;
        if (node.X > 0)
        {
            neighbor = new(node.X - 1, node.Y, 'a');
            neighbor.V = map[neighbor.Y][neighbor.X];
            //La marche suivante doit être AU MAXIMUM 1 plus haute que la marche actuelle
            if (neighbor.V <= node.V + 1 && prevDistances[neighbor] + 1 < distances[node])
            {
                distances[node] = prevDistances[neighbor] + 1;
                change = true;
            }
        }

        if (node.X < map[0].Length - 1)
        {
            neighbor = new(node.X + 1, node.Y, 'a');
            neighbor.V = map[neighbor.Y][neighbor.X];
            if (neighbor.V <= node.V + 1 && prevDistances[neighbor] + 1 < distances[node])
            {
                distances[node] = prevDistances[neighbor] + 1;
                change = true;
            }
        }

        if (node.Y > 0)
        {
            neighbor = new(node.X, node.Y - 1, 'a');
            neighbor.V = map[neighbor.Y][neighbor.X];
            if (neighbor.V <= node.V + 1 && prevDistances[neighbor] + 1 < distances[node])
            {
                distances[node] = prevDistances[neighbor] + 1;
                change = true;
            }
        }

        if (node.Y < map.Count - 1)
        {
            neighbor = new(node.X, node.Y + 1, 'a');
            neighbor.V = map[neighbor.Y][neighbor.X];
            if (neighbor.V <= node.V + 1 && prevDistances[neighbor] + 1 < distances[node])
            {
                distances[node] = prevDistances[neighbor] + 1;
                change = true;
            }
        }
    }
    prevDistances = new Dictionary<Position, int>(distances);
}
while (k < distances.Count && change == true);
Console.WriteLine("Part one : distance = {0}", distances[Start]);
int d = distances.Where(_ => _.Key.V == 'a').OrderBy(_ => _.Value).Select(_ => _.Value).First();
Console.WriteLine("Part two : distance = {0}", d);
#endregion


#region Dijkstra
//List<Position> Inside = new List<Position>();
//Position pivot = Start, next = new(0, 0, 'a');
//Inside.Add(Start);
//distances[pivot] = 0;
//while (true)
//{
//    //On regarde toute autour du pivot les chemins possibles
//    if (pivot.X > 0)
//    {
//        next = new(pivot.X - 1, pivot.Y, 'a');
//        next.V = map[next.Y][next.X];
//        //La marche suivante doit être AU MAXIMUM 1 plus haute que la marche actuelle
//        if (Inside.Contains(next) == false && next.V <= pivot.V + 1)
//        {
//            if (distances[pivot] + 1 < distances[next])
//            {
//                distances[next] = distances[pivot] + 1;
//            }
//        }
//    }

//    if (pivot.X < map[0].Length - 1)
//    {
//        next = new(pivot.X + 1, pivot.Y, 'a');
//        next.V = map[next.Y][next.X];
//        if (Inside.Contains(next) == false && next.V <= pivot.V + 1)
//        {
//            if (distances[pivot] + 1 < distances[next])
//            {
//                distances[next] = distances[pivot] + 1;
//            }
//        }
//    }

//    if (pivot.Y > 0)
//    {
//        next = new(pivot.X, pivot.Y - 1, 'a');
//        next.V = map[next.Y][next.X];
//        if (Inside.Contains(next) == false && next.V <= pivot.V + 1)
//        {
//            if (distances[pivot] + 1 < distances[next])
//            {
//                distances[next] = distances[pivot] + 1;
//            }
//        }
//    }

//    if (pivot.Y < map.Count - 1)
//    {
//        next = new(pivot.X, pivot.Y + 1, 'a');
//        next.V = map[next.Y][next.X];
//        if (Inside.Contains(next) == false && next.V <= pivot.V + 1)
//        {
//            if (distances[pivot] + 1 < distances[next])
//            {
//                distances[next] = distances[pivot] + 1;
//            }
//        }
//    }

//    next = distances.OrderBy(_ => _.Value).Select(_ => _.Key).Except(Inside).First();
//    Inside.Add(next);
//    if (next == End)
//    {
//        break;
//    }
//    pivot = next;
//}
//Console.WriteLine("Part one : distance = {0}", distances[End]);
#endregion




struct Position(int x, int y, char v)
{
    public int X = x;
    public int Y = y;
    public char V = v;

    public static bool operator==(Position left, Position right)
    {
        return (left.X == right.X && left.Y == right.Y && left.V == right.V);
    }

    public static bool operator !=(Position left, Position right)
    {
        return (left.X != right.X || left.Y != right.Y || left.V != right.V);
    }

    public override bool Equals(object obj)
    {
        if (obj == null || obj is not Position) return false;
        return this == (Position)obj;
    }

    public override int GetHashCode()
    {
        return X ^ Y ^ V;
    }
}



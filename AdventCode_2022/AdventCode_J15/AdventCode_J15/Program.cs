using System;
using System.Text;
using System.IO;
using System.Linq;
using System.Collections;
using System.Collections.Generic;
using System.Text.RegularExpressions;

string path = @"..\..\..\..\input.txt";
List<Position> sensors = new List<Position>();
List<Position> beacons = new List<Position>();
using (StreamReader sr = File.OpenText(path))
{
    string s;
    string pattern = @"-*[0-9]+";
    while ((s = sr.ReadLine()) != null)
    {
        int[] coords = Regex.Matches(s, pattern).Select(_ => int.Parse(_.Value)).ToArray();
        Position sensor = new(coords[0], coords[1]);
        Position beacon = new(coords[2], coords[3]);
        sensors.Add(sensor);
        beacons.Add(beacon);
    }
}

SortedSet<int> notPossible = new SortedSet<int>();
int lineIndex = 2000000;
//int lineIndex = 10;
for (int i = 0; i < beacons.Count; i++)
{
    int mhd = Math.Abs(beacons[i].Y - sensors[i].Y) + Math.Abs(beacons[i].X - sensors[i].X);
    //Si la ligne que l'on étudie se trouve dans la zone où l'on ne peut pas avoir de beacon pour le sensor
    if ((sensors[i].Y + mhd >= lineIndex && sensors[i].Y <= lineIndex) || (sensors[i].Y - mhd <= lineIndex && sensors[i].Y >= lineIndex))
    {
        //On va calculer toutes les positions sur lesquelles on ne peut pas avoir de beacon pour le sensor
        int x = sensors[i].X;
        int delta = mhd - Math.Abs(sensors[i].Y - lineIndex);
        for (int j = 0; j <= delta; j++)
        {
            notPossible.Add(x + j);
            notPossible.Add(x - j);
        } 
    }
    //On enlève la position du beacon si on en trouve un sur la ligne
    if (beacons[i].Y == lineIndex)
        notPossible.Remove(beacons[i].X);
}
Console.WriteLine($"Part one : count = {notPossible.Count}");

//Partie 2 
//Pour chaque ligne entre 0 et 4000000, garder une liste d'intervalles où trouver un beacon est impossible (même principe que partie 1)
//Si des intervalles se chevauchent, fusionner ces intervalles
//La ligne contenant le beacon cherché sera la seule ligne où on aura deux intervalles distincts

//Autre solution possible :
//Pour avoir un résultat unique, il faut que le beacon que l'on cherche soit une case à côté de la zone couverte par un sensor. Dans le cas contraire, on pourrait le rapprocher d'une case de ce sensor et ce ne serais toujours pas le beacon le plus proche (donc autre possibilité d'emplacement de beacon)
//Pour chacun des sensor, checker toutes les cases juste en dehors de la périphérie pour trouver celle couverte par aucun sensor
//On peut alors essayer de calculer les intersections des droites définissant la périphérie
//En effet, le point que l'on cherche doit appartenir à la périphérie d'au moins deux losanges. Si il appartenait à la périphérie d'un seul losange, alors tous les points adjacents doivent appartenir au losange, sans quoi on n'aurait pas de solution unique.
//Cela implique donc que notre emplacement se trouve au milieu de quatre pixels appartenant tous au losange. Cela est impossible car alors au moins un des pixels se trouve à une distance d+1 du capteur. L'emplacement recherché est donc sur l'intersection des limites de losanges
//Comme leur forme est un losange et que la zone est composée de pixels, leur pente est 1 ou -1, ce qui rend les équations de droite très simples
//On va garder deux listes, les droites de pente 1 et celles de pente -1, dont on calculera ensuite les intersections entre ces deux listes (les droites de même pente sont parallèles et ne pourront donc pas se croiser
//On gardera enfin uniquement le point d'intersection compris dans la limite de X et Y >= 0 et <= 4000000 et qui ne se trouve pas dans les zones couvertes par les sensors

//On va stocker les équations dans une struct Position avec x la pente et y l'ordonnée à l'origine
List<Position> croissant = new List<Position>();
List<Position> decroissant = new List<Position>();
//int maxSize = 20;
int maxSize = 4000000;
for (int i = 0; i < beacons.Count; i++)
{
    int mhd = Math.Abs(beacons[i].Y - sensors[i].Y) + Math.Abs(beacons[i].X - sensors[i].X) + 1;
    //On va utiliser le point le plus à gauche et le point le plus à droite pour calculer les équations
    Position left = new(sensors[i].X - mhd, sensors[i].Y);
    Position right = new(sensors[i].X + mhd, sensors[i].Y);
    //Comme les droites sont de pente 1 et -1, deltaY = deltaX donc si on a un couple de coordonnées (X, Y), alors on a (0, Y-X) (pour droite de pente 1) et (0, Y + X) (pour droite de pente -1)
    Position d = new(1, left.Y - left.X);
    croissant.Add(d);
    d = new(-1, left.Y + left.X);
    decroissant.Add(d);
    d = new(1, right.Y - right.X);
    croissant.Add(d) ;
    d = new(-1, right.Y + right.X);
    decroissant.Add(d);
}
//Ensuite, pour calculer le point d'intersection, on doit résoudre l'équation y1 = y2 donc ax + b1 = ax+b2. Comme on a des droites de pente 1 et -1 respectivement, on a alors x + b1 = -x + b2 donc x = (b2 - b1)/2 
HashSet<Position> possibilites = new HashSet<Position>();
//string pp = @"data.txt";
//using (StreamWriter sw = File.CreateText(pp))
//{
    foreach (var d in croissant)
    {
        foreach (var dc in decroissant)
        {
            Position intersect = new(0, 0);
            intersect.X = (dc.Y - d.Y) / 2;
            intersect.Y = intersect.X + d.Y;
            if (intersect.X >= 0 && intersect.X <= maxSize && intersect.Y >= 0 && intersect.Y <= maxSize)
            {
                possibilites.Add(intersect);
                //sw.WriteLine($"{intersect.X}:{intersect.Y}");
            }
        }
    }
//}


foreach (var p in possibilites)
{
    bool found = true;
    for (int i = 0; i < sensors.Count; i++)
    {
        int mhd = Math.Abs(beacons[i].Y - sensors[i].Y) + Math.Abs(beacons[i].X - sensors[i].X);
        int dist = Math.Abs(p.Y - sensors[i].Y) + Math.Abs(p.X - sensors[i].X);
        if (dist <= mhd)
        {
            found = false;
            break;
        }
    }
    if (found)
    {
        Console.WriteLine($"Part two : beacon = {p.X}:{p.Y}, result = {(long)p.X * 4000000 + (long)p.Y}");
        return;
    }
}
Console.WriteLine("Not found");


struct Position(int x, int y)
{
    public int X = x;
    public int Y = y;

    public static bool operator==(Position left, Position right)
    {
        return left.X == right.X && left.Y == right.Y;  
    }

    public static bool operator!=(Position left, Position right)
    {
        return left.X != right.X || left.Y != right.Y; 
    }
}
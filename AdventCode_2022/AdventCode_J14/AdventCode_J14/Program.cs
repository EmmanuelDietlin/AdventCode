using System;
using System.Text;
using System.IO;
using System.Linq;
using System.Collections;
using System.Collections.Generic;
using System.Reflection.Metadata;

string path = @"..\..\..\..\input.txt";
List<StringBuilder> map = new List<StringBuilder>();
List<Path> paths = new List<Path>();
int xmin = int.MaxValue, xmax = int.MinValue, ymax = 0;
using (StreamReader sr = File.OpenText(path))
{
    string s;
    while((s = sr.ReadLine()) != null)
    {
        string[] c = s.Split(" -> ");
        int[] coord = c[0].Split(',').Select(int.Parse).ToArray();
        Position pos = new(coord[0], coord[1]);
        Path p = new(pos);
        if (p.pos.X < xmin) xmin = p.pos.X;
        if (p.pos.X > xmax) xmax = p.pos.X;
        if (p.pos.Y > ymax) ymax = p.pos.Y;
        for (int i = 1; i < c.Length; i++)
        {
            coord = c[i].Split(',').Select(int.Parse).ToArray();
            Position d = new(coord[0] - pos.X, coord[1] - pos.Y);
            if (coord[0] < xmin) xmin = coord[0];
            if (coord[0] > xmax) xmax = coord[0];
            if (coord[1] > ymax) ymax = coord[1];
            p.directions.Add(d);
            pos += d;
        }
        paths.Add(p);
    }
}
//On donne de la marge sur les côtés pour ne pas a voir à vérifier les limites gauche et droites lors du calcul de chute du sable
xmin--;
xmax++;
string grid = new string('.', xmax - xmin + 1);
for (int i = 0; i <= ymax; i++)
{
    map.Add(new(grid));
}

foreach(var p in paths)
{
    Position pos = p.pos;
    int i = 0;
    int factor = 1;
    map[pos.Y][pos.X - xmin] = '#';
    foreach (var dir in p.directions)
    {
        if (dir.X > 0 || dir.Y > 0) factor = 1;
        else factor = -1;
        
        if (dir.X != 0)
        {
            for (i = 1; i <= Math.Abs(dir.X); i++)
            {
                pos.X += factor;
                map[pos.Y][pos.X - xmin] = '#';
            }
        }
        else
        {
            for (i = 1; i <= Math.Abs(dir.Y); i++)
            {
                pos.Y += factor;
                map[pos.Y][pos.X - xmin] = '#';
            }
        }
    }
}
foreach (var line in map)
{
    Console.WriteLine(line);
}
Position sandBegin = new(500 - xmin, 0);
Position sand = new(sandBegin);
int count = 0;
while(true)
{
    //Le sable tombe dans le vide, on s'arrête
    if (sand.Y + 1 > ymax)
    {
        break;
    }
    if (map[sand.Y + 1][sand.X] == '.')
    {
        sand.Y++;
    }
    else if (map[sand.Y + 1][sand.X - 1] == '.')
    {
        sand.X--;
        sand.Y++;
    }
    else if (map[sand.Y + 1][sand.X + 1] == '.')
    {
        sand.X++;
        sand.Y++;
    }
    else
    {
        map[sand.Y][sand.X] = 'O';
        sand = new(sandBegin);
        count++;
    }
}
foreach (var line in map)
{
    Console.WriteLine(line);
}
Console.WriteLine($"Part one : count = {count}");
xmin -= 500;
xmax += 500;
grid = new string('.', xmax - xmin + 1);
map.Clear();
for (int i = 0; i <= ymax+1; i++)
{
    map.Add(new(grid));
}
map.Add(new(new string('#', xmax - xmin + 1)));
foreach (var p in paths)
{
    Position pos = p.pos;
    int i = 0;
    int factor = 1;
    map[pos.Y][pos.X - xmin] = '#';
    foreach (var dir in p.directions)
    {
        if (dir.X > 0 || dir.Y > 0) factor = 1;
        else factor = -1;

        if (dir.X != 0)
        {
            for (i = 1; i <= Math.Abs(dir.X); i++)
            {
                pos.X += factor;
                map[pos.Y][pos.X - xmin] = '#';
            }
        }
        else
        {
            for (i = 1; i <= Math.Abs(dir.Y); i++)
            {
                pos.Y += factor;
                map[pos.Y][pos.X - xmin] = '#';
            }
        }
    }
}
sandBegin = new(500 - xmin, 0);
sand = new(sandBegin);
count = 0;
while (true)
{
    //Le sable tombe dans le vide, on s'arrête
    if (map[sand.Y + 1][sand.X] == '.')
    {
        sand.Y++;
    }
    else if (map[sand.Y + 1][sand.X - 1] == '.')
    {
        sand.X--;
        sand.Y++;
    }
    else if (map[sand.Y + 1][sand.X + 1] == '.')
    {
        sand.X++;
        sand.Y++;
    }
    else
    {
        map[sand.Y][sand.X] = 'O';
        count++;
        if (sand == sandBegin) break;
        sand = new(sandBegin);
        
    }
}
foreach (var line in map)
{
    Console.WriteLine(line);
}
Console.WriteLine($"Part two : count = {count}");




class Path(Position p)
{
    public Position pos = new(p);
    public List<Position> directions = new List<Position>();
}

struct Position
{
    public int X;
    public int Y;

    public Position(Position p)
    {
        this.X = p.X;
        this.Y = p.Y;
    }

    public Position(int X, int Y)
    {
        this.X = X; this.Y = Y;
    }

    public static Position operator+(Position first, Position second)
    {
        Position newPos = new Position(first);
        newPos.X += second.X;
        newPos.Y += second.Y;
        return newPos;
    }

    public static bool operator==(Position first, Position second)
    {
        return first.X == second.X && first.Y == second.Y;
    }

    public static bool operator !=(Position first, Position second)
    {
        return first.X != second.X || first.Y != second.Y;
    }
}
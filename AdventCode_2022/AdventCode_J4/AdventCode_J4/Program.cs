using System;
using System.IO;
using System.Linq;
using System.Collections;

string path = @"..\..\..\..\input.txt";
List<Tuple<int, int>> tasks = new List<Tuple<int, int>>();
using (StreamReader sr = File.OpenText(path))
{
    string s;
    while((s = sr.ReadLine()) != null)
    {
        string[] t = s.Split(',');
        int[] pair = t[0].Split('-').Select(int.Parse).ToArray();
        tasks.Add(new Tuple<int, int>(pair[0], pair[1]));
        pair = t[1].Split('-').Select(int.Parse).ToArray();
        tasks.Add(new Tuple<int, int>(pair[0], pair[1]));
    }
}

int count = 0;
for (int i = 0; i < tasks.Count - 1; i+=2)
{
    if ((tasks[i].Item1 >= tasks[i+1].Item1 && tasks[i].Item2 <= tasks[i + 1].Item2) ||
        (tasks[i+1].Item1 >= tasks[i].Item1 && tasks[i+1].Item2 <= tasks[i].Item2))
    {
        count++;
    }
}
Console.WriteLine("Part one : count = {0}", count);

int overlapCount = 0;
for (int i = 0; i < tasks.Count - 1; i+=2)
{
    if ((tasks[i].Item1 >= tasks[i + 1].Item1 && tasks[i].Item1 <= tasks[i + 1].Item2) ||
        (tasks[i].Item2 >= tasks[i + 1].Item1 && tasks[i].Item2 <= tasks[i + 1].Item2) ||
        (tasks[i+1].Item1 >= tasks[i].Item1 && tasks[i+1].Item1 <= tasks[i].Item2) ||
        (tasks[i+1].Item2 >= tasks[i].Item1 && tasks[i+1].Item2 <= tasks[i].Item2))
    {
        overlapCount++;
    }
}

Console.WriteLine("Part two : overlap count = {0}", overlapCount);

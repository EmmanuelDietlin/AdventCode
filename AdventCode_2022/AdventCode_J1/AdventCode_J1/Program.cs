using System;
using System.IO;
using System.Collections;

string path = @"..\..\..\..\input.txt";

List<int> calories = new List<int>();

Console.WriteLine(File.Exists(path));
using (StreamReader sr = File.OpenText(path))
{
    string s;
    int cal = 0;
    while((s = sr.ReadLine()) != null)
    {
        if (s.Length == 0)
        {
            calories.Add(cal);
            cal = 0;
        }
        else
        {
            cal += int.Parse(s);
        }
    }
}
calories.Sort();
Console.WriteLine("Part one : {0} calories ", calories[calories.Count - 1]);
int c = 0;
for (int i = 0; i < 3; i++)
{
    c += calories[calories.Count - 1 - i];
}
Console.WriteLine("Part two : {0} calories ", c);



using System;
using System.IO;
using System.Linq;
using System.Collections;


List<string> lefts = new List<string>(), rights = new List<string>();
List<string> sacks = new List<string>();
string path = @"..\..\..\..\input.txt";
using (StreamReader sr = File.OpenText(path))
{
    string s;
    while((s = sr.ReadLine()) != null)
    {
        //On est sûr que les deux compartiments de chaque sac contiennent le même nombre d'objets
        lefts.Add(s.Substring(0, s.Length / 2));
        rights.Add(s.Substring(s.Length / 2));
        sacks.Add(s);
    }
}

int priority = 0;
for (int i = 0; i < lefts.Count; i++)
{
    //On est sûr qu'il y a uniquement un objet en commun
    char c = (lefts[i].Intersect(rights[i])).ToArray()[0];
    priority += char.IsLower(c) ? c - 'a' + 1 : c - 'A' + 27;
}

Console.WriteLine("Part one : total priority = {0}", priority);

int badgePriority = 0;
for (int i = 0; i < sacks.Count - 2; i+=3)
{
    char c = sacks[i].Intersect(sacks[i + 1]).Intersect(sacks[i + 2]).ToArray()[0];
    badgePriority += char.IsLower(c) ? c - 'a' + 1 : c - 'A' + 27;
}

Console.WriteLine("Part two : total badge priority = {0}", badgePriority);


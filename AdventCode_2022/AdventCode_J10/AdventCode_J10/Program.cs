using System;
using System.IO;
using System.Linq;
using System.Collections;

string path = @"..\..\..\..\input.txt";
List<string> instructions = new List<string>();
using (StreamReader sr = File.OpenText(path))
{
    string s;
    while((s = sr.ReadLine()) != null)
    {
        instructions.Add(s);
    }
}

int X = 1;
int cycles = 1;
int nextCycleCount = 20;
int tot = 0;
foreach (var instruction in instructions)
{
    string[] instr = instruction.Split(' ');

    if (cycles == nextCycleCount)
    {
        tot += nextCycleCount * X;
        nextCycleCount += 40;
    }
    cycles++;
    if (instr[0] == "addx")
    {
        if (cycles == nextCycleCount)
        {
            tot += nextCycleCount * X;
            nextCycleCount += 40;
        }
        cycles++;
        X += int.Parse(instr[1]);
    }
}
Console.WriteLine("Part one : strength = {0}", tot);

//Part 2
X = 1;
cycles = 1;
nextCycleCount = 40;
int crtPos = 0;
string img = string.Empty;
foreach (var instruction in instructions)
{
    string[] instr = instruction.Split(' ');
    if (crtPos >= X-1 && crtPos <= X+1)
    {
        img += '#';
    }
    else
    {
        img += '.';
    }

    if (cycles == nextCycleCount)
    {
        Console.WriteLine(img);
        img = string.Empty;
        //On le met à -1 car on va l'incrémenter juste après
        crtPos = -1;
        nextCycleCount += 40;
    }
    cycles++;
    crtPos++;
    if (instr[0] == "addx")
    {
        if (crtPos >= X - 1 && crtPos <= X + 1)
        {
            img += '#';
        }
        else
        {
            img += '.';
        }
        if (cycles == nextCycleCount)
        {
            Console.WriteLine(img);
            img = string.Empty;
            //On le met à -1 car on va l'incrémenter juste après
            crtPos = -1;
            nextCycleCount += 40;
        }
        cycles++;
        crtPos++;
        X += int.Parse(instr[1]);
    }
}

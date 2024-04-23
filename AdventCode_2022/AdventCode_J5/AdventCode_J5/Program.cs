using System;
using System.IO;
using System.Linq;
using System.Collections;


List<Stack<char>> stacks = new List<Stack<char>>();
List<Moves> moves = new List<Moves>();  
string path = @"..\..\..\..\input.txt";
using (StreamReader sr = File.OpenText(path))
{
    State st = State.Crates;
    string s;
    while ((s = sr.ReadLine()) != null)
    {
        int index = 1;
        if (s.Length == 0) continue;
        if (st == State.Crates)
        {
            for (int i = 0; i < s.Length - 2; i += 4)
            {
                if (stacks.Count < index)
                {
                    stacks.Add(new Stack<char>());
                }
                if (char.IsAsciiLetter(s[i + 1]))
                {
                    stacks[index - 1].Push(s[i + 1]);
                }
                else if (char.IsDigit(s[i + 1]))
                {
                    st = State.Moves;
                    break;
                }
                index++;
            }
        }
        else
        {
            string[] mv = s.Split(' ');
            Moves m = new(int.Parse(mv[1]), int.Parse(mv[3]), int.Parse(mv[5]));
            moves.Add(m);
        }
        
    }
}
//for (int i = 0; i < stacks.Count; i++)
//{
//    //On inverse le sens des piles car lors du parcours du fichier, on a eu les caisses de haut en bas, ce qui fait que les caisses les plus en bas se sont retrouvées en haut des piles (puisque ajoutées en dernier, les piles étant des files LIFO)
//    stacks[i] = new Stack<char>(stacks[i]);
//}


List<Stack<char>> stacks_cpy = new List<Stack<char>>();
//l'ordre des caisses dans les piles est inversé car on à ajouté les caisses de haut en bas (donc les caisses les plus en bas on été placées au-dessus de la pile
//Par constructeur copie, l'ordre de la pile copiée sera alors correct (puisque le constructeur va parcourir la pile de haut en bas)
foreach (Stack<char> s in stacks)
{
    stacks_cpy.Add(new Stack<char>(s));
}

foreach (Moves mv in moves)
{
    for (int i = 0; i < mv.Number; i++)
    {
        char crate;
        if (stacks_cpy[mv.From - 1].TryPop(out crate))
        {
            stacks_cpy[mv.To - 1].Push(crate);  
        }
    }
}
string res = string.Empty;
foreach (var stack in stacks_cpy)
{
    char top;
    if (stack.TryPeek(out top))
    {
        res += top;
    }
}
Console.WriteLine("Part one : result = {0}", res);

//Part two
stacks_cpy = new List<Stack<char>>();
foreach (Stack<char> s in stacks)
{
    stacks_cpy.Add(new Stack<char>(s));
}
foreach (Moves mv in moves)
{
    Stack<char> tempStack = new Stack<char>();
    for (int i = 0; i < mv.Number; i++)
    {
        char crate;
        if (stacks_cpy[mv.From - 1].TryPop(out crate))
        {
            tempStack.Push(crate);
        }
    }
    foreach (var crate in tempStack)
    {
        stacks_cpy[mv.To - 1].Push(crate);
    }
}
res = string.Empty;
foreach (var stack in stacks_cpy)
{
    char top;
    if (stack.TryPeek(out top))
    {
        res += top;
    }
}
Console.WriteLine("Part two : result = {0}", res);





enum State
{
    Crates,
    Moves
};

public struct Moves
{
    public int Number;
    public int From;
    public int To;

    public Moves(int Number, int From, int To)
    {
        this.Number = Number;
        this.From = From;
        this.To = To;
    }
}
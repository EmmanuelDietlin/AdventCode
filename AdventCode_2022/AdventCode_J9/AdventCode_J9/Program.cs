using System;
using System.IO;
using System.Linq;
using System.Collections;

string path = @"..\..\..\..\input.txt";
List<Tuple<Move, int>> moves = new List<Tuple<Move, int>>();  
HashSet<Position> visited = new HashSet<Position>();
using (StreamReader sr = File.OpenText(path))
{
    string s;
    while ((s = sr.ReadLine()) != null)
    {
        string[] move = s.Split(' ');
        int d = int.Parse(move[1]);
        Move m = (Move)move[0][0];
        Tuple<Move, int> mv = new(m, d);
        moves.Add(mv);
    }
}

Position[] rope = { new(0,0), new(0,0)};
visited.Add(rope[1]);
foreach (var move in moves)
{
    //En se déplaçant case par case, on s'assure qu'il y aura toujours au maximum un espace libre d'écart entre la tête et le bout
    for (int i = 0; i < move.Item2; i++)
    {
        switch (move.Item1)
        {
            case Move.Left:
                rope[0].X--;
                break;
            case Move.Right:
                rope[0].X++;
                break;
            case Move.Up:
                rope[0].Y--;
                break;
            case Move.Down:
                rope[0].Y++;
                break;
        }
        //Si le bout de la corde est trop éloigné de la tête
        if (Math.Abs(rope[1].X - rope[0].X) > 1 || Math.Abs(rope[1].Y - rope[0].Y) > 1)
        {
            //Dans le cas où l'on doit se déplacer en diagonale, les deux inégalités seront vérifiées, on aura bien un déplacement en diagonale

            //On se déplace selon l'axe x
            if ((rope[1].X - rope[0].X) != 0)
            {
                rope[1].X += (rope[0].X - rope[1].X) / Math.Abs(rope[0].X - rope[1].X);
            }
            //On se déplace selon l'axe Y
            if ((rope[1].Y - rope[0].Y) != 0)
            {
                rope[1].Y += (rope[0].Y - rope[1].Y) / Math.Abs(rope[0].Y - rope[1].Y);
            }
            visited.Add(rope[1]);
        }
    } 
}
int count = visited.Count;
Console.WriteLine("Part one : count = {0}", count);

Position[] bigRope = new Position[10];
visited.Clear();
visited.Add(bigRope[9]);
foreach (var move in moves)
{
    //En se déplaçant case par case, on s'assure qu'il y aura toujours au maximum un espace libre d'écart entre la tête et le bout
    for (int i = 0; i < move.Item2; i++)
    {
        switch (move.Item1)
        {
            case Move.Left:
                bigRope[0].X--;
                break;
            case Move.Right:
                bigRope[0].X++;
                break;
            case Move.Up:
                bigRope[0].Y--;
                break;
            case Move.Down:
                bigRope[0].Y++;
                break;
        }
        for (int j = 1; j < 10; j++)
        {
            //On compare maintenant chaque noeuf à son prédécésseur
            if (Math.Abs(bigRope[j].X - bigRope[j-1].X) > 1 || Math.Abs(bigRope[j].Y - bigRope[j-1].Y) > 1)
            {
                //Dans le cas où l'on doit se déplacer en diagonale, les deux inégalités seront vérifiées, on aura bien un déplacement en diagonale

                //On se déplace selon l'axe x
                if ((bigRope[j].X - bigRope[j-1].X) != 0)
                {
                    bigRope[j].X += (bigRope[j-1].X - bigRope[j].X) / Math.Abs(bigRope[j-1].X - bigRope[j].X);
                }
                //On se déplace selon l'axe Y
                if ((bigRope[j].Y - bigRope[j-1].Y) != 0)
                {
                    bigRope[j].Y += (bigRope[j-1].Y - bigRope[j].Y) / Math.Abs(bigRope[j-1].Y - bigRope[j].Y);
                }
                if (j == 9)
                    visited.Add(bigRope[9]);
            }
        }
        
    }
}
int bigRopeCount = visited.Count;
Console.WriteLine("Part two : count = {0}", bigRopeCount);



struct Position
{
    public int X;
    public int Y;

    public Position(int x, int y)
    {
        X = x;
        Y = y;
    }

    public Position()
    {
        X = 0;
        Y = 0;
    }
}

enum Move
{
    Left = 'L',
    Right = 'R',
    Up = 'U',
    Down = 'D'
};

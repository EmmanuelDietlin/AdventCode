using System;
using System.IO;
using System.Linq;
using System.Collections;
using System.Text;
using System.Text.RegularExpressions;

string path = @"..\..\..\..\input.txt";
string[] regexPatterns =
{
    @"\b[0-9]+",
    @"[*+]"
};

List<Monkey> monkeys_orig = new List<Monkey>();

using (StreamReader sr = File.OpenText(path))
{
    string s;
    while((s = sr.ReadLine()) != null)
    {
        if (s.Length == 0) continue;

        Monkey monkey = new Monkey(int.Parse(Regex.Match(s, regexPatterns[0]).Value));
        s = sr.ReadLine();
        monkey.items = Regex.Matches(s, regexPatterns[0]).Select(_ => long.Parse(_.Value)).ToList();
        s = sr.ReadLine();
        char op = Regex.Match(s, regexPatterns[1]).Value[0];
        if (Regex.IsMatch(s, regexPatterns[0]))
        {
            int factor = int.Parse(Regex.Match(s, regexPatterns[0]).Value);
            if (op == '+')
            {
                monkey.Operation += (_ => _ + factor);
            }
            else
            {
                monkey.Operation += (_ => _ * factor);
            }
        }
        else
        {
            if (op == '+')
            {
                monkey.Operation += (_ => _ + _);
            }
            else
            {
                monkey.Operation += (_ => _ * _);
            }
        }
        s = sr.ReadLine();
        int div = int.Parse(Regex.Match(s, regexPatterns[0]).Value);
        monkey.Divider = div;
        s = sr.ReadLine();
        int tr = int.Parse(Regex.Match(s, regexPatterns[0]).Value);
        s = sr.ReadLine();
        int fl = int.Parse(Regex.Match(s, regexPatterns[0]).Value);
        monkey.Test += (_ => _ % div == 0 ? tr : fl);
        monkeys_orig.Add(monkey);
    }
}

List<Monkey> monkeys = new List<Monkey>();
foreach (var monkey in monkeys_orig)
{
    monkeys.Add(new(monkey));
}

for (int i = 0; i < 20; i++)
{
    for (int mk = 0; mk < monkeys.Count; mk++)
    {
        for (int j = 0; j < monkeys[mk].items.Count; j++)
        {
            monkeys[mk].items[j] = monkeys[mk].Operation(monkeys[mk].items[j]);
            monkeys[mk].items[j] /= 3;
            monkeys[mk].InspectCount++;
            int nextMk = monkeys[mk].Test(monkeys[mk].items[j]);
            monkeys[nextMk].items.Add(monkeys[mk].items[j]);
            monkeys[mk].items.RemoveAt(j);
            j--;
        }
    }
}
long[] mks = monkeys.OrderByDescending(_ => _.InspectCount).Take(2).Select(_ => _.InspectCount).ToArray();
long monkeyBusiness = mks[0] * mks[1];
Console.WriteLine("Part one : business = {0}", monkeyBusiness);

monkeys = new List<Monkey>();
int multipleDiviseurs = 1;
foreach (var monkey in monkeys_orig)
{
    monkeys.Add(new(monkey));
    multipleDiviseurs *= monkey.Divider;
}

for (int i = 0; i < 10000; i++)
{
    for (int mk = 0; mk < monkeys.Count; mk++)
    {
        for (int j = 0; j < monkeys[mk].items.Count; j++)
        {
            //Garder le résultat MODULO le produit de tous les diviseurs nous permet de continuer à vérifier la divisibilité par chacun des diviseurs sans avoir les valeurs qui explosent
            monkeys[mk].items[j] = (monkeys[mk].Operation(monkeys[mk].items[j]) % multipleDiviseurs);
            monkeys[mk].InspectCount++;
            int nextMk = monkeys[mk].Test(monkeys[mk].items[j]);
            monkeys[nextMk].items.Add(monkeys[mk].items[j]);
            monkeys[mk].items.RemoveAt(j);
            j--;
        }
    }
}
mks = monkeys.OrderByDescending(_ => _.InspectCount).Take(2).Select(_ => _.InspectCount).ToArray();
monkeyBusiness = mks[0] * mks[1];
Console.WriteLine("Part two : business = {0}", monkeyBusiness);




public delegate long Operation(long old);

public delegate int Test(long value);

class Monkey
{
    public int Number;
    public long InspectCount;
    public int Divider;
    public List<long> items = new List<long>();
    public Operation Operation { get; set; }
    public Test Test { get; set; }
    public Monkey(int number)
    {
        this.Number = number;
    }

    public Monkey(int number, long inspectCount,  List<long> items, Operation op, Test test, int divider)
    {
        this.Number = number;
        this.InspectCount = inspectCount;
        this.items = new List<long>(items);
        this.Operation = op;
        this.Test = test;
        this.Divider = divider;
    }

    public Monkey(Monkey monkey)
    {
        Number = monkey.Number;
        InspectCount = monkey.InspectCount;
        items = new List<long>(monkey.items);
        Operation = monkey.Operation;
        Test = monkey.Test;
        Divider = monkey.Divider;
    }
}

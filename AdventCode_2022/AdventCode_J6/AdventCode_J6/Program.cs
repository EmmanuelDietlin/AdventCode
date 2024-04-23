using System;
using System.IO;
using System.Linq;
using System.Collections;

string path = @"..\..\..\..\input.txt";
string input = string.Empty;
using (StreamReader sr = File.OpenText(path))
{
    string s;
    s = sr.ReadLine();
    input = s;
}

int firstIndex = 0;
for (int i = 0; i < input.Length - 3; i++)
{
    string substr = input.Substring(i, 4);
    //On pourrait aussi itérer sur les éléments de la substr en les comparant chacun aux éléments suivants (i=0 comparé à 1,2,3, i=1 comparé à 2,3, i=2 comparé à 3, et regarder si on a un doublon quelque part)
    if (substr.Length == substr.Distinct().Count())
    {
        firstIndex = i + 3;
        break;
    }
}
Console.WriteLine("Part one : index = {0}", firstIndex + 1);

int messageIndex = 0;
for (int i = 0; i < input.Length - 13; i++)
{
    string substr = input.Substring(i, 14);
    if (substr.Length == substr.Distinct().Count())
    {
        messageIndex = i + 13;
        break;
    }
}

Console.WriteLine("Part two : index = {0}", messageIndex + 1);


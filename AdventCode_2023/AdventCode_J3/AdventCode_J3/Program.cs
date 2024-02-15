// See https://aka.ms/new-console-template for more information

Dictionary<int, char> symbolsIndex = new Dictionary<int, char>();
Dictionary<int, string> numbersIndex = new Dictionary<int, string>();
int w = 1, h = 1;
int sum = 0;
int sum_p2 = 0;
using (StreamReader reader = new StreamReader("input.txt"))
{
    string? line;
    int lineIndex = 0;
    while ((line = reader.ReadLine()) != null)
    {
        w = line.Length;
        string number = "";
        int numIndex = 0;
        for (int i = 0; i < line.Length; i++)
        {
            if (Char.IsDigit(line[i]))
            {
                if (number.Length == 0)
                    numIndex = i + line.Length * lineIndex;
                number += line[i];
                
            }
            else if (line[i] != '.')
            {
                symbolsIndex.Add(i + line.Length * lineIndex, line[i]);
                if (number.Length > 0)
                {
                    numbersIndex.Add(numIndex, number);
                    number = "";
                }
            }
            else if (number.Length > 0)
            {
                numbersIndex.Add(numIndex, number);
                number = "";
            }
        }
        if (number.Length > 0) numbersIndex.Add(numIndex, number);
        lineIndex++;
    }
    h = lineIndex;
}
//PART ONE
//Note : on pourrait également résoudre en itérant sur les symboles au lieu d'itérer sur les chiffres 
//=> il serait possible d'observer un gain de performance, car on aurait une liste d'indexs adjacents plus petite
// que pour les chiffres, dû au fait que les symboles ne font d'1 charactère de large
//Il faudra toutefois prendre en compte le fait que les nombres sont de largeur variant entre 1 et 3

//foreach (var num in numbersIndex)
//{
//    List<int> possibleIndexes = new List<int>();
//    int x = num.Key % w;
//    int y = num.Key / w;
//    if (x > 0) possibleIndexes.Add(num.Key - 1);
//    if (x < w - 1) possibleIndexes.Add(num.Key + num.Value.Length);
//    if (y > 0)
//    {
//        for (int i = 0; i < num.Value.Length; i++) possibleIndexes.Add(num.Key - w + i);
//        if (x > 0) possibleIndexes.Add(num.Key - w - 1);
//        if (x < w - 1) possibleIndexes.Add(num.Key - w + num.Value.Length);
//    }
//    if (y < h - 1)
//    {
//        for (int i = 0; i < num.Value.Length; i++) possibleIndexes.Add(num.Key + w + i);
//        if (x > 0) possibleIndexes.Add(num.Key + w - 1);
//        if (x < w - 1) possibleIndexes.Add(num.Key + w + num.Value.Length);
//    }

//    if (symbolsIndex.Keys.Intersect(possibleIndexes).Any())
//    {
//        sum += int.Parse(num.Value);
//    }
//}

//PART TWO
foreach (var star in symbolsIndex.Where(_ => _.Value == '*'))
{
    List<int> possibleIndexes = new List<int>();
    int x = star.Key % w;
    int y = star.Key / w;
    if (x > 0) possibleIndexes.Add(star.Key - 1);
    if (x < w - 1) possibleIndexes.Add(star.Key + 1);
    if (y > 0)
    {
        possibleIndexes.Add(star.Key - w);
        if (x > 0) possibleIndexes.Add(star.Key - w - 1);
        if (x < w - 1) possibleIndexes.Add(star.Key - w + 1);
    }
    if (y < h - 1)
    {
        possibleIndexes.Add(star.Key + w);
        if (x > 0) possibleIndexes.Add(star.Key + w - 1);
        if (x < w - 1) possibleIndexes.Add(star.Key + w + 1);
    }
    //Comme on stocke seulement l'indice de départ des différents nombres, on doit d'abord chercher si l'indice de départ se recoupe
    //avec les indices de proximité des étoiles
    //Puis on prend l'indice de fin et on répète l'opération, afin de prendre en compte les cas
    //Où le nombre est à gauche de l'étoile
    List<int> first = numbersIndex.Where(_ => possibleIndexes.Contains(_.Key)).Select(_ => _.Key).ToList();
    List<int> second = numbersIndex.Where(_ => possibleIndexes.Contains(_.Key + _.Value.Length - 1) && first.Contains(_.Key) == false).Select(_ => _.Key).ToList();
    List<int> result = first.Concat(second).ToList();
    if (result.Count == 2)
    {
        Console.WriteLine(result[0] + " : " + result[1]);
        Console.WriteLine("aaaaa");
        sum_p2 += int.Parse(numbersIndex[result[0]]) * int.Parse(numbersIndex[result[1]]);
    }
}

Console.WriteLine(sum);
Console.WriteLine(sum_p2);

// See https://aka.ms/new-console-template for more information

using System.Formats.Asn1;

char[] separators = new char[] {':', ';', ','};
int sum = 0;
Dictionary<string, int> colourCount = new Dictionary<string, int>()
{
    {"red",12 }, {"green",13}, {"blue",14}
};
using (StreamReader reader = new StreamReader("input.txt"))
{
    //PART ONE
    //string? line;
    //while ((line = reader.ReadLine()) != null)
    //{
    //    string[] infos = line.Split(separators);
    //    int index = int.Parse(infos[0].Split(' ')[1]);

    //    foreach (string handful in infos.Skip(1))
    //    {
    //        string[] cubes = handful.Split(' ').Where(_ => _.Length > 0).ToArray();
    //        int count = int.Parse(cubes[0]);
    //        Console.WriteLine("count : " + count + ", colour : " + cubes[1]);
    //        if (colourCount[cubes[1]] < count) goto Error;
    //    }
    //    sum += index;
    //    Console.WriteLine(index);
    //Error:
    //    continue;       
    //}

    //PART TWO
    string? line;
    while ((line = reader.ReadLine()) != null)
    {
        string[] infos = line.Split(separators);
        int index = int.Parse(infos[0].Split(' ')[1]);
        Dictionary<string, int> gameMinColourCount = new Dictionary<string, int>()
        {
            {"red", 0 }, {"green", 0}, {"blue", 0}
        };
        foreach (string handful in infos.Skip(1))
        {
            string[] cubes = handful.Split(' ').Where(_ => _.Length > 0).ToArray();
            int count = int.Parse(cubes[0]);
            Console.WriteLine("count : " + count + ", colour : " + cubes[1]);
            if (count > gameMinColourCount[cubes[1]]) gameMinColourCount[cubes[1]] = count;
        }
        sum += gameMinColourCount["red"] * gameMinColourCount["green"] * gameMinColourCount["blue"];
    }
}

Console.WriteLine(sum);

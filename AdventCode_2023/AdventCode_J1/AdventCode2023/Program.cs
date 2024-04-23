// See https://aka.ms/new-console-template for more information
using System.ComponentModel.Design;
using System.Linq;

int count = 0;
Dictionary<string, int> values = new Dictionary<string, int>()
{
    { "one",1}, { "two",2}, { "three",3}, { "four",4}, { "five",5}, { "six",6}, { "seven",7}, { "eight",8}, { "nine",9}
};
using (StreamReader reader = new StreamReader("input.txt"))
{
    string? line;
    while((line = reader.ReadLine()) != null)
    {
        if (line == null) break;
        int first = -1, last = -1;
        string substr = "";
        int index = 0, r_index = line.Length - 1;
        //Solution : parcourir la liste dans les deux sens, ainsi pour le dernier on parcours la liste en 
        // sens INVERSE
        Console.WriteLine(line);
        while (index < line.Length && r_index >= 0 && (first < 0 || last < 0) )
        {
            if (first < 0)
            {
                if (Char.IsDigit(line[index]))
                {
                    first = line[index] - '0';
                }
                else
                {
                    foreach (var val in values)
                    {
                        substr = line.Substring(index, Math.Min(line.Length - index, val.Key.Length));
                        if (substr == val.Key) first = val.Value;
                    }
                }
            }
            if (last < 0)
            {
                if (Char.IsDigit(line[r_index]))
                {
                    last = line[r_index] - '0';
                }
                else
                {
                    foreach (var val in values)
                    {
                        int min_index = Math.Max(r_index - val.Key.Length, 0);
                        substr = line.Substring(min_index + 1, r_index - min_index);
                        if (substr == val.Key) last = val.Value;
                    }
                }
            }
            if (first < 0) index++;
            if (last < 0) r_index--;
        }
        Console.WriteLine(first * 10 + (last == 0 ? first : last));
        count += first * 10 + (last == 0 ? first : last);
    }
}


Console.WriteLine(count);

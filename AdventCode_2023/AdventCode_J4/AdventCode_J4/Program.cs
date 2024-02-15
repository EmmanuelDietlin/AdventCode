// See https://aka.ms/new-console-template for more information

char[] separators = new char[] { ':', '|' };
int points = 0;
Dictionary<int, int> cardWinNums = new Dictionary<int, int>();
List<int> cardsCount = new List<int>();
using (StreamReader reader = new StreamReader("input.txt"))
{
    string? line;
    int index = 0;
    while ((line = reader.ReadLine()) != null)
    {
        //PART ONE
        //string[] card = line.Split(separators);
        //List<int> win = card[1].Trim().Split(' ').Where(_ => _.Length > 0).Select(_ => int.Parse(_)).ToList();
        //List<int> nums = card[2].Trim().Split(' ').Where(_ => _.Length > 0).Select(_ => int.Parse(_)).ToList();
        //Console.WriteLine((int)Math.Pow(2, win.Intersect(nums).Count()));
        //points += (int)Math.Pow(2, win.Intersect(nums).Count() - 1);

        //PART TWO
        string[] card = line.Split(separators);
        List<int> win = card[1].Trim().Split(' ').Where(_ => _.Length > 0).Select(_ => int.Parse(_)).ToList();
        List<int> nums = card[2].Trim().Split(' ').Where(_ => _.Length > 0).Select(_ => int.Parse(_)).ToList();
        int cardNumber = int.Parse(card[0].Split(' ')[^1]);
        cardWinNums.Add(cardNumber, win.Intersect(nums).Count());
        index++;
        cardsCount.Add(1);
    }
}
foreach (var card in cardWinNums)
{
    for (int j = 0; j < cardsCount[card.Key - 1]; j++)
    {
        for (int i = card.Key; i <= Math.Min(card.Key - 1 + card.Value, cardsCount.Count - 1); i++)
        {
            cardsCount[i]++;
        }
    }
    
}

points = cardsCount.Sum();
Console.WriteLine(points);

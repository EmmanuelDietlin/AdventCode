using System;
using System.IO;
using System.Linq;
using System.Collections;

string path = @"..\..\..\..\input.txt";
List<string> patch = new List<string>();
Dictionary<Tuple<int, int>, PatchElem> elems = new Dictionary<Tuple<int, int>, PatchElem>();
using (StreamReader sr = File.OpenText(path))
{
    string s;
    while ((s = sr.ReadLine()) != null)
    {
        patch.Add(s);
    }
}

//On crée un dictionnaire où, pour chaque arbre, on a la taille maximale des arbres à gauche, en haut, à droite et en bas de celui-ci.
//On ne considère que les arbres à l'intérieur du rectangle et pas ceux sur la frontière, qu'on sait déjà visible d'au moins un côté
//On remplit d'abord le dictionnaire avec les max à gauche et à droite ainsi qu'avec la taille des arbres (itération plus simple de part la manière dont on récupère les données, ie ligne par ligne)
for (int i = 1; i < patch.Count - 1; i++)
{
    //Hauteur max à gauche et à droite
    int Lmax = patch[i][0] - '0';
    int Rmax = patch[i][patch[i].Length - 1] - '0';
    for (int j = 1, k = patch[i].Length - 2; j < patch[i].Length - 1 && k > 0; j++, k--)
    {
        Tuple<int, int> elem = new(j, i);
        if (elems.ContainsKey(elem) == false)
        {
            elems[elem] = new(patch[i][j] - '0');
        }
        int size = patch[i][j] - '0';
        elems[elem].Lmax = Lmax;
        Lmax = size > Lmax ? size : Lmax;

        elem = new(k, i);
        if (elems.ContainsKey(elem) == false)
        {
            elems[elem] = new(patch[i][k] - '0');
        }
        size = patch[i][k] - '0';
        elems[elem].Rmax = Rmax;
        Rmax = size > Rmax ? size : Rmax;
    }
}

for (int j = 1; j < patch[0].Length - 1; j++)
{
    int Tmax = patch[0][j] - '0';
    int Dmax = patch[patch.Count - 1][j] - '0';
    for (int i = 1, k = patch.Count - 2; i < patch.Count - 1 && k > 0; i++, k--)
    {
        Tuple<int, int> elem = new(j, i);
        elems[elem].Tmax = Tmax;
        Tmax = elems[elem].Size > Tmax ? elems[elem].Size : Tmax;

        elem = new(j, k);
        elems[elem].Dmax = Dmax;
        Dmax = elems[elem].Size > Dmax ? elems[elem].Size : Dmax;
    }
}

int count = 0;
//Les arbres qui composent le contour sont visibles
count += (patch.Count * 2 + (patch[0].Length - 2) * 2);
foreach (var tree in elems)
{
    if (tree.Value.IsVisible()) count++;
}
Console.WriteLine("Part one : visible = {0}", count);

int maxScenic = 0;
foreach (var tree in elems)
{
    int x = tree.Key.Item2;
    int y = tree.Key.Item1;
    int[] scores = { 0, 0, 0, 0 };
    int i = x-1, j = x+1;
    for (; i > 0; i--)
    {
        Tuple<int, int> t = new(y, i);
        scores[0]++;
        //Si on rencontre un arbre de taille égale ou supérieure, on s'arrête
        if (elems[t].Size >= tree.Value.Size) break;
    }
    for (; j < patch[0].Length - 1; j++)
    {
        Tuple<int, int> t = new(y, j);
        scores[1]++;
        if (elems[t].Size >= tree.Value.Size) break;
    }
    //On a atteint la bordure du rectangle sans rencontrer d'arbre bloquant, donc on voit l'arbre de la bordure
    if (i == 0) scores[0]++;
    if (j == patch[0].Length - 1) scores[1]++;

    i = y-1;
    j = y+1;
    for (; i > 0; i--)
    {
        Tuple<int, int> t = new(i, x);
        scores[2]++;
        //Si on rencontre un arbre de taille égale ou supérieure, on s'arrête
        if (elems[t].Size >= tree.Value.Size) break;
    }
    for (; j < patch.Count - 1; j++)
    {
        Tuple<int, int> t = new(j, x);
        scores[3]++;
        if (elems[t].Size >= tree.Value.Size) break;
    }
    //On a atteint la bordure du rectangle sans rencontrer d'arbre bloquant, donc on voit l'arbre de la bordure
    if (i == 0) scores[2]++;
    if (j == patch.Count - 1) scores[3]++;


    int sc = scores[0] * scores[1] * scores[2] * scores[3];
    if (sc > maxScenic) maxScenic = sc;
}
Console.WriteLine("Part two : scenic = {0}", maxScenic);



class PatchElem
{
    public int Lmax = default;
    public int Rmax = default;
    public int Tmax = default;
    public int Dmax = default;
    public int Size = default;

    public PatchElem(int size)
    {
        this.Size = size;
    }

    public bool IsVisible()
    {
        return (Size > Lmax || Size > Rmax || Size > Tmax || Size > Dmax);
    }
}

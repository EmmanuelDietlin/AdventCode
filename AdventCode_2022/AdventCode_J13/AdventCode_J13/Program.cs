using System;
using System.IO;
using System.Linq;
using System.Collections;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Runtime.Intrinsics.Wasm;

string path = @"..\..\..\..\input.txt";
List<string> packets = new List<string>();
using (StreamReader sr = File.OpenText(path))
{
    do
    {
        string[] packet = [sr.ReadLine(), sr.ReadLine()];
        //10 est le seul entier > 9 dans le jeu de données, on le remplace par un seul caractère afin de faciliter la comparaison plus tard
        packet[0] = packet[0].Replace("10", "A");
        packet[1] = packet[1].Replace("10", "A");
        packets.Add(packet[0]);
        packets.Add(packet[1]);
    }
    while (sr.ReadLine() != null);
}

int total = 0;
int correct = 0;
for (int i = 0; i < packets.Count; i+=2)
{
    correct = ComparePackets(packets[i], packets[i+1]);
    if (correct > 0)
    {
        total += i/2 + 1;
    }

    #region FIRST_IDEAD
    //for (; j < left.Length && k < right.Length; j++, k++)
    //{
    //    if (left[j] == right[k]) continue;

    //    if (left[j] == ']')
    //    {
    //        //On regarde si il reste une valeur dans la chaine de droite, on est donc dans le bon ordre
    //        //Si oui, alors on doit aller au bout de la liste dans la chaine de droite
    //        //=> on remonte ensuite autant de profondeur que possible à gauche et à droite on skippe jusqu'à '[' et on remonte
    //        if (right[k] != ']')
    //        {
    //            correct = true;
    //            break;
    //        }
    //        while (j + 1 < left.Length && left[j + 1] == ']')
    //        {
    //            j++;
    //        }
    //        while (k + 1 < right.Length && right[k + 1] == ']')
    //        {
    //            k++;
    //        }
    //    }
    //    else if (right[k] == ']') 
    //    {

    //        //On regarde si il reste une valeur dans la chaine de gauche
    //        //Si oui, alors on est plus petit à droite qu'a gauche, on ne se trouve pas dans le bon ordre
    //        //Sinon , on remonte ensuite autant de profondeur que possible à gauche et à droite on skippe jusqu'à '[' et on remonte
    //        if (left[j] != ']')
    //        {
    //            correct = false;
    //            break;
    //        }
    //        while (j + 1 < left.Length && left[j + 1] == ']')
    //        {
    //            j++;
    //        }
    //        while (k + 1 < right.Length && right[k + 1] == ']')
    //        {
    //            k++;
    //        }
    //    }
    //    else if ((char.IsDigit(left[j]) || left[j] == 'A') && (char.IsDigit(right[k]) || right[k] == 'A'))
    //    {
    //        //On s'arrête dès qu'une des valeurs permet de savoir si on est dans le bon ordre ou non
    //        if (left[j] == right[k]) continue;
    //        else
    //        {
    //            correct = left[j] < right[k];
    //            break;
    //        }
    //    }

    //    if (left[j] == '[' && (char.IsDigit(right[k]) || right[k] == 'A'))
    //    {
    //        //On comparera l'entier avec l'entier suivant de left
    //        k--;
    //    }
    //    else if (right[k] == '[' && (char.IsDigit(left[j]) || left[j] == 'A'))
    //    {
    //        //On comparera l'entier avec l'entier suivant de right
    //        j--;
    //    }
    //}
    #endregion

}
Console.WriteLine($"Part one : total = {total}");
//Ajout des deux paquets séparateurs
string sep1 = "[[2]]", sep2 = "[[6]]";
packets.Add(sep1);
packets.Add(sep2);
//Les paquets seront triés dans l'ordre inverse, de par les valeurs renvoyées par la fonction de comparaison, on doit donc inverser
packets.Sort(ComparePackets);
packets.Reverse();
for (int i = 0; i < packets.Count; i++)
{
    Console.WriteLine(packets[i]);
}
int i1, i2;
i1 = packets.IndexOf(sep1) + 1;
i2 = packets.IndexOf(sep2) + 1;
Console.WriteLine($"Part two : result = {i1 * i2}");


//Fonction de comparaison
//Renvoie 1 si bon sens, -1 si mauvais sens, 0 si égal
static int ComparePackets(string left, string right)
{
    int j = 0, k = 0;
    int comp = 0;
    if (left == right) return 0;
    for (; j < left.Length && k < right.Length; j++, k++)
    {
        if (left[j] == right[k]) continue;
        //La liste de droite est plus longue que la liste de gauche
        if (left[j] == ']' && right[k] != ']')
        {
            comp = 1;
            break;
        }
        //La liste de gauche est plus longue que la liste de droite
        if (right[k] == ']' && left[j] != ']')
        {
            comp = -1;
            break;
        }
        //On compare deux entiers
        if ((char.IsDigit(left[j]) || left[j] == 'A') && (char.IsDigit(right[k]) || right[k] == 'A'))
        {
            //On s'arrête dès qu'une des valeurs permet de savoir si on est dans le bon ordre ou non
            if (left[j] == right[k]) continue;
            else
            {
                comp = (left[j] < right[k] ? 1 : -1);
                break;
            }
        }

        //Cas spécifique où on doit comparer un entier avec une liste => on intègre l'entier dans une liste, on ajoute donc les symboles de liste dans la chaine autour de l'entier
        //Une piste d'amélioration serait de parvenir à ne pas modifier la chaine pour ajouter les symboles pour comparer l'entier et la chaine
        if (left[j] == '[' && (char.IsDigit(right[k]) || right[k] == 'A'))
        {
            right = right.Insert(k, "[").Insert(k + 2, "]");
        }
        else if (right[k] == '[' && (char.IsDigit(left[j]) || left[j] == 'A'))
        {
            left = left.Insert(j, "[").Insert(j + 2, "]");
        }
    }
    //On a moins d'éléments à gauche qu'à droite
    if (j == left.Length) comp = 1;
    //On a moins d'élément à droite qu'a gauche
    if (k == right.Length) comp = -1;
    return comp;
}


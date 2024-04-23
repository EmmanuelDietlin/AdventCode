// AdventCode_J25.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <map>
#include <vector>
#include <string>
#include <cstdlib>
#include <list>

using namespace std;

int main()
{
	ifstream input;
	input.open("input.txt");
	string line;
	map<string, vector<string>> nodes;
	//vector<pair<string, string>> edges;
	list<pair<string, string>> edges;
	while (getline(input, line)) 
	{
		int separator = find(line.begin(), line.end(), ':') - line.begin();
		string key = line.substr(0, separator);
		line = line.substr(separator + 1);
		stringstream ss(line);
		string value;
		while (getline(ss, value, ' ')) 
		{
			if (value.length() == 0) continue;
			//On ajoute l'arête dans la liste (pour algo de Karger)
			edges.push_back({ key, value });
			nodes[key].push_back(value);
			//On ajoute de manière symétrique les nodes
			if (nodes.count(value) == 0 || find(nodes[value].begin(), nodes[value].end(), key) == nodes[value].end()) {
				nodes[value].push_back(key);
			}
		}
	}
	input.close();
	//S'appuyer sur le problème des ponts de Koënigsberg ?
	//Problème flot-max / coupe-min ?
	//Algorithme de Stoer-Wagner
	//On peut appliquer l'algorithme de Stoer-Wagner pour déterminer les deux sous-ensembles tels que la coupe du graphe soit 
	//de valeur 3 (en considérant que les arcs soient tous de valeur 1)
	//Algorithme de Karger ? => répeter tant que cardinal de la coupe (ie nombre d'arcs coupés) est différent de 3
	int cuts = 0;
	string node_1, node_2;
	while (cuts != 3) 
	{
		cuts = 0;
		//vector<pair<string, string>> KargerEdges(edges);
		list<pair<string, string>> KargerEdges(edges);
		int NodesCount = nodes.size();
		//Tant qu'on a plus de deux sommets dans notre graphe de karger
		while (NodesCount > 2) 
		{
			//On prend une des arêtes, et on superpose les deux sommets de ces arêtes en un nouveau sommet
			//Les arêtes des autres sommets qui reliaient un ou plusieurs de ces sommets sont alors converties chacunes
			//en une arête reliant le nouveau sommet
			int index = rand() % KargerEdges.size();
			//Le nom de ce point sera la concaténation des deux nodes à son origine
			auto iter = KargerEdges.begin();
			advance(iter, index);
			//string first = KargerEdges[index].first, second = KargerEdges[index].second;
			string first = iter->first, second = iter->second;
			string newNode = first + second;
			//On retire toutes les arêtes reliant les deux sommets
			//Note : ici, on a au maximum une connexion entre 2 éléments, on peut donc retirer uniquement à l'index trouvé
			/*KargerEdges.erase(remove_if(KargerEdges.begin(), KargerEdges.end(), [&](const pair<string, string>& element) {return (element.first == first && element.second == second) || 
				(element.second == first && element.first == second); }));*/
			//KargerEdges.erase(KargerEdges.begin() + index);
			KargerEdges.remove_if([&](const pair<string, string>& element) {return (element.first == first && element.second == second) ||
				(element.second == first && element.first == second); });
			//On remplace dans toutes les arêtes reliant un des deux sommets l'ancien sommet par le nouveau sommet
			for (auto& it : KargerEdges) 
			{
				if (it.first == "" || it.second == "") continue;
				if (it.first == first) {
					it.first = newNode;
				}
				else if (it.second == first) {
					it.second = newNode;
				}
				else if (it.first == second) {
					it.first = newNode;
				}
				else if (it.second == second) {
					it.second = newNode;
				}
			}
			NodesCount--;

		}
		//On calcule le cardinal de la coupe
		for (auto& it : KargerEdges) {
			if (it.first == "" || it.second == "") continue;
			cuts++;
		}
		//On récupère le nom des deux nodes finales
		auto iter = KargerEdges.begin();
		node_1 = iter->first;
		node_2 = iter->second;
		cout << cuts << endl;
	}
	//On sait que les noms des nodes sont composés de 3 lettres
	//Donc pour avoir le nombre de nodes par groupe, il nous suffit de diviser la longueur du nom par 3
	//Dans le cas général, il faudrait garder quelque part le nombre de nodes dans chaque superposition
	int total = (node_1.length() / 3) * (node_2.length() / 3);



	cout << "Part one : " << total << endl;


}

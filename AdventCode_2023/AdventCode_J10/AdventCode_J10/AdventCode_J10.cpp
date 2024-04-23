// AdventCode_J10.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

//On stocke la différence de coordonnée entre les deux case reliées par le tuyau
//Le repère est tel que le 0,0 se trouve en haut à gauche
map<char, pair<int, int>> pipes
{
	{'|', { 0, 2 }},
	{ '-', {2, 0} },
	{ 'L', {1, -1} },
	{ 'J', {-1, -1} },
	{ '7', {-1, 1} },
	{ 'F', {1, 1} },
	{ '.', {0, 0} }
};

map<string, char> startPipe
{
	{"LT", '7'},
	{"RT", 'L'},
	{"TD", '|'},
	{"LD", 'J'},
	{"RD", 'F'},
	{"LR", '-'}
};

int main()
{
	ifstream input;
	input.open("input.txt");
	vector<string> diagram;
	string line;
	int x = 0, y = 0, index = 0;
	while (getline(input, line))
	{
		int s_index = line.find('S');
		if (s_index != string::npos)
		{
			x = s_index;
			y = index;
		}
		diagram.push_back(line);
		index++;
	}
	input.close();
	//int prev_x = x, prev_y = y;
	vector<vector<pair<int, int>>> coords;
	vector<pair<int, int>> startCoords = { {x,y} };
	coords.push_back(startCoords);
	coords.push_back(startCoords);
	index = 0;
	//On récupère les deux cases de la boucle adjacentes au point de départ
	//(On fait attention de bien gérer le cas des barres droites, de la forme {0,2} ou  {2,0}
	string startPipeShape = "";
	if (x > 0 && pipes[diagram[y][x - 1]].first >= 1)
	{
		startPipeShape += "L";
		coords[index].push_back({ x - 1, y });
		index++;
	}
	if (x < diagram[0].length() - 1 && (pipes[diagram[y][x + 1]].first == -1 || pipes[diagram[y][x + 1]].first == 2))
	{
		startPipeShape += "R";
		coords[index].push_back({ x + 1, y });
		index++;
	}
	if (y > 0 && pipes[diagram[y - 1][x]].second >= 1)
	{
		startPipeShape += "T";
		coords[index].push_back({ x, y - 1 });
		index++;
	}
	if (y < diagram[0].length() - 1 && (pipes[diagram[y + 1][x]].second == -1 || pipes[diagram[y + 1][x]].second == 2))
	{
		startPipeShape += "D";
		coords[index].push_back({ x, y + 1 });
		index++;
	}
	//On modifie le tuyau de départ
	diagram[y][x] = startPipe[startPipeShape];

	//PART ONE
	//On va ensuite itérer sur les deux côté de la boucle en même temps, afin d'accélerer le processus
	//Structure de coords : 
	//coords[0] : case adjacente 1
	//coords[1] : case adjacente 2
	//coords[i][0] : coordonnées précédentes pour la case i, de la forme {x,y}
	//coords[i][1] : coordonnées actuelles pour la case i, de la forme {x,y}
	bool looped = false;
	int steps = 1;
	//Pour la partie 2, on sauvegarde les positions de la bordure de la boucle
	//On initialise la bordure avec les trois premiers points que l'on connaît
	//On stocke la bordure sous forme d'une map de vecteurs d'entiers, tels que les y soient les clés et les vecteurs des vecteurs de x
	//Cela permettra de simplifier les opérations de vérification si les points sont dans la boucle ou non par la suite
	map<int, vector<int>> border;
	border[y].push_back(x);
	border[coords[0][1].second].push_back(coords[0][1].first);
	border[coords[1][1].second].push_back(coords[1][1].first);
	//On cherche les coordonnées du carré le plus petit englobant la boucle
	pair<int, int> bottomLeft = { 
		min(min(x, coords[0][1].first), coords[1][1].first),max(max(y, coords[0][1].second), coords[1][1].second)},
		topRight = { 
		max(max(x, coords[0][1].first), coords[1][1].first),min(min(y, coords[0][1].second), coords[1][1].second) };

	while (looped == false) 
	{
		if (coords[0][0].first != x && coords[0][0].second != y && coords[0][0] == coords[1][0] || coords[0][0] == coords[1][1] || coords[0][1] == coords[1][0] || coords[0][1] == coords[1][1]) {
			looped = true;
			break;
		}
		for (int i = 0; i < coords.size(); i++) 
		{
			pair<int, int> cur = coords[i][1];
			pair<int, int> prev = coords[i][0];
			char pipe = diagram[cur.second][cur.first];
			//std::cout << pipe << " ";
			pair<int, int> pipeMovement = pipes[pipe];
			//On calcule le signe de la différence entre la position de la case actuelle et celle de la case précédente
			//afin d'avoir le sens de parcours du tuyau, et donc pouvoir calculer la prochaine position correctement.
			if (pipeMovement.first > 1) {
				//Dans le cas où on a une barre horizontale, on cherche le sens dans lequel appliquer l'incrément de position (gauche / droite)
				prev.first += (pipeMovement.first * (cur.first - prev.first));
			}
			else if (pipeMovement.second > 1) {
				//Dans le cas où on a une barre verticale, on cherche le sens dans lequel appliquer l'incrément de position (haut/ bas)
				prev.second += (pipeMovement.second * (cur.second - prev.second));
			}
			//Dans les cas suivants, on n'a que des coudes (les barres on été gérées précédemment)
			else if (cur.first - prev.first != 0) 
			{
				//dans le cas où la case actuelle se trouve à gauche ou a droite de la case précédente
				//on prend la coord X du tuyau et on calcule le nouvel Y
				prev.first = cur.first;
				prev.second += pipeMovement.second;

			}
			else if (cur.second - prev.second != 0) {
				//dans le cas où la case actuelle se trouve en haut ou bas de la case précédente
				//on prend la coord Y du tuyau et on calcule le nouvel X
				prev.first += pipeMovement.first;
				prev.second = cur.second;
			}
			//On a ensuite la nouvelle case à traiter
			coords[i][0] = cur;
			//La case actuelle devient la case précédente
			coords[i][1] = prev;
			//cout << cur.first << " " << cur.second << ", " << prev.first << " " << prev.second << endl;
			border[prev.second].push_back(prev.first);
			//cout << prev.first << " " << prev.second << endl;

			if (prev.first < bottomLeft.first) bottomLeft.first = prev.first;
			if (prev.first > topRight.first) topRight.first = prev.first;
			if (prev.second > bottomLeft.second) bottomLeft.second = prev.second;
			if (prev.second < topRight.second) topRight.second = prev.second;
		}
		//std::cout << endl;
		steps++;
	}
	std::cout << steps << endl;
	std::cout << bottomLeft.first << " " << bottomLeft.second << ", " << topRight.first << " " << topRight.second << endl;
	
	for (auto& it : border) {
		cout << it.first << " " << endl;
		for (auto& i : it.second) {
			cout << i;
		}
		cout << endl;
	}
	
	//Pour chacun des points contenus dans le carré englobant la boucle (en excluant les points de la bordure de la boucle)
	int inside = 0;
	//Est-il utile de considérer la bordure du carré ? => oui, dans le cas où le carré fait la taille de la grille de départ
	for (int i = bottomLeft.first; i <= topRight.first; i++) {
		for (int j = topRight.second; j <= bottomLeft.second; j++) {
			//Si le point appartient à la bordure de la boucle, on passe au point suivant
			if (border.count(j) && find(border[j].begin(), border[j].end(), i) != border[j].end()) {
				continue;
			}
			else
			{
				std::cout << i << " " << j << endl;
				//On va "tracer" un segment entre le point et un point en dehors du carré englobant, et regarder combien de 
				//fois on traverse la frontière de la boucle
				// Si on trouve un nombre impair de traversée de la boucle, alors on est à l'intérieur
				// Sinon on est à l'extérieur
				// ........
				// ........
				// ..F--7..
				// .*|*.|..
				// ..L--J..
				// ........
				// ........
				// ........
				int crossing = 0;
				pair<int, int> segmentPoint;
				
				//On choisit le segment tel que il relie le point actuel à un point à gauche en dehors du carré 
				//On ne démarre la comparaison du point qu'à partir de la bordure du carré car tout point en dehors
				//du carré est forcément en dehors de la boucle (le carré englobe la boucle)
				//Note : on traite le cas où le segment longe un des bords de la boucle
				//On ne doit en effet considérer cela comme aucune coupe (le segment est infiniment proche de la bordure)
				//Donc, pour chacun des segments de bordure que l'on rencontre le long du segment, on doit vérifier si c'est une bordure longée
				//Celle-ci sera de cette forme 
				//.|..|.
				//*L--J*
				//......
				// Ou
				//......
				//*F--7*
				//.|..|.
				//On va donc regarder si aux extrémités de ce segment, on trouve les points suivants du même côté du segment ou bien de côtés différents
				//Si même, alors bordure non coupée,
				//Si différents, alors bordure coupée

				int first_x = bottomLeft.first, last_x = bottomLeft.first;
				int seg_length = 0;
				//Pour optimiser, on pourrait chercher la direction dans laquelle se trouvent le moins de cases à vérifier
				for (int k = bottomLeft.first; k <= i; k++) 
				{
					//Si vrai, alors la ligne où se trouve le point ne contient aucun point de la bordure de la boucle, il est donc en dehors de la boucle
					if (border.count(j) == 0) {
						break;
					}
					//On regarde si la case actuelle fait partie de la bordure ET connecte avec la case précédente ou bien commence une section de frontière
					if (find(border[j].begin(), border[j].end(), k) != border[j].end()) 
					{
						char symbol = diagram[j][k];
						//Si on n'a pas encore de segment de bordure en cours de parcours
						if (seg_length == 0) 
						{
							first_x = k;
							seg_length = 1;
						}
						//Sinon, on a déjà un segment en cours de parcours
						else 
						{
							//On continue une section de bordure
							if (diagram[j][k] == '-' || diagram[j][k] == '7' || diagram[j][k] == 'J') {
								//La case actuelle fait partie de la bordure et continue la section de bordure précédente
								seg_length++;
							}
							//La section de bordure s'arrête
							else 
							{
								last_x = k - 1;
								//On regarde si les extrémités partent du même côté ou non, ou bien si le segment est de longueur 1
								char f = diagram[j][first_x], l = diagram[j][last_x];
								if (pipes[f].second != pipes[l].second || seg_length == 1) {
									crossing++;
								}
								//Puisqu'on est sûr que le segment appartient à la bordure,comme ce segment ne fait pas la
								//continuité du segment précédent, c'est forcément le début d'un nouveau segment
								seg_length = 1;
								first_x = k;
							}
						}
					}
					//Si la case ne fait pas partie de la bordure
					else 
					{
						//Si on avait un segment en cours de parcours, on l'arrête
						if (seg_length > 0) {
							last_x = k - 1;
							//On regarde maintenant si les deux extrémités partent bien une vers le haut et une vers le bas
							char f = diagram[j][first_x], l = diagram[j][last_x];
							if (pipes[f].second != pipes[l].second || seg_length == 1) {
								crossing++;
							}
							seg_length = 0;
						}
					}
					
				}
				if (crossing % 2 != 0) {
					inside++;
					std::cout << "inside point" << endl;
				}
			}
		}
	}

	std::cout << inside << endl;

}

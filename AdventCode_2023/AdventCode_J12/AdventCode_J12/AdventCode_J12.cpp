// AdventCode_J12.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <list>

using namespace std;

//bool find_sequence(int start, int length, string sequence) 
//{
//	//Si la séquence que l'on cherche est strictement plus grande que la taille de la sequence que l'on a, la séquence ne peut se trouver ici
//	if (start + length > sequence.length()) return false;
//	//Sinon si la sequence fait exactement la taille, on retourne vrai
//	else if (start + length == sequence.length()) return true;
//	//Sinon si après le segment que l'on a pris on trouve un #, la séquence qu'on cherche ne peut pas se trouver ici (sinon elle serait plus longue)
//	else if (sequence[start + length] == '#') return false;
//	//Si on n'est pas dans le cas précédent, on a bien trouvé la sous séquence
//	else return true;
//}
//
//vector<int> find_borders(int start, vector<int>& sequencesLength, string sequence, vector<pair<int, int>>& hashtagSequences) 
//{
//	/*int remainingLength = 0;
//	for (int i = sequence_index + 1; i < sequencesLength.size(); i++) {
//		remainingLength += (sequencesLength[i] + 1);
//	}*/
//	int first = start, last = start, last_count = 0;
//	bool setFirst = false;
//	//On regarde, pour chacune des séquences de '#', si on doit la prendre en compte pour notre séquence
//	for (auto& it : hashtagSequences) 
//	{
//		//Si on à déjà une borne inférieur et que la borne supérieure est supérieur à la longueur max de la chapine, on ne la prend pas en compte et on s'arrête
//		if (first >= 0 && it.first >= first + sequencesLength[0]) break;
//		//Les indices suivants seront en effet plus grands, du fait que l'on a regardé les chaînes de gauche à droite
//		if (setFirst == false) {
//			setFirst = true;
//			first = it.first;
//		}
//		last = it.first + it.second - 1;
//		last_count = it.second;
//	}
//	if (setFirst == false) {
//		//Dans ce cas, on n'a trouvé aucune séquence de '#' concernant notre sous-séquence
//		last = sequence.length() - 1;
//	}
//	else {
//		//Si les '#' ne bornent pas la séquence (par exemple .?##?. pour une séquence de longueur 3)
//		int missingLength = sequencesLength[0] - (last - first + 1);
//		if (missingLength > 0)
//		{
//			//On détermine quelles sont les séquences contenant les deux extrémités
//			//Ex : pour .?#?##?. avec une séquence de longueur 5
//			first = max(0, first - missingLength);
//			last = min((int)sequence.length() - 1, last + missingLength);
//		}
//	}
//	vector<int> result({first, last, last_count});
//	return result;
//}
//
////On calcule récursivement tous les arrangements possibles
////Comme on doit avoir les fontaines côtes à côtes, dans une séquence de ? de longueur N (> n), on a 
////au maximum N - n + 1 séquences de # de longueur n  
//pair<int, vector<int>> sequence_possibilites(int start, string sequence, vector<int>& sequences_length, vector<pair<int, int>>& hastagSequences) 
//{
//
//	//On calcule la longueur totale des sequences restantes à trouver
//	// = somme (longueur + 1) - 1 (le +1 est dû au point entre 2 sequences, et le -1 car la 1ère sequence le point est déjà pris en compte
//	/*int seq_tot_length = -1;
//	for (int i = sequence_index; i < sequences_length.size(); i++) {
//		seq_tot_length += (sequences_length[i] + 1);
//	}*/
//	int tot_possibilites = 0;
//
//	//On étudie toutes les séquences consécutives de gauche à droite
//	//Ex : .??????.. et on veut placer 2,2
//	//On regarde donc pour .##.???.. (A) et ..##.??.. (B) à cette hauteur
//	//Par appel récursif, on aura donc à la profondeur + 1 .##.##... et .##..##.. pour A
//	//Et ..##.##.. pour B
//	//Donc en tout 3 possibilités
//	vector<int> remainingSequences;
//	//Si on trouve un/des '#' dans la partie comprise entre les indices 0 et longueur totale - somme longueur partie (cette section exclue)
//	//Alors elle doit obligatoirement appartenir à la section que l'on considère actuellement
//	//Cela réduit donc le champs des possibilités, puisque l'on doit obligatoirement comporter dans la chaîne ce/ces '#'
//	//On va donc calculer l'indice min et l'indice max possible pour notre chaîne actuelle, tels que la chaîne contienne toujours le / les '#' existants
//	vector<int> borders = find_borders(start, sequences_length, sequence, hastagSequences);
//	//Premier indice du début de la séquence que l'on considère pour les possibilités
//	int first = borders[0];
//	//Dernier indice du début de la séquence que l'on considère pour les possibilités
//	int last = borders[1] - sequences_length[0] + 1;
//	for (int i = first; i <= last; i++) 
//	{
//		if (find_sequence(start, sequences_length[0], sequence) == false) break;
//		//Si on est la dernière séquence ou bien qu'on ne peut plus placer une autre séquence après celle-ci, on est la dernière séquence de cette sous-séquence
//		if (sequences_length.size() == 1 || sequences_length[1] > (int)sequence.length() - sequences_length[0] - 1) {
//			//On est à la dernière séquence, on ajoute donc une possibilité et on continue
//			tot_possibilites += 1;
//			continue;
//		}
//		//Si on ne peut plus trouver un nouveau début de séquence dans la chaîne, on s'arrête
//		if (i + sequences_length[0] + 1 >= sequence.length()) break;
//		//Il reste encore au moins une séquence suivante à placer, on applique donc récursivement la fonction
//		vector<pair<int, int>> nextHashtagSequences;
//		for (int i = 0; i < hastagSequences.size(); i++) {
//			if (hastagSequences[i].first > borders[1]) nextHashtagSequences.push_back(hastagSequences[i]);
//		}
//		vector<int> remainingSequences;
//		for (int i = 1; i < sequences_length.size(); i++) {
//			remainingSequences.push_back(sequences_length[i]);
//		}
//		pair<int, vector<int>> result = sequence_possibilites(i + sequences_length[0] + 1, sequence, remainingSequences, nextHashtagSequences);
//		tot_possibilites += result.first;
//		if (remainingSequences.size() == 0 || remainingSequences.size() > result.second.size())
//			remainingSequences = vector<int>(result.second);
//	}
//	for (int i = 1; i < sequences_length.size(); i++) {
//		remainingSequences.push_back(sequences_length[i]);
//	}
//	return pair<int, vector<int>>(tot_possibilites, remainingSequences);
//}

bool possible_sequence(int start, int seq_index, string sequence, int length) 
{
	if (start + length > (int)sequence.length()) return false;
	//On ne peut pas commencer la séquence là car on a un '#' juste avant (donc la séquence la contenant à déjà été prise en compte)
	if (start > 0 && sequence[start - 1] == '#') return false;
	for (int i = 0; i < length; i++) 
	{
		//On a un point au milieu de la séquence, ce qui n'est pas valide
		if (sequence[i + start] == '.') return false;
	}
	//On ne peut pas placer la séquence car on a un '#' juste après (ce qui nécessiterait que la séquence soit plus longue
	if (sequence[start + length] == '#') return false;
	return true;
}

bool point_in_sequence(int start, int seq_index, string sequence, int length) 
{
	for (int i = 0; i < length; i++)
	{
		//On a un point au milieu de la séquence, ce qui n'est pas valide
		if (sequence[i + start] == '.') return true;
	}
	return false;
}

//On cherche le point le plus proche tel qu'il se situe APRES une séquence
int find_next_point(int start, string sequence) 
{
	bool found_symbol = false;
	for (int i = start; i < sequence.length(); i++)
	{
		if (sequence[i] != '.') {
			found_symbol = true;
		}
		else if (found_symbol == true) {
			return i;
		}
	}
	return -1;

}

//Le principe de l'algorithme consiste à parcourir récursivement la chaine de caractères
//Pour chaque séquence à placer (de gauche à droite), on va étudier toutes les possibilités de combinaison en appelant
//récursivement la fonction pour la séquence suivante et le reste de la chaine, et ainsi de suite jusqu'à arriver au placement de la dernière séquence
long long calculate_combinations(int start, int seq_index, string sequence, vector<int>& sequences_length, map<pair<int,int>, long long>& cache, int sequence_base_length) 
{
	long long total = 0, max_index = 0;
	//On calcule l'indice max auquel on peut aller pour ne pas travailler sur une possibilité impossible
	//Si on n'a plus assez de place pour poser les séquences suivantes, la combinaison sera impossible, on peut donc s'arrêter avant
	max_index = sequence.length();
	for (int i = seq_index + 1; i < sequences_length.size(); i++)
	{
		max_index -= (sequences_length[i] + 1);
	}
	max_index += 1;
	int remaining_length = 0;
	for (int i = seq_index; i < sequences_length.size(); i++) {
		remaining_length += sequences_length[i];
	}
	//On va étudier toutes les possibilités de placement de la séquence seq_index
	for (int i = start; i <= max_index; i++) 
	{
		
		//Si la séquence n'est pas possible, on continue avec la suivante
		if (possible_sequence(i, seq_index, sequence, sequences_length[seq_index]) == false) continue;

		string substr_left = sequence.substr(start, i - start);
		//On vérifie s'il ne reste pas des '#' non couverts par la séquence à gauche de celle-ci
		//Si c'est le cas, alors la combinaison actuelle n'est pas valide
		if (substr_left.length() > 0 && find(substr_left.begin(), substr_left.end(), '#') != substr_left.end())
		{
			//cout << "invalid" << endl;
			//On arrête alors la boucle car on s'éloignera encore plus des '#' au fur et à mesure
			break;
		}

		if (seq_index == sequences_length.size() - 1) 
		{
			//On a réussi à placer la dernière séquence
			//On va donc vérifier si il ne reste pas des '#' dans les morceaux de chaîne restant de chaque côté de notre séquence
			//Si il en reste, la combinaison n'est pas valide
			//Sinon, c'est ok
			
			//Il ne reste plus rien dans la séquence, donc on est au bout et la combinaison est valide
			if (i + sequences_length[seq_index] + 1 >= (int)sequence.length()) {
				total += 1;
				//PART TWO
				cache[{i, remaining_length}] = 1;
				//END PART TWO

				//cout << seq_index << " : " << i << " " << i + sequences_length[seq_index] - 1 << endl;
				continue;
			}
			string substr = sequence.substr(i + sequences_length[seq_index] + 1);
			//Il reste un ou plusieurs '#', la séquence n'est donc pas valide
			if (find(substr.begin(), substr.end(), '#') != substr.end()) {
				//cout << "invalid" << endl;
				continue;
			}
			else 
			{
				total += 1;
				//PART TWO
				cache[{i, remaining_length}] = 1;
				//END PART TWO
				//cout << seq_index << " : " << i << " " << i + sequences_length[seq_index] - 1 << endl;
				continue;
			}
		}

		//PART TWO
		//On utilise un cache afin d'éviter de recalculer des possibilités déjà calculées (dû au fait que l'on concatène la séquence plusieurs fois pour composer la nouvellme)
		//On regarde ainsi si on n'a pas déjà les combinaisons pour un index précis, pour un certain nombre de fontaines cassées restantes à placer
		//Chercher le cache pour élément ACTUEL (
		if (cache.find({ i , remaining_length }) != cache.end())
		{
			//Si oui, on additionne le nombre de combinaisons que l'on avait déjà calculé
			total += cache[{ i , remaining_length}];
			continue;
		}
		//END PART TWO

		//cout << seq_index << " : " << i << " " << i + sequences_length[seq_index] - 1 << endl;
		long long count = calculate_combinations(i + sequences_length[seq_index] + 1, seq_index + 1, sequence, sequences_length, cache, sequence_base_length);
		
		//PART TWO
		cache[{i, remaining_length}] = count;
		//END PART TWO
		
		total += count;
	}
	return total;
}

int main()
{
	ifstream input;
	long long result = 0;
	string line, subLine;
	input.open("input.txt");
	vector<pair<string, vector<int>>> springs;
	while (getline(input, line)) 
	{
		//On parcours les lignes du fichier une à une
		//On stocke chaque ligne, et pour chaque ligne on stocke la liste des nombres
		stringstream ss(line);
		getline(ss, subLine, ' ');
		string numbers, num;
		getline(ss, numbers, ' ');
		vector<int> counts;
		stringstream nums(numbers);
		while (getline(nums, num, ',')) {
			counts.push_back(stoi(num));
		}
		springs.push_back({ subLine, counts });

	}
	input.close();
	//Les entiers indiquent la longueur de chaque rangée de fontaines cassées, i.e. la longueur de chaque section de # dans la ligne
	for (auto& it : springs) 
	{
		stringstream ss(it.first);
		string sequence;
		int seq_index = 0;
		long long seq_comb = 0;
		vector<int> sequences_length;
		for (auto itt : it.second) {
			sequences_length.push_back(itt);
		}
		
		//while (getline(ss, sequence, '.')) 
		//{
		//	if (sequence.length() > 0) 
		//	{
		//		//Pour chaque séquence, on regarde combien de séquences de fontaines cassées on peut placer
		//		//A SUPPRIMER CAR IL Y A DES CAS OU LE PREMIER ? EST UN POINT OBLIGATOIREMENT,CE QUI FAUSSE LES CALCULS
		//		//int num_seq = 0, seq_length = -1;
		//		//for (int i = seq_index; i < it.second.size(); i++) {
		//		//	//On regarde si on peut mettre une séquence de plus dans la longueur de la séquence actuelle
		//		//	if (seq_length + 1 + it.second[i] <= sequence.length()) {
		//		//		num_seq++;
		//		//		seq_length += 1 + it.second[i];
		//		//	}
		//		//	else {
		//		//		break;
		//		//	}
		//		//}
		//		
		//		//Si il n'y a pas de '?', alors la séquence est déjà complète (on skip)
		//		if (find(sequence.begin(), sequence.end(), '?') == sequence.end()) {
		//			seq_index++;
		//			continue;
		//		}
		//		//On va récupérer l'emplacement de toutes les séquences de '#' déjà placées ainsi que la longueur de ces 
		//		/*vector<pair<int, int>> hastagSequences;
		//		if (find(sequence.begin(), sequence.end(), '#') != sequence.end()) 
		//		{
		//			int first = find(sequence.begin(), sequence.end(), '#') - sequence.begin();
		//			hastagSequences.push_back({ first, 1 }); 
		//			int j = 0;
		//			for (int i = first + 1; i < sequence.length(); i++) 
		//			{
		//				if (sequence[i] == '#') {
		//					if (sequence[i - 1] == '#') hastagSequences[j].second += 1;
		//					else {
		//						hastagSequences.push_back({ i, 1 });
		//						j++;
		//					}
		//				}
		//			}
		//		}*/
		//		
		//		//Ensuite, pour la sous séquence, on va calculer récursivement combien de possibilités existent de disposition
		//		/*vector<int> sequences_length;
		//		for (int i = 0; i < num_seq; i++) {
		//			sequences_length.push_back(it.second[seq_index + i]);
		//		}*/

		//		//TODO
		//		//Enlever les sequences au fur et à mesure que l'on les parcours dans le vecteur "sequences_length"
		//		//Ou alors promener l'indice de départ via REFERENCE
		//		// TODO
		//		pair<int, vector<int>> result = sequence_possibilites(0, sequence, sequences_length, hastagSequences);
		//		seq_comb *= result.first;
		//		sequences_length = vector<int>(result.second);
		//		//seq_index += num_seq;

		//	
		//	}
		//}

		//PART ONE
		sequence = it.first;
		//PART TWO
		int base_length = sequence.length() + 1;
		for (int i = 0; i < 4; i++) 
		{
			sequence += "?";
			sequence += it.first;
			for (auto itt : it.second) {
				sequences_length.push_back(itt);
			}
		}

		map<pair<int,int>, long long> cache;
		//cout << sequence << endl;

		seq_comb = calculate_combinations(0, 0, sequence, sequences_length, cache, base_length);
		result += seq_comb;
		cout << seq_comb << endl;
	}
	cout << result << endl;


}

// AdventCode_J20.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>

using namespace std;

long long pgcd(long long a, long long b) 
{
    int tmp = 0;
    while (b > 0) 
    {
        tmp = b;
        b = a % b;
        a = tmp;
    }
    return a;
}

long long ppcm(long long a, long long b) 
{
    return (a * b) / pgcd(a, b);
}

int main()
{
    ifstream input;
    input.open("input.txt");
    string line, delimiter = " ->";
    int low = 0, high = 0;
    //Map stockant comme clé les modules d'entrées et comme valeur un couple le type de module d'entrée et les différents modules de destination
    map<string, pair<char, vector<string>>> modules;
    //MAp stockant, pour chaque module, la dernière valeur reçue des parents
    //Pour un module &, on stocke effectivement l'état des différents parents (car l'état de la sortie dépend des pulse les plus récents reçus des parents)
    //Pour un module %, on va seulement stocker l'état du module (car l'état de la sortie dépend uniquement de son état)
    //Le booléen indique si le module est à HAUT (true) ou BAS (false)
    map<string, map<string, bool>> modules_parents;
    while (getline(input, line)) 
    {
        int index = line.find(delimiter);
        if (index == string::npos) return -1;
        string subline = line.substr(0, index);
        vector<string> childs;
        stringstream ss(line.substr(index + delimiter.length()));
        string module_;
        while (getline(ss, module_, ',')) 
        {
            //On enlève l'espace au début
            childs.push_back(module_.substr(1));
        }
        switch (subline[0]) {
        case '%':            
            //Flip flop
            modules[subline.substr(1)] = pair<char, vector<string>>({ '%' }, childs);
            //Pour un flip-flop, l'état de sortie dépend de l'état du module
            modules_parents[subline.substr(1)][subline.substr(1)] = false;
            for (auto& it : childs) {
                //Pour chacun des enfants, on donne l'état du parent
                modules_parents[it][subline.substr(1)] = false;
            }
            break;
        case '&':
            //Conjunction
            modules[subline.substr(1)] = pair<char, vector<string>>({ '&' }, childs);
            //Pour un conjunction, l'état de sortie dépend des valeurs les plus récentes reçues de chaque parent
            for (auto& it : childs) {
                modules_parents[it][subline.substr(1)] = false;
            }
            break;
        default:
            //Broadcaster
            modules[subline] = pair<char, vector<string>>({ ' ', childs });
            //on n'a pas de parents pour le broadcaster (si ce n'est évidemment le bouton)
            break;
        }
    }
    input.close();
    //Liste des pulses à traiter à l'étape n et celle à l'étape n+1
    //On a, pour le module qui doit traiter le signal, la valeur envoyée ainsi que le module qui l'a envoyé
    vector<pair<string, pair<bool, string>>> pulses, nextPulses;

    //PART ONE
    //for (int i = 0; i < 1000; i++) {
    //    //Le premier signal est celui envoyé au broadcaster par le bouton
    //    pulses.push_back({ "broadcaster", {false, ""} });
    //    while (pulses.size() > 0) {
    //        for (auto& it : pulses)
    //        {
    //            bool output = false;
    //            switch (modules[it.first].first)
    //            {
    //            case '%':
    //                //On ajoute le signal qu'on a reçu au compte des signaux
    //                if (it.second.first == true) high++;
    //                else low++;
    //                //Si un flip-flop reçoit un HIGH, on ne fait rien
    //                if (it.second.first == true) continue;
    //                //On inverse l'état du flip-flop et on envoie un signal HAUt ou BAS selon le nouvel état du flip-flop
    //                modules_parents[it.first][it.first] = !modules_parents[it.first][it.first];
    //                output = modules_parents[it.first][it.first];
    //                for (auto& nextModules : modules[it.first].second)
    //                {
    //                    //On envoie un signal à tous les enfants du module
    //                    nextPulses.push_back({ nextModules, {output, it.first } });
    //                }
    //                break;
    //            case '&':
    //                //On ajoute le signal qu'on a reçu au compte des signaux
    //                if (it.second.first == true) high++;
    //                else low++;
    //                //On met à jour le dernier signal reçu pour ce module par le parent
    //                modules_parents[it.first][it.second.second] = it.second.first;
    //                //On détermine ensuite l'état du signal de sortie
    //                //Si on a que des HIGH en entrée, on envoye LOW; sinon on envoye HIGH
    //                for (auto& parents : modules_parents[it.first]) {
    //                    if (parents.second == false) {
    //                        output = true;
    //                        break;
    //                    }
    //                }
    //                for (auto& nextModules : modules[it.first].second) {
    //                    //On envoie un signal à tous les enfants du module
    //                    nextPulses.push_back({ nextModules, {output, it.first } });
    //                }
    //                break;
    //            default:
    //                //On ajoute le signal qu'on a reçu au compte des signaux
    //                if (it.second.first == true) high++;
    //                else low++;
    //                //On envoie le même signal reçu par le broadcaster à tous ses enfants
    //                output = it.second.first;
    //                for (auto& nextModules : modules[it.first].second) {
    //                    nextPulses.push_back({ nextModules, {it.second.first, it.first} });
    //                }
    //                break;
    //            }
    //        }
    //        pulses = vector<pair<string, pair<bool, string>>>(nextPulses);
    //        nextPulses.clear();
    //    }
    //}
    /*long long total = (long long)low * (long long)high;
    cout << total << endl;*/

    //PART TWO
    int bp_mp = 0, bp_qt = 0, bp_qb = 0, bp_ng = 0, buttonPresses = 0;
    while (bp_mp == 0 || bp_qt == 0 || bp_qb == 0 || bp_ng == 0) 
    {
        buttonPresses++;
        //cout << buttonPresses << endl;
        //Le premier signal est celui envoyé au broadcaster par le bouton

        //TODO
        //Pour trouver à quel moment on arrive à avoir un LOW à rx, on va chercher à savoir à quel moment (=combien de pressions de bouton) on arrive à avoir un HIGH
        // Sur chacune des entrées de &dr (comme c'est un &, si toutes les entrées sont à HIGH il envoie LOW)
        // Ces entrées sont mp, qt, qb et ng
        // Une fois qu'on aura pour chacune des entrées, on aura plus qu'a calculer le PPCM de ces 4 nombres
        // 
        // Pour que ces entrées soient à HIGH, il faut qu'au moins une de leurs entrées soit à LOW
        // On doit donc essayer de déterminer à quel nombre d'appuis au moins une des entrées de chaque entrée de dr est à LOW
        //TODO

        pulses.push_back({ "broadcaster", {false, ""} });
        while (pulses.size() > 0) {
            for (auto& it : pulses)
            {
                if (bp_mp != 0 && bp_ng != 0 && bp_qb != 0 && bp_qt != 0) {
                    goto end;
                }
                bool output = false;
                switch (modules[it.first].first)
                {
                case '%':
                    //On ajoute le signal qu'on a reçu au compte des signaux
                    if (it.second.first == true) high++;
                    else low++;
                    //Si un flip-flop reçoit un HIGH, on ne fait rien
                    if (it.second.first == true) continue;
                    //On inverse l'état du flip-flop et on envoie un signal HAUt ou BAS selon le nouvel état du flip-flop
                    modules_parents[it.first][it.first] = !modules_parents[it.first][it.first];
                    output = modules_parents[it.first][it.first];
                    for (auto& nextModules : modules[it.first].second)
                    {
                        //On envoie un signal à tous les enfants du module
                        nextPulses.push_back({ nextModules, {output, it.first } });
                        if (nextModules.compare("ng") == 0 && output == false && bp_ng == 0) {
                            bp_ng = buttonPresses;
                        }
                        if (nextModules.compare("mp") == 0 && output == false && bp_mp == 0) {
                            bp_mp = buttonPresses;
                        }
                        if (nextModules.compare("qt") == 0 && output == false && bp_qt == 0) {
                            bp_qt = buttonPresses;
                        }
                        if (nextModules.compare("qb") == 0 && output == false && bp_qb == 0) {
                            bp_qb = buttonPresses;
                        }
                    }
                    break;
                case '&':
                    //On ajoute le signal qu'on a reçu au compte des signaux
                    if (it.second.first == true) high++;
                    else low++;
                    //On met à jour le dernier signal reçu pour ce module par le parent
                    modules_parents[it.first][it.second.second] = it.second.first;
                    //On détermine ensuite l'état du signal de sortie
                    //Si on a que des HIGH en entrée, on envoye LOW; sinon on envoye HIGH
                    for (auto& parents : modules_parents[it.first]) {
                        if (parents.second == false) {
                            output = true;
                            break;
                        }
                    }
                    for (auto& nextModules : modules[it.first].second) {
                        //On envoie un signal à tous les enfants du module
                        nextPulses.push_back({ nextModules, {output, it.first } });
                        if (nextModules.compare("ng") == 0 && output == false && bp_ng == 0) {
                            bp_ng = buttonPresses;
                        }
                        if (nextModules.compare("mp") == 0 && output == false && bp_mp == 0) {
                            bp_mp = buttonPresses;
                        }
                        if (nextModules.compare("qt") == 0 && output == false && bp_qt == 0) {
                            bp_qt = buttonPresses;
                        }
                        if (nextModules.compare("qb") == 0 && output == false && bp_qb == 0) {
                            bp_qb = buttonPresses;
                        }
                    }
                    break;
                default:
                    //On ajoute le signal qu'on a reçu au compte des signaux
                    if (it.second.first == true) high++;
                    else low++;
                    //On envoie le même signal reçu par le broadcaster à tous ses enfants
                    output = it.second.first;
                    for (auto& nextModules : modules[it.first].second) {
                        nextPulses.push_back({ nextModules, {it.second.first, it.first} });
                        if (nextModules.compare("ng") == 0 && output == false && bp_ng == 0) {
                            bp_ng = buttonPresses;
                        }
                        if (nextModules.compare("mp") == 0 && output == false && bp_mp == 0) {
                            bp_mp = buttonPresses;
                        }
                        if (nextModules.compare("qt") == 0 && output == false && bp_qt == 0) {
                            bp_qt = buttonPresses;
                        }
                        if (nextModules.compare("qb") == 0 && output == false && bp_qb == 0) {
                            bp_qb = buttonPresses;
                        }
                    }
                    break;
                }
            }
            pulses = vector<pair<string, pair<bool, string>>>(nextPulses);
            nextPulses.clear();
        }
    }
end:

    long long res = ppcm(bp_qb, bp_qt);
    res = ppcm(res, bp_ng);
    res = ppcm(res, bp_mp);
    cout << res << endl;



}

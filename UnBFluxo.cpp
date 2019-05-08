#include <bits/stdc++.h>

using namespace std;


struct materia{ //Estrutura da materia
    string nome;
    int creditos;
    int id;
};

typedef vector<materia> vertex; //Abstração Vertices do Grafo
typedef vector<pair<materia,vertex>> unbFluxo; //Abstração o Grafo 

void graphCreate(unbFluxo CiC){

    ifstream myfile ("materias.txt");
    string line;
    int i = 0;

    if(myfile.is_open()){ //verifico se o arquivo esta aberto
        while(getline(myfile,line)){ //pego todas as linhas
            materia nova_materia;
            materia pre_requisito;
            if (line[line.length() - 1] == '{'){ //verfico se a primeira materia pegada 
                CiC.push_back(vertex());
                line.pop_back();
                nova_materia.nome = line;
                getline(myfile,line);
                nova_materia.id = stoi(line);
                getline(myfile,line);
                nova_materia.creditos = stoi(line);
                getline(myfile,line);
                line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
                if( line == "PreRequisitos:")
                // CiC[i].push_back(make_pair(nova_materia, pre_requisito));
            }
            i++;
        }
        myfile.close();
    } else {
        cout << "Não foi possível abrir o arquivo para criar o fluxo verifique se possui o arquivo materias.txt" << endl;
    }
}



int main() {

    unbFluxo CiC;

    graphCreate(CiC);


    return 0;
}
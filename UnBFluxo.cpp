#include <bits/stdc++.h>

using namespace std;


struct materia{ //Estrutura da materia
    string nome;
    int creditos;
    int id;
};

typedef vector<materia> vertex; //Abstração Vertices do Grafo
typedef vector<pair<materia,vertex>> unbFluxo; //Abstração o Grafo 

unbFluxo graphCreate(unbFluxo CiC){

    ifstream myfile ("materias.txt");
    string line;

    if(myfile.is_open()){ //verifico se o arquivo esta aberto
        while(getline(myfile,line)){ //pego todas as linhas
            materia nova_materia, pre_materia;
            vertex pre_requisito;
            if (line[line.length() - 1] == '{'){ //verfico se a primeira materia pegada 
                line.pop_back();  //tiro a chaves da string
                nova_materia.nome = line; //bota na estrutura nome da materia
                getline(myfile,line); //pego id da materia
                nova_materia.id = stoi(line); //bota na estrutura da id materia
                getline(myfile,line); //pego a estrutura 
                nova_materia.creditos = stoi(line); //bota estrutura 
                getline(myfile,line); //Pega a linhas de PreRequisitos
                while(line[0] != '}'){ //Pega todas as materias ate o fecha chaves 
                    getline(myfile,line); //Pega todas as linhas  
                    if(line[0] != '}'){ //Pega so os valores dos PreRequisitos
                        pre_materia.nome = line;
                        getline(myfile,line);
                        pre_materia.id = stoi(line);
                        getline(myfile,line);
                        pre_materia.creditos = stoi(line);
                        pre_requisito.push_back(pre_materia); 
                    }
                }
                CiC.push_back(make_pair(nova_materia,pre_requisito)); //cria um vertices de tuplas 
            }
        }
        myfile.close();
    } else {
        cout << "Não foi possível abrir o arquivo para criar o fluxo verifique se possui o arquivo materias.txt" << endl;
    }
    return CiC;
}

void printaPreRequisitos(materia mat){
    cout << "E tem como Pre Requisitos essas materias" << endl;
    cout << mat.nome << endl;
    cout << mat.id << endl;
    cout << mat.creditos << endl;

    cout << endl;
    cout << endl;
}

void printaPair(pair<materia, vertex> p){
    cout << "A materia é : " << p.first.nome << endl;
    for_each(p.second.begin(), p.second.end(),printaPreRequisitos);
}

void printaGrafo(unbFluxo Graph){
    for_each(Graph.begin(), Graph.end(), printaPair);
}




int main() {

    unbFluxo CiC;

    CiC = graphCreate(CiC);

    cout << CiC.size() << endl;

    printaGrafo(CiC);


    return 0;
}
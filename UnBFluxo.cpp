#include <bits/stdc++.h>
using namespace std;


struct Materia{
    string nome;
    int creditos;
    int numReq;
    int codigo;
    int id;
};

typedef vector<int> vertex; //Abstração Vertices do Grafo
typedef vector<vertex> Grafo; //Abstração o Grafo 

pair<vector<Materia>, pair<Grafo, Grafo>> graphCreate(string nomeArquivo){
    
    pair<vector<Materia>, pair<Grafo, Grafo>> res;
    auto &materias = res.first;
    auto &grafo = res.second.first;
    auto &grafoInverso = res.second.second;
    ifstream myfile(nomeArquivo);
    string line;
    int aux, val1, val2, val3, chave = 0;
    map<int, int> mapa1;
    vector<int> mapa2;

    if(myfile.is_open()){ //verifico se o arquivo esta aberto
        getline(myfile,line);
        aux = stoi(line);
        materias.resize(aux);
        grafo.resize(aux);
        grafoInverso.resize(aux);
        mapa2.resize(aux);
        for(auto&p : materias){
            getline(myfile, p.nome);
            getline(myfile, line);
            stringstream linha(line);
            linha >> val1 >> val2 >> val3;
            mapa2[chave] = val1;
            mapa1[val1] = chave;
            p.creditos = val3;
            p.codigo = val1;
            p.numReq = val2;
            p.id = chave;
            ++chave;
        }
        getline(myfile,line);
        aux = stoi(line);
        for(int i = 0; i < aux; ++i){
            getline(myfile,line);
            stringstream linha(line);
            linha >> val1 >> val2;
            grafo[mapa1[val2]].push_back(mapa1[val1]);
            grafoInverso[mapa1[val1]].push_back(mapa1[val2]);
        }
        myfile.close();
    } else {
        cout << "Não foi possível abrir o arquivo para criar o fluxo verifique se possui o arquivo materias.txt" << endl;
    }
    return res;
}


int main(int argc, char *argv[]) {
    auto grafoTotal = graphCreate("materias.txt");
    auto &materias = grafoTotal.first;
    auto &grafo = grafoTotal.second.first;
    auto &grafoInverso = grafoTotal.second.second;
    
    stack<int> atuais;
    vector<int> preReg(materias.size()), ordem(materias.size());
    vector<pair<int, int>> caminhoCritico(materias.size() , {0, 0});

    int indOrdem = 0;
    for(int i = 0; i < materias.size(); ++i){
        preReg[i] = materias[i].numReq;
    }
    for(int i = 0; i < materias.size(); ++i){
        if(preReg[i] == 0){
            ordem[indOrdem] = i;
            ++indOrdem;
        }
    }
    for(int i = 0; i < materias.size(); ++i){
        if(preReg[i] == 0){
            atuais.push(i);
        }
    }

    while (!atuais.empty()){
        auto a = atuais.top();
        atuais.pop();
        for(auto &p: grafo[a]){
            if(preReg[p] > 0){
                --preReg[p];
            }
            if(preReg[p] == 0){
                preReg[p] = -1;
                atuais.push(p);
                ordem[indOrdem] = p;
                ++indOrdem;
            }
        }
    }
    for(auto&p : ordem){
        cout << "-------------------------\n";
        cout << "nome\t" << materias[p].nome << '\n';
        cout << "codigo\t" << materias[p].codigo << '\n';
        cout << "creditos\t" << materias[p].creditos << '\n';
        cout << "Numero de Requisitos\t" << materias[p].numReq << '\n';
        cout << "-------------------------\n";
    }



    queue<int> fila;
    for(int i = 0; i < materias.size(); ++i){
        preReg[i] = materias[i].numReq;
    }
    for(int i = 0; i < materias.size(); ++i){
        if(preReg[i] == 0){
            fila.push(i);
            caminhoCritico[i] = {materias[i].creditos, -1};
        }
    }
    while (!fila.empty()){
        auto a = fila.front();
        fila.pop();
        for(auto &p: grafo[a]){
            if(preReg[p] > 0){
                --preReg[p];
            }
            if(preReg[p] == 0){
                preReg[p] = -1;
                fila.push(p);
                ++indOrdem;
            }
            if(caminhoCritico[a].first + materias[p].creditos > caminhoCritico[p].first){
                caminhoCritico[p].first = caminhoCritico[a].first + materias[p].creditos;
                caminhoCritico[p].second = a; 
                cerr <<  materias[a].nome << " | " << materias[p].nome << '\n';
            }
        }
    }
    pair<int, int> maximo = {-1, -1};
    vector<int> ordemCCritico;
    for(int i = 0; i < caminhoCritico.size(); ++i){
        auto &p = caminhoCritico[i];
        if(p.first > maximo.first){
            maximo.first = p.first;
            maximo.second = i;
        }
    }
    int proximo = maximo.second;
    while(proximo != -1){
        ordemCCritico.push_back(proximo);
        proximo = caminhoCritico[proximo].second;
    }

    reverse(ordemCCritico.begin(), ordemCCritico.end());
    cout << endl << endl;
    cout << "O tamanho do caminho critico eh " << ordemCCritico.size() << " com peso " << maximo.first << endl;
    for(auto&p : ordemCCritico){
        cout << "-------------------------\n";
        cout << "nome\t" << materias[p].nome << '\n';
        cout << "codigo\t" << materias[p].codigo << '\n';
        cout << "creditos\t" << materias[p].creditos << '\n';
        cout << "Numero de Requisitos\t" << materias[p].numReq << '\n';
        cout << "-------------------------\n";
    }



    return 0;
}

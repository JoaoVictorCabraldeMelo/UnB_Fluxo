/*
Este Código foi feito pelo alunos :
    Nome: Oscar E. B. Madureira da Silva
    Matrícula:17/0112209
    
    Nome: João Victor Cabral de Melo
    Matrícula: 16/0127670
    
    Estes alunos são estudantes da UnB Universidade de Brasília - Brasil
    Este código foi criado como trabalho da matéria de TAG - Teoria e Aplicação de Grafos
*/

#include <bits/stdc++.h>
// #include <gvc.h>
#include <graphviz/gvc.h>

using namespace std;


struct Materia{
    string nome;
    int creditos;
    int numReq;
    int codigo;
    int id;
};

class DotGenerator{
    private:
        Agraph_t *g;
        vector<Agnode_t *> n;
        vector<Agedge_t *> e;
        map<string, Agnode_t *> mapa;
    public:
        DotGenerator(int vertice, int aresta, const string &nome): n(vertice), e(aresta){
            char *aux = new char[nome.size()+1]; 
            strcpy(aux, nome.c_str());
            g = agopen(aux, Agdirected, 0);
            delete[] aux;
        }
        template<class T>
        void createNode(const vector<T> &vetor, function<string(const T&)> f){
            for(int i = 0; i < vetor.size(); ++i){
                string nome  = f(vetor[i]);
                char *aux = new char[nome.size()+1]; 
                strcpy(aux, nome.c_str());
                n[i] = agnode(g, aux, 1);
                mapa[nome] = n[i];
                delete[] aux;
            }
        }
        void createEdge(const vector<pair<int, int>> &vetor){
            for(int i = 0; i < vetor.size(); ++i){
                string nome1 = to_string(vetor[i].first);
                string nome2 = to_string(vetor[i].second);
                e[i] = agedge(g, mapa[nome2], mapa[nome1], 0, 1);
            }
        }
        void write(void *canal){
            agwrite(g, canal);
        }
        ~DotGenerator(){
            agclose(g);
        }

};

typedef vector<int> vertex; //Abstração Vertices do Grafo
typedef vector<vertex> Grafo; //Abstração o Grafo 

pair<  vector<Materia> , pair<  Grafo, vector<pair<int, int>>  >  > graphCreate(string nomeArquivo){
    
    pair<  vector<Materia> , pair<  Grafo, vector<pair<int, int>>  >  >  res;
    auto &materias = res.first;
    auto &arestas = res.second.second;
    auto &grafo = res.second.first;
    ifstream myfile(nomeArquivo);
    string line;
    int aux, val1, val2, val3, chave = 0;
    map<int, int> mapa1;

    if(myfile.is_open()){ //verifico se o arquivo esta aberto
        getline(myfile,line);
        aux = stoi(line);
        materias.resize(aux);
        grafo.resize(aux);
        for(auto&p : materias){
            getline(myfile, p.nome);
            getline(myfile, line);
            stringstream linha(line);
            linha >> val1 >> val2 >> val3;
            mapa1[val1] = chave;
            p.creditos = val3;
            p.codigo = val1;
            p.numReq = val2;
            p.id = chave;
            ++chave;
        }
        getline(myfile,line);
        aux = stoi(line);
        arestas.resize(aux);
        for(int i = 0; i < aux; ++i){
            getline(myfile,line);
            stringstream linha(line);
            linha >> val1 >> val2;
            arestas[i] = {val1, val2};
            grafo[mapa1[val2]].push_back(mapa1[val1]);
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
    auto &arestas = grafoTotal.second.second;
    auto &grafo = grafoTotal.second.first;
    DotGenerator *dot1;
    FILE *fp;
    
    dot1 = new DotGenerator(materias.size(), arestas.size(), "Grafo Completo");
    dot1->createNode(materias, function<string(const Materia&)>([](const Materia &materia) -> string{
        return to_string(materia.codigo);
    }));
    dot1->createEdge(arestas);
    fp = fopen("GrafoCompleto.dot", "w");
    dot1->write(fp);
    system("dot -Tpdf GrafoCompleto.dot -o GrafoCompleto.pdf");
    fclose(fp);
    delete dot1;
    
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
                //++indOrdem;
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
    vector<pair<int, int>> arestas2(ordemCCritico.size()-1);
    for(int i = 0; i < ordemCCritico.size() - 1; ++i){
        arestas2[i] = {materias[ordemCCritico[i]].codigo, 
          materias[ordemCCritico[i+1]].codigo};
    }
    dot1 = new DotGenerator(ordemCCritico.size(), ordemCCritico.size()-1, "Caminho Critco");
    dot1->createNode(ordemCCritico, function<string(const int&)>([&materias](const int &materia) -> string{
        return to_string(materias[materia].codigo);
    }));
    dot1->createEdge(arestas2);
    fp = fopen("CaminhoCritco.dot", "w");
    dot1->write(fp);
    system("dot -Tpdf CaminhoCritco.dot -o CaminhoCritco.pdf");
    fclose(fp);
    delete dot1;

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

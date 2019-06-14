#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <cstdio>
#include "stdc++.h"

#define PLA 130.0
#define ABS 85.0 

using namespace std;

class Impressao{
    protected:
        char objectName[100], cliente[100];
        float layer_height, infill, cost, filament_used;
        int filament_type, minutes, os; // 1 ABS. 2 PLA.
        char aux[10];
    public:
        void set_os(int ordem){ this->os = ordem; }
        void set_ClientName(char nome[]){ strcpy(this->cliente, nome); }
        void set_name(char nome[]){ strcpy(this->objectName, nome); }
        void set_layer_height(float height){ this->layer_height = height; }
        void set_infill(float fill){ this->infill = fill; }
        void set_filament_type(int type){ this->filament_type = type; }
        void set_filament_used(float used){ this->filament_used = used; }
        void set_time(int min){ this->minutes = min; }
        void set_cost(float cst){this->cost = cst; }

        int get_OS() const { return this->os; }
        char * get_client_name(){ return this->cliente;}
        char * get_name(){ return this->objectName; }
        float get_layer_height(){ return this->layer_height; }
        float get_infill(){ return this->infill; }
        float getCost(){            
            float Faux = 0;
            int Iaux = 0;
            if(filament_type == 1) this->cost = (1.4/60 * minutes) + (filament_used * ABS/1000);
            else if(filament_type == 2) this->cost = (1.4/60 * minutes) + (filament_used * PLA/1000);
            
            Iaux = int(this->cost);
            Faux = this->cost - (float)Iaux;           
            this->cost = float(Iaux);
            Faux = Faux * 100;
            Iaux = int(Faux);
            Faux = float(Iaux)/100;
            this->cost += Faux;

            return this->cost;
        }
        float get_filament_used(){ return this->filament_used; }
        int get_filament_type(){ return this->filament_type; }
        int get_minutes(){ return this->minutes; }
        char * getType(){
            if(filament_type == 1) strcpy(this->aux, "ABS");
            else if(filament_type == 2) strcpy(this->aux, "PLA"); 
            return this->aux;
        }
        void viewPrint(){
            cout << "Ordem de servico: " << this->os << endl;
            cout << "Nome do cliente: " << this->cliente << endl;
            cout << "Impressao: " << this->objectName << endl;
            cout << "Altura de camada: " << this->layer_height << "mm" << endl;
            cout << "Infill: " << this->infill << "%" << endl;
            cout << "Filamento: " << getType() << endl;
            cout << "Filamento usado: " << this->filament_used << " gramas" << endl;
            cout << "Tempo de impressao: " << this->minutes<< " minutos" << endl;
            cout << endl;
            cout << "Custo: R$" << this->getCost() << endl << endl;
        }
};

vector <Impressao> lista_impressao;
static int ordem_de_servico = 0;

fstream impressoes;
fstream recibo;

void textTitle(string complement = "");

void inserirOS(int option = 0);
void custoImpressoes();
void totalOS();

void load_file();
void save_file();
void gerar_recibo(float total = 0, int totalpecas = 0, int os = -1);

bool ordem_de_prioridade(const Impressao &a, const Impressao &b) {
    return ( a.get_OS() < b.get_OS() );
}

int main(){
    int op;
    load_file();
    do{
        system("clear");
        textTitle();
        sort(lista_impressao.begin(), lista_impressao.end(), ordem_de_prioridade);

        cout << "1. Criar ordem de servico." << endl;
        cout << "2. Inserir impressao a ordem de servico." << endl;
        cout << "3. Custo individual das impressoes." << endl;
        cout << "4. Custo total por OS." << endl;
        cout << endl;
        cout << "0. Sair." << endl << endl;

        cout << "opcao: ";
        cin >> op;
        cin.ignore();

        if(op != 0){
            switch(op){
                case 1:
                    inserirOS();
                    break;
                case 2:
                    inserirOS(1);
                    break;
                case 3:
                    system("clear");
                    if(lista_impressao.size() != 0) custoImpressoes();
                    else{
                        textTitle("Custo -");
                        cout << "lista zerada." << endl << endl;
                        system("read -p \"Pressione enter para continuar...\"");
                    }
                    break;
                case 4:
                    system("clear");
                    if(lista_impressao.size() != 0) totalOS();
                    break;
                default:
                    system("clear");
                    textTitle();
                    cout << "opcao invalida." << endl;
            }
        }
    }while(op != 0);
    
    save_file();

    return 0;
}
void textTitle(string complement){
    if(complement != "") cout << "\t"<< complement << " impressao 3D: " << endl << endl;  
    else cout << "\t Impressao 3D" << endl << endl;
}
void inserirOS(int option){
    char name[100], cliente[100];
    int fill_type, min, op, os_search;
    float lh, fill_used, infill; 
    bool check = false;

    Impressao aux;
    
    system("clear");
    cout << "\tOrdem de Servico: \n\n ";

    if(option == 0){    
        cout << "Digite o nome do cliente: ";
        cin.getline(cliente, 100);
        aux.set_ClientName(cliente);
    }
    else if(option == 1){
        do{
            system("clear");
            cout << "\tOrdem de Servico: \n\n ";

            cout << "digite o numero da OS: ";
            cin >> os_search;
            cin.ignore();

            for(int i = 0; i < lista_impressao.size(); i++){
                if(os_search == lista_impressao[i].get_OS()) check = true; 
                if(check) { 
                    aux.set_ClientName( lista_impressao[i].get_client_name() );
                    aux.set_os( lista_impressao[i].get_OS() );
                    break;
                }
            }
            if(!check){
                cout << "ordem de servico nao cadastrada.\n\nDigite novamente.";
                system("read -p \"Pressione enter para continuar...\"");
            } 
        }while(!check);
    }
    
    do{
        system("clear");
        textTitle("Inserir");

        cout << "digite o titulo da impressao: ";
        cin.getline(name, 100);
        
        system("clear");
        textTitle("Inserir");
        cout << "Tipo de filamento: " << endl << endl;
        cout << "1. ABS\n2. PLA\n\nopcao: ";
        cin >> fill_type;

        system("clear");
        textTitle("Inserir");
        cout << "quantidade de filamento (gramas): ";
        cin >> fill_used;

        system("clear");
        textTitle("Inserir");
        cout << "altura de camada: ";
        cin >> lh;

        system("clear");
        textTitle("Inserir");
        cout << "Infill (%): ";
        cin >> infill;

        system("clear");
        textTitle("Inserir");
        cout << "tempo de impressao (minutos): ";
        cin >> min;

        if(option == 0) aux.set_os(ordem_de_servico);
        
        aux.set_name(name);
        aux.set_layer_height(lh);
        aux.set_infill(infill);
        aux.set_filament_type(fill_type);
        aux.set_filament_used(fill_used);
        aux.set_time(min);
        lista_impressao.push_back(aux);

        system("clear");
        cout << "deseja inserir outra peca? \n\n1.SIM\n2.NAO \n\nopcao:";
        cin >> op;
        cin.ignore();
    }while(op == 1);
    
    if(option == 0) ordem_de_servico++;
}
void totalOS(){
    float total = 0;
    int os_search, totalq = 0, op;
    bool check = false;
    
    system("clear");
    
    textTitle("Busca de Ordem de Servico");
    cout << "Digite a OS: ";
    cin >> os_search;

    system("clear");

    textTitle("Calculo total de OS");

    for(int i = 0; i < lista_impressao.size(); i++){
        if(os_search == lista_impressao[i].get_OS()) {
            check = true;
            totalq++;
            total += lista_impressao[i].getCost();
            lista_impressao[i].viewPrint();
        }
    }
    if(check){
        if(totalq <=1) cout << "Total de pecas = " << totalq << " unidade" << endl;
        else cout << "Total de pecas = " << totalq << " unidades" << endl;
        cout << "Total = R$ "<< total << endl << endl;

        cout << "Deseja gerar um recibo?\n\n1.SIM\n2.NAO\n\nopcao: ";
        cin >> op;
        
        if(op == 1){
            gerar_recibo(total, totalq, os_search);
            system("open recibo.html");
            cout << "\nrecibo gerado na pasta raiz do programa como recibo.html" << endl << endl;
        }

    }
    else{
        cout << "Ordem de servico nao encontrada." << endl << endl;
    }

    system("read -p \"Pressione enter para continuar...\"");
}
void custoImpressoes(){
    system("clear");
    textTitle("Custo - ");
    for(int i = 0; i < lista_impressao.size(); i++){
        lista_impressao[i].viewPrint();
    }
    system("read -p \"Pressione enter para continuar...\"");
}
void load_file(){
	Impressao *auxp = new Impressao;
	string aux[9];
    int bigger = 0;
	char auxC[100];
	
	bool check = false;

	impressoes.open("impressao.csv", ios::in); // leitura;
	if(!impressoes.is_open()) cout << "arquivo impressoes.csv nao carregado" << endl;

	while(impressoes.good()){
		getline(impressoes, aux[0], ';');
		if(aux[0] != ""){
			getline(impressoes, aux[1], ';');
			getline(impressoes, aux[2], ';');
			getline(impressoes, aux[3], ';');
			getline(impressoes, aux[4], ';');
			getline(impressoes, aux[5], ';');
            getline(impressoes, aux[6], ';');
            getline(impressoes, aux[7], ';');
			getline(impressoes, aux[8], '\n');
            check = true;
		}
		
		if(check){
            auxp->set_os(atoi(aux[0].c_str()));
            strcpy(auxC, aux[1].c_str());
			auxp->set_ClientName(auxC);
            strcpy(auxC, aux[2].c_str());
			auxp->set_name(auxC);
			auxp->set_layer_height( atof(aux[3].c_str()));
			auxp->set_infill( atof(aux[4].c_str()));
			auxp->set_cost( atof(aux[5].c_str()));
			auxp->set_filament_used(atof(aux[6].c_str()));
            auxp->set_filament_type(atoi(aux[7].c_str()));
            auxp->set_time(atoi(aux[8].c_str()));

            if(auxp->get_OS() > bigger) bigger = auxp->get_OS();

			lista_impressao.push_back(*auxp);
		}
	}
    ordem_de_servico = bigger + 1;

	impressoes.close();
}
void save_file(){
	impressoes.open("impressao.csv", ios::out); // escrever no arquivo	
	
	for(int i = 0; i < lista_impressao.size(); i++){
        impressoes << lista_impressao[i].get_OS() << ";";
        impressoes << lista_impressao[i].get_client_name() << ";";    
		impressoes << lista_impressao[i].get_name() << ";";
		impressoes << lista_impressao[i].get_layer_height() << ";";
		impressoes << lista_impressao[i].get_infill() << ";";
		impressoes << lista_impressao[i].getCost() << ";";
		impressoes << lista_impressao[i].get_filament_used() << ";";
		impressoes << lista_impressao[i].get_filament_type() << ";";

		if(i < lista_impressao.size() - 1){
			impressoes << lista_impressao[i].get_minutes() << "\n";
		}
		else{
			impressoes << lista_impressao[i].get_minutes();
		}
	}	
	impressoes.close();
}
void gerar_recibo(float total,int totalpecas, int os){
    int indice;
    for(int i = 0; i < lista_impressao.size(); i++){
         if(os == lista_impressao[i].get_OS()){
            indice = i;
            break;
         } 
    }

    recibo.open("recibo.html", ios::out);

    recibo << "<!DOCTYPE html><html><head><title>Recibo</title></head><body><h2>Recibo</h2>";
    recibo << "<p>Ordem de Servico: " << lista_impressao[indice].get_OS() << "</p>";
    recibo << "<p>Cliente: " << lista_impressao[indice].get_client_name() << "</p>";
    
    recibo << "<table style=\"width:50%\">";
    
    for(int i = 0; i < lista_impressao.size(); i++){
        if(os == lista_impressao[i].get_OS()){
            recibo << "<tr><td>Titulo</td><td>"<< lista_impressao[i].get_name() << "</td></tr>";
            recibo << "<tr><td>Tempo de impressao</td><td>"<< lista_impressao[i].get_minutes() << " minutos</td></tr>";
            recibo << "<tr><td>Altura de camada</td><td>"<< lista_impressao[i].get_layer_height() << "mm</td></tr>";
            recibo << "<tr><td>Infill</td><td>"<< lista_impressao[i].get_infill() << "%</td></tr>";
            recibo << "<tr><td>Tipo de filamento</td><td>"<< lista_impressao[i].getType() << "</td></tr>";
            recibo << "<tr><td>Quantidade de filamento</td><td>"<< lista_impressao[i].get_filament_used() << "g</td></tr>";
            recibo << "<tr><td>Custo</td><td>R$"<< lista_impressao[i].getCost() << "</td></tr>";
            recibo << "<tr><td</td><tr>";
            recibo << "<tr><td</td><tr>";
            recibo << "<tr><td</td><tr>";
        }
    }
    recibo << "</table>";
    recibo << "<p>Quantidade de pecas: " << totalpecas << "</p>";
    recibo << "<p>Total: R$" << total << "</p>";
    recibo << "</body></html>";
    
    recibo.close();
}
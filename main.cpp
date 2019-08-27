#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <cstdio>
#include "stdc++.h"

//g++ -std=c++11 -o program main.cpp

#define PLA 130.0
#define ABS 85.0 

using namespace std;

class Impressao{
    protected:
        char objectName[100], cliente[100];
        float layer_height, infill, cost_time, cost_used, filament_used;
        int filament_type, minutes, os; // 1 ABS. 2 PLA.
        char aux[10];
    public:
        void set_os(int ordem){ os = ordem; }
        void set_ClientName(char nome[]){ strcpy(cliente, nome); }
        void set_name(char nome[]){ strcpy(objectName, nome); }
        void set_layer_height(float height){ layer_height = height; }
        void set_infill(float fill){ infill = fill; }
        void set_filament_type(int type){ filament_type = type; }
        void set_filament_used(float used){ filament_used = used; }
        void set_time(int min){ minutes = min; }
        void set_cost_time(float cst){cost_time = cst; }
        void set_cost_used(float cst){cost_used = cst; }

        int get_OS() const { return os; }
        char * get_client_name(){ return cliente;}
        char * get_name(){ return objectName; }
        float get_layer_height(){ return layer_height; }
        float get_infill(){ return infill; }
        float get_cost_used(){            
            float Faux = 0;
            int Iaux = 0;

            if(filament_type == 1) cost_used = filament_used * (ABS/1000);
            else if(filament_type == 2) cost_used = filament_used * (PLA/1000);
            
            Iaux = int(cost_used);
            Faux = cost_used - (float)Iaux;           
            cost_used = float(Iaux);
            Faux = Faux * 100;
            Iaux = int(Faux);
            Faux = float(Iaux)/100;
            cost_used += Faux;

            return cost_used;
        }
        float get_cost_time(){            
            float Faux = 0;
            int Iaux = 0;
            
            cost_time = (1.4/60) * minutes;
            
            Iaux = int(cost_time);
            Faux = cost_time - (float)Iaux;           
            cost_time = float(Iaux);
            Faux = Faux * 100;
            Iaux = int(Faux);
            Faux = float(Iaux)/100;
            cost_time += Faux;

            return cost_time;
        }
        float get_filament_used(){ return filament_used; }
        int get_filament_type(){ return filament_type; }
        int get_minutes(){ return minutes; }
        char * getType(){
            if(filament_type == 1) strcpy(aux, "ABS");
            else if(filament_type == 2) strcpy(aux, "PLA"); 
            return aux;
        }
        int getFillType(){
            return filament_type;
        }
        void viewPrint(){
            cout << "Ordem de servico: " << os << endl;
            cout << "Nome do cliente: " << cliente << endl;
            cout << "Impressao: " << objectName << endl;
            cout << "Altura de camada: " << layer_height << "mm" << endl;
            cout << "Infill: " << infill << "%" << endl;
            cout << "Filamento: " << getType() << endl;
            cout << "Filamento usado: " << filament_used << " gramas" << endl;
            cout << "Tempo de impressao: " << minutes<< " minutos" << endl;
            cout << endl;
            cout << "Custo por tempo: R$" << get_cost_time() << endl;
            cout << "Custo por filamento usado: R$" << get_cost_used() << endl;
            cout << "Custo total da peca: R$" << get_cost_time() + get_cost_used() << endl << endl; 
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
void totalUSO();

void load_file();
void save_file();
string gerar_recibo(float total = 0, int totalpecas = 0, int os = -1);

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
        cout << "5. Uso total" << endl;
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
                        system("read -p \"Enter para continuar\"");
                    }
                    break;
                case 4:
                    system("clear");
                    if(lista_impressao.size() != 0) totalOS();
                    break;
                case 5:
                    system("clear");
                    totalUSO();
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
                system("read -p \"Enter para continuar\"");
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
            total += lista_impressao[i].get_cost_time() + lista_impressao[i].get_cost_used();
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
            //windows string rec;
            string rec = "open ./";

            rec += gerar_recibo(total, totalq, os_search);
            system(rec.c_str());
            
            cout << "\nrecibo gerado na pasta raiz do programa" << endl << endl;
            system("read -p \"Enter para continuar\"");
        }

    }
    else{
        cout << "Ordem de servico nao encontrada." << endl << endl;
    }

    system("read -p \"Enter para continuar\"");
}
void custoImpressoes(){
    system("clear");
    textTitle("Custo - ");
    for(int i = 0; i < lista_impressao.size(); i++){
        lista_impressao[i].viewPrint();
    }
    system("read -p \"Enter para continuar\"");
}
void load_file(){
	Impressao *auxp = new Impressao;
	string aux[10];
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
            getline(impressoes, aux[8], ';');
			getline(impressoes, aux[9], '\n');
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
			auxp->set_cost_used( atof(aux[5].c_str()));
            auxp->set_cost_time( atof(aux[6].c_str()));
			auxp->set_filament_used(atof(aux[7].c_str()));
            auxp->set_filament_type(atoi(aux[8].c_str()));
            auxp->set_time(atoi(aux[9].c_str()));

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
		impressoes << lista_impressao[i].get_cost_used() << ";";
        impressoes << lista_impressao[i].get_cost_time() << ";";
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
string gerar_recibo(float total,int totalpecas, int os){
    int indice;
    int totalTempo = 0;
    for(int i = 0; i < lista_impressao.size(); i++){
         if(os == lista_impressao[i].get_OS()){
            indice = i;
            break;
         } 
    }
    string rec = "recibo", format = ".html", client, OS, space;
    //string rec = "recibo", format = ".html", client, OS, space; Windows

    client = lista_impressao[indice].get_client_name();
    OS = to_string(lista_impressao[indice].get_OS());

    rec = rec + client + OS + format;
    
    recibo.open(rec.c_str(), ios::out);

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
            recibo << "<tr><td>Custo por filamento</td><td>R$"<< lista_impressao[i].get_cost_used() << "</td></tr>";
            recibo << "<tr><td>Custo por tempo</td><td>R$"<< lista_impressao[i].get_cost_time() << "</td></tr>";
            recibo << "<tr><td</td><tr>";
            recibo << "<tr><td</td><tr>";
            recibo << "<tr><td>Custo da peca</td><td>R$"<< lista_impressao[i].get_cost_time() + lista_impressao[i].get_cost_used() << "</td></tr>";
            recibo << "<tr><td</td><tr>";
            recibo << "<tr><td</td><tr>";
            recibo << "<tr><td</td><tr>";
            totalTempo += lista_impressao[i].get_minutes();
        }
    }
    recibo << "</table>";
    recibo << "<p>Quantidade de pecas: " << totalpecas << "</p>";
    recibo << "<p>Total: R$" << total << "</p>";
    recibo << "<p>Tempo total de impressão: " << totalTempo << " minutos</p>";
    recibo << "</body></html>";
    
    recibo.close();
    rec = '\"' + rec + '\"'; 
    return rec;
}

void totalUSO(){
    int tempoTotal = 0, pecas =0;
    float custoTotalMaterial = 0, custoTotalTempo = 0, totalFilamento = 0;
    float mediaMat, mediaCustoMat, mediaCustoTempo, mediaTempo, mediapecas;

    for (int i = 0; i < lista_impressao.size(); i++){
        if(lista_impressao[i].getFillType() == 1){ // ABS
            tempoTotal = tempoTotal + lista_impressao[i].get_minutes();
            custoTotalMaterial +=  lista_impressao[i].get_cost_used();
            custoTotalTempo +=  lista_impressao[i].get_cost_time();
            totalFilamento += lista_impressao[i].get_filament_used();
            pecas++;
        }            
    }

    system("clear");
    
    cout << "ABS: " << endl << endl;
    cout << "Quantidade de pecas impressas: " << pecas << endl;
    cout << "Tempo total: " << tempoTotal <<" minutos" << endl;
    cout << "Material usado: " << totalFilamento << "g"<<endl;
    cout << "Custo por tempo total: R$" << custoTotalTempo << endl;
    cout << "Custo por material total: R$" << custoTotalMaterial << endl;
    
    mediaMat = totalFilamento;
    mediaCustoMat = custoTotalMaterial;
    mediaCustoTempo = custoTotalTempo;
    mediapecas = float(pecas);
    mediaTempo = float(tempoTotal);

    tempoTotal = 0;
    custoTotalMaterial = 0;
    custoTotalTempo = 0;
    pecas = 0;
    totalFilamento = 0;

    for (int i = 0; i < lista_impressao.size(); i++){
        if(lista_impressao[i].getFillType() == 2){ // PLA
            tempoTotal += lista_impressao[i].get_minutes();
            custoTotalMaterial +=  lista_impressao[i].get_cost_used();
            custoTotalTempo +=  lista_impressao[i].get_cost_time();
            totalFilamento += lista_impressao[i].get_filament_used();
            pecas++;
        }                   
    }

    cout << "\nPLA: " << endl << endl;
    cout << "Quantidade de pecas impressas: " << pecas << endl;
    cout << "Tempo total: " << tempoTotal <<" minutos" << endl;
    cout << "Material usado: " << totalFilamento << "g"<<endl;
    cout << "Custo por tempo total: R$" << custoTotalTempo << endl;
    cout << "Custo por material total: R$" << custoTotalMaterial << endl << endl;

    cout << "Quantidade total de pecas impressas: " << pecas + mediapecas << endl;
    cout << "Media de custo de material por impressao  (independente de material): R$" << (mediaCustoMat + custoTotalMaterial)/(pecas + mediapecas) << endl;
    cout << "Media de custo de tempo por impressao  (independente de material): R$" << (mediaCustoTempo + custoTotalTempo)/(pecas + mediapecas) << endl;
    cout << "quantidade media de filamento usado por impressao: " << (totalFilamento + mediaMat)/(pecas + mediapecas) <<"g" <<  endl;
    cout << "tempo medio por impressao: " << (float(tempoTotal) + mediaTempo) / (pecas + mediapecas) << " minutos"<< endl << endl;
    

    system("read -p \"Enter para continuar\"");
}
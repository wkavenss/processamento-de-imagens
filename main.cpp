#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#define STRMAX 100
#define MAX 512

using namespace std;

struct Img{
    int numero_linhas;
    int numero_colunas;
    int mat_r[MAX][MAX];
    int mat_g[MAX][MAX];
    int mat_b[MAX][MAX];
};

void abre_img(char nome[], Img& img);
void salva_img(char nome[], Img img);
void corta_rgb(Img img_in, Img& img_out, int li, int ci, int lf, int cf);
void converte_para_cinza(Img img_in, Img& img_out);
void computa_negativo(Img img_in, Img& img_out);
void borra_imagem(Img img_in, Img& img_out);
void aumenta_tamanho(Img img_in, Img& img_out);
void inverte(Img img_in, Img& img_out);

int main(){
    int numero_processamento,corte,li,ci,lf,cf;
    char nome[STRMAX];
    Img img_in,img_out;
    cout<<"_____________MENU DO PROGRAMA_____________"<<endl;
    cout<<"Informe o nome do arquivo a ser executado"<<endl;
    cin.getline(nome,STRMAX);
    abre_img(nome,img_in);
    cout<<endl;
    cout<<"Deseja cortar a imagem? Digite 1 para sim ou 0 para nao"<<endl;
    cin>>corte;
    if(corte==1){
        cout<<"Informe a linha inicial, coluna inicial, linha final e coluna final"<<endl;
        cout<<"Linha inicial: ";
        cin>>li;
        cout<<"Coluna inicial: ";
        cin>>ci;
        cout<<"Linha final: ";
        cin>>lf;
        cout<<"Coluna final: ";
        cin>>cf;
        corta_rgb(img_in,img_out,li,ci,lf,cf);
        img_in=img_out;
    }
    while(1){
        cout<<"Informe o numero do processamento a ser aplicado na imagem:"<<endl;
        cout<<"1: Converter para niveis de cinza"<<endl;
        cout<<"2: Computar negativo"<<endl;
        cout<<"3: Filtrar com passa baixas (tamanho fixo)"<<endl;
        cout<<"4: Aumentar tamanho"<<endl;
        cout<<"5: Inverter horizontalmente"<<endl;
        cout<<"6: Finalizar"<<endl;
        cin>>numero_processamento;
        while(numero_processamento>6 || numero_processamento<1){
            cout<<"O numero deve ser entre 1 e 6. Informe corretamente:"<<endl;
            cin>>numero_processamento;
        }
        cout<<endl;
        if(numero_processamento==1){
            converte_para_cinza(img_in,img_out);
        }
        if(numero_processamento==2){
            computa_negativo(img_in,img_out);
        }
        if(numero_processamento==3){
            borra_imagem(img_in,img_out);
        }
        if(numero_processamento==4){
            aumenta_tamanho(img_in,img_out);
        }
        if(numero_processamento==5){
            inverte(img_in,img_out);
        }
        salva_img(nome,img_out);
        if(numero_processamento==6){
            break;
        }
        img_in=img_out;
    }
    return 0;
}

void abre_img(char nome[], Img& img){
    int i,j,vmax_pixel;
    char tipo[STRMAX];
    fstream arq;
    arq.open(nome);
    if(!arq.is_open()){
        cout<<"Erro. Arquivo nao encontrado!"<<endl;
        exit(0);
    }
    arq>>tipo;
    if(strcmp(tipo,"P3")!=0){
        cout<<"Arquivo nao eh P3"<<endl;
        exit(0);
    }
    arq>>img.numero_colunas>>img.numero_linhas;
    arq>>vmax_pixel;
    for(i=0;i<img.numero_linhas;i++){
        for(j=0;j<img.numero_colunas;j++){
            arq>>img.mat_r[i][j];
            arq>>img.mat_g[i][j];
            arq>>img.mat_b[i][j];
        }
    }
    arq.close();
}

void salva_img(char nome[], Img img){
    int i,j;
    char novo_nome[STRMAX]= {"Novo-"};
    strcat(novo_nome,nome);
    ofstream arq;
    arq.open(novo_nome);
    if(!arq.is_open()){
        cout<<"Erro. Arquivo nao criado!"<<endl;
        exit(0);
    }
    arq<<"P3"<<endl;
    arq<<img.numero_colunas<<" "<<img.numero_linhas<<endl;
    arq<<255<<endl;
    for(i=0;i<img.numero_linhas;i++){
        for(j=0;j<img.numero_colunas;j++){
            arq<<img.mat_r[i][j]<<" ";
            arq<<img.mat_g[i][j]<<" ";
            arq<<img.mat_b[i][j]<<" ";
        }
        arq<<endl;
    }
    arq.close();
}

void corta_rgb(Img img_in, Img& img_out, int li, int ci, int lf, int cf){
    int i,j;
    for(i=li;i<lf;i++){
        for(j=ci;j<cf;j++){
            img_out.mat_r[i-li][j-ci]=img_in.mat_r[i][j];
            img_out.mat_g[i-li][j-ci]=img_in.mat_g[i][j];
            img_out.mat_b[i-li][j-ci]=img_in.mat_b[i][j];
        }
    }
    img_out.numero_linhas=lf-li;
    img_out.numero_colunas=cf-ci;
}

void converte_para_cinza(Img img_in, Img& img_out){
    int i,j;
    for(i=0;i<img_in.numero_linhas;i++){
        for(j=0;j<img_in.numero_colunas;j++){
            img_out.mat_r[i][j]=(img_in.mat_r[i][j] + img_in.mat_g[i][j] + img_in.mat_b[i][j])/3;
            img_out.mat_g[i][j]=(img_in.mat_r[i][j] + img_in.mat_g[i][j] + img_in.mat_b[i][j])/3;
            img_out.mat_b[i][j]=(img_in.mat_r[i][j] + img_in.mat_g[i][j] + img_in.mat_b[i][j])/3;
        }
    }
    img_out.numero_linhas=img_in.numero_linhas;
    img_out.numero_colunas=img_in.numero_colunas;
}

void computa_negativo(Img img_in, Img& img_out){
    int i,j;
    for(i=0;i<img_in.numero_linhas;i++){
        for(j=0;j<img_in.numero_colunas;j++){
            img_out.mat_r[i][j]=(255-img_in.mat_r[i][j]);
            img_out.mat_g[i][j]=(255-img_in.mat_g[i][j]);
            img_out.mat_b[i][j]=(255-img_in.mat_b[i][j]);
        }
    }
    img_out.numero_linhas=img_in.numero_linhas;
    img_out.numero_colunas=img_in.numero_colunas;
}

void borra_imagem(Img img_in, Img& img_out){
    float filtro[3][3]= {{1.0/9,1.0/9,1.0/9},
                        {1.0/9,1.0/9,1.0/9},
                        {1.0/9,1.0/9,1.0/9}};

    int i,j,p,q,u,v;
    img_out.numero_linhas=img_in.numero_linhas;
    img_out.numero_colunas=img_in.numero_colunas;
    for(i=0;i<img_in.numero_linhas;i++){
        for(j=0;j<img_in.numero_colunas;j++){
            img_out.mat_r[i][j]=0;
            img_out.mat_g[i][j]=0;
            img_out.mat_b[i][j]=0;
            for(p=i-1,u=0;p<=i+1;p++,u++){
                for(q=j-1,v=0;q<=j+1;q++,v++){
                    if(p>=0 && q>=0 && p<img_out.numero_linhas && q<img_out.numero_colunas){
                        img_out.mat_r[i][j]+=filtro[u][v]*img_in.mat_r[p][q];
                        img_out.mat_g[i][j]+=filtro[u][v]*img_in.mat_g[p][q];
                        img_out.mat_b[i][j]+=filtro[u][v]*img_in.mat_b[p][q];
                    }else{
                        img_out.mat_r[i][j]+=filtro[u][v]*0;
                        img_out.mat_g[i][j]+=filtro[u][v]*0;
                        img_out.mat_b[i][j]+=filtro[u][v]*0;
                    }
                }
            }
        }
    }
}

void aumenta_tamanho(Img img_in, Img& img_out){
    int i,j;
    img_out.numero_linhas=img_in.numero_linhas*2 -1;
    img_out.numero_colunas=img_in.numero_colunas*2 -1;
    //passo 0
    for(i=0;i<img_out.numero_linhas;i+=2){
        for(j=0;j<img_out.numero_colunas;j+=2){
            img_out.mat_r[i][j]=img_in.mat_r[i/2][j/2];
            img_out.mat_g[i][j]=img_in.mat_g[i/2][j/2];
            img_out.mat_b[i][j]=img_in.mat_b[i/2][j/2];
        }
    }
    //etapa 1
    for(j=1;j<img_out.numero_colunas;j+=2){
        i=0;
        img_out.mat_r[i][j]=(img_out.mat_r[i][j-1]+img_out.mat_r[i][j+1])/2;
        img_out.mat_g[i][j]=(img_out.mat_g[i][j-1]+img_out.mat_g[i][j+1])/2;
        img_out.mat_b[i][j]=(img_out.mat_b[i][j-1]+img_out.mat_b[i][j+1])/2;
        i=img_out.numero_linhas-1;
        img_out.mat_r[i][j]=(img_out.mat_r[i][j-1]+img_out.mat_r[i][j+1])/2;
        img_out.mat_g[i][j]=(img_out.mat_g[i][j-1]+img_out.mat_g[i][j+1])/2;
        img_out.mat_b[i][j]=(img_out.mat_b[i][j-1]+img_out.mat_b[i][j+1])/2;
    }
    for(i=1;i<img_out.numero_linhas;i+=2){
        j=0;
        img_out.mat_r[i][j]=(img_out.mat_r[i-1][j]+img_out.mat_r[i+1][j])/2;
        img_out.mat_g[i][j]=(img_out.mat_g[i-1][j]+img_out.mat_g[i+1][j])/2;
        img_out.mat_b[i][j]=(img_out.mat_b[i-1][j]+img_out.mat_b[i+1][j])/2;
        j=img_out.numero_colunas-1;
        img_out.mat_r[i][j]=(img_out.mat_r[i-1][j]+img_out.mat_r[i+1][j])/2;
        img_out.mat_g[i][j]=(img_out.mat_g[i-1][j]+img_out.mat_g[i+1][j])/2;
        img_out.mat_b[i][j]=(img_out.mat_b[i-1][j]+img_out.mat_b[i+1][j])/2;
    }
    //etapa 2
    for(i=1;i<img_out.numero_linhas-1;i+=2){
        for(j=1;j<img_out.numero_colunas-1;j+=2){
            img_out.mat_r[i][j]=(img_out.mat_r[i-1][j-1]+img_out.mat_r[i-1][j+1]+img_out.mat_r[i+1][j-1]+img_out.mat_r[i+1][j+1])/4;
            img_out.mat_g[i][j]=(img_out.mat_g[i-1][j-1]+img_out.mat_g[i-1][j+1]+img_out.mat_g[i+1][j-1]+img_out.mat_g[i+1][j+1])/4;
            img_out.mat_b[i][j]=(img_out.mat_b[i-1][j-1]+img_out.mat_b[i-1][j+1]+img_out.mat_b[i+1][j-1]+img_out.mat_b[i+1][j+1])/4;
        }
    }
    //etapa 3
    for(i=2;i<img_out.numero_linhas-1;i+=2){
        for(j=1;j<img_out.numero_colunas-1;j+=2){
            img_out.mat_r[i][j]=(img_out.mat_r[i][j-1]+img_out.mat_r[i][j+1]+img_out.mat_r[i+1][j]+img_out.mat_r[i-1][j])/4;
            img_out.mat_g[i][j]=(img_out.mat_g[i][j-1]+img_out.mat_g[i][j+1]+img_out.mat_g[i+1][j]+img_out.mat_g[i-1][j])/4;
            img_out.mat_b[i][j]=(img_out.mat_b[i][j-1]+img_out.mat_b[i][j+1]+img_out.mat_b[i+1][j]+img_out.mat_b[i-1][j])/4;
        }
    }
    for(i=1;i<img_out.numero_linhas-1;i+=2){
        for(j=2;j<img_out.numero_colunas-1;j+=2){
            img_out.mat_r[i][j]=(img_out.mat_r[i][j-1]+img_out.mat_r[i][j+1]+img_out.mat_r[i+1][j]+img_out.mat_r[i-1][j])/4;
            img_out.mat_g[i][j]=(img_out.mat_g[i][j-1]+img_out.mat_g[i][j+1]+img_out.mat_g[i+1][j]+img_out.mat_g[i-1][j])/4;
            img_out.mat_b[i][j]=(img_out.mat_b[i][j-1]+img_out.mat_b[i][j+1]+img_out.mat_b[i+1][j]+img_out.mat_b[i-1][j])/4;
        }
    }
}

///EXTRA///

void inverte(Img img_in, Img& img_out){
    int i,j,k;
    for(i=0;i<img_in.numero_linhas;i++){
        for(j=0,k=img_in.numero_colunas;j<img_in.numero_colunas;j++,k--){
            img_out.mat_r[i][j]=img_in.mat_r[i][k];
            img_out.mat_g[i][j]=img_in.mat_g[i][k];
            img_out.mat_b[i][j]=img_in.mat_b[i][k];
        }
    }
    img_out.numero_linhas=img_in.numero_linhas;
    img_out.numero_colunas=img_in.numero_colunas;
}
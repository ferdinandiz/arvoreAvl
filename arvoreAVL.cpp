#include<iostream>
#include<cstdlib>
#include<iomanip>
#include<string>

#define NUMERO_MAX_DE_NOS 255

using namespace std;

class No{

private:
    int valor;
    No* esq;
    No* dir;
    int altura;

public:
    No(int v){
        this->valor = v;
        this->esq = NULL;
        this->dir = NULL;
        this->altura = 0;
    }
    // Getters
    int getValor(){
        return this->valor;
    }
    No* getDir(){
        return this->dir;
    }
    No* getEsq(){
        return this->esq;
    }
    int getAltura(){
        return this->altura;
    }
    //Setters
    void setValor(int v){
        this->valor = v;
    }
    void setDir(No* d){
        this->dir = d;
        atualizaAltura();
    }
    void setEsq(No* e){
        this->esq = e;
        atualizaAltura();
    }

    //Funções Árvore
    int fatorBalanceamento(){
        int alturaDir = this->dir ? this->dir->altura : -1;
        int alturaEsq = this->esq ? this->esq->altura : -1;
        return alturaDir - alturaEsq;
    }
    void atualizaAltura(){
        int alturaDir = this->dir ? this->dir->altura:-1;
        int alturaEsq = this->esq ? this->esq->altura:-1;
        this->altura = 1 + max(alturaDir,alturaEsq);
    }
};

class Arvore {

private:
    No* raiz;

    No* inserir(No* no, int v){
        if(no == nullptr){
            return new No(v);
        }
        if(v < no->getValor()){
            no->setEsq(inserir(no->getEsq(),v));
        } else if(v > no->getValor()) {
            no->setDir(inserir(no->getDir(),v));
        }
        else{
            cout<<"\nValor Repetido!!\n";
            return no;
        }
        return balancear(no);
    }

    pair<No*, bool> remover(No* no, int v){
        if (!no) {
            return {nullptr, false};
        }

        bool removido = false;

        if (v < no->getValor()) {
            auto resultado = remover(no->getEsq(), v);
            no->setEsq(resultado.first);
            removido = resultado.second;
        } else if (v > no->getValor()) {
            auto resultado = remover(no->getDir(), v);
            no->setDir(resultado.first);
            removido = resultado.second;
        } else {
            removido = true;
            if (!no->getEsq()) {
                No* aux = no->getDir();
                delete no;
                return {aux, true};
            }
            if (!no->getDir()) {
                No* aux = no->getEsq();
                delete no;
                return {aux, true};
            }

            No* sucessor = obterSucessor(no->getDir());
            no->setValor(sucessor->getValor());
            auto resultado = remover(no->getDir(), sucessor->getValor());
            no->setDir(resultado.first);
        }

        return {balancear(no), removido};
    }

    No* balancear (No* no){
        if(!no) return nullptr;
        no->atualizaAltura();
        int fb = no->fatorBalanceamento();
        if(fb > 1){
            if(no->getDir()->fatorBalanceamento() < 0) {
                no->setDir(rotacaoDireita(no->getDir()));
            }
            return rotacaoEsquerda(no);
        }
        if(fb < -1){
            if(no->getEsq()->fatorBalanceamento() > 0) {
                no->setEsq(rotacaoEsquerda(no->getEsq()));
            }
            return rotacaoDireita(no);
        }
        return no;
    }

    No* rotacaoEsquerda(No* no){
        No* novaRaiz = no->getDir();
        no->setDir (novaRaiz->getEsq());
        novaRaiz->setEsq(no);
        no->atualizaAltura();
        novaRaiz->atualizaAltura();
        return novaRaiz;
    }

    No* rotacaoDireita(No* no){
        No* novaRaiz = no->getEsq();
        no->setEsq(novaRaiz->getDir());
        novaRaiz->setDir(no);

        no->atualizaAltura();
        novaRaiz->atualizaAltura();
        return novaRaiz;
    }


    No* obterSucessor(No* no){
        while(no->getEsq()){
            no = no->getEsq();
        }
        return no;
    }

    void limpar (No* no){
        if(!no) return;
        limpar(no->getDir());
        limpar(no->getEsq());
        delete no;
    }

    void imprimirSimetrico(No* no){
        if(!no) return;
        cout<<"(";
        imprimirSimetrico(no->getEsq());
        cout<< no->getValor();
        imprimirSimetrico(no->getDir());
        cout<<")";
    }

    void imprimirPreFixado(No* no){
        if(!no) return;
        cout<<"(";
        cout<< no->getValor();
        imprimirPreFixado(no->getEsq());
        imprimirPreFixado(no->getDir());
        cout<<")";
    }

    void imprimirPosFixado(No* no){
        if(!no) return;
        cout<<"(";
        imprimirPosFixado(no->getEsq());
        imprimirPosFixado(no->getDir());
        cout<< no->getValor();
        cout<<")";
    }

    void imprimirHorizontal(No* arv, bool *mapa, int nivel=0, bool esq = true){
        if(arv !=nullptr) {
            mapa[nivel] = true;
            if(nivel){
                mapa[nivel-1] = (esq)? false:true;
            }

            imprimirHorizontal(arv->getEsq(), mapa, nivel+1, true);

            cout<<left;
            for(int i=0;i<nivel;i++){
                cout.width(4);
                if(i<nivel-1){
                    cout<<(mapa[i]?"|":"");
                }else {
                    cout.fill('-');
                    cout<<"+";
                    cout.fill(' ');
                }
            }
            cout<<"("<<arv->getValor()<<")"<<endl;
            if(nivel){
                mapa[nivel-1] = (!esq)? false:true;
            }
            imprimirHorizontal(arv->getDir(), mapa, nivel+1, false);
        }
    }

    int contarNos(No* no){
        if(!no) return 0;
        return 1+contarNos(no->getEsq())+contarNos(no->getDir());
    }

    int altura(No* no){
        return no ? no->getAltura():-1;
    }


public:
    Arvore() : raiz(nullptr){}

    ~Arvore() {
        limpar(raiz);
    }

    void inserir(int v) {
        raiz = inserir(raiz, v);
    }

    bool remover(int v){
        auto resultado = remover(raiz, v);
        raiz = resultado.first;
        return resultado.second;
    }

    void imprimirCentral(){
        imprimirSimetrico(raiz);
        cout<< endl;
    }

    void imprimirPre(){
        imprimirPreFixado(raiz);
        cout<< endl;
    }

    void imprimirPos(){
        imprimirPosFixado(raiz);
        cout<< endl;
    }
    void imprimir(){
        bool mapa[NUMERO_MAX_DE_NOS]={};
        imprimirHorizontal(raiz,mapa);
    }

    int contarNosArvore(){
        return contarNos(raiz);
    }

    int altura(){
        return altura(raiz);
    }
    bool vazia(){
        return raiz == nullptr;
    }

    void limpar(){
        limpar(raiz);
        raiz = nullptr;
    }
};

int main(){
    Arvore arv;
    int menu = -1;
    int submenu = -1;
    int v;
    do{
        system("cls");
        cout<<"0 - SAIR"<<endl;
        cout<<"1 - Inserir"<<endl;
        cout<<"2 - Imprimir"<<endl;
        cout<<"3 - Remover no"<<endl;
        cout<<"4 - Podar Arvore"<<endl;
        cout<<"5 - Contar nos"<<endl;
        cout<<"6 - Altura da Arvore"<<endl;
        cout<<endl<<">> ";
        cin>>menu;

        switch(menu){
        case 0:
            cout<<"Programa Finalizado"<<endl;
            break;

        case 1:
            cout<<"Informe o valor a ser inserido: ";
            cin>>v;
            arv.inserir(v);
            break;

        case 2:
            if(arv.vazia()){
                cout<<"Arvore Vazia!!"<<endl;
                system("pause");
            }else{
                system("cls");
                cout<<"Digite a opcao de impressao:"<<endl;
                cout<<"1 - Imprimir Horizontal"<<endl;
                cout<<"2 - Imprimir Pre Fixado"<<endl;
                cout<<"3 - Imprimir Pos Fixado"<<endl;
                cout<<"4 - Imprimir Simetrica"<<endl;
                cout<<">> ";
                cin>>submenu;
                switch(submenu){

                case 1:
                    arv.imprimir();
                    break;
                case 2:
                    arv.imprimirPre();
                    break;
                case 3:
                    arv.imprimirPos();
                    break;
                case 4:
                    arv.imprimirCentral();
                    break;
                default:
                    cout<<"Opcao invalida para imprimir..."<<endl;
                }
                system("pause");
            }
            break;

        case 3:
            if(!arv.vazia()){
                cout<<"Infome o valor a ser cortado: ";
                cin>>v;

                if(arv.remover(v))
                    cout<<"No \""<<v<< "\" removido"<<endl;
                else
                    cout<<"No inexistente! "<<endl;
            }else{
                cout<<"Arvore vazia."<<endl;
            }
            system("pause");
            break;

        case 4:
            arv.limpar();
            cout<<"Arvore limpa com sucesso."<<endl;
            system ("pause");
            break;

        case 5:
            cout<<"A arvore possui "<<arv.contarNosArvore()<<" no(s)."<<endl;
            system("pause");
            break;
        case 6:
            cout<<"A arvore possui altura "<<arv.altura()<<"."<<endl;
            system("pause");
            break;
        default:
            cout<<"Opcao invalida!"<<endl;
            system("pause");
            break;
        }

    }while(menu != 0);

}

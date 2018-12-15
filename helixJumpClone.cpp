/*
  Author:      Gabriel Dias de Abreu e Leticia Florentino Pires
  Date:        25/10/2018
  Description: codigo para o jogo da disciplina de computação gráfica
*/

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <list>
#include <iterator>
#define PI 3.1415927
#define NUM 120
#define PONTO_INICIAL 10-sqrt(45/2)
#define DISTANCIA_INTERPLATAFORMA 60
//limite ate onde o jogo pode ir a direita
#define LIMITE_LATERAL_DIR 200
//limite ate onde o jogo pode ir a esquerda
#define LIMITE_LATERAL_ESQ -200
//numero de niveis
#define NUM_NIVEIS 40
using namespace std;
//instnaciacao da camera
////Camera g_camera;
//coisas do cilindro
float ANGLE = 360/ NUM;
float angle = 0;
//float raio = 50;
float dir = 1;
float xAcc=0, yAcc =0, zAcc=0;
//vector<int> teste{90, 1, 5, 0, 30, 2, 5, 0, 55, 3, 5, 0, 165, 4};
bool normal = false;
int numTriang = 1;
float current = 0;
bool ehEixo=true;
typedef struct // Estrutura para armazenar vertices
{
    double x,y,z;
} vertice;

bool toCaindo = true;
//variaveis usadas para controle do mouse
//bool 	inverseMouse = false;
bool	releaseMouse = false;


float pos;
//variavel para pausar o jogo
bool pause = true;

float rotationX = 25.0, rotationY= 0.0,scale= 5.0;
float anguloBolinha = 0;
//nivel atual
int nivel = NUM_NIVEIS-1;// quantas plataformas foram ultrapassadas pelo jogador

//translacaoEsfera => se refere a movimentacao vertical da esfera
//aceleracaoEsfera => se refere a velocidade com que a bolinha quica; se essa variavel for mudada, a variavel "limiar" também ter que o ser.
float translacaoEsfera = PONTO_INICIAL, aceleracaoEsfera = 0.15;
//desloDir se refere a translacao para lado esquerdo/direito
//aceleracaoPlano se refere a  velocidade de movimentacao do plano
//tamanhoPlano se refere ao tamanho que sera escalado para formar o plano
float desloDir=0, desloCima=0, aceleracaoPlano = 0.5, tamanhoPlano = 20;
//deformacaoEsfera e a variavel que controla a escala na esfera para dar a sensacao de achatamento quando toca os planos

// transposicao , translada a parabola no eixo y
float transposicao = (NUM_NIVEIS-1)*(DISTANCIA_INTERPLATAFORMA);//mexer jaja
int   last_x, last_y;
//int width = 600, height = 800;
//int width = 250, height = 250;
int width  = 800;
int height = 600;

//flag de fullscreen
bool fullScreen=true;

int transNivel=1;
//para aproximar e desaproximar a camera, mas nao esta sendo usado
int distOrigem = 100; // 45
//animacao da camera descendo
float descida=DISTANCIA_INTERPLATAFORMA*NUM_NIVEIS;
//move a bolinha e a tela no X
float moveTelaX = 0; //controla o inicio da tela




//cima/direita  desloCima vai sendo acrescentado ate chegar no Cima,
//o mesmo ocorre com direita: vai deslocando desloDir até chegar em direita, para que se tenha a sensacao de deslizamento
int cima=0;
int direita =0;
float limiteInferior = -0;

//vetor com os graus dos limites dos planos pretos ,a parte estatica ja ta preenchida

vector< vector<int*> > limPlanosPretos = vector< vector<int*> >(NUM_NIVEIS);
//mesmo acima mas para partes vermelhas
vector< vector<int*> > limPlanosVermelhos = vector< vector<int*> >(NUM_NIVEIS);
int velocidadeVermelha[NUM_NIVEIS] = {1, 3, 1, -2, -1, -3, 2, -1, 2, -1, -1, -2, 1, 1, 3, -1, 3, 2, 1, -1, 1, 3, 1, -2, -1, -3, 2, -1, 2, -1, -1, -2, 1, 1, 3, -1, 3, 2, 1, 0};

//10 padroes de plataforma diferentes
int numPadroesPlataforma = 10;

//os 10 padroes pedidos, baseados no jogo helix jump
//2 eh material preto e 1 eh vermelho
vector<int> padroes[] = {
    vector<int>{0, 0, 40, 1, 40, 2 , 40, 1, 40, 0, 40, 1, 40, 2, 40, 1  },//0
    vector<int>{0, 0, 40, 2, 40, 1 , 40, 2, 40, 0, 40, 2, 40, 1, 40, 2  },//1
    vector<int>{0, 0, 290, 2},//2
    vector<int>{0, 0, 50, 2, 50, 1 , 70, 2, 20, 0, 50, 1, 70, 2 },//3
    vector<int>{0, 0, 50, 2, 50, 1 , 70, 2, 50, 1, 20, 0, 70, 2 },//4
    vector<int>{0, 0, 70, 2, 50, 1 , 70, 2, 50, 0, 70, 2 },//5
    vector<int>{0, 0, 70, 2, 50, 0 , 70, 2, 50, 0, 70, 2 },//6

    vector<int>{0, 0, 130, 2, 50, 0, 130, 2 },//7
    vector<int>{0, 0, 40, 2, 40, 0, 30, 1, 30, 0,  40, 2, 40, 0, 30, 1, 30, 0, 20, 2},//8
    vector<int>{0, 0, 95, 2, 40, 0, 30, 1, 50, 0,  95, 2 },//9

};
//vetor que guarda o efeito de quando se passa de nivel
vector<int> destroi{30, 2, 30, 7,30, 2, 30, 7,30, 2, 30, 7,30, 2, 30, 7,30, 2, 30, 7,30, 2, 30, 7, };
//auxiliar do efeito de quando se passa de nivel
vector<int> raiosDeVetores;

//vetor que tem os desenhos a serem exibidos
vector< vector<int> > cenario;




//void criaNivel(int x1, int x2, int y,int transNivel, int indice);

//material vermelho
void setMaterial1(void)
{

    GLfloat objeto_ambient[]   = { .17, .01, .01, 0.55};
    GLfloat objeto_difusa[]    = { .61, .04, .04, 0.55 };
    GLfloat objeto_especular[] = { .73, .63, .63, 0.55 };
    GLfloat objeto_brilho[]    = { 77.0f };

    // Define os parametros da superficie a ser iluminada
    glMaterialfv(GL_FRONT, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, objeto_difusa);
    glMaterialfv(GL_FRONT, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT, GL_SHININESS, objeto_brilho);
}

//material preto
void setMaterial2(void)
{

    GLfloat objeto_ambient[]   = { 0, 0, 0, 1 };
    GLfloat objeto_difusa[]    = { .01, .01 , .01, 1.0 };
    GLfloat objeto_especular[] = { .5, .5, .5, 1.0 };
    GLfloat objeto_brilho[]    = { 32.0f };

    // Define os parametros da superficie a ser iluminada
    glMaterialfv(GL_FRONT, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, objeto_difusa);
    glMaterialfv(GL_FRONT, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT, GL_SHININESS, objeto_brilho);
}

//material amarelo
void setMaterial7(void)
{

    GLfloat objeto_ambient[]   = { .33, .22, .03, 1.0 };
    GLfloat objeto_difusa[]    = { .78, .57 , .11, 1.0 };
    GLfloat objeto_especular[] = { .99, .94, .81, 1.0 };
    GLfloat objeto_brilho[]    = { 28.0f };

    // Define os parametros da superficie a ser iluminada
    glMaterialfv(GL_FRONT, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, objeto_difusa);
    glMaterialfv(GL_FRONT, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT, GL_SHININESS, objeto_brilho);
}

void setMaterial3(void)
{

    GLfloat objeto_ambient[]   = { .11, .06, .11, 1.0 };
    GLfloat objeto_difusa[]    = { .43, .47 , .54, 1.0 };
    GLfloat objeto_especular[] = { .33, .33, .52, 1.0 };
    GLfloat objeto_brilho[]    = { 10.0f };

    // Define os parametros da superficie a ser iluminada
    glMaterialfv(GL_FRONT, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, objeto_difusa);
    glMaterialfv(GL_FRONT, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT, GL_SHININESS, objeto_brilho);
}

//material cinza
void setMaterial4(void)
{

    GLfloat objeto_ambient[]   = { .25, .25, .25, 1};//.55
    GLfloat objeto_difusa[]    = { .40, .40 , .40, 1};
    GLfloat objeto_especular[] = { .77, .77, .77, 1 };
    GLfloat objeto_brilho[]    = { 77.0f };

    // Define os parametros da superficie a ser iluminada
    glMaterialfv(GL_FRONT, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, objeto_difusa);
    glMaterialfv(GL_FRONT, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT, GL_SHININESS, objeto_brilho);
}

void setMaterial5(void)
{

    GLfloat objeto_ambient[]   = { .25, .21, .21, .92 };
    GLfloat objeto_difusa[]    = { 1, .83 , .83, .92 };
    GLfloat objeto_especular[] = { .3, .30, .30, .92 };
    GLfloat objeto_brilho[]    = { 11.0f };

    // Define os parametros da superficie a ser iluminada
    glMaterialfv(GL_FRONT, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, objeto_difusa);
    glMaterialfv(GL_FRONT, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT, GL_SHININESS, objeto_brilho);
}

//calculo do produto vetorial que gera o vetor normal
vertice* CalculaNormal(float x1,float y1,float z1,float x2,float y2,float z2) // Vertices
{
    vertice* vn = new vertice;
    double len;


    /* Calculo do produto vetorial de v1 e v2 */
    vn->x = (y1 * z2) - (z1 * y2);
    vn->y = (z1 * x2) - (x1 * z2);
    vn->z = (x1 * y2) - (y1 * x2);

    /* normalizacao de n */
    len = sqrt(pow(vn->x,2) + pow(vn->y,2) + pow(vn->z,2));

    vn->x /= len;
    vn->y /= len;
    vn->z /= len;

    return vn;
}


//escolhe um dos materiais
void setMaterial(int tipo){
    switch(tipo){
    case 1: setMaterial1(); break;
    case 2: setMaterial2(); break;
    case 3: setMaterial3(); break;
    case 4: setMaterial4(); break;
    case 7: setMaterial7(); break;

    default: break;
    }
}


//desenha as superficies que fecham o cilindro pela direita ou esquerda
void desenhaLateralzinhaDoMeio(float inicio, float altura, float g, bool frente, float raio){
//    cout << "vou desenhar no ponto x " << raio * cos(g * PI / 180)<< " z " << raio * sin(g * PI / 180)<< endl;
    glPushMatrix();
    //calcula normal
    vertice* vn = CalculaNormal(raio * cos(g * PI / 180), inicio, raio * sin(g * PI / 180), raio * cos(g * PI / 180), altura, raio * sin(g * PI / 180));
    //coloca o vetor normal corretamente
    if(!frente){
        vn->x = - vn->x;
        vn->y = - vn->y;
        vn->z = - vn->z;
    }
    glNormal3f(vn->x, vn->y, vn->z);
    //desenha com quads do centro ate o ponto especifiado
    glBegin(GL_QUADS);
                glVertex3f(raio * cos(g * PI / 180), inicio, raio * sin(g * PI / 180));
                glVertex3f(raio * cos(g * PI / 180), altura, raio * sin(g * PI / 180));
                glVertex3f(0, altura, 0);
                glVertex3f(0, inicio, 0);
    glEnd();
//        glNormal3f(-raio * sin(g * PI / 180), 0 , -raio * cos(g * PI / 180));


    glPopMatrix();
}

//desenha as paredes circulares do cilindro
void desenhaLateral(float raio, float inicio, float altura, vector<int>& vetor){
    //indica o lado que vai ficar a normal
    bool direita=false;
    float i = 0;
    float g = 0;
    //percorrido eh o que faz ele andar o angulo
    float percorrido = 0;

            for( int k = 0; k < vetor.size(); k+=2){ // percorre lista de fatias

//

                //pega do vetor que foi passado, qual eh o tamanho da fatia
                angle=vetor[k];//ate onde vou?

    //pega do vetor passado, qual eh o material da fatia em questao
                if(vetor[(k+1)] != 0){//desenha soh se material nao for nulo
                    setMaterial(vetor[(k+1)]);
                    glPushMatrix();
                        glBegin(GL_QUAD_STRIP);
                        //comeca em percorrido, pq ele considera tambem o que ja se passou
                        for(g = percorrido; g <= percorrido + angle /*ate aqui*/; g += 1){
                                glVertex3f(raio * cos(g * PI / 180), inicio, raio * sin(g * PI / 180));
                                glVertex3f(raio * cos(g * PI / 180), altura, raio * sin(g * PI / 180));
                                glNormal3f(cos(g * PI / 180), 0, sin(g * PI / 180));

                        }
                        //para mehorar o desenho, pq o passo de 1 acima eh muito grande pra alguns detalhes
                        if(angle==360){
                            for(g = 360; g <= 362/*ate aqui*/; g += 0.01){
                                glVertex3f(raio * cos(g * PI / 180), inicio, raio * sin(g * PI / 180));
                                glVertex3f(raio * cos(g * PI / 180), altura, raio * sin(g * PI / 180));
                                glNormal3f(cos(g * PI / 180), 0, sin(g * PI / 180));
                            }
                        }
                        glEnd();
                    glPopMatrix();
                    //desenha a parede do inicio
                    desenhaLateralzinhaDoMeio(inicio, altura, percorrido, direita, raio);
                    //agora desenha a normal para o outro lado
                    direita=!direita;
                }else{ //quando tem um vazio
                    if(percorrido!=0){ //percorrido nao esta mais no inicio
                            //desenha a capinha do final
                        desenhaLateralzinhaDoMeio(inicio, altura, percorrido, direita, raio);
                        direita=!direita;
                    }
                }
                 //depois disso, atualiza-se o percorrido
                percorrido += angle;
            }
            //desenha parede do ultimo de todos
            desenhaLateralzinhaDoMeio(inicio, altura, percorrido, direita, raio);
            //seta percorrido para o inicio de novo
            percorrido=0;

}

//desenha parte de cima e de baixo do cilindro
void desenhaTampa(float raio, float altura, bool ehTampa, vector<int>& vetor){
    float i = 0;
    int g = 0;
    //percorrido eh o que faz ele andar o angulo
    int percorrido = 0;

        for( int k = 0; k < vetor.size(); k+=2){ // percorre lista de fatias
//
            //pega do vetor que foi passado, qual eh o tamanho da fatia
            angle=vetor[k];//ate onde vou?

//pega do vetor passado, qual eh o material da fatia em questao
            if(vetor[(k+1)] != 0){// desenha se o material nao for "vazio"
                setMaterial(vetor[(k+1)]);
                //se for tampa de cima a normal eh 1, se for de baixo eh negativa
                    glNormal3f(0, ehTampa ? 1 : -1 , 0);

                    glBegin(GL_TRIANGLE_FAN);
                    glVertex3f(0, altura, 0);

                    //comeca em percorrido, pq ele considera tambem o que ja se passou
                    for(g = percorrido; g <= percorrido + angle /*ate aqui*/; g += 1){
                            glVertex3f(raio * cos(g * PI / 180), altura, raio * sin(g * PI / 180));
                            glNormal3f(0, ehTampa ? 1 : -1 , 0);
                    }
                    glEnd();
            }
            //aualiza o percorrido
            percorrido += angle;
        }
        //volta o percorrido p inicio
        percorrido=0;
}

//desenha o nivel
void desenhaNivel( float raio, float inicio, float altura, vector<int>& vetor ){
    float percorrido = 0;
    float i, g;
//desenha as partes do cilindro, com as funcoes ja explicadas
    glPushMatrix();
        desenhaLateral(raio, inicio, altura, vetor);
        desenhaTampa(raio, inicio, false, vetor);
        desenhaTampa(raio, altura, true, vetor);
    glPopMatrix();
}

// refaz vetor de limites
//quando esta movendo os vermelhos, ela corrige a estrutura de limites dos planos pretos
// ea  de cenarios
void reestruturaVetor(int nivel){

        int pos = 0;
        //vai desenhar tudo de novo
        limPlanosVermelhos[nivel].clear();
        limPlanosPretos[nivel].clear();
        //percorre todos os elementos de um nivel do cenario
        for(int i=0; i < cenario[nivel].size(); i+=2){
                //se o proximo elemento for vazio
            if(cenario[nivel][i+1] == 0){
                //entao pode mover
                pos+=cenario[nivel][i];
            }else{
                //se for um vermelho
                if(cenario[nivel][i+1] == 1){
                    //redefine os limites do vermelho, para ele poder andar
                    int *aux = new int[2];
                    aux[0] = pos;
                    pos += cenario[nivel][i];// soma o tamanho
                    aux[1] = pos;
                    limPlanosVermelhos[nivel].push_back(aux);
                }else{ //se for um preto
                    //tambem anda a mesma quantidade com ele e redefine seus limites
                    int *aux = new int[2];
                    aux[0] = pos;
                    pos += cenario[nivel][i];// soma o tamanho
                    aux[1] = pos;
                    limPlanosPretos[nivel].push_back(aux);
                }
            }
        }
        //serve para exibir as mudancas feitas
//        cout << "limites pretos" << " , ";
//        for(int* u:limPlanosPretos[nivel]){
//            cout << u[0] <<" , " << u[1] << " , ";
//        }
//        cout << endl;
//
//        cout << "limites vermelhos" << " , ";
//        for(int* u:limPlanosVermelhos[nivel]){
//            cout << u[0] <<" , " << u[1] << " , ";
//        }
//        cout << endl;
//
//        cout << "vetor de desenho" << " , ";
//        for(int u:cenario[nivel]){
//            cout << u <<" , " ;
//        }
//        cout << endl;

}



//preenche o vetor com as coordenadas geradas na funcao acima
//gera e cria os limites dos vermelhos e as respectivas velocidades
void criaCenarioAleatorio(){
     for(int indice=0; indice < NUM_NIVEIS; indice++){
        //pega um dos 10 possiveis cenarios
        int aleatorio = rand()%numPadroesPlataforma;
        //antes do oitavo cenario nao mexe vermelho
        if(aleatorio >= 8)
            //da velocidade p os dois lados, senao todos iam ir para mesmo lado
            velocidadeVermelha[indice] = (rand()%2==0? -1: 1) * (rand()%4+1);
        else
            velocidadeVermelha[indice] = 0;
        //da um giro aleatorio de no maximo 50 graus
        int giroAleatorio = rand()%30 + 20;

        padroes[aleatorio][0] = giroAleatorio;//giro aleatorio
        //troca o tipo da base, preto ou transparente, para melhorar a jogabilidade
        padroes[aleatorio][1] = rand()%2 == 1? 0: 2;

        //passa o padrao gerado aleatoriamente para o cenario
        cenario.push_back(padroes[aleatorio]);
        int pos = 0;
        //faz o mesmo que o reestrutura vetor acima
        //gera os limites para colisao a partir do cenario
        for(int i=0; i < padroes[aleatorio].size(); i+=2){
            if(padroes[aleatorio][i+1] == 0){//se o proximo material eh vazio, soh anda
                pos+=padroes[aleatorio][i];
            }else{//senao
                //se for vermelho
                if(padroes[aleatorio][i+1] == 1){
                    int *aux = new int[2];
                    aux[0] = pos;
                    pos += padroes[aleatorio][i];// soma o tamanho
                    aux[1] = pos;
                    limPlanosVermelhos[indice].push_back(aux);
                }else{ //se for preto
                    int *aux = new int[2];
                    aux[0] = pos;
                    pos += padroes[aleatorio][i];// soma o tamanho
                    aux[1] = pos;
                    limPlanosPretos[indice].push_back(aux);
                }
            }
            //imprime como pedido
            cout << padroes[aleatorio][i] << " , ";
        }
        cout << endl;

    }
}


//funcao para mover os vermelhos
void moveVermelhos(vector<int>& cenario, int* velocidade, int nivel){// mexer a partir das laterais dos espaços vazios(0)
    if((*velocidade) != 0){// é movel
        for( int i=0; i < cenario.size(); i+=2 ){
            if(cenario[i+1] == 1){//achei vermelho
                if((*velocidade)>0){//indo no sentido positivo
                    if(cenario[i+2] >= (*velocidade)){//tem espacp para andar para a direita
                        cenario[i+2]-=(*velocidade);//vazio da frente diminui
                        cenario[i-2]+=(*velocidade);//vazio de tras aumenta
                    }else{//não tem mais espaco para andar
                        (*velocidade)*=-1;
                    }
                }else{//indo para a esquerda, entao acontece o inverso do acima
                    if(cenario[i-2] >= -1*(*velocidade)){//tem espacp para andar para a esquerda
                        cenario[i-2]+=(*velocidade);
                        cenario[i+2]-=(*velocidade);
                    }else{//não tem mais espaco para andar
                        (*velocidade)*=-1;
                    }
                }

            }

        }
        //com os limites redefinidos, chama o reestrutura vetor
        reestruturaVetor(nivel);
    }
    cout <<"novo vetor de desenho:" << endl;
    for(int u:cenario){
        cout << u << " , ";
    }
    cout << endl;
}

//desenha o cenario em si
//chama desenha niveis pretos e desenha niveis vermelhos
void desenhaCenario(){
//percorre todos os niveis


    for(int indice=0; indice<nivel+1; indice++){
        //move os vermelhos
        moveVermelhos(cenario[indice], &(velocidadeVermelha[indice]), nivel);
        //desenha o nivel
        desenhaNivel(raiosDeVetores[indice], DISTANCIA_INTERPLATAFORMA*indice, (DISTANCIA_INTERPLATAFORMA*indice)+10, cenario[indice]);
    }
//quando ja passou por aquele nivel, dah o efeito de diminuir raio
    for(int indice=nivel+1; indice<NUM_NIVEIS; indice++){
        if(raiosDeVetores[indice])
           raiosDeVetores[indice]-=0.5;
        desenhaNivel(raiosDeVetores[indice], DISTANCIA_INTERPLATAFORMA*(nivel+1), (DISTANCIA_INTERPLATAFORMA*(nivel+1))+10, destroi);
    }
}




void init(void)
{
   glClearColor (0.1, 0.1, 0.1, 0.0);

//   glClearColor (0.5, 0.5, 0.5, 0.5);
   glShadeModel (GL_SMOOTH);

   glEnable(GL_LIGHT0);                   // habilita luz 0
   glEnable(GL_LIGHTING);                // Habilita luz
   glEnable(GL_DEPTH_TEST);              // Habilita Z-buffer
   glEnable(GL_NORMALIZE);               // Normalização dos vetores normais

//   glEnable(GL_COLOR_MATERIAL);           // Utiliza cor do objeto como material
 // Cor da fonte de luz (RGBA)
    GLfloat cor_luz[]     = { 1.0, 1.0, 1.0, 0.0};
//    // Posicao da fonte de luz. Ultimo parametro define se a luz sera direcional (0.0) ou tera uma posicional (1.0)
    GLfloat posicao_luz[] = {  0 , 0, 100, 0.0};

// Define parametros básicos da luz
    glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz);
    glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz);
    glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);

//   glColorMaterial(GL_FRONT, GL_DIFFUSE);

//   glEnable(GL_CULL_FACE);                // Habilita Backface-Culling
}

//colisao
bool planoEstaEmbaixoDaBolinhaNoMesmoNivel(vector<int*>& xPlano){
    if(!xPlano.empty()){
        for(int i=0; i<xPlano.size(); i++){
                //a bolinha esta entre esses limites de graus
                if(moveTelaX >= xPlano[i][0]  && moveTelaX<= xPlano[i][1]){
                    return true;
                }
        }
    }
    return false;

}


// verifica se a esfera tocou por cima o plano, enquanto estava descendo
bool tocouPlanoPorCimaNoMesmoNivel(float posicaoBolinhaX, float posicaoBolinhaY, vector<int*>& limPlanosPretos){
    //derivada da equacao que descreve o movimento
    double dx = -4*(posicaoBolinhaX-10);
    //limiar que e um artificio para evitar "==" no codigo, visto que o programa se vale de pontos flutuantes
    double limiar = 1.2 + 2.5; // aumentar se aumentar a aceleracao
    //se a posicao da esfera em Y for menor do q a posicao do plano movel+limiar
    // E a posicao da esfera em Y for maior que a posicao do plano movel - limiar
    // seria posicaoBolinhaY == posicaoPlataforma , se fosse possivel  uma precisao forte para ponto flutuante
    //o que indica que a esfera esta na mesma altura que o plano
    // 540 + 1.2
    bool comp = posicaoBolinhaY - 5  < (nivel) * DISTANCIA_INTERPLATAFORMA + limiar && posicaoBolinhaY - 5 > (nivel)*DISTANCIA_INTERPLATAFORMA - limiar;

    //derivada menor que zero indica que a bolinha esta descendo
    //se plano esta embaixo da bolinha (por que poderia ocorrer mesmo sem o plano por baixo, mas nos interessa so esse caso)
    return (comp && dx < 0 && planoEstaEmbaixoDaBolinhaNoMesmoNivel(limPlanosPretos));
}

//funcao que controla as acoes de quando algo que levou a perda do jogo acontece
void perdeuJogo(){
    pause = true;
}

//reinicia o jogo
void reiniciaJogo(){
    //tem que colocar todos os raios dos cilindros de cada nivel de novo para 50
    for(int indice=0; indice<NUM_NIVEIS; indice++){
        raiosDeVetores[indice] = 50;
    }
    //voltar moveTelaX para inicio
    moveTelaX = 0;

    translacaoEsfera = PONTO_INICIAL;
    //os niveis voltam a ser la em cima
    nivel = NUM_NIVEIS-1;
    //a equacao da bolinha volta ao inicio
    transposicao = nivel*DISTANCIA_INTERPLATAFORMA;
    //pausa o jogo
    descida = nivel*DISTANCIA_INTERPLATAFORMA;
    pause = false;
}

// equacao que descreve o movimento da esfera
float quedaLivre(float x){
//pos= posicao da esfera em y
//a pos em y da esfera e dada por essa equacao, considerando que quando o plano tem sua posicao em y
//modificada e a esfera esbarra nele por cima, a parabola eh transposta em Y
    pos = -2* pow((x-10),2)+50 + transposicao;


//se ela passa por algum intervalo, diminui a variavel nivel, porque ela passou p baixo
    if(( (pos + 5 + 1 < (nivel)*DISTANCIA_INTERPLATAFORMA)) && !tocouPlanoPorCimaNoMesmoNivel(x, pos-5, limPlanosPretos[nivel])){
        nivel--;

    }
    //pos -5 deixa na cabeça da bolinha
    if(( (pos -5 < (nivel)*DISTANCIA_INTERPLATAFORMA)) && !tocouPlanoPorCimaNoMesmoNivel(x, pos-5, limPlanosPretos[nivel]))//evita engasgada ao passar de nivel
            toCaindo = true;


    if(toCaindo) // flag que indica quando esta descendo
        descida = pos;

//se tocou veremlho, perde o jogo
    if(tocouPlanoPorCimaNoMesmoNivel(moveTelaX, pos-5, limPlanosVermelhos[nivel])){
        perdeuJogo();
    }
    //se a esfera tocou o plano por cima, isso significa que o parametro transposicao da equacao deve
    //ser mudado para este nivel atual, e translacaoEsfera volta p ponto inicial para fazer o movimento de nv
    if(tocouPlanoPorCimaNoMesmoNivel(x, pos-5, limPlanosPretos[nivel])){
        transposicao = (nivel)*DISTANCIA_INTERPLATAFORMA;

        translacaoEsfera = PONTO_INICIAL;
        if(toCaindo ){
            toCaindo = false;
        }
    }


         //se chegou no fim do cenario, pausa o jogo
    if( pos < 0 ){
        perdeuJogo();
    }



    return pos;
}


void idle()
{
    float t, desiredFrameTime, frameTime;
    static float tLast = 0.0;

    if(current <= 360){
      angle += ANGLE;
      current++;
    }

//    deformacaoEsfera = 1;
    // Get elapsed time
    t = glutGet(GLUT_ELAPSED_TIME);
    // convert milliseconds to seconds
    t /= 1000.0;

    // Calculate frame time
    frameTime = t - tLast;
    // Calculate desired frame time
//    desiredFrameTime = 1.0 / (float) (desiredFPS);

    // Check if the desired frame time was achieved. If not, skip animation.
    if( frameTime <= desiredFrameTime)
        return;
//se pausar o jogo, nao desenha nada novo
    if(pause)
        return;
    /*
     *UPDATE ANIMATION VARIABLES
     */
    translacaoEsfera += aceleracaoEsfera;



    /* Update tLast for next time, using static local variable */
    tLast = t;

    glutPostRedisplay();

}


void desenhaJogo(){
    glPushMatrix();
//o jogo estava de cabeca p baixo, entao -90 joga p cima
        glRotatef( -90.0, 1.0, 0.0, 0.0 );

        glPushMatrix();
//esfera e plano movel sao criados na mesma matriz para que se um for movido, movam-se todos
                glPushMatrix();
                //aplica a animacao
                    glutIdleFunc(idle); //animacao
                    // passa a posicao em x da esfera para ser calculada sua trajetoria e com isso a
                    //esfera ser transladada
                    glTranslatef( 0, -40,  quedaLivre(translacaoEsfera));
                    //material da bolinha eh amarelo
                    setMaterial(7);
                    glutSolidSphere(5, 100, 100);

                glPopMatrix();

        glPopMatrix();
    glPopMatrix();

}




void display(void)
{
//    if(pause)
//        return;
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   // o aspect esta sendo dado por width/height
   //o plano mais proximo esta em 1 e o mais distante em 200, isso passa a ideia de 3D
   //o angulo esta 90 graus para parecer 2D
   gluPerspective(90.0, (GLfloat) width/(GLfloat) height, 1.0, 1000.0);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity ();
    GLfloat light[] = {0, 0, 100, 0};
    glLightfv(GL_LIGHT0, GL_POSITION, light );
    //o movimento da bola e do mouse sao acompanhados pela camera, a fim de dar a impressao de que o cenario esta descendo

    gluLookAt(0, descida+50, 120, 0.0, 0.0+descida+50-58, 0, 0.0, 58, 0);

    vector<int> eixoPrincipal{360, 4};//cilindro cinza do meio


    desenhaJogo();
    glPushMatrix();
    //para comecar do zero corretamente, para ficar na frente do jogador
        glRotatef(moveTelaX-90, 0.0, 1.0, 0.0);
        desenhaNivel( 25, 0, DISTANCIA_INTERPLATAFORMA*(NUM_NIVEIS), eixoPrincipal);

        desenhaCenario();
    glPopMatrix();

    glutSwapBuffers();
}


// esse reshape reescala a imagem para fullscreen
void reshape (int w, int h)
{

        width = w;
        height = h;


    glViewport (0, 0, (GLsizei) width, (GLsizei) height);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

    if (w <= h)
        glOrtho (-1, 1, -1*(GLfloat)height/(GLfloat)width, 1*(GLfloat)height/(GLfloat)width, -10.0, 10.0);
    else
        glOrtho (-1*(GLfloat)width/(GLfloat)height, 1*(GLfloat)width/(GLfloat)height, -1, 1, -10.0, 10.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void keyboard (unsigned char key, int x, int y)
{
   switch (tolower(key))
   {
      case '+' :
         distOrigem--;
//         if(distOrigem<20) distOrigem=20;
      break;
      case '-' :
         distOrigem++;
//         if(distOrigem>180) distOrigem=180;
      break;

      case 'w' :
         desloCima+=10;

      break;
      case 'a' :
         desloDir-=3;

      break;
      case 's' :
        if(desloCima >= 10)// nao desce para baixo do plano
            desloCima-=10;

      break;
      case 'd' :
         desloDir+=3;

      break;
      case 'f': //deixa o jogo em tela cheia
        fullScreen = !fullScreen;
        if (fullScreen)
        {
            glutFullScreen();
        }
        else
        {
            glutReshapeWindow(width, height);
        }
        break;
        case 'r':
            perdeuJogo(); // reinicia o jogo
        break;
        case 'p':
            pause = !pause;//trava todos os evenntos
        break;

      case 27:
         exit(0);
      break;
   }
}


void motion(int x, int y)
{
	// This variable is hack to stop glutWarpPointer from triggering an event callback to Mouse(...)
	// This avoids it being called recursively and hanging up the event loop
	static bool just_warped = false;

	if(just_warped)
	{
		just_warped = false;
		return;
	}

        float dx = x - width/2;

        if(dx>0) moveTelaX += -1*(abs(dx)); // move no sentido positivo
        if(dx<0) moveTelaX += +1*(abs(dx));// move no sentido negativo
//nao deixa moveTelaX sair entre 0 e 360
        if(moveTelaX>360)
            moveTelaX = 0 + moveTelaX - 359;
        if(moveTelaX<0)
            moveTelaX = 359 + moveTelaX;
        moveTelaX = moveTelaX;


	if(!releaseMouse)	glutWarpPointer(width/2, height/2); // trava o mouse

	just_warped = true;
}



// Mouse callback



//habilita uso do mouse para zoom
void mouse(int button, int state, int x, int y)
{

    if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
    {
        last_x = x;
        last_y = y;
    }
     if(button == 0) // Scroll up
    {
        reiniciaJogo();
    }

    if(button == 3) // Scroll up
    {
        distOrigem--;
         if(distOrigem<20) distOrigem=20;
    }
    if(button==4)   // Scroll down
    {
       distOrigem++;
         if(distOrigem>180) distOrigem=180;
    }
}


int main(int argc, char** argv)
{
    srand( (unsigned)time(NULL) );

    for(int i = 0 ; i< NUM_NIVEIS; i++ ){
        raiosDeVetores.push_back(50);
    }

   criaCenarioAleatorio();

   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (width, height);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);


   glutSetCursor(GLUT_CURSOR_NONE); // esconde o mouse da tela

    glutIgnoreKeyRepeat(1);

   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
//   glutWarpPointer(width/2, height/2);// na primira vez da reshape
   glutMouseFunc( mouse );
//   glutMotionFunc( motion );
    glutPassiveMotionFunc(motion);

   glutKeyboardFunc(keyboard);
   glutFullScreen();

   glutIdleFunc(idle);
   glutMainLoop();
   return 0;
}

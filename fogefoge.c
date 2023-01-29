#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fogefoge.h"
#include "mapa.h"
#include "ui.h"

MAPA m;
POSICAO boneco;
int tempilula = 0;

int direcaofantasma(int xatual, int yatual, int *xdestino, int *ydestino)
{
	int opcoes[4][2]=
	{
		{xatual,yatual+1}, //mover para a direita
		{xatual+1,yatual}, //mover para baixo
		{xatual,yatual-1}, //mover para esquerda
		{xatual-1,yatual} //mover para cima
	};
	
	srand(time(0));
	
	for(int i=0;i<10;i++)
	{
		int posicao=(rand()%4);
		
		if((podeandar(&m, FANTASMA, opcoes[posicao][0], opcoes[posicao][1]) && !ehpersonagem(&m, PILULA, opcoes[posicao][0], opcoes[posicao][1])))
		{
			*xdestino=opcoes[posicao][0];
			*ydestino=opcoes[posicao][1];
			
			return 1;
		}
	}
	
	return 0;
}
	

void fantasmas()
{
	MAPA copia;
	
	copiamapa(&copia,&m);
	
	for(int i=0;i<m.linhas;i++)
	{
		for(int j=0;j<m.colunas;j++)
		{
			if(copia.matriz[i][j]==FANTASMA)
			{
				int xdestino;
				int ydestino;
				
				int encontrou = direcaofantasma(i,j,&xdestino,&ydestino);
				
				if(encontrou)
				{
					andanomapa(&m,i,j,xdestino,ydestino);
				}
			}
		}
	}
	
	liberamapa(&copia);
}

int acabou()
{
    POSICAO pos;
    int fogefogenomapa = encontramapa(&m, &pos, BONECO);
    return !fogefogenomapa;
}

int ehdirecao(char direcao)
{
	return direcao == 'a' || direcao == 'w' || direcao == 'd' || direcao == 's';
}

void move(char direcao)
{
	if(!ehdirecao(direcao))
	{
		return;
	}

	int proximox = boneco.x;
	int proximoy = boneco.y;

    switch(direcao)
    {
        case ESQUERDA:
            proximoy--;
            break;
        case CIMA:
            proximox--;
            break;
        case DIREITA:
            proximoy++;
            break;
        case BAIXO:
           proximox++;
            break; 
    }
    
	if(!podeandar(&m,BONECO,proximox,proximoy))
		return;
		
	if(ehpersonagem(&m, PILULA, proximox, proximoy))
	{
		tempilula = 1;
	}
    
    	
    andanomapa(&m, boneco.x, boneco.y, proximox, proximoy);
    boneco.x=proximox;
    boneco.y=proximoy;

}

void explodepilula()
{
	if(!tempilula) return;
	
	explodepilula2(boneco.x,boneco.y,0,1,3); //direita
	explodepilula2(boneco.x,boneco.y,0,-1,3); //esquerda
	explodepilula2(boneco.x,boneco.y,1,0,3); //cima
	explodepilula2(boneco.x,boneco.y,-1,0,3); //baixo
	
	tempilula=0;
}

void explodepilula2(int x, int y, int somax, int somay, int qtd)
{
	if(qtd==0) return;
	
	int novox=x+somax;
	int novoy=y+somay;
	
	if(!ehvalida(&m,novox,novoy)) return;
	if(ehparede(&m,novox,novoy)) return;
	
	
	m.matriz[novox][novoy] = VAZIO;
	explodepilula2(novox, novoy, somax, somay, qtd - 1);
}

int main()
{
    lermapa(&m);
    encontramapa(&m,&boneco,BONECO);

    do
    {	
    	printf("Tem pilula: %s\n", (tempilula ? "SIM" : "NAO"));
        imprimemapa(&m);

        char comando;
        scanf(" %c",&comando);
        
        if(ehdirecao(comando)) move(comando);
        if(comando == BOMBA) explodepilula(boneco.x,boneco.y, 3);
        
        fantasmas();

    } while(!acabou());

    liberamapa(&m);
}
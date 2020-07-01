#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<stdlib.h>
#include<list>
using namespace std;

//Estas definiciones permiten asignar el uso de las flechas
#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80

void gotoxy(int x, int y){
	HANDLE hCon; 
	hCon=GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	SetConsoleCursorPosition(hCon, dwPos);//Toma el control del cursor 
 
}

void OcultarCursor(){
	HANDLE hCon; 
	hCon=GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 50;//depende del valor es el estilo del cursor
	cci.bVisible=false;//Toma dos valores true or false(False ocualta cursor)
	//Esta funcion permite controlar las caracteristicas del cursor en patalla
	SetConsoleCursorInfo(hCon,&cci);//el amperson es para pasar por referencia el valor
}

void Dibujar_Limites(){
	//Limite Superior e inferior
	for(int i=2;i<118;i++){
		gotoxy(i,3);
		printf("%c",205);
		gotoxy(i,30);
		printf("%c",205);
	}
	
	//Limite de los costados
	for(int i = 4;i<30;i++){
		gotoxy(1,i);
		printf("%c",186);
		gotoxy(118,i);
		printf("%c",186);
	}
	
	//Aqui se dibujan las esquinas de los limites
	gotoxy(1,3); printf("%c",201);
	gotoxy(1,30); printf("%c",200);
	gotoxy(118,3); printf("%c",187);
	gotoxy(118,30); printf("%c",188);
}

class NAVE{
	 int x,y;//Variables de forma local 
	 int corazones;
	 int vidas;
public:
	NAVE(int _x, int _y, int _corazones, int _vidas):x(_x),y(_y),corazones(_corazones),vidas(_vidas){}//El constructor lleva el mismo nombre que la clase
	int X(){return x;}
	int Y(){return y;}
	int VID() {return vidas;}//Esto es una funcion
	void COR (){corazones--;}
	void Dibujar_Nave();
	void Borrar_Nave();
	void Mover_Nave();
	void Dibujar_Corazones();
	void Morir();	
};


void NAVE::Dibujar_Nave(){
	gotoxy(x,y);   printf("  %c",218);
	gotoxy(x,y+1); printf("  %c",64);
	gotoxy(x,y+2); printf(" %c%c%c",47,124,92);
	gotoxy(x,y+3); printf(" %c %c",47,92);
}

void NAVE::Borrar_Nave(){
	gotoxy(x,y);   printf("       ");
	gotoxy(x,y+1); printf("       ");
	gotoxy(x,y+2); printf("       ");
	gotoxy(x,y+3); printf("       ");
}

void NAVE::Mover_Nave(){
		if(kbhit()){
			char tecla = getch();
			Borrar_Nave();
			if(tecla == IZQUIERDA && x>3) 
				x--;
			if(tecla == DERECHA && x+3<114) 
				x++;
			if(tecla == ARRIBA && y>4) 
				y--;
			if(tecla == ABAJO && y+3<28) 
				y++;
			if(tecla == 'e')corazones--;
			Dibujar_Nave();
			Dibujar_Corazones();
		}
	
}

void NAVE::Dibujar_Corazones(){
	
	gotoxy(90,2); printf("Vidas: %d",vidas);
	gotoxy(105,2); printf("Salud:");
	gotoxy(111,2); printf("      ");
	for(int i= 0;i<corazones;i++){
		gotoxy(111+i,2);
		printf("%c ",254);
	}
}
void NAVE::Morir(){
	if(corazones==0){
		Borrar_Nave();
		gotoxy(x,y);  printf("  ** ");
		gotoxy(x,y+1);printf("* ** *");
		gotoxy(x,y+2);printf("  **  ");
		Sleep(300);
		
		Borrar_Nave();
		gotoxy(x,y);  printf("* ** *");
		gotoxy(x,y+1);printf(" **** ");
		gotoxy(x,y+2);printf("* ** *");
		Sleep(300);
		Borrar_Nave();
		
		vidas--;
		corazones=3;
		Dibujar_Corazones();
		Dibujar_Nave();
	}
}

class AST{
	int x,y;
public:
	AST(int _x, int _y): x( _x),y( _y){}
	void Dibujar_Ast();
	void Mover_Ast();
	void choque( NAVE &N);
	int X(){return x;}//Permite obtener las coordenads de los asteroides en X
	int Y(){return y;}//Permite obtener las coordenads de los asteroides en y
};

void AST::Dibujar_Ast(){
	gotoxy(x,y); printf("%c",207);
}

void AST::Mover_Ast(){
	gotoxy(x,y); printf(" ");
	y++;
	if(y>29){
		x = rand()%113+4;
		y = 4;
	}
	Dibujar_Ast();
}

void AST::choque(NAVE &N){
	if(x>=N.X()&&x<N.X()+5 && y>=N.Y() && y<=N.Y()+2){
		N.COR();
		N.Borrar_Nave();
		N.Dibujar_Nave();
		N.Dibujar_Corazones();
		x = rand()%113+4;
		y = 4;
	}
}

class BALA{
	int x,y;
public:
	BALA(int _x, int _y):x(_x),y(_y){}
	int X(){return x;}//Permite obtener las coordenads de los asteroides en X
	int Y(){return y;}//Permite obtener las coordenads de los asteroides en y
	void Mover();
	bool Fuera();
};

void BALA::Mover(){
	gotoxy(x,y); printf(" ");
	y--;
	gotoxy(x,y); printf("%c",220);
}

bool BALA::Fuera(){
	if(y==4)return true;
	return false;
}

int main (){

	system ("color 5F" );
	OcultarCursor(); 
	Dibujar_Limites();
	NAVE N(37,25,3,3);//Define donde va a aparecer la nave,corazones,vidas
	
	N.Dibujar_Nave();
	N.Dibujar_Corazones();
	
	list<AST*>A;
	list<AST*>::iterator itA;
	for(int i=0;i<15;i++){
		A.push_back(new AST(rand()%114+3,rand()%5+4)); //el primer rand es para el ancho y el segundo es donde va a aparecer inicialmente el asteroide
	}
	
	list<BALA*>B;//Esto es un puntero
	list<BALA*>::iterator it;
	
	bool game_over = false;
	int puntos = 0;
	while(!game_over){
		
		gotoxy(4,2); printf("Puntos: %d",puntos);
		
		if(kbhit()){
			char tecla = getch();
			if(tecla=='a')
			B.push_back(new BALA(N.X()+2,N.Y()-1)); 
		}
		
		for(it = B.begin(); it!=B.end();it++){
			(*it)-> Mover ();
			
			if((*it)->Fuera()){
				gotoxy((*it)->X(),(*it)->Y());//Esto ayuda a obtener la coordenada
				printf(" ");//Este espacio es para que la bala se borre
				delete(*it);
				it=B.erase(it);//Con esta linea quitamos un espacio y pasamos el iterador a la siguiente linea 
			}
		}
		
		for(itA=A.begin(); itA!=A.end();itA++){
			(*itA)->Mover_Ast();
			(*itA)->choque(N);
		}
		
		for(itA = A.begin(); itA != A.end(); itA++){//Recorre los asteroides
			for(it = B.begin(); it != B.end(); it++){//Recorre las balas 
				if((*itA)->X() == (*it)->X() &&(
					(*itA)->Y()+1 == (*it)->Y() ||
					(*itA)->Y() == (*it)->Y()))
					{
						gotoxy((*it)->X(),(*it)->Y()); printf("   ");				
						delete(*it);
						it=B.erase(it);//pasa al siguiente elemento de la lista 
						
						A.push_back(new AST(rand() %110+5, 4));
						gotoxy((*itA)->X(),(*itA)->Y()); printf("   ");				
						delete(*itA);//Borra el objeto de la lista
						itA = A.erase(itA);
						
						puntos+=5;
					}
			}
		}
		
		if(N.VID()==0) game_over=true;
		N.Morir();
		N.Mover_Nave();
		Sleep(30);
	}
	
	
	system("cls"); // Limpiar pantalla (clear screen)
    printf("\n\n\n\n\n\n\n");
    Sleep(400);
    system ("color 90" );
    gotoxy(33,10);printf("Total de puntos: %d",puntos);
    Sleep(600);
    gotoxy(34,13);printf("!..GAME ");
    Sleep(600);
    gotoxy(41,13);printf(" OVER..!");
    Sleep(2000);
    printf("\n\n\n\n\n\n\n");
    
   	printf("\n\n\n\n\n\n\n");
   	gotoxy(33,16);printf("Gracias por jugar");
   	printf("\n\n\n\n");
   	
   	getch();
	return 0;
}

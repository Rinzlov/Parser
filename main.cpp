#include <iostream>
#include <cmath>
#include <math.h>
#include <string.h>

using namespace std;

float operation(float wynik, char sign, float liczba);
float count(char buf[300], float x);
float chopp(char inside[300], float x);

float operation(float wynik, char sign, float liczba)
{
	switch(sign)
	{
		case '*':
			wynik*=liczba;
			break;
		case '/':
			wynik=wynik/liczba;
			break;
		case '^':
			wynik=pow(wynik,liczba);
			break;
		case 's':
			wynik*=sin(liczba*(M_PI/180));
			break;
		case 'c':
			wynik*=cos(liczba*(M_PI/180));
			break;
		case 'L': // Ln
			wynik*=log(liczba);
			break;
		case 'l': // Log
			wynik*=log10(liczba);
			break;
	}
	return wynik;
}

float count(char buf[300], float x)
{
	int len=strlen(buf), i, j=0, k=0, nawias=0, b=0, sens=0;
	char buf2[300]=" ", sign='*';
	float wynik=1, liczba=0;
	for(i=0; i<len; ++i)
	{
		// DZIAŁANIE
		if(buf[i]=='*' || buf[i]=='/' || buf[i]==':' || buf[i]=='^') {sign=buf[i]; printf("%c", sign);}
		// X
		else if(buf[i]=='x') {printf("%0.2f", x); wynik=operation(wynik, sign, x); sens=1;}
		// NAWIAS - zawartośc wysyłamy znów do funkcji count
		else if(buf[i]=='(' || buf[i]=='[' || buf[i]=='{')
		{
			nawias=1;
			sens=1;
			k=0;
			i++;
			printf("(");
			while(nawias!=0)
			{
				if(buf[i+k]=='(' || buf[i+k]=='[') nawias++;
				else if(buf[i+k]==')' || buf[i+k]==']') nawias--;
				if(nawias>0) buf2[k]=buf[i+k];
				//printf("<%c>", buf2[k]);
				k++;
			}
			i+=k-1;
			//### printf("%s)=", buf2);
			liczba=chopp(buf2, x);
			//### printf("}");
			wynik=operation(wynik, sign, liczba);
			printf(")");
		}
		// LICZBA - obliczamy jej wartość
		else if(buf[i]>47 && buf[i]<58)
		{
			k=0;
			sens=1;
			while(((buf[i+k]>47 && buf[i+k]<58) || buf[i+k]==46) && k<len)
			{
				buf2[k]=buf[i+k];
				k++;
			}
			i+=k-1;
			liczba=strtod(buf2, NULL);
			printf("%0.2f", liczba);
			wynik=operation(wynik, sign, liczba);
		}
		else if(buf[i]=='+' || buf[i]=='-' || buf[i]==')' || buf[i]==']') {}
		// FUNKCJE SPECJALNE
		// SIN
		else if((buf[i]=='S' || buf[i]=='s') && (buf[i+1]=='i' || buf[i+1]=='I') && (buf[i+2]=='n' || buf[i+2]=='N'))
		{
			sign='s';
			printf("Sin");
			i+=2;
		}
		// COS
		else if((buf[i]=='C' || buf[i]=='c') && (buf[i+1]=='O' || buf[i+1]=='o') && (buf[i+2]=='S' || buf[i+2]=='s'))
		{
			sign='c';
			printf("Cos");
			i+=2;
		}
		// LN
		else if((buf[i]=='L' || buf[i]=='l') && (buf[i+1]=='N' || buf[i+1]=='n'))
		{
			sign='L';
			printf("Ln");
			i+=1;
		}
		// LOG
		else if((buf[i]=='L' || buf[i]=='l') && (buf[i+1]=='O' || buf[i+1]=='o') && (buf[i+2]=='G' || buf[i+2]=='g'))
		{
			sign='l';
			printf("Log");
			i+=2;
		}
		else printf("Nieznany znak: <%c> !\n", buf[i]);
		for(b=0; b<300; b++)
		{buf2[b]='\0';}
	}
	//### printf("=%i", wynik);
	return wynik*sens;
}

float chopp(char inside[300], float x)
{
	int i, b, j=0, nawias=0, len=strlen(inside);
	float wynik=0, wkoncowy=0, wkoncowy2=0;
	char znaknastepnego='+';
	char buf[300];
	// odśmiecanie bufora
	for(b=0; b<300; b++)
	{buf[b]='\0';}
	
	// dzielenie funkcji na części i oblicznie
	for(i=0; i<len; ++i)
	{
		//skopiowanie wartości
		buf[j]=inside[i];
		//włączenie lub wyłączenie trybu nawiasu
		if(inside[i]=='(' || inside[i]=='[') nawias++;
		if(inside[i]==')' || inside[i]==']') nawias--;
		//podział
		if((inside[i]=='+' || inside[i]=='-' || i==(len-1)) && nawias==0)
		{
			//### printf("Wyrazenie: <%c><%s> \nObliczanie: ", znaknastepnego, buf);
			// wyświetlenie i obliczenie wartości całego wyrażenia
			printf("%c", znaknastepnego);
			wynik=count(buf, x);
			// w zależności od znaku przed wyrażeniem + lub - od wyniku głównego
			wkoncowy2=wkoncowy;
			if(znaknastepnego=='-') wkoncowy-=wynik;
			else wkoncowy+=wynik;
			//### printf(" Aktualna wartosc calego wyrazenia: %0.2f%c%0.2f=%0.2f\n\n",wkoncowy2, znaknastepnego, wynik, wkoncowy);
			// znak NASTĘPNEGO wyrażenia
			znaknastepnego=buf[j];
			// wyczyszczenie bufora i jego zmiennej iteracyjnej
			j=0;
			for(b=0; b<300; b++)
			{buf[b]='\0';}
		}
		else j++;
	}
	return wkoncowy;
}



int main()
{
	char inside[300];
	float x=2, wynik=0;
	int	len=0;
	FILE *funkcja=fopen("funkcja.txt", "r");
	system("clear");
	if(funkcja==NULL)
	{
		printf("Brak pliku funkcja.txt.\nExit.\n");
		exit(1);
	}
	fscanf(funkcja, "%s", &inside);
	printf("Funkcja w calosci:\n%s\n\n", inside);
	printf("X wynosi: %0.2f  ", x);
	len=strlen(inside);
	printf("Dlugosc lancucha: %i\n\nObliczanie:\n\n", len);
	wynik=chopp(inside, x);
	printf("=%0.5f", wynik);
	printf("\n\nWynik to: %0.5f\n\n", wynik);
	
	fclose(funkcja);
	//return wynik; zmienić int main() na float parser
}

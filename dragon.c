#include <stdio.h>
#include <stdlib.h>


int exponent(int num, int e) {
	int n = 1;
	while(e--)
		n*=num;
	return n;
}


void iteruj(int *pole, int *iteracia) {
	int i;
	for(i = 1; i < *iteracia; i++) {
		pole[exponent(2,i)-1] = 0;

		int rozdiel = 2;
		for(int j = exponent(2,i); j < exponent(2,i+1)-1; j++) {
			pole[j] = abs(pole[j - rozdiel]-1);
			rozdiel+=2;
		}
	}
}


void napl_pole_suradnic(int *pole, int *pole_suradnic[], int *velkost_pola) {
	int chodec = 0;
	pole_suradnic[0][0] = 1; //pociatocne body
	pole_suradnic[0][1] = 1;

	pole_suradnic[1][0] = 1;
	pole_suradnic[1][1] = 0;

	for(int i = 2; i < *velkost_pola+2; i++) {
		if(chodec == 0) {
			pole_suradnic[i][0] = pole_suradnic[i-1][0]+1;
			pole_suradnic[i][1] = pole_suradnic[i-1][1];
		}
		if(chodec == 1) {
			pole_suradnic[i][0] = pole_suradnic[i-1][0];
			pole_suradnic[i][1] = pole_suradnic[i-1][1]-1;
		}
		if(chodec == 2) {
			pole_suradnic[i][0] = pole_suradnic[i-1][0]-1;
			pole_suradnic[i][1] = pole_suradnic[i-1][1];
		}
		if(chodec == 3) {
			pole_suradnic[i][0] = pole_suradnic[i-1][0];
			pole_suradnic[i][1] = pole_suradnic[i-1][1]+1;
		}
		
		if(pole[i-2] == 0)
			chodec--;
		else{chodec++;}

		if(chodec < 0)
			chodec = 3;
		else {
			if(chodec > 3)
				chodec = 0;
		}
	}
}



void maxima(int *pole_suradnic[], int *velkost_pola, int *posun, int *rozmer) {
	int min_x, min_y, max_x, max_y;
	for(int i = 0; i < *velkost_pola; i++) {
		if(i == 0) {
			min_x = pole_suradnic[i][0];
			min_y = pole_suradnic[i][1];

			max_x = pole_suradnic[i][0];
			max_y = pole_suradnic[i][1];
		}
		if(min_x > pole_suradnic[i][0])
			min_x = pole_suradnic[i][0];

		if(min_y > pole_suradnic[i][1])
			min_y = pole_suradnic[i][1];

		if(max_x < pole_suradnic[i][0])
			max_x = pole_suradnic[i][0];

		if(max_y < pole_suradnic[i][1])
			max_y = pole_suradnic[i][1];
	}
	int extremy[] = {min_x,min_y,max_x,max_y};
	int min_e, max_e;
	min_e = extremy[0];
	max_e = extremy[0];

	for(int i = 1; i < 4; i++) {
		if(min_e > extremy[i])
			min_e = extremy[i];
		if(max_e < extremy[i])
			max_e = extremy[i];
	}
	*posun = abs(min_e);
	*rozmer = max_e + (*posun);
}

void vpis_do_matice(int *pole_suradnic[], int *velkost_pola, int *posun, int *matica[]) {
	for(int i =0; i < *velkost_pola; i++) {
		int x = pole_suradnic[i][0] + (*posun) + 180;
		int y = pole_suradnic[i][1] + (*posun) - 270;
		matica[x][y] = 1;
	}
}

void zapis_do_suboru(int *matica[], int *rozmer) {
	//printf("%d\n", *rozmer);
	FILE *fw;
	fw = fopen("dragon.ppm","w");
	fprintf(fw, "%s\n", "P3");
	fprintf(fw, "%d %d\n", *rozmer,*rozmer);
	fprintf(fw, "%d\n%c\n%c\n%c", 255,'r','g','b');

	for(int i = 0; i < *rozmer; i++)
		for(int j = 0; j < *rozmer; j++)
			if(matica[i][j] == 0)
				fprintf(fw, "\n%d\n%d\n%d", 0,0,0);
			else{fprintf(fw, "\n%d\n%d\n%d", 255,255,255);}

	fclose(fw);
}


int main() {
	int iteracia, velkost_pola;
	scanf("%d", &iteracia);

	velkost_pola = exponent(2,iteracia)-1;

	int *pole = (int *) malloc(velkost_pola*sizeof(int));
	*pole = 0;

	int *pole_suradnic[velkost_pola];
	pole_suradnic[velkost_pola] = (int *) malloc((velkost_pola+2)*sizeof(int));
	for(int i =0; i < velkost_pola+2; i++)
		pole_suradnic[i] = (int *) malloc(2*sizeof(int));

	iteruj(pole, &iteracia);
	
	napl_pole_suradnic(pole, pole_suradnic, &velkost_pola);
	
	int posun, rozmer;
	maxima(pole_suradnic, &velkost_pola, &posun, &rozmer);

	int *matica[rozmer];
	matica[rozmer] = (int*) malloc(rozmer*sizeof(int));
	for(int i =0; i < rozmer; i++)
		matica[i] = (int *) malloc(rozmer*sizeof(int));

	vpis_do_matice(pole_suradnic, &velkost_pola, &posun, matica);
	zapis_do_suboru(matica, &rozmer);

	return 0;
}
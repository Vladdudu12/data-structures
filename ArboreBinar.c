#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<malloc.h>

typedef struct CursaFeroviara CursaFeroviara;
typedef struct Nod Nod;

struct CursaFeroviara {
	unsigned int nrCursa;
	char* dataCursa;
	unsigned char numarDeVagoane;
	unsigned short int nrBileteCumparate[2];
	float pretBilete[2];
};

struct Nod {
	CursaFeroviara info;
	Nod* dreapta;
	Nod* stanga;
};

CursaFeroviara initializareCursaFeroviara(unsigned int nrCursa, char* dataCursa, unsigned char numarDeVagoane, unsigned short int nrBileteCumparate[2], float pretBilete[2]) {
	CursaFeroviara cursaFeroviara;
	cursaFeroviara.nrCursa = nrCursa;
	cursaFeroviara.dataCursa = (char*)malloc(sizeof(char) * (strlen(dataCursa) + 1));
	strcpy(cursaFeroviara.dataCursa, dataCursa);
	cursaFeroviara.numarDeVagoane = numarDeVagoane;
	cursaFeroviara.nrBileteCumparate[0] = nrBileteCumparate[0];
	cursaFeroviara.nrBileteCumparate[1] = nrBileteCumparate[1];
	cursaFeroviara.pretBilete[0] = pretBilete[0];
	cursaFeroviara.pretBilete[1] = pretBilete[1];

	return cursaFeroviara;
}

void inserareInArbore(Nod** radacina, CursaFeroviara cf) {
	if (*radacina == NULL) {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = cf;
		nou->dreapta = NULL;
		nou->stanga = NULL;
		(*radacina) = nou;
	}
	else if((*radacina)->info.nrCursa > cf.nrCursa){
		inserareInArbore(&(*radacina)->stanga, cf);
	}
	else {
		inserareInArbore(&(*radacina)->dreapta, cf);
	}
}

void afisareCursaFeroviara(CursaFeroviara cursaFeroviara) {
	printf("\nCursa feroviara cu numarul %d, de la data de %s are %c vagoane.\nAu fost cumparate %d bilete la clasa 1 cu pretul %.2f.\nAu fost cumparate %d bilete la clasa a2a cu pretul %.2f.\n", cursaFeroviara.nrCursa, cursaFeroviara.dataCursa, cursaFeroviara.numarDeVagoane, cursaFeroviara.nrBileteCumparate[0], cursaFeroviara.pretBilete[0], cursaFeroviara.nrBileteCumparate[1], cursaFeroviara.pretBilete[1]);
}

void citireCursaFeroviaraDinFisier(FILE* file, Nod** radacina) {
	if (file == NULL) {
		return;
	}
	else {
		int totalCurse = 0;

		fscanf(file, "%d", &totalCurse);

		for (int i = 0; i < totalCurse; i++) {
			unsigned int nrCursa = 0;
			char* dataCursa;
			unsigned char numarDeVagoane;
			unsigned short int nrBileteCumparate[2];
			float pretBilete[2];

			fscanf(file, "%d", &nrCursa);
			char buffer[20];
			fscanf(file, "%s", buffer);
			dataCursa = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
			strcpy(dataCursa, buffer);
			fscanf(file, "%s", buffer);
			numarDeVagoane = buffer[0];
			fscanf(file, "%hu", &(nrBileteCumparate)[0]);
			fscanf(file, "%hu", &(nrBileteCumparate)[1]);
			fscanf(file, "%f", &(pretBilete)[0]);
			fscanf(file, "%f", &(pretBilete)[1]);

			CursaFeroviara cursaFeroviara = initializareCursaFeroviara(nrCursa, dataCursa, numarDeVagoane, nrBileteCumparate, pretBilete);
			
			inserareInArbore(&(*radacina), cursaFeroviara);
			afisareCursaFeroviara(cursaFeroviara);
		}

		fclose(file);
	}
}

int sumaBileteParcurgereArbore(Nod* radacina) {
	if (radacina) {
		int sumaStanga = sumaBileteParcurgereArbore(radacina->stanga);
		int sumaDreapta = sumaBileteParcurgereArbore(radacina->dreapta);
		int suma = 0;
		suma = sumaStanga + sumaDreapta  + radacina->info.nrBileteCumparate[1];
		return suma;
	}
	return 0;
}

void modificaPret(Nod** radacina, const char* dataCeruta, float procentDeModificare) {
	if (*radacina) {
		modificaPret(&(*radacina)->stanga, dataCeruta, procentDeModificare);
		modificaPret(&(*radacina)->dreapta, dataCeruta, procentDeModificare);
		if (strcmp((*radacina)->info.dataCursa, dataCeruta) == 0) {
			(*radacina)->info.pretBilete[0] = (*radacina)->info.pretBilete[0] + (*radacina)->info.pretBilete[0] * procentDeModificare;
		}
	}
}

void afisareInOrdine(Nod* radacina) {
	if (radacina) {
		afisareInOrdine(radacina->stanga);
		afisareCursaFeroviara(radacina->info);
		afisareInOrdine(radacina->dreapta);
	}
}

CursaFeroviara cursaCuIncasariMaxime(Nod* radacina) {
	CursaFeroviara cfMax = initializareCursaFeroviara(0, "-", 0, (short int[]) { 0, 0 }, (float[]) { 0, 0 });
	if (radacina) {
		CursaFeroviara cfMaxStanga = cursaCuIncasariMaxime(radacina->stanga);
		CursaFeroviara cfMaxDreapta = cursaCuIncasariMaxime(radacina->dreapta);
		if (radacina->info.nrBileteCumparate[0] * radacina->info.pretBilete[0] + radacina->info.nrBileteCumparate[1] * radacina->info.pretBilete[1] > cfMaxStanga.nrBileteCumparate[0] * cfMaxStanga.pretBilete[0] + cfMaxStanga.nrBileteCumparate[1] * cfMaxStanga.pretBilete[1] && radacina->info.nrBileteCumparate[0] * radacina->info.pretBilete[0] + radacina->info.nrBileteCumparate[1] * radacina->info.pretBilete[1] > cfMaxDreapta.nrBileteCumparate[0] * cfMaxDreapta.pretBilete[0] + cfMaxDreapta.nrBileteCumparate[1] * cfMaxDreapta.pretBilete[1]) {
			cfMax = radacina->info;
		}
		else if (cfMaxStanga.nrBileteCumparate[0] * cfMaxStanga.pretBilete[0] + cfMaxStanga.nrBileteCumparate[1] * cfMaxStanga.pretBilete[1] > radacina->info.nrBileteCumparate[0] * radacina->info.pretBilete[0] + radacina->info.nrBileteCumparate[1] * radacina->info.pretBilete[1] && cfMaxStanga.nrBileteCumparate[0] * cfMaxStanga.pretBilete[0] + cfMaxStanga.nrBileteCumparate[1] * cfMaxStanga.pretBilete[1] > cfMaxDreapta.nrBileteCumparate[0] * cfMaxDreapta.pretBilete[0] + cfMaxDreapta.nrBileteCumparate[1] * cfMaxDreapta.pretBilete[1]) {
			cfMax = cfMaxStanga;
		}
		else {
			cfMax = cfMaxDreapta;
		}
		return cfMax;
	}
	return cfMax;
}

int nrCurseFrunze(Nod* radacina) {
	if (radacina) {
		int ks = nrCurseFrunze(radacina->stanga);		
		int kd = nrCurseFrunze(radacina->dreapta);
		int k = 0;
		if (radacina->stanga == NULL && radacina->dreapta == NULL) {
			k++;
		}
		k = k + ks + kd;
		return k;
	}
	return 0;
}

void gasesteCurseFrunze(Nod* radacina, int** vector, int* i) {
	if (radacina) {
		gasesteCurseFrunze(radacina->stanga, &(*vector), i);
		gasesteCurseFrunze(radacina->dreapta, &(*vector), i);
		if (radacina->stanga == NULL && radacina->dreapta == NULL) {
			vector[*i] = radacina->info.nrCursa;
			(*i)++;
		}
	}
}

void dezalocare(Nod** radacina) {
	if (*radacina) {
		dezalocare(&(*radacina)->stanga);
		dezalocare(&(*radacina)->dreapta);
		free((*radacina)->info.dataCursa);
		free((*radacina));
	}
	*radacina = NULL;
}

void main() {
	//CursaFeroviara cf1 = initializareCursaFeroviara(1, "1 noiembrie", '3', (short int[]) { 5, 2 }, (float[]) { 55.2, 23.3 });
	//CursaFeroviara cf2 = initializareCursaFeroviara(2, "2 noiembrie", '6', (short int[]) { 8, 13 }, (float[]) { 55.2, 23.3 });
	//CursaFeroviara cf3 = initializareCursaFeroviara(3, "3 noiembrie", '8', (short int[]) { 11, 5 }, (float[]) { 55.2, 23.3 });
	//CursaFeroviara cf4 = initializareCursaFeroviara(4, "4 noiembrie", '2', (short int[]) { 12, 11 }, (float[]) { 55.2, 23.3 });
	//CursaFeroviara cf5 = initializareCursaFeroviara(5, "5 noiembrie", '3', (short int[]) { 17, 10 }, (float[]) { 55.2, 23.3 });

	Nod* radacina = NULL;

	//inserareInArbore(&radacina, cf3);
	//inserareInArbore(&radacina, cf2);
	//inserareInArbore(&radacina, cf1);
	//inserareInArbore(&radacina, cf4); 
	//inserareInArbore(&radacina, cf5);

	//afisareCursaFeroviara(cf1);

	FILE* file = fopen("CurseFeroviare.txt", "r");

	citireCursaFeroviaraDinFisier(file, &radacina);

	printf("\n\n%d", sumaBileteParcurgereArbore(radacina));

	const char* dataCeruta = "1/noiembrie";
	float procentDeModificare = 0.15;

	modificaPret(&radacina, dataCeruta, procentDeModificare);
	afisareInOrdine(radacina);

	afisareCursaFeroviara(cursaCuIncasariMaxime(radacina));
	
	int* vector;
	int k;
	int i = 0;
	
	printf("\n%d", nrCurseFrunze(radacina));
	k = nrCurseFrunze(radacina);

	vector = (int*)malloc(sizeof(int) * k);
	gasesteCurseFrunze(radacina, vector, &i);

	for (int j = 0; j < k; j++) {
		printf("\n%d", vector[j]);
	}

	printf("\n------------------------------DEZALOCARE----------------------------");

	dezalocare(&radacina);
	printf("\nS-a facut dezalocare.");

}
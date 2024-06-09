#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<stdio.h>
#include<malloc.h>
#include<string.h>

typedef struct Localitate Localitate;
typedef struct Nod Nod;
typedef struct LDI LDI;

struct Localitate {
	char* denumire;
	char* judet;
	unsigned int populatie;
	char* tipLocalitate;
	float bugetAlocat;
};

struct Nod {
	Localitate info;
	Nod* prev;
	Nod* next;
};

struct LDI {
	Nod* prim;
	Nod* ultim;
};

//Localitate inserareLocalitate(Nod** cap, Localitate info) {
//	if (cap == NULL) {
//		Nod* nou = (Nod*)malloc(sizeof(Nod));
//		nou->prev = NULL;
//		nou->next = NULL;
//		nou->info = info;
//		(*cap) = nou;
//	}
//	else {
//		Nod* nou = (Nod*)malloc(sizeof(Nod));
//		nou->prev = NULL;
//		nou->next = NULL;
//		nou->info = info;
//
//		Nod* aux = (Nod*)malloc(sizeof(Nod));
//		aux = cap;
//		while (aux->next) {
//			aux = aux->next;
//		}
//		aux->next = nou;
//		nou->prev = aux;
//	}
//}

Localitate initializareLocalitate(char* denumire, char* judet, unsigned int populatie, char* tipLocalitate, float bugetAlocat) {
	Localitate localitate;
	localitate.denumire = (char*)malloc(sizeof(char) * (strlen(denumire) + 1));
	localitate.judet = (char*)malloc(sizeof(char) * (strlen(judet) + 1));
	localitate.tipLocalitate = (char*)malloc(sizeof(char) * (strlen(tipLocalitate) + 1));
	strcpy(localitate.denumire, denumire);
	strcpy(localitate.judet, judet);
	localitate.populatie = populatie;
	strcpy(localitate.tipLocalitate, tipLocalitate);
	localitate.bugetAlocat = bugetAlocat;

	return localitate;
}

void afisareLocalitate(Localitate localitate) {
	printf("\nLocalitatea %s din judetul %s are o populatie de %d locuitori. Fiind de tipul %s, are un buget alocat de %.2f euro", localitate.denumire, localitate.judet, localitate.populatie, localitate.tipLocalitate, localitate.bugetAlocat);
}

void inserareLocalitateLaInceput(LDI* ldi, Localitate info) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->prev = NULL;
	nou->next = (*ldi).prim;
	nou->info = info;

	if ((*ldi).prim == NULL && (*ldi).ultim ==	NULL) {
		(*ldi).prim = (*ldi).ultim = nou;
	}
	else {
		(*ldi).prim->prev = nou;
		(*ldi).prim = nou;
	}
}

void inserareLocalitateLaFinal(LDI* ldi, Localitate	info) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->prev = ldi->ultim;
	nou->next = NULL;
	nou->info = info;

	if (ldi == NULL) {
		ldi->prim = ldi->ultim = nou;
	}
	else {
		ldi->ultim->next = nou;
		ldi->ultim = nou;
	}
}

void citireDinFisier(FILE* file, LDI* ldi) {
	if (file == NULL) {
		return;
	}
	else {
		int nrLocalitati = 0;

		char* denumire;
		char* judet;
		unsigned int populatie = 0;
		char* tipLocalitate;
		float bugetAlocat = 0;

		fscanf(file, "%d", &nrLocalitati);
		for (int i = 0; i < nrLocalitati; i++) {
			char buffer[25];
			fscanf(file, "%s", buffer);
			denumire = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
			strcpy(denumire, buffer);
			fscanf(file, "%s", buffer);
			judet = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
			strcpy(judet, buffer);
			fscanf(file, "%u", &populatie);
			fscanf(file, "%s", buffer);
			tipLocalitate = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
			strcpy(tipLocalitate, buffer);
			fscanf(file, "%f", &bugetAlocat);

			Localitate localitate = initializareLocalitate(denumire, judet, populatie, tipLocalitate, bugetAlocat);
			inserareLocalitateLaInceput(ldi, localitate);
			//afisareLocalitate(localitate);
		}
	}
}

int calculPopulatiaTotala(LDI ldi, char* tipLocalitate) {
	int populatieTotala = 0;
	while (ldi.prim) {
		if (strcmp(ldi.prim->info.tipLocalitate, tipLocalitate) == 0) {
			populatieTotala = populatieTotala + ldi.prim->info.populatie;
		}
		ldi.prim = ldi.prim->next;
	}
	return populatieTotala;
}

void actualizarePopulatie(LDI* ldi, char* judetDat, float procentModificare) {
	Nod* aux = ldi->prim;
	while (aux) {
		if (strcmp(aux->info.judet, judetDat) == 0) {
			aux->info.populatie = aux->info.populatie + aux->info.populatie * procentModificare;
		}
		aux = aux->next;
	}
}

void afisareLista(LDI* ldi) {
	Nod* aux = (*ldi).prim;
	while (aux) {
		afisareLocalitate(aux->info);
		aux = aux->next;
	}
}

float calculMediePopulatie(LDI ldi) {
	float suma = 0;
	int contor = 0;
	while (ldi.prim) {
		suma = suma + ldi.prim->info.populatie;
		contor++;
		ldi.prim = ldi.prim->next;
	}
	suma = suma / contor;
	return suma;
}

void eliminaNodSubMedie(LDI* ldi, float mediePopulatie) {
	Nod* aux = (*ldi).prim;
	while (aux) {
		if (aux->info.populatie < mediePopulatie) {
			Nod* dreapta = aux->next;
			Nod* stanga = aux->prev;

			if (dreapta != NULL) {
				dreapta->prev = stanga;
			}
			else {
				(*ldi).ultim = stanga;
			}

			if (stanga != NULL) {
				stanga->next = dreapta;
			}
			else {
				(*ldi).prim = dreapta;
			}

			Nod* aux2 = aux;
			aux = aux->next;

			free(aux2->info.denumire);
			free(aux2->info.judet);
			free(aux2->info.tipLocalitate);
			free(aux2);
		}
		else {
			aux = aux->next;
		}
	}
}

void calculVectorLocalitati(LDI ldi, Localitate** vector, float mediePopulatie, int* k) {
	Nod* aux = ldi.prim;
	while (aux) {
		if (aux->info.populatie > mediePopulatie) {
			(*k)++;
		}
		aux = aux->next;
	}

	*vector = (Localitate*)malloc(sizeof(Localitate) * (*k));
	int i = 0;
	aux = ldi.prim;

	while (aux) {
		if (aux->info.populatie > mediePopulatie) {
			Localitate nou = initializareLocalitate(aux->info.denumire, aux->info.judet, aux->info.populatie, aux->info.tipLocalitate, aux->info.bugetAlocat);
			(*vector)[i] = nou;
			i++;
		}
		aux = aux->next;
	}
}

void dezalocare(LDI* ldi) {
	while ((*ldi).ultim) {
		Nod* aux = (*ldi).ultim;
		(*ldi).ultim = (*ldi).ultim->prev;
		free(aux->info.denumire);
		free(aux->info.judet);
		free(aux->info.tipLocalitate);
		free(aux);
	}

}

void main() {

	//Localitate loc1 = initializareLocalitate("Buhusi", "Bacau", 10000, "sat", 13000);
	//Localitate loc2 = initializareLocalitate("Bucuresti", "Ilfov", 123456, "municipiu", 99999);
	//Localitate loc3 = initializareLocalitate("Pitesti", "Pitesti", 22000, "sat", 50505);
	//Localitate loc4 = initializareLocalitate("Chiajna", "Ilfov", 10000, "comuna", 15000);
	//Localitate loc5 = initializareLocalitate("Ciliemi", "Dambovita", 2000, "oras", 1654);

	//afisareLocalitate(loc1);
	//afisareLocalitate(loc2);
	//afisareLocalitate(loc3);
	//afisareLocalitate(loc4);
	//afisareLocalitate(loc5);

	LDI ldi;
	ldi.prim = NULL;
	ldi.ultim = NULL;

	FILE* file = fopen("Localitati.txt", "r");

	citireDinFisier(file, &ldi);
	//afisareLocalitate(ldi.prim->info);
	//afisareLocalitate(ldi.ultim->info);
	char* tipLocalitate = "sat";
	int populatieTotala = calculPopulatiaTotala(ldi, tipLocalitate);
	printf("\n%d", populatieTotala);

	char* judetDat = "Ilfov";
	float procentModificare = 0.20;

	//actualizarePopulatie(&ldi, judetDat, procentModificare);
	//afisareLista(ldi);

	float mediePopulatie = calculMediePopulatie(ldi);
	printf("\n%.2f", mediePopulatie);

	//eliminaNodSubMedie(&ldi, mediePopulatie);
	//afisareLista(&ldi);

	Localitate* vector;
	int k = 0;
	calculVectorLocalitati(ldi, &vector, mediePopulatie, &k);

	for (int i = 0; i < k; i++) {
		afisareLocalitate(vector[i]);
	}

	dezalocare(&ldi);
	printf("\nS-a facut dezalocare!");
}
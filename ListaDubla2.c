#define _CRT_SECURE_NO_WARNINGS
#include<malloc.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

typedef struct Proiect Proiect;
typedef struct Nod Nod;
typedef struct LDI LDI;

struct Proiect {
	unsigned int codProiect;
	char* titluProiect;
	char* beneficiar;
	unsigned char numarExecutanti;
	float bugetAlocat;
};

struct Nod {
	Proiect info;
	Nod* prev;
	Nod* next;
};

struct LDI {
	Nod* prim;
	Nod* ultim;
};

Proiect initializareProiect(unsigned int codProiect, char* titluProiect, char* beneficiar, unsigned char numarExecutanti, float bugetAlocat) {
	Proiect proiect;
	proiect.titluProiect = (char*)malloc(sizeof(char) * (strlen(titluProiect) + 1));
	proiect.beneficiar = (char*)malloc(sizeof(char) * (strlen(beneficiar) + 1));
	proiect.codProiect = codProiect;
	strcpy(proiect.titluProiect, titluProiect);
	strcpy(proiect.beneficiar, beneficiar);
	proiect.numarExecutanti = numarExecutanti;
	proiect.bugetAlocat = bugetAlocat;

	return proiect;
}

void afisareProiect(Proiect proiect) {
	printf("\nProiectul de investitii %s cu codul %u, are beneficiarul %s.\nAcesta are %c executanti si un buget alocat de %.2f euro.\n", proiect.titluProiect, proiect.codProiect, proiect.beneficiar, proiect.numarExecutanti, proiect.bugetAlocat);
}

void inserareProiectLaFinal(LDI* ldi, Proiect proiect) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = proiect;
	nou->next = NULL;
	nou->prev = (*ldi).ultim;

	if ((*ldi).prim == NULL && (*ldi).ultim == NULL) {
		(*ldi).prim = (*ldi).ultim = nou;
	}
	else {
		(*ldi).ultim->next = nou;
		(*ldi).ultim = nou;
	}
}

void inserareProiectLaInceput(LDI* ldi, Proiect proiect) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = proiect;
	nou->next = (*ldi).prim;
	nou->prev = NULL;

	if ((*ldi).prim == NULL && (*ldi).ultim == NULL) {
		(*ldi).prim = (*ldi).ultim = nou;
	}
	else {
		(*ldi).prim->prev = nou;
		(*ldi).prim = nou;
	}
}

void afisareListaProiecte(LDI* ldi) {
	Nod* aux = (*ldi).prim;
	while (aux) {
		afisareProiect(aux->info);
		aux = aux->next;
	}
}

void afisareListaProiecteDeLaFinal(LDI* ldi) {
	Nod* aux = (*ldi).ultim;
	while (aux) {
		afisareProiect(aux->info);
		aux = aux->prev;
	}
}

void citireDinFisier(FILE* file, LDI* ldi) {
	char buffer[20];
	while (fgets(buffer, 5, file) != NULL) {
		Proiect proiect;
		proiect.codProiect = atoi(buffer);

		fgets(buffer, 20, file);
		char* sir = strtok(buffer, "\n");
		proiect.titluProiect = (char*)malloc(sizeof(char) * (strlen(sir) + 1));
		strcpy(proiect.titluProiect, sir);

		fgets(buffer, 20, file);
		char* sir2 = strtok(buffer, "\n");
		proiect.beneficiar = (char*)malloc(sizeof(char) * (strlen(sir2) + 1));
		strcpy(proiect.beneficiar, sir2);

		fgets(buffer, 4, file);
		proiect.numarExecutanti = buffer[0];

		fgets(buffer, 8, file);
		proiect.bugetAlocat = atof(buffer);

		inserareProiectLaInceput(ldi, proiect);
		afisareProiect(proiect);
	}
}

int calculeazaNrProiecte(LDI* ldi, unsigned char nrExecutanti) {
	int k = 0;
	Nod* aux = (*ldi).prim;
	while (aux) {
		if (aux->info.numarExecutanti > nrExecutanti) {
			k++;
		}
		aux = aux->next;
	}
	return k;
}

void sortareListaDupaBuget(LDI* ldi) {
	if (ldi->prim == NULL) {
		return;
	}

	Nod* i, * j;
	for (i = ldi->prim; i->next != NULL; i = i->next) {
		for (j = i->next; j != NULL; j = j->next) {
			if (i->info.bugetAlocat > j->info.bugetAlocat) {
				Proiect aux = i->info;
				i->info = j->info;
				j->info = aux;
			}
		}
	}
}

void actualizeazaBuget(LDI* ldi, char* beneficiar, float procent) {
	Nod* aux = (*ldi).prim;
	while (aux) {
		if (strcmp(aux->info.beneficiar, beneficiar) == 0) {
			aux->info.bugetAlocat = aux->info.bugetAlocat + aux->info.bugetAlocat * procent;
		}
		aux = aux->next;
	}
}

void main() {
	//Proiect p1 = initializareProiect(1, "a b", "ba", '5', 5000.2);

	//afisareProiect(p1);

	printf("\n\n-----------------------------SUBIECT 1--------------------------------");
	LDI ldi;
	ldi.prim = NULL;
	ldi.ultim = NULL;
	FILE* file = fopen("Proiecte.txt", "r");
	citireDinFisier(file, &ldi);

	printf("\n\n-----------------------------SUBIECT 2--------------------------------");
	unsigned char nrExecutanti = '3';
	int nrProiecte = calculeazaNrProiecte(&ldi, nrExecutanti);
	printf("\n%d", nrProiecte);

	printf("\n\n-----------------------------SUBIECT 3--------------------------------");
	char* beneficiar = "dc";
	float procent = 0.2;
	actualizeazaBuget(&ldi, beneficiar, procent);
	afisareListaProiecte(&ldi);

	printf("\n\n-----------------------------SUBIECT 4--------------------------------");
	sortareListaDupaBuget(&ldi);
	printf("\nLista sortata dupa buget (crescator):\n");
	afisareListaProiecte(&ldi);
	printf("\nLista sortata dupa buget (de la final):\n");
	afisareListaProiecteDeLaFinal(&ldi);

}
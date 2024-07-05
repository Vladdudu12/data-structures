#define _CRT_SECURE_NO_WARNINGS
#include<string.h>
#include<malloc.h>
#include<stdlib.h>
#include<stdio.h>

typedef struct Comanda Comanda;
typedef struct Nod Nod;
typedef struct TabelaDispersie TabelaDispersie;

struct Comanda {
	unsigned int idComanda;
	char* dataLansare;
	char* datalivrare;
	char* numeClient;
	float sumaPlata;
};

struct Nod {
	Comanda info;
	Nod* next;
};

struct TabelaDispersie {
	int dimensiune;
	Nod** vector;
};

void afisare(Comanda comanda) {
	printf("\nComanda cu id-ul %u a fost inregistrata pe %s si livrarea este programata in data de %s.", comanda.idComanda, comanda.dataLansare, comanda.datalivrare);
	printf("\nNumele clientului este %s si are de platit %.2f lei.\n", comanda.numeClient, comanda.sumaPlata);
}

void inserareaLaSfarsitInlIsta(Nod** cap, Comanda comanda) {
	if ((*cap) == NULL) {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = comanda;
		nou->next = NULL;

		(*cap) = nou;
	}
	else {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = comanda;
		nou->next = NULL;

		Nod* aux = (*cap);
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nou;
	}
}

TabelaDispersie initializareTabelaDispersie(int dimensiune) {
	TabelaDispersie tabela;
	tabela.dimensiune = dimensiune;
	tabela.vector = (Nod**)malloc(sizeof(Nod*) * dimensiune);
	for (int i = 0; i < dimensiune; i++) {
		tabela.vector[i] = NULL;
	}
	return tabela;
}

int hash(char* numeClient, int dimensiune) {
	return strlen(numeClient) % dimensiune;
}

void adaugareInTabelaDeDispersie(TabelaDispersie tabela, Comanda comanda) {
	if (tabela.dimensiune > 0) {
		int pozitie = hash(comanda.numeClient, tabela.dimensiune);
		if (pozitie < tabela.dimensiune && pozitie >= 0) {
			inserareaLaSfarsitInlIsta(&(tabela.vector[pozitie]), comanda);
		}
	}
}

parcurgereLista(Nod* cap) {
	Nod* aux = cap;
	while (aux) {
		afisare(aux->info);
		aux = aux->next;
	}
}

parcurgereTD(TabelaDispersie tabela) {
	for (int i = 0; i < tabela.dimensiune; i++) {
		parcurgereLista(tabela.vector[i]);
	}
}

void citireDinFisier(FILE* file, TabelaDispersie* tabela) {
	if (file == NULL) {
		return;
	}
	else {
		char buffer[20];
		while (fgets(buffer, 5, file)) {
			Comanda comanda;
			comanda.idComanda = atoi(buffer);

			fgets(buffer, 20, file);
			char* sir = strtok(buffer, "\n");
			comanda.dataLansare = (char*)malloc(sizeof(char) * (strlen(sir) + 1));
			strcpy(comanda.dataLansare, sir);

			fgets(buffer, 20, file);
			char* sir2 = strtok(buffer, "\n");
			comanda.datalivrare = (char*)malloc(sizeof(char) * (strlen(sir2) + 1));
			strcpy(comanda.datalivrare, sir2);

			fgets(buffer, 20, file);
			char* sir3 = strtok(buffer, "\n");
			comanda.numeClient = (char*)malloc(sizeof(char) * (strlen(sir3) + 1));
			strcpy(comanda.numeClient, sir3);

			fgets(buffer, 10, file);
			comanda.sumaPlata = atof(buffer);

			adaugareInTabelaDeDispersie((*tabela), comanda);
			afisare(comanda);
		}
	}
}

int nrComenziAcelasiClient(TabelaDispersie tabela, char* numeClient) {
	int nrComenzi = 0;
	for (int i = 0; i < tabela.dimensiune; i++) {
		Nod* aux = tabela.vector[i];
		while (aux) {
			if (strcmp(tabela.vector[i]->info.numeClient, numeClient) == 0) {
				nrComenzi++;
			}
			aux = aux->next;
		}
	}
	return nrComenzi;
}

void main() {

	printf("\n\n---------------------------------------SUBIECT 1----------------------------------\n");
	Nod* cap = NULL;
	TabelaDispersie td1 = initializareTabelaDispersie(2);

	FILE* file = fopen("comenzi.txt", "r");

	citireDinFisier(file, &td1);

	printf("\n\n---------------------------------------SUBIECT 2----------------------------------\n");
	char* numeClient = "marin mar";
	int nrComenzi = nrComenziAcelasiClient(td1, numeClient);
	printf("\n\n%d", nrComenzi);
}
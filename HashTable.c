#define _CRT_SECURE_NO_WARNINGS		
#include<stdlib.h>
#include<stdio.h>
#include<malloc.h>
#include<string.h>

typedef struct Comanda Comanda;
typedef struct Nod Nod;
typedef struct TabelaDispersie TabelaDispersie;

struct Comanda {
	unsigned int idComanda;
	char* dataLansareComanda;
	char* dataLivrareProgramata;
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

Comanda initializareComanda(unsigned int idComanda, char* dataLansareComanda, char* dataLivrareProgramata, char* numeClient, float sumaPlata) {
	Comanda comanda;
	comanda.idComanda = idComanda;
	comanda.dataLansareComanda = (char*)malloc(sizeof(char) * (strlen(dataLansareComanda) + 1));
	comanda.dataLivrareProgramata = (char*)malloc(sizeof(char) * (strlen(dataLivrareProgramata) + 1));
	comanda.numeClient = (char*)malloc(sizeof(char) * (strlen(numeClient) + 1));
	strcpy(comanda.dataLansareComanda,dataLansareComanda);
	strcpy(comanda.dataLivrareProgramata,dataLivrareProgramata);
	strcpy(comanda.numeClient, numeClient);
	comanda.sumaPlata = sumaPlata;

	return comanda;
}

void afisareComanda(Comanda comanda) {
	printf("\nComanda cu id-ul %u a fost inregistrata pe %s si livrarea este programata in data de %s.", comanda.idComanda, comanda.dataLansareComanda, comanda.dataLivrareProgramata);
	printf("\nNumele clientului este %s si are de platit %.2f lei.\n", comanda.numeClient, comanda.sumaPlata);
}

void inserareLaSfarsitInLista(Nod** cap, Comanda comanda) {
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
		while (aux) {
			aux = aux->next;
		}
		aux = nou;
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

void adaugareinTabelaDispersie(TabelaDispersie tabela, Comanda comanda) {
	if (tabela.dimensiune > 0) {
		int pozitie = hash(comanda.numeClient, tabela.dimensiune);
		if (pozitie < tabela.dimensiune && pozitie >= 0) {
			inserareLaSfarsitInLista(&(tabela.vector[pozitie]), comanda);
		}

	}
}

void parcurgereLista(Nod* cap) {
	while (cap) {
		afisareComanda(cap->info);
		cap = cap->next;
	}
}

void parcurgereTD(TabelaDispersie tabela) {
	for (int i = 0; i < tabela.dimensiune; i++) {
		parcurgereLista(tabela.vector[i]);
	}
}

void citireDinFisier(FILE* file, TabelaDispersie* tabela) {
	if (file == NULL) {
		return;
	}
	else {
		int totalComenzi = 0;
		unsigned int idComanda = 0;
		char* dataLansareComanda;
		char* dataLivrareProgramata;
		char* numeClient;
		float sumaPlata = 0;

		fscanf(file, "%d", &totalComenzi);
		*tabela = initializareTabelaDispersie(totalComenzi);
		for (int i = 0; i < totalComenzi; i++) {
			fscanf(file, "%u", &idComanda);
			char buffer[30];
			fscanf(file, "%s", buffer);
			dataLansareComanda = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
			strcpy(dataLansareComanda, buffer);
			fscanf(file, "%s", buffer);
			dataLivrareProgramata = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
			strcpy(dataLivrareProgramata, buffer);
			fscanf(file, "%s", buffer);
			numeClient = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
			strcpy(numeClient, buffer);
			fscanf(file, "%f", &sumaPlata);

			Comanda comanda = initializareComanda(idComanda, dataLansareComanda, dataLivrareProgramata, numeClient, sumaPlata);
			
			adaugareinTabelaDispersie(*tabela, comanda);
			//afisareComanda(comanda);
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

void modificaDataDeLivrare(TabelaDispersie tabela, unsigned int idComanda, char* dataNouaDeLivrare) {
	for (int i = 0; i < tabela.dimensiune; i++) {
		Nod* aux = tabela.vector[i];
		while (aux) {
			if (aux->info.idComanda == idComanda) {
				free(aux->info.dataLivrareProgramata);
				aux->info.dataLivrareProgramata = (char*)malloc(sizeof(char) + (strlen(dataNouaDeLivrare) + 1));
				strcpy(aux->info.dataLivrareProgramata, dataNouaDeLivrare);
				afisareComanda(aux->info);
			}
			aux = aux->next;
		}
	}
}

void stergereComenziDinTabela(TabelaDispersie tabela, const char* dataCurenta) {
	for (int i = 0; i < tabela.dimensiune; i++) {
		Nod* aux = tabela.vector[i];
		Nod* prev = NULL;
		while (aux) {
			if (strcmp(aux->info.dataLivrareProgramata, dataCurenta) < 0) {
				if (prev == NULL) {
					// Removing the head of the list
					tabela.vector[i] = aux->next;
				}
				else {
					// Removing a non-head node
					prev->next = aux->next;
				}
				Nod* toDelete = aux;
				aux = aux->next;

				// Free memory of the toDelete node
				free(toDelete->info.dataLansareComanda);
				free(toDelete->info.dataLivrareProgramata);
				free(toDelete->info.numeClient);
				free(toDelete);
			}
			else {
				prev = aux;
				aux = aux->next;
			}
		}
	}
}

void salveazaInLista(TabelaDispersie tabela, float sumaSpecificata, Nod** cap) {
	for (int i = 0; i < tabela.dimensiune; i++) {
		Nod* aux = tabela.vector[i];
		while (aux) {
			if (aux->info.sumaPlata >= sumaSpecificata) {
				inserareLaSfarsitInLista(&(*cap), aux->info);
			}
			aux = aux->next;
		}
	}
}

void dezalocare(TabelaDispersie tabela) {
	for (int i = 0; i < tabela.dimensiune; i++) {
		while (tabela.vector[i]) {
			Nod* aux = tabela.vector[i];
			tabela.vector[i] = tabela.vector[i]->next;
			free(aux->info.dataLansareComanda);
			free(aux->info.dataLivrareProgramata);
			free(aux->info.numeClient);
			free(aux);
		}
	}
}

void main() {

	//Comanda c1 = initializareComanda(1, "1/11/2023", "5/11/2023", "Ion Ionel", 300);
	//Comanda c2 = initializareComanda(2, "2/11/2023", "6/11/2023", "Maria Mariana Pop-Popescu", 150);

	//afisareComanda(c1);

	Nod* cap = NULL;
	TabelaDispersie td1 = initializareTabelaDispersie(2);

	//adaugareinTabelaDispersie(td1, c1);
	//adaugareinTabelaDispersie(td1, c2);

	//parcurgereTD(td1);

	FILE* file = fopen("dateDeIntrare.txt", "r");

	citireDinFisier(file, &td1);

	printf("\n-------------------------SUBIECT 2---------------");
	char* numeClient = "Ion-Ionel";
	int nrComenzi = nrComenziAcelasiClient(td1, numeClient);
	printf("\n\n%d", nrComenzi);

	char* dataNouaDeLivrare = "10/06/2024";

	printf("\n-------------------------SUBIECT 3---------------");
	modificaDataDeLivrare(td1, 1, dataNouaDeLivrare);
	
	printf("\n-------------------------SUBIECT 4---------------");
	stergereComenziDinTabela(td1, "09/06/2024");

	parcurgereTD(td1);

	printf("\n-------------------------SUBIECT 5---------------");

	salveazaInLista(td1, 700, &cap);
	parcurgereLista(cap);

	printf("\n-------------------------DEZALOCARE---------------");
	dezalocare(td1);
	printf("\nS-a facut dezalozare!");

}
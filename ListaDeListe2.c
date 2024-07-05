#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<stdio.h>
#include<malloc.h>
#include<string.h>

typedef struct Consultatie Consultatie;
typedef struct NodLS NodLS;
typedef struct NodLP NodLP;


struct Consultatie {
	char* dataConsultatie;
	char* numeMedic;
	char* specialitate;
	char* diagnostic;
	int pretConsultatie;
};

struct NodLS {
	Consultatie info;
	NodLS* next;
};

struct NodLP {
	NodLS* lista;
	NodLP* next;
};

Consultatie initializareConsultatie(char* dataConsultatie, char* numeMedic, char* specialitate, char* diagnostic, int pretConsultatie) {
	Consultatie consultatie;
	consultatie.dataConsultatie = (char*)malloc(sizeof(char) * (strlen(dataConsultatie) + 1));
	consultatie.numeMedic = (char*)malloc(sizeof(char) * (strlen(numeMedic) + 1));
	consultatie.specialitate = (char*)malloc(sizeof(char) * (strlen(specialitate) + 1));
	consultatie.diagnostic = (char*)malloc(sizeof(char) * (strlen(diagnostic) + 1));
	strcpy(consultatie.dataConsultatie, dataConsultatie);
	strcpy(consultatie.numeMedic, numeMedic);
	strcpy(consultatie.specialitate, specialitate);
	strcpy(consultatie.diagnostic, diagnostic);
	consultatie.pretConsultatie = pretConsultatie;

	return consultatie;
}

void afisareConsultatie(Consultatie consultatie) {
	printf("\nConsultatia de la data %s a fost sustinuta de medicul %s de specialitate %s.", consultatie.dataConsultatie, consultatie.numeMedic, consultatie.specialitate);
	printf("\nDiagnosticul pacientului este %s, iar pretul consultatiei a fost de %d de lei.\n", consultatie.diagnostic, consultatie.pretConsultatie);
}

void inserareInListaLaFinal(NodLS** cap, Consultatie info) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	nou->info = info;
	nou->next = NULL;

	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		NodLS* aux = *cap;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nou;
	}
}

void parcurgereLista(NodLS* cap) {
	NodLS* aux = cap;
	while (aux) {
		afisareConsultatie(aux->info);
		aux = aux->next;
	}
}

void inserareInListDeListe(NodLP** cap, Consultatie consultatieDeAdaugat) {
	if (*cap == NULL) {
		NodLP* nou = (NodLP*)malloc(sizeof(NodLP));
		nou->lista = NULL;
		inserareInListaLaFinal(&(nou->lista), consultatieDeAdaugat);
		nou->next = NULL;

		*cap = nou;
	}
	else {
		NodLP* aux = *cap;
		while (aux->next) {
			if (strcmp(aux->lista->info.specialitate, consultatieDeAdaugat.specialitate) == 0) {
				inserareInListaLaFinal(&(aux->lista), consultatieDeAdaugat);
				return;
			}
			aux = aux->next;
		}
		NodLP* nou = (NodLP*)malloc(sizeof(NodLP));
		nou->lista = NULL;
		inserareInListaLaFinal(&(nou->lista), consultatieDeAdaugat);
		nou->next = NULL;

		aux->next = nou;
	}
}

void parcurgereNodLP(NodLP* cap) {
	NodLP* aux = cap;
	while (aux) {
		parcurgereLista(aux->lista);
		aux = aux->next;
	}
}

void citireDinFisier(FILE* file, NodLP** lista) {
	char buffer[20];
	while (fgets(buffer, 20, file) != NULL) {
		Consultatie consultatie;
		char* sir = strtok(buffer, "\n");
		consultatie.dataConsultatie = (char*)malloc(sizeof(char) * (strlen(sir) + 1));
		strcpy(consultatie.dataConsultatie, sir);

		fgets(buffer, 20, file);
		char* sir2 = strtok(buffer, "\n");
		consultatie.numeMedic = (char*)malloc(sizeof(char) * (strlen(sir2) + 1));
		strcpy(consultatie.numeMedic, sir2);

		fgets(buffer, 20, file);
		char* sir3 = strtok(buffer, "\n");
		consultatie.specialitate = (char*)malloc(sizeof(char) * (strlen(sir3) + 1));
		strcpy(consultatie.specialitate, sir3);

		fgets(buffer, 20, file);
		char* sir4 = strtok(buffer, "\n");
		consultatie.diagnostic = (char*)malloc(sizeof(char) * (strlen(sir4) + 1));
		strcpy(consultatie.diagnostic, sir4);

		fgets(buffer, 5, file);
		consultatie.pretConsultatie = atoi(buffer);

		inserareInListDeListe(&(*lista), consultatie);
	}
}

int valoareTotalaSpecialitate(NodLP* lista, char* specialitate) {
	int suma = 0;
	if (lista == NULL) {
		return suma;
	}
	else {
		NodLP* aux = lista;
		while (aux) {
			if (strcmp(aux->lista->info.specialitate, specialitate) == 0) {
				NodLS* aux2 = aux->lista;
				while (aux2) {
					suma = suma + aux2->info.pretConsultatie;
					aux2 = aux2->next;
				}
			}
			aux = aux->next;
		}
	}
	return suma;
}

void modificaPretConsultatie(NodLP** lista, char* dataConsultatie, int pretNou) {
	if ((*lista) == NULL) {
		return;
	}
	else {
		NodLP* aux = *lista;
		while (aux) {
			NodLS* aux2 = aux->lista;
			while (aux2) {
				if (strcmp(aux2->info.dataConsultatie, dataConsultatie) == 0) {
					aux2->info.pretConsultatie = pretNou;
				}
				aux2 = aux2->next;
			}
			aux = aux->next;
		}
	}
}

void stergeConsultatieDinLS(NodLS** nod, char* diagnostic) {
	if (*nod == NULL) {
		return;
	}
	else {
		NodLS* aux = *nod;
		NodLS* prev = NULL;
		while (aux->next) {
			if (strcmp((*nod)->info.diagnostic, diagnostic) == 0) {
				if (prev == NULL) {
					*nod = aux->next;
				}
				else {
					prev->next = aux->next;
				}
				NodLS* nodDeSters = aux;
				aux = aux->next;

				free(nodDeSters->info.dataConsultatie);
				free(nodDeSters->info.diagnostic);
				free(nodDeSters->info.numeMedic);
				free(nodDeSters->info.specialitate);
			}
			else {
				prev = aux;
				aux = aux->next;
			}
		}
	}
}

void stergereDinLP(NodLP** lista, char* diagnostic) {
	if (*lista == NULL) {
		return;
	}
	else {
		NodLP* aux = *lista;
		while (aux->next) {
			stergeConsultatieDinLS(&(aux->lista), diagnostic);
			aux = aux->next;
		}
	}
}

Consultatie* creareVectorConsultatii(NodLP* lista, float pretDat, int* k) {
	if (lista == NULL) {
		return;
	}
	else {
		NodLP* aux = lista;
		while (aux) {
			if (aux->lista->info.pretConsultatie > pretDat) {
				(*k)++;
			}
			aux = aux->next;
		}

		aux = lista;
		Consultatie* vector = (Consultatie*)malloc(sizeof(Consultatie) * (*k));
		int contor = 0;

		while (aux) {
			if (aux->lista->info.pretConsultatie > pretDat) {
				vector[contor] = aux->lista->info;
				contor++;
			}
			aux = aux->next;
		}
		return vector;
	}
}

void dezalocareLS(NodLS** nod) {
	while ((*nod)) {
		NodLS* aux = *nod;
		(*nod) = (*nod)->next;
		free(aux->info.dataConsultatie);
		free(aux->info.diagnostic);
		free(aux->info.numeMedic);
		free(aux->info.specialitate);
		free(aux);
	}
}

void dezalocareLP(NodLP** lista) {
	while ((*lista)) {
		NodLP* aux = *lista;
		*lista = (*lista)->next;
		dezalocareLS(&(aux->lista));
		free(aux);
	}
}

void main() {

	printf("\n---------------------------------SUBIECT 1-----------------------------------\n");
	NodLS* capNodLS = NULL;
	NodLP* capNodLP = NULL;
	FILE* file = fopen("dateDeIntrare.txt", "r");
	citireDinFisier(file, &capNodLP);
	parcurgereNodLP(capNodLP);

	printf("\n---------------------------------SUBIECT 2-----------------------------------\n");
	char* specialitate = "dermato";
	int suma = valoareTotalaSpecialitate(capNodLP, specialitate);
	printf("\n%d", suma);

	printf("\n---------------------------------SUBIECT 3-----------------------------------\n");
	char* dataConsultatie = "11/11/2011";
	int pretNou = 350;
	modificaPretConsultatie(&capNodLP, dataConsultatie, pretNou);
	parcurgereNodLP(capNodLP);

	printf("\n---------------------------------SUBIECT 4-----------------------------------\n");
	char* diagnostic = "rau";
	stergereDinLP(&capNodLP, diagnostic);
	parcurgereNodLP(capNodLP);

	printf("\n---------------------------------SUBIECT 5-----------------------------------\n");
	int k = 0;
	Consultatie* vector;

	vector = creareVectorConsultatii(capNodLP, 460, &k);
	for (int i = 0; i < k; i++) {
		afisareConsultatie(vector[i]);
	}

	printf("\n---------------------------------DEZALOCARE-----------------------------------\n");

	dezalocareLP(&capNodLP);

	printf("\nS-a facut dezalocare");
	parcurgereNodLP(capNodLP);
}
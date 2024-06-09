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
	strcpy(consultatie.dataConsultatie, dataConsultatie);
	consultatie.numeMedic = (char*)malloc(sizeof(char) * (strlen(numeMedic) + 1));
	strcpy(consultatie.numeMedic, numeMedic);
	consultatie.specialitate = (char*)malloc(sizeof(char) * (strlen(specialitate) + 1));
	strcpy(consultatie.specialitate, specialitate);
	consultatie.diagnostic = (char*)malloc(sizeof(char) * (strlen(diagnostic) + 1));
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

void inserareInListaDeListe(NodLP** cap, Consultatie consultatieDeAdaugat) {
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

void citireDinFisier(FILE* file, NodLP** listaDinFisier) {
	if (file == NULL) {
		return;
	}
	else {
		int nrConsultatii = 0;
		char* dataConsultatie;
		char* numeMedic;
		char* specialitate;
		char* diagnostic;
		int pretConsultatie = 0;

		fscanf(file, "%d", &nrConsultatii);

		for (int i = 0; i < nrConsultatii; i++) {
			char buffer[30];
			fscanf(file, "%s", buffer);
			dataConsultatie = (NodLP**)malloc(sizeof(NodLP*) * (strlen(buffer) + 1));
			strcpy(dataConsultatie, buffer);

			fscanf(file, "%s", buffer);
			numeMedic = (NodLP**)malloc(sizeof(NodLP*) * (strlen(buffer) + 1));
			strcpy(numeMedic, buffer);

			fscanf(file, "%s", buffer);
			specialitate = (NodLP**)malloc(sizeof(NodLP*) * (strlen(buffer) + 1));
			strcpy(specialitate, buffer);

			fscanf(file, "%s", buffer);
			diagnostic = (NodLP**)malloc(sizeof(NodLP*) * (strlen(buffer) + 1));
			strcpy(diagnostic, buffer);

			fscanf(file, "%d", &pretConsultatie);

			inserareInListaDeListe(&(*listaDinFisier), initializareConsultatie(dataConsultatie, numeMedic, specialitate, diagnostic, pretConsultatie));
		}
	}
}

int nrConsultatiiSpecialitate(NodLP* lista, char* specialitate) {
	int contor = 0;
	if (lista == NULL) {
		return contor;
	}
	else {
		NodLP* aux = lista;
		while (aux) {
			if (strcmp(aux->lista->info.specialitate, specialitate) == 0) {
				NodLS* aux2 = aux->lista;
				while (aux2) {
					contor++;
					aux2 = aux2->next;
				}
				return contor;
			}
			aux = aux->next;
		}
	}
	return contor;
}

void modificaPretConsultatie(NodLP** lista, char* dataConsultatieData, float pretNou){
	if ((*lista) == NULL) {
		return;
	}
	else {
		NodLP* aux = *lista;
		while (aux) {
			NodLS* aux2 = aux->lista;
			while (aux2) {
				if (strcmp(aux2->info.dataConsultatie, dataConsultatieData) == 0) {
					aux2->info.pretConsultatie = pretNou;
				}
				aux2 = aux2->next;
			}
			aux = aux->next;
		}
	}
}

void stergeConsultatie(NodLS** nod, char* diagnostic) {
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

void stergereMare(NodLP** lista, char* diagnostic) {
	if (*lista == NULL) {
		return;
	}
	else {
		NodLP* aux = *lista;
		while (aux->next) {
			stergeConsultatie(&(aux->lista), diagnostic);
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
	//Consultatie c1 = initializareConsultatie("11/11/2011", "Gavrilescu", "dermatolog", "bolnav", 300);
	//Consultatie c2 = initializareConsultatie("12/12/2023", "Popescu", "medic", "suparat", 450);

	//afisareConsultatie(c1);
	//afisareConsultatie(c2);

	NodLS* capNodLS = NULL;
	NodLP* capLista = NULL;
	//inserareInListaLaFinal(&capNodLS, c1);
	//inserareInListaLaFinal(&capNodLS, c2);

	//parcurgereLista(capNodLS);

	//inserareInNodLP(&capLista, capNodLS);
	//parcurgereNodLP(capLista);

	//inserareInListaDeListe(&capLista, c1);
	//inserareInListaDeListe(&capLista, c2);
	//parcurgereNodLP(capLista);

	printf("\n------------------------SUBIECT 1------------------------------\n");
	FILE* file = fopen("dateDeIntrare.txt", "r");
	citireDinFisier(file, &capLista);
	parcurgereNodLP(capLista);

	printf("\n------------------------SUBIECT 2------------------------------\n");

	int nrConsultatii = 0;
	char* specialitateData = "dermatolog";
	nrConsultatii = nrConsultatiiSpecialitate(capLista, specialitateData);
	printf("\n%d", nrConsultatii);

	printf("\n------------------------SUBIECT 3------------------------------\n");

	char* dataConsultatieData = "11/11/2011";
	modificaPretConsultatie(&capLista, dataConsultatieData, 400);
	parcurgereNodLP(capLista);

	printf("\n------------------------SUBIECT 4------------------------------\n");

	char* diagnostic = "bolnav";
	stergereMare(&capLista, diagnostic);
	parcurgereNodLP(capLista);

	printf("\n------------------------SUBIECT 5------------------------------\n");

	int k = 0;
	Consultatie* vector;

	vector = creareVectorConsultatii(capLista, 460, &k);
	for (int i = 0; i < k; i++) {
		afisareConsultatie(vector[i]);
	}

	printf("\n------------------------DEZALOCARE------------------------------\n");

	dezalocareLP(&capLista);

	printf("S-a facut dezalocare!");


}
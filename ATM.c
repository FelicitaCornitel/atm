#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct history
{
	char *result;
	char *operation;
	struct history *next;
} THistory, *TListaHistory, **AListaHistory;

typedef struct card
{
	char card_number[17], pin[5], cvv[4], expiry_data[6];
	char *status;
	int balance;
	TListaHistory history;

} TCard, *TListaCard, **AListaCard;

typedef struct LSC
{
	int gresit;
	TListaCard card;
	struct LSC *next;
} TLSC, *TListaLSC, **AListaLSC;

typedef struct LC
{
	TListaLSC LSC;
	struct LC *next;
} TLC, *TListaLC, **AListaLC;

TListaLC AlocLista(size_t d) // d=nr_max_card
{
	TListaLC aux, celula, lista;
	aux = calloc(1, sizeof(TLC));
	if (!aux)
		return NULL;
	d = d - 1; //am alocat deja o celula
	lista = aux;
	while (d)
	{
		celula = calloc(1, sizeof(TLC));
		if (!celula)
			return NULL;
		aux->next = celula;
		aux = celula;
		d = d - 1;
	}
	return lista;
}
//functia de adaugarea cardului
void add_card(char card_number[17], char pin[5], char expiry_data[6],
			  char cvv[4], int nr_max_card, TListaLC LC, FILE *out)
{
	TListaLSC aux, c;
	int i, poz, suma = 0;
	//calculez pozitia cardului si
	for (i = 0; i < 16; i++)
		suma = suma + (card_number[i] - '0');
	poz = suma % nr_max_card;
	for (i = 0; i < poz; i++)
		LC = LC->next;
	c = LC->LSC;
	//parcurg submultimea de carduri
	for (c = LC->LSC; c != NULL; c = c->next)
		if (strcmp(c->card->card_number, card_number) == 0)
		{
			fprintf(out, "The card already exists\n");
			return;
		}
	//daca cardul nu exista, construiesc cardul
	aux = malloc(sizeof(TLSC));
	if (!aux)
	{
		return;
	}
	aux->card = malloc(sizeof(TCard));
	if (!aux->card)
	{
		free(aux);
		return;
	}
	strcpy(aux->card->card_number, card_number);
	strcpy(aux->card->pin, pin);
	strcpy(aux->card->expiry_data, expiry_data);
	strcpy(aux->card->cvv, cvv);
	aux->card->balance = 0;
	aux->card->status = strdup("NEW");
	aux->card->history = NULL;
	aux->next = LC->LSC;
	LC->LSC = aux;
}
//fuctia de stergere a cardului
void delete_card(char card_number[17], int nr_max_card, TListaLC LC)
{
	TListaLSC c, ant = NULL;
	int i, poz, suma = 0;
	for (i = 0; i < 16; i++)
		suma = suma + (card_number[i] - '0');
	poz = suma % nr_max_card;
	for (i = 0; i < poz; i++)
		LC = LC->next;
	c = LC->LSC;
	for (c = LC->LSC; c != NULL; ant = c, c = c->next)
	{
		if (strcmp(card_number, c->card->card_number) == 0)
		{
			printf("lala\n");
			if (ant == NULL)
				LC->LSC = c->next;
			else
				ant->next = c->next;
			break;
		}
	}
}
void show(TListaLC LC, FILE *out)
{
	TListaLC p, s;
	TListaHistory h;
	TListaLSC c;
	p = LC;
	int i = 0;
	while (p != NULL)
	{
		if (p->LSC == NULL)
		{
			s = p;
			while (s != NULL)
			{
				if (s->LSC != NULL)
					break;

				s = s->next;
			}
			if (s == NULL)
				break;
		}
		fprintf(out, "pos%d: [", i);
		c = p->LSC;
		if (c == NULL)
			fprintf(out, "]\n");
		else
		{
			while (c != NULL)
			{
				h = c->card->history;
				fprintf(out, "\n(%s: %s, %s: %s, %s: %s, %s: %s, %s: %d, %s: %s, ", "card number",
						c->card->card_number, "PIN", c->card->pin, "expiry date",
						c->card->expiry_data, "CVV", c->card->cvv, "balance",
						c->card->balance, "status", c->card->status);
				fprintf(out, "%s: [", "history");
				while (h != NULL)
				{
					fprintf(out, "(%s, %s)", h->result, h->operation);
					if (h->next != NULL)
						fprintf(out, ", ");
					h = h->next;
				}
				fprintf(out, "])");
				c = c->next;
			}

			fprintf(out, "\n]\n");
		}
		p = p->next;
		i++;
	}
}
void show_param(char card_number[17], TListaLC LC, FILE *out, int nr_max_card)
{
	TListaLC l;
	l = LC;
	int i, poz, suma = 0;
	TListaLSC c;
	TListaHistory h;
	for (i = 0; i <= 16; i++)
		suma = suma + (card_number[i] - '0');
	poz = suma % nr_max_card;
	for (i = 0; i < poz; i++)
		l = l->next;
	c = l->LSC;
	while (c != NULL)
	{

		if (strcmp(c->card->card_number, card_number) == 0)
		{

			h = c->card->history;
			fprintf(out, "(%s: %s, %s: %s, %s: %s, %s: %s, %s: %d, %s: %s, ", "card number", c->card->card_number, "PIN", c->card->pin, "expiry date", c->card->expiry_data, "CVV", c->card->cvv, "balance", c->card->balance, "status", c->card->status);
			fprintf(out, "%s: [", "history");
			while (h != NULL)
			{
				fprintf(out, "(%s, %s)", h->result, h->operation);
				if (h->next != NULL)
					fprintf(out, ", ");
				h = h->next;
			}
			fprintf(out, "])\n");
		}

		c = c->next;
	}
}

void insert_card(char card_number[17], char pin[5], int nr_max_card, TListaLC LC, FILE *out)
{
	int i, poz, suma = 0;
	TListaLSC c;
	TListaHistory aux;
	c = LC->LSC;
	for (i = 0; i < 16; i++)
		suma = suma + (card_number[i] - '0');
	poz = suma % nr_max_card;
	for (i = 0; i < poz; i++)
		LC = LC->next;
	while (c != NULL)
	{
		if (strcmp(card_number, c->card->card_number) == 0)
		{
			if (strcmp(c->card->status, "LOCKED") == 0)
			{
				aux = calloc(1, sizeof(THistory));
				if (!aux)
					return;
				aux->result = malloc(100 * sizeof(char));
				if (!aux->result)
					return;
				aux->operation = malloc(100 * sizeof(char));
				if (!aux->operation)
					return;
				strcpy(aux->result, "FAIL");
				sprintf(aux->operation, "%s %s %s", "insert_card", card_number, pin);
				aux->next = c->card->history;
				c->card->history = aux;
				fprintf(out, "The card is blocked. Please contact the administrator.\n");
				break;
			}
			if (strcmp(pin, c->card->pin) != 0)
			{
				fprintf(out, "Invalid PIN\n");
				c->gresit = c->gresit + 1;
				if (c->gresit >= 3)
				{
					fprintf(out, "The card is blocked. Please contact the administrator.\n");
					c->card->status = strdup("LOCKED");
				}
				aux = calloc(1, sizeof(THistory));
				if (!aux)
					return;
				aux->result = malloc(100 * sizeof(char));
				if (!aux->result)
					return;
				aux->operation = malloc(100 * sizeof(char));
				if (!aux->operation)
					return;
				strcpy(aux->result, "FAIL");
				sprintf(aux->operation, "%s %s %s", "insert_card", card_number, pin);
				aux->next = c->card->history;
				c->card->history = aux;
			}
			else
			{
				aux = calloc(1, sizeof(THistory));
				if (!aux)
					return;
				aux->result = malloc(100 * sizeof(char));
				if (!aux->result)
					return;
				aux->operation = malloc(100 * sizeof(char));
				if (!aux->operation)
					return;
				strcpy(aux->result, "SUCCESS");
				sprintf(aux->operation, "%s %s %s", "insert_card", card_number, pin);
				aux->next = c->card->history;
				c->card->history = aux;
				c->gresit = 0;
				if (strcmp("NEW", c->card->status) == 0)
					fprintf(out, "You must change your PIN.\n");
			}
		}
		c = c->next;
	}
}
void cancel(char card_number[17], int nr_max_card, TListaLC LC)
{
	TListaLSC c;
	TListaHistory aux;
	c = LC->LSC;
	int i, poz, suma = 0;
	for (i = 0; i < 16; i++)
		suma = suma + (card_number[i] - '0');
	poz = suma % nr_max_card;
	for (i = 0; i < poz; i++)
		LC = LC->next;
	while (c != NULL)
	{
		if (strcmp(card_number, c->card->card_number) == 0)
		{
			aux = calloc(1, sizeof(THistory));
			if (!aux)
				return;
			aux->result = malloc(100 * sizeof(char));
				if (!aux->result)
					return;
				aux->operation = malloc(100 * sizeof(char));
				if (!aux->operation)
					return;
			strcpy(aux->result, "SUCCESS");
			sprintf(aux->operation, "%s %s", "cancel", card_number);
			aux->next = c->card->history;
			c->card->history = aux;
		}
		c = c->next;
	}
}
void recharge(char card_number[17], int nr_max_card, TListaLC LC, int suma, FILE *out)
{
	TListaLSC c;
	TListaHistory aux;
	int i, poz, sum = 0;
	c = LC->LSC;
	for (i = 0; i < 16; i++)
		sum = sum + (card_number[i] - '0');
	poz = sum % nr_max_card;
	for (i = 0; i < poz; i++)
		LC = LC->next;
	while (c != NULL)
	{
		if (strcmp(card_number, c->card->card_number) == 0)
		{
			if (strcmp(c->card->status, "LOCKED") == 0)
			{
				aux = calloc(1, sizeof(THistory));
				if (!aux)
					return;
				aux->result = malloc(100 * sizeof(char));
				if (!aux->result)
					return;
				aux->operation = malloc(100 * sizeof(char));
				if (!aux->operation)
					return;
				strcpy(aux->result, "FAIL");
				sprintf(aux->operation, "%s %s %d", "recharge", card_number, suma);
				aux->next = c->card->history;
				c->card->history = aux;
				fprintf(out, "The card is blocked. Please contact the administrator.\n");
				break;
			}
			if (suma % 10 != 0)
			{
				fprintf(out, "The added amount must be multiple of 10\n");
				aux = calloc(1, sizeof(THistory));
				if (!aux)
					return;
				aux->result = malloc(100 * sizeof(char));
				if (!aux->result)
					return;
				aux->operation = malloc(100 * sizeof(char));
				if (!aux->operation)
					return;
				strcpy(aux->result, "FAIL");
				sprintf(aux->operation, "%s %s %d", "recharge", card_number, suma);
				aux->next = c->card->history;
				c->card->history = aux;
			}
			else
			{
				c->card->balance = c->card->balance + suma;
				fprintf(out, "%d\n", c->card->balance);
				aux = calloc(1, sizeof(THistory));
				if (!aux)
					return;
				aux->result = malloc(100 * sizeof(char));
				aux->operation = malloc(100 * sizeof(char));
				strcpy(aux->result, "SUCCESS");
				sprintf(aux->operation, "%s %s %d", "recharge", card_number, suma);
				aux->next = c->card->history;
				c->card->history = aux;
			}
		}

		c = c->next;
	}
}
void cash_withdrawal(char card_number[17], int nr_max_card, TListaLC LC, int s, FILE *out)
{
	TListaLSC c;
	TListaHistory aux;
	int i, poz, sum = 0;
	c = LC->LSC;
	for (i = 0; i < 16; i++)
		sum = sum + (card_number[i] - '0');
	poz = sum % nr_max_card;
	for (i = 0; i < poz; i++)
		LC = LC->next;
	while (c != NULL)
	{
		if (strcmp(card_number, c->card->card_number) == 0)
		{
			if (strcmp(c->card->status, "LOCKED") == 0)
			{
				aux = calloc(1, sizeof(THistory));
				if (!aux)
					return;
				aux->result = malloc(100 * sizeof(char));
				aux->operation = malloc(100 * sizeof(char));
				strcpy(aux->result, "FAIL");
				sprintf(aux->operation, "%s %s %d", "cash_withdrawal", card_number, s);
				aux->next = c->card->history;
				c->card->history = aux;
				fprintf(out, "The card is blocked. Please contact the administrator.\n");
				break;
			}
			if (s % 10 != 0)
			{
				fprintf(out, "The requested amount must be multiple of 10\n");
				aux = calloc(1, sizeof(THistory));
				if (!aux)
					return;
				aux->result = malloc(100 * sizeof(char));
				aux->operation = malloc(100 * sizeof(char));
				strcpy(aux->result, "FAIL");
				sprintf(aux->operation, "%s %s %d", "cash_withdrawal", card_number, s);
				aux->next = c->card->history;
				c->card->history = aux;
				break;
			}
			if (c->card->balance < s)
			{
				fprintf(out, "Insufficient funds\n");
				aux = calloc(1, sizeof(THistory));
				if (!aux)
					return;
				aux->result = malloc(100 * sizeof(char));
				aux->operation = malloc(100 * sizeof(char));
				strcpy(aux->result, "FAIL");
				sprintf(aux->operation, "%s %s %d", "cash_withdrawal", card_number, s);
				aux->next = c->card->history;
				c->card->history = aux;
				break;
			}
			else
			{
				c->card->balance = c->card->balance - s;
				fprintf(out, "%d\n", c->card->balance);
				aux = calloc(1, sizeof(THistory));
				if (!aux)
					return;
				aux->result = malloc(100 * sizeof(char));
				aux->operation = malloc(100 * sizeof(char));
				strcpy(aux->result, "SUCCESS");
				sprintf(aux->operation, "%s %s %d", "cash_withdrawal", card_number, s);
				aux->next = c->card->history;
				c->card->history = aux;
				break;
			}
		}
		c = c->next;
	}
}
void transfer_funds(char card_number_source[17], char card_number_dest[17], int nr_max_card, TListaLC LC, int suma, FILE *out)
{
	TListaLSC c, b;
	TListaHistory aux;
	TListaLC copie = LC;
	int i, j, poz, p, sum = 0, s = 0;
	c = LC->LSC;
	for (i = 0; i < 16; i++)
		sum = sum + (card_number_source[i] - '0');
	poz = sum % nr_max_card;
	for (i = 0; i < poz; i++)
		LC = LC->next;
	for (j = 0; j < 16; j++)
		s = s + (card_number_dest[j] - '0');
	p = s % nr_max_card;
	for (j = 0; j < p; j++)
		copie = copie->next;
	while (c != NULL)
	{
		if (strcmp(card_number_source, c->card->card_number) == 0)
		{
			if (strcmp(c->card->status, "LOCKED") == 0)
			{
				aux = calloc(1, sizeof(THistory));
				if (!aux)
					return;
				aux->result = malloc(100 * sizeof(char));
				aux->operation = malloc(100 * sizeof(char));
				strcpy(aux->result, "FAIL");
				sprintf(aux->operation, "%s %s %s %d", "transfer_funds", card_number_source, card_number_dest, suma);
				aux->next = c->card->history;
				c->card->history = aux;
				fprintf(out, "The card is blocked. Please contact the administrator.\n");
				break;
			}
			if (c->card->balance < suma)
			{

				fprintf(out, "Insufficient funds\n");
				aux = calloc(1, sizeof(THistory));
				if (!aux)
					return;
				aux->result = malloc(100 * sizeof(char));
				aux->operation = malloc(100 * sizeof(char));
				strcpy(aux->result, "FAIL");
				sprintf(aux->operation, "%s %s %s %d", "transfer_funds", card_number_source, card_number_dest, suma);
				aux->next = c->card->history;
				c->card->history = aux;
				break;
			}
			if (suma % 10 != 0)
			{
				fprintf(out, "The transferred amount must be multiple of 10\n");
				aux = calloc(1, sizeof(THistory));
				if (!aux)
					return;
				aux->result = malloc(100 * sizeof(char));
				aux->operation = malloc(100 * sizeof(char));
				strcpy(aux->result, "FAIL");
				sprintf(aux->operation, "%s %s %s %d", "transfer_funds", card_number_source, card_number_dest, suma);
				aux->next = c->card->history;
				c->card->history = aux;
			}
			else
			{
				c->card->balance = c->card->balance - suma;
				fprintf(out, "%d\n", c->card->balance);
				aux = calloc(1, sizeof(THistory));
				if (!aux)
					return;
				aux->result = malloc(100 * sizeof(char));
				aux->operation = malloc(100 * sizeof(char));
				strcpy(aux->result, "SUCCESS");
				sprintf(aux->operation, "%s %s %s %d", "transfer_funds", card_number_source, card_number_dest, suma);
				aux->next = c->card->history;
				c->card->history = aux;
				b = copie->LSC;
				while (b != NULL)
				{
					if (strcmp(card_number_dest, b->card->card_number) == 0)
					{
						b->card->balance = b->card->balance + suma;
						aux = calloc(1, sizeof(THistory));
						if (!aux)
							return;
						aux->result = malloc(100 * sizeof(char));
						aux->operation = malloc(100 * sizeof(char));
						strcpy(aux->result, "SUCCESS");
						sprintf(aux->operation, "%s %s %s %d", "transfer_funds", card_number_source, card_number_dest, suma);
						aux->next = b->card->history;
						b->card->history = aux;
					}
					b = b->next;
				}
			}
		}
		c = c->next;
	}
}

void balance_inquiry(char card_number[17], int nr_max_card, TListaLC LC, FILE *out)
{
	TListaLSC c;
	TListaHistory aux;
	int i, poz, sum = 0;
	c = LC->LSC;
	for (i = 0; i < 16; i++)
		sum = sum + (card_number[i] - '0');
	poz = sum % nr_max_card;
	for (i = 0; i < poz; i++)
		LC = LC->next;
	while (c != NULL)
	{
		if (strcmp(card_number, c->card->card_number) == 0)
		{
			if (strcmp(c->card->status, "LOCKED") == 0)
			{
				aux = calloc(1, sizeof(THistory));
				if (!aux)
					return;
				aux->result = malloc(100 * sizeof(char));
				aux->operation = malloc(100 * sizeof(char));
				strcpy(aux->result, "FAIL");
				sprintf(aux->operation, "%s %s", "balance_inquiry", card_number);
				aux->next = c->card->history;
				c->card->history = aux;
				fprintf(out, "The card is blocked. Please contact the administrator.\n");
				break;
			}
			fprintf(out, "%d\n", c->card->balance);
			aux = calloc(1, sizeof(THistory));
			if (!aux)
				return;
			aux->result = malloc(100 * sizeof(char));
			aux->operation = malloc(100 * sizeof(char));
			strcpy(aux->result, "SUCCESS");
			sprintf(aux->operation, "%s %s", "balance_inquiry", card_number);
			aux->next = c->card->history;
			c->card->history = aux;
		}
		c = c->next;
	}
}
void unblock_card(char card_number[17], int nr_max_card, TListaLC LC)
{
	TListaLSC c;
	int i, poz, sum = 0;
	c = LC->LSC;
	for (i = 0; i < 16; i++)
		sum = sum + (card_number[i] - '0');
	poz = sum % nr_max_card;
	for (i = 0; i < poz; i++)
		LC = LC->next;
	while (c != NULL)
	{
		if (strcmp(card_number, c->card->card_number) == 0)
		{
			c->card->status = strdup("ACTIVE");
			c->gresit = 0;
		}
		c = c->next;
	}
}
void pin_change(char card_number[17], int nr_max_card, TListaLC LC, char *new, FILE *out)
{
	TListaLSC c;
	TListaHistory aux;
	int i, poz, sum = 0;
	c = LC->LSC;
	for (i = 0; i < 16; i++)
		sum = sum + (card_number[i] - '0');
	poz = sum % nr_max_card;
	for (i = 0; i < poz; i++)
		LC = LC->next;
	while (c != NULL)
	{
		if (strcmp(card_number, c->card->card_number) == 0)
		{
			if (strcmp(c->card->status, "LOCKED") == 0)
			{
				aux = calloc(1, sizeof(THistory));
				if (!aux)
					return;
				aux->result = malloc(100 * sizeof(char));
				aux->operation = malloc(100 * sizeof(char));
				strcpy(aux->result, "FAIL");
				sprintf(aux->operation, "%s %s %s", "pin_change", card_number, new);
				aux->next = c->card->history;
				c->card->history = aux;
				fprintf(out, "The card is blocked. Please contact the administrator.\n");
				break;
			}
			if (strlen(new) != 4)
			{
				fprintf(out, "Invalid PIN\n");
				aux = calloc(1, sizeof(THistory));
				if (!aux)
					return;
				aux->result = malloc(100 * sizeof(char));
				aux->operation = malloc(100 * sizeof(char));
				strcpy(aux->result, "FAIL");
				sprintf(aux->operation, "%s %s %s", "pin_change", card_number, new);
				aux->next = c->card->history;
				c->card->history = aux;
				break;
			}
			for (i = 0; i < 4; i++)
			{
				if (new[i] < '0' || new[i] > '9')
				{
					fprintf(out, "Invalid PIN\n");
					aux = calloc(1, sizeof(THistory));
					if (!aux)
						return;
					aux->result = malloc(100 * sizeof(char));
					aux->operation = malloc(100 * sizeof(char));
					strcpy(aux->result, "FAIL");
					sprintf(aux->operation, "%s %s %s", "pin_change", card_number, new);
					aux->next = c->card->history;
					c->card->history = aux;
					break;
				}
			}
			strcpy(c->card->pin, new);
			c->card->status = strdup("ACTIVE");
			aux = calloc(1, sizeof(THistory));
			if (!aux)
				return;
			aux->result = malloc(100 * sizeof(char));
			aux->operation = malloc(100 * sizeof(char));
			strcpy(aux->result, "SUCCESS");
			sprintf(aux->operation, "%s %s %s", "pin_change", card_number, new);
			aux->next = c->card->history;
			c->card->history = aux;
			break;
		}
		c = c->next;
	}
}
int main()
{

	TListaLC L;

	FILE *in = fopen("input.in", "r");
	if (!in)
	{
		printf("Fisierul nu s-a deschis\n");
		return 0;
	}

	FILE *out = fopen("output.out", "wt");
	if (!out)
	{
		printf("Fisierul nu s-a deschis\n");
		return 0;
	}

	char aux;
	int Max;
	fscanf(in, "%d", &Max);
	L = AlocLista(Max);

	//citirea functiilor din fisier
	char func[Max];
	while (fscanf(in, "%s", func) == 1)
	{
		if (strcmp(func, "add_card") == 0)
		{
			char card_number[17], expiry_date[6];
			char pin[5], cvv[4];

			fscanf(in, "%s%s%s%s", card_number, pin, expiry_date, cvv);
			add_card(card_number, pin, expiry_date, cvv, Max, L, out);
		}
		else if (strcmp(func, "delete_card") == 0)
		{
			char card_number[17];
			fscanf(in, "%s", card_number);
			delete_card(card_number, Max, L);
		}
		else if (strcmp(func, "reverse_transaction") == 0)
		{
			char card_number_source[Max], card_number_dest[Max];
			int sum;
			fscanf(in, "%s%s%d", card_number_source, card_number_dest, &sum);
			//reverse_transaction(card_number_source,card_number_dest,sum);
		}
		else if (strcmp(func, "unblock_card") == 0)
		{
			char card_number[17];
			fscanf(in, "%s", card_number);
			unblock_card(card_number, Max, L);
		}
		else if (strcmp(func, "show") == 0)
		{
			//char card_number[17];
			//se citeste un caracter daca e rand nou sau sfarsit de fisier
			// atunci nu are parametru
			fscanf(in, "%c", &aux);
			if (aux == '\n' || aux == '\0')
			{
				//fara parametru

				show(L, out);
			}
			else
			{
				//cu parametru
				char card_number[17];
				fscanf(in, "%s", card_number);
				show_param(card_number, L, out, Max);
			}
		}
		else if (strcmp(func, "insert_card") == 0)
		{
			char card_number[17];
			char pin[5];
			fscanf(in, "%s%s", card_number, pin);
			insert_card(card_number, pin, Max, L, out);
		}
		else if (strcmp(func, "recharge") == 0)
		{
			char card_number[17];
			int suma;
			fscanf(in, "%s%d", card_number, &suma);
			recharge(card_number, Max, L, suma, out);
		}
		else if (strcmp(func, "cash_withdrawal") == 0)
		{
			char card_number[17];
			int s;
			fscanf(in, "%s%d", card_number, &s);
			cash_withdrawal(card_number, Max, L, s, out);
		}
		else if (strcmp(func, "balance_inquiry") == 0)
		{
			char card_number[17];
			fscanf(in, "%s", card_number);
			balance_inquiry(card_number, Max, L, out);
		}
		else if (strcmp(func, "transfer_funds") == 0)
		{
			char card_number_source[17], card_number_dest[17];
			int sum;
			fscanf(in, "%s%s%d", card_number_source, card_number_dest, &sum);
			transfer_funds(card_number_source, card_number_dest, Max, L, sum, out);
		}
		else if (strcmp(func, "pin_change") == 0)
		{
			char card_number[17];
			char *new = malloc(100 * sizeof(char));
			fscanf(in, "%s%s", card_number, new);
			pin_change(card_number, Max, L, new, out);
		}
		else if (strcmp(func, "cancel") == 0)
		{
			char card_number[17];
			fscanf(in, "%s", card_number);
			cancel(card_number, Max, L);
		}
	}

	fclose(in);
	fclose(out);
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX 100

typedef struct operand {
	char sign;
	int  value;
	struct operand* next;
}	operand;

typedef struct node {
	char value;
	int level;
	int dx;
	struct node* left, * right;
}	node;

typedef struct stack {
	int top;
	int size;
	node** array;
}	stack;

typedef struct charstack {
	int top;
	int size;
	char* array;
}	charstack;

typedef struct floatstack {
	int top;
	int size;
	float* array;
}	floatstack;

stack* createStack(int n) {
	stack* s = malloc(sizeof(stack));
	s->size = n;
	s->top = -1;
	s->array = malloc(n * sizeof(node*));
	return s;
}

charstack* createcStack(int n) {
	charstack* s = malloc(sizeof(charstack));
	s->size = n;
	s->top = -1;
	s->array = malloc(n * sizeof(char));
	return s;
}

floatstack* createfStack(int n) {
	floatstack* s = malloc(sizeof(floatstack));
	s->size = n;
	s->top = -1;
	s->array = malloc(n * sizeof(float));
	return s;
}

void deleteStack(stack* s) {
	free(s->array);
	free(s);
}

void deletecStack(charstack* s) {
	free(s->array);
	free(s);
}

void deletefStack(floatstack* s) {
	free(s->array);
	free(s);
}

int isEmpty(stack* s) {
	return s->top == -1;
}

int isEmpty_c(charstack* s) {
	return s->top == -1;
}

int isEmpty_f(floatstack* s) {
	return s->top == -1;
}

void push(stack* s, node* n) {
	if (s->top == s->size - 1) return;
	s->top++;
	s->array[s->top] = n;
}

void push_c(charstack* s, char n) {
	if (s->top == s->size - 1) return;
	s->top++;
	s->array[s->top] = n;
}

void push_f(floatstack* s, float n) {
	if (s->top == s->size - 1) return;
	s->top++;
	s->array[s->top] = n;
}

node* pop(stack* s) {
	return s->array[s->top--];
}

char pop_c(charstack* s) {
	return s->array[s->top--];
}

float pop_f(floatstack* s) {
	return s->array[s->top--];
}

node* top(stack* s) {
	return s->array[s->top];
}

char top_c(charstack * s) {
	return s->array[s->top];
}

node* createNode(char n) {
	node* p = malloc(sizeof(node));
	p->value = n;
	p->level = 0 ;
	p->dx = 0;
	p->left = NULL;
	p->right = NULL;
	return p;
}

node* rewriteNode(node* n,node*a) {
	a->value = n->value;
	a->level = n->level;
	a->dx = n->dx;
	a->left = n->left;
	a->right = n->right;
	return a;
}

int isOperand(char a) {
	if (a >= 'A' && a <= 'Z') { return 1;}
	else { return 0; }
}

int isDigit(char a) {
	if (a >= '0' && a <= '9') { return 1; }
	else { return 0; }
}

int isbOperator(char a) {
	if( (a == '+' ) || (a == '-') || (a == '*') || (a == '/') || (a == '^')) { return 1; }
	else { return 0; }
}

int isuOperator(char c) {
	if ((c == 's') || (c == 'c') || (c == 'l') || (c=='#')) { return 1; }
	else { return 0; }
}

operand* linkOperands(operand* opr, char a) { //dodaje operand A u listu operanada, ukoliko vec nije u njoj
	operand* q = opr;
	operand* p = opr;
	if (opr == NULL) {
		p = malloc(sizeof(operand));
		p->sign = a; 
		p->next = NULL;
		return p;
	}
	while (p != NULL) {
		if (p->sign == a) { return opr; }
		p = p->next;
		if (p != NULL) { q = p; }
	}
	p = malloc(sizeof(operand));
	p->sign = a; 
	p->next = NULL;
	q->next = p;	
	return opr;
}

operand* createOperandsList(char* a) { //vraca listu svih operanada
	char* s = a;
	operand* opr = NULL;
	while (*s != '\0') {
		if (isOperand(*s)) {
			opr = linkOperands(opr, *s);
		}
		s++;
	}
	return opr;
}

void deleteOperandsList(operand* opr) {
	operand* p = opr, * q;
	while (p) {
		q = p->next;
		free(p);
		p = q;
	}
}

operand* insertValues(operand* opr) {
	operand *op = opr;
	int a;
	while (op) {
		printf("Unesite vrednost parametra %c: ", op->sign);
		scanf("%d", &a);
		op->value = a;
		op = op->next;
	}
	return opr;
}

int getValue(operand* opr, char c) {
	while (opr) {
		if (opr->sign == c) {
			int a = opr->value;
			return a;	}
		opr = opr->next;
	}
	int b = c-'0';
	return b;
}

int stpriority(char n) {
	switch (n)
	{
	case '+':
	case '-':	return 2;
	case '*':
	case '/':	return 3;
	case '^':	return 4;
	case '(':	return 0;
	case '#':
	case 's':
	case 'c':
	case 'l':	return 6;
	}
	return -1;
}

int inpriority(char n) {
	switch (n)
	{
	case '+':
	case '-':	return 2;
	case '*':
	case '/':	return 3;
	case '^':	return 5;
	case '(':	return 8;
	case ')':	return 1;
	case '#':
	case 's':
	case 'c':
	case 'l':	return 7;
	}
	return -1;
}

char* transformInput(char* str) { //u ucitanom stringu menja sin=s, cos=c, ln=l
	char* p = str, * q = str;
	while (*p != '\0') {
		if (*p == 'i' || *p == 'n' || *p == 'o' || (*p == 's' && *(p + 1) != 'i')) {
			p++;	}
		else {	
			*q = *p;
			q++;
			p++;	}
		if (p > str) {
			if (*p == '-' && *(p - 1) == '(') { *p = '#'; }
		}
	}
	*q = '\0';
	return str;
}

char* infixToPostfix(char* infix, int n) { //uneti izraz pretvara iz infiksa u postfiks
	charstack* stek = createcStack(n);
	char x, * postfix, * p, * i;
	postfix = malloc(n * sizeof(char)); p = postfix;
	for (i = infix; i < infix + n; i++) {
		if (isOperand(*i)) {
			*p = *i;	p++;
		}
		else {
			while (!isEmpty_c(stek)){	if (inpriority(*i) <= stpriority(top_c(stek))) {
				x = pop_c(stek);
				*p = x;	p++;
			}
			else { break; }
			}
			if (*i != ')') { push_c(stek, *i); }
			else { x = pop_c(stek); }
		}
	}
	while (!isEmpty_c(stek)) {
		x = pop_c(stek);
		*p = x; p++;
	}
	*p = '\0';
	free(stek->array);
	free(stek);
	return postfix; 
}

node* makeTree(char* postfix, int n) { //n=strlen(infix)
	stack* stek = createStack(n);
	char* p = postfix;
	node* a;
	while (*p != '\0') {
		if (isOperand(*p)) {
			a = createNode(*p);
			push(stek, a);
		}
		if (isbOperator(*p)) {
			a = createNode(*p);
			a->right = pop(stek);
			a->left = pop(stek);
			push(stek, a);
		}
		if (isuOperator(*p)) {
			a = createNode(*p);
			a->left = pop(stek);
			push(stek, a);
		}
		p++;
	}
	free(stek->array);
	free(stek);
	return a;
}


void deleteTree(node* root, int n) {
	stack* stek = createStack(n);
	node* r,* p;
	push(stek, root);
	while (!isEmpty(stek)) {
		r = pop(stek);
		while (r) {
			if (r->right) { push(stek, r->right); }
			p = r->left;
			free(r);
			r = p;
		}
	}
	free(stek->array);
	free(stek);
	return;
}

 
void printFormated(node* root,int n) { //preorder
	node* p, * r = root;
	stack* stek = createStack(n);
	push(stek, r);
	while (!isEmpty(stek)) {
		r = pop(stek);
		while (r) {
			printf("|"); 
			for (int j = 0; j < r->level; j++) {
				printf("___");	}
			printf("_(%c)\n", r->value);

			if (r->right) { push(stek, r->right); p = r->right; p->level = r->level + 1; }
			if (r->left) { p  = r->left; p->level = r->level + 1; }
			r = r->left;
		}
	}
	//vraca r->level na 0
	push(stek, root);
	while (!isEmpty(stek)) {
		r = pop(stek);
		while (r) {
			r->level = 0;
			if (r->right) { push(stek, r->right); }
			r = r->left;
		}
	}
	free(stek->array);
	free(stek);
}

void printPostfix(node* root, int n) { //p->level=0 prvi put, p->level=1 drugi put na steku
	node* p = root;
	stack* stek = createStack(n);
	while (p) {
		push(stek, p);
		p = p->left;
	}
	while (!isEmpty(stek)) {
		p = pop(stek);
		if (p->level==0) {
			p->level = 1;
			push(stek, p);
			p = p->right;
			while (p) {
				push(stek, p);
				p = p->left;
			}
		}
		else {
			p->level = 0;
			//printf("%c", p->value);
			if (isuOperator(p->value)) {
				if (p->value == 'l') {
					printf("ln"); }
				if (p->value == 's') { 
					printf("sin"); }
				if (p->value == 'c') { 
					printf("cos"); }
				if (p->value == '#') { 
					printf("-"); }

			}
			else { printf("%c", p->value); } 
		}
	}
}

float calculate(node* root, int n, operand* opr) {
	node* p = root;
	stack* stek = createStack(n);
	floatstack* rez = createfStack(n);
	float a, b, c;
	while (p) {
		push(stek, p);
		p = p->left;
	}
	while (!isEmpty(stek)) {
		p = pop(stek);
		if (p->level == 0) {
			p->level = 1;
			push(stek, p);
			p = p->right;
			while (p) {
				push(stek, p);
				p = p->left;
			}
		}
		else {
			p->level = 0;
			if (isOperand(p->value)|| isDigit(p->value)) {
				a = (float) getValue(opr,p->value);
				push_f(rez, a);
			}
			if (isuOperator(p->value)) {
				a = pop_f(rez);
				if (p->value == 's') { b = sinf(a); }
				if (p->value == 'c') { b = cosf(a); }
				if (p->value == 'l') { b = logf(a); }
				if (p->value == '#') { b = -1 * a; }
				push_f(rez, b);
			}
			if (isbOperator(p->value)) {
				b = pop_f(rez);
				a = pop_f(rez);
				if (p->value == '+') { c = a + b; }
				if (p->value == '-') { c = a - b; }
				if (p->value == '*') { c = a * b; }
				if (p->value == '/') { c = a / b; }
 				if (p->value == '^') { c = powf(a, b); }
				push_f(rez, c);

			}
		}
	}
	a = pop_f(rez);
	return a;
}

node* derivation(node* root, int n) { //n=strlen(postfix), x-promenljiva po kojoj se diferencira
	char x;
	scanf("%c", &x);
	int i, j = 0;
	node* l, * r, * A, * B, * C, * D, * L, * R, * p = root;
	char a, b, c, d;
	stack* stek = createStack(n);
	stack* izvod = createStack(n);
	while (p) {
		push(stek, p);
		p = p->left;
	}
	while (!isEmpty(stek)) {
		p = pop(stek);
		if (p->level == 0) {
			p->level = 1;
			push(stek, p);
			p = p->right;
			while (p) {
				push(stek, p);
				p = p->left;
			}
		}
		else { //oznacavanje onih grana ciji je neki od sinova x
			p->level = 0;
			if (isOperand(p->value)) {
				if (p->value == x) { p->dx = 1; }
			}
			else {
				l = p->left; r = p->right;
				if (isbOperator(p->value)) {
					if ((l->dx == 1) || (r->dx == 1)) { p->dx = 1; }
				}
				if (isuOperator(p->value)) {
					if (l->dx == 1) { p->dx = 1; }
				}
			} 
			
//glavna obrada
			if (p->dx) {
				A = createNode(p->value);
				A->left = p->left;
				A->right = p->right;
				if (p->value == x) { 
					push(izvod, A); 
					p->value = '1'; }
		//sabiranje
				if (p->value == '+') {
					l = p->left; r = p->right; 
					if (l->dx && r->dx) {
						B = pop(izvod); C = pop(izvod);
						A->left = C; A->right = B;
					}
					if (l->dx && !r->dx) {
						B = pop(izvod);
						A->left = B; 
						p = rewriteNode(l, p);
					}
					if (!l->dx && r->dx) {
						B = pop(izvod);
						A->right = B;
						p = rewriteNode(r, p);
					}
					push(izvod, A);
				}
		//oduzimanje
				if (p->value == '-') {
					l = p->left; r = p->right;
					if (l->dx && r->dx) {
						B = pop(izvod); C = pop(izvod);
						A->left = C; A->right = B;
					}
					if (l->dx && !r->dx) {
						B = pop(izvod);
						A->left = B;
						p = rewriteNode(l, p);
					}
					if (!l->dx && r->dx) {
						B = pop(izvod);
						A->right = B;
						p = rewriteNode(r, p);
					}
					push(izvod, A);
				}
		//mnozenje
				if (p->value == '*') {
					l = p->left; r = p->right;
					if (l->dx && !r->dx) {
						B = pop(izvod);
						A->left = B;
					}
					if (!l->dx && r->dx) {
						B = pop(izvod);
						A->right = B;
					}
					if (l->dx && r->dx) {
						p->value = '+';
						a = '*';
						B = pop(izvod);
						C = pop(izvod);
						L = createNode(a);
						L->left = l;
						L->right = B;
						D = createNode(a);
						D->left = C;
						D->right = r;
						p->left = L;
						p->right = D;
						A->left = C;
						A->right = B;
					}
					push(izvod, A);
				}
		//deljenje
				if (p->value == '/') {
					l = p->left; r = p->right;
					if (l->dx && !r->dx) {
						B = pop(izvod);
						A->left = B;
					}
					if (!l->dx && r->dx) {
						a = '#';
						B = createNode(a);
						B->left = l;
						p->left = B;
						p->value = '*';
						b = '/';
						c = '^';
						B = createNode(b);
						B->left = r;
						C = createNode(c);
						D = top(izvod);
						C->left = D;
						a = '2';
						C->right = createNode(a);
						B->right = C;
						p->right = B;

						B = pop(izvod);
						A->right = B;
					}
					if (l->dx && r->dx) {
						a = '^';
						D = createNode(a);
						a = '2';
						L = createNode(a);
						D->right = L;
						B = pop(izvod);		A->right = B;
						D->left = B;
						p->right = D;
						a = '+';
						L = createNode(a);
						p->left = L;
						a = '*';
						D = createNode(a);
						L->left = D;
						D->right = r;
						C = pop(izvod);		A->left = C;
						D->left = C;
						D = createNode(a);
						L->right = D;
						D->right = B;
						D->left = l;
					}
					push(izvod, A);
				}
		//stepenovanje
				if (p->value == '^') {
					l = p->left; r = p->right;
					if (l->dx && !r->dx) {
						p->value = '*';
						p->left = r;
						a = '^';
						L = createNode(a);
						p->right = L;
						B = pop(izvod);		A->left = B;
						L->left = B;
						a = '-';
						D = createNode(a);
						L->right = D;
						a = '1';
						C = createNode(a);
						D->right = C;
						D->left = r;
						push(izvod, A);
					}
					if (!l->dx && r->dx) {
						p->value = '*';
						a = 'l';
						D = createNode(a);
						p->right = D;
						D->left = l;
						a = '^';
						L = createNode(a);
						p->left = L;
						L->left = l;
						B = pop(izvod); A->right = B;
						L->right = B;
						push(izvod, A);
					}
					if(l->dx && r->dx) { 
						printf("Nekorektan unos u stepenu funkciju. Povratak na prethodni unos"); 
						return root; }
				}
		//logaritam
				if (p->value == 'l') {
					B = pop(izvod);
					A->left = B;
					push(izvod, A);
					p->value = '/';
					p->right = B;
				}
		//sinus
				if (p->value == 's') {
					B = pop(izvod);
					A->left = B;
					push(izvod, A);
					p->value = '*';
					a = 'c';
					C = createNode(a);
					C->left = B;
					p->right = C;
				}
		//cosinus
				if (p->value == 'c') {
					B = pop(izvod);
					A->left = B;
					push(izvod, A);
					p->value = '*';
					a = '#';
					D = createNode(a);
					p->right = D;
					a = 's';
					C = createNode(a);
					D->left = C;
					C->left = B;
				}
		//minus
				if (p->value == '#') {
					B = pop(izvod);
					A->left = B;
					push(izvod, A);
				}
			}			
		}
	}
	free(stek->array);
	free(stek);
	free(izvod->array);
	free(izvod);
	return p;
}
//mala napomena, imam problema u main(), opcija 4 radi (izvod), nakon sto se ispise stablo treba uneti vrednost promenljive po kojoj se diferencira i sve radi
int main() {
	printf("DOBRODOSLI!\nUnesite izraz\n");
	int a = MAX;
	char inf[MAX];
	scanf("%s", inf);
	char* infix = transformInput(inf);
	char* postfix = infixToPostfix(infix, a);
	node* drvo = makeTree(postfix, a);
	operand* operatori;
	float b;
	while (1) {
		printf("\nGLAVNI MENI\nUnesite pocetni broj operacije koju zelite da izvrsite:\n");
		printf("0. Unos novog izraza\n1. Ispis formiranog stabla po nivoima\n");
		printf("2. Ispis unetog izraza u postfiksnoj notaciji\n");
		printf("3. Racunanje vrednosti unetog izraza\n4. Diferenciranje izraza\n5. Kraj programa\n");
		int n;
		scanf("%d", &n);
		if (n == 0) {
			deleteTree(drvo, a);
			scanf("%s", inf);
			infix = transformInput(inf);
			postfix = infixToPostfix(infix, a);
			drvo = makeTree(postfix, a);
			continue;
		}
		if (n == 1) {
			printf("\nStablo formatirano po nivoima:\n");
			printFormated(drvo, a);
			continue;
		}
		if (n == 2) {
			printf("\nPostfix: ");
			printPostfix(drvo, a);
			printf("\n");
			continue;
		}
		if (n == 3) {
			operatori = createOperandsList(postfix);
			operatori = insertValues(operatori);
			b = calculate(drvo, a, operatori);
			printf("\nVrednost izraza je: %f \n", b);
			deleteOperandsList(operatori);
			continue;
		}
		if (n == 4) {
			printf("\n\nUnesite promenljivu po kojoj zelite da diferencirate\n");
			drvo=derivation(drvo, a);
			printFormated(drvo,a);
		}
		if (n == 5) {
			break;
		}
	}
	deleteTree(drvo, a);
	printf("DOVIDJENJA!");

	return 0;
}
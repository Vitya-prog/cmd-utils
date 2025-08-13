#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#define NUMBER 1
#define CHAR 2


typedef struct {
	int value;
	char znak;
} Znak;

typedef struct {
	int type;
	union {
		float number;
		Znak znak;
	} u;
} Token;

Token token[50];
int out = 0;
int top = 49;

int sp = 0;
float stack[10];

int convertToRNP(char *s);
int getValueZnak(char z);
float calculate();
int getNumber(char *s,float *number);
void printStruct();
void push(float op);
float pop();

int size;

int main(int argc, char *argv[]){
	argv++;
	if(convertToRNP(*argv)!= -1)
		calculate();
	return 0;
}

float calculate()
{
	float op;
	for(int i = 0; i < out;i++)
	{
		switch(token[i].type) {
			case NUMBER:
				stack[sp++] = token[i].u.number;
				break;
			case CHAR:
				switch(token[i].u.znak.znak){
					case '-':
						op = pop();
						push(pop() - op);
						break;
					case '+':
						push(pop() + pop());
						break;
					case '/': 
						op = pop();
						push(pop()/op);
						break;
					case '*':
						push(pop()* pop());
						break;
				};
				break;
		};
	}
	for(int i = 0; i< sp;i++)
		printf("%.2f ",stack[i]);
	printf("\n");
	return 0;
}

void push(float op){
	if(sp > 9) 
		printf("Stack is full");
	else 
		stack[sp++] = op;
			
}

float pop()
{
	if(sp > 0)
		return stack[--sp];
	else 
		return 0.0;
}

int convertToRNP(char *s) {
    int capacity = 50;
    out = 0;
    top = capacity - 1; 

    while (*s != '\0') {
        if (*s == ' ' || *s == '\t') {
            s++;
            continue;
        }
		
		if (*s >= 'a' && *s <= 'z') {
		int i = 0;
		char f[100];
		while (*s >= 'a' && *s <= 'z' && *s != ' ' && *s != '\t') {
        f[i++] = *s;
        s++;
		}
		f[i] = '\0';
		
		}
		
		if (*s == '(') {
			token[top].type = CHAR;
			token[top].u.znak.znak = *s;
			token[top--].u.znak.value = -1; 
			s++;
			continue;
		}


        if (!isdigit(*s) && *s != '.') {
			if (*s == ')')
			{
				while(token[top + 1].u.znak.znak != '(')
				{
					token[out++] = token[++top];
				}
				top++;
			} else 
			{
            int curr_priority = getValueZnak(*s);

            while (top < capacity - 1 && token[top + 1].u.znak.value >= curr_priority) {
                token[out++] = token[++top];
            }
			
			token[top].type = CHAR;
			token[top].u.znak.znak = *s;
			token[top--].u.znak.value = curr_priority;	
							
			}


        }

        if (isdigit(*s) || *s == '.') {
			float number = 0.0;
			s += getNumber(s,&number);
            token[out].type = NUMBER;
            token[out++].u.number = number;
            continue; 
        }

        s++;
    }

    while (top < capacity - 1) {
        token[out++] = token[++top];
    }

    for (int j = 0; j < out; j++) {
        if (token[j].type == NUMBER)
            printf("%.2f ", token[j].u.number);
        if (token[j].type == CHAR)
            printf("%c ", token[j].u.znak.znak);
    }
    printf("\n");
	return 0;
}

int getNumber(char *s,float *number)
{	
    int i = 0;
	char num[100];
        while (isdigit(*s)) 
			num[i++] = *s++;
    if (*s == '.') {
        num[i++] = *s++;
        while (isdigit(*s)) 
			num[i++] = *s++;
    }
    num[i] = '\0';
	*number = atof(num);
	return i;
}


int getValueZnak(char z){
	switch(z){
	case '-':
		return 1;
	case '+':
		return 1;
	case '*':
		return 2;
	case '/':
		return 2;
	default:
		return -1;
	}
}


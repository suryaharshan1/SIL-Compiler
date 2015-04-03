#include <string.h>

#define STYPE_INT 1
#define STYPE_BOOLEAN 2
#define STYPE_ARR_INT 3
#define STYPE_ARR_BOOLEAN 4
#define STYPE_FUNCTION_INT 5
#define STYPE_FUNCTION_BOOLEAN 6
#define PTYPE_REF 7
#define PTYPE_VAL 8
#define ARG_INT 9
#define ARG_BOOLEAN 10
#define ARG_REF_INT 11
#define ARG_REF_BOOLEAN 12
#define STYPE_PAIR 13
#define ARG_PAIR 14
#define STYPE_FUNCTION_PAIR 15
#define ARG_REF_PAIR 16

struct Gsymbol {
    int type;  		//integer or boolean 
    int size;		// size in case of arrays
    int binding;	//binding	
    char* name;		//name of entry
    struct ArgStruct *Arglist;     
    struct Gsymbol *next;

};

struct Lsymbol {
    char* name;
    int type;		
    int binding;
    int size;
    struct Lsymbol *next;
};

struct ArgStruct {
	int type;
	char *name;
	int passType;
	struct ArgStruct *next;
};

int globalbind = 0;

int localbind = 1;

int argbind = -4;

int functionbind = 0;

struct Gsymbol *Ghead,*Gtemp;

struct Lsymbol *Lhead,*Ltemp;

struct ArgStruct *ArgHead,*Argtemp;

struct Gsymbol *Glookup(char* name);

void Ginstall(char* name, int type,int size,struct ArgStruct *Arglist);

struct Lsymbol *Llookup(char* name);

void Linstall(char* name, int type);

struct ArgStruct *Arginstall(char* name,int type,int passType);

void validateFunction(char* name, int type, struct ArgStruct *ArgList,struct Tnode *Body);

void ReinitialiseLocalVariables();

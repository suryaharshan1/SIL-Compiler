#include <string.h>

#define STYPE_INT 1
#define STYPE_BOOLEAN 2
#define STYPE_ARR_INT 3
#define STYPE_ARR_BOOLEAN 4

struct Gsymbol {
    int type;    
    int size;
    int binding;
    char* name;
    struct ArgStruct *Arglist;    
    struct Gsymbol *next;
};

struct Lsymbol {
    char* name;
    int type;
    int size;
    int binding;
    struct Lsymbol *next;
};

int mem_word_count = 0;

struct Gsymbol *Ghead,*Gtemp;

struct Lsymbol *Lhead,*Ltemp;

struct Gsymbol *Glookup(char* name);

void Ginstall(char* name, int type,int size,struct ArgStruct *Arglist);

struct Lsymbol *Llookup(char* name);

void Linstall(char* name, int type,int size);


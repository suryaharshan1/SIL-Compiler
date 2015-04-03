struct Gsymbol* Glookup(char* name){
    if(name == NULL){
        printf("Error : Cannot look up for an identifier with name NULL in global symbol table\n");
        exit(1);
    }
    struct Gsymbol *temp;
    temp = Ghead;
    while(temp != NULL && strcmp(temp->name,name) != 0){
   	 temp = temp->next;    
    }
    return temp;    
}

void Ginstall(char* name, int type,int size,struct ArgStruct *Arglist){
    /*checking whether already a variable of same name exists*/
    struct Gsymbol *check;
    check = Glookup(name);
    if(check != NULL)	//error on redefining the variable		
	  {
        yyerror("variable redefined ");
        printf(" %s",name);
        exit(0);
    }
    struct Gsymbol *temp;
    temp = (struct Gsymbol *)malloc(sizeof(struct Gsymbol));
    temp->name = (char *)malloc(sizeof(name));
    strcpy(temp->name,name);
    temp->type = type;
    temp->size = size;
    temp->Arglist = Arglist;
    temp->next = NULL;
    if(temp->type == STYPE_FUNCTION_INT || temp->type == STYPE_FUNCTION_BOOLEAN){
        temp->binding = functionbind;   //Make the appropriate value here
        functionbind++;
    }
    else if(temp->type == STYPE_INT || temp->type == STYPE_BOOLEAN || temp->type == STYPE_ARR_INT || temp->type == STYPE_ARR_BOOLEAN){
        temp->binding = globalbind;
        globalbind = globalbind + temp->size;
    }
    /*adding the node to symbol table entries*/
    if(Ghead == NULL){
   	 Ghead = temp;   	 
   	 return;    
    }
    temp->next = Ghead;
    Ghead = temp;
    return;
}

struct Lsymbol *Llookup(char* name){
    struct Lsymbol *temp;
    temp = Lhead;
    if(name == NULL){
        printf("Error : Cannot look up for an identifier with name NULL in local symbol table\n");
        exit(1);
    }
    while(temp != NULL && strcmp(temp->name,name) != 0){
   	 temp = temp->next;    
    }
    return temp;    
}

void Linstall(char* name, int type){
    struct Lsymbol *check;
    check = Llookup(name);
    if(check != NULL)   //error on redefining the variable      
      {
        yyerror("local variable redefined ");
        printf(" %s",name);
        exit(0);
    }
    struct Lsymbol *temp;
    temp = (struct Lsymbol *)malloc(sizeof(struct Lsymbol));
    temp->name = (char *)malloc(sizeof(name));
    strcpy(temp->name,name);
    temp->type = type;
    if(temp->type == ARG_INT || temp->type == ARG_BOOLEAN || temp->type == ARG_REF_INT || temp->type == ARG_REF_BOOLEAN){
        temp->binding = argbind; //Make the appropriate value here
        argbind--;
    }
    else if(temp->type == STYPE_INT || temp->type == STYPE_BOOLEAN){
        temp->binding = localbind;
        localbind++;
    }
    temp->next = NULL;
    if(Lhead == NULL){
   	 Lhead = temp;   	 
   	 return;    
    }
    temp->next = Lhead;
    Lhead = temp;
    return;    
}

struct ArgStruct *Arginstall(char* name,int type,int passType){
    struct ArgStruct *temp;
    temp = (struct ArgStruct *)malloc(sizeof(struct ArgStruct));
    temp->name = (char *)malloc(sizeof(name));
    strcpy(temp->name,name);
    temp->type = type;
    temp->passType = passType;
    temp->next = NULL;
	if(type == STYPE_INT){
        if(passType == PTYPE_REF)
            Linstall(name,ARG_REF_INT);
        else if(passType == PTYPE_VAL)
            Linstall(name,ARG_INT);
    }
    else if(type == STYPE_BOOLEAN){
        if(passType == PTYPE_REF)
            Linstall(name,ARG_REF_BOOLEAN);
        else if(passType == PTYPE_VAL)
            Linstall(name,ARG_BOOLEAN);
    }
    return temp;    
}

void validateFunction(char* name, int type, struct ArgStruct *ArgList,struct Tnode *Body){
    Gtemp = Glookup(name);

    /*check function is declared or not*/
    if(Gtemp == NULL){
        printf("Error : undefined function %s",name);
        exit(1);
    }
    
    /*return value type check*/
    if(!(Gtemp->type == STYPE_FUNCTION_INT && type == STYPE_INT) && !(Gtemp->type == STYPE_FUNCTION_BOOLEAN && type == STYPE_BOOLEAN)){
        printf("Error : Conflicting types of the function %s : type mismatch",name);
        exit(1);
    }
    
    if(!(Gtemp->type == STYPE_FUNCTION_INT && Body->Ptr2->TYPE == TYPE_INT) && !(Gtemp->type == STYPE_FUNCTION_BOOLEAN && Body->Ptr2->TYPE == TYPE_BOOLEAN)){
        printf("Error : Conflicting type of the function and returning type of the function %s : type mismatch",name);
        exit(1);
    }

    /*checking the type and name of arguments match with declaration , also sequence maintained as in decl*/
    Argtemp = Gtemp->Arglist;
    while(Argtemp != NULL && ArgList != NULL){ 
        if(strcmp(Argtemp->name,ArgList->name) != 0){
            printf("Error : Conflicts in function %s : mismatch in arguement name %s", name, Argtemp->name);
            exit(1);
        }
        if(Argtemp->type != ArgList->type){
            printf("Error : Conflicts in function %s : mismatch in arguement type %s",name,Argtemp->name);
            exit(1);
        }
        if(Argtemp->passType != ArgList->passType){
            printf("Error : Conflicts in function %s : mismatch in arguement passType %s", name,Argtemp->name);
            exit(1);
        }
        Argtemp = Argtemp->next;
        ArgList = ArgList->next; 
    }
    
    /*No of arguments in both declaration and definition must be same*/
    if(Argtemp != NULL || ArgList != NULL){
        printf("Error : Conflicts in function %s : mismatch in number of arguements", name);
        exit(1);
    }
}

void ReinitialiseLocalVariables(){
    Ltemp = Lhead;
    while(Ltemp != NULL){
        Lhead = Ltemp;
        Ltemp = Lhead->next;
        free(Lhead->name);
        free(Lhead);
    }
	Lhead = NULL;
	localbind = 1;
	argbind = -3;
}




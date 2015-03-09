struct Gsymbol* Glookup(char* name){
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
    /*
    switch(temp->type){
   	 case STYPE_INT:
   		 temp->binding = malloc(sizeof(int));
   		 break;
   	 case STYPE_BOOLEAN:
   		 temp->binding = malloc(sizeof(int));
   		 break;
   	 case STYPE_ARR_INT:
   		 temp->binding = malloc(sizeof(int)*(temp->size));
   		 break;
   	 case STYPE_ARR_BOOLEAN:
   		 temp->binding = malloc(sizeof(int)*(temp->size));
   		 break;
   	 default:
   		 printf("unknown type to allocate space in Gsymbol table\n");
   		 break;
    }
    memset(temp->binding,0,temp->size);    
    */
    
    temp->binding = mem_word_count;
    mem_word_count += temp->size;

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
    while(temp != NULL && strcmp(temp->name,name) != 0){
   	 temp = temp->next;    
    }
    return temp;    
}

void Linstall(char* name, int type,int size){
    struct Lsymbol *temp;
    temp = (struct Lsymbol *)malloc(sizeof(struct Lsymbol));
    temp->name = (char *)malloc(sizeof(name));
    strcpy(temp->name,name);
    temp->type = type;
    temp->size = size;
    temp->next = NULL;
    /*
    switch(temp->type){
   	 case STYPE_INT:
   		 temp->binding = malloc(sizeof(int));
   		 break;
   	 case STYPE_BOOLEAN:
   		 temp->binding = malloc(sizeof(int));
   		 break;
   	 case STYPE_ARR_INT:
   		 temp->binding = malloc(sizeof(int)*(temp->size));
   		 break;
   	 case STYPE_ARR_BOOLEAN:
   		 temp->binding = malloc(sizeof(int)*(temp->size));
   		 break;
   	 default:
   		 printf("unknown type to allocate space in Lsymbol table\n");
   		 break;
    }    
    memset(temp->binding,0,temp->size);    
    */
    temp->binding = mem_word_count;
    mem_word_count += temp->size;
    
    if(Lhead == NULL){
   	 Lhead = temp;   	 
   	 return;    
    }
    temp->next = Lhead;
    Lhead = temp;
    return;    
}




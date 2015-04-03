FILE *fp;

int reg_count = -1,label_count = -1;

/*
void generate_code(struct Tnode *tree){
	fprintf(fp, "START\n");
	fprintf(fp, "MOV SP 1000\n");	
	gen_code_eval(tree);
	fprintf(fp, "HALT\n");
	fclose(fp);
}
*/

int register_count(){
	return reg_count;
}

int get_register(){
	if(reg_count < 7){
		reg_count++;
		return reg_count;
	}
	else{
		printf("Error : No enough registers to get_register | all registers are busy\n");
		exit(1);
	}
}

void free_register(){
	if(reg_count >= -1){
		reg_count--;
	}
}

void free_all_register(){
	reg_count = -1;
}

int get_label(){
	label_count++;
	return label_count;
}

int gen_code_eval(struct Tnode *tree){
	int index,i,j,k;
	if(fp == NULL){
		printf("Error : Output file for code generation in gen_code_eval function\n");
		exit(1);
	}
	//printf("yes %d %d\n",tree->TYPE,tree->NODETYPE );
	switch(tree->TYPE){
		case TYPE_INT:
			switch(tree->NODETYPE){
   			 	case NODETYPE_PLUS:
	   				i = gen_code_eval(tree->Ptr1);
	   				j = gen_code_eval(tree->Ptr2);
	   				fprintf(fp, "ADD R%d R%d\n",i,j);
	   				free_register();
	   				return i;
	   				break;
	   		 	case NODETYPE_MINUS:
	   				i = gen_code_eval(tree->Ptr1);
	   				j = gen_code_eval(tree->Ptr2);
	   				fprintf(fp, "SUB R%d R%d\n",i,j);
	   				free_register();
	   				return i;
	   				break;
	   		 	case NODETYPE_MUL:
   				 	i = gen_code_eval(tree->Ptr1);
	   				j = gen_code_eval(tree->Ptr2);
	   				fprintf(fp, "MUL R%d R%d\n",i,j);
	   				free_register();
	   				return i;
   				 	break;
   			 	case NODETYPE_MOD:
   				 	i = gen_code_eval(tree->Ptr1);
	   				j = gen_code_eval(tree->Ptr2);
	   				fprintf(fp, "MOD R%d R%d\n",i,j);
	   				free_register();
	   				return i;
   				 	break;
   			 	case NODETYPE_DIV:
   				 	i = gen_code_eval(tree->Ptr1);
	   				j = gen_code_eval(tree->Ptr2);
	   				fprintf(fp, "DIV R%d R%d\n",i,j);
	   				free_register();
	   				return i;
   				 	break;
   			 	case NODETYPE_LEAF:
   				 	i = get_register();
   				 	fprintf(fp, "MOV R%d %d\n",i,tree->VALUE);
   				 	return i;
   				 	break;
	         	case NODETYPE_ID:
	            	//Check for errors???
	            	i = get_register();
	        		if(tree->Lentry == NULL){
	        			fprintf(fp,"MOV R%d [%d]\n",i,getBinding(tree));	            		
	        		}
	        		else {
	        			fprintf(fp, "MOV R%d %d\n",i,getBinding(tree));
	    				j = get_register();
	    				fprintf(fp, "MOV R%d BP\n",j);
	    				fprintf(fp, "ADD R%d R%d\n",i,j);
	    				free_register();
	        			fprintf(fp,"MOV R%d [R%d]\n",i,i);
	        			if(tree->Lentry->type == ARG_REF_INT){
		    				fprintf(fp,"MOV R%d [R%d]\n",i,i);
		        		}	
	        		}
					return i;
					break;
   			 	case NODETYPE_ARR_ID:
	           		i = get_register();
	           		fprintf(fp, "MOV R%d %d\n",i,getBinding(tree));
	           		j = gen_code_eval(tree->Ptr2);	//offset | index of the array
	           		fprintf(fp, "ADD R%d R%d\n",i,j);
	           		free_register();
	           		fprintf(fp, "MOV R%d [R%d]\n",i,i);
	           		return i;
	           		break;
				case NODETYPE_FUNCTION:
					i = generate_function_call(tree);
					//fprintf(fp,"OUT R%d\n",i);
					return i;
					break;
         		case NODETYPE_RET:
         			i = gen_code_eval(tree->Ptr1);
         			j = get_register();
         			k = get_register();
         			fprintf(fp, "MOV R%d BP\n",j);
         			fprintf(fp, "MOV R%d 2\n", k);
         			fprintf(fp, "SUB R%d R%d\n",j,k);
         			free_register();
         			fprintf(fp, "MOV [R%d] R%d\n",j,i);
         			free_register();
         			free_register();
         			break;
				case NODETYPE_MAIN:
					fprintf(fp,"START\n");
					fprintf(fp,"MOV SP 1000\n");
					fprintf(fp,"MOV BP SP\n");
					i = get_register();
				    Ltemp = Lhead;
    				while(Ltemp != NULL){
    					if(Ltemp->type != ARG_INT && Ltemp->type != ARG_BOOLEAN && Ltemp->type != ARG_REF_INT && Ltemp->type != ARG_REF_BOOLEAN)
        					fprintf(fp,"PUSH R%d\n",i); //allocating memory for each local varaiable
        					Ltemp = Ltemp->next;
    					}
					free_register();
					fprintf(fp,"MAIN:\n");
					gen_code_eval(tree->Ptr1);
					fprintf(fp,"HALT\n");
					break;
         		default:
   				 	printf("Error : Unknown NODETYPE under TYPE_INT\n");
   				 	exit(1);
   				 	break;
   		 	}
   			break;
		
		case TYPE_BOOLEAN:
			switch(tree->NODETYPE){
   				case NODETYPE_RET:
         			i = gen_code_eval(tree->Ptr1);
         			j = get_register();
         			k = get_register();
         			fprintf(fp, "MOV R%d BP\n",j);
         			fprintf(fp, "MOV R%d 2\n", k);
         			fprintf(fp, "SUB R%d R%d\n",j,k);
         			free_register();
         			fprintf(fp, "MOV [R%d] R%d\n",j,i);
         			free_register();
         			free_register();
         			break;
   				case NODETYPE_GT:	
					i = gen_code_eval(tree->Ptr1);
					j = gen_code_eval(tree->Ptr2);
					fprintf(fp,"GT R%d R%d\n",i,j);
					free_register();
					return i;					
   					break;
   				case NODETYPE_LT:
					i = gen_code_eval(tree->Ptr1);
					j = gen_code_eval(tree->Ptr2);
					fprintf(fp,"LT R%d R%d\n",i,j);									
					free_register();
					return i;   					
					break;
   			 	case NODETYPE_EQ:
					i = gen_code_eval(tree->Ptr1);
					j = gen_code_eval(tree->Ptr2);
					fprintf(fp,"EQ R%d R%d\n",i,j);
					free_register();
					return i;									
   					break;
   			 	case NODETYPE_GE:
           			i = gen_code_eval(tree->Ptr1);
					j = gen_code_eval(tree->Ptr2);
					fprintf(fp,"GE R%d R%d\n",i,j);
					free_register();
					return i;									
   					break;
         		case NODETYPE_LE:
           			i = gen_code_eval(tree->Ptr1);
					j = gen_code_eval(tree->Ptr2);
					fprintf(fp,"LE R%d R%d\n",i,j);
					free_register();
					return i;									
   					break;
         		case NODETYPE_NE:
           			i = gen_code_eval(tree->Ptr1);
					j = gen_code_eval(tree->Ptr2);
					fprintf(fp,"NE R%d R%d\n",i,j);
					free_register();
					return i;									
   					break;
				case NODETYPE_AND:
           			i = gen_code_eval(tree->Ptr1);
					j = gen_code_eval(tree->Ptr2);				
					fprintf(fp,"ADD R%d R%d\n",i,j);
					fprintf(fp,"MOV R%d 2\n",j);
					fprintf(fp,"EQ R%d R%d\n",i,j);
					free_register();
					return i;									
   					break;
				case NODETYPE_OR:
           			i = gen_code_eval(tree->Ptr1);
					j = gen_code_eval(tree->Ptr2);				
					fprintf(fp,"ADD R%d R%d\n",i,j);
					fprintf(fp,"MOV R%d 0\n",j);
					fprintf(fp,"GT R%d R%d\n",i,j);
					free_register();
					return i;									
   					break;
         		case NODETYPE_LEAF:
   					i = get_register();
					fprintf(fp, "MOV R%d %d\n",i,tree->VALUE);
   				 	return i;
   				 	break;
         		case NODETYPE_ID:
					//Check for errors???
					i = get_register();
					if(tree->Lentry == NULL){
	        			fprintf(fp,"MOV R%d [%d]\n",i,getBinding(tree));	            		
	        		}
	        		else {
	    				fprintf(fp, "MOV R%d %d\n",i,getBinding(tree));
	    				j = get_register();
	    				fprintf(fp, "MOV R%d BP\n",j);
	    				fprintf(fp, "ADD R%d R%d\n",i,j);
	    				free_register();
	        			fprintf(fp,"MOV R%d [R%d]\n",i,i);
	        			if(tree->Lentry->type == ARG_REF_BOOLEAN){
		    				fprintf(fp,"MOV R%d [R%d]\n",i,i);
		        		}	
	        		}
	        		return i;
           			break;
         		case NODETYPE_ARR_ID:
           			i = get_register();
	           		fprintf(fp, "MOV R%d %d\n",i,getBinding(tree));
	           		j = gen_code_eval(tree->Ptr2);	//offset | index of the array
	           		fprintf(fp, "ADD R%d R%d\n",i,j);
	           		free_register();
	           		fprintf(fp, "MOV R%d [R%d]\n",i,i);
	           		return i;
           			break;
				case NODETYPE_FUNCTION:
					i = generate_function_call(tree);
					//fprintf(fp,"OUT R%d\n",i);
					return i;
					break;
   			 	default:
   					printf("Error : Unknown NODETYPE under TYPE_BOOLEAN\n");
   				 	exit(1);
   					break;
   		 	}   		 
			break;

		case TYPE_VOID:
			switch(tree->NODETYPE){
				case NODETYPE_BODY:
					gen_code_eval(tree->Ptr1);
					gen_code_eval(tree->Ptr2);
					break;
				case NODETYPE_ASGN:
					i = gen_code_eval(tree->Ptr2);
					if(tree->Ptr1->Lentry == NULL){
	        			fprintf(fp, "MOV [%d] R%d\n",getBinding(tree),i);	            		
	        		}
	        		else {
	        			j = get_register();
	    				fprintf(fp, "MOV R%d %d\n",j,getBinding(tree));
	    				k = get_register();
	    				fprintf(fp, "MOV R%d BP\n",k);
	    				fprintf(fp, "ADD R%d R%d\n",j,k);
	    				free_register();
	        			if(tree->Ptr1->Lentry->type == ARG_REF_BOOLEAN || tree->Ptr1->Lentry->type == ARG_REF_INT){
		    				fprintf(fp,"MOV R%d [R%d]\n",j,j);
		        		}
	        			fprintf(fp,"MOV [R%d] R%d\n",j,i);
	        			free_register();	
	        		}
	        		free_register();
					break;
				case NODETYPE_ARR_ASGN:
   					i = get_register();
   					fprintf(fp, "MOV R%d %d\n",i,getBinding(tree));
	           		j = gen_code_eval(tree->Ptr2);	//offset | index of the array
	           		fprintf(fp, "ADD R%d R%d\n",i,j);
	           		free_register();
	           		j = gen_code_eval(tree->Ptr3);
	           		fprintf(fp, "MOV [R%d] R%d\n",i,j);
   					free_register();
   					free_register();
   					break;
				case NODETYPE_READ:
					i = get_register();
					fprintf(fp, "IN R%d\n",i);
					if(tree->Ptr1->Lentry == NULL){
			            fprintf(fp,"MOV [%d] R%d\n",getBinding(tree),i);
			        }
			        else {
			        	j = get_register();
	    				fprintf(fp, "MOV R%d %d\n",j,getBinding(tree));
	    				k = get_register();
	    				fprintf(fp, "MOV R%d BP\n",k);
	    				fprintf(fp, "ADD R%d R%d\n",j,k);
	    				free_register();
	        			if(tree->Ptr1->Lentry->type == ARG_REF_BOOLEAN || tree->Ptr1->Lentry->type == ARG_REF_INT){
		    				fprintf(fp,"MOV R%d [R%d]\n",j,j);
		        		}
	        			fprintf(fp,"MOV [R%d] R%d\n",j,i);
	        			free_register();
			        }
					free_register();
					break;
				case NODETYPE_ARR_READ:
   					i = get_register();
   					fprintf(fp, "MOV R%d %d\n",i,getBinding(tree));
	           		j = gen_code_eval(tree->Ptr2);	//offset | index of the array
	           		fprintf(fp, "ADD R%d R%d\n",i,j);
	           		free_register();
	           		j = get_register();
	           		fprintf(fp, "IN R%d\n",j);
					fprintf(fp,"MOV [R%d] R%d\n",i,j);
					free_register();
	           		free_register();
	           		break;
				case NODETYPE_WRITE:
					i = gen_code_eval(tree->Ptr1);
					fprintf(fp, "OUT R%d\n",i);
					free_register();
					break;
				case NODETYPE_IF:
					i = gen_code_eval(tree->Ptr1);
					j = get_label();
					fprintf(fp, "JZ R%d L%d\n",i,j);
					gen_code_eval(tree->Ptr2);
					if(tree->Ptr3 != NULL){	
						k = get_label();
						fprintf(fp,"JMP L%d\n",k);
						fprintf(fp,"L%d:\n",j);
						gen_code_eval(tree->Ptr3);
						fprintf(fp,"L%d:\n",k);
					}
					else{
						fprintf(fp,"L%d:\n",j);
					}
					free_register();						
					break;
				case NODETYPE_WHILE:
					i = get_label();
					k = get_label();
					fprintf(fp,"L%d:\n",i);
					j = gen_code_eval(tree->Ptr1);
					fprintf(fp,"JZ R%d L%d\n",j,k);
					free_register();
					gen_code_eval(tree->Ptr2);
					fprintf(fp,"JMP L%d\n",i);
					fprintf(fp,"L%d:\n",k);				
					break;
				case NODETYPE_ID:
					i = get_register();
					if(tree->Lentry == NULL){
	        			fprintf(fp,"MOV R%d [%d]\n",i,getBinding(tree));	            		
	        		}
	        		else {
	    				fprintf(fp, "MOV R%d %d\n",i,getBinding(tree));
	    				j = get_register();
	    				fprintf(fp, "MOV R%d BP\n",j);
	    				fprintf(fp, "ADD R%d R%d\n",i,j);
	    				free_register();
	        			fprintf(fp,"MOV R%d [R%d]\n",i,i);	
	        			if(tree->Lentry->type == ARG_REF_BOOLEAN || tree->Lentry->type == ARG_REF_INT){
		    				fprintf(fp,"MOV R%d [R%d]\n",i,i);
		        		}
	        		}
	        		return i;
					break;
				case NODETYPE_ARR_ID:
   					i = get_register();
	           		fprintf(fp, "MOV R%d %d\n",i,getBinding(tree));
	           		j = gen_code_eval(tree->Ptr2);	//offset | index of the array
	           		fprintf(fp, "ADD R%d R%d\n",i,j);
	           		free_register();
	           		fprintf(fp, "MOV R%d [R%d]\n",i,i);
	           		return i;
   					break;
				case NODETYPE_NONE:
					gen_code_eval(tree->Ptr1);
					gen_code_eval(tree->Ptr2);
					break;
				default:
					printf("Error : Unknown NODETYPE under TYPE_VOID\n");
					exit(1);
					break;
			}
			break;
	
		default:
			printf("Error : Incorrect TYPE of the node\n");
			exit(1);
			break;
	}
}


void codegen_function(char *name,struct Tnode *root){
	
	int i=0;
    
	Gtemp = Glookup(name);
    
	free_all_register();
	
    fprintf(fp,"LL%d:\n",Gtemp->binding);
    fprintf(fp,"PUSH BP\n");
    fprintf(fp,"MOV BP SP\n");
    
	i = get_register();
    Ltemp = Lhead;
    while(Ltemp != NULL){
    	if(Ltemp->type != ARG_INT && Ltemp->type != ARG_BOOLEAN && Ltemp->type != ARG_REF_INT && Ltemp->type != ARG_REF_BOOLEAN)
        	fprintf(fp,"PUSH R%d\n",i); //allocating memory for each local varaiable
        Ltemp = Ltemp->next;
    }
	free_register();

    /*or moving SP that many steps would do i guess*/
    
    /*code generation for function body*/
    gen_code_eval(root); 
    
    /*body contains return statement also , required to set the return value also*/
    
    /*pop all the local symbol table entries instead again we can just decrement the sp but may be no of arguments can be stored as function size*/

	i = get_register();	
    Ltemp = Lhead;
    while(Ltemp != NULL){
    	if(Ltemp->type != ARG_INT && Ltemp->type != ARG_BOOLEAN && Ltemp->type != ARG_REF_INT && Ltemp->type != ARG_REF_BOOLEAN)
    		fprintf(fp,"POP R%d\n",i);
    	Ltemp = Ltemp->next;
    }
	free_register();
	
    fprintf(fp,"POP BP\n");
    fprintf(fp,"RET\n");
}

int generate_function_call(struct Tnode *tree){
	/*push all the registers in use*/
	struct ArgStruct *Argtemp;	
	int push_reg_count,i,j;
	push_reg_count = 0;
	while(push_reg_count <= register_count()){
		fprintf(fp,"PUSH R%d\n",push_reg_count);
		push_reg_count = push_reg_count + 1;
	}
	push_reg_count--;
	
	Gtemp = Glookup(tree->NAME);		
	Argtemp = Gtemp->Arglist;
	
	//push all arguments into the stack
	struct Tnode *temp;
	temp = tree->ArgList;
	while(temp != NULL){
		if(Argtemp->passType == PTYPE_VAL){
			i = gen_code_eval(temp->Ptr2);
			fprintf(fp,"PUSH R%d\n",i);
			free_register();
		}
		else if(Argtemp->passType == PTYPE_REF){
			if(temp->Ptr2->Lentry != NULL){
				i = get_register();
				Ltemp = Llookup(temp->Ptr2->NAME);
				fprintf(fp,"MOV R%d %d\n",i,Ltemp->binding);
				j = get_register();
				fprintf(fp,"MOV R%d BP\n",j);
				fprintf(fp,"ADD R%d R%d\n",i,j);
				free_register();
				printf("%d\n",temp->Ptr2->Lentry->type);
				if(temp->Ptr2->Lentry->type == ARG_REF_INT || temp->Ptr2->Lentry->type == ARG_REF_BOOLEAN){
					fprintf(fp,"MOV R%d [R%d]\n",i,i);
					fprintf(fp,"PUSH R%d\n",i);				
				}
				else
					fprintf(fp, "PUSH R%d\n",i);
				free_register();
			}
			else{
				i = get_register();
				Gtemp = Glookup(temp->Ptr2->NAME);
				fprintf(fp,"MOV R%d %d\n",i,Gtemp->binding);
				fprintf(fp,"PUSH R%d\n",i);
				free_register();
			}
		}
		temp = temp->Ptr1;	
		Argtemp = Argtemp->next;
	}
		
	i = get_register();
	fprintf(fp,"PUSH R%d\n",i); //for return value
	free_register();	
	Gtemp = Glookup(tree->NAME);
	fprintf(fp,"CALL LL%d\n",Gtemp->binding); //space for return addr is pushed on execution of call statement
	
	i = get_register(); 	 //return value register to be returned
	fprintf(fp,"POP R%d\n",i);

	//fprintf(fp,"OUT R%d\n",i);
	
	//check for PASS type reference before popping off

	j = get_register();
	Gtemp = Glookup(tree->NAME);		
	Argtemp = Gtemp->Arglist;
	while(Argtemp != NULL){
		fprintf(fp,"POP R%d\n",j);
		Argtemp = Argtemp->next;
	}
	free_register();
	
	//pop all register values
	while(push_reg_count >= 0){
		fprintf(fp,"POP R%d\n",push_reg_count);
		push_reg_count = push_reg_count -1;
	}
				
	return i;		
}

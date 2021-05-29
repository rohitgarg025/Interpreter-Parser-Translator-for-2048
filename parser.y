%{
#include "driver.h"
#define MAX_VAR_LIST_SIZE 16

extern int yyparse();
extern int yylex();
extern int yyerror (char *s);
extern FILE* yyin;


State* state;

%}

%union{
    int num;
    char *str;
}

%start program

%token <str> ADD SUB MUL DIV LEFT RIGHT UP DOWN ASSIGN TO VAR IS VALUE IN DOT COMMA ID EXIT NEWLINE
%token <num> NUM




%%


program: %empty
        | EXIT {printf("2048>Bye!\n"); exit(0);}
        | assignment
        | naming 
        | query
        | move
        | NEWLINE {return 1;}
        | incomplete NEWLINE {printf("2048> ERROR You need to end a command with a full-stop.\n"); return 1;}
        | VAR keyword IS NUM COMMA NUM DOT NEWLINE { printf("2048> ERROR No, a keyword cannot be a variable name\n"); return 1;}
        | error NEWLINE{return 1;};

assignment: ASSIGN NUM TO NUM COMMA NUM DOT NEWLINE {
    int err=0;
    state = set_value(state,&err,$4-1,$6-1,$2);

    if(!err){
        printf("2048> Thanks, assignment done.\n");
    }
        
    return err;
    };


naming: VAR ID IS NUM COMMA NUM DOT NEWLINE {
    
        int err=0;
        state = assign_var_name(state,&err,$4-1,$6-1,$2);
        
        if(!err){
            printf("2048> Thanks, naming done.\n");
        }
        
        return err;
    
    };

query: VALUE IN NUM COMMA NUM DOT NEWLINE{
    int err=0;
    int x = get_value(state,&err,$3-1,$5-1);
    if(err != 1){
        printf("2048> The value is %d\n",x);
    }
        
        return err;
    };

move:   ADD UP DOT NEWLINE {state = move_up(state,0); printf("2048> Thanks, up move done, random tile added.\n"); return 0;}
    |   ADD DOWN DOT NEWLINE {state = move_down(state,0); printf("2048> Thanks, down move done, random tile added.\n"); return 0;}
    |   ADD RIGHT DOT NEWLINE {state = move_right(state,0); printf("2048> Thanks, right move done, random tile added.\n"); return 0;}
    |   ADD LEFT DOT NEWLINE {state = move_left(state,0); printf("2048> Thanks, left move done, random tile added.\n"); return 0;}
    |   SUB UP DOT NEWLINE {state = move_up(state,1); printf("2048> Thanks, up move done, random tile added.\n"); return 0;}
    |   SUB DOWN DOT NEWLINE {state = move_down(state,1); printf("2048> Thanks, down move done, random tile added.\n"); return 0;}
    |   SUB RIGHT DOT NEWLINE {state = move_right(state,1); printf("2048> Thanks, right move done, random tile added.\n"); return 0;}
    |   SUB LEFT DOT NEWLINE {state = move_left(state,1); printf("2048> Thanks, left move done, random tile added.\n"); return 0;}
    |   MUL UP DOT NEWLINE {state = move_up(state,2); printf("2048> Thanks, up move done, random tile added.\n"); return 0;}
    |   MUL DOWN DOT NEWLINE {state = move_down(state,2); printf("2048> Thanks, down move done, random tile added.\n"); return 0;}
    |   MUL RIGHT DOT NEWLINE {state = move_right(state,2); printf("2048> Thanks, right move done, random tile added.\n"); return 0;}
    |   MUL LEFT DOT NEWLINE {state = move_left(state,2); printf("2048> Thanks, left move done, random tile added.\n"); return 0;}
    |   DIV UP DOT NEWLINE {state = move_up(state,3); printf("2048> Thanks, up move done, random tile added.\n"); return 0;}
    |   DIV DOWN DOT NEWLINE {state = move_down(state,3); printf("2048> Thanks, down move done, random tile added.\n"); return 0;}
    |   DIV RIGHT DOT NEWLINE {state = move_right(state,3); printf("2048> Thanks, right move done, random tile added.\n"); return 0;}
    |   DIV LEFT DOT NEWLINE {state = move_left(state,3); printf("2048> Thanks, left move done, random tile added.\n"); return 0;}
    ;

keyword: ADD | SUB | MUL | DIV | LEFT | RIGHT | UP | DOWN | ASSIGN | TO | VAR | IS | VALUE | IN | DOT | COMMA | EXIT; 

incomplete: ADD UP | ADD DOWN | ADD RIGHT | ADD LEFT | SUB UP | SUB DOWN | SUB RIGHT | SUB LEFT | MUL UP |MUL DOWN |MUL RIGHT |MUL LEFT | DIV UP |DIV DOWN |DIV RIGHT |DIV LEFT|
            VALUE IN NUM COMMA NUM | VAR ID IS NUM COMMA NUM | ASSIGN NUM TO NUM COMMA NUM; 

%%

int yyerror(char *s)
{
	printf("2048> ERROR: Syntax Error\n");
    // fprintf(stderr, "-1\n");
    return 1;
	
}

int getValueErrorConst(int err){
return (err == 5);
}

int inValidMoveErrorConst(int err){
return (err == 1);
}


int main()
{   
    
    printf("2048> Hi I'm your 2048 game engine.\n");
    printf("2048> The start state is:\n");
    state = (State*)malloc(sizeof(State));
    init_board(state);
    random_tile(state);
    print_board(state);
    print_error_board(state);

    yyin = stdin;
    do {
        printf("2048> Please type a command!\n");
        int error_code = 1;
        while(getValueErrorConst(error_code) || inValidMoveErrorConst(error_code) || error_code == 2)
        {
            printf("----> ");
            error_code = yyparse();
            if(error_code == 1 || error_code == 2){
                fprintf(stderr, "-1\n");
            }

        }
        printf("2048> The current state is:\n");
    print_board(state);
    print_error_board(state);

	} while(!feof(yyin));
    return 0;
}
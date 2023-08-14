#include "stdio.h"
#include "stdlib.h"
#define SV_IMPLEMENTATION
#include "./sv.h"

#define PROG_MAX 200
#define MAX_PROCS 200
#define MAX_REC_DEPTH 100
#define MAX_PROC_NUM 200
typedef struct {
  int col;
  int line;
  String_View filename;
} Position;

typedef enum {
  OP_PUSH = 0,
  OP_ADD,
  OP_SUB,
  OP_MUL,
  OP_DUP,
  OP_DIV,
  OP_MOD,
  OP_EQUAL,
  OP_L,
  OP_G,
  OP_DROP,
  OP_SWAP,
  OP_PRINT,
  OP_NOT,
  OP_IF,
  OP_END,
  OP_WHILE,
  OP_DO,
  OP_INT,
  OP_STORE,
  OP_FETCH,
  OP_PROC,
  OP_COUNT,
} Token_Op;

typedef enum {
  TYPE_INT = 0,
  TYPE_BOOL,
  TYPE_COUNT,
} Type;

typedef struct {
  Position pos;
  String_View lexeme;
  Token_Op op;
  Type type;
  int cross_ref;
  void *data;
} Token;

typedef struct {
  Token toks[PROG_MAX];
  int length;
} Program;

String_View read_file(char *filename);

typedef struct {
  int data[PROG_MAX];
  int l;
} int_stack;

void int_push(int_stack *stack, int a);
int int_pop(int_stack *stack);
int int_peek(int_stack *stack);
String_View int_to_sv(int a);
Program get_prog_from_file(char *filename);
Program parseProg(String_View *lexemes, int count);
void push(Program *program, Token token);
Token pop(Program *program);
Token peek(Program *program);
Token void_tok(void);
int compile(Program prog, char *filename);
void sysprintf(const char *format, ...);

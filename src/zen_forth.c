#include "./zen_forth.h"
#include <assert.h>
#include <stdarg.h>
#include <string.h>

String_View read_file(char *filename)
{
    char *buffer;
    size_t length;
    FILE *f = fopen(filename, "rb");
    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);

        buffer = (char *)malloc(length);
        if (buffer)
        {
            fread(buffer, 1, length, f);
        }
        fclose(f);
    }
    else
    {
        printf("Unable to read input file %s!\n", filename);
        return (String_View){.count = 0};
    }
    const char *data = buffer;
    return sv_from_parts(data, length);
}

bool is_separator(char c)
{
    if (c == ' ' || c == '\n' || c == '+' || c == '.' || c == '=')
        return true;
    return false;
}

bool is_not_separator(char c)
{
    return !is_separator(c);
}

bool is_whitespace(char c)
{
    if (c == ' ' || c == '\n')
        return true;
    return false;
}

bool is_not_whitespace(char c)
{
    return !is_whitespace(c);
}

void push(Program *program, Token token)
{
    assert(program->length + 1 < PROG_MAX);
    program->toks[program->length++] = token;
}

Token pop(Program *program)
{
    assert(program->length > 0);
    return program->toks[--program->length];
}

Token peek(Program *program)
{
    assert(program->length > 0);
    return program->toks[program->length - 1];
}

bool is_sv_useless(String_View sv)
{
    if (sv.count == 0)
        return true;

    if (sv.count == 1 && is_whitespace(sv.data[0]))
        return true;
    return false;
}

bool is_num(char c)
{
    return '0' <= c && c <= '9';
}

int pow_ten(int i)
{
    int res = 1;
    for (int j = 1; j < i; j++)
    {
        res *= 10;
    }
    return res;
}

int sv_to_int(String_View sv)
{
    if (sv.count == 0)
        return 0;
    int res = 0;
    for (size_t i = 0; i < sv.count; ++i)
    {
        assert(is_num(sv.data[i]));
        res += (int)(sv.data[i] - 0x30) * pow_ten(sv.count - (int)i);
    }
    return res;
}

Token_Op type_of_lexeme(String_View lexeme)
{
    assert(lexeme.count > 0);
    assert(OP_COUNT == 22 && "Exhaustive handling of operations in type_of_lexeme.");
    if (lexeme.count == 1)
    {
        if (lexeme.data[0] == '+')
            return OP_ADD;
        if (lexeme.data[0] == '-')
            return OP_SUB;
        if (lexeme.data[0] == '*')
            return OP_MUL;
        if (lexeme.data[0] == '/')
            return OP_DIV;
        if (lexeme.data[0] == '%')
            return OP_MOD;
        if (lexeme.data[0] == '.')
            return OP_PRINT;
        if (lexeme.data[0] == '=')
            return OP_EQUAL;
        if (lexeme.data[0] == '<')
            return OP_L;
        if (lexeme.data[0] == '>')
            return OP_G;
        if (lexeme.data[0] == '!')
            return OP_STORE;
        if (lexeme.data[0] == '@')
            return OP_FETCH;
    }
    if (sv_eq(lexeme, sv_from_cstr("not")))
        return OP_NOT;
    if (sv_eq(lexeme, sv_from_cstr("dup")))
        return OP_DUP;
    if (sv_eq(lexeme, sv_from_cstr("drop")))
        return OP_DUP;
    if (sv_eq(lexeme, sv_from_cstr("swap")))
        return OP_SWAP;
    if (sv_eq(lexeme, sv_from_cstr("if")))
        return OP_IF;
    if (sv_eq(lexeme, sv_from_cstr("while")))
        return OP_WHILE;
    if (sv_eq(lexeme, sv_from_cstr("do")))
        return OP_DO;
    if (sv_eq(lexeme, sv_from_cstr("end")))
        return OP_END;
    if (sv_eq(lexeme, sv_from_cstr("int")))
        return OP_INT;
    if (sv_eq(lexeme, sv_from_cstr("proc")))
        return OP_PROC;
    return OP_PUSH;
}

bool is_sv_num(String_View sv)
{
    for (size_t i = 0; i < sv.count; ++i)
    {
        if (!is_num(sv.data[i]))
        {
            return false;
        }
    }
    return true;
}

Token void_tok(void)
{
    Token tok;
    Position pos;
    pos.col = 0;
    pos.line = 0;
    pos.filename = sv_from_cstr("");
    tok.lexeme = sv_from_cstr("");
    tok.op = OP_COUNT;
    tok.pos = pos;
    tok.type = TYPE_INT;
    return tok;
}

Program parseProg(String_View *lexemes, int count)
{
    Program prog;
    prog.length = 0;
    for (int i = 0; i < PROG_MAX; i++)
    {
        prog.toks[i] = void_tok();
    }
    for (int i = 0; i < count; i++)
    {
        Position pos;
        pos.col = 0;
        pos.line = 0;
        pos.filename = sv_from_cstr("");
        prog.toks[prog.length].lexeme = lexemes[i];
        if (is_sv_num(prog.toks[prog.length].lexeme))
        {
            prog.toks[prog.length].type = TYPE_INT;
            prog.toks[prog.length].data = malloc(sizeof(int));
            *(int *)prog.toks[prog.length].data = sv_to_int(prog.toks[prog.length].lexeme);
        }
        else if (sv_eq(prog.toks[prog.length].lexeme, sv_from_cstr("true")))
        {
            prog.toks[prog.length].type = TYPE_BOOL;
            prog.toks[prog.length].data = malloc(sizeof(int));
            *(bool *)prog.toks[prog.length].data = true;
        }
        else if (sv_eq(prog.toks[prog.length].lexeme, sv_from_cstr("false")))
        {
            prog.toks[prog.length].type = TYPE_BOOL;
            prog.toks[prog.length].data = malloc(sizeof(int));
            *(bool *)prog.toks[prog.length].data = false;
        }
        prog.toks[prog.length].op = type_of_lexeme(lexemes[i]);
        prog.toks[prog.length].pos = pos;
        prog.length++;
    }
    return prog;
}

bool sv_contains(String_View sv, char c)
{
    for (size_t i = 0; i < sv.count; ++i)
    {
        if (sv.data[i] == c)
        {
            return true;
        }
    }
    return false;
}

Program get_prog_from_file(char *filename)
{
    String_View lexemes[100];
    int l_i = 0;
    String_View file = read_file(filename);
    String_View temp;
    String_View delim;
    bool in_comment = false;
    while (l_i < PROG_MAX && file.count > 0)
    {
        if (file.count == 0)
            break;
        temp = sv_chop_left_while(&file, is_not_separator);
        delim = sv_chop_left_while_separator(&file, is_separator);
        if (in_comment && temp.count > 0 && temp.data[0] == '\n')
            in_comment = false;
        if (in_comment && delim.count > 0 && delim.data[0] == '\n')
            in_comment = false;
        if (sv_contains(temp, '#'))
            in_comment = true;
        if (sv_contains(delim, '#'))
            in_comment = true;
        if (!in_comment)
        {
            if (!is_sv_useless(temp))
                lexemes[l_i++] = temp;
            if (!is_sv_useless(delim))
                lexemes[l_i++] = delim;
        }
    }
    Program prog = parseProg(lexemes, l_i);
    return prog;
}

String_View int_to_sv(int a)
{
    char buff[20];
    int n = 0;
    while (a > 0)
    {
        buff[n] = a % 10 + 0x30;
        a = a / 10;
        n++;
    }
    int i = n;
    char *buff2 = malloc(sizeof(char) * n);
    while (i > 0)
    {
        buff2[n - i] = buff[i - 1];
        i--;
    }
    return sv_from_parts(buff2, n);
}

void int_push(int_stack *stack, int a)
{
    assert(stack->l < PROG_MAX - 1);
    stack->data[stack->l++] = a;
}

int int_pop(int_stack *stack)
{
    assert(stack->l > 0);
    return stack->data[--stack->l];
}

int int_peek(int_stack *stack)
{
    assert(stack->l > 0);
    return stack->data[stack->l - 1];
}

void dump_def(FILE *out)
{
    fprintf(out, "dump:\n");
    fprintf(out, "        mov     r9, -3689348814741910323\n");
    fprintf(out, "        sub     rsp, 40\n");
    fprintf(out, "        mov     BYTE [rsp+31], 10\n");
    fprintf(out, "        lea     rcx, [rsp+30]\n");
    fprintf(out, ".L2:\n");
    fprintf(out, "        mov     rax, rdi\n");
    fprintf(out, "        lea     r8, [rsp+32]\n");
    fprintf(out, "        mul     r9\n");
    fprintf(out, "        mov     rax, rdi\n");
    fprintf(out, "        sub     r8, rcx\n");
    fprintf(out, "        shr     rdx, 3\n");
    fprintf(out, "        lea     rsi, [rdx+rdx*4]\n");
    fprintf(out, "        add     rsi, rsi\n");
    fprintf(out, "        sub     rax, rsi\n");
    fprintf(out, "        add     eax, 48\n");
    fprintf(out, "        mov     BYTE [rcx], al\n");
    fprintf(out, "        mov     rax, rdi\n");
    fprintf(out, "        mov     rdi, rdx\n");
    fprintf(out, "        mov     rdx, rcx\n");
    fprintf(out, "        sub     rcx, 1\n");
    fprintf(out, "        cmp     rax, 9\n");
    fprintf(out, "        ja      .L2\n");
    fprintf(out, "        lea     rax, [rsp+32]\n");
    fprintf(out, "        mov     edi, 1\n");
    fprintf(out, "        sub     rdx, rax\n");
    fprintf(out, "        lea     rsi, [rsp+32+rdx]\n");
    fprintf(out, "        mov     rdx, r8\n");
    fprintf(out, "        mov     rax, 1\n");
    fprintf(out, "        syscall\n");
    fprintf(out, "        add     rsp, 40\n");
    fprintf(out, "        ret\n");
}

void add_def(FILE *out)
{
    fprintf(out, "    ;; ADD\n");
    fprintf(out, "    pop rax\n");
    fprintf(out, "    pop rbx\n");
    fprintf(out, "    add rbx, rax\n");
    fprintf(out, "    push rbx\n");
}

void sub_def(FILE *out)
{
    fprintf(out, "    ;; SUB\n");
    fprintf(out, "    pop rax\n");
    fprintf(out, "    pop rbx\n");
    fprintf(out, "    sub rbx, rax\n");
    fprintf(out, "    push rbx\n");
}

void mul_def(FILE *out)
{
    fprintf(out, "    ;; MUL\n");
    fprintf(out, "    pop rdx\n");
    fprintf(out, "    pop rax\n");
    fprintf(out, "    mul rdx\n");
    fprintf(out, "    push rax\n");
}

void div_def(FILE *out)
{
    fprintf(out, "    ;; DIV\n");
    fprintf(out, "    mov rdx, 0\n");
    fprintf(out, "    pop rcx\n");
    fprintf(out, "    pop rax\n");
    fprintf(out, "    div rcx\n");
    fprintf(out, "    push rax\n");
}

void mod_def(FILE *out)
{
    fprintf(out, "    ;; DIV\n");
    fprintf(out, "    mov rdx, 0\n");
    fprintf(out, "    pop rcx\n");
    fprintf(out, "    pop rax\n");
    fprintf(out, "    div rcx\n");
    fprintf(out, "    push rdx\n");
}

void print_def(FILE *out)
{
    fprintf(out, "    ;; PRINT\n");
    fprintf(out, "    pop rdi\n");
    fprintf(out, "    call dump\n");
}

void not_def(FILE *out)
{
    fprintf(out, "    ;; NOT\n");
    fprintf(out, "    pop rax\n");
    fprintf(out, "    cmp rax, 0\n");
    fprintf(out, "    mov rcx, 0\n");
    fprintf(out, "    mov rdx, 1\n");
    fprintf(out, "    cmove rcx, rdx\n");
    fprintf(out, "    push rcx\n");
}

void dup_def(FILE *out)
{
    fprintf(out, "    ;; DUP\n");
    fprintf(out, "    pop rax\n");
    fprintf(out, "    push rax\n");
    fprintf(out, "    push rax\n");
}

void drop_def(FILE *out)
{
    fprintf(out, "    ;; DROP\n");
    fprintf(out, "    pop rax\n");
}

void swap_def(FILE *out)
{
    fprintf(out, "    ;; SWAP\n");
    fprintf(out, "    pop rax\n");
    fprintf(out, "    pop rbx\n");
    fprintf(out, "    push rax\n");
    fprintf(out, "    push rbx\n");
}

void equal_def(FILE *out)
{
    fprintf(out, "    ;; EQUAL\n");
    fprintf(out, "    pop rax\n");
    fprintf(out, "    pop rbx\n");
    fprintf(out, "    cmp rax, rbx\n");
    fprintf(out, "    mov rcx, 0\n");
    fprintf(out, "    mov rdx, 1\n");
    fprintf(out, "    cmove rcx, rdx\n");
    fprintf(out, "    push rcx\n");
}

void l_def(FILE *out)
{
    fprintf(out, "    ;; LESS\n");
    fprintf(out, "    pop rax\n");
    fprintf(out, "    pop rbx\n");
    fprintf(out, "    cmp rbx, rax\n");
    fprintf(out, "    mov rcx, 0\n");
    fprintf(out, "    mov rdx, 1\n");
    fprintf(out, "    cmovl rcx, rdx\n");
    fprintf(out, "    push rcx\n");
}

void g_def(FILE *out)
{
    fprintf(out, "    ;; GREATER\n");
    fprintf(out, "    pop rax\n");
    fprintf(out, "    pop rbx\n");
    fprintf(out, "    cmp rbx, rax\n");
    fprintf(out, "    mov rcx, 0\n");
    fprintf(out, "    mov rdx, 1\n");
    fprintf(out, "    cmovg rcx, rdx\n");
    fprintf(out, "    push rcx\n");
}
void store_def(FILE *out)
{
    fprintf(out, "    ;; STORE\n");
    fprintf(out, "    pop rbx\n");
    fprintf(out, "    pop rax\n");
    fprintf(out, "    mov [rbx], rax\n");
}
void fetch_def(FILE *out)
{
    fprintf(out, "    ;; FETCH\n");
    fprintf(out, "    pop rbx\n");
    fprintf(out, "    mov rax, [rbx]\n");
    fprintf(out, "    push rax\n");
}

void exit_def(FILE *out)
{
    fprintf(out, "    ;; EXIT PROGRAM\n");
    fprintf(out, "    mov rax, 60\n");
    fprintf(out, "    mov rdi, 0\n");
    fprintf(out, "    syscall\n");
}

int get_total_var_size(int_stack sizes)
{
    int acc = 0;
    while (sizes.l > 0)
    {
        acc += int_pop(&sizes);
    }
    return acc;
}

int get_var_offset(String_View id, Program vars, int_stack sizes)
{
    int offset = 0;
    int length = get_total_var_size(sizes);

    Token var;
    while (vars.length > 0)
    {

        offset += int_pop(&sizes);
        var = pop(&vars);
        if (sv_eq(id, var.lexeme))
        {
            break;
        }
    }
    if (!sv_eq(id, var.lexeme))
    {
        fprintf(stderr, "ERROR: local variable `" SV_Fmt "` is not declared in the current scope.\n", SV_Arg(id));
    }
    return length - offset;
}

typedef struct
{
    Program progs[MAX_PROC_NUM];
    size_t l;
} prog_stack;

void prog_push(prog_stack *p, Program prog)
{
    assert(p->l < MAX_PROC_NUM - 1);
    p->progs[p->l++] = prog;
}

Program prog_pop(prog_stack *p)
{
    assert(p->l > 0);
    return p->progs[--p->l];
}

prog_stack get_procs_from_prog(Program prog)
{
    prog_stack procs;
    procs.l = 0;
    int scope = 0;
    bool in_proc = false;
    Program curr_prog;
    curr_prog.length = 0;
    for (int i = 0; i < prog.length; ++i)
    {

        Token tok = prog.toks[i];
        push(&curr_prog, tok);
        if (tok.op == OP_PROC)
        {
            scope++;
            if (in_proc)
            {
                fprintf(stderr, "Error: Can't declare procedure inside of another procedure.\n");
            }
            in_proc = true;
        }
        else if (tok.op == OP_IF || tok.op == OP_WHILE)
        {
            scope++;
        }
        else if (tok.op == OP_END)
        {
            scope--;
            if (scope == 0)
            {
                in_proc = false;
                prog_push(&procs, curr_prog);
                curr_prog.length = 0;
            }
        }
    }
    return procs;
}

Program get_procless_prog(Program prog)
{
    int scope = 0;
    bool in_proc = false;
    Program toret;
    toret.length = 0;
    for (int i = 0; i < prog.length; ++i)
    {
        Token tok = prog.toks[i];

        if (tok.op == OP_PROC)
        {
            scope++;
            in_proc = true;
        }
        if (!in_proc)
        {
            push(&toret, tok);
        }
        if (tok.op == OP_IF || tok.op == OP_WHILE)
        {
            scope++;
        }
        if (tok.op == OP_END)
        {
            scope--;
            if (scope == 0 && in_proc)
            {
                in_proc = false;
            }
        }
    }
    return toret;
}

bool sv_in_arr(String_View sv, String_View *arr, int length)
{
    for (int i = 0; i < length; ++i)
    {
        if (sv_eq(sv, arr[i]))
            return true;
    }
    return false;
}

int write_asm(Program prog, FILE *out, String_View *names, int l)
{
    assert(OP_COUNT == 22 && "Exhaustive handling of operations in compile.");

    int if_counter = 0;
    int wh_counter = 0;
    int_stack if_stack;
    int_stack wh_stack;
    int_stack scope;
    scope.l = 0;
    if_stack.l = 0;
    wh_stack.l = 0;
    int iff = 0;
    int whi = 1;
    int pro = 2;
    Type var_type = TYPE_BOOL;
    bool in_var = false;

    int_stack loc_var_sizes;
    int_stack loc_var_n;
    Program loc_vars;
    loc_var_sizes.l = 0;
    loc_var_n.l = 0;
    loc_vars.length = 0;
    int max_loc_var = 0;

    // bool is_proc = false;
    int i = 0;
    if (prog.toks[0].op == OP_PROC)
    {
        int_push(&loc_var_n, 0);
        int_push(&scope, pro);
        // is_proc = true;
        i = 2;
    }
    for (; i < prog.length; ++i)
    {
        int total_size = get_total_var_size(loc_var_sizes);
        Token tok = prog.toks[i];

        if (total_size > max_loc_var)
        {
            max_loc_var = total_size;
        }
        if (tok.op == OP_PUSH)
        {
            if (sv_in_arr(tok.lexeme, names, l))
            {
                fprintf(out, "    ;; CALL " SV_Fmt "\n", SV_Arg(tok.lexeme));
                fprintf(out, "    mov rax, rsp\n");
                fprintf(out, "    mov rsp, [ret_stack_rsp]\n");
                fprintf(out, "    call proc_" SV_Fmt "\n", SV_Arg(tok.lexeme));
                fprintf(out, "    mov [ret_stack_rsp], rsp\n");
                fprintf(out, "    mov rsp, rax\n");
            }
            else if (in_var && !is_sv_num(tok.lexeme))
            {

                if (!sv_in_arr(tok.lexeme, names, l))
                {
                    if (var_type == TYPE_INT)
                    {
                        int_push(&loc_var_sizes, 16);
                    }
                    else if (var_type == TYPE_BOOL)
                    {
                        int_push(&loc_var_sizes, 16);
                    }
                    in_var = false;
                    tok.type = var_type;
                    loc_var_n.data[loc_var_n.l - 1]++;
                    push(&loc_vars, tok);
                }
            }
            else if (!is_sv_num(tok.lexeme) && tok.type != TYPE_BOOL)
            {
                fprintf(out, "    ;; PUSH\n");
                int offset = get_var_offset(tok.lexeme, loc_vars, loc_var_sizes);
                fprintf(out, "    mov rbx, vars + %d\n", offset);
                fprintf(out, "    mov rax, [proc_depth]\n");
                fprintf(out, "    add rbx, rax\n");
                fprintf(out, "    push rbx\n");
            }
            else
            {
                fprintf(out, "    ;; PUSH\n");
                if (tok.type == TYPE_INT)
                {
                    fprintf(out, "    push %d\n", *(int *)tok.data);
                }
                else if (tok.type == TYPE_BOOL && sv_eq(tok.lexeme, sv_from_cstr("true")))
                    fprintf(out, "    push 1\n");
                else if (tok.type == TYPE_BOOL && sv_eq(tok.lexeme, sv_from_cstr("false")))
                    fprintf(out, "    push 0\n");
                else
                    fprintf(out, "    push " SV_Fmt "\n", SV_Arg(tok.lexeme));
            }
        }
        else if (tok.op == OP_ADD)
        {
            add_def(out);
        }
        else if (tok.op == OP_SUB)
        {
            sub_def(out);
        }
        else if (tok.op == OP_MUL)
        {
            mul_def(out);
        }
        else if (tok.op == OP_DIV)
        {
            div_def(out);
        }
        else if (tok.op == OP_MOD)
        {
            mod_def(out);
        }
        else if (tok.op == OP_PRINT)
        {
            print_def(out);
        }
        else if (tok.op == OP_NOT)
        {
            not_def(out);
        }
        else if (tok.op == OP_DUP)
        {
            dup_def(out);
        }
        else if (tok.op == OP_DROP)
        {
            drop_def(out);
        }
        else if (tok.op == OP_SWAP)
        {
            swap_def(out);
        }
        else if (tok.op == OP_EQUAL)
        {
            equal_def(out);
        }
        else if (tok.op == OP_L)
        {
            l_def(out);
        }
        else if (tok.op == OP_G)
        {
            g_def(out);
        }
        else if (tok.op == OP_INT)
        {
            var_type = TYPE_INT;
            in_var = true;
        }
        else if (tok.op == OP_STORE)
        {
            store_def(out);
        }
        else if (tok.op == OP_FETCH)
        {
            fetch_def(out);
        }
        else if (tok.op == OP_IF)
        {
            fprintf(out, "    ;; IF\n");
            fprintf(out, "    pop rax\n");
            fprintf(out, "    cmp rax, 0\n");
            fprintf(out, "    je .IF%d\n", if_counter);
            int_push(&if_stack, if_counter);
            int_push(&scope, iff);
            int_push(&loc_var_n, 0);
            if_counter++;
        }
        else if (tok.op == OP_END)
        {
            int count = int_pop(&loc_var_n);
            for (int var_n = 0; var_n < count; var_n++)
            {
                int_pop(&loc_var_sizes);
                pop(&loc_vars);
            }
            fprintf(out, "    ;; END\n");
            int a = int_pop(&scope);
            if (a == iff)
            {
                fprintf(out, ".IF%d:\n", int_pop(&if_stack));
            }
            else if (a == whi)
            {
                int wh_i = int_pop(&wh_stack);
                fprintf(out, "    jmp .DO%d\n", wh_i);
                fprintf(out, ".WHILE%d:\n", wh_i);
            }
        }
        else if (tok.op == OP_WHILE)
        {
            fprintf(out, "    ;; WHILE\n");
            fprintf(out, ".DO%d:\n", wh_counter);
            int_push(&wh_stack, wh_counter);
            int_push(&scope, whi);
            int_push(&loc_var_n, 0);

            wh_counter++;
        }
        else if (tok.op == OP_DO)
        {
            fprintf(out, "    ;; DO\n");
            fprintf(out, "    pop rax\n");
            fprintf(out, "    cmp rax, 0\n");
            fprintf(out, "    je .WHILE%d\n", int_peek(&wh_stack));
        }
    }
    return 0;
}

int get_max_loc_var(Program prog)
{
    (void)prog;
    return 16;
}

int compile(Program src, char *filename)
{
    Program main = get_procless_prog(src);
    prog_stack procs = get_procs_from_prog(src);
    String_View procs_names[procs.l];
    int num_procs = procs.l;
    for (size_t i = 0; i < procs.l; i++)
    {
        procs_names[i] = procs.progs[i].toks[1].lexeme;
    }
    int max_loc_var = get_max_loc_var(src);
    FILE *out;
    out = fopen(filename, "wb");
    fprintf(out, "format ELF64 executable 3\n");
    fprintf(out, "segment readable executable\n");
    dump_def(out);
    for (size_t i = 0; i < procs.l; ++i)
    {
        Program proc = procs.progs[i];
        String_View proc_name = proc.toks[1].lexeme;
        fprintf(out, "proc_" SV_Fmt ":\n", SV_Arg(proc_name));
        fprintf(out, "    mov [ret_stack_rsp], rsp\n");
        fprintf(out, "    mov rsp, rax\n");

        write_asm(proc, out, procs_names, num_procs);

        fprintf(out, "    mov rax, rsp\n");
        fprintf(out, "    mov rsp, [ret_stack_rsp]\n");
        fprintf(out, "    add rsp, 0\n");
        fprintf(out, "    ret\n");
    }
    fprintf(out, "\nentry start\n");

    fprintf(out, "start:\n");
    fprintf(out, "    mov rax, ret_stack_end\n");
    fprintf(out, "    mov [ret_stack_rsp], rax\n");
    write_asm(main, out, procs_names, num_procs);
    exit_def(out);
    fprintf(out, "segment readable writable\n");
    fprintf(out, "ret_stack_rsp: rq 1\n");
    fprintf(out, "ret_stack: rb 65536\n");
    fprintf(out, "ret_stack_end:\n");

    fprintf(out, "proc_depth: db 0\n");
    fprintf(out, "vars: rb %d\n", max_loc_var * MAX_REC_DEPTH);
    fclose(out);
    return 0;
}

void usage(void)
{
    printf("./zf <input filename> <output filename>\n");
}

void sysprintf(const char *format, ...)
{
    char command[100];
    va_list args;

    va_start(args, format);
    vsnprintf(command, sizeof(command), format, args);
    va_end(args);

    system(command);
}

int compile_easy_fn(Program prog, char *filename)
{
    char *output = malloc(sizeof(filename) + 4 * sizeof(char));

    sprintf(output, "%s.asm", filename);
    return compile(prog, output);
}

int main(int argc, char **argv)
{
    if (argc > 3)
    {
        printf("Too much arguments !\n");
        usage();
        return -1;
    }
    else if (argc < 3)
    {
        printf("Not enough arguments !\n");
        usage();
        return -1;
    }
    Program prog = get_prog_from_file(argv[1]);
    if (compile_easy_fn(prog, argv[2]) != 0)
    {
        return -1;
    }
    sysprintf("fasm -m 524288 %s.asm  %s", argv[2], argv[2]);
    sysprintf("chmod +x %s", argv[2], argv[2]);

    // sysprintf("ld -o %s %s.o", argv[2], argv[2]);

    return 0;
}
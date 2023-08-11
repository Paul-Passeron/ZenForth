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
    assert(OP_COUNT == 21 && "Exhaustive handling of operations in type_of_lexeme.");
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

Program get_prog_from_file(char *filename)
{
    String_View lexemes[100];
    int l_i = 0;
    String_View file = read_file(filename);
    String_View temp;
    String_View delim;
    while (l_i < PROG_MAX && file.count > 0)
    {
        if (file.count == 0)
            break;
        temp = sv_chop_left_while(&file, is_not_separator);
        delim = sv_chop_left_while_separator(&file, is_separator);
        if (!is_sv_useless(temp))
            lexemes[l_i++] = temp;
        if (!is_sv_useless(delim))
            lexemes[l_i++] = delim;
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
void compile(Program prog, char *filename)
{
    assert(OP_COUNT == 21 && "Exhaustive handling of operations in compile.");
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

    Program vars;
    vars.length = 0;
    Type var_type = TYPE_BOOL;
    bool in_var = false;

    FILE *out;
    out = fopen(filename, "wb");
    fprintf(out, "section .text\n");
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
    fprintf(out, "\nglobal _start\n");
    fprintf(out, "_start:\n");

    for (int i = 0; i < prog.length; ++i)
    {
        Token tok = prog.toks[i];
        if (tok.op == OP_PUSH)
        {
            if (in_var && !is_sv_num(tok.lexeme))
            {
                in_var = false;
                tok.type = var_type;
                push(&vars, tok);
            }
            else if (!is_sv_num(tok.lexeme) && tok.type != TYPE_BOOL)
            {
                // We pushed a variable (more so its pointer onto the stack).
                fprintf(out, "    ;; PUSH\n");
                fprintf(out, "    mov rax, " SV_Fmt "\n", SV_Arg(tok.lexeme));
                fprintf(out, "    push rax\n");
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
            fprintf(out, "    ;; ADD\n");
            fprintf(out, "    pop rax\n");
            fprintf(out, "    pop rbx\n");
            fprintf(out, "    add rbx, rax\n");
            fprintf(out, "    push rbx\n");
        }
        else if (tok.op == OP_SUB)
        {
            fprintf(out, "    ;; SUB\n");
            fprintf(out, "    pop rax\n");
            fprintf(out, "    pop rbx\n");
            fprintf(out, "    sub rbx, rax\n");
            fprintf(out, "    push rbx\n");
        }
        else if (tok.op == OP_MUL)
        {
            fprintf(out, "    ;; MUL\n");
            fprintf(out, "    pop rdx\n");
            fprintf(out, "    pop rax\n");
            fprintf(out, "    mul rdx\n");
            fprintf(out, "    push rax\n");
        }
        else if (tok.op == OP_DIV)
        {
            fprintf(out, "    ;; DIV\n");
            fprintf(out, "    mov rdx, 0\n");
            fprintf(out, "    pop rcx\n");
            fprintf(out, "    pop rax\n");
            fprintf(out, "    div rcx\n");
            fprintf(out, "    push rax\n");
        }
        else if (tok.op == OP_MOD)
        {
            fprintf(out, "    ;; DIV\n");
            fprintf(out, "    mov rdx, 0\n");
            fprintf(out, "    pop rcx\n");
            fprintf(out, "    pop rax\n");
            fprintf(out, "    div rcx\n");
            fprintf(out, "    push rdx\n");
        }
        else if (tok.op == OP_PRINT)
        {
            fprintf(out, "    ;; PRINT\n");
            fprintf(out, "    pop rdi\n");
            fprintf(out, "    call dump\n");
        }
        else if (tok.op == OP_NOT)
        {
            fprintf(out, "    ;; NOT\n");
            fprintf(out, "    pop rax\n");
            fprintf(out, "    cmp rax, 0\n");
            fprintf(out, "    mov rcx, 0\n");
            fprintf(out, "    mov rdx, 1\n");
            fprintf(out, "    cmove rcx, rdx\n");
            fprintf(out, "    push rcx\n");
        }
        else if (tok.op == OP_DUP)
        {
            fprintf(out, "    ;; DUP\n");
            fprintf(out, "    pop rax\n");
            fprintf(out, "    push rax\n");
            fprintf(out, "    push rax\n");
        }
        else if (tok.op == OP_DROP)
        {
            fprintf(out, "    ;; DROP\n");
            fprintf(out, "    pop rax\n");
        }
        else if (tok.op == OP_SWAP)
        {
            fprintf(out, "    ;; SWAP\n");
            fprintf(out, "    pop rax\n");
            fprintf(out, "    pop rbx\n");
            fprintf(out, "    push rax\n");
            fprintf(out, "    push rbx\n");
        }
        else if (tok.op == OP_IF)
        {
            fprintf(out, "    ;; IF\n");
            fprintf(out, "    pop rax\n");
            fprintf(out, "    cmp rax, 0\n");
            fprintf(out, "    je .IF%d\n", if_counter);
            int_push(&if_stack, if_counter);
            int_push(&scope, iff);
            if_counter++;
        }
        else if (tok.op == OP_END)
        {
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
        else if (tok.op == OP_EQUAL)
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
        else if (tok.op == OP_L)
        {
            fprintf(out, "    ;; EQUAL\n");
            fprintf(out, "    pop rax\n");
            fprintf(out, "    pop rbx\n");
            fprintf(out, "    cmp rbx, rax\n");
            fprintf(out, "    mov rcx, 0\n");
            fprintf(out, "    mov rdx, 1\n");
            fprintf(out, "    cmovl rcx, rdx\n");
            fprintf(out, "    push rcx\n");
        }
        else if (tok.op == OP_G)
        {
            fprintf(out, "    ;; EQUAL\n");
            fprintf(out, "    pop rax\n");
            fprintf(out, "    pop rbx\n");
            fprintf(out, "    cmp rbx, rax\n");
            fprintf(out, "    mov rcx, 0\n");
            fprintf(out, "    mov rdx, 1\n");
            fprintf(out, "    cmovg rcx, rdx\n");
            fprintf(out, "    push rcx\n");
        }
        else if (tok.op == OP_WHILE)
        {
            fprintf(out, "    ;; WHILE\n");
            fprintf(out, ".DO%d:\n", wh_counter);
            int_push(&wh_stack, wh_counter);
            int_push(&scope, whi);
            wh_counter++;
        }
        else if (tok.op == OP_DO)
        {
            fprintf(out, "    ;; DO\n");
            fprintf(out, "    pop rax\n");
            fprintf(out, "    cmp rax, 0\n");
            fprintf(out, "    je .WHILE%d\n", int_peek(&wh_stack));
        }
        else if (tok.op == OP_INT)
        {
            var_type = TYPE_INT;
            in_var = true;
        }
        else if (tok.op == OP_STORE)
        {
            // assert(false && "! op is not implemented yet.");
            fprintf(out, "    ;; STORE\n");
            fprintf(out, "    pop rbx\n");
            fprintf(out, "    pop rax\n");
            fprintf(out, "    mov qword [rbx], rax\n");
        }
        else if (tok.op == OP_FETCH)
        {
            // assert(false && "@ op is not implemented yet.");
            fprintf(out, "    ;; FETCH\n");
            fprintf(out, "    pop rbx\n");
            fprintf(out, "    mov rax, [rbx]\n");
            fprintf(out, "    push rax\n");
        }
    }
    fprintf(out, "    ;; EXIT PROGRAM\n");
    fprintf(out, "    mov rax, 60\n");
    fprintf(out, "    mov rdi, 0\n");
    fprintf(out, "    syscall\n");

    fprintf(out, "section .data\n");
    for (int i = 0; i < vars.length; i++)
    {
        Token tok = vars.toks[i];
        if (tok.type == TYPE_INT)
        {
            fprintf(out, SV_Fmt ":\n", SV_Arg(tok.lexeme));
            fprintf(out, "    dw 0, 0, 0, 0");
        }
        else if (tok.type == TYPE_BOOL)
        {
            fprintf(out, SV_Fmt ":\n", SV_Arg(tok.lexeme));
            fprintf(out, "    db 0");
        }
    }
    fclose(out);
}

void usage(void)
{
    printf("./zf <input filename> <output filename>\n");
    // printf("./zf <input filename>\n");
}

void sysprintf(const char *format, ...)
{
    char command[1024]; // Adjust the size as needed
    va_list args;

    va_start(args, format);
    vsnprintf(command, sizeof(command), format, args);
    va_end(args);

    system(command);
}

void compile_easy_fn(Program prog, char *filename)
{
    char *output = malloc(sizeof(filename) + 4 * sizeof(char));

    sprintf(output, "%s.asm", filename);
    compile(prog, output);
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
    char *out = argv[2];

    Program prog = get_prog_from_file(argv[1]);
    compile_easy_fn(prog, out);
    sysprintf("nasm -felf64 %s.asm -o %s.o", out, out);
    sysprintf("ld -o %s %s.o", out, out);

    return 0;
}
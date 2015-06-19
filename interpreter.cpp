#include <iostream>

typedef struct {
    int array[100];
    const int lenght = 100;
} dynamic_memory_t;

typedef struct environment_s environment_t;

typedef int8_t opcode_t;

struct environment_s {
    int program_counter;
    opcode_t* data;
    int lenght;
    dynamic_memory_t memory;
    int memory_adress;
};

enum opcodes {
    OP_PTR_INCREASE_ATAT = 0, // >
    OP_PTR_DECREASE_ATGC = 1, // <
    OP_VAL_INCREASE_ATTA = 2, // +
    OP_VAL_DECREASE_ATCG = 3, // -
    OP_PRINT_CHAR_GCAT = 4,   // .
    OP_SCAN_CHAR_GCGC = 5,    // ,
    OP_BEGIN_LOOP_GCTA = 6,   // [
    OP_END_LOOP_GCCG = 7,     // ]
    OP_ASSIGN_TAAT = 8,       // :=
    OP_ADD_TAGC = 9,          // +=
    OP_SUB_TATA = 10,         // -=
    OP_MUL_TACG = 11,         // *=
    OP_DIV_CGAT = 12,         // /=
    OP_PRINT_INT_CGGC = 13,   // ~
    OP_SCAN_INT_CGTA = 14,    // ?
    OP_NOP_CGCG = 15          // X
};

#define BYTECODE_DEBUG false

#define OP_DATA              (0x0F)
#define OP_HEADER_SHIFT      (4)
#define OP_HEADER_ADD        (0x40)
#define OP_HEADER_ADD_MASK   (0xC0)
#define OP_HEADER_ADD_DATA   (0x30)
#define OP_HEADER_LOCAL      (0x80)
#define OP_HEADER_LOCAL_DATA (0x70)

// value in [0, ..., 3]
#define OP_HEADER_ADD_CREATE(value) (OP_HEADER_ADD | ((value) << OP_HEADER_SHIFT))

// value in [1, ..., 8]
#define OP_HEADER_LOCAL_CREATE(value) (OP_HEADER_LOCAL | (((value) - 1) << OP_HEADER_SHIFT))

#define OP_HEADER_ADD_CHECK(value) (((value) & OP_HEADER_ADD_MASK) == OP_HEADER_ADD)
#define OP_HEADER_LOCAL_CHECK(value) (((value) & OP_HEADER_LOCAL) == OP_HEADER_LOCAL)
#define OP_HEADER_ADD_GET(value) ((OP_HEADER_ADD_DATA & (value)) >> OP_HEADER_SHIFT)
#define OP_HEADER_LOCAL_GET(value) (((OP_HEADER_LOCAL_DATA & (value)) >> OP_HEADER_SHIFT) + 1)

int* get_memory(environment_t* env, int64_t offset) {
    int adress = (env->memory_adress + offset) % env->memory.lenght;
    if(adress < 0) adress = env->memory.lenght + adress;
    return &env->memory.array[adress];
}

int* get_memory(environment_t* env) {
    int adress = env->memory_adress % env->memory.lenght;
    if(adress < 0) adress = env->memory.lenght + adress;
    return &env->memory.array[adress];
}

typedef struct {
    opcode_t* data;
    int lenght;
    int pointer;
} opcode_dynamic_array_t;

void check_space(opcode_dynamic_array_t** code, int additional) {
    if (*code == 0) {
        *code = (opcode_dynamic_array_t*)malloc(sizeof(opcode_dynamic_array_t));
        (*code)->data = (opcode_t*)malloc(((*code)->lenght = 16) * sizeof(opcode_t));
        (*code)->pointer = 0;
    }
    if((*code)->pointer + additional > (*code)->lenght) {
        (*code)->data = (opcode_t*)realloc((*code)->data, ((*code)->lenght *= 2) * sizeof(opcode_t));
    }
}

int add_opcode(opcode_dynamic_array_t** code, opcodes operation) {
    check_space(code, 1);
    (*code)->data[(*code)->pointer++] = operation;
    return (*code)->pointer;
}

int add_opcode(opcode_dynamic_array_t** code, opcodes operation, int64_t value) {
    check_space(code, 9);
    if(1 <= value && value <= 8) {
        (*code)->data[(*code)->pointer++] = operation | OP_HEADER_LOCAL_CREATE(value);
    } else if (INT8_MIN <= value && value <= INT8_MAX) {
        (*code)->data[(*code)->pointer++] = operation | OP_HEADER_ADD_CREATE(0);
        (*code)->data[(*code)->pointer++] = value;
    } else if (INT16_MIN <= value && value <= INT16_MAX) {
        (*code)->data[(*code)->pointer++] = operation | OP_HEADER_ADD_CREATE(1);
        opcode_t* data = &(*code)->data[(*code)->pointer];
        int16_t* data16 = (int16_t*)data;
        *data16 = value;
        (*code)->pointer += 2;
    } else if (INT32_MIN <= value && value <= INT32_MAX) {
        (*code)->data[(*code)->pointer++] = operation | OP_HEADER_ADD_CREATE(2);
        opcode_t* data = &(*code)->data[(*code)->pointer];
        int32_t* data32 = (int32_t*)data;
        *data32 = (int32_t)value;
        (*code)->pointer += 4;
    }  else if (INT64_MIN <= value && value <= INT64_MAX) {
        (*code)->data[(*code)->pointer++] = operation | OP_HEADER_ADD_CREATE(3);
        opcode_t* data = &(*code)->data[(*code)->pointer];
        int64_t* data64 = (int64_t*)data;
        *data64 = value;
        (*code)->pointer += 8;
    }
    return (*code)->pointer;
}

void put_opcode_value(opcode_dynamic_array_t** code, int pos, int64_t value) {
    opcode_t* data = &(*code)->data[pos];
    int64_t* data64 = (int64_t*)data;
    *data64 = value;
}

typedef struct int_stack_s {
    int data;
    struct int_stack_s* next;
} int_stack_t;

void push(int_stack_t** stack, int from) {
    int_stack_t* element = (int_stack_t*)malloc(sizeof(int_stack_t));
    element->data = from;
    element->next = *stack;
    *stack = element;
}

int pop(int_stack_t** stack) {
    int_stack_t* element = *stack;
    *stack = (*stack)->next;
    int from = element->data;
    free(element);
    return from;
}

void step(environment_t* env) {
    opcode_t opcode = env->data[env->program_counter++];
    
    int64_t param = 0;
    if (OP_HEADER_ADD_CHECK(opcode)) {
        int size = OP_HEADER_ADD_GET(opcode);
        switch (size) {
            case 0:
            param = *((int8_t*)&env->data[env->program_counter]);
            break;
            case 1:
            param = *((int16_t*)&env->data[env->program_counter]);
            break;
            case 2:
            param = *((int32_t*)&env->data[env->program_counter]);
            break;
            case 3:
            param = *((int64_t*)&env->data[env->program_counter]);
            break;
        }
        env->program_counter += 1 << size;
        
    } else if(OP_HEADER_LOCAL_CHECK(opcode)) {
        param = OP_HEADER_LOCAL_GET(opcode);
    }
    
    opcode &= OP_DATA;
    
    if (BYTECODE_DEBUG) printf("[%d] opcode=%x (value=%lld) (array=%d)\n", env->program_counter, opcode, param, env->memory_adress);
    
    switch (opcode) {
        case OP_PTR_INCREASE_ATAT: // >
        env->memory_adress += param;
        break;
        case OP_PTR_DECREASE_ATGC: // <
        env->memory_adress -= param;
        break;
        case OP_VAL_INCREASE_ATTA: // +
        *get_memory(env) += param;
        break;
        case OP_VAL_DECREASE_ATCG: // -
        *get_memory(env) -= param;
        break;
        case OP_PRINT_CHAR_GCAT:   // .
        printf("%c", *get_memory(env));
        break;
        case OP_SCAN_CHAR_GCGC:    // ,
        {
            char c = 0;
            scanf("%c", &c);
            *get_memory(env) = c;
        }
        break;
        case OP_BEGIN_LOOP_GCTA:   // [
        if(*get_memory(env) == 0) env->program_counter += param;
        break;
        case OP_END_LOOP_GCCG:     // ]
        if(*get_memory(env) != 0) env->program_counter += param;
        break;
        // extended
        case OP_ASSIGN_TAAT:       // :=
        *get_memory(env) = *get_memory(env, param);
        break;
        case OP_ADD_TAGC:          // +=
        *get_memory(env) += *get_memory(env, param);
        break;
        case OP_SUB_TATA:         // -=
        *get_memory(env) -= *get_memory(env, param);
        break;
        case OP_MUL_TACG:         // *=
        *get_memory(env) *= *get_memory(env, param);
        break;
        case OP_DIV_CGAT:         // /=
        *get_memory(env) /= *get_memory(env, param);
        break;
        case OP_PRINT_INT_CGGC:   // ~
        printf("%d", *get_memory(env));
        break;
        case OP_SCAN_INT_CGTA:    // ?
        {
            int i = 0;
            scanf("%d", &i);
            *get_memory(env) = i;
        }
        break;
        case OP_NOP_CGCG:          // X
        break;
    }
    if (BYTECODE_DEBUG) {
        for(int i = 0; i < 10 && i < env->memory.lenght; i++) {
            printf("%d: %d\n", i, env->memory.array[i]);
        }
    }
}

int main(int argc, const char * argv[]) {
    const char* hello_world = "+++++*=X>>+>>:=<<<<+++++++<<<<[>>>:=<+=<<X<<:=>X>:=>X>~>.<<<<-]";
    
    int pointer = 0;
    size_t lenght = strlen(hello_world);
    opcode_dynamic_array_t* code = 0;
    int_stack_t* stack = 0;
    
    while (pointer < lenght) {
        int value = 1;
        bool special = false;
        opcodes ocode = OP_NOP_CGCG;
        if(pointer+1 < lenght && hello_world[pointer+1] == '=') {
            switch (hello_world[pointer]) {
                case ':':
                special = true;
                ocode = OP_ASSIGN_TAAT;
                break;
                case '+':
                special = true;
                ocode = OP_ADD_TAGC;
                break;
                case '-':
                special = true;
                ocode = OP_SUB_TATA;
                break;
                case '*':
                special = true;
                ocode = OP_MUL_TACG;
                break;
                case '/':
                special = true;
                ocode = OP_DIV_CGAT;
                break;
            }
        }
        if (hello_world[pointer] == '=') {
            special = true;
            ocode = OP_ASSIGN_TAAT;
            pointer--;
        }
        if (special) {
            pointer++;
            int64_t value = 0;
            while (pointer+1 < lenght && (hello_world[++pointer] == '<' || hello_world[pointer] == '>')) {
                value += hello_world[pointer] == '>' ? 1 : -1;
            }
            add_opcode(&code, ocode, value);
            continue;
        }
        switch (hello_world[pointer]) {
            default:
            pointer++;
            break;
            case '+':
            while (pointer+1 < lenght && (hello_world[++pointer] == '+' && hello_world[pointer+1] != '=')) {
                value++;
            }
            add_opcode(&code, OP_VAL_INCREASE_ATTA, value);
            break;
            case '-':
            while (pointer+1 < lenght && (hello_world[++pointer] == '-' && hello_world[pointer+1] != '=')) {
                value++;
            }
            add_opcode(&code, OP_VAL_DECREASE_ATCG, value);
            break;
            case '>':
            while (pointer+1 < lenght && hello_world[++pointer] == '>') {
                value++;
            }
            add_opcode(&code, OP_PTR_INCREASE_ATAT, value);
            break;
            case '<':
            while (pointer+1 < lenght && hello_world[++pointer] == '<') {
                value++;
            }
            add_opcode(&code, OP_PTR_DECREASE_ATGC, value);
            break;
            case '.':
            pointer++;
            add_opcode(&code, OP_PRINT_CHAR_GCAT);
            break;
            case '~':
            pointer++;
            add_opcode(&code, OP_PRINT_INT_CGGC);
            break;
            case ',':
            pointer++;
            add_opcode(&code, OP_SCAN_CHAR_GCGC);
            break;
            case '?':
            pointer++;
            add_opcode(&code, OP_SCAN_INT_CGTA);
            break;
            case '[':
            pointer++;
            {
                int from = add_opcode(&code, OP_BEGIN_LOOP_GCTA, INT64_MIN);
                push(&stack, from);
            }
            break;
            case ']':
            pointer++;
            int from = pop(&stack);
            int to = add_opcode(&code, OP_END_LOOP_GCCG, INT64_MIN);
            put_opcode_value(&code, to - 8, from - to);
            put_opcode_value(&code, from - 8, to - from);
            break;
        }
    }
    
    environment_t env;
    env.data = (*code).data;
    env.lenght = (*code).pointer;
    for(int i = 0; i < env.memory.lenght; i++) env.memory.array[i] = 0;
    env.memory_adress = 0;
    env.program_counter = 0;
    
    while (env.program_counter < env.lenght) {
        step(&env);
    }
    
    return 0;
}

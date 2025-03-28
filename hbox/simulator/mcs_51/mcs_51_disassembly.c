/***************************************************************
 * Name:      mcs_51_disassembly.c
 * Purpose:   实现mcs_51_disassembly接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-02-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "mcs_51_disassembly.h"


const hs_mcs_51_disassembly_instruction_t hs_mcs_51_disassembly_instruction_table[]=
{
    { 0x00, 0xff, ' ', 1, "NOP",false},
    { 0x01, 0xff, 'A', 2, "AJMP %A",false},
    { 0x02, 0xff, 'L', 3, "LJMP %l",false},
    { 0x03, 0xff, ' ', 1, "RR A",false},
    { 0x04, 0xff, ' ', 1, "INC A",false},
    { 0x05, 0xff, ' ', 2, "INC %a",false},
    { 0x06, 0xff, ' ', 1, "INC @R0",false},
    { 0x07, 0xff, ' ', 1, "INC @R1",false},
    { 0x08, 0xff, ' ', 1, "INC R0",false},
    { 0x09, 0xff, ' ', 1, "INC R1",false},
    { 0x0a, 0xff, ' ', 1, "INC R2",false},
    { 0x0b, 0xff, ' ', 1, "INC R3",false},
    { 0x0c, 0xff, ' ', 1, "INC R4",false},
    { 0x0d, 0xff, ' ', 1, "INC R5",false},
    { 0x0e, 0xff, ' ', 1, "INC R6",false},
    { 0x0f, 0xff, ' ', 1, "INC R7",false},
    { 0x10, 0xff, 'R', 3, "JBC %b,%R",false},
    { 0x11, 0xff, 'a', 2, "ACALL %A",false},
    { 0x12, 0xff, 'l', 3, "LCALL %l", true},
    { 0x13, 0xff, ' ', 1, "RRC A",false},
    { 0x14, 0xff, ' ', 1, "DEC A",false},
    { 0x15, 0xff, ' ', 2, "DEC %a",false},
    { 0x16, 0xff, ' ', 1, "DEC @R0",false},
    { 0x17, 0xff, ' ', 1, "DEC @R1",false},
    { 0x18, 0xff, ' ', 1, "DEC R0",false},
    { 0x19, 0xff, ' ', 1, "DEC R1",false},
    { 0x1a, 0xff, ' ', 1, "DEC R2",false},
    { 0x1b, 0xff, ' ', 1, "DEC R3",false},
    { 0x1c, 0xff, ' ', 1, "DEC R4",false},
    { 0x1d, 0xff, ' ', 1, "DEC R5",false},
    { 0x1e, 0xff, ' ', 1, "DEC R6",false},
    { 0x1f, 0xff, ' ', 1, "DEC R7",false},
    { 0x20, 0xff, 'R', 3, "JB %b,%R",false},
    { 0x21, 0xff, 'A', 2, "AJMP %A",false},
    { 0x22, 0xff, '_', 1, "RET",false},
    { 0x23, 0xff, ' ', 1, "RL A",false},
    { 0x24, 0xff, ' ', 2, "ADD A,#%d",false},
    { 0x25, 0xff, ' ', 2, "ADD A,%a",false},
    { 0x26, 0xff, ' ', 1, "ADD A,@R0",false},
    { 0x27, 0xff, ' ', 1, "ADD A,@R1",false},
    { 0x28, 0xff, ' ', 1, "ADD A,R0",false},
    { 0x29, 0xff, ' ', 1, "ADD A,R1",false},
    { 0x2a, 0xff, ' ', 1, "ADD A,R2",false},
    { 0x2b, 0xff, ' ', 1, "ADD A,R3",false},
    { 0x2c, 0xff, ' ', 1, "ADD A,R4",false},
    { 0x2d, 0xff, ' ', 1, "ADD A,R5",false},
    { 0x2e, 0xff, ' ', 1, "ADD A,R6",false},
    { 0x2f, 0xff, ' ', 1, "ADD A,R7",false},
    { 0x30, 0xff, 'R', 3, "JNB %b,%R",false},
    { 0x31, 0xff, 'a', 2, "ACALL %A", true},
    { 0x32, 0xff, '_', 1, "RETI",false},
    { 0x33, 0xff, ' ', 1, "RLC A",false},
    { 0x34, 0xff, ' ', 2, "ADDC A,#%d",false},
    { 0x35, 0xff, ' ', 2, "ADDC A,%a",false},
    { 0x36, 0xff, ' ', 1, "ADDC A,@R0",false},
    { 0x37, 0xff, ' ', 1, "ADDC A,@R1",false},
    { 0x38, 0xff, ' ', 1, "ADDC A,R0",false},
    { 0x39, 0xff, ' ', 1, "ADDC A,R1",false},
    { 0x3a, 0xff, ' ', 1, "ADDC A,R2",false},
    { 0x3b, 0xff, ' ', 1, "ADDC A,R3",false},
    { 0x3c, 0xff, ' ', 1, "ADDC A,R4",false},
    { 0x3d, 0xff, ' ', 1, "ADDC A,R5",false},
    { 0x3e, 0xff, ' ', 1, "ADDC A,R6",false},
    { 0x3f, 0xff, ' ', 1, "ADDC A,R7",false},
    { 0x40, 0xff, 'r', 2, "JC %r",false},
    { 0x41, 0xff, 'A', 2, "AJMP %A",false},
    { 0x42, 0xff, ' ', 2, "ORL %a,A",false},
    { 0x43, 0xff, ' ', 3, "ORL %a,#%D",false},
    { 0x44, 0xff, ' ', 2, "ORL A,#%d",false},
    { 0x45, 0xff, ' ', 2, "ORL A,%a",false},
    { 0x46, 0xff, ' ', 1, "ORL A,@R0",false},
    { 0x47, 0xff, ' ', 1, "ORL A,@R1",false},
    { 0x48, 0xff, ' ', 1, "ORL A,R0",false},
    { 0x49, 0xff, ' ', 1, "ORL A,R1",false},
    { 0x4a, 0xff, ' ', 1, "ORL A,R2",false},
    { 0x4b, 0xff, ' ', 1, "ORL A,R3",false},
    { 0x4c, 0xff, ' ', 1, "ORL A,R4",false},
    { 0x4d, 0xff, ' ', 1, "ORL A,R5",false},
    { 0x4e, 0xff, ' ', 1, "ORL A,R6",false},
    { 0x4f, 0xff, ' ', 1, "ORL A,R7",false},
    { 0x50, 0xff, 'r', 2, "JNC %r",false},
    { 0x51, 0xff, 'a', 2, "ACALL %A", true},
    { 0x52, 0xff, ' ', 2, "ANL %a,A",false},
    { 0x53, 0xff, ' ', 3, "ANL %a,#%D",false},
    { 0x54, 0xff, ' ', 2, "ANL A,#%d",false},
    { 0x55, 0xff, ' ', 2, "ANL A,%a",false},
    { 0x56, 0xff, ' ', 1, "ANL A,@R0",false},
    { 0x57, 0xff, ' ', 1, "ANL A,@R1",false},
    { 0x58, 0xff, ' ', 1, "ANL A,R0",false},
    { 0x59, 0xff, ' ', 1, "ANL A,R1",false},
    { 0x5a, 0xff, ' ', 1, "ANL A,R2",false},
    { 0x5b, 0xff, ' ', 1, "ANL A,R3",false},
    { 0x5c, 0xff, ' ', 1, "ANL A,R4",false},
    { 0x5d, 0xff, ' ', 1, "ANL A,R5",false},
    { 0x5e, 0xff, ' ', 1, "ANL A,R6",false},
    { 0x5f, 0xff, ' ', 1, "ANL A,R7",false},
    { 0x60, 0xff, 'r', 2, "JZ %r",false},
    { 0x61, 0xff, 'A', 2, "AJMP %A",false},
    { 0x62, 0xff, ' ', 2, "XRL %a,A",false},
    { 0x63, 0xff, ' ', 3, "XRL %a,#%D",false},
    { 0x64, 0xff, ' ', 2, "XRL A,#%d",false},
    { 0x65, 0xff, ' ', 2, "XRL A,%a",false},
    { 0x66, 0xff, ' ', 1, "XRL A,@R0",false},
    { 0x67, 0xff, ' ', 1, "XRL A,@R1",false},
    { 0x68, 0xff, ' ', 1, "XRL A,R0",false},
    { 0x69, 0xff, ' ', 1, "XRL A,R1",false},
    { 0x6a, 0xff, ' ', 1, "XRL A,R2",false},
    { 0x6b, 0xff, ' ', 1, "XRL A,R3",false},
    { 0x6c, 0xff, ' ', 1, "XRL A,R4",false},
    { 0x6d, 0xff, ' ', 1, "XRL A,R5",false},
    { 0x6e, 0xff, ' ', 1, "XRL A,R6",false},
    { 0x6f, 0xff, ' ', 1, "XRL A,R7",false},
    { 0x70, 0xff, 'r', 2, "JNZ %r",false},
    { 0x71, 0xff, 'a', 2, "ACALL %A", true},
    { 0x72, 0xff, ' ', 2, "ORL C,%b",false},
    { 0x73, 0xff, '_', 1, "JMP @A+DPTR",false},
    { 0x74, 0xff, ' ', 2, "MOV A,#%d",false},
    { 0x75, 0xff, ' ', 3, "MOV %a,#%D",false},
    { 0x76, 0xff, ' ', 2, "MOV @R0,#%d",false},
    { 0x77, 0xff, ' ', 2, "MOV @R1,#%d",false},
    { 0x78, 0xff, ' ', 2, "MOV R0,#%d",false},
    { 0x79, 0xff, ' ', 2, "MOV R1,#%d",false},
    { 0x7a, 0xff, ' ', 2, "MOV R2,#%d",false},
    { 0x7b, 0xff, ' ', 2, "MOV R3,#%d",false},
    { 0x7c, 0xff, ' ', 2, "MOV R4,#%d",false},
    { 0x7d, 0xff, ' ', 2, "MOV R5,#%d",false},
    { 0x7e, 0xff, ' ', 2, "MOV R6,#%d",false},
    { 0x7f, 0xff, ' ', 2, "MOV R7,#%d",false},
    { 0x80, 0xff, 's', 2, "SJMP %r",false},
    { 0x81, 0xff, 'A', 2, "AJMP %A",false},
    { 0x82, 0xff, ' ', 2, "ANL C,%b",false},
    { 0x83, 0xff, ' ', 1, "MOVC A,@A+PC",false},
    { 0x84, 0xff, ' ', 1, "DIV AB",false},
    { 0x85, 0xff, ' ', 3, "MOV %8,%a",false},
    { 0x86, 0xff, ' ', 2, "MOV %a,@R0",false},
    { 0x87, 0xff, ' ', 2, "MOV %a,@R1",false},
    { 0x88, 0xff, ' ', 2, "MOV %a,R0",false},
    { 0x89, 0xff, ' ', 2, "MOV %a,R1",false},
    { 0x8a, 0xff, ' ', 2, "MOV %a,R2",false},
    { 0x8b, 0xff, ' ', 2, "MOV %a,R3",false},
    { 0x8c, 0xff, ' ', 2, "MOV %a,R4",false},
    { 0x8d, 0xff, ' ', 2, "MOV %a,R5",false},
    { 0x8e, 0xff, ' ', 2, "MOV %a,R6",false},
    { 0x8f, 0xff, ' ', 2, "MOV %a,R7",false},
    { 0x90, 0xff, ' ', 3, "MOV DPTR,#%6",false},
    { 0x91, 0xff, 'a', 2, "ACALL %A", true},
    { 0x92, 0xff, ' ', 2, "MOV %b,C",false},
    { 0x93, 0xff, ' ', 1, "MOVC A,@A+DPTR",false},
    { 0x94, 0xff, ' ', 2, "SUBB A,#%d",false},
    { 0x95, 0xff, ' ', 2, "SUBB A,%a",false},
    { 0x96, 0xff, ' ', 1, "SUBB A,@R0",false},
    { 0x97, 0xff, ' ', 1, "SUBB A,@R1",false},
    { 0x98, 0xff, ' ', 1, "SUBB A,R0",false},
    { 0x99, 0xff, ' ', 1, "SUBB A,R1",false},
    { 0x9a, 0xff, ' ', 1, "SUBB A,R2",false},
    { 0x9b, 0xff, ' ', 1, "SUBB A,R3",false},
    { 0x9c, 0xff, ' ', 1, "SUBB A,R4",false},
    { 0x9d, 0xff, ' ', 1, "SUBB A,R5",false},
    { 0x9e, 0xff, ' ', 1, "SUBB A,R6",false},
    { 0x9f, 0xff, ' ', 1, "SUBB A,R7",false},
    { 0xa0, 0xff, ' ', 2, "ORL C,/%b",false},
    { 0xa1, 0xff, 'A', 2, "AJMP %A",false},
    { 0xa2, 0xff, ' ', 2, "MOV C,%b",false},
    { 0xa3, 0xff, ' ', 1, "INC DPTR",false},
    { 0xa4, 0xff, ' ', 1, "MUL AB",false},
    { 0xa5, 0xff, '_', 1, "-",false},
    { 0xa6, 0xff, ' ', 2, "MOV @R0,%a",false},
    { 0xa7, 0xff, ' ', 2, "MOV @R1,%a",false},
    { 0xa8, 0xff, ' ', 2, "MOV R0,%a",false},
    { 0xa9, 0xff, ' ', 2, "MOV R1,%a",false},
    { 0xaa, 0xff, ' ', 2, "MOV R2,%a",false},
    { 0xab, 0xff, ' ', 2, "MOV R3,%a",false},
    { 0xac, 0xff, ' ', 2, "MOV R4,%a",false},
    { 0xad, 0xff, ' ', 2, "MOV R5,%a",false},
    { 0xae, 0xff, ' ', 2, "MOV R6,%a",false},
    { 0xaf, 0xff, ' ', 2, "MOV R7,%a",false},
    { 0xb0, 0xff, ' ', 2, "ANL C,/%b",false},
    { 0xb1, 0xff, 'a', 2, "ACALL %A", true},
    { 0xb2, 0xff, ' ', 2, "CPL %b",false},
    { 0xb3, 0xff, ' ', 1, "CPL C",false},
    { 0xb4, 0xff, 'R', 3, "CJNE A,#%d,%R",false},
    { 0xb5, 0xff, 'R', 3, "CJNE A,%a,%R",false},
    { 0xb6, 0xff, 'R', 3, "CJNE @R0,#%d,%R",false},
    { 0xb7, 0xff, 'R', 3, "CJNE @R1,#%d,%R",false},
    { 0xb8, 0xff, 'R', 3, "CJNE R0,#%d,%R",false},
    { 0xb9, 0xff, 'R', 3, "CJNE R1,#%d,%R",false},
    { 0xba, 0xff, 'R', 3, "CJNE R2,#%d,%R",false},
    { 0xbb, 0xff, 'R', 3, "CJNE R3,#%d,%R",false},
    { 0xbc, 0xff, 'R', 3, "CJNE R4,#%d,%R",false},
    { 0xbd, 0xff, 'R', 3, "CJNE R5,#%d,%R",false},
    { 0xbe, 0xff, 'R', 3, "CJNE R6,#%d,%R",false},
    { 0xbf, 0xff, 'R', 3, "CJNE R7,#%d,%R",false},
    { 0xc0, 0xff, ' ', 2, "PUSH %a",false},
    { 0xc1, 0xff, 'A', 2, "AJMP %A",false},
    { 0xc2, 0xff, ' ', 2, "CLR %b",false},
    { 0xc3, 0xff, ' ', 1, "CLR C",false},
    { 0xc4, 0xff, ' ', 1, "SWAP A",false},
    { 0xc5, 0xff, ' ', 2, "XCH A,%a",false},
    { 0xc6, 0xff, ' ', 1, "XCH A,@R0",false},
    { 0xc7, 0xff, ' ', 1, "XCH A,@R1",false},
    { 0xc8, 0xff, ' ', 1, "XCH A,R0",false},
    { 0xc9, 0xff, ' ', 1, "XCH A,R1",false},
    { 0xca, 0xff, ' ', 1, "XCH A,R2",false},
    { 0xcb, 0xff, ' ', 1, "XCH A,R3",false},
    { 0xcc, 0xff, ' ', 1, "XCH A,R4",false},
    { 0xcd, 0xff, ' ', 1, "XCH A,R5",false},
    { 0xce, 0xff, ' ', 1, "XCH A,R6",false},
    { 0xcf, 0xff, ' ', 1, "XCH A,R7",false},
    { 0xd0, 0xff, ' ', 2, "POP %a",false},
    { 0xd1, 0xff, 'a', 2, "ACALL %A", true},
    { 0xd2, 0xff, ' ', 2, "SETB %b",false},
    { 0xd3, 0xff, ' ', 1, "SETB C",false},
    { 0xd4, 0xff, ' ', 1, "DA A",false},
    { 0xd5, 0xff, 'R', 3, "DJNZ %a,%R",false},
    { 0xd6, 0xff, ' ', 1, "XCHD A,@R0",false},
    { 0xd7, 0xff, ' ', 1, "XCHD A,@R1",false},
    { 0xd8, 0xff, 'r', 2, "DJNZ R0,%r",false},
    { 0xd9, 0xff, 'r', 2, "DJNZ R1,%r",false},
    { 0xda, 0xff, 'r', 2, "DJNZ R2,%r",false},
    { 0xdb, 0xff, 'r', 2, "DJNZ R3,%r",false},
    { 0xdc, 0xff, 'r', 2, "DJNZ R4,%r",false},
    { 0xdd, 0xff, 'r', 2, "DJNZ R5,%r",false},
    { 0xde, 0xff, 'r', 2, "DJNZ R6,%r",false},
    { 0xdf, 0xff, 'r', 2, "DJNZ R7,%r",false},
    { 0xe0, 0xff, ' ', 1, "MOVX A,@DPTR",false},
    { 0xe1, 0xff, 'A', 2, "AJMP %A",false},
    { 0xe2, 0xff, ' ', 1, "MOVX A,@R0",false},
    { 0xe3, 0xff, ' ', 1, "MOVX A,@R1",false},
    { 0xe4, 0xff, ' ', 1, "CLR A",false},
    { 0xe5, 0xff, ' ', 2, "MOV A,%a",false},
    { 0xe6, 0xff, ' ', 1, "MOV A,@R0",false},
    { 0xe7, 0xff, ' ', 1, "MOV A,@R1",false},
    { 0xe8, 0xff, ' ', 1, "MOV A,R0",false},
    { 0xe9, 0xff, ' ', 1, "MOV A,R1",false},
    { 0xea, 0xff, ' ', 1, "MOV A,R2",false},
    { 0xeb, 0xff, ' ', 1, "MOV A,R3",false},
    { 0xec, 0xff, ' ', 1, "MOV A,R4",false},
    { 0xed, 0xff, ' ', 1, "MOV A,R5",false},
    { 0xee, 0xff, ' ', 1, "MOV A,R6",false},
    { 0xef, 0xff, ' ', 1, "MOV A,R7",false},
    { 0xf0, 0xff, ' ', 1, "MOVX @DPTR,A",false},
    { 0xf1, 0xff, 'a', 2, "ACALL %A", true},
    { 0xf2, 0xff, ' ', 1, "MOVX @R0,A",false},
    { 0xf3, 0xff, ' ', 1, "MOVX @R1,A",false},
    { 0xf4, 0xff, ' ', 1, "CPL A",false},
    { 0xf5, 0xff, ' ', 2, "MOV %a,A",false},
    { 0xf6, 0xff, ' ', 1, "MOV @R0,A",false},
    { 0xf7, 0xff, ' ', 1, "MOV @R1,A",false},
    { 0xf8, 0xff, ' ', 1, "MOV R0,A",false},
    { 0xf9, 0xff, ' ', 1, "MOV R1,A",false},
    { 0xfa, 0xff, ' ', 1, "MOV R2,A",false},
    { 0xfb, 0xff, ' ', 1, "MOV R3,A",false},
    { 0xfc, 0xff, ' ', 1, "MOV R4,A",false},
    { 0xfd, 0xff, ' ', 1, "MOV R5,A",false},
    { 0xfe, 0xff, ' ', 1, "MOV R6,A",false},
    { 0xff, 0xff, ' ', 1, "MOV R7,A",false},
    { 0, 0, 0, 0, NULL,false }
};

const hs_mcs_51_disassembly_instruction_t *hs_mcs_51_disassembly_instruction_table_get(const uint8_t *instruction)
{
    if(instruction!=NULL)
    {
        return &hs_mcs_51_disassembly_instruction_table[(*instruction)];
    }
    return NULL;
}

size_t  hs_mcs_51_disassembly_instruction_length(const uint8_t *instruction)
{
    if(instruction!=NULL)
    {
        return hs_mcs_51_disassembly_instruction_table[(*instruction)].length;
    }
    return 0;
}

size_t  hs_mcs_51_disassembly_code_instruction_count(const uint8_t *code,size_t length)
{
    size_t ret=0;
    if(code!=NULL && length > 0)
    {
        size_t pc=0;
        while(pc<length)
        {
            ret++;
            pc+=hs_mcs_51_disassembly_instruction_length(&code[pc]);
        }
    }
    return ret;
}

size_t  hs_mcs_51_disassembly_code_instruction_type_count(const uint8_t *code,size_t length)
{
    size_t ret=0;
    if(code!=NULL && length > 0)
    {
        size_t pc=0;
        uint8_t mask[32]= {0};
        while(pc<length)
        {
            {
                mask[code[pc]/8]|=(1ULL<<(code[pc]%8));
            }
            pc+=hs_mcs_51_disassembly_instruction_length(&code[pc]);
        }
        for(size_t i=0; i<256; i++)
        {
            if((mask[i/8] & (1ULL<<(i%8)))!=0)
            {
                ret++;
            }
        }
    }
    return ret;
}

static void hs_mcs_51_disassembly_print_uint16(uint16_t data,void (*char_output)(const char ch,void *usr),void *usr)
{
    if(char_output==NULL)
    {
        return;
    }
    for(size_t i=0; i<sizeof(data)*2; i++)
    {
        int8_t val=((data&(0xF << ((sizeof(data)*2-i-1)*4)))>>((sizeof(data)*2-i-1)*4));
        if(val <= 9)
        {
            char_output('0'+val,usr);
        }
        else
        {
            char_output('A'+val-10,usr);
        }
    }
    char_output('H',usr);
}

static void hs_mcs_51_disassembly_print_uint8(uint8_t data,void (*char_output)(const char ch,void *usr),void *usr)
{
    if(char_output==NULL)
    {
        return;
    }
    for(size_t i=0; i<sizeof(data)*2; i++)
    {
        int8_t val=((data&(0xF << ((sizeof(data)*2-i-1)*4)))>>((sizeof(data)*2-i-1)*4));
        if(val <= 9)
        {
            char_output('0'+val,usr);
        }
        else
        {
            char_output('A'+val-10,usr);
        }
    }
    char_output('H',usr);
}

static void hs_mcs_51_disassembly_print_int8(int8_t data,void (*char_output)(const char ch,void *usr),void *usr)
{
    if(char_output==NULL)
    {
        return;
    }
    if(data<0)
    {
        char_output('-',usr);
        data=-data;
    }
    for(size_t i=0; i<sizeof(data)*2; i++)
    {
        int8_t val=((data&(0xF << ((sizeof(data)*2-i-1)*4)))>>((sizeof(data)*2-i-1)*4));
        if(val <= 9)
        {
            char_output('0'+val,usr);
        }
        else
        {
            char_output('A'+val-10,usr);
        }
    }
    char_output('H',usr);
}

void hs_mcs_51_disassembly_print(const uint8_t *instruction,size_t length,void (*char_output)(const char ch,void *usr),void *usr)
{
    if(char_output==NULL)
    {
        return;
    }
    if(instruction == NULL || length < hs_mcs_51_disassembly_instruction_length(instruction))
    {
        //指令数据不对
        return;
    }
    const hs_mcs_51_disassembly_instruction_t * disasm=hs_mcs_51_disassembly_instruction_table_get(instruction);
    if(disasm==NULL || disasm->mnemonic == NULL)
    {
        //未找到反汇编
        return;
    }
    const char *mnemoic=disasm->mnemonic;
    while((*mnemoic)!='\0')
    {
        switch(*mnemoic)
        {
        case '%':
        {
            mnemoic++;
            switch(*mnemoic)
            {
            case 'A':
            {
                uint16_t addr11=256*(instruction[0]>>5)+instruction[1];
                hs_mcs_51_disassembly_print_uint16(addr11,char_output,usr);
                mnemoic++;
            }
            break;
            case 'l':
            {
                uint16_t addr=256*instruction[1]+instruction[2];
                hs_mcs_51_disassembly_print_uint16(addr,char_output,usr);
                mnemoic++;
            }
            break;
            case 'a':
            {
                uint8_t addr=instruction[1];
                hs_mcs_51_disassembly_print_uint8(addr,char_output,usr);
                mnemoic++;
            }
            break;
            case 'b':
            {
                uint8_t bit_address=instruction[1];
                hs_mcs_51_disassembly_print_uint8(bit_address,char_output,usr);
                mnemoic++;
            }
            break;
            case 'R':
            {
                int8_t  rel_addr=instruction[2];
                hs_mcs_51_disassembly_print_int8(rel_addr,char_output,usr);
                mnemoic++;
            }
            break;
            case 'd':
            {
                uint8_t data=instruction[1];
                hs_mcs_51_disassembly_print_uint8(data,char_output,usr);
                mnemoic++;
            }
            break;
            case 'D':
            {
                uint8_t data=instruction[2];
                hs_mcs_51_disassembly_print_uint8(data,char_output,usr);
                mnemoic++;
            }
            break;
            case 'r':
            {
                int8_t  rel_addr=instruction[1];
                hs_mcs_51_disassembly_print_int8(rel_addr,char_output,usr);
                mnemoic++;
            }
            break;
            case '8':
            {
                uint8_t addr_dst=instruction[2];
                hs_mcs_51_disassembly_print_uint8(addr_dst,char_output,usr);
                mnemoic++;
            }
            break;
            case '6':
            {
                uint8_t dph=instruction[1];
                uint8_t dpl=instruction[2];
                hs_mcs_51_disassembly_print_uint16(dph*256+dpl,char_output,usr);
                mnemoic++;
            }
            break;
            default:
            {

            }
            break;
            }
        }
        break;
        default:
        {
            char_output(*mnemoic,usr);
            mnemoic++;
        }
        break;
        }
    }
}

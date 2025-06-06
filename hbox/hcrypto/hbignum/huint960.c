/***************************************************************
 * Name:      huint960.c
 * Purpose:   实现huint960接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-23
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "huint960.h"

void huint960_load(huint960_t *num,const uint8_t *bytes,size_t bytes_count)
{
    if(num==NULL || bytes==NULL)
    {
        return;
    }
    size_t max_index=0;
    //将已有字节赋值
    for(size_t i=0; i<bytes_count;)
    {
        uint32_t val=0;
        size_t bytes_remain=bytes_count-i;
        if(bytes_remain >= 4)
        {
            val<<=8;
            val+=bytes[i+3];
        }
        if(bytes_remain >= 3)
        {
            val<<=8;
            val+=bytes[i+2];
        }
        if(bytes_remain >= 2)
        {
            val<<=8;
            val+=bytes[i+1];
        }
        if(bytes_remain >= 1)
        {
            val<<=8;
            val+=bytes[i+0];
        }
        {
            size_t index=(i/sizeof(num->val[0]));
            if(index < (sizeof(num->val)/sizeof(num->val[0])))
            {
                num->val[index]=val;
            }
            max_index=index;
        }
        if(bytes_remain >= sizeof(num->val[0]))
        {
            i+=sizeof(num->val[0]);
        }
        else
        {
            i+=bytes_remain;
        }
    }

    //清零剩余值
    for(size_t i=(max_index+1); i< (sizeof(num->val)/sizeof(num->val[0])); i++)
    {
        num->val[i]=0;
    }
}

void huint960_load_be(huint960_t *num,const uint8_t *bytes,size_t bytes_count)
{
    if(num==NULL || bytes==NULL)
    {
        return;
    }
    size_t max_index=0;
    if(bytes_count > sizeof(num->val))
    {
        //丢弃高位
        size_t bytes_overflow=bytes_count-sizeof(huint960_t);
        bytes+=bytes_overflow;
        bytes_count-=bytes_overflow;
    }
    //将已有字节赋值
    for(size_t i=0; i<bytes_count;)
    {
        uint32_t val=0;
        size_t bytes_remain=bytes_count-i;
        if(bytes_remain >= 4)
        {
            val<<=8;
            val+=bytes[(bytes_count-1-i)-3];
        }
        if(bytes_remain >= 3)
        {
            val<<=8;
            val+=bytes[(bytes_count-1-i)-2];
        }
        if(bytes_remain >= 2)
        {
            val<<=8;
            val+=bytes[(bytes_count-1-i)-1];
        }
        if(bytes_remain >= 1)
        {
            val<<=8;
            val+=bytes[(bytes_count-1-i)-0];
        }
        {
            size_t index=(i/sizeof(num->val[0]));
            if(index < (sizeof(num->val)/sizeof(num->val[0])))
            {
                num->val[index]=val;
            }
            max_index=index;
        }
        if(bytes_remain >= sizeof(num->val[0]))
        {
            i+=sizeof(num->val[0]);
        }
        else
        {
            i+=bytes_remain;
        }
    }

    //清零剩余值
    for(size_t i=(max_index+1); i< (sizeof(num->val)/sizeof(num->val[0])); i++)
    {
        num->val[i]=0;
    }
}

void huint960_load_uint32(huint960_t *num,uint32_t uint32_num)
{
    if(num==NULL)
    {
        return;
    }
    num->val[0]=uint32_num;

    //清零剩余值
    for(size_t i=(1); i< (sizeof(num->val)/sizeof(num->val[0])); i++)
    {
        num->val[i]=0;
    }
}

void huint960_load_uint64(huint960_t *num,uint64_t uint64_num)
{
    if(num==NULL)
    {
        return;
    }
    num->val[0]=((uint64_num>>0)&0xFFFFFFFF);
    num->val[1]=((uint64_num>>32)&0xFFFFFFFF);

    //清零剩余值
    for(size_t i=(2); i< (sizeof(num->val)/sizeof(num->val[0])); i++)
    {
        num->val[i]=0;
    }
}

void huint960_store(huint960_t *num,uint8_t *bytes,size_t bytes_count)
{
    if(num==NULL || bytes==NULL)
    {
        return;
    }
    for(size_t i=0; i< (sizeof(num->val)/sizeof(num->val[0])); i++)
    {
        if(sizeof(num->val[0])*i >= bytes_count)
        {
            break;
        }
        size_t bytes_remain=bytes_count-sizeof(num->val[0])*i;
        if(bytes_remain >=1)
        {
            bytes[sizeof(num->val[0])*i+0]=((num->val[i] >> 0)&0xFF);
        }
        if(bytes_remain >=2)
        {
            bytes[sizeof(num->val[0])*i+1]=((num->val[i] >> 8)&0xFF);
        }
        if(bytes_remain >=3)
        {
            bytes[sizeof(num->val[0])*i+2]=((num->val[i] >> 16)&0xFF);
        }
        if(bytes_remain >=4)
        {
            bytes[sizeof(num->val[0])*i+3]=((num->val[i] >> 24)&0xFF);
        }
    }
}

void huint960_store_be(huint960_t *num,uint8_t *bytes,size_t bytes_count)
{
    if(num==NULL || bytes==NULL)
    {
        return;
    }
    if(bytes_count > sizeof(num->val))
    {
        //高位置零
        size_t bytes_overflow=bytes_count-sizeof(huint960_t);
        memset(bytes,0,bytes_overflow);
        bytes+=bytes_overflow;
        bytes_count-=bytes_overflow;
    }
    for(size_t i=0; i< (sizeof(num->val)/sizeof(num->val[0])); i++)
    {
        if(sizeof(num->val[0])*i >= bytes_count)
        {
            break;
        }
        size_t bytes_remain=bytes_count-sizeof(num->val[0])*i;
        if(bytes_remain >=1)
        {
            bytes[(bytes_count-1)-sizeof(num->val[0])*i-0]=((num->val[i] >> 0)&0xFF);
        }
        if(bytes_remain >=2)
        {
            bytes[(bytes_count-1)-sizeof(num->val[0])*i-1]=((num->val[i] >> 8)&0xFF);
        }
        if(bytes_remain >=3)
        {
            bytes[(bytes_count-1)-sizeof(num->val[0])*i-2]=((num->val[i] >> 16)&0xFF);
        }
        if(bytes_remain >=4)
        {
            bytes[(bytes_count-1)-sizeof(num->val[0])*i-3]=((num->val[i] >> 24)&0xFF);
        }
    }
}

void huint960_store_uint32(const huint960_t *num,uint32_t* uint32_num)
{
    if(num==NULL || uint32_num ==NULL)
    {
        return;
    }
    (*uint32_num)=num->val[0];
}

void huint960_store_uint64(const huint960_t *num,uint64_t* uint64_num)
{
    if(num==NULL || uint64_num ==NULL)
    {
        return;
    }
    (*uint64_num)=num->val[0]+(((uint64_t)num->val[1]) << 32);
}

void huint960_copy(huint960_t *dst,const huint960_t *src)
{
    if(dst==NULL || src==NULL)
    {
        return;
    }
    for(size_t i=0; i< (sizeof(dst->val)/sizeof(dst->val[0])); i++)
    {
        dst->val[i]=src->val[i];
    }
}

void huint960_bitwise_not(huint960_t *dst,const huint960_t *src)
{
    if(dst==NULL || src==NULL)
    {
        return;
    }
    for(size_t i=0; i< (sizeof(dst->val)/sizeof(dst->val[0])); i++)
    {
        dst->val[i]=(~src->val[i]);
    }
}

void huint960_bitwise_and(huint960_t *dst,const huint960_t *src1,const huint960_t *src2)
{
    if(dst==NULL || src1==NULL || src2==NULL)
    {
        return;
    }
    for(size_t i=0; i< (sizeof(dst->val)/sizeof(dst->val[0])); i++)
    {
        dst->val[i]=(src1->val[i] & src2->val[i]);
    }
}

void huint960_bitwise_or(huint960_t *dst,const huint960_t *src1,const huint960_t *src2)
{
    if(dst==NULL || src1==NULL || src2==NULL)
    {
        return;
    }
    for(size_t i=0; i< (sizeof(dst->val)/sizeof(dst->val[0])); i++)
    {
        dst->val[i]=(src1->val[i] | src2->val[i]);
    }
}

void huint960_bitwise_xor(huint960_t *dst,const huint960_t *src1,const huint960_t *src2)
{
    if(dst==NULL || src1==NULL || src2==NULL)
    {
        return;
    }
    for(size_t i=0; i< (sizeof(dst->val)/sizeof(dst->val[0])); i++)
    {
        dst->val[i]=(src1->val[i] ^ src2->val[i]);
    }
}

int huint960_compare(const huint960_t *src1,const huint960_t *src2)
{
    if(src1==NULL || src2==NULL)
    {
        return -2;
    }
    for(int i=((sizeof(src1->val)/sizeof(src2->val[0]))-1); i>=0 ; i--)
    {
        if(src1->val[i] > src2->val[i])
        {
            return 1;
        }
        if(src1->val[i] < src2->val[i])
        {
            return -1;
        }
    }
    return 0;
}

void huint960_complement(huint960_t *dst,const huint960_t *src)
{
    if(dst==NULL || src==NULL)
    {
        return;
    }
    //取反
    huint960_bitwise_not(dst,src);

    //加1
    uint64_t temp=0;
    for(size_t i=0; i< (sizeof(dst->val)/sizeof(dst->val[0])); i++)
    {
        if(i==0)
        {
            temp+=((uint64_t)dst->val[i])+1;
            dst->val[i]=(uint32_t)temp;
            temp>>=32;
        }
        else
        {
            temp+=((uint64_t)dst->val[i]);
            dst->val[i]=(uint32_t)temp;
            temp>>=32;
        }
    }
}

static size_t huint960_left_shift_internal(huint960_t *dst,const huint960_t *src,size_t bits)
{
    if(dst==NULL || src==NULL)
    {
        return 0;
    }

    size_t bit_shift=sizeof(uint32_t)*8;
    if(bit_shift>bits)
    {
        bit_shift=bits;
    }

    uint64_t temp=0;
    for(size_t i=0; i< (sizeof(dst->val)/sizeof(dst->val[0])); i++)
    {
        temp=((((uint64_t)src->val[i]) << (bit_shift)) | temp);
        dst->val[i]=(uint32_t)temp;
        temp >>= (sizeof(uint32_t)*8);
    }

    return bit_shift;
}

void huint960_left_shift(huint960_t *dst,const huint960_t *src,size_t bits)
{
    if(dst==NULL || src==NULL)
    {
        return;
    }

    if(bits >= HUINT960_BITS_COUNT)
    {
        //大于等于大数的位数，直接置0
        huint960_load_uint32(dst,0);
        return;
    }

    huint960_copy(dst,src);

    while(bits > 0)
    {
        bits-=huint960_left_shift_internal(dst,dst,bits);
    }
}

static size_t huint960_right_shift_internal(huint960_t *dst,const huint960_t *src,size_t bits)
{
    if(dst==NULL || src==NULL)
    {
        return 0;
    }

    size_t bit_shift=sizeof(uint32_t)*8;
    if(bit_shift>bits)
    {
        bit_shift=bits;
    }

    uint64_t temp=0;
    for(int i=((sizeof(dst->val)/sizeof(dst->val[0]))-1); i>=0 ; i--)
    {
        temp = (((((uint64_t)src->val[i]) << (sizeof(uint32_t)*8)) >> (bit_shift)) | temp);
        dst->val[i]=(uint32_t)(temp >> (sizeof(uint32_t)*8));
        temp <<= (sizeof(uint32_t)*8);
    }

    return bit_shift;
}


void huint960_right_shift(huint960_t *dst,const huint960_t *src,size_t bits)
{
    if(dst==NULL || src==NULL)
    {
        return;
    }

    if(bits >= HUINT960_BITS_COUNT)
    {
        //大于等于大数的位数，直接置0
        huint960_load_uint32(dst,0);
        return;
    }

    huint960_copy(dst,src);

    while(bits > 0)
    {
        bits-=huint960_right_shift_internal(dst,dst,bits);
    }
}

void huint960_bit_set(huint960_t *dst,size_t bits)
{
    if(dst!=NULL)
    {
        if(bits >= HUINT960_BITS_COUNT)
        {
            return;
        }

        dst->val[bits/(sizeof(dst->val[0])*8)] |= (1ULL << (bits%(sizeof(dst->val[0])*8)));
    }
}

void huint960_bit_clear(huint960_t *dst,size_t bits)
{
    if(dst!=NULL)
    {
        if(bits >= HUINT960_BITS_COUNT)
        {
            return;
        }

        dst->val[bits/(sizeof(dst->val[0])*8)] &= (~(1ULL << (bits%(sizeof(dst->val[0])*8))));
    }
}

bool huint960_bit(const huint960_t *dst,size_t bits)
{
    if(dst!=NULL)
    {
        if(bits >= HUINT960_BITS_COUNT)
        {
            return false;
        }

        return 0!=(dst->val[bits/(sizeof(dst->val[0])*8)] & (1ULL << (bits%(sizeof(dst->val[0])*8))));
    }

    return false;
}

size_t huint960_clz(const huint960_t *dst)
{
    if(dst!=NULL)
    {
        for(size_t i=0; i < HUINT960_BITS_COUNT; i++)
        {
            if(huint960_bit(dst,HUINT960_BITS_COUNT-1-i))
            {
                return i;
            }
        }
    }

    return HUINT960_BITS_COUNT;
}

size_t huint960_ctz(const huint960_t *dst)
{
    if(dst!=NULL)
    {
        for(size_t i=0; i < HUINT960_BITS_COUNT; i++)
        {
            if(huint960_bit(dst,i))
            {
                return i;
            }
        }
    }

    return HUINT960_BITS_COUNT;
}

void huint960_add(huint960_t *dst,const huint960_t *src1,const huint960_t *src2)
{
    if(dst==NULL || src1==NULL || src2 == NULL)
    {
        return;
    }

    uint64_t temp=0;
    for(size_t i=0; i< (sizeof(dst->val)/sizeof(dst->val[0])); i++)
    {
        temp=((uint64_t)src1->val[i])+src2->val[i]+temp;
        dst->val[i]=(uint32_t)temp;
        temp>>=32;
    }
}

void huint960_sub(huint960_t *dst,const huint960_t *src1,const huint960_t *src2)
{
    if(dst==NULL || src1==NULL || src2 == NULL)
    {
        return;
    }

    //求补码
    huint960_complement(dst,src2);

    //对补码进行加
    huint960_add(dst,dst,src1);
}

void huint960_mul(huint960_t *state,huint960_t *dst,const huint960_t *src1,const huint960_t *src2)
{
    if(state == NULL || dst==NULL || src1==NULL || src2 == NULL)
    {
        return;
    }

    huint960_load_uint32(dst,0);
    size_t clz=huint960_clz(src2);
    size_t ctz=huint960_ctz(src2);
    for(size_t i=ctz; i < (HUINT960_BITS_COUNT-clz); i++)
    {
        if(huint960_bit(src2,i))
        {
            //当前位是1, src1左移后累加至结果
            huint960_left_shift(state,src1,i);
            huint960_add(dst,dst,state);
        }
    }
}

void huint960_div(huint960_t *state,huint960_t *state1,huint960_t *state2,huint960_t *dst,const huint960_t *src1,const huint960_t *src2)
{
    if(state == NULL || state1==NULL || state2== NULL || dst==NULL || src1==NULL || src2 == NULL)
    {
        return;
    }


    huint960_load_uint32(dst,0);
    if(huint960_compare(src2,state)==0 )
    {
        huint960_load_uint32(state,0);
        //除0错误
        return;
    }

    if(huint960_compare(src2,src1) > 0)
    {
        //除数大于被除数
        huint960_copy(state,src1);
        huint960_load_uint32(dst,0);
        return;
    }

    size_t clz1=huint960_clz(src1);
    size_t clz2=huint960_clz(src2);
    if(clz1 > clz2)
    {
        //除数大于被除数
        huint960_copy(state,src1);
        huint960_load_uint32(dst,0);
        return;
    }

    huint960_copy(state,src1);

    for(size_t i=0; i<= clz2-clz1; i++)
    {
        huint960_left_shift(state1,src2,(clz2-clz1)-i);
        if(huint960_compare(state,state1) >= 0)
        {
            //被除数大于左移后的除数，直接相减并将相应位置1
            huint960_sub(state2,state,state1);
            huint960_bit_set(dst,(clz2-clz1)-i);
            //余数保存至state
            huint960_copy(state,state2);
        }
    }

}

void huint960_div_with_stack(huint960_t *mod,huint960_t *dst,const huint960_t *src1,const huint960_t *src2)
{
    huint960_t state[3]= {0};
    huint960_div(&state[0],&state[1],&state[2],dst,src1,src2);
    if(mod!=NULL)
    {
        huint960_copy(mod,&state[0]);
    }
}

void huint960_power(huint960_t *state,huint960_t *state1,huint960_t *state2,huint960_t *dst,const huint960_t *src1,const huint960_t *src2)
{
    if(state == NULL || state1==NULL || state2== NULL || dst==NULL || src1==NULL || src2 == NULL)
    {
        return;
    }

    huint960_load_uint32(state,0);
    huint960_load_uint32(dst,1);
    if(huint960_compare(src1,state)==0 )
    {
        //底数为0
        huint960_load_uint32(dst,0);
        return;
    }
    if(huint960_compare(src2,state)==0 )
    {
        //任意数的0次方=1
        huint960_load_uint32(dst,1);
        return;
    }

    size_t clz2=huint960_clz(src2);
    huint960_copy(state,src1);
    for(size_t i=0; i<(HUINT960_BITS_COUNT - clz2); i++)
    {
        if(huint960_bit(src2,i))
        {
            //,结果应当乘上当前state的值(将幂函数的指数按照2进制进行拆分成乘法表达式)
            huint960_mul(state1,state2,dst,state);
            huint960_copy(dst,state2);
        }
        //计算state的平方（state的平方相当于state的指数乘2，正好对应src2下一个二进制位）
        huint960_mul(state1,state2,state,state);
        huint960_copy(state,state2);
    }

}

void huint960_power_with_stack(huint960_t *dst,const huint960_t *src1,const huint960_t *src2)
{
    huint960_t state[3]= {0};
    huint960_power(&state[0],&state[1],&state[2],dst,src1,src2);
}

void huint960_power_mod(huint960_t *state,huint960_t *state1,huint960_t *state2,huint960_t *state3,huint960_t *dst,const huint960_t *src1,const huint960_t *src2,const huint960_t *src3)
{
    if(state == NULL || state1==NULL || state2== NULL || state3 == NULL || dst==NULL || src1==NULL || src2 == NULL)
    {
        return;
    }

    huint960_load_uint32(state,0);
    huint960_load_uint32(dst,1);
    if(huint960_compare(src1,state)==0 )
    {
        //底数为0
        huint960_load_uint32(dst,0);
        return;
    }
    if(huint960_compare(src2,state)==0 )
    {
        //任意数的0次方=1
        huint960_load_uint32(dst,1);
        return;
    }

    size_t clz2=huint960_clz(src2);
    huint960_copy(state,src1);
    for(size_t i=0; i<(HUINT960_BITS_COUNT - clz2); i++)
    {
        if(huint960_bit(src2,i))
        {
            //,结果应当乘上当前state的值(将幂函数的指数按照2进制进行拆分成乘法表达式)
            huint960_mul(state1,state2,dst,state);
            huint960_copy(dst,state2);

            //对dst提前取模，防止计算过程溢出(先取模再做乘法=先做乘法再取模)
            huint960_div(dst,state1,state2,state3,dst,src3);
        }
        //计算state的平方（state的平方相当于state的指数乘2，正好对应src2下一个二进制位）
        huint960_mul(state1,state2,state,state);
        huint960_copy(state,state2);

        //对state提前取模，防止计算过程溢出(先取模再做乘法=先做乘法再取模)
        huint960_div(state,state1,state2,state3,state,src3);
    }

}

void huint960_power_mod_with_stack(huint960_t *dst,const huint960_t *src1,const huint960_t *src2,const huint960_t *src3)
{
    huint960_t state[4]= {0};
    huint960_power_mod(&state[0],&state[1],&state[2],&state[3],dst,src1,src2,src3);
}

/***************************************************************
 * Name:      huint4192.c
 * Purpose:   实现huint4192接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-23
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "huint4192.h"

void huint4192_load(huint4192_t *num,const uint8_t *bytes,size_t bytes_count)
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

void huint4192_load_be(huint4192_t *num,const uint8_t *bytes,size_t bytes_count)
{
    if(num==NULL || bytes==NULL)
    {
        return;
    }
    size_t max_index=0;
    if(bytes_count > sizeof(num->val))
    {
        //丢弃高位
        size_t bytes_overflow=bytes_count-sizeof(huint4192_t);
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

void huint4192_load_uint32(huint4192_t *num,uint32_t uint32_num)
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

void huint4192_load_uint64(huint4192_t *num,uint64_t uint64_num)
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

void huint4192_store(huint4192_t *num,uint8_t *bytes,size_t bytes_count)
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

void huint4192_store_be(huint4192_t *num,uint8_t *bytes,size_t bytes_count)
{
    if(num==NULL || bytes==NULL)
    {
        return;
    }
    if(bytes_count > sizeof(num->val))
    {
        //高位置零
        size_t bytes_overflow=bytes_count-sizeof(huint4192_t);
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

void huint4192_store_uint32(const huint4192_t *num,uint32_t* uint32_num)
{
    if(num==NULL || uint32_num ==NULL)
    {
        return;
    }
    (*uint32_num)=num->val[0];
}

void huint4192_store_uint64(const huint4192_t *num,uint64_t* uint64_num)
{
    if(num==NULL || uint64_num ==NULL)
    {
        return;
    }
    (*uint64_num)=num->val[0]+(((uint64_t)num->val[1]) << 32);
}

void huint4192_copy(huint4192_t *dst,const huint4192_t *src)
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

void huint4192_bitwise_not(huint4192_t *dst,const huint4192_t *src)
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

void huint4192_bitwise_and(huint4192_t *dst,const huint4192_t *src1,const huint4192_t *src2)
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

void huint4192_bitwise_or(huint4192_t *dst,const huint4192_t *src1,const huint4192_t *src2)
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

void huint4192_bitwise_xor(huint4192_t *dst,const huint4192_t *src1,const huint4192_t *src2)
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

int huint4192_compare(const huint4192_t *src1,const huint4192_t *src2)
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

void huint4192_complement(huint4192_t *dst,const huint4192_t *src)
{
    if(dst==NULL || src==NULL)
    {
        return;
    }
    //取反
    huint4192_bitwise_not(dst,src);

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

static size_t huint4192_left_shift_internal(huint4192_t *dst,const huint4192_t *src,size_t bits)
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

void huint4192_left_shift(huint4192_t *dst,const huint4192_t *src,size_t bits)
{
    if(dst==NULL || src==NULL)
    {
        return;
    }

    if(bits >= HUINT4192_BITS_COUNT)
    {
        //大于等于大数的位数，直接置0
        huint4192_load_uint32(dst,0);
        return;
    }

    huint4192_copy(dst,src);

    while(bits > 0)
    {
        bits-=huint4192_left_shift_internal(dst,dst,bits);
    }
}

static size_t huint4192_right_shift_internal(huint4192_t *dst,const huint4192_t *src,size_t bits)
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


void huint4192_right_shift(huint4192_t *dst,const huint4192_t *src,size_t bits)
{
    if(dst==NULL || src==NULL)
    {
        return;
    }

    if(bits >= HUINT4192_BITS_COUNT)
    {
        //大于等于大数的位数，直接置0
        huint4192_load_uint32(dst,0);
        return;
    }

    huint4192_copy(dst,src);

    while(bits > 0)
    {
        bits-=huint4192_right_shift_internal(dst,dst,bits);
    }
}

void huint4192_bit_set(huint4192_t *dst,size_t bits)
{
    if(dst!=NULL)
    {
        if(bits >= HUINT4192_BITS_COUNT)
        {
            return;
        }

        dst->val[bits/(sizeof(dst->val[0])*8)] |= (1ULL << (bits%(sizeof(dst->val[0])*8)));
    }
}

void huint4192_bit_clear(huint4192_t *dst,size_t bits)
{
    if(dst!=NULL)
    {
        if(bits >= HUINT4192_BITS_COUNT)
        {
            return;
        }

        dst->val[bits/(sizeof(dst->val[0])*8)] &= (~(1ULL << (bits%(sizeof(dst->val[0])*8))));
    }
}

bool huint4192_bit(const huint4192_t *dst,size_t bits)
{
    if(dst!=NULL)
    {
        if(bits >= HUINT4192_BITS_COUNT)
        {
            return false;
        }

        return 0!=(dst->val[bits/(sizeof(dst->val[0])*8)] & (1ULL << (bits%(sizeof(dst->val[0])*8))));
    }

    return false;
}

size_t huint4192_clz(const huint4192_t *dst)
{
    if(dst!=NULL)
    {
        for(size_t i=0; i < HUINT4192_BITS_COUNT; i++)
        {
            if(huint4192_bit(dst,HUINT4192_BITS_COUNT-1-i))
            {
                return i;
            }
        }
    }

    return HUINT4192_BITS_COUNT;
}

size_t huint4192_ctz(const huint4192_t *dst)
{
    if(dst!=NULL)
    {
        for(size_t i=0; i < HUINT4192_BITS_COUNT; i++)
        {
            if(huint4192_bit(dst,i))
            {
                return i;
            }
        }
    }

    return HUINT4192_BITS_COUNT;
}

void huint4192_add(huint4192_t *dst,const huint4192_t *src1,const huint4192_t *src2)
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

void huint4192_sub(huint4192_t *dst,const huint4192_t *src1,const huint4192_t *src2)
{
    if(dst==NULL || src1==NULL || src2 == NULL)
    {
        return;
    }

    //求补码
    huint4192_complement(dst,src2);

    //对补码进行加
    huint4192_add(dst,dst,src1);
}

void huint4192_mul(huint4192_t *state,huint4192_t *dst,const huint4192_t *src1,const huint4192_t *src2)
{
    if(state == NULL || dst==NULL || src1==NULL || src2 == NULL)
    {
        return;
    }

    huint4192_load_uint32(dst,0);
    size_t clz=huint4192_clz(src2);
    size_t ctz=huint4192_ctz(src2);
    for(size_t i=ctz; i < (HUINT4192_BITS_COUNT-clz); i++)
    {
        if(huint4192_bit(src2,i))
        {
            //当前位是1, src1左移后累加至结果
            huint4192_left_shift(state,src1,i);
            huint4192_add(dst,dst,state);
        }
    }
}

void huint4192_div(huint4192_t *state,huint4192_t *state1,huint4192_t *state2,huint4192_t *dst,const huint4192_t *src1,const huint4192_t *src2)
{
    if(state == NULL || state1==NULL || state2== NULL || dst==NULL || src1==NULL || src2 == NULL)
    {
        return;
    }


    huint4192_load_uint32(dst,0);
    if(huint4192_compare(src2,state)==0 )
    {
        huint4192_load_uint32(state,0);
        //除0错误
        return;
    }

    if(huint4192_compare(src2,src1) > 0)
    {
        //除数大于被除数
        huint4192_copy(state,src1);
        huint4192_load_uint32(dst,0);
        return;
    }

    size_t clz1=huint4192_clz(src1);
    size_t clz2=huint4192_clz(src2);
    if(clz1 > clz2)
    {
        //除数大于被除数
        huint4192_copy(state,src1);
        huint4192_load_uint32(dst,0);
        return;
    }

    huint4192_copy(state,src1);

    for(size_t i=0; i<= clz2-clz1; i++)
    {
        huint4192_left_shift(state1,src2,(clz2-clz1)-i);
        if(huint4192_compare(state,state1) >= 0)
        {
            //被除数大于左移后的除数，直接相减并将相应位置1
            huint4192_sub(state2,state,state1);
            huint4192_bit_set(dst,(clz2-clz1)-i);
            //余数保存至state
            huint4192_copy(state,state2);
        }
    }

}

void huint4192_div_with_stack(huint4192_t *mod,huint4192_t *dst,const huint4192_t *src1,const huint4192_t *src2)
{
    huint4192_t state[3]= {0};
    huint4192_div(&state[0],&state[1],&state[2],dst,src1,src2);
    if(mod!=NULL)
    {
        huint4192_copy(mod,&state[0]);
    }
}

void huint4192_power(huint4192_t *state,huint4192_t *state1,huint4192_t *state2,huint4192_t *dst,const huint4192_t *src1,const huint4192_t *src2)
{
    if(state == NULL || state1==NULL || state2== NULL || dst==NULL || src1==NULL || src2 == NULL)
    {
        return;
    }

    huint4192_load_uint32(state,0);
    huint4192_load_uint32(dst,1);
    if(huint4192_compare(src1,state)==0 )
    {
        //底数为0
        huint4192_load_uint32(dst,0);
        return;
    }
    if(huint4192_compare(src2,state)==0 )
    {
        //任意数的0次方=1
        huint4192_load_uint32(dst,1);
        return;
    }

    size_t clz2=huint4192_clz(src2);
    huint4192_copy(state,src1);
    for(size_t i=0; i<(HUINT4192_BITS_COUNT - clz2); i++)
    {
        if(huint4192_bit(src2,i))
        {
            //,结果应当乘上当前state的值(将幂函数的指数按照2进制进行拆分成乘法表达式)
            huint4192_mul(state1,state2,dst,state);
            huint4192_copy(dst,state2);
        }
        //计算state的平方（state的平方相当于state的指数乘2，正好对应src2下一个二进制位）
        huint4192_mul(state1,state2,state,state);
        huint4192_copy(state,state2);
    }

}

void huint4192_power_with_stack(huint4192_t *dst,const huint4192_t *src1,const huint4192_t *src2)
{
    huint4192_t state[3]= {0};
    huint4192_power(&state[0],&state[1],&state[2],dst,src1,src2);
}

void huint4192_power_mod(huint4192_t *state,huint4192_t *state1,huint4192_t *state2,huint4192_t *state3,huint4192_t *dst,const huint4192_t *src1,const huint4192_t *src2,const huint4192_t *src3)
{
    if(state == NULL || state1==NULL || state2== NULL || state3 == NULL || dst==NULL || src1==NULL || src2 == NULL)
    {
        return;
    }

    huint4192_load_uint32(state,0);
    huint4192_load_uint32(dst,1);
    if(huint4192_compare(src1,state)==0 )
    {
        //底数为0
        huint4192_load_uint32(dst,0);
        return;
    }
    if(huint4192_compare(src2,state)==0 )
    {
        //任意数的0次方=1
        huint4192_load_uint32(dst,1);
        return;
    }

    size_t clz2=huint4192_clz(src2);
    huint4192_copy(state,src1);
    for(size_t i=0; i<(HUINT4192_BITS_COUNT - clz2); i++)
    {
        if(huint4192_bit(src2,i))
        {
            //,结果应当乘上当前state的值(将幂函数的指数按照2进制进行拆分成乘法表达式)
            huint4192_mul(state1,state2,dst,state);
            huint4192_copy(dst,state2);

            //对dst提前取模，防止计算过程溢出(先取模再做乘法=先做乘法再取模)
            huint4192_div(dst,state1,state2,state3,dst,src3);
        }
        //计算state的平方（state的平方相当于state的指数乘2，正好对应src2下一个二进制位）
        huint4192_mul(state1,state2,state,state);
        huint4192_copy(state,state2);

        //对state提前取模，防止计算过程溢出(先取模再做乘法=先做乘法再取模)
        huint4192_div(state,state1,state2,state3,state,src3);
    }

}

void huint4192_power_mod_with_stack(huint4192_t *dst,const huint4192_t *src1,const huint4192_t *src2,const huint4192_t *src3)
{
    huint4192_t state[4]= {0};
    huint4192_power_mod(&state[0],&state[1],&state[2],&state[3],dst,src1,src2,src3);
}

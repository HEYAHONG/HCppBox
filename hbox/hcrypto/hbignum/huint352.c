/***************************************************************
 * Name:      huint352.c
 * Purpose:   实现huint352接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-23
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "huint352.h"

void huint352_load(huint352_t *num,const uint8_t *bytes,size_t bytes_count)
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

void huint352_load_be(huint352_t *num,const uint8_t *bytes,size_t bytes_count)
{
    if(num==NULL || bytes==NULL)
    {
        return;
    }
    size_t max_index=0;
    if(bytes_count > sizeof(num->val))
    {
        //丢弃高位
        size_t bytes_overflow=bytes_count-sizeof(huint352_t);
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

void huint352_load_uint32(huint352_t *num,uint32_t uint32_num)
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

void huint352_load_uint64(huint352_t *num,uint64_t uint64_num)
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

void huint352_store(huint352_t *num,uint8_t *bytes,size_t bytes_count)
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

void huint352_store_be(huint352_t *num,uint8_t *bytes,size_t bytes_count)
{
    if(num==NULL || bytes==NULL)
    {
        return;
    }
    if(bytes_count > sizeof(num->val))
    {
        //高位置零
        size_t bytes_overflow=bytes_count-sizeof(huint352_t);
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

void huint352_store_uint32(const huint352_t *num,uint32_t* uint32_num)
{
    if(num==NULL || uint32_num ==NULL)
    {
        return;
    }
    (*uint32_num)=num->val[0];
}

void huint352_store_uint64(const huint352_t *num,uint64_t* uint64_num)
{
    if(num==NULL || uint64_num ==NULL)
    {
        return;
    }
    (*uint64_num)=num->val[0]+(((uint64_t)num->val[1]) << 32);
}

void huint352_copy(huint352_t *dst,const huint352_t *src)
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

void huint352_bitwise_not(huint352_t *dst,const huint352_t *src)
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

void huint352_bitwise_and(huint352_t *dst,const huint352_t *src1,const huint352_t *src2)
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

void huint352_bitwise_or(huint352_t *dst,const huint352_t *src1,const huint352_t *src2)
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

void huint352_bitwise_xor(huint352_t *dst,const huint352_t *src1,const huint352_t *src2)
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

int huint352_compare(const huint352_t *src1,const huint352_t *src2)
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

void huint352_complement(huint352_t *dst,const huint352_t *src)
{
    if(dst==NULL || src==NULL)
    {
        return;
    }
    //取反
    huint352_bitwise_not(dst,src);

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

static size_t huint352_left_shift_internal(huint352_t *dst,const huint352_t *src,size_t bits)
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

void huint352_left_shift(huint352_t *dst,const huint352_t *src,size_t bits)
{
    if(dst==NULL || src==NULL)
    {
        return;
    }

    if(bits >= HUINT352_BITS_COUNT)
    {
        //大于等于大数的位数，直接置0
        huint352_load_uint32(dst,0);
        return;
    }

    huint352_copy(dst,src);

    while(bits > 0)
    {
        bits-=huint352_left_shift_internal(dst,dst,bits);
    }
}

static size_t huint352_right_shift_internal(huint352_t *dst,const huint352_t *src,size_t bits)
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


void huint352_right_shift(huint352_t *dst,const huint352_t *src,size_t bits)
{
    if(dst==NULL || src==NULL)
    {
        return;
    }

    if(bits >= HUINT352_BITS_COUNT)
    {
        //大于等于大数的位数，直接置0
        huint352_load_uint32(dst,0);
        return;
    }

    huint352_copy(dst,src);

    while(bits > 0)
    {
        bits-=huint352_right_shift_internal(dst,dst,bits);
    }
}

void huint352_bit_set(huint352_t *dst,size_t bits)
{
    if(dst!=NULL)
    {
        if(bits >= HUINT352_BITS_COUNT)
        {
            return;
        }

        dst->val[bits/(sizeof(dst->val[0])*8)] |= (1ULL << (bits%(sizeof(dst->val[0])*8)));
    }
}

void huint352_bit_clear(huint352_t *dst,size_t bits)
{
    if(dst!=NULL)
    {
        if(bits >= HUINT352_BITS_COUNT)
        {
            return;
        }

        dst->val[bits/(sizeof(dst->val[0])*8)] &= (~(1ULL << (bits%(sizeof(dst->val[0])*8))));
    }
}

bool huint352_bit(const huint352_t *dst,size_t bits)
{
    if(dst!=NULL)
    {
        if(bits >= HUINT352_BITS_COUNT)
        {
            return false;
        }

        return 0!=(dst->val[bits/(sizeof(dst->val[0])*8)] & (1ULL << (bits%(sizeof(dst->val[0])*8))));
    }

    return false;
}

size_t huint352_clz(const huint352_t *dst)
{
    if(dst!=NULL)
    {
        size_t index_start=0;
        for(size_t i=0; i<sizeof(dst->val)/sizeof(dst->val[0]); i++)
        {
            if(dst->val[sizeof(dst->val)/sizeof(dst->val[0])-1-i]!=0)
            {
                break;
            }
            index_start+=(sizeof(dst->val[0])*8);
        }

        for(size_t i=index_start; i < HUINT352_BITS_COUNT; i++)
        {
            if(huint352_bit(dst,HUINT352_BITS_COUNT-1-i))
            {
                return i;
            }
        }
    }

    return HUINT352_BITS_COUNT;
}

size_t huint352_ctz(const huint352_t *dst)
{
    if(dst!=NULL)
    {
        size_t index_start=0;
        for(size_t i=0; i<sizeof(dst->val)/sizeof(dst->val[0]); i++)
        {
            if(dst->val[i]!=0)
            {
                break;
            }
            index_start+=(sizeof(dst->val[0])*8);
        }

        for(size_t i=index_start; i < HUINT352_BITS_COUNT; i++)
        {
            if(huint352_bit(dst,i))
            {
                return i;
            }
        }
    }

    return HUINT352_BITS_COUNT;
}

bool huint352_t_is_zero(const huint352_t * src)
{
    bool ret=false;
    if(src==NULL)
    {
        return ret;
    }
    for(size_t i=0; i< sizeof(src->val)/sizeof(src->val[0]); i++)
    {
        if(i==0)
        {
            ret=(src->val[0]==0);
        }
        else
        {
            ret=(src->val[i]==0);
        }

        if(!ret)
        {
            break;
        }
    }

    return ret;
}

bool huint352_t_is_one(const huint352_t * src)
{
    bool ret=false;
    if(src==NULL)
    {
        return ret;
    }
    for(size_t i=0; i< sizeof(src->val)/sizeof(src->val[0]); i++)
    {
        if(i==0)
        {
            ret=(src->val[0]==1);
        }
        else
        {
            ret=(src->val[i]==0);
        }

        if(!ret)
        {
            break;
        }
    }

    return ret;
}

void huint352_add(huint352_t *dst,const huint352_t *src1,const huint352_t *src2)
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

void huint352_sub(huint352_t *dst,const huint352_t *src1,const huint352_t *src2)
{
    if(dst==NULL || src1==NULL || src2 == NULL)
    {
        return;
    }

    //求补码
    huint352_complement(dst,src2);

    //对补码进行加
    huint352_add(dst,dst,src1);
}

void huint352_mul(huint352_t *state,huint352_t *dst,const huint352_t *src1,const huint352_t *src2)
{
    if(state == NULL || dst==NULL || src1==NULL || src2 == NULL)
    {
        return;
    }

    huint352_load_uint32(dst,0);
    size_t max_index=0;
    size_t min_index=0;
    for(size_t i=0; i<sizeof(src2->val)/sizeof(src2->val[0]); i++)
    {
        if(src2->val[i]!=0)
        {
            min_index=i;
            break;
        }
    }
    for(size_t i=0; i<sizeof(src2->val)/sizeof(src2->val[0]); i++)
    {
        if(src2->val[sizeof(src2->val)/sizeof(src2->val[0])-1-i]!=0)
        {
            max_index=sizeof(src2->val)/sizeof(src2->val[0])-1-i;
            break;
        }
    }
    for(size_t i=min_index; i<=max_index; i++)
    {
        uint64_t temp=0;
        for(size_t k=0; k<i; k++)
        {
            state->val[k]=0;
        }
        for(size_t j=0; j<(sizeof(src1->val)/sizeof(src1->val[0])); j++)
        {
            if(i+j >= (sizeof(state->val)/sizeof(state->val[0])))
            {
                break;
            }
            temp+=src1->val[j]*((uint64_t)src2->val[i]);
            state->val[i+j]=temp;
            temp>>=(sizeof(state->val[0])*8);
        }
        huint352_add(dst,dst,state);
    }
}

void huint352_mul_with_stack(huint352_t *dst,const huint352_t *src1,const huint352_t *src2)
{
    huint352_t state;

    huint352_mul(&state,dst,src1,src2);
}

void huint352_mul_with_external_state(huint352_state_t *state,huint352_t *dst,const huint352_t *src1,const huint352_t *src2)
{
    if(state==NULL)
    {
        return;
    }

    huint352_mul(&state->state[0],dst,src1,src2);
}

void huint352_div(huint352_t *state,huint352_t *state1,huint352_t *state2,huint352_t *dst,const huint352_t *src1,const huint352_t *src2)
{
    if(state == NULL || state1==NULL || state2== NULL || dst==NULL || src1==NULL || src2 == NULL)
    {
        return;
    }


    huint352_load_uint32(dst,0);
    if(huint352_compare(src2,dst)==0 )
    {
        huint352_load_uint32(state,0);
        //除0错误
        return;
    }

    if(huint352_compare(src2,src1) > 0)
    {
        //除数大于被除数
        huint352_copy(state,src1);
        huint352_load_uint32(dst,0);
        return;
    }

    size_t clz1=huint352_clz(src1);
    size_t clz2=huint352_clz(src2);
    if(clz1 > clz2)
    {
        //除数大于被除数
        huint352_copy(state,src1);
        huint352_load_uint32(dst,0);
        return;
    }

    huint352_copy(state,src1);
    size_t last_index=0;
    huint352_left_shift(state1,src2,(clz2-clz1)-last_index);
    for(size_t i=0; i<= clz2-clz1; i++)
    {
        huint352_right_shift_internal(state1,state1,i-last_index);
        last_index=i;
        if(huint352_compare(state,state1) >= 0)
        {
            //被除数大于左移后的除数，直接相减并将相应位置1
            {
                //求补码
                huint352_complement(state2,state1);
                //对补码进行加
                huint352_add(state,state,state2);
            }
            huint352_bit_set(dst,(clz2-clz1)-i);
        }
    }

}

void huint352_div_with_stack(huint352_t *mod,huint352_t *dst,const huint352_t *src1,const huint352_t *src2)
{
    huint352_t state[3]= {0};
    huint352_div(&state[0],&state[1],&state[2],dst,src1,src2);
    if(mod!=NULL)
    {
        huint352_copy(mod,&state[0]);
    }
}

void huint352_div_with_external_state(huint352_state_t * state,huint352_t *mod,huint352_t *dst,const huint352_t *src1,const huint352_t *src2)
{
    if(state==NULL)
    {
        return;
    }
    huint352_div(&state->state[0],&state->state[1],&state->state[2],dst,src1,src2);
    if(mod!=NULL)
    {
        huint352_copy(mod,&state->state[0]);
    }
}

void huint352_power(huint352_t *state,huint352_t *state1,huint352_t *state2,huint352_t *dst,const huint352_t *src1,const huint352_t *src2)
{
    if(state == NULL || state1==NULL || state2== NULL || dst==NULL || src1==NULL || src2 == NULL)
    {
        return;
    }

    huint352_load_uint32(state,0);
    huint352_load_uint32(dst,1);
    if(huint352_compare(src1,state)==0 )
    {
        //底数为0
        huint352_load_uint32(dst,0);
        return;
    }
    if(huint352_compare(src2,state)==0 )
    {
        //任意数的0次方=1
        huint352_load_uint32(dst,1);
        return;
    }

    size_t clz2=huint352_clz(src2);
    huint352_copy(state,src1);
    for(size_t i=0; i<(HUINT352_BITS_COUNT - clz2); i++)
    {
        if(huint352_bit(src2,i))
        {
            //,结果应当乘上当前state的值(将幂函数的指数按照2进制进行拆分成乘法表达式)
            huint352_mul(state1,state2,dst,state);
            huint352_copy(dst,state2);
        }
        //计算state的平方（state的平方相当于state的指数乘2，正好对应src2下一个二进制位）
        huint352_mul(state1,state2,state,state);
        huint352_copy(state,state2);
    }

}

void huint352_power_with_stack(huint352_t *dst,const huint352_t *src1,const huint352_t *src2)
{
    huint352_t state[3]= {0};
    huint352_power(&state[0],&state[1],&state[2],dst,src1,src2);
}

void huint352_power_with_external_state(huint352_state_t * state,huint352_t *dst,const huint352_t *src1,const huint352_t *src2)
{
    if(state==NULL)
    {
        return;
    }
    huint352_power(&state->state[0],&state->state[1],&state->state[2],dst,src1,src2);
}

void huint352_root(huint352_t *state,huint352_t *state1,huint352_t *state2,huint352_t *state3,huint352_t *state4,huint352_t *dst,const huint352_t *src,size_t index)
{
    if(state==NULL || state1==NULL || state2==NULL || state3==NULL || state4==NULL || dst==NULL || src==NULL || index == 0)
    {
        return;
    }

    if(index==1)
    {
        huint352_copy(dst,src);
        return;
    }

    huint352_load_uint32(dst,0);
    huint352_load_uint64(state4,index);

    size_t dst_max_bit=(HUINT352_BITS_COUNT-huint352_clz(src)+index-1)/index;

    if(dst_max_bit*index > HUINT352_BITS_COUNT)
    {
        dst_max_bit--;
    }

    for(size_t i=0; i<=dst_max_bit; i++)
    {
        huint352_bit_set(dst,dst_max_bit-i);
        huint352_power(state,state1,state2,state3,dst,state4);
        int compare_ret=huint352_compare(state3,src);
        if(compare_ret == 0)
        {
            break;
        }
        if(compare_ret > 0)
        {
            huint352_bit_clear(dst,dst_max_bit-i);
        }
    }

}

void huint352_root_with_stack(huint352_t *dst,const huint352_t *src,size_t index)
{
    huint352_t state[5];
    huint352_root(&state[0],&state[1],&state[2],&state[3],&state[4],dst,src,index);
}

void huint352_root_with_external_state(huint352_state_t * state,huint352_t *dst,const huint352_t *src,size_t index)
{
    if(state==NULL)
    {
        return;
    }
    huint352_root(&state->state[0],&state->state[1],&state->state[2],&state->state[3],&state->state[4],dst,src,index);
}

void huint352_power_mod(huint352_t *state,huint352_t *state1,huint352_t *state2,huint352_t *state3,huint352_t *dst,const huint352_t *src1,const huint352_t *src2,const huint352_t *src3)
{
    if(state == NULL || state1==NULL || state2== NULL || state3 == NULL || dst==NULL || src1==NULL || src2 == NULL)
    {
        return;
    }

    huint352_load_uint32(state,0);
    huint352_load_uint32(dst,1);
    if(huint352_compare(src1,state)==0 )
    {
        //底数为0
        huint352_load_uint32(dst,0);
        return;
    }
    if(huint352_compare(src2,state)==0 )
    {
        //任意数的0次方=1
        huint352_load_uint32(dst,1);
        return;
    }

    size_t clz2=huint352_clz(src2);
    huint352_copy(state,src1);
    for(size_t i=0; i<(HUINT352_BITS_COUNT - clz2); i++)
    {
        if(huint352_bit(src2,i))
        {
            //,结果应当乘上当前state的值(将幂函数的指数按照2进制进行拆分成乘法表达式)
            huint352_mul(state1,state2,dst,state);
            huint352_copy(dst,state2);

            //对dst提前取模，防止计算过程溢出(先取模再做乘法=先做乘法再取模)
            huint352_div(dst,state1,state2,state3,dst,src3);
        }
        //计算state的平方（state的平方相当于state的指数乘2，正好对应src2下一个二进制位）
        huint352_mul(state1,state2,state,state);
        huint352_copy(state,state2);

        //对state提前取模，防止计算过程溢出(先取模再做乘法=先做乘法再取模)
        huint352_div(state,state1,state2,state3,state,src3);
    }

}

void huint352_power_mod_with_stack(huint352_t *dst,const huint352_t *src1,const huint352_t *src2,const huint352_t *src3)
{
    huint352_t state[4]= {0};
    huint352_power_mod(&state[0],&state[1],&state[2],&state[3],dst,src1,src2,src3);
}

void huint352_power_mod_with_external_state(huint352_state_t * state,huint352_t *dst,const huint352_t *src1,const huint352_t *src2,const huint352_t *src3)
{
    if(state==NULL)
    {
        return;
    }
    huint352_power_mod(&state->state[0],&state->state[1],&state->state[2],&state->state[3],dst,src1,src2,src3);
}

void huint352_gcd(huint352_state_t * state,huint352_t *dst,const huint352_t *src1,const huint352_t *src2)
{
    if(state==NULL || dst==NULL || src1==NULL || src2==NULL)
    {
        return;
    }
    /*
     * 寄存器6,7分别存储除数与被除数
     */
    if(huint352_compare(src1,src2) >=0)
    {
        huint352_copy(&state->state[7],src1);
        huint352_copy(&state->state[6],src2);
    }
    else
    {
        huint352_copy(&state->state[6],src1);
        huint352_copy(&state->state[7],src2);
    }
    /*
     * 寄存器4,5分别存储商与余数
     */
    huint352_load_uint32(&state->state[5],1);//初始时余数不为0
    /*
     * 寄存器3为0寄存器
     */
    huint352_load_uint32(&state->state[3],0);
    if(huint352_compare(&state->state[3],&state->state[6])==0)
    {
        /*
         * 当其中一个数为0时,返回较大的数
         */
        huint352_copy(dst,&state->state[7]);
        return;
    }

    do
    {
        huint352_div(&state->state[5],&state->state[0],&state->state[1],&state->state[4],&state->state[7],&state->state[6]);
        /*
         * 重新设置被除数与除数
         */
        huint352_copy(&state->state[7],&state->state[6]);
        huint352_copy(&state->state[6],&state->state[5]);
    }
    while(huint352_compare(&state->state[3],&state->state[5])!=0);

    /*
     * 返回剩余的数
     */
    huint352_copy(dst,&state->state[7]);

}

size_t huint352_dec_number_count(huint352_state_t * state,const huint352_t *src)
{
    size_t dec_number_count=0;
    if(state==NULL || src==NULL)
    {
        return dec_number_count;
    }

    dec_number_count=0;

    bool is_ok=false;
    while(!is_ok)
    {
        /*
         * 寄存器6存储10，寄存器7存储10的指数
         */
        huint352_load_uint32(&state->state[6],10);
        huint352_load_uint32(&state->state[7],dec_number_count);
        /*
         * 寄存器5存储10的指数的结果
         */
        huint352_power_with_external_state(state,&state->state[5],&state->state[6],&state->state[7]);

        /*
         * 比较大小
         */
        switch(huint352_compare(&state->state[5],src))
        {
        case 1:
        {
            is_ok=true;
        }
        break;
        case 0:
        {
            dec_number_count++;
            is_ok=true;
        }
        break;
        case -1:
        {
            dec_number_count++;
        }
        break;
        case -2:
        {
            is_ok=true;
        }
        break;
        default:
        {

        }
        break;
        }
    }

    return dec_number_count;
}

size_t huint352_dec_number(huint352_state_t * state,const huint352_t *src,size_t index)
{
    size_t dec_number=0;
    if(state==NULL || src==NULL)
    {
        return dec_number;
    }

    /*
     * 寄存器6存储10，寄存器7存储10的指数
     */
    huint352_load_uint32(&state->state[6],10);
    huint352_load_uint32(&state->state[7],index+1);
    /*
     * 寄存器5存储10的指数的结果
     */
    huint352_power_with_external_state(state,&state->state[5],&state->state[6],&state->state[7]);
    /*
     * 第一次除法
     */
    huint352_copy(&state->state[7],&state->state[5]);
    huint352_div_with_external_state(state,&state->state[4],&state->state[5],src,&state->state[7]);

    /*
     * 寄存器6存储10，寄存器7存储10的指数
     */
    huint352_load_uint32(&state->state[7],index);
    huint352_power_with_external_state(state,&state->state[5],&state->state[6],&state->state[7]);

    /*
     * 第二次除法
     */
    huint352_copy(&state->state[6],&state->state[4]);
    huint352_copy(&state->state[7],&state->state[5]);
    huint352_div_with_external_state(state,&state->state[4],&state->state[5],&state->state[6],&state->state[7]);

    uint32_t result=0;
    huint352_store_uint32(&state->state[5],&result);
    dec_number=result;

    return dec_number;
}

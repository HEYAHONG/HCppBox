/***************************************************************
 * Name:      hcoff_pe.c
 * Purpose:   实现hcoff_pe接口(Portable Executable,PE/PE32+)
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hcoff_pe.h"
#include "hdefaults.h"
#include "hmemory.h"
#include "string.h"

#define HCOFF_PE_EXPORT_DIRECTORY_SIZE          40
#define HCOFF_PE_IMPORT_DESCRIPTOR_SIZE         20
#define HCOFF_PE_RELOCATION_BLOCK_HEADER_SIZE   8
#define HCOFF_PE_TLS_DIRECTORY_SIZE32           24
#define HCOFF_PE_TLS_DIRECTORY_SIZE64           40

static bool hcoff_pe_read_uint16(hcoff_file_input_t *input_file,uintptr_t offset,uint16_t *value)
{
    uint8_t buffer[2]= {0};
    if(value==NULL || 2 > hcoff_file_input_read(input_file,offset,buffer,sizeof(buffer)))
    {
        return false;
    }
    *value=hcoff_get_uint16_le(buffer);
    return true;
}

static bool hcoff_pe_read_uint32(hcoff_file_input_t *input_file,uintptr_t offset,uint32_t *value)
{
    uint8_t buffer[4]= {0};
    if(value==NULL || 4 > hcoff_file_input_read(input_file,offset,buffer,sizeof(buffer)))
    {
        return false;
    }
    *value=hcoff_get_uint32_le(buffer);
    return true;
}

bool hcoff_pe_is_pe(hcoff_file_input_t *input_file)
{
    if(input_file==NULL)
    {
        return false;
    }
    uint16_t dos_magic=0;
    if(!hcoff_pe_read_uint16(input_file,0,&dos_magic) || dos_magic != HCOFF_PE_DOS_SIGNATURE)
    {
        return false;
    }
    uint32_t nt_offset=0;
    if(!hcoff_pe_read_uint32(input_file,0x3C,&nt_offset) || nt_offset < 0x40)
    {
        return false;
    }
    uint32_t signature=0;
    if(!hcoff_pe_read_uint32(input_file,nt_offset,&signature) || signature != HCOFF_PE_NT_SIGNATURE)
    {
        return false;
    }
    return true;
}

bool hcoff_pe_header_read(hcoff_pe_header_t *header,hcoff_file_input_t *input_file)
{
    bool ret=false;
    if(header==NULL || input_file==NULL)
    {
        return ret;
    }
    memset(header,0,sizeof(hcoff_pe_header_t));
    uint16_t dos_magic=0;
    if(!hcoff_pe_read_uint16(input_file,0,&dos_magic) || dos_magic != HCOFF_PE_DOS_SIGNATURE)
    {
        return ret;
    }
    header->dos_header_offset=0;
    uint32_t nt_offset=0;
    if(!hcoff_pe_read_uint32(input_file,0x3C,&nt_offset) || nt_offset < 0x40)
    {
        return ret;
    }
    header->nt_headers_offset=nt_offset;
    uint32_t signature=0;
    if(!hcoff_pe_read_uint32(input_file,nt_offset,&signature) || signature != HCOFF_PE_NT_SIGNATURE)
    {
        return ret;
    }
    if(!hcoff_fileheader_read_at(&header->coff_header,input_file,nt_offset+4))
    {
        return ret;
    }
    header->optional_header_offset=nt_offset+4+20;
    header->optional_header_size=header->coff_header.f_opthdr;
    header->section_table_offset=header->optional_header_offset+header->optional_header_size;
    if(header->optional_header_size < 2)
    {
        return ret;
    }
    uint8_t optional_buffer[256]= {0};
    size_t optional_read_size=header->optional_header_size;
    if(optional_read_size > sizeof(optional_buffer))
    {
        optional_read_size=sizeof(optional_buffer);
    }
    if(optional_read_size > hcoff_file_input_read(input_file,header->optional_header_offset,optional_buffer,optional_read_size))
    {
        return ret;
    }
    header->optional_magic=hcoff_get_uint16_le(optional_buffer);
    if(header->optional_magic == HCOFF_PE_OPTIONAL_HEADER_MAGIC_PE32)
    {
        header->is_64bit=false;
        if(optional_read_size < 96)
        {
            return ret;
        }
        header->major_linker_version=optional_buffer[2];
        header->minor_linker_version=optional_buffer[3];
        header->size_of_code=hcoff_get_uint32_le(optional_buffer+4);
        header->size_of_initialized_data=hcoff_get_uint32_le(optional_buffer+8);
        header->size_of_uninitialized_data=hcoff_get_uint32_le(optional_buffer+12);
        header->address_of_entry_point=hcoff_get_uint32_le(optional_buffer+16);
        header->base_of_code=hcoff_get_uint32_le(optional_buffer+20);
        header->base_of_data=hcoff_get_uint32_le(optional_buffer+24);
        header->image_base=hcoff_get_uint32_le(optional_buffer+28);
        header->section_alignment=hcoff_get_uint32_le(optional_buffer+32);
        header->file_alignment=hcoff_get_uint32_le(optional_buffer+36);
        header->major_operating_system_version=hcoff_get_uint16_le(optional_buffer+40);
        header->minor_operating_system_version=hcoff_get_uint16_le(optional_buffer+42);
        header->major_image_version=hcoff_get_uint16_le(optional_buffer+44);
        header->minor_image_version=hcoff_get_uint16_le(optional_buffer+46);
        header->major_subsystem_version=hcoff_get_uint16_le(optional_buffer+48);
        header->minor_subsystem_version=hcoff_get_uint16_le(optional_buffer+50);
        header->win32_version_value=hcoff_get_uint32_le(optional_buffer+52);
        header->size_of_image=hcoff_get_uint32_le(optional_buffer+56);
        header->size_of_headers=hcoff_get_uint32_le(optional_buffer+60);
        header->checksum=hcoff_get_uint32_le(optional_buffer+64);
        header->subsystem=hcoff_get_uint16_le(optional_buffer+68);
        header->dll_characteristics=hcoff_get_uint16_le(optional_buffer+70);
        header->size_of_stack_reserve=hcoff_get_uint32_le(optional_buffer+72);
        header->size_of_stack_commit=hcoff_get_uint32_le(optional_buffer+76);
        header->size_of_heap_reserve=hcoff_get_uint32_le(optional_buffer+80);
        header->size_of_heap_commit=hcoff_get_uint32_le(optional_buffer+84);
        header->loader_flags=hcoff_get_uint32_le(optional_buffer+88);
        header->number_of_rva_and_sizes=hcoff_get_uint32_le(optional_buffer+92);
    }
    else if(header->optional_magic == HCOFF_PE_OPTIONAL_HEADER_MAGIC_PE32PLUS)
    {
        header->is_64bit=true;
        if(optional_read_size < 112)
        {
            return ret;
        }
        header->major_linker_version=optional_buffer[2];
        header->minor_linker_version=optional_buffer[3];
        header->size_of_code=hcoff_get_uint32_le(optional_buffer+4);
        header->size_of_initialized_data=hcoff_get_uint32_le(optional_buffer+8);
        header->size_of_uninitialized_data=hcoff_get_uint32_le(optional_buffer+12);
        header->address_of_entry_point=hcoff_get_uint32_le(optional_buffer+16);
        header->base_of_code=hcoff_get_uint32_le(optional_buffer+20);
        header->base_of_data=0;
        header->image_base=hcoff_get_uint64_le(optional_buffer+24);
        header->section_alignment=hcoff_get_uint32_le(optional_buffer+32);
        header->file_alignment=hcoff_get_uint32_le(optional_buffer+36);
        header->major_operating_system_version=hcoff_get_uint16_le(optional_buffer+40);
        header->minor_operating_system_version=hcoff_get_uint16_le(optional_buffer+42);
        header->major_image_version=hcoff_get_uint16_le(optional_buffer+44);
        header->minor_image_version=hcoff_get_uint16_le(optional_buffer+46);
        header->major_subsystem_version=hcoff_get_uint16_le(optional_buffer+48);
        header->minor_subsystem_version=hcoff_get_uint16_le(optional_buffer+50);
        header->win32_version_value=hcoff_get_uint32_le(optional_buffer+52);
        header->size_of_image=hcoff_get_uint32_le(optional_buffer+56);
        header->size_of_headers=hcoff_get_uint32_le(optional_buffer+60);
        header->checksum=hcoff_get_uint32_le(optional_buffer+64);
        header->subsystem=hcoff_get_uint16_le(optional_buffer+68);
        header->dll_characteristics=hcoff_get_uint16_le(optional_buffer+70);
        header->size_of_stack_reserve=hcoff_get_uint64_le(optional_buffer+72);
        header->size_of_stack_commit=hcoff_get_uint64_le(optional_buffer+80);
        header->size_of_heap_reserve=hcoff_get_uint64_le(optional_buffer+88);
        header->size_of_heap_commit=hcoff_get_uint64_le(optional_buffer+96);
        header->loader_flags=hcoff_get_uint32_le(optional_buffer+104);
        header->number_of_rva_and_sizes=hcoff_get_uint32_le(optional_buffer+108);
    }
    else
    {
        return ret;
    }

    size_t data_directory_offset=header->is_64bit?112:96;
    size_t data_directory_count=header->number_of_rva_and_sizes;
    if(data_directory_count > HCOFF_PE_NUMBEROF_DIRECTORY_ENTRIES)
    {
        data_directory_count=HCOFF_PE_NUMBEROF_DIRECTORY_ENTRIES;
    }
    if(data_directory_offset+data_directory_count*8 > optional_read_size)
    {
        //可选头长度不足,仅读取可读取的部分
        if(data_directory_offset >= optional_read_size)
        {
            data_directory_count=0;
        }
        else
        {
            data_directory_count=(optional_read_size-data_directory_offset)/8;
        }
    }
    for(size_t i=0; i<data_directory_count; i++)
    {
        header->data_directory[i].virtual_address=hcoff_get_uint32_le(optional_buffer+data_directory_offset+i*8);
        header->data_directory[i].size=hcoff_get_uint32_le(optional_buffer+data_directory_offset+i*8+4);
    }
    ret=true;
    return ret;
}

bool hcoff_pe_header_is_dll(const hcoff_pe_header_t *header)
{
    if(header==NULL)
    {
        return false;
    }
    return (header->coff_header.f_flags & HCOFF_FILEHEADER_F_FLAGS_IMAGE_FILE_DLL)!=0;
}

bool hcoff_pe_header_is_executable(const hcoff_pe_header_t *header)
{
    if(header==NULL)
    {
        return false;
    }
    return (header->coff_header.f_flags & HCOFF_FILEHEADER_F_FLAGS_IMAGE_FILE_EXECUTABLE_IMAGE)!=0;
}

bool hcoff_pe_sectionheader_read(const hcoff_pe_header_t *header,hcoff_sectionheader_t *sectionheader,size_t index,hcoff_file_input_t *input_file)
{
    if(header==NULL || sectionheader==NULL || input_file==NULL)
    {
        return false;
    }
    if(index >= header->coff_header.f_nscns)
    {
        return false;
    }
    return hcoff_sectionheader_read_at(sectionheader,index,header->section_table_offset,input_file);
}

bool hcoff_pe_rva_to_file_offset(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,uint32_t rva,uint32_t *file_offset,uint32_t *available_size)
{
    bool ret=false;
    if(header==NULL || input_file==NULL || file_offset==NULL)
    {
        return ret;
    }
    if(rva < header->size_of_headers)
    {
        *file_offset=rva;
        if(available_size!=NULL)
        {
            *available_size=header->size_of_headers-rva;
        }
        return true;
    }
    for(size_t i=0; i<header->coff_header.f_nscns; i++)
    {
        hcoff_sectionheader_t sectionheader= {0};
        if(!hcoff_pe_sectionheader_read(header,&sectionheader,i,input_file))
        {
            return false;
        }
        uint32_t virtual_size=sectionheader.s_paddr;
        if(virtual_size < sectionheader.s_size)
        {
            virtual_size=sectionheader.s_size;
        }
        if(rva < sectionheader.s_vaddr || rva >= sectionheader.s_vaddr+virtual_size)
        {
            continue;
        }
        uint32_t delta=rva-sectionheader.s_vaddr;
        if(delta >= sectionheader.s_size)
        {
            //位于未初始化数据区域,文件中不存在对应数据
            *file_offset=sectionheader.s_scnptr+delta;
            if(available_size!=NULL)
            {
                *available_size=0;
            }
            return true;
        }
        *file_offset=sectionheader.s_scnptr+delta;
        if(available_size!=NULL)
        {
            *available_size=sectionheader.s_size-delta;
        }
        ret=true;
        break;
    }
    return ret;
}

size_t hcoff_pe_read_at_rva(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,uint32_t rva,void *buffer,size_t buffer_length)
{
    if(header==NULL || input_file==NULL || buffer==NULL || buffer_length==0)
    {
        return 0;
    }
    uint32_t file_offset=0;
    uint32_t available_size=0;
    if(!hcoff_pe_rva_to_file_offset(header,input_file,rva,&file_offset,&available_size))
    {
        return 0;
    }
    if(buffer_length > available_size)
    {
        buffer_length=available_size;
    }
    if(buffer_length==0)
    {
        return 0;
    }
    size_t read_size=hcoff_file_input_read(input_file,file_offset,buffer,buffer_length);
    return read_size;
}

const char *hcoff_pe_read_string_at_rva(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,uint32_t rva,char *buffer,size_t buffer_length)
{
    if(buffer==NULL || buffer_length < 2)
    {
        return NULL;
    }
    size_t i=0;
    for(; i<buffer_length-1; i++)
    {
        if(1 > hcoff_pe_read_at_rva(header,input_file,rva+i,&buffer[i],1))
        {
            return NULL;
        }
        if(buffer[i]=='\0')
        {
            break;
        }
    }
    buffer[i]='\0';
    return buffer;
}

static bool hcoff_pe_export_directory_read(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,uint8_t *buffer)
{
    if(header==NULL || input_file==NULL || buffer==NULL)
    {
        return false;
    }
    if(header->data_directory[HCOFF_PE_DIRECTORY_ENTRY_EXPORT].virtual_address==0 ||
            header->data_directory[HCOFF_PE_DIRECTORY_ENTRY_EXPORT].size==0)
    {
        return false;
    }
    return HCOFF_PE_EXPORT_DIRECTORY_SIZE <= hcoff_pe_read_at_rva(header,input_file,header->data_directory[HCOFF_PE_DIRECTORY_ENTRY_EXPORT].virtual_address,buffer,HCOFF_PE_EXPORT_DIRECTORY_SIZE);
}

size_t hcoff_pe_export_count(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file)
{
    size_t ret=0;
    uint8_t buffer[HCOFF_PE_EXPORT_DIRECTORY_SIZE]= {0};
    if(hcoff_pe_export_directory_read(header,input_file,buffer))
    {
        ret=hcoff_get_uint32_le(buffer+20);
    }
    return ret;
}

size_t hcoff_pe_export_name_count(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file)
{
    size_t ret=0;
    uint8_t buffer[HCOFF_PE_EXPORT_DIRECTORY_SIZE]= {0};
    if(hcoff_pe_export_directory_read(header,input_file,buffer))
    {
        ret=hcoff_get_uint32_le(buffer+24);
    }
    return ret;
}

bool hcoff_pe_export_name_get(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,size_t index,hcoff_pe_export_t *export_info,char *name_buffer,size_t name_buffer_length,char *forwarder_buffer,size_t forwarder_buffer_length)
{
    bool ret=false;
    if(export_info==NULL)
    {
        return ret;
    }
    memset(export_info,0,sizeof(hcoff_pe_export_t));
    uint8_t export_buffer[HCOFF_PE_EXPORT_DIRECTORY_SIZE]= {0};
    if(!hcoff_pe_export_directory_read(header,input_file,export_buffer))
    {
        return ret;
    }
    uint32_t name_count=hcoff_get_uint32_le(export_buffer+24);
    uint32_t function_count=hcoff_get_uint32_le(export_buffer+20);
    uint32_t ordinal_base=hcoff_get_uint32_le(export_buffer+16);
    uint32_t address_of_functions=hcoff_get_uint32_le(export_buffer+28);
    uint32_t address_of_names=hcoff_get_uint32_le(export_buffer+32);
    uint32_t address_of_name_ordinals=hcoff_get_uint32_le(export_buffer+36);
    if(index >= name_count)
    {
        return ret;
    }
    {
        uint32_t name_table_offset=0;
        uint32_t available=0;
        if(!hcoff_pe_rva_to_file_offset(header,input_file,address_of_names,&name_table_offset,&available))
        {
            return ret;
        }
        uint32_t name_rva=0;
        uint8_t name_ptr_buffer[4]= {0};
        if(4 > hcoff_file_input_read(input_file,name_table_offset+index*4,name_ptr_buffer,sizeof(name_ptr_buffer)))
        {
            return ret;
        }
        name_rva=hcoff_get_uint32_le(name_ptr_buffer);

        uint32_t ord_table_offset=0;
        if(!hcoff_pe_rva_to_file_offset(header,input_file,address_of_name_ordinals,&ord_table_offset,&available))
        {
            return ret;
        }
        uint8_t ord_buffer[2]= {0};
        if(2 > hcoff_file_input_read(input_file,ord_table_offset+index*2,ord_buffer,sizeof(ord_buffer)))
        {
            return ret;
        }
        uint16_t function_index=hcoff_get_uint16_le(ord_buffer);
        if(function_index >= function_count)
        {
            return ret;
        }
        uint32_t function_table_offset=0;
        if(!hcoff_pe_rva_to_file_offset(header,input_file,address_of_functions,&function_table_offset,&available))
        {
            return ret;
        }
        uint8_t function_buffer[4]= {0};
        if(4 > hcoff_file_input_read(input_file,function_table_offset+(uint32_t)function_index*4,function_buffer,sizeof(function_buffer)))
        {
            return ret;
        }
        uint32_t function_rva=hcoff_get_uint32_le(function_buffer);

        export_info->name_rva=name_rva;
        export_info->function_index=function_index;
        export_info->function_rva=function_rva;
        export_info->ordinal=ordinal_base+function_index;
        if(name_buffer!=NULL && name_buffer_length > 0)
        {
            export_info->name=hcoff_pe_read_string_at_rva(header,input_file,name_rva,name_buffer,name_buffer_length);
        }
        uint32_t export_directory_rva=header->data_directory[HCOFF_PE_DIRECTORY_ENTRY_EXPORT].virtual_address;
        uint32_t export_directory_size=header->data_directory[HCOFF_PE_DIRECTORY_ENTRY_EXPORT].size;
        if(function_rva >= export_directory_rva && function_rva < export_directory_rva+export_directory_size)
        {
            export_info->is_forwarder=true;
            export_info->forwarder_rva=function_rva;
            if(forwarder_buffer!=NULL && forwarder_buffer_length > 0)
            {
                export_info->forwarder=hcoff_pe_read_string_at_rva(header,input_file,function_rva,forwarder_buffer,forwarder_buffer_length);
            }
        }
        ret=true;
    }
    return ret;
}

bool hcoff_pe_export_find(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,const char *name,hcoff_pe_export_t *export_info,char *name_buffer,size_t name_buffer_length,char *forwarder_buffer,size_t forwarder_buffer_length)
{
    bool ret=false;
    if(name==NULL || export_info==NULL)
    {
        return ret;
    }
    size_t count=hcoff_pe_export_name_count(header,input_file);
    for(size_t i=0; i<count; i++)
    {
        hcoff_pe_export_t temp_export= {0};
        if(!hcoff_pe_export_name_get(header,input_file,i,&temp_export,name_buffer,name_buffer_length,forwarder_buffer,forwarder_buffer_length))
        {
            continue;
        }
        if(temp_export.name!=NULL && strcmp(temp_export.name,name)==0)
        {
            *export_info=temp_export;
            ret=true;
            break;
        }
    }
    return ret;
}

const char *hcoff_pe_export_forwarder_name_get(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,const hcoff_pe_export_t *export_info,char *buffer,size_t buffer_length)
{
    if(export_info==NULL || !export_info->is_forwarder)
    {
        return NULL;
    }
    return hcoff_pe_read_string_at_rva(header,input_file,export_info->forwarder_rva,buffer,buffer_length);
}

const char *hcoff_pe_export_name_ptr_get(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,const hcoff_pe_export_t *export_info,char *buffer,size_t buffer_length)
{
    if(export_info==NULL)
    {
        return NULL;
    }
    return hcoff_pe_read_string_at_rva(header,input_file,export_info->name_rva,buffer,buffer_length);
}

static bool hcoff_pe_import_descriptor_read(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,size_t index,uint8_t *buffer)
{
    if(header==NULL || input_file==NULL || buffer==NULL)
    {
        return false;
    }
    if(header->data_directory[HCOFF_PE_DIRECTORY_ENTRY_IMPORT].virtual_address==0)
    {
        return false;
    }
    uint32_t descriptor_rva=header->data_directory[HCOFF_PE_DIRECTORY_ENTRY_IMPORT].virtual_address+(uint32_t)index*HCOFF_PE_IMPORT_DESCRIPTOR_SIZE;
    return HCOFF_PE_IMPORT_DESCRIPTOR_SIZE <= hcoff_pe_read_at_rva(header,input_file,descriptor_rva,buffer,HCOFF_PE_IMPORT_DESCRIPTOR_SIZE);
}

size_t hcoff_pe_import_dll_count(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file)
{
    size_t ret=0;
    for(size_t i=0; ; i++)
    {
        uint8_t buffer[HCOFF_PE_IMPORT_DESCRIPTOR_SIZE]= {0};
        if(!hcoff_pe_import_descriptor_read(header,input_file,i,buffer))
        {
            break;
        }
        uint32_t original_first_thunk=hcoff_get_uint32_le(buffer);
        uint32_t name_rva=hcoff_get_uint32_le(buffer+12);
        uint32_t first_thunk=hcoff_get_uint32_le(buffer+16);
        if(name_rva==0 && original_first_thunk==0 && first_thunk==0)
        {
            break;
        }
        if(name_rva==0)
        {
            break;
        }
        ret++;
        if(ret > 0x100000)
        {
            break;
        }
    }
    return ret;
}

bool hcoff_pe_is_static_linked(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file)
{
    if(header==NULL || input_file==NULL)
    {
        return false;
    }
    if(header->data_directory[HCOFF_PE_DIRECTORY_ENTRY_DELAY_IMPORT].virtual_address!=0 &&
            header->data_directory[HCOFF_PE_DIRECTORY_ENTRY_DELAY_IMPORT].size!=0)
    {
        return false;
    }
    return hcoff_pe_import_dll_count(header,input_file)==0;
}

const char *hcoff_pe_import_dll_name_get(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,size_t dll_index,char *buffer,size_t buffer_length)
{
    uint8_t descriptor[HCOFF_PE_IMPORT_DESCRIPTOR_SIZE]= {0};
    if(!hcoff_pe_import_descriptor_read(header,input_file,dll_index,descriptor))
    {
        return NULL;
    }
    uint32_t name_rva=hcoff_get_uint32_le(descriptor+12);
    return hcoff_pe_read_string_at_rva(header,input_file,name_rva,buffer,buffer_length);
}

size_t hcoff_pe_import_count(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,size_t dll_index)
{
    size_t ret=0;
    uint8_t descriptor[HCOFF_PE_IMPORT_DESCRIPTOR_SIZE]= {0};
    if(!hcoff_pe_import_descriptor_read(header,input_file,dll_index,descriptor))
    {
        return ret;
    }
    uint32_t lookup_rva=hcoff_get_uint32_le(descriptor);
    if(lookup_rva==0)
    {
        lookup_rva=hcoff_get_uint32_le(descriptor+16);
    }
    size_t thunk_size=header->is_64bit?8:4;
    for(size_t i=0; ; i++)
    {
        uint8_t thunk[8]= {0};
        if(thunk_size > hcoff_pe_read_at_rva(header,input_file,lookup_rva+(uint32_t)(i*thunk_size),thunk,thunk_size))
        {
            break;
        }
        uint64_t value=header->is_64bit?hcoff_get_uint64_le(thunk):hcoff_get_uint32_le(thunk);
        if(value==0)
        {
            break;
        }
        ret++;
        if(ret > 0x100000)
        {
            break;
        }
    }
    return ret;
}

bool hcoff_pe_import_get(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,size_t dll_index,size_t thunk_index,hcoff_pe_import_t *import_info,char *dll_name_buffer,size_t dll_name_buffer_length,char *function_name_buffer,size_t function_name_buffer_length)
{
    bool ret=false;
    if(import_info==NULL)
    {
        return ret;
    }
    memset(import_info,0,sizeof(hcoff_pe_import_t));
    uint8_t descriptor[HCOFF_PE_IMPORT_DESCRIPTOR_SIZE]= {0};
    if(!hcoff_pe_import_descriptor_read(header,input_file,dll_index,descriptor))
    {
        return ret;
    }
    uint32_t original_first_thunk=hcoff_get_uint32_le(descriptor);
    uint32_t name_rva=hcoff_get_uint32_le(descriptor+12);
    uint32_t first_thunk=hcoff_get_uint32_le(descriptor+16);
    uint32_t lookup_rva=original_first_thunk;
    if(lookup_rva==0)
    {
        lookup_rva=first_thunk;
    }
    size_t thunk_size=header->is_64bit?8:4;
    uint8_t thunk[8]= {0};
    if(thunk_size > hcoff_pe_read_at_rva(header,input_file,lookup_rva+(uint32_t)(thunk_index*thunk_size),thunk,thunk_size))
    {
        return ret;
    }
    uint64_t value=header->is_64bit?hcoff_get_uint64_le(thunk):hcoff_get_uint32_le(thunk);
    if(value==0)
    {
        return ret;
    }
    import_info->dll_index=(uint32_t)dll_index;
    import_info->thunk_index=(uint32_t)thunk_index;
    import_info->lookup_rva=lookup_rva+(uint32_t)(thunk_index*thunk_size);
    import_info->iat_rva=first_thunk+(uint32_t)(thunk_index*thunk_size);
    if(dll_name_buffer!=NULL && dll_name_buffer_length > 0)
    {
        import_info->dll_name=hcoff_pe_read_string_at_rva(header,input_file,name_rva,dll_name_buffer,dll_name_buffer_length);
    }
    uint64_t ordinal_flag=header->is_64bit?(1ULL << 63):(1ULL << 31);
    if((value & ordinal_flag)!=0)
    {
        import_info->by_ordinal=true;
        import_info->ordinal=(uint16_t)(value & 0xFFFF);
    }
    else
    {
        uint32_t hint_name_rva=(uint32_t)(value & 0x7FFFFFFF);
        uint8_t hint_buffer[2]= {0};
        if(2 <= hcoff_pe_read_at_rva(header,input_file,hint_name_rva,hint_buffer,sizeof(hint_buffer)))
        {
            import_info->hint=hcoff_get_uint16_le(hint_buffer);
        }
        if(function_name_buffer!=NULL && function_name_buffer_length > 0)
        {
            import_info->function_name=hcoff_pe_read_string_at_rva(header,input_file,hint_name_rva+2,function_name_buffer,function_name_buffer_length);
        }
    }
    ret=true;
    return ret;
}

bool hcoff_pe_import_find(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,const char *name,hcoff_pe_import_t *import_info,char *dll_name_buffer,size_t dll_name_buffer_length,char *function_name_buffer,size_t function_name_buffer_length)
{
    bool ret=false;
    if(name==NULL || import_info==NULL)
    {
        return ret;
    }
    size_t dll_count=hcoff_pe_import_dll_count(header,input_file);
    for(size_t i=0; i<dll_count; i++)
    {
        size_t count=hcoff_pe_import_count(header,input_file,i);
        for(size_t j=0; j<count; j++)
        {
            hcoff_pe_import_t temp_import= {0};
            if(!hcoff_pe_import_get(header,input_file,i,j,&temp_import,dll_name_buffer,dll_name_buffer_length,function_name_buffer,function_name_buffer_length))
            {
                continue;
            }
            if(temp_import.function_name!=NULL && strcmp(temp_import.function_name,name)==0)
            {
                *import_info=temp_import;
                ret=true;
                break;
            }
        }
        if(ret)
        {
            break;
        }
    }
    return ret;
}

typedef struct
{
    const hcoff_pe_header_t *header;
    hcoff_file_input_t *input_file;
    hcoff_pe_base_relocation_callback_t callback;
    void *usr;
    size_t count;
} hcoff_pe_base_relocation_context_t;

static bool hcoff_pe_base_relocation_block_foreach(hcoff_pe_base_relocation_context_t *context,uint32_t block_rva,uint32_t block_directory_rva,uint32_t block_size)
{
    if(block_size < HCOFF_PE_RELOCATION_BLOCK_HEADER_SIZE || block_size > 0x100000)
    {
        return false;
    }
    size_t entry_count=(block_size-HCOFF_PE_RELOCATION_BLOCK_HEADER_SIZE)/2;
    for(size_t i=0; i<entry_count; i++)
    {
        uint8_t entry_buffer[2]= {0};
        if(2 > hcoff_pe_read_at_rva(context->header,context->input_file,block_directory_rva+HCOFF_PE_RELOCATION_BLOCK_HEADER_SIZE+(uint32_t)(i*2),entry_buffer,sizeof(entry_buffer)))
        {
            return false;
        }
        uint16_t entry=hcoff_get_uint16_le(entry_buffer);
        uint16_t type=entry >> 12;
        uint16_t offset=entry & 0x0FFF;
        uint16_t data=entry;
        uint32_t rva=block_rva+offset;
        if(type == HCOFF_PE_REL_BASED_HIGHADJ)
        {
            //HIGHADJ的下一个条目存储低16位,应当被跳过
            if(i+1 >= entry_count)
            {
                return false;
            }
            uint8_t next_entry_buffer[2]= {0};
            if(2 > hcoff_pe_read_at_rva(context->header,context->input_file,block_directory_rva+HCOFF_PE_RELOCATION_BLOCK_HEADER_SIZE+(uint32_t)((i+1)*2),next_entry_buffer,sizeof(next_entry_buffer)))
            {
                return false;
            }
            data=hcoff_get_uint16_le(next_entry_buffer);
            i++;
            context->count++;
        }
        context->count++;
        if(context->callback!=NULL)
        {
            if(!context->callback(rva,type,data,context->usr))
            {
                return true;
            }
        }
    }
    return true;
}

size_t hcoff_pe_base_relocation_foreach(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,hcoff_pe_base_relocation_callback_t callback,void *usr)
{
    hcoff_pe_base_relocation_context_t context= {0};
    context.header=header;
    context.input_file=input_file;
    context.callback=callback;
    context.usr=usr;
    if(header==NULL || input_file==NULL)
    {
        return 0;
    }
    uint32_t directory_rva=header->data_directory[HCOFF_PE_DIRECTORY_ENTRY_BASERELOC].virtual_address;
    uint32_t directory_size=header->data_directory[HCOFF_PE_DIRECTORY_ENTRY_BASERELOC].size;
    if(directory_rva==0 || directory_size==0)
    {
        return 0;
    }
    uint32_t offset=0;
    while(offset+HCOFF_PE_RELOCATION_BLOCK_HEADER_SIZE <= directory_size)
    {
        uint8_t block_header[HCOFF_PE_RELOCATION_BLOCK_HEADER_SIZE]= {0};
        if(HCOFF_PE_RELOCATION_BLOCK_HEADER_SIZE > hcoff_pe_read_at_rva(header,input_file,directory_rva+offset,block_header,sizeof(block_header)))
        {
            break;
        }
        uint32_t block_rva=hcoff_get_uint32_le(block_header);
        uint32_t block_size=hcoff_get_uint32_le(block_header+4);
        if(block_size < HCOFF_PE_RELOCATION_BLOCK_HEADER_SIZE)
        {
            break;
        }
        if(!hcoff_pe_base_relocation_block_foreach(&context,block_rva,directory_rva+offset,block_size))
        {
            break;
        }
        offset+=block_size;
    }
    return context.count;
}

static void hcoff_pe_apply_arm_mov32(uint8_t *ptr,uint32_t value)
{
    //MOVW/MOVT指令对
    uint32_t movw=hcoff_get_uint32_le(ptr);
    uint32_t movt=hcoff_get_uint32_le(ptr+4);
    movw=(movw & 0xFFF0F000U) | ((value & 0xFFFF) & 0x0FFF) | (((value >> 12) & 0xFFFF) << 16);
    movt=(movt & 0xFFF0F000U) | (((value >> 16) & 0xFFFF) & 0x0FFF) | (((value >> 28) & 0xFFFF) << 16);
    hcoff_put_uint32_le(ptr,movw);
    hcoff_put_uint32_le(ptr+4,movt);
}

static void hcoff_pe_apply_thumb_mov32(uint8_t *ptr,uint32_t value)
{
    //Thumb-2 MOVW/MOVT指令对
    uint32_t movw=hcoff_get_uint32_le(ptr);
    uint32_t movt=hcoff_get_uint32_le(ptr+4);
    movw=(movw & 0x8F00FBF0U) | (((value >> 0) & 0xF) << 0) | (((value >> 4) & 0x7) << 12) | (((value >> 11) & 0x1) << 26);
    movt=(movt & 0x8F00FBF0U) | (((value >> 16) & 0xF) << 0) | (((value >> 20) & 0x7) << 12) | (((value >> 27) & 0x1) << 26);
    hcoff_put_uint32_le(ptr,movw);
    hcoff_put_uint32_le(ptr+4,movt);
}

static bool hcoff_pe_apply_riscv_high20(uint8_t *ptr,uint32_t value)
{
    uint32_t inst=hcoff_get_uint32_le(ptr);
    int32_t hi=(int32_t)((value+0x800)>>12);
    inst=(inst & 0xFFF) | (((uint32_t)hi & 0xFFFFF) << 12);
    hcoff_put_uint32_le(ptr,inst);
    return true;
}

static bool hcoff_pe_apply_riscv_low12i(uint8_t *ptr,uint32_t value)
{
    uint32_t inst=hcoff_get_uint32_le(ptr);
    inst=(inst & 0xFFFFF) | ((value & 0xFFF) << 20);
    hcoff_put_uint32_le(ptr,inst);
    return true;
}

static bool hcoff_pe_apply_riscv_low12s(uint8_t *ptr,uint32_t value)
{
    uint32_t inst=hcoff_get_uint32_le(ptr);
    uint32_t imm=value & 0xFFF;
    inst=(inst & 0x01FFF07F) | (((imm >> 0) & 0x1F) << 7) | (((imm >> 5) & 0x7F) << 25);
    hcoff_put_uint32_le(ptr,inst);
    return true;
}

static bool hcoff_pe_apply_mips_jmpaddr(uint8_t *ptr,uint32_t value,int64_t delta)
{
    uint32_t inst=hcoff_get_uint32_le(ptr);
    uint32_t target=(uint32_t)((value + (uint32_t)delta) >> 2);
    inst=(inst & 0xFC000000U) | (target & 0x03FFFFFFU);
    hcoff_put_uint32_le(ptr,inst);
    return true;
}

typedef struct
{
    uint8_t *image;
    size_t image_size;
    int64_t delta;
    const hcoff_pe_header_t *header;
    hcoff_file_input_t *input_file;
    uint32_t directory_rva;
    uint32_t directory_size;
    bool success;
} hcoff_pe_base_relocation_apply_context_t;

static bool hcoff_pe_base_relocation_apply_callback(uint32_t rva,uint16_t type,uint16_t data,void *usr)
{
    hcoff_pe_base_relocation_apply_context_t *context=(hcoff_pe_base_relocation_apply_context_t *)usr;
    if(context==NULL || rva >= context->image_size)
    {
        return false;
    }
    uint8_t *ptr=context->image+rva;
    size_t ptr_size=context->image_size-rva;
    switch(type)
    {
    case HCOFF_PE_REL_BASED_ABSOLUTE:
    {
        //填充条目,无需处理
    }
    break;
    case HCOFF_PE_REL_BASED_HIGH:
    {
        if(ptr_size < 2)
        {
            return false;
        }
        uint16_t value=hcoff_get_uint16_le(ptr);
        value=(uint16_t)(value+((context->delta >> 16) & 0xFFFF));
        hcoff_put_uint16_le(ptr,value);
    }
    break;
    case HCOFF_PE_REL_BASED_LOW:
    {
        if(ptr_size < 2)
        {
            return false;
        }
        uint16_t value=hcoff_get_uint16_le(ptr);
        value=(uint16_t)(value+(context->delta & 0xFFFF));
        hcoff_put_uint16_le(ptr,value);
    }
    break;
    case HCOFF_PE_REL_BASED_HIGHLOW:
    {
        if(ptr_size < 4)
        {
            return false;
        }
        uint32_t value=hcoff_get_uint32_le(ptr);
        value=(uint32_t)(value+(uint32_t)context->delta);
        hcoff_put_uint32_le(ptr,value);
    }
    break;
    case HCOFF_PE_REL_BASED_HIGHADJ:
    {
        //HIGHADJ的下一个条目存储低16位
        if(ptr_size < 2)
        {
            return false;
        }
        uint16_t high=hcoff_get_uint16_le(ptr);
        int32_t low=(int16_t)data;
        int32_t value=(int32_t)(((uint32_t)high << 16) + (uint32_t)low + (uint32_t)context->delta);
        value+=0x8000;
        hcoff_put_uint16_le(ptr,(uint16_t)((uint32_t)value >> 16));
    }
    break;
    case HCOFF_PE_REL_BASED_DIR64:
    {
        if(ptr_size < 8)
        {
            return false;
        }
        uint64_t value=hcoff_get_uint64_le(ptr);
        value=(uint64_t)(value+(uint64_t)context->delta);
        hcoff_put_uint64_le(ptr,value);
    }
    break;
    case HCOFF_PE_REL_BASED_ARM_MOV32:
    {
        uint16_t machine=context->header->coff_header.f_magic;
        if(machine==HCOFF_FILEHEADER_F_MAGIC_ARM || machine==HCOFF_FILEHEADER_F_MAGIC_ARMNT || machine==HCOFF_FILEHEADER_F_MAGIC_THUMB)
        {
            if(ptr_size < 8)
            {
                return false;
            }
            uint32_t value=hcoff_get_uint32_le(ptr);
            hcoff_pe_apply_arm_mov32(ptr,(uint32_t)(value+(uint32_t)context->delta));
        }
        else if(machine==HCOFF_FILEHEADER_F_MAGIC_RISCV32 || machine==HCOFF_FILEHEADER_F_MAGIC_RISCV64)
        {
            if(ptr_size < 4)
            {
                return false;
            }
            uint32_t value=hcoff_get_uint32_le(ptr) + (uint32_t)context->delta;
            hcoff_pe_apply_riscv_high20(ptr,value);
        }
        else if(machine==HCOFF_FILEHEADER_F_MAGIC_R3000 || machine==HCOFF_FILEHEADER_F_MAGIC_R4000 ||
                machine==HCOFF_FILEHEADER_F_MAGIC_R10000 || machine==HCOFF_FILEHEADER_F_MAGIC_WCEMIPSV2 ||
                machine==HCOFF_FILEHEADER_F_MAGIC_MIPS16 || machine==HCOFF_FILEHEADER_F_MAGIC_MIPSFPU ||
                machine==HCOFF_FILEHEADER_F_MAGIC_MIPSFPU16)
        {
            if(ptr_size < 4)
            {
                return false;
            }
            uint32_t value=hcoff_get_uint32_le(ptr);
            hcoff_pe_apply_mips_jmpaddr(ptr,value,context->delta);
        }
        else
        {
            return false;
        }
    }
    break;
    case HCOFF_PE_REL_BASED_THUMB_MOV32:
    {
        uint16_t machine=context->header->coff_header.f_magic;
        if(machine==HCOFF_FILEHEADER_F_MAGIC_ARM || machine==HCOFF_FILEHEADER_F_MAGIC_ARMNT || machine==HCOFF_FILEHEADER_F_MAGIC_THUMB)
        {
            if(ptr_size < 8)
            {
                return false;
            }
            uint32_t value=hcoff_get_uint32_le(ptr);
            hcoff_pe_apply_thumb_mov32(ptr,(uint32_t)(value+(uint32_t)context->delta));
        }
        else if(machine==HCOFF_FILEHEADER_F_MAGIC_RISCV32 || machine==HCOFF_FILEHEADER_F_MAGIC_RISCV64)
        {
            if(ptr_size < 4)
            {
                return false;
            }
            uint32_t value=hcoff_get_uint32_le(ptr);
            hcoff_pe_apply_riscv_low12i(ptr,value);
        }
        else
        {
            return false;
        }
    }
    break;
    case HCOFF_PE_REL_BASED_RISCV_LOW12S:
    {
        uint16_t machine=context->header->coff_header.f_magic;
        if(machine==HCOFF_FILEHEADER_F_MAGIC_RISCV32 || machine==HCOFF_FILEHEADER_F_MAGIC_RISCV64)
        {
            if(ptr_size < 4)
            {
                return false;
            }
            uint32_t value=hcoff_get_uint32_le(ptr);
            hcoff_pe_apply_riscv_low12s(ptr,value);
        }
        else if(machine==HCOFF_FILEHEADER_F_MAGIC_LOONGARCH64)
        {
            if(ptr_size < 8)
            {
                return false;
            }
            uint32_t insn1=hcoff_get_uint32_le(ptr);
            uint32_t insn2=hcoff_get_uint32_le(ptr+4);
            uint32_t value=(insn1 << 12) | ((insn2 >> 10) & 0xFFF);
            value=(uint32_t)(value+(uint32_t)context->delta);
            uint32_t hi=(value+0x800)>>12;
            uint32_t lo=value & 0xFFF;
            insn1=(insn1 & 0x9F00001F) | ((hi & 0xFFFFF) << 5);
            insn2=(insn2 & 0xFFC003FF) | ((lo & 0xFFF) << 10);
            hcoff_put_uint32_le(ptr,insn1);
            hcoff_put_uint32_le(ptr+4,insn2);
        }
        else
        {
            return false;
        }
    }
    break;
    case HCOFF_PE_REL_BASED_MIPS_JMPADDR16:
    {
        //MIPS16跳转重定位,暂不支持
        return false;
    }
    default:
    {
        //未知重定位类型
        return false;
    }
    }
    return true;
}

bool hcoff_pe_base_relocation_apply(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,uint8_t *image,size_t image_size,int64_t delta)
{
    if(header==NULL || input_file==NULL || image==NULL || image_size==0)
    {
        return false;
    }
    if(delta==0)
    {
        return true;
    }
    hcoff_pe_base_relocation_apply_context_t context= {0};
    context.image=image;
    context.image_size=image_size;
    context.delta=delta;
    context.header=header;
    context.input_file=input_file;
    context.success=true;
    hcoff_pe_base_relocation_foreach(header,input_file,hcoff_pe_base_relocation_apply_callback,&context);
    return context.success;
}

bool hcoff_pe_tls_directory_read(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,hcoff_pe_tls_directory_t *tls)
{
    if(header==NULL || input_file==NULL || tls==NULL)
    {
        return false;
    }
    memset(tls,0,sizeof(hcoff_pe_tls_directory_t));
    uint32_t directory_rva=header->data_directory[HCOFF_PE_DIRECTORY_ENTRY_TLS].virtual_address;
    uint32_t directory_size=header->data_directory[HCOFF_PE_DIRECTORY_ENTRY_TLS].size;
    if(directory_rva==0 || directory_size==0)
    {
        return false;
    }
    uint8_t buffer[HCOFF_PE_TLS_DIRECTORY_SIZE64]= {0};
    size_t read_size=header->is_64bit?HCOFF_PE_TLS_DIRECTORY_SIZE64:HCOFF_PE_TLS_DIRECTORY_SIZE32;
    if(read_size > hcoff_pe_read_at_rva(header,input_file,directory_rva,buffer,sizeof(buffer)))
    {
        return false;
    }
    if(header->is_64bit)
    {
        tls->start_address_of_raw_data=hcoff_get_uint64_le(buffer);
        tls->end_address_of_raw_data=hcoff_get_uint64_le(buffer+8);
        tls->address_of_index=hcoff_get_uint64_le(buffer+16);
        tls->address_of_callbacks=hcoff_get_uint64_le(buffer+24);
        tls->size_of_zero_fill=hcoff_get_uint32_le(buffer+32);
        tls->characteristics=hcoff_get_uint32_le(buffer+36);
    }
    else
    {
        tls->start_address_of_raw_data=hcoff_get_uint32_le(buffer);
        tls->end_address_of_raw_data=hcoff_get_uint32_le(buffer+4);
        tls->address_of_index=hcoff_get_uint32_le(buffer+8);
        tls->address_of_callbacks=hcoff_get_uint32_le(buffer+12);
        tls->size_of_zero_fill=hcoff_get_uint32_le(buffer+16);
        tls->characteristics=hcoff_get_uint32_le(buffer+20);
    }
    return true;
}

static size_t hcoff_pe_section_image_size(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file)
{
    uint32_t ret=header->size_of_image;
    uint32_t end=header->size_of_headers;
    for(size_t i=0; i<header->coff_header.f_nscns; i++)
    {
        hcoff_sectionheader_t sectionheader= {0};
        if(!hcoff_pe_sectionheader_read(header,&sectionheader,i,input_file))
        {
            continue;
        }
        uint32_t section_size=sectionheader.s_paddr;
        if(section_size < sectionheader.s_size)
        {
            section_size=sectionheader.s_size;
        }
        uint32_t section_end=sectionheader.s_vaddr+section_size;
        if(section_end > end)
        {
            end=section_end;
        }
    }
    if(end > ret)
    {
        ret=end;
    }
    return ret;
}

size_t hcoff_pe_image_buffer_size_get(hcoff_file_input_t *input_file)
{
    if(input_file==NULL)
    {
        return 0;
    }
    hcoff_pe_header_t header= {0};
    if(!hcoff_pe_header_read(&header,input_file))
    {
        return 0;
    }
    size_t image_size=hcoff_pe_section_image_size(&header,input_file);
    if(image_size < header.size_of_headers || image_size==0)
    {
        return 0;
    }
    return image_size;
}

bool hcoff_pe_image_init(hcoff_pe_image_t *image,hcoff_file_input_t *input_file,void *buffer,size_t buffer_size)
{
    if(image==NULL || input_file==NULL || buffer==NULL)
    {
        return false;
    }
    memset(image,0,sizeof(hcoff_pe_image_t));
    if(!hcoff_pe_header_read(&image->header,input_file))
    {
        return false;
    }
    image->image_size=hcoff_pe_section_image_size(&image->header,input_file);
    if(image->image_size < image->header.size_of_headers || image->image_size==0 || image->image_size > buffer_size)
    {
        hcoff_pe_image_deinit(image);
        return false;
    }
    image->image=(uint8_t *)buffer;
    image->buffer_size=buffer_size;
    image->preferred_base=image->header.image_base;
    image->loaded_base=(uint64_t)(uintptr_t)image->image;
    {
        size_t header_size=image->header.size_of_headers;
        if(header_size > image->image_size)
        {
            header_size=image->image_size;
        }
        if(header_size > hcoff_file_input_read(input_file,0,image->image,header_size))
        {
            hcoff_pe_image_deinit(image);
            return false;
        }
    }
    for(size_t i=0; i<image->header.coff_header.f_nscns; i++)
    {
        hcoff_sectionheader_t sectionheader= {0};
        if(!hcoff_pe_sectionheader_read(&image->header,&sectionheader,i,input_file))
        {
            hcoff_pe_image_deinit(image);
            return false;
        }
        if(sectionheader.s_size==0 || sectionheader.s_scnptr==0)
        {
            continue;
        }
        if(sectionheader.s_vaddr+sectionheader.s_size > image->image_size)
        {
            hcoff_pe_image_deinit(image);
            return false;
        }
        if(sectionheader.s_size > hcoff_file_input_read(input_file,sectionheader.s_scnptr,image->image+sectionheader.s_vaddr,sectionheader.s_size))
        {
            hcoff_pe_image_deinit(image);
            return false;
        }
    }
    //按照实际加载地址进行基址重定位
    int64_t delta=(int64_t)(uintptr_t)image->image-(int64_t)image->preferred_base;
    if(delta!=0 && image->header.data_directory[HCOFF_PE_DIRECTORY_ENTRY_BASERELOC].size!=0)
    {
        if(hcoff_pe_base_relocation_apply(&image->header,input_file,image->image,image->image_size,delta))
        {
            image->relocated=true;
        }
    }
    return true;
}

void hcoff_pe_image_deinit(hcoff_pe_image_t *image)
{
    if(image!=NULL)
    {
        memset(image,0,sizeof(hcoff_pe_image_t));
    }
}

hcoff_pe_image_t *hcoff_pe_image_new(hcoff_file_input_t *input_file)
{
    if(input_file==NULL)
    {
        return NULL;
    }
    size_t image_size=hcoff_pe_image_buffer_size_get(input_file);
    if(image_size==0)
    {
        return NULL;
    }
    hcoff_pe_image_t *image=(hcoff_pe_image_t *)hmalloc(sizeof(hcoff_pe_image_t));
    if(image==NULL)
    {
        return NULL;
    }
    memset(image,0,sizeof(hcoff_pe_image_t));
    void *buffer=hmmap(NULL,image_size,HMMAN_PROT_READ | HMMAN_PROT_WRITE | HMMAN_PROT_EXEC,HMMAN_MAP_ANONYMOUS | HMMAN_MAP_SHARED,(-1),0);
    if(buffer==NULL)
    {
        hfree(image);
        return NULL;
    }
    if(!hcoff_pe_image_init(image,input_file,buffer,image_size))
    {
        hmunmap(buffer,image_size);
        hfree(image);
        return NULL;
    }
    image->buffer_owned=true;
    return image;
}

void hcoff_pe_image_delete(hcoff_pe_image_t *image)
{
    if(image!=NULL)
    {
        void *buffer=image->image;
        size_t buffer_size=image->buffer_size;
        bool buffer_owned=image->buffer_owned;
        hcoff_pe_image_deinit(image);
        if(buffer_owned && buffer!=NULL && buffer_size!=0)
        {
            hmunmap(buffer,buffer_size);
        }
        hfree(image);
    }
}

void *hcoff_pe_image_rva_to_ptr(const hcoff_pe_image_t *image,uint32_t rva)
{
    if(image==NULL || image->image==NULL || rva >= image->image_size)
    {
        return NULL;
    }
    return image->image+rva;
}

void *hcoff_pe_image_entry_point(const hcoff_pe_image_t *image)
{
    if(image==NULL)
    {
        return NULL;
    }
    return hcoff_pe_image_rva_to_ptr(image,image->header.address_of_entry_point);
}

static const uint8_t *hcoff_pe_image_export_directory(const hcoff_pe_image_t *image)
{
    if(image==NULL || image->image==NULL)
    {
        return NULL;
    }
    uint32_t rva=image->header.data_directory[HCOFF_PE_DIRECTORY_ENTRY_EXPORT].virtual_address;
    uint32_t size=image->header.data_directory[HCOFF_PE_DIRECTORY_ENTRY_EXPORT].size;
    if(rva==0 || size < HCOFF_PE_EXPORT_DIRECTORY_SIZE || rva+HCOFF_PE_EXPORT_DIRECTORY_SIZE > image->image_size)
    {
        return NULL;
    }
    return image->image+rva;
}

void *hcoff_pe_image_export_find(const hcoff_pe_image_t *image,const char *name)
{
    if(image==NULL || name==NULL || image->image==NULL)
    {
        return NULL;
    }
    const uint8_t *export_dir=hcoff_pe_image_export_directory(image);
    if(export_dir==NULL)
    {
        return NULL;
    }
    uint32_t name_count=hcoff_get_uint32_le(export_dir+24);
    uint32_t function_count=hcoff_get_uint32_le(export_dir+20);
    uint32_t address_of_functions=hcoff_get_uint32_le(export_dir+28);
    uint32_t address_of_names=hcoff_get_uint32_le(export_dir+32);
    uint32_t address_of_name_ordinals=hcoff_get_uint32_le(export_dir+36);
    if(address_of_functions >= image->image_size || address_of_names >= image->image_size || address_of_name_ordinals >= image->image_size)
    {
        return NULL;
    }
    for(size_t i=0; i<name_count; i++)
    {
        if(address_of_names+i*4+4 > image->image_size || address_of_name_ordinals+i*2+2 > image->image_size)
        {
            break;
        }
        uint32_t name_rva=hcoff_get_uint32_le(image->image+address_of_names+i*4);
        uint16_t function_index=hcoff_get_uint16_le(image->image+address_of_name_ordinals+i*2);
        if(name_rva >= image->image_size || function_index >= function_count)
        {
            continue;
        }
        const char *export_name=(const char *)image->image+name_rva;
        if(0==strcmp(export_name,name))
        {
            if(address_of_functions+(uint32_t)function_index*4+4 > image->image_size)
            {
                return NULL;
            }
            uint32_t function_rva=hcoff_get_uint32_le(image->image+address_of_functions+(uint32_t)function_index*4);
            return hcoff_pe_image_rva_to_ptr(image,function_rva);
        }
    }
    return NULL;
}

bool hcoff_pe_image_resolve_imports(hcoff_pe_image_t *image,hcoff_pe_import_resolve_t resolve,void *usr)
{
    if(image==NULL || image->image==NULL || resolve==NULL)
    {
        return false;
    }
    uint32_t import_rva=image->header.data_directory[HCOFF_PE_DIRECTORY_ENTRY_IMPORT].virtual_address;
    if(import_rva==0 || import_rva >= image->image_size)
    {
        return true;
    }
    size_t thunk_size=image->header.is_64bit?8:4;
    for(size_t i=0; ; i++)
    {
        size_t descriptor_rva=import_rva+i*HCOFF_PE_IMPORT_DESCRIPTOR_SIZE;
        if(descriptor_rva+HCOFF_PE_IMPORT_DESCRIPTOR_SIZE > image->image_size)
        {
            return false;
        }
        const uint8_t *descriptor=image->image+descriptor_rva;
        uint32_t original_first_thunk=hcoff_get_uint32_le(descriptor);
        uint32_t name_rva=hcoff_get_uint32_le(descriptor+12);
        uint32_t first_thunk=hcoff_get_uint32_le(descriptor+16);
        if(name_rva==0 && original_first_thunk==0 && first_thunk==0)
        {
            break;
        }
        if(name_rva==0 || name_rva >= image->image_size)
        {
            return false;
        }
        const char *dll_name=(const char *)image->image+name_rva;
        uint32_t lookup_rva=original_first_thunk;
        if(lookup_rva==0)
        {
            lookup_rva=first_thunk;
        }
        for(size_t j=0; ; j++)
        {
            uint32_t lookup_address=lookup_rva+(uint32_t)(j*thunk_size);
            uint32_t iat_address=first_thunk+(uint32_t)(j*thunk_size);
            if(lookup_address+thunk_size > image->image_size || iat_address+thunk_size > image->image_size)
            {
                return false;
            }
            uint64_t value=image->header.is_64bit?hcoff_get_uint64_le(image->image+lookup_address):hcoff_get_uint32_le(image->image+lookup_address);
            if(value==0)
            {
                break;
            }
            uint64_t ordinal_flag=image->header.is_64bit?(1ULL << 63):(1ULL << 31);
            const char *function_name=NULL;
            uint16_t ordinal=0;
            if((value & ordinal_flag)!=0)
            {
                ordinal=(uint16_t)(value & 0xFFFF);
            }
            else
            {
                uint32_t hint_name_rva=(uint32_t)(value & 0x7FFFFFFF);
                if(hint_name_rva+2 >= image->image_size)
                {
                    return false;
                }
                function_name=(const char *)(image->image+hint_name_rva+2);
            }
            uintptr_t address=resolve(dll_name,function_name,ordinal,usr);
            if(address==0)
            {
                return false;
            }
            if(image->header.is_64bit)
            {
                hcoff_put_uint64_le(image->image+iat_address,(uint64_t)address);
            }
            else
            {
                hcoff_put_uint32_le(image->image+iat_address,(uint32_t)address);
            }
        }
    }
    return true;
}

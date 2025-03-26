/***************************************************************
 * Name:      hsim.h
 * Purpose:   hsim
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-03-25
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSIM_H__
#define __HSIM_H__
#include "hsimulator.h"
#ifdef __cplusplus

class hsimmcs51tiny
{
protected:
    /*
     * 用于子类实现的接口
     */
    virtual bool on_uart(hs_mcs_51_serial_t *serial,hs_mcs_51_serial_io_t io_type,uint16_t *data)
    {
        return true;
    }
    virtual void on_pin(hs_mcs_51_pin_t *pin,hs_mcs_51_pin_io_t io_type,hs_mcs_51_pin_port_t port,uint8_t pinnum)
    {

    }
    virtual bool on_io(hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr)
    {
        return true;
    }
private:
    uint8_t core[HS_MCS_51_CORE_SIZE()];
    hs_mcs_51_rom_t rom;
    hs_mcs_51_serial_t uart;
    hs_mcs_51_ram_model_tiny_t ram;
    hs_mcs_51_pin_t pin;
    hs_mcs_51_timer_t timer;
    hs_mcs_51_core_t *get_core()
    {
        return (hs_mcs_51_core_t *)core;
    }
    void init_core_and_peripheral()
    {
        hs_mcs_51_serial_init(&uart,[](hs_mcs_51_serial_t *serial,hs_mcs_51_serial_io_t io_type,uint16_t *data) -> bool
        {
            if(serial==NULL || serial->usr==NULL)
            {
                return false;
            }
            hsimmcs51tiny &obj=*(hsimmcs51tiny *)serial->usr;
            obj.on_uart(serial,io_type,data);
            return true;
        },this);
        hs_mcs_51_pin_init(&pin,[](hs_mcs_51_pin_t *pin,hs_mcs_51_pin_io_t io_type,hs_mcs_51_pin_port_t port,uint8_t pinnum) -> void
        {
            if(pin==NULL || pin->usr==NULL)
            {
                return;
            }
            hsimmcs51tiny &obj=*(hsimmcs51tiny *)pin->usr;
            obj.on_pin(pin,io_type,port,pinnum);
        },this);

        hs_mcs_51_timer_init(&timer);
        hs_mcs_51_core_init(core,[](hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr)->bool
        {
            if(usr==NULL)
            {
                return false;
            }
            hsimmcs51tiny &obj=*(hsimmcs51tiny *)usr;
            //RAM操作(注意：此操作前不应当有外设操作)
            hs_mcs_51_ram_model_tiny_bus_io(core,opt,address,data,length,usr,&obj.ram);
            //ROM操作
            hs_mcs_51_rom_bus_io(core,opt,address,data,length,usr,&obj.rom);
            //处理串口外设
            hs_mcs_51_serial_bus_io(core,opt,address,data,length,usr,&obj.uart);
            //处理PIN
            hs_mcs_51_pin_bus_io(core,opt,address,data,length,usr,&obj.pin);
            //处理定时器
            hs_mcs_51_timer_bus_io(core,opt,address,data,length,usr,&obj.timer);
            return obj.on_io(core,opt,address,data,length,usr);
        }
        ,this);
    }
    void copy_from_other_machine(hsimmcs51tiny *oths)
    {
        if(oths==NULL)
        {
            return;
        }
        //只复制程序
        rom=oths->rom;
        init_core_and_peripheral();
    }
public:
    hsimmcs51tiny():rom(HS_MCS_51_ROM_INITIALIZER)
    {
        init_core_and_peripheral();
    }

    hsimmcs51tiny(hsimmcs51tiny &oths):rom(HS_MCS_51_ROM_INITIALIZER)
    {
        copy_from_other_machine(&oths);
    }

    hsimmcs51tiny(hsimmcs51tiny &&oths):rom(HS_MCS_51_ROM_INITIALIZER)
    {
        copy_from_other_machine(&oths);
    }

    hsimmcs51tiny &operator =(hsimmcs51tiny &oths)
    {
        if(&oths==this)
        {
            return *this;
        }
        copy_from_other_machine(&oths);
        return *this;
    }

    hsimmcs51tiny &operator =(hsimmcs51tiny &&oths)
    {
        if(&oths==this)
        {
            return *this;
        }
        copy_from_other_machine(&oths);
        return *this;
    }

    void reset()
    {
        hs_mcs_51_core_reset(get_core());
    }

    void tick(size_t cycles=1)
    {
        hs_mcs_51_core_tick(get_core(),cycles);
    }

    hs_mcs_51_core_t * core_get()
    {
        return get_core();
    }

    hs_mcs_51_rom_t rom_get()
    {
        return rom;
    }

    hs_mcs_51_ram_model_tiny_t *ram_get()
    {
        return &ram;
    }

    hs_mcs_51_rom_t rom_set(const hs_mcs_51_rom_t &new_rom)
    {
        hs_mcs_51_rom_t old_rom=rom;
        rom=new_rom;
        return old_rom;
    }

    hs_mcs_51_serial_t *serial_get()
    {
        return &uart;
    }

    hs_mcs_51_pin_t *pin_get()
    {
        return &pin;
    }

    hs_mcs_51_timer_t *timer_get()
    {
        return &timer;
    }


    virtual ~hsimmcs51tiny()
    {

    }
};

class hsimmcs51small
{
protected:
    /*
     * 用于子类实现的接口
     */
    virtual bool on_uart(hs_mcs_51_serial_t *serial,hs_mcs_51_serial_io_t io_type,uint16_t *data)
    {
        return true;
    }
    virtual void on_pin(hs_mcs_51_pin_t *pin,hs_mcs_51_pin_io_t io_type,hs_mcs_51_pin_port_t port,uint8_t pinnum)
    {

    }
    virtual bool on_io(hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr)
    {
        return true;
    }
private:
    uint8_t core[HS_MCS_51_CORE_SIZE()];
    hs_mcs_51_rom_t rom;
    hs_mcs_51_serial_t uart;
    hs_mcs_51_ram_model_small_t ram;
    hs_mcs_51_pin_t pin;
    hs_mcs_51_timer_t timer;
    hs_mcs_51_core_t *get_core()
    {
        return (hs_mcs_51_core_t *)core;
    }
    void init_core_and_peripheral()
    {
        hs_mcs_51_serial_init(&uart,[](hs_mcs_51_serial_t *serial,hs_mcs_51_serial_io_t io_type,uint16_t *data) -> bool
        {
            if(serial==NULL || serial->usr==NULL)
            {
                return false;
            }
            hsimmcs51small &obj=*(hsimmcs51small *)serial->usr;
            obj.on_uart(serial,io_type,data);
            return true;
        },this);
        hs_mcs_51_pin_init(&pin,[](hs_mcs_51_pin_t *pin,hs_mcs_51_pin_io_t io_type,hs_mcs_51_pin_port_t port,uint8_t pinnum) -> void
        {
            if(pin==NULL || pin->usr==NULL)
            {
                return;
            }
            hsimmcs51small &obj=*(hsimmcs51small *)pin->usr;
            obj.on_pin(pin,io_type,port,pinnum);
        },this);

        hs_mcs_51_timer_init(&timer);
        hs_mcs_51_core_init(core,[](hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr)->bool
        {
            if(usr==NULL)
            {
                return false;
            }
            hsimmcs51small &obj=*(hsimmcs51small *)usr;
            //RAM操作(注意：此操作前不应当有外设操作)
            hs_mcs_51_ram_model_small_bus_io(core,opt,address,data,length,usr,&obj.ram);
            //ROM操作
            hs_mcs_51_rom_bus_io(core,opt,address,data,length,usr,&obj.rom);
            //处理串口外设
            hs_mcs_51_serial_bus_io(core,opt,address,data,length,usr,&obj.uart);
            //处理PIN
            hs_mcs_51_pin_bus_io(core,opt,address,data,length,usr,&obj.pin);
            //处理定时器
            hs_mcs_51_timer_bus_io(core,opt,address,data,length,usr,&obj.timer);
            return obj.on_io(core,opt,address,data,length,usr);
        }
        ,this);
    }
    void copy_from_other_machine(hsimmcs51small *oths)
    {
        if(oths==NULL)
        {
            return;
        }
        //只复制程序
        rom=oths->rom;
        init_core_and_peripheral();
    }
public:
    hsimmcs51small():rom(HS_MCS_51_ROM_INITIALIZER)
    {
        init_core_and_peripheral();
    }

    hsimmcs51small(hsimmcs51small &oths):rom(HS_MCS_51_ROM_INITIALIZER)
    {
        copy_from_other_machine(&oths);
    }

    hsimmcs51small(hsimmcs51small &&oths):rom(HS_MCS_51_ROM_INITIALIZER)
    {
        copy_from_other_machine(&oths);
    }

    hsimmcs51small &operator =(hsimmcs51small &oths)
    {
        if(&oths==this)
        {
            return *this;
        }
        copy_from_other_machine(&oths);
        return *this;
    }

    hsimmcs51small &operator =(hsimmcs51small &&oths)
    {
        if(&oths==this)
        {
            return *this;
        }
        copy_from_other_machine(&oths);
        return *this;
    }

    void reset()
    {
        hs_mcs_51_core_reset(get_core());
    }

    void tick(size_t cycles=1)
    {
        hs_mcs_51_core_tick(get_core(),cycles);
    }

    hs_mcs_51_core_t * core_get()
    {
        return get_core();
    }

    hs_mcs_51_rom_t rom_get()
    {
        return rom;
    }

    hs_mcs_51_ram_model_small_t *ram_get()
    {
        return &ram;
    }

    hs_mcs_51_rom_t rom_set(const hs_mcs_51_rom_t &new_rom)
    {
        hs_mcs_51_rom_t old_rom=rom;
        rom=new_rom;
        return old_rom;
    }

    hs_mcs_51_serial_t *serial_get()
    {
        return &uart;
    }

    hs_mcs_51_pin_t *pin_get()
    {
        return &pin;
    }

    hs_mcs_51_timer_t *timer_get()
    {
        return &timer;
    }


    virtual ~hsimmcs51small()
    {

    }
};


class hsimmcs51medium
{
protected:
    /*
     * 用于子类实现的接口
     */
    virtual bool on_uart(hs_mcs_51_serial_t *serial,hs_mcs_51_serial_io_t io_type,uint16_t *data)
    {
        return true;
    }
    virtual void on_pin(hs_mcs_51_pin_t *pin,hs_mcs_51_pin_io_t io_type,hs_mcs_51_pin_port_t port,uint8_t pinnum)
    {

    }
    virtual bool on_io(hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr)
    {
        return true;
    }
private:
    uint8_t core[HS_MCS_51_CORE_SIZE()];
    hs_mcs_51_rom_t rom;
    hs_mcs_51_serial_t uart;
    hs_mcs_51_ram_model_medium_t ram;
    hs_mcs_51_pin_t pin;
    hs_mcs_51_timer_t timer;
    hs_mcs_51_core_t *get_core()
    {
        return (hs_mcs_51_core_t *)core;
    }
    void init_core_and_peripheral()
    {
        hs_mcs_51_serial_init(&uart,[](hs_mcs_51_serial_t *serial,hs_mcs_51_serial_io_t io_type,uint16_t *data) -> bool
        {
            if(serial==NULL || serial->usr==NULL)
            {
                return false;
            }
            hsimmcs51medium &obj=*(hsimmcs51medium *)serial->usr;
            obj.on_uart(serial,io_type,data);
            return true;
        },this);
        hs_mcs_51_pin_init(&pin,[](hs_mcs_51_pin_t *pin,hs_mcs_51_pin_io_t io_type,hs_mcs_51_pin_port_t port,uint8_t pinnum) -> void
        {
            if(pin==NULL || pin->usr==NULL)
            {
                return;
            }
            hsimmcs51medium &obj=*(hsimmcs51medium *)pin->usr;
            obj.on_pin(pin,io_type,port,pinnum);
        },this);

        hs_mcs_51_timer_init(&timer);
        hs_mcs_51_core_init(core,[](hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr)->bool
        {
            if(usr==NULL)
            {
                return false;
            }
            hsimmcs51medium &obj=*(hsimmcs51medium *)usr;
            //RAM操作(注意：此操作前不应当有外设操作)
            hs_mcs_51_ram_model_medium_bus_io(core,opt,address,data,length,usr,&obj.ram);
            //ROM操作
            hs_mcs_51_rom_bus_io(core,opt,address,data,length,usr,&obj.rom);
            //处理串口外设
            hs_mcs_51_serial_bus_io(core,opt,address,data,length,usr,&obj.uart);
            //处理PIN
            hs_mcs_51_pin_bus_io(core,opt,address,data,length,usr,&obj.pin);
            //处理定时器
            hs_mcs_51_timer_bus_io(core,opt,address,data,length,usr,&obj.timer);
            return obj.on_io(core,opt,address,data,length,usr);
        }
        ,this);
    }
    void copy_from_other_machine(hsimmcs51medium *oths)
    {
        if(oths==NULL)
        {
            return;
        }
        //只复制程序
        rom=oths->rom;
        init_core_and_peripheral();
    }
public:
    hsimmcs51medium():rom(HS_MCS_51_ROM_INITIALIZER)
    {
        init_core_and_peripheral();
    }

    hsimmcs51medium(hsimmcs51medium &oths):rom(HS_MCS_51_ROM_INITIALIZER)
    {
        copy_from_other_machine(&oths);
    }

    hsimmcs51medium(hsimmcs51medium &&oths):rom(HS_MCS_51_ROM_INITIALIZER)
    {
        copy_from_other_machine(&oths);
    }

    hsimmcs51medium &operator =(hsimmcs51medium &oths)
    {
        if(&oths==this)
        {
            return *this;
        }
        copy_from_other_machine(&oths);
        return *this;
    }

    hsimmcs51medium &operator =(hsimmcs51medium &&oths)
    {
        if(&oths==this)
        {
            return *this;
        }
        copy_from_other_machine(&oths);
        return *this;
    }

    void reset()
    {
        hs_mcs_51_core_reset(get_core());
    }

    void tick(size_t cycles=1)
    {
        hs_mcs_51_core_tick(get_core(),cycles);
    }

    hs_mcs_51_core_t * core_get()
    {
        return get_core();
    }

    hs_mcs_51_rom_t rom_get()
    {
        return rom;
    }

    hs_mcs_51_ram_model_medium_t *ram_get()
    {
        return &ram;
    }

    hs_mcs_51_rom_t rom_set(const hs_mcs_51_rom_t &new_rom)
    {
        hs_mcs_51_rom_t old_rom=rom;
        rom=new_rom;
        reset();
        return old_rom;
    }

    hs_mcs_51_serial_t *serial_get()
    {
        return &uart;
    }

    hs_mcs_51_pin_t *pin_get()
    {
        return &pin;
    }

    hs_mcs_51_timer_t *timer_get()
    {
        return &timer;
    }


    virtual ~hsimmcs51medium()
    {

    }
};


class hsimmcs51large
{
protected:
    /*
     * 用于子类实现的接口
     */
    virtual bool on_uart(hs_mcs_51_serial_t *serial,hs_mcs_51_serial_io_t io_type,uint16_t *data)
    {
        return true;
    }
    virtual void on_pin(hs_mcs_51_pin_t *pin,hs_mcs_51_pin_io_t io_type,hs_mcs_51_pin_port_t port,uint8_t pinnum)
    {

    }
    virtual bool on_io(hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr)
    {
        return true;
    }
private:
    uint8_t core[HS_MCS_51_CORE_SIZE()];
    hs_mcs_51_rom_t rom;
    hs_mcs_51_serial_t uart;
    hs_mcs_51_ram_model_large_t ram;
    hs_mcs_51_pin_t pin;
    hs_mcs_51_timer_t timer;
    hs_mcs_51_core_t *get_core()
    {
        return (hs_mcs_51_core_t *)core;
    }
    void init_core_and_peripheral()
    {
        hs_mcs_51_serial_init(&uart,[](hs_mcs_51_serial_t *serial,hs_mcs_51_serial_io_t io_type,uint16_t *data) -> bool
        {
            if(serial==NULL || serial->usr==NULL)
            {
                return false;
            }
            hsimmcs51large &obj=*(hsimmcs51large *)serial->usr;
            obj.on_uart(serial,io_type,data);
            return true;
        },this);
        hs_mcs_51_pin_init(&pin,[](hs_mcs_51_pin_t *pin,hs_mcs_51_pin_io_t io_type,hs_mcs_51_pin_port_t port,uint8_t pinnum) -> void
        {
            if(pin==NULL || pin->usr==NULL)
            {
                return;
            }
            hsimmcs51large &obj=*(hsimmcs51large *)pin->usr;
            obj.on_pin(pin,io_type,port,pinnum);
        },this);

        hs_mcs_51_timer_init(&timer);
        hs_mcs_51_core_init(core,[](hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr)->bool
        {
            if(usr==NULL)
            {
                return false;
            }
            hsimmcs51large &obj=*(hsimmcs51large *)usr;
            //RAM操作(注意：此操作前不应当有外设操作)
            hs_mcs_51_ram_model_large_bus_io(core,opt,address,data,length,usr,&obj.ram);
            //ROM操作
            hs_mcs_51_rom_bus_io(core,opt,address,data,length,usr,&obj.rom);
            //处理串口外设
            hs_mcs_51_serial_bus_io(core,opt,address,data,length,usr,&obj.uart);
            //处理PIN
            hs_mcs_51_pin_bus_io(core,opt,address,data,length,usr,&obj.pin);
            //处理定时器
            hs_mcs_51_timer_bus_io(core,opt,address,data,length,usr,&obj.timer);
            return obj.on_io(core,opt,address,data,length,usr);
        }
        ,this);
    }
    void copy_from_other_machine(hsimmcs51large *oths)
    {
        if(oths==NULL)
        {
            return;
        }
        //只复制程序
        rom=oths->rom;
        init_core_and_peripheral();
    }
public:
    hsimmcs51large():rom(HS_MCS_51_ROM_INITIALIZER)
    {
        init_core_and_peripheral();
    }

    hsimmcs51large(hsimmcs51large &oths):rom(HS_MCS_51_ROM_INITIALIZER)
    {
        copy_from_other_machine(&oths);
    }

    hsimmcs51large(hsimmcs51large &&oths):rom(HS_MCS_51_ROM_INITIALIZER)
    {
        copy_from_other_machine(&oths);
    }

    hsimmcs51large &operator =(hsimmcs51large &oths)
    {
        if(&oths==this)
        {
            return *this;
        }
        copy_from_other_machine(&oths);
        return *this;
    }

    hsimmcs51large &operator =(hsimmcs51large &&oths)
    {
        if(&oths==this)
        {
            return *this;
        }
        copy_from_other_machine(&oths);
        return *this;
    }

    void reset()
    {
        hs_mcs_51_core_reset(get_core());
    }

    void tick(size_t cycles=1)
    {
        hs_mcs_51_core_tick(get_core(),cycles);
    }

    hs_mcs_51_core_t * core_get()
    {
        return get_core();
    }

    hs_mcs_51_rom_t rom_get()
    {
        return rom;
    }

    hs_mcs_51_ram_model_large_t *ram_get()
    {
        return &ram;
    }

    hs_mcs_51_rom_t rom_set(const hs_mcs_51_rom_t &new_rom)
    {
        hs_mcs_51_rom_t old_rom=rom;
        rom=new_rom;
        return old_rom;
    }

    hs_mcs_51_serial_t *serial_get()
    {
        return &uart;
    }

    hs_mcs_51_pin_t *pin_get()
    {
        return &pin;
    }

    hs_mcs_51_timer_t *timer_get()
    {
        return &timer;
    }


    virtual ~hsimmcs51large()
    {

    }
};

class hsimmcs51huge
{
protected:
    /*
     * 用于子类实现的接口
     */
    virtual bool on_uart(hs_mcs_51_serial_t *serial,hs_mcs_51_serial_io_t io_type,uint16_t *data)
    {
        return true;
    }
    virtual void on_pin(hs_mcs_51_pin_t *pin,hs_mcs_51_pin_io_t io_type,hs_mcs_51_pin_port_t port,uint8_t pinnum)
    {

    }
    virtual bool on_io(hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr)
    {
        return true;
    }
private:
    uint8_t core[HS_MCS_51_CORE_SIZE()];
    hs_mcs_51_rom_t rom;
    hs_mcs_51_serial_t uart;
    hs_mcs_51_ram_model_huge_t ram;
    hs_mcs_51_pin_t pin;
    hs_mcs_51_timer_t timer;
    hs_mcs_51_core_t *get_core()
    {
        return (hs_mcs_51_core_t *)core;
    }
    void init_core_and_peripheral()
    {
        hs_mcs_51_serial_init(&uart,[](hs_mcs_51_serial_t *serial,hs_mcs_51_serial_io_t io_type,uint16_t *data) -> bool
        {
            if(serial==NULL || serial->usr==NULL)
            {
                return false;
            }
            hsimmcs51huge &obj=*(hsimmcs51huge *)serial->usr;
            obj.on_uart(serial,io_type,data);
            return true;
        },this);
        hs_mcs_51_pin_init(&pin,[](hs_mcs_51_pin_t *pin,hs_mcs_51_pin_io_t io_type,hs_mcs_51_pin_port_t port,uint8_t pinnum) -> void
        {
            if(pin==NULL || pin->usr==NULL)
            {
                return;
            }
            hsimmcs51huge &obj=*(hsimmcs51huge *)pin->usr;
            obj.on_pin(pin,io_type,port,pinnum);
        },this);

        hs_mcs_51_timer_init(&timer);
        hs_mcs_51_core_init(core,[](hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr)->bool
        {
            if(usr==NULL)
            {
                return false;
            }
            hsimmcs51huge &obj=*(hsimmcs51huge *)usr;
            //RAM操作(注意：此操作前不应当有外设操作)
            hs_mcs_51_ram_model_huge_bus_io(core,opt,address,data,length,usr,&obj.ram);
            //ROM操作
            hs_mcs_51_rom_bus_io(core,opt,address,data,length,usr,&obj.rom);
            //处理串口外设
            hs_mcs_51_serial_bus_io(core,opt,address,data,length,usr,&obj.uart);
            //处理PIN
            hs_mcs_51_pin_bus_io(core,opt,address,data,length,usr,&obj.pin);
            //处理定时器
            hs_mcs_51_timer_bus_io(core,opt,address,data,length,usr,&obj.timer);
            return obj.on_io(core,opt,address,data,length,usr);
        }
        ,this);
    }
    void copy_from_other_machine(hsimmcs51huge *oths)
    {
        if(oths==NULL)
        {
            return;
        }
        //只复制程序
        rom=oths->rom;
        init_core_and_peripheral();
    }
public:
    hsimmcs51huge():rom(HS_MCS_51_ROM_INITIALIZER)
    {
        init_core_and_peripheral();
    }

    hsimmcs51huge(hsimmcs51huge &oths):rom(HS_MCS_51_ROM_INITIALIZER)
    {
        copy_from_other_machine(&oths);
    }

    hsimmcs51huge(hsimmcs51huge &&oths):rom(HS_MCS_51_ROM_INITIALIZER)
    {
        copy_from_other_machine(&oths);
    }

    hsimmcs51huge &operator =(hsimmcs51huge &oths)
    {
        if(&oths==this)
        {
            return *this;
        }
        copy_from_other_machine(&oths);
        return *this;
    }

    hsimmcs51huge &operator =(hsimmcs51huge &&oths)
    {
        if(&oths==this)
        {
            return *this;
        }
        copy_from_other_machine(&oths);
        return *this;
    }

    void reset()
    {
        hs_mcs_51_core_reset(get_core());
    }

    void tick(size_t cycles=1)
    {
        hs_mcs_51_core_tick(get_core(),cycles);
    }

    hs_mcs_51_core_t * core_get()
    {
        return get_core();
    }

    hs_mcs_51_rom_t rom_get()
    {
        return rom;
    }

    hs_mcs_51_ram_model_huge_t *ram_get()
    {
        return &ram;
    }

    hs_mcs_51_rom_t rom_set(const hs_mcs_51_rom_t &new_rom)
    {
        hs_mcs_51_rom_t old_rom=rom;
        rom=new_rom;
        return old_rom;
    }

    hs_mcs_51_serial_t *serial_get()
    {
        return &uart;
    }

    hs_mcs_51_pin_t *pin_get()
    {
        return &pin;
    }

    hs_mcs_51_timer_t *timer_get()
    {
        return &timer;
    }


    virtual ~hsimmcs51huge()
    {

    }
};



#endif
#endif // __HSIM_H__


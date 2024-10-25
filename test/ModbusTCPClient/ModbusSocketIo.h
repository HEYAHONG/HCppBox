#ifndef __MODBUSSOCKETIO_H_INCLUDED__
#define __MODBUSSOCKETIO_H_INCLUDED__
#include "HCPPBox.h"
#include "hbox.h"

class ModbusSocketIo
{
private:
    int socket_fd;
public:
    bool IsConnected()
    {
        return socket_fd!=INVALID_SOCKET;
    }
    size_t send(const uint8_t *buffer,size_t length)
    {
        size_t offset=0;
        while( IsConnected() && offset < length)
        {
            int sendlen=::send(socket_fd,(const char *)(buffer+offset),length-offset,0);
            if(sendlen>0)
            {
                offset+=sendlen;
            }
            if(sendlen==0)
            {
                //连接已断开
                closesocket(socket_fd);
                socket_fd=INVALID_SOCKET;
            }

        }
        return offset;
    }
    size_t recv(uint8_t *buffer,size_t length)
    {
        if(!IsConnected())
        {
            return 0;
        }
        int recvlen=::recv(socket_fd,(char *)buffer,length,0);
        if(recvlen==0)
        {
            //连接已断开
            closesocket(socket_fd);
            socket_fd=INVALID_SOCKET;
        }
        if(recvlen<0)
        {
            int socket_errno=errno;
#ifdef WIN32
            if(socket_errno!=EWOULDBLOCK)
#else
            if(socket_errno!=EAGAIN)
#endif // WIN32
            {
                closesocket(socket_fd);
                socket_fd=INVALID_SOCKET;
            }
        }
        if(recvlen > 0)
        {
            return recvlen;
        }
        return 0;
    }
    bool connect_ipv4(const char *host,const char *port)
    {
        if(IsConnected())
        {
            return false;
        }
        bool ret=false;
        socket_fd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
        if(socket_fd!=INVALID_SOCKET)
        {
            HCPPSocketAddressIPV4 addr= {0};
            {
                //DNS查询
                struct addrinfo hints = { 0 };
                struct addrinfo *ai_result = NULL;
                hints.ai_family = AF_INET;
                hints.ai_socktype = SOCK_STREAM;
                hints.ai_flags = AI_CANONNAME;
                hints.ai_protocol = IPPROTO_TCP;
                if (getaddrinfo(host, port, &hints, &ai_result) == 0)
                {
                    for (struct addrinfo* info = ai_result; info != NULL; info = info->ai_next)
                    {
                        if (info->ai_family == AF_INET)
                        {
                            addr = *(HCPPSocketAddressIPV4*)info->ai_addr;
                            ret=true;
                        }
                    }
                    freeaddrinfo(ai_result);
                }
            }
            if(ret)
            {
                //连接
                if(0!=connect(socket_fd,(HCPPSocketAddress *)&addr,sizeof(addr)))
                {
                    ret=false;
                }
                else
                {
                    {
                        //设定接收超时5ms
                        struct timeval  tv;
                        tv.tv_sec = 0;
                        tv.tv_usec = 500*1000;
                        ret=(0==setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv)));
                    }
                }
            }
        }
        if(!ret)
        {
            if(socket_fd!=INVALID_SOCKET)
            {
                closesocket(socket_fd);
                socket_fd=INVALID_SOCKET;
            }
        }
        return ret;
    }
    bool disconnect()
    {
        if(IsConnected())
        {
            closesocket(socket_fd);
            socket_fd=INVALID_SOCKET;
            return true;
        }
        return false;
    }
private:
    static size_t _send(modbus_io_interface_t *io,const uint8_t *adu,size_t adu_length)
    {
        if(io==NULL || io->usr == NULL)
        {
            return 0;
        }
        ModbusSocketIo &IO=*(ModbusSocketIo *)io->usr;

        return IO.send(adu,adu_length);

    }
    static size_t  _recv(modbus_io_interface_t *io,uint8_t *buffer,size_t buffer_length)
    {
        if(io==NULL || io->usr == NULL)
        {
            return 0;
        }
        ModbusSocketIo &IO=*(ModbusSocketIo *)io->usr;

        return IO.recv(buffer,buffer_length);
    }
public:
    ModbusSocketIo():socket_fd{INVALID_SOCKET}
    {

    }
    virtual ~ModbusSocketIo()
    {
        disconnect();
    }
    modbus_io_interface_t GetIoInterface()
    {
        modbus_io_interface_t ret = modbus_io_interface_default();
        ret.usr=(ModbusSocketIo *)this;
        ret.send=_send;
        ret.recv=_recv;
        return ret;
    }
};

#endif // __MODBUSSOCKETIO_H_INCLUDED__

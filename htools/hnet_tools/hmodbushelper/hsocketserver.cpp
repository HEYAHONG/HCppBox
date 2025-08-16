#include "hsocketserver.h"



HSocketServer::HSocketServer()
{
    system_loop_slot=heventslots_register_system_loop_slot(this,[](void * evt,void *usr)
    {
        if(usr==NULL)
        {
            return;
        }
        HSocketServer & server=*(HSocketServer*)usr;
        server.ServerLoop();
    },
    NULL);
}

HSocketServer::~HSocketServer()
{
    heventslots_unregister_system_loop_slot(system_loop_slot);
}

bool HSocketServer::IsIPV4ServerOk()
{
    std::lock_guard<std::recursive_mutex> lock(m_server_loop_lock);
    if(ipv4_server!=NULL)
    {
        return ipv4_server->IsOk();
    }
    return false;
}

bool HSocketServer::IsIPV6ServerOk()
{
    std::lock_guard<std::recursive_mutex> lock(m_server_loop_lock);
    if(ipv6_server!=NULL)
    {
        return ipv6_server->IsOk();
    }
    return false;
}

void HSocketServer::StopIPV4()
{
    std::lock_guard<std::recursive_mutex> lock(m_server_loop_lock);
    if(ipv4_server!=NULL)
    {
        ipv4_server.reset();
    }
}

void HSocketServer::StopIPV6()
{
    std::lock_guard<std::recursive_mutex> lock(m_server_loop_lock);
    if(ipv6_server!=NULL)
    {
        ipv6_server.reset();
    }
}

bool HSocketServer::StartIPV4(wxString hostname,uint16_t port)
{
    std::lock_guard<std::recursive_mutex> lock(m_server_loop_lock);
    wxIPV4address address;
    if(hostname.IsEmpty())
    {
        address.Hostname(wxString(_T("0.0.0.0")));
    }
    else
    {
        address.Hostname(hostname);
    }
    address.Service(port);

    ipv4_server=new wxSocketServer(address);

    if(!IsIPV4ServerOk())
    {
        ipv4_server.reset();
    }

    return IsIPV4ServerOk();
}

bool  HSocketServer::StartIPV6(wxString hostname,uint16_t port)
{
    std::lock_guard<std::recursive_mutex> lock(m_server_loop_lock);
    wxIPV6address address;
    if(hostname.IsEmpty())
    {
        address.Hostname(wxString(_T("::")));
    }
    else
    {
        address.Hostname(hostname);
    }
    address.Service(port);

    ipv6_server=new wxSocketServer(address);

    if(!IsIPV6ServerOk())
    {
        ipv6_server.reset();
    }

    return IsIPV6ServerOk();
}

void HSocketServer::SetOnConnect(std::function<void(HSocketServer&,wxSharedPtr<wxSocketServer>,wxSharedPtr<wxSocketBase>)> func)
{
    on_connect=func;
}

void HSocketServer::SetOnDisconnect(std::function<void(HSocketServer&,wxSharedPtr<wxSocketServer>,wxSharedPtr<wxSocketBase>)> func)
{
    on_disconnect=func;
}

void HSocketServer::SetOnData(std::function<void(HSocketServer&,wxSharedPtr<wxSocketServer>,wxSharedPtr<wxSocketBase>)> func)
{
    on_data=func;
}

void HSocketServer::ServerLoop()
{
    ServerIPV4Loop();
    ServerIPV6Loop();
}

void HSocketServer::ServerIPV4Loop()
{
    std::lock_guard<std::recursive_mutex> lock(m_server_loop_lock);
    auto server=ipv4_server;
    if(server==NULL || !server->IsOk())
    {
        return;
    }
    wxSharedPtr<wxSocketBase> con(server->Accept(false));
    if(con!=NULL)
    {
        if(on_connect!=NULL)
        {
            on_connect(*this,server,con);
        }
        ipv4_connection_list.push_back(con);
    }

    while(true)
    {
        bool IsCleanup=true;
        for(auto it=ipv4_connection_list.begin(); it!=ipv4_connection_list.end(); it++)
        {
            if((*it)->IsDisconnected())
            {
                if(on_disconnect!=NULL)
                {
                    on_disconnect(*this,server,(*it));
                }
                ipv4_connection_list.erase(it);
                IsCleanup=false;
                break;
            }
        }
        if(IsCleanup)
        {
            break;
        }
    }

    for(auto it=ipv4_connection_list.begin(); it!=ipv4_connection_list.end(); it++)
    {
        if((*it)->IsData())
        {
            //接收数据
            if(on_data!=NULL)
            {
                on_data(*this,server,(*it));
            }
        }
    }
}

void HSocketServer::ServerIPV6Loop()
{
    std::lock_guard<std::recursive_mutex> lock(m_server_loop_lock);
    auto server=ipv6_server;
    if(server==NULL || !server->IsOk())
    {
        return;
    }
    wxSharedPtr<wxSocketBase> con(server->Accept(false));
    if(con!=NULL)
    {
        if(on_connect!=NULL)
        {
            on_connect(*this,server,con);
        }
        ipv6_connection_list.push_back(con);
    }

    while(true)
    {
        bool IsCleanup=true;
        for(auto it=ipv6_connection_list.begin(); it!=ipv6_connection_list.end(); it++)
        {
            if((*it)->IsDisconnected())
            {
                if(on_disconnect!=NULL)
                {
                    on_disconnect(*this,server,(*it));
                }
                ipv6_connection_list.erase(it);
                IsCleanup=false;
                break;
            }
        }
        if(IsCleanup)
        {
            break;
        }
    }

    for(auto it=ipv6_connection_list.begin(); it!=ipv6_connection_list.end(); it++)
    {
        if((*it)->IsData())
        {
            //接收数据
            if(on_data!=NULL)
            {
                on_data(*this,server,(*it));
            }
        }
    }
}


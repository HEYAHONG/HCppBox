#ifndef HSOCKETSERVER_H
#define HSOCKETSERVER_H

#ifdef __cplusplus
#include "wx/sharedptr.h"
#include "wx/vector.h"
#include "wx/socket.h"
#include "stdint.h"
#include "stdlib.h"
#include "functional"
#include "mutex"
#include "HCPPBox.h"

class HSocketServer
{
public:
    HSocketServer();
    virtual ~HSocketServer();
    bool IsIPV4ServerOk();
    bool IsIPV6ServerOk();
    void StopIPV4();
    void StopIPV6();
    bool StartIPV4(wxString hostname,uint16_t port);
    bool StartIPV6(wxString hostname,uint16_t port);
    void SetOnConnect(std::function<void(HSocketServer&,wxSharedPtr<wxSocketServer>,wxSharedPtr<wxSocketBase>)> func);
    void SetOnDisconnect(std::function<void(HSocketServer&,wxSharedPtr<wxSocketServer>,wxSharedPtr<wxSocketBase>)> func);
    void SetOnData(std::function<void(HSocketServer&,wxSharedPtr<wxSocketServer>,wxSharedPtr<wxSocketBase>)> func);
private:
    wxSharedPtr<wxSocketServer> ipv4_server;
    wxVector<wxSharedPtr<wxSocketBase>> ipv4_connection_list;
    wxSharedPtr<wxSocketServer> ipv6_server;
    wxVector<wxSharedPtr<wxSocketBase>> ipv6_connection_list;
    std::function<void(HSocketServer&,wxSharedPtr<wxSocketServer>,wxSharedPtr<wxSocketBase>)> on_connect;
    std::function<void(HSocketServer&,wxSharedPtr<wxSocketServer>,wxSharedPtr<wxSocketBase>)> on_disconnect;
    std::function<void(HSocketServer&,wxSharedPtr<wxSocketServer>,wxSharedPtr<wxSocketBase>)> on_data;
    uint32_t system_loop_slot;
    std::recursive_mutex m_server_loop_lock;
    void ServerLoop();
    void ServerIPV4Loop();
    void ServerIPV6Loop();
};

#endif

#endif // HSOCKETSERVER_H

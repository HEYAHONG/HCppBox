#include "HCPPBox.h"
#include "hrc.h"
#ifdef HDEFAULTS_OS_UNIX
#include <signal.h>
#endif // HDEFAULTS_OS_UNIX
#include "inttypes.h"
#include H3RDPARTY_ARGTABLE3_HEADER
#ifdef HAVE_SYSLOG_H
#include "syslog.h"
#endif // HAVE_SYSLOG_H
#include "socket_tcp_helper.h"
#include "memory"
#if defined(HAVE_OPENSSL)
#include <openssl/ssl.h>
#include <openssl/err.h>
class openssl_guaurd
{
public:
    SSL_CTX *ctx;
    openssl_guaurd()
    {
        SSL_library_init();
        SSL_load_error_strings();
        OpenSSL_add_all_algorithms();
        ctx=SSL_CTX_new(TLS_client_method());
        if(ctx!=NULL)
        {
            SSL_CTX_set_default_verify_paths(ctx);
#if defined(WIN32) || defined(_WIN32)
            {
                /*
                 * 从Windows证书存储导入证书
                 */
                X509_STORE *store=SSL_CTX_get_cert_store(ctx);
                if(store!=NULL)
                {
                    HCERTSTORE hStore=CertOpenSystemStoreA(0,"ROOT");
                    if(hStore!=NULL)
                    {
                        PCCERT_CONTEXT pContext = NULL;
                        while((pContext = CertEnumCertificatesInStore(hStore, pContext)) != NULL)
                        {
                            if (!(pContext->dwCertEncodingType & X509_ASN_ENCODING))
                            {
                                continue;
                            }
                            const unsigned char *encoded_cert = pContext->pbCertEncoded;
                            X509 *x509 = d2i_X509(NULL, &encoded_cert, pContext->cbCertEncoded);
                            if (x509 != NULL)
                            {
                                if (X509_STORE_add_cert(store, x509) == 1)
                                {

                                }
                                else
                                {

                                    ERR_clear_error();
                                }
                                X509_free(x509);
                            }
                        }
                        CertCloseStore(hStore, 0);
                    }
                }
            }
#endif
            /*
             * 默认不检验证书
             */
            SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
        }
    }
    ~openssl_guaurd()
    {
        if(ctx!=NULL)
        {
            SSL_CTX_free(ctx);
            ctx=NULL;
        }
    }
} g_ssl;
#endif


static std::string uri_string="http://syscall.online/";

static void show_banner()
{
    const char * banner=(const char *)RCGetHandle((const char *)"banner");
    if(banner!=NULL)
    {
        hfprintf(stderr,"\r\n%s\r\n",banner);
    }
}


static void check_args(int argc,char *argv[])
{
    struct arg_lit  * help=NULL;
    struct arg_str  * uri=NULL;
    void *argtable[]=
    {
        uri=arg_str0(NULL,NULL,NULL,"http uri"),
        help=arg_lit0("H","help","print this help and exit"),
        arg_end(20)
    };
    if(arg_nullcheck(argtable)!=0)
    {
        hfprintf(stderr,"arg_nullcheck error!\r\n");
        hexit(-1);
    }

    if(arg_parse(argc,argv,argtable)>0)
    {
        hfputs("arg_parse error!\r\n",stderr);
        hfputs("Usage:\r\n",stderr);
        arg_print_glossary(stderr,argtable,"  %-30s %s\n");
        hexit(-1);
    }

    if(help->count > 0)
    {
        hfputs("Usage:\r\n",stdout);
        arg_print_glossary(stdout,argtable,"  %-30s %s\n");
        hexit(-1);
    }


    if(uri->count > 0)
    {
        uri_string=uri->sval[0];
    }

    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
}

#if defined(HAVE_OPENSSL)
class client_socket_ssl
{
public:
    SSL *ssl_socket;
    client_socket_ssl():ssl_socket(NULL)
    {

    }
    ~client_socket_ssl()
    {
        if(ssl_socket!=NULL)
        {
            SSL_free(ssl_socket);
            ssl_socket=NULL;
        }
    }
} g_ssl_socket;
#endif
HCPPSocketAddressIPV4 server_socket_addr;
static bool http_server_addr_check(void)
{

    HCPPSocketInit();

    bool ret=false;

    if(!ret)
    {
        {
            /*
             * 检查shceme
             */
            huriparser_string_t scheme_string=huriparser_parse_scheme(uri_string.c_str());
            if(huriparser_string_is_empty(scheme_string))
            {
                hfprintf(stderr,"[http] scheme is empty!\r\n");
                return false;
            }
            std::string scheme(scheme_string.str,scheme_string.str_len);
#if defined(HAVE_OPENSSL)
            if((scheme == "https" || scheme == "HTTPS") &&g_ssl.ctx!=NULL)
            {
                g_ssl_socket.ssl_socket=SSL_new(g_ssl.ctx);
            }
            if((scheme != "http" &&scheme != "HTTP") && (g_ssl.ctx==NULL || (scheme != "https" &&scheme != "HTTPS")))
#else
            if(scheme != "http" &&scheme != "HTTP")
#endif
            {
                hfprintf(stderr,"[http] scheme %s not support!\r\n",scheme.c_str());
                return false;
            }

        }

        std::string server_addr;
        {
            huriparser_string_t host_string=huriparser_parse_host(uri_string.c_str());
            if(!huriparser_string_is_empty(host_string))
            {
                server_addr=std::string(host_string.str,host_string.str_len);
            }
            else
            {
                hfprintf(stderr,"[dns] host is empty!\r\n");
                return false;
            }
        }
        std::string server_port="80";
#if defined(HAVE_OPENSSL)
        if(g_ssl_socket.ssl_socket!=NULL)
        {
            server_port="443";
        }
#endif
        {
            huriparser_string_t port_string=huriparser_parse_port(uri_string.c_str());
            if(!huriparser_string_is_empty(port_string))
            {
                server_port=std::string(port_string.str,port_string.str_len);
            }
        }
        ret=HCPPSocketNslookup(server_addr.c_str(),
                               [=](const char* hostname, const char*addr_string, HCPPSocketAddressIPV4* sock_addr,void *usr)
        {
            server_socket_addr=(*sock_addr);
            server_socket_addr.sin_port=htons(std::stoul(server_port));
            hfprintf(stderr,"[dns] %s:%s\r\n",hostname,addr_string);
        },NULL);
    }

    if(!ret)
    {
        hfprintf(stderr,"[dns] error!\r\n");
    }

    return ret;

}

static int http_main(void)
{
    if(!http_server_addr_check())
    {
        return -1;
    }

    SOCKET client_fd=socket(AF_INET,SOCK_STREAM,0);
    if(client_fd == INVALID_SOCKET)
    {
        hfprintf(stderr,"[http] new socket error!\r\n");
        return -1;
    }

    if(connect(client_fd,(HCPPSocketAddress *)&server_socket_addr,sizeof(server_socket_addr)) < 0)
    {
        hfprintf(stderr,"[http] connect error!\r\n");
        closesocket(client_fd);
        client_fd=INVALID_SOCKET;
        return -1;
    }


#if defined(HAVE_OPENSSL)
    if(g_ssl_socket.ssl_socket!=NULL)
    {
        SSL_set_fd(g_ssl_socket.ssl_socket, client_fd);
        if(SSL_connect(g_ssl_socket.ssl_socket) <= 0)
        {
            hfprintf(stderr,"[http] ssl connect error!");
            ERR_print_errors_fp(stderr);
            hfprintf(stderr,"\r\n");
            SSL_shutdown(g_ssl_socket.ssl_socket);
            closesocket(client_fd);
            client_fd=INVALID_SOCKET;
            return -1;
        }
    }
#endif

    {
        tcp_socket_set_nonblock(client_fd);
    }

    hhttp_client_io_t socket_io;
    {
        socket_io.send=[](hhttp_client_io_t *io,hhttp_string_t part_data)
        {
            if(io==NULL || ((void *)io->usr)==NULL)
            {
                return;
            }
            SOCKET &client_fd=(*(SOCKET*)io->usr);
            if(client_fd!=INVALID_SOCKET)
            {
                uint8_t *data=(uint8_t *)part_data.str;
                size_t len=part_data.str_len;

#if defined(HAVE_OPENSSL)
                if(g_ssl_socket.ssl_socket!=NULL)
                {
                    if(len > 0)
                    {
                        SSL_write(g_ssl_socket.ssl_socket,data,len);
                    }
                }
                else
#endif
                {
                    size_t offset=0;
                    do
                    {
                        int sendlen=send(client_fd,(const char *)(&((uint8_t *)data)[offset]),len-offset,0);
                        if(sendlen>0)
                        {
                            offset+=sendlen;
                        }
                        if(sendlen==0)
                        {
                            //连接已断开
                            offset=len;
                        }
                    }
                    while(offset < len);
                }
            }

        };
        socket_io.close=[](hhttp_client_io_t *io)
        {
            if(io==NULL || ((void *)io->usr)==NULL)
            {
                return;
            }
#if defined(HAVE_OPENSSL)
            if(g_ssl_socket.ssl_socket!=NULL)
            {
                SSL_shutdown(g_ssl_socket.ssl_socket);
            }
#endif
            SOCKET &client_fd=(*(SOCKET*)io->usr);
            if(client_fd==INVALID_SOCKET)
            {
                return;
            }
            closesocket(client_fd);
            client_fd=INVALID_SOCKET;
        };
        socket_io.is_ok=[](hhttp_client_io_t *io) -> bool
        {
            if(io==NULL || ((void *)io->usr)==NULL)
            {
                return false;
            }
            SOCKET &client_fd=(*(SOCKET*)io->usr);

            if(client_fd==INVALID_SOCKET)
            {
                return false;
            }
            return true;
        };
        socket_io.usr=(intptr_t)&client_fd;
    }

    hhttp_client_t http_client;
    hhttp_client_init(&http_client);
    {
        /*
         * 填写成员
         */
        http_client.io=socket_io;

        http_client.request.field_process_alloc=[](hhttp_client_t *client,hhttp_field_t *field,bool is_internal)->bool
        {
            return false;
        };
        http_client.request.field_process_free=[](hhttp_client_t *client,hhttp_field_t *field,bool is_internal)->bool
        {
            return false;
        };

        http_client.response.process_status=[](hhttp_client_t *client,hhttp_status_line_t line)
        {
            std::string version(hhttp_version_std_to_string(line.version).str);
            std::string reason;
            if(!hhttp_string_is_empty(line.reason))
            {
                reason=std::string(line.reason.str,line.reason.str_len);
            }
            hfprintf(stderr,"[http] > %s %d %s\r\n",version.c_str(),(int)line.status,reason.c_str());
        };
        http_client.response.process_field=[](hhttp_client_t *client,hhttp_field_t field)
        {
            std::string name;
            if(!hhttp_string_is_empty(field.field_name))
            {
                name=std::string(field.field_name.str,field.field_name.str_len);
            }
            std::string values;
            if(!hhttp_string_is_empty(field.field_values))
            {
                values=std::string(field.field_values.str,field.field_values.str_len);
            }
            hfprintf(stderr,"[http] > %s: %s\r\n",name.c_str(),values.c_str());
        };
        http_client.response.process_body=[](hhttp_client_t *client,uint64_t offset,hhttp_string_t part_data)
        {
            if(hhttp_string_is_empty(part_data))
            {
                return;
            }
            std::string data_string(part_data.str,part_data.str_len);
            hfwrite(data_string.c_str(),1,data_string.length(),stdout);
        };
        http_client.response.process_body_end=[](hhttp_client_t *client)
        {

        };

        http_client.usr=&client_fd;
    }

    {
        /*
         * 发送请求
         */
        hhttp_method_t method=HHTTP_METHOD_GET;
        hhttp_string_t body=hhttp_string_from_cstring("");
        if(!hhttp_client_send_request(&http_client,method,uri_string.c_str(),body))
        {
            hhttp_client_io_close(&socket_io);
            hfprintf(stderr,"[http] send request error!\r\n");
            return -1;
        }

    }

    while(!hhttp_client_is_end(&http_client))
    {
        uint8_t buffer[4096];
        int readlen=0;
#if defined(HAVE_OPENSSL)
        if(g_ssl_socket.ssl_socket!=NULL)
        {
            readlen=SSL_read(g_ssl_socket.ssl_socket,buffer,sizeof(buffer));
        }
        else
#endif
        {
            readlen=recv(client_fd,(char *)buffer,sizeof(buffer),0);
        }
        if(readlen > 0)
        {
            if(!hhttp_client_input(&http_client,buffer,readlen))
            {
                break;
            }
        }
        if(readlen == 0 || (readlen < 0 && !tcp_socket_is_egain()))
        {
            if(!hhttp_client_input_end(&http_client))
            {
                break;
            }
            break;
        }
    }

    hhttp_client_io_close(&socket_io);

    return -hhttp_client_is_error(&http_client);
}

int main(int argc,char *argv[])
{

    check_args(argc,argv);

    show_banner();

    return http_main();
}

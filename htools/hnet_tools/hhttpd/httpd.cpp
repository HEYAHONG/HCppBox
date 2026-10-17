#include "httpd.h"
#include H3RDPARTY_URIPARSER_URI_HEADER
#include "string"
#include "map"

class httpd_session;
class httpd_file
{
    hhttp_tinyserver_file_t file;
    static uint64_t file_content_read(hhttp_tinyserver_file_t *file,uint64_t offset,uint8_t *buffer,uint64_t buffer_size);
    static bool file_field_process_alloc(hhttp_tinyserver_file_t *file,hhttp_field_t *field,bool is_internal);
    static bool file_field_process_free(hhttp_tinyserver_file_t *file,hhttp_field_t *field,bool is_internal);
protected:
    httpd_session &session;
    virtual uint64_t content_read(hhttp_tinyserver_file_t *file,uint64_t offset,uint8_t *buffer,uint64_t buffer_size)
    {
        return (uint64_t)-1;
    }
    virtual bool field_process_alloc(hhttp_tinyserver_file_t *file,hhttp_field_t *field,bool is_internal)
    {
        return false;
    }
    virtual bool field_process_free(hhttp_tinyserver_file_t *file,hhttp_field_t *field,bool is_internal)
    {
        return false;
    }
public:
    httpd_file(httpd_session &m_session):session(m_session)
    {
        file.file_content_read=file_content_read;
        file.file_field_process_alloc=file_field_process_alloc;
        file.file_field_process_free=file_field_process_free;
        file.usr=this;
    }
    virtual ~httpd_file()
    {

    }
};

class httpd_session
{
    friend class httpd_file;
    hhttp_request_line_t request;                   /**< 请求行 */
    std::map<std::string,std::string> query;        /**< 处理查询字符串 */
    std::map<std::string,std::string> field;        /**< 头部字段 */
    std::string body;                               /**< 主体内容 */
    hhttp_tinyserver_session_t session;
    static int  session_target_match(hhttp_tinyserver_session_t *session,hhttp_request_line_t request_line)
    {
        httpd_session *m_httpd_session=NULL;
        if(session!=NULL && session->usr!=NULL)
        {
            m_httpd_session=(httpd_session *)session->usr;
        }
        int status=HHTTP_STATUS_CODE_404;
        if(m_httpd_session==nullptr)
        {
            /*
             * 返回500
             */
            status=HHTTP_STATUS_CODE_500;
            return status;
        }
        m_httpd_session->request=request_line;
        {
            /*
             * 处理uri
             */
            UriUriA uri;
            const char *errorPos=NULL;
            std::string target_string(request_line.target.str,request_line.target.str_len);
            if(uriParseSingleUriA(&uri, target_string.c_str(), &errorPos) != URI_SUCCESS)
            {
                /*
                 * 返回400
                 */
                status=HHTTP_STATUS_CODE_400;
                return status;
            }
            else
            {
                {
                    /*
                     * 查询字符串
                     */
                    UriQueryListA * queryList=NULL;
                    int itemCount=0;
                    if(uriDissectQueryMallocA(&queryList, &itemCount, uri.query.first,uri.query.afterLast) == URI_SUCCESS)
                    {
                        UriQueryListA * query_item=queryList;
                        while(query_item!=NULL)
                        {
                            m_httpd_session->query[query_item->key]=query_item->value;
                            query_item=query_item->next;
                        }
                        uriFreeQueryListA(queryList);
                    }
                }
                uriSetFragmentA(&uri,NULL,NULL);
                uriSetQueryA(&uri,NULL,NULL);
                uriSetSchemeA(&uri,NULL,NULL);
                uriSetPortTextA(&uri,NULL,NULL);
                uriSetHostAutoA(&uri,NULL,NULL);
                uriSetUserInfoA(&uri,NULL,NULL);
                {
                    std::string path(target_string);
                    int length=0;
                    uriToStringA((char *)path.c_str(),&uri,path.length(),&length);
                    path.resize(length);
                    hfprintf(stderr,"[httpd] Path: %s!\r\n",path.c_str());
                }
                uriFreeUriMembersA(&uri);
            }

        }

        return status;
    }
    static void session_process_field(hhttp_tinyserver_session_t *session,hhttp_field_t field)
    {
        httpd_session *m_httpd_session=NULL;
        if(session!=NULL && session->usr!=NULL)
        {
            m_httpd_session=(httpd_session *)session->usr;
        }
        if(m_httpd_session==nullptr)
        {
            return;
        }
        if(hhttp_string_is_empty(field.field_name) || hhttp_string_is_empty(field.field_values) )
        {
            return;
        }


        /*
         * 头部字段存储在map中
         */
        m_httpd_session->field[std::string(field.field_name.str,field.field_name.str_len)]=std::string(field.field_values.str,field.field_values.str_len);
    }
    static void session_process_body(hhttp_tinyserver_session_t *session,uint64_t offset,hhttp_string_t part_data)
    {
        httpd_session *m_httpd_session=NULL;
        if(session!=NULL && session->usr!=NULL)
        {
            m_httpd_session=(httpd_session *)session->usr;
        }
        if(m_httpd_session==nullptr)
        {
            return;
        }

        if(hhttp_string_is_empty(part_data))
        {
            return;
        }

        /*
         * 默认情况下存储在内存中
         * TODO:处理主体过大问题
         */
        if( m_httpd_session->body.length() >= 64*1024)
        {
            /*
             * 限制请求大小
             */
            return;
        }
        m_httpd_session->body+=std::string(part_data.str,part_data.str_len);
    }
    static void session_process_body_end(hhttp_tinyserver_session_t *session)
    {
        httpd_session *m_httpd_session=NULL;
        if(session!=NULL && session->usr!=NULL)
        {
            m_httpd_session=(httpd_session *)session->usr;
        }
        if(m_httpd_session==nullptr)
        {
            return;
        }
    }
    static hhttp_tinyserver_file_t  *session_target_file_open(hhttp_tinyserver_session_t *session,int http_status_code,hhttp_request_line_t request_line)
    {
        hhttp_tinyserver_file_t *file=NULL;
        httpd_session *m_httpd_session=NULL;
        if(session!=NULL && session->usr!=NULL)
        {
            m_httpd_session=(httpd_session *)session->usr;
        }
        if(m_httpd_session==nullptr)
        {
            return file;
        }

        return file;
    }
    static void session_target_file_close(hhttp_tinyserver_session_t *session,hhttp_tinyserver_file_t *file)
    {
        httpd_session *m_httpd_session=NULL;
        if(session!=NULL && session->usr!=NULL)
        {
            m_httpd_session=(httpd_session *)session->usr;
        }
        if(m_httpd_session==nullptr)
        {
            return;
        }
        if(file!=NULL &&file->usr !=NULL)
        {
            httpd_file *m_httpd_file=(httpd_file *)file->usr;
            delete m_httpd_file;
        }
    }
public:
    hhttp_tinyserver_session_t *hhttp_tinyserver_session(void)
    {
        return &session;
    }
    httpd_session(hhttp_request_line_t request_line)
    {
        request=request_line;
        session.session_target_match=session_target_match;
        session.session_process_field=session_process_field;
        session.session_process_body=session_process_body;
        session.session_process_body_end=session_process_body_end;
        session.session_target_file_open=session_target_file_open;
        session.session_target_file_close=session_target_file_close;
        session.usr=this;

    }
    virtual ~httpd_session()
    {

    }
    hhttp_request_line_t &session_request()
    {
        return request;
    }
    std::map<std::string,std::string> & session_query()
    {
        return query;
    }
    std::map<std::string,std::string> & session_field()
    {
        return field;
    }
    std::string &session_body()
    {
        return body;
    }


};

uint64_t httpd_file::file_content_read(hhttp_tinyserver_file_t *file,uint64_t offset,uint8_t *buffer,uint64_t buffer_size)
{
    uint64_t ret=(uint64_t)-1;
    if(file==NULL || file->usr==NULL)
    {
        return ret;
    }
    httpd_file *m_httpd_file=(httpd_file *)file->usr;
    ret=m_httpd_file->content_read(file,offset,buffer,buffer_size);
    return ret;
}
bool httpd_file::file_field_process_alloc(hhttp_tinyserver_file_t *file,hhttp_field_t *field,bool is_internal)
{
    bool ret=false;
    if(file==NULL || file->usr==NULL)
    {
        return ret;
    }
    httpd_file *m_httpd_file=(httpd_file *)file->usr;
    ret=m_httpd_file->field_process_alloc(file,field,is_internal);
    return ret;
}
bool httpd_file::file_field_process_free(hhttp_tinyserver_file_t *file,hhttp_field_t *field,bool is_internal)
{
    bool ret=false;
    if(file==NULL || file->usr==NULL)
    {
        return ret;
    }
    httpd_file *m_httpd_file=(httpd_file *)file->usr;
    ret=m_httpd_file->field_process_free(file,field,is_internal);
    return ret;
}

static bool default_vhost_match(const hhttp_tinyserver_vhost_t *vhost,hhttp_string_t host)
{
    hfprintf(stderr,"[httpd] Host: %s!\r\n",std::string(host.str,host.str_len).c_str());
    return true;
}

static hhttp_tinyserver_session_t *default_vhost_session_open(const hhttp_tinyserver_vhost_t *vhost,hhttp_request_line_t request_line)
{
    hhttp_tinyserver_session_t * session=NULL;
    hfprintf(stderr,"[httpd] %s %s !\r\n", hhttp_method_std_to_string(request_line.method).str,std::string(request_line.target.str,request_line.target.str_len).c_str());
    httpd_session *m_httpd_session=new httpd_session(request_line);
    if(m_httpd_session!=nullptr)
    {
        session=m_httpd_session->hhttp_tinyserver_session();
    }
    return session;
}

static void default_vhost_session_close(const hhttp_tinyserver_vhost_t *vhost,hhttp_tinyserver_session_t *session)
{
    if(session!=NULL && session->usr!=NULL)
    {
        httpd_session *m_httpd_session=(httpd_session *)session->usr;
        delete m_httpd_session;
    }
}

static hhttp_tinyserver_vhost_t default_vhost=
{
    default_vhost_match,
    default_vhost_session_open,
    default_vhost_session_close,
    NULL
};


static const hhttp_tinyserver_service_t service=
{
    &default_vhost,
    &default_vhost,
    1,
    NULL
};

const hhttp_tinyserver_service_t * httpd_service(void)
{
    return &service;
}



#include "httpd.h"
#include H3RDPARTY_URIPARSER_URI_HEADER
#include "string"
#include "map"
#include "fstream"
#include "filesystem"
#if defined(HDEFAULTS_OS_WINDOWS)
#include "io.h"
#endif

static std::string index_file_name="index.html";

void httpd_set_index_file_name(const char *file_name)
{
    if(file_name==NULL)
    {
        index_file_name="index.html";
    }
    else
    {
        index_file_name=file_name;
    }
}

static std::string root_path=".";
void httpd_set_root_path(const char * path)
{
    if(path!=NULL)
    {
        root_path=path;
    }
}

class httpd_session;
class httpd_file
{
    friend class httpd_session;
    hhttp_tinyserver_file_t file;
    static uint64_t file_content_read(hhttp_tinyserver_file_t *file,uint64_t offset,uint8_t *buffer,uint64_t buffer_size);
    static bool file_field_process_alloc(hhttp_tinyserver_file_t *file,hhttp_field_t *field,bool is_internal);
    static bool file_field_process_free(hhttp_tinyserver_file_t *file,hhttp_field_t *field,bool is_internal);
protected:
    httpd_session &session;
    std::string resp_body;                              /**< 从内存中返回使用此成员 */
    std::map<std::string,std::string> resp_field;       /**< 返回文件需要的头 */
    virtual uint64_t content_read(hhttp_tinyserver_file_t *file,uint64_t offset,uint8_t *buffer,uint64_t buffer_size)
    {
        if(offset < resp_body.length() && (file!=NULL && buffer!=NULL && buffer_size!=0))
        {
            if(buffer_size+offset > resp_body.length())
            {
                buffer_size=resp_body.length()-offset;
            }
            memcpy(buffer,&resp_body.c_str()[offset],buffer_size);
            return buffer_size;
        }
        return (uint64_t)-1;
    }
    virtual bool field_process_alloc(hhttp_tinyserver_file_t *file,hhttp_field_t *field,bool is_internal)
    {
        if(file==NULL || field == NULL )
        {
            return false;
        }

        if(resp_field.size() == 0)
        {
            return false;
        }

        auto it=resp_field.begin();
        if(is_internal)
        {
            std::string name=std::string(field->field_name.str,field->field_name.str_len);
            it=resp_field.find(name);
        }

        if(it==resp_field.end())
        {
            return false;
        }

        field->field_name=hhttp_string_from_cstring(it->first.c_str());
        field->field_values=hhttp_string_from_cstring(it->second.c_str());

        return true;
    }
    virtual bool field_process_free(hhttp_tinyserver_file_t *file,hhttp_field_t *field,bool is_internal)
    {
        if(file==NULL || field == NULL )
        {
            return false;
        }

        if(hhttp_string_is_empty(field->field_name))
        {
            return false;
        }

        std::string name=std::string(field->field_name.str,field->field_name.str_len);
        auto it=resp_field.find(name);
        if(it!=resp_field.end())
        {
            resp_field.erase(it);
        }

        return true;
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

class httpd_file_directory:public httpd_file
{
public:
    httpd_file_directory(httpd_session &m_session,std::string path):httpd_file(m_session)
    {
        /*
         * 重定向至引索文件
         */
        resp_field["Location"]=path+index_file_name;
    }
    virtual ~httpd_file_directory()
    {

    }

    static httpd_file *open(httpd_session &m_session,std::string path,int &status_code)
    {
        httpd_file_directory * m_file=new httpd_file_directory(m_session,path);
        status_code=HHTTP_STATUS_CODE_302;
        return m_file;
    }
};

class httpd_file_error:public httpd_file
{

public:
    httpd_file_error(httpd_session &m_session,std::string path):httpd_file(m_session)
    {
        /*
         * 纯文本输出
         */
        resp_field["Content-Type"]="text/plain";
    }
    virtual ~httpd_file_error()
    {

    }

    static httpd_file *open(httpd_session &m_session,std::string path,int &status_code)
    {
        httpd_file_error * m_file=new httpd_file_error(m_session,path);
        m_file->resp_body += path+" ";
        m_file->resp_body += std::string(hhttp_status_code_std_to_string((hhttp_status_code_t)status_code).str)+" "+std::string(hhttp_status_code_std_to_reason_string((hhttp_status_code_t)status_code).str);
        return m_file;
    }
};


class httpd_file_regular:public httpd_file
{
    std::fstream file_regular;
protected:
    virtual uint64_t content_read(hhttp_tinyserver_file_t *file,uint64_t offset,uint8_t *buffer,uint64_t buffer_size)
    {
        if(file_regular.is_open())
        {
            if(file_regular.eof())
            {
                return (uint64_t)-1;
            }
            file_regular.read((char *)buffer,buffer_size);
            int readlen=file_regular.gcount();
            if(readlen > 0)
            {
                return readlen;
            }
            else
            {
                return (uint64_t)-1;
            }
        }
        return httpd_file::content_read(file,offset,buffer,buffer_size);
    }
public:
    /*
     * 注意:必须path传入完整路径
     */
    httpd_file_regular(httpd_session &m_session,std::string path):httpd_file(m_session)
    {
        /*
         * 默认二进制输出,需要设置正确的类型才能正常显示
         */
        resp_field["Content-Type"]="application/octet-stream";
        if(path.length() > (4+1))
        {
            std::string path_ext_4=path.substr(path.length()-(4+1));
            for(size_t i=0; i<path_ext_4.length(); i++)
            {
                if(path_ext_4.c_str()[i] >= 'A' && path_ext_4.c_str()[i] <='Z')
                {
                    (*(char *)&path_ext_4.c_str()[i])=path_ext_4.c_str()[i]-'A'+'a';
                }
            }

            if(path_ext_4 == ".html")
            {
                resp_field["Content-Type"]="text/html";
            }

            if(path_ext_4 == ".json")
            {
                resp_field["Content-Type"]="application/json";
            }

            if(path_ext_4 == ".jpeg")
            {
                resp_field["Content-Type"]="image/jpeg";
            }

            if(path_ext_4 == ".webp")
            {
                resp_field["Content-Type"]="image/webp";
            }

            if(path_ext_4 == ".avif")
            {
                resp_field["Content-Type"]="image/avif";
            }
        }
        if(path.length() > (3+1))
        {
            std::string path_ext_3=path.substr(path.length()-(3+1));
            for(size_t i=0; i<path_ext_3.length(); i++)
            {
                if(path_ext_3.c_str()[i] >= 'A' && path_ext_3.c_str()[i] <='Z')
                {
                    (*(char *)&path_ext_3.c_str()[i])=path_ext_3.c_str()[i]-'A'+'a';
                }
            }

            if(path_ext_3 == ".htm")
            {
                resp_field["Content-Type"]="text/html";
            }

            if(path_ext_3 == ".css")
            {
                resp_field["Content-Type"]="text/css";
            }

            if(path_ext_3 == ".xml")
            {
                resp_field["Content-Type"]="text/xml";
            }

            if(path_ext_3 == ".jpg")
            {
                resp_field["Content-Type"]="image/jpeg";
            }

            if(path_ext_3 == ".png")
            {
                resp_field["Content-Type"]="image/png";
            }

            if(path_ext_3 == ".gif")
            {
                resp_field["Content-Type"]="image/gif";
            }

            if(path_ext_3 == ".svg")
            {
                resp_field["Content-Type"]="image/svg+xml";
            }

            if(path_ext_3 == ".bmp")
            {
                resp_field["Content-Type"]="image/bmp";
            }

            if(path_ext_3 == ".ico")
            {
                resp_field["Content-Type"]="image/x-icon";
            }

        }

        if(path.length() > (2+1))
        {
            std::string path_ext_2=path.substr(path.length()-(2+1));
            for(size_t i=0; i<path_ext_2.length(); i++)
            {
                if(path_ext_2.c_str()[i] >= 'A' && path_ext_2.c_str()[i] <='Z')
                {
                    (*(char *)&path_ext_2.c_str()[i])=path_ext_2.c_str()[i]-'A'+'a';
                }
            }

            if(path_ext_2 == ".js")
            {
                resp_field["Content-Type"]="application/javascript";
            }

        }
    }
    virtual ~httpd_file_regular()
    {
        if(file_regular.is_open())
        {
            file_regular.close();
        }
    }

    /*
     * 注意:必须path传入完整路径
     */
    static httpd_file *open(httpd_session &m_session,std::string path,int &status_code)
    {
        httpd_file_regular * m_file=new httpd_file_regular(m_session,path);
        m_file->file_regular.open((path).c_str(),std::ios_base::in |std::ios_base::binary);
        if(!m_file->file_regular.is_open())
        {
            status_code=HHTTP_STATUS_CODE_404;
        }
        else
        {
            status_code=HHTTP_STATUS_CODE_200;
        }
        return m_file;
    }
};

class httpd_session
{
    friend class httpd_file;
    hhttp_request_line_t request;                   /**< 请求行 */
    std::map<std::string,std::string> query;        /**< 处理查询字符串 */
    std::map<std::string,std::string> field;        /**< 头部字段 */
    std::string body;                               /**< 主体内容 */
    httpd_file *resp_file;
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
        std::string path;
        if(request_line.method==HHTTP_METHOD_GET || request_line.method== HHTTP_METHOD_POST)
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
                int dir_depth=0;
                {
                    /*
                     * 检查目录深度，防止通过..逃逸
                     */
                    UriPathSegmentA *seg=uri.pathHead;
                    while(seg!=NULL)
                    {
                        dir_depth++;
                        if(seg->text.first!=NULL && seg->text.afterLast != seg->text.first)
                        {
                            std::string name(seg->text.first,seg->text.afterLast-seg->text.first);
                            if(name==".")
                            {
                                dir_depth-=1;
                            }
                            if(name=="..")
                            {
                                dir_depth-=2;
                            }
                        }
                        seg=seg->next;
                    }
                }
                if(dir_depth >= 0)
                {
                    path+=target_string+std::string("dummy");
                    int length=0;
                    uriToStringA((char *)path.c_str(),&uri,path.length(),&length);
                    path.resize(strlen(path.c_str()));
                    {
                        /*
                         * 去除%组,用于支持中文
                         */
                        uriUnescapeInPlaceA((char *)path.c_str());
                        path.resize(hstrlen(path.c_str()));
                    }
                    hfprintf(stderr,"[httpd] Path: %s!\r\n",path.c_str());
                    if(path.c_str()[path.length()-1]=='/')
                    {
                        /*
                         * 目录
                         */
                        m_httpd_session->resp_file=httpd_file_directory::open(*m_httpd_session,path,status);
                    }
                    else
                    {
                        /*
                         * 文件
                         */
                        std::string full_path=root_path+path;
#if !defined(HDEFAULTS_OS_WINDOWS)
                        if(0==access(full_path.c_str(),0))
#else
                        if(0==_access(full_path.c_str(),0))
#endif
                        {
                            if(std::filesystem::is_directory(std::filesystem::path(full_path)))
                            {
                                /*
                                 * 目录
                                 */
                                m_httpd_session->resp_file=httpd_file_directory::open(*m_httpd_session,path+"/",status);
                            }
                            else
#if !defined(HDEFAULTS_OS_WINDOWS)
                                if(0==access(full_path.c_str(),R_OK))
#else
                                if(0==_access(full_path.c_str(),4))
#endif
                                {
                                    /*
                                     * 文件
                                     */
                                    m_httpd_session->resp_file=httpd_file_regular::open(*m_httpd_session,full_path,status);
                                }
                                else
                                {
                                    status=HHTTP_STATUS_CODE_403;
                                }

                        }
                        else
                        {
                            status=HHTTP_STATUS_CODE_404;
                        }
                    }
                }
                else
                {
                    status=HHTTP_STATUS_CODE_403;
                }
                uriFreeUriMembersA(&uri);
            }

        }
        else
        {
            status=HHTTP_STATUS_CODE_501;
        }

        if(m_httpd_session->resp_file==NULL && status >= HHTTP_STATUS_CODE_400)
        {
            m_httpd_session->resp_file=httpd_file_error::open(*m_httpd_session,path,status);
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

        if(m_httpd_session->resp_file!=nullptr)
        {
            file=&m_httpd_session->resp_file->file;
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
            m_httpd_session->resp_file=nullptr;
        }
    }
public:
    hhttp_tinyserver_session_t *hhttp_tinyserver_session(void)
    {
        return &session;
    }
    httpd_session(hhttp_request_line_t request_line):resp_file(nullptr)
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



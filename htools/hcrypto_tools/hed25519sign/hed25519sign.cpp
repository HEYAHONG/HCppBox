#include "HCPPBox.h"
#include "hrc.h"
#include "string"
#include "fstream"
#include "stdarg.h"
#include H3RDPARTY_ARGTABLE3_HEADER


static bool be_quiet=false;
static int  local_fprintf(FILE *fp,const char *format,...)
{
    if(fp==NULL)
    {
        fp=stdout;
    }

    if(be_quiet && (fp==stdout))
    {
        /*
         * 不向stdout输出
         */
        return 0;
    }

    int ret=0;
    va_list va;
    va_start(va,format);
    ret=hvfprintf(fp,format,va);
    va_end(va);
    return ret;
}

enum
{
    CMD_VERIFY      =   1,
    CMD_SIGN        =   2,
    CMD_FINGERPRINT =   3,
    CMD_GENERATE    =   4,
} cmd;
static std::string comment;
static std::string message_file;
static std::string publickey_file;
static std::string secretkey_file;
static std::string sign_file;
static void check_args(int argc,char *argv[])
{
    /*
     * 帮助
     */
    struct arg_lit * help=NULL;
    /*
     * 命令
     */
    struct arg_lit * cmd_verify=NULL;
    struct arg_lit * cmd_sign=NULL;
    struct arg_lit * cmd_fingerprint=NULL;
    struct arg_lit * cmd_generate=NULL;
    /*
     * 选项
     */
    struct arg_str * opt_comment=NULL;
    struct arg_file* opt_message_file=NULL;
    struct arg_file* opt_public_key_file=NULL;
    struct arg_file* opt_secret_key_file=NULL;
    struct arg_file* opt_sign_file=NULL;
    struct arg_lit * opt_quiet=NULL;
    /*
     * 参数结束
     */
    struct arg_end * e=NULL;
    void *argtable[]=
    {
        help=               arg_lit0("h","help",                           "print this help and exit"),
        arg_rem("\r\nCommands:",                                  ""),
        cmd_verify=         arg_lit0("V","verify",                         "verify"),
        cmd_sign=           arg_lit0("S","sign",                           "sign"),
        cmd_fingerprint=    arg_lit0("F","fingerprint",                    "print key fingerprint of public/secret key or signature"),
        cmd_generate=       arg_lit0("G","generate",                       "generate a new keypair"),
        arg_rem("\r\nOptions:",                        ""),
        opt_comment=        arg_str0("c","comment","<string>",             "add comment to keys"),
        opt_message_file=   arg_file0("m","message","<file>",              "message file"),
        opt_public_key_file=arg_file0("p","publickey","<file>",            "public key file (verify/fingerprint only)"),
        opt_secret_key_file=arg_file0("s","secretkey","<file>",            "secret key file (sign/fingerprint only)"),
        opt_sign_file=      arg_file0("x","signfile","<file>",             "signature file (defaults to <message file>.sig)"),
        opt_quiet=          arg_lit0("q","quiet",                          "quiet"),
        e=arg_end(20)
    };
    if(arg_nullcheck(argtable)!=0)
    {
        hfputs("arg_nullcheck error!\r\n",stderr);
        hexit(-1);
    }

    if(arg_parse(argc,argv,argtable)>0)
    {
        hfputs("arg_parse error!\r\n",stderr);
        hfputs("Usage:\r\n",stderr);
        arg_print_glossary(stderr,argtable,"  %-25s %s\n");
        hexit(-1);
    }

    if(help->count > 0)
    {
        const char *banner="";
        if(RCGetHandle("banner")!=NULL)
        {
            banner=(const char *)RCGetHandle("banner");
        }
        local_fprintf(stdout,"%s\r\nUsage: %s [Commands] [Options]\r\n",banner,argv[0]);
        arg_print_glossary(stdout,argtable,"  %-25s %s\n");
        hexit(0);
    }

    if((cmd_verify->count + cmd_sign->count + cmd_fingerprint->count + cmd_generate->count) != 1)
    {
        /*
         * 只能指定一个命令
         */
        local_fprintf(stderr,"must specify one command!\r\nUsage: %s [Commands] [Options]\r\n",argv[0]);
        arg_print_glossary(stderr,argtable,"  %-25s %s\n");
        hexit(-1);
    }

    if(cmd_verify->count > 0 && (opt_public_key_file->count == 0 || opt_message_file->count == 0))
    {
        local_fprintf(stderr,"verify need  at least message file and publickey file\r\n");
        hexit(-1);
    }

    if(cmd_sign->count > 0 && (opt_secret_key_file->count == 0 || opt_message_file->count == 0))
    {
        local_fprintf(stderr,"sign need  at least message file and secretkey file\r\n");
        hexit(-1);
    }

    if(cmd_fingerprint->count > 0 && ((opt_secret_key_file->count + opt_public_key_file->count + opt_sign_file->count)==0))
    {
        local_fprintf(stderr,"fingerprint need  secretkey file  or secretkey file or sign file\r\n");
        hexit(-1);
    }

    if(cmd_generate->count > 0 && (opt_secret_key_file->count == 0 || opt_public_key_file->count == 0))
    {
        local_fprintf(stderr,"sign need  at least publickey file and secretkey file\r\n");
        hexit(-1);
    }

    if(cmd_verify->count > 0)
    {
        cmd=CMD_VERIFY;
    }

    if(cmd_sign->count > 0)
    {
        cmd=CMD_SIGN;
    }

    if(cmd_fingerprint->count > 0)
    {
        cmd=CMD_FINGERPRINT;
    }

    if(cmd_generate->count > 0)
    {
        cmd=CMD_GENERATE;
    }

    if(opt_comment->count > 0)
    {
        if(opt_comment->sval[0]!=NULL)
        {
            comment=opt_comment->sval[0];
        }
    }

    if(opt_message_file->count > 0)
    {
        if(opt_message_file->filename[0]!=NULL)
        {
            message_file=opt_message_file->filename[0];
        }
    }

    if(opt_public_key_file->count > 0)
    {
        if(opt_public_key_file->filename[0]!=NULL)
        {
            publickey_file=opt_public_key_file->filename[0];
        }
    }

    if(opt_secret_key_file->count > 0)
    {
        if(opt_secret_key_file->filename[0]!=NULL)
        {
            secretkey_file=opt_secret_key_file->filename[0];
        }
    }

    if(opt_sign_file->count > 0)
    {
        if(opt_sign_file->filename[0]!=NULL)
        {
            sign_file=opt_sign_file->filename[0];
        }
    }


    if(opt_quiet->count > 0)
    {
        be_quiet=true;
    }

    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));

    if(!message_file.empty() && sign_file.empty())
    {
        /*
         * 默认签名文件名为消息文件名加后缀.sig
         */
        sign_file=message_file+".sig";
    }
}


static bool check_file_exist(std::string filename)
{
    bool ret=false;
    std::fstream file(filename.c_str(),std::ios::in);
    if(file.is_open())
    {
        file.close();
        ret=true;
    }
    return ret;
}


/*
 * usign文件说明，第一行为注释，第二行为数据的base64编码
 */


/** \brief 读取usign文件
 *
 * \param filename std::string 文件名
 * \param data std::string& 读取后的数据
 * \return bool 是否读取成功
 *
 */
static bool load_usign_base64_file(std::string filename,std::string &data)
{
    bool ret=false;
    if(&data == nullptr)
    {
        return ret;
    }
    std::fstream file(filename.c_str(),std::ios::in);
    if(file.is_open())
    {
        {
            /*
             * 获取注释
             */
            std::string  comment_in_file;
            std::getline(file,comment_in_file);
        }

        {
            /*
             * 获取base64编码的数据
             */
            std::string data_in_file;
            std::getline(file,data_in_file);

            if(data_in_file.length() > 0)
            {
                uint8_t * byte_data=new uint8_t[data_in_file.length()+32];
                size_t  byte_data_length=hbase64_decode(byte_data,data_in_file.length()+32,data_in_file.c_str(),data_in_file.length());
                if(byte_data != nullptr && byte_data_length > 0)
                {
                    data=std::string((char *)byte_data,byte_data_length);
                    ret=true;
                }
                delete [] byte_data;
            }
        }

        file.close();
    }
    return ret;
}


/** \brief 写入usign文件
 *
 * \param filename std::string 文件名
 * \param data std::string 写入后的数据
 * \return bool 是否写入成功
 *
 */
static bool save_usign_base64_file(std::string filename,std::string data)
{
    bool ret=false;
    std::fstream file(filename.c_str(),std::ios::out | std::ios::trunc);
    if(file.is_open())
    {
        {
            /*
             * 写入注释
             */
            file << "untrusted comment:" ;
            if(comment.length() > 0)
            {
                file << comment;
            }
            else
            {
                file << "saved by ed25519sign";
            }

            file << "\n";
        }

        {
            /*
             * 写入数据
             */
            char * base64_data=new char[data.length()*4/3+32];
            size_t base64_data_length=hbase64_encode_with_null_terminator(base64_data,data.length()*4/3+32,(uint8_t *)data.c_str(),data.length());
            if(base64_data != nullptr && base64_data_length > 0)
            {
                file << base64_data << "\n";
            }
            delete [] base64_data;
        }
        file.close();
        ret=true;
    }

    return ret;
}

static int process_cmd_verify(int argc,char *argv[])
{
    hed25519_public_key_file_struct_t publickey = {0};
    {
        /*
         * 公钥
         */
        bool isok=true;
        std::string data;
        load_usign_base64_file(publickey_file,data);
        if(data.length() >= sizeof(publickey))
        {
            memcpy(&publickey,data.c_str(),sizeof(publickey));
        }
        else
        {
            isok=false;
        }

        if(isok)
        {
            if(publickey.pkalg[0] != 'E' || publickey.pkalg[1] != 'd')
            {
                isok=false;
            }
        }

        if(!isok)
        {
            local_fprintf(stderr,"load publickey failed!\r\n");
            hexit(-1);
        }
    }
    hed25519_sign_file_struct_t       sign= {0};
    {
        /*
         * 签名文件
         */
        bool isok=true;
        std::string data;
        load_usign_base64_file(sign_file,data);
        if(data.length() >= sizeof(sign))
        {
            memcpy(&sign,data.c_str(),sizeof(sign));
        }
        else
        {
            isok=false;
        }

        if(isok)
        {
            if(sign.pkalg[0] != 'E' || sign.pkalg[1] != 'd')
            {
                isok=false;
            }
        }

        if(!isok)
        {
            local_fprintf(stderr,"load sign failed!\r\n");
            hexit(-1);
        }
    }

    if(memcmp(publickey.fingerprint,sign.fingerprint,sizeof(sign.fingerprint))!=0)
    {
        local_fprintf(stderr,"fingerprint mismatch!\r\n");
    }

    hed25519_edsign_verify_state_t verify_state = {0};
    hed25519_edsign_verify_init(&verify_state,NULL,sign.sig,publickey.pubkey);
    {
        std::fstream file(message_file.c_str(),std::ios::in | std::ios::binary);
        if(file.is_open())
        {
            uint8_t buffer[4096];
            while(!file.eof())
            {
                file.read((char *)buffer,sizeof(buffer));
                size_t bytes_read=file.gcount();
                if(bytes_read > 0)
                {
                    hed25519_edsign_verify_add(&verify_state,buffer,bytes_read);
                }
            }
            file.close();
        }
    }
    if(hed25519_edsign_verify(&verify_state,sign.sig,publickey.pubkey))
    {
        local_fprintf(stdout,"%s verify ok!\r\n",message_file.c_str());
    }
    else
    {
        local_fprintf(stderr,"%s verify failed!\r\n",message_file.c_str());
        hexit(1);
    }
    return 0;
}

static int process_cmd_sign(int argc,char *argv[])
{
    hed25519_private_key_file_struct_t secretkey = {0};
    {
        /*
         * 私钥
         */
        bool isok=true;
        std::string data;
        load_usign_base64_file(secretkey_file,data);
        if(data.length() >= sizeof(secretkey))
        {
            memcpy(&secretkey,data.c_str(),sizeof(secretkey));
        }
        else
        {
            isok=false;
        }

        if(isok)
        {
            if(secretkey.pkalg[0] != 'E' || secretkey.pkalg[1] != 'd')
            {
                isok=false;
            }
        }

        if(isok)
        {
            /*
             * 不支持私钥加密
             */
            if(secretkey.kdfalg[0] != 'B' || secretkey.kdfalg[1] != 'K' || secretkey.kdfrounds!=0)
            {
                isok=false;
            }
        }

        if(isok)
        {
            /*
             * 检查校验
             */
            hed25519_sha512_t checksum;
            {
                hed25519_sha512_context_t ctx = {0};
                hed25519_sha512_starts(NULL,&ctx);
                hed25519_sha512_update(NULL,&ctx,secretkey.seckey,sizeof(secretkey.seckey));
                hed25519_sha512_finish(NULL,&ctx,checksum);
            }
            if(memcmp(secretkey.checksum,checksum,sizeof(secretkey.checksum))!=0)
            {
                isok=false;
            }
        }

        if(!isok)
        {
            local_fprintf(stderr,"load secretkey failed!\r\n");
            hexit(-1);
        }
    }

    hed25519_sign_file_struct_t sign = {0};
    memcpy(sign.pkalg,secretkey.pkalg,sizeof(sign.pkalg));
    memcpy(sign.fingerprint,secretkey.fingerprint,sizeof(sign.fingerprint));

    hed25519_edsign_public_key_t pubkey;
    hed25519_edsign_sec_to_pub(NULL,pubkey,secretkey.seckey);

    {
        std::fstream file(message_file.c_str(),std::ios::in | std::ios::binary);
        if(file.is_open())
        {
            hed25519_edsign_sign2(sign.sig,NULL,pubkey,secretkey.seckey,[](void *usr,size_t offset,void *buffer,size_t buffer_length)->int
            {
                if(usr == nullptr)
                {
                    return 0;
                }
                std::fstream &file=*(std::fstream *)usr;
                int old_offset=file.tellg();
                {
                    if(old_offset == -1 || !file.is_open())
                    {
                        /*
                         * 重新打开
                         */
                        file.close();
                        file.open(message_file.c_str(),std::ios::in | std::ios::binary);
                    }
                }
                file.seekg(offset);
                file.read((char *)buffer,buffer_length);
                size_t ret=file.gcount();
                return ret;
            },&file);
            file.close();
        }
        else
        {
            local_fprintf(stderr,"load message file failed!\r\n");
            hexit(1);
        }
    }

    if(!save_usign_base64_file(sign_file,std::string((char *)&sign,sizeof(sign))))
    {
        local_fprintf(stderr,"save sign file failed!\r\n");
        hexit(1);
    }
    else
    {
        local_fprintf(stderr,"sign %s ok!\r\n",message_file.c_str());
    }

    return 0;
}

static int process_cmd_fingerprint(int argc,char *argv[])
{
    {
        hed25519_public_key_file_struct_t publickey = {0};
        {
            /*
             * 公钥
             */
            bool isok=true;
            std::string data;
            load_usign_base64_file(publickey_file,data);
            if(data.length() >= sizeof(publickey))
            {
                memcpy(&publickey,data.c_str(),sizeof(publickey));
            }
            else
            {
                isok=false;
            }

            if(isok)
            {
                if(publickey.pkalg[0] != 'E' || publickey.pkalg[1] != 'd')
                {
                    isok=false;
                }
            }

            if(isok)
            {
                char buffer[64]= {0};
                hbase16_encode(buffer,sizeof(buffer),publickey.fingerprint,sizeof(publickey.fingerprint));
                local_fprintf(stdout,"public key fingerprint: %s \r\n",buffer);
            }
        }
    }
    {
        hed25519_private_key_file_struct_t secretkey = {0};
        {
            /*
             * 私钥
             */
            bool isok=true;
            std::string data;
            load_usign_base64_file(secretkey_file,data);
            if(data.length() >= sizeof(secretkey))
            {
                memcpy(&secretkey,data.c_str(),sizeof(secretkey));
            }
            else
            {
                isok=false;
            }

            if(isok)
            {
                if(secretkey.pkalg[0] != 'E' || secretkey.pkalg[1] != 'd')
                {
                    isok=false;
                }
            }

            if(isok)
            {
                /*
                 * 不支持私钥加密
                 */
                if(secretkey.kdfalg[0] != 'B' || secretkey.kdfalg[1] != 'K' || secretkey.kdfrounds!=0)
                {
                    isok=false;
                }
            }

            if(isok)
            {
                /*
                 * 检查校验
                 */
                hed25519_sha512_t checksum;
                {
                    hed25519_sha512_context_t ctx = {0};
                    hed25519_sha512_starts(NULL,&ctx);
                    hed25519_sha512_update(NULL,&ctx,secretkey.seckey,sizeof(secretkey.seckey));
                    hed25519_sha512_finish(NULL,&ctx,checksum);
                }
                if(memcmp(secretkey.checksum,checksum,sizeof(secretkey.checksum))!=0)
                {
                    isok=false;
                }
            }

            if(isok)
            {
                char buffer[64]= {0};
                hbase16_encode(buffer,sizeof(buffer),secretkey.fingerprint,sizeof(secretkey.fingerprint));
                local_fprintf(stdout,"secret key fingerprint: %s \r\n",buffer);
            }
        }
    }
    {
        hed25519_sign_file_struct_t       sign= {0};
        {
            /*
             * 签名文件
             */
            bool isok=true;
            std::string data;
            load_usign_base64_file(sign_file,data);
            if(data.length() >= sizeof(sign))
            {
                memcpy(&sign,data.c_str(),sizeof(sign));
            }
            else
            {
                isok=false;
            }

            if(isok)
            {
                if(sign.pkalg[0] != 'E' || sign.pkalg[1] != 'd')
                {
                    isok=false;
                }
            }

            if(isok)
            {
                char buffer[64]= {0};
                hbase16_encode(buffer,sizeof(buffer),sign.fingerprint,sizeof(sign.fingerprint));
                local_fprintf(stdout,"sign       fingerprint: %s \r\n",buffer);
            }
        }
    }
    return 0;
}

static int process_cmd_generate(int argc,char *argv[])
{
    hed25519_private_key_file_struct_t secretkey = {0};
    /*
     * 使用随机数填充所有字段
     */
    HED25519_GETRANDOM((uint8_t *)&secretkey,sizeof(secretkey),0);


    /*
     * 密钥算法
     */
    secretkey.pkalg[0]='E';
    secretkey.pkalg[1]='d';

    /*
     * 密钥加密设置（无加密）
     */
    secretkey.kdfalg[0]='B';
    secretkey.kdfalg[1]='K';
    secretkey.kdfrounds=0;

    /*
     *私钥生成
     */
    hed25519_edsign_sec_generate(&secretkey.seckey[0]);

    /*
     * 将私钥高32字节设置为公钥
     */
    hed25519_edsign_sec_to_pub(NULL,&secretkey.seckey[32],&secretkey.seckey[0]);


    /*
     * 生成校验
     */
    {
        hed25519_sha512_t checksum;
        {
            hed25519_sha512_context_t ctx = {0};
            hed25519_sha512_starts(NULL,&ctx);
            hed25519_sha512_update(NULL,&ctx,secretkey.seckey,sizeof(secretkey.seckey));
            hed25519_sha512_finish(NULL,&ctx,checksum);
        }
        memcpy(secretkey.checksum,checksum,sizeof(secretkey.checksum));
    }

    /*
     * 写入私钥文件
     */
    if(!save_usign_base64_file(secretkey_file,std::string((char *)&secretkey,sizeof(secretkey))))
    {
        local_fprintf(stderr,"save secretkey file failed!\r\n");
        hexit(1);
    }

    hed25519_public_key_file_struct_t publickey = {0};
    memcpy(publickey.pkalg,secretkey.pkalg,sizeof(publickey.pkalg));
    memcpy(publickey.fingerprint,secretkey.fingerprint,sizeof(publickey.fingerprint));
    hed25519_edsign_sec_to_pub(NULL,publickey.pubkey,&secretkey.seckey[0]);

    /*
     * 写入公钥文件
     */
    if(!save_usign_base64_file(publickey_file,std::string((char *)&publickey,sizeof(publickey))))
    {
        local_fprintf(stderr,"save publickey file failed!\r\n");
        hexit(1);
    }


    return 0;
}

static int process_cmd(int argc,char *argv[])
{
    switch(cmd)
    {
    case CMD_VERIFY:
    {
        if(!check_file_exist(message_file))
        {
            local_fprintf(stderr,"can't open %s!\r\n",message_file.c_str());
            hexit(-1);
        }
        if(!check_file_exist(publickey_file))
        {
            local_fprintf(stderr,"can't open %s!\r\n",publickey_file.c_str());
            hexit(-1);
        }
        if(!check_file_exist(sign_file))
        {
            local_fprintf(stderr,"can't open %s!\r\n",sign_file.c_str());
            hexit(-1);
        }

        process_cmd_verify(argc,argv);
    }
    break;
    case CMD_SIGN:
    {
        if(!check_file_exist(message_file))
        {
            local_fprintf(stderr,"can't open %s!\r\n",message_file.c_str());
            hexit(-1);
        }
        if(!check_file_exist(secretkey_file))
        {
            local_fprintf(stderr,"can't open %s!\r\n",secretkey_file.c_str());
            hexit(-1);
        }

        process_cmd_sign(argc,argv);
    }
    break;
    case CMD_FINGERPRINT:
    {
        process_cmd_fingerprint(argc,argv);
    }
    break;
    case CMD_GENERATE:
    {
        process_cmd_generate(argc,argv);
    }
    break;
    default:
    {
        local_fprintf(stderr,"unkonwn command!\r\n");
        hexit(-1);
    }
    break;
    }
    return 0;
}

int main(int argc,char *argv[])
{
    check_args(argc,argv);

    process_cmd(argc,argv);

    return 0;
}

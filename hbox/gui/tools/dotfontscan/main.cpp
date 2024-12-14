#include "HCPPBox.h"
#include "hbox.h"
#include "hrc.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include "stdlib.h"
#include "stdio.h"
#include <string>
#include <codecvt>
#include <iostream>
#include <locale>
#include <set>
#include <fstream>
#include <algorithm>


static class ft_lib
{
    FT_Library library;
public:
    operator FT_Library()
    {
        return library;
    }
    ft_lib():library(NULL)
    {
        FT_Init_FreeType(&library);
    }
    ~ft_lib()
    {
        FT_Done_FreeType(library);
    }
} g_ft_lib;


static int cmd_help(int argc,const char *argv[]);
static int cmd_font(int argc,const char *argv[]);
static int cmd_input_file(int argc,const char *argv[]);
static int cmd_output_file(int argc,const char *argv[]);
static struct
{
    const char * cmd;
    const char * cmd_short;
    int (*cmd_entry)(int argc,const char *argv[]);
    const char * usage;
    const char * help;
} cmd_list[]=
{
    {
        "--help",
        "-h",
        cmd_help,
        "--help  / -h ",
        "get  help"
    },
    {
        "--font",
        "-f",
        cmd_font,
        "--font=[font_file_path]  / -f [font_file_path] ",
        "fontfile path"
    },
    {
        "--input",
        "-i",
        cmd_input_file,
        "--input=[input_file_path]  / -i [input_file_path] ",
        "input file(utf-8) path"
    },
    {
        "--output",
        "-o",
        cmd_output_file,
        "--output=[output_file_path]  / -o [output_file_path] ",
        "output file(C file) path"
    }
};

static int cmd_help(int argc,const char *argv[])
{
    const size_t cmd_max_len=16;
    const size_t usage_max_len=50;
    {
        //打印banner
        const char *banner=(const char *)RCGetHandle("banner");
        if(banner!=NULL)
        {
            printf("%s",banner);
        }
    }
    {
        printf("\r\n%s [options]\r\n",argv[0]);
    }
    {
        //打印标题栏
        for(size_t i=0; i<cmd_max_len; i++)
        {
            printf("%s","-");
        }
        printf("\t");
        for(size_t i=0; i<usage_max_len; i++)
        {
            printf("%s","-");
        }
        printf("\r\n");
    }
    {
        for(size_t i=0; i<sizeof(cmd_list)/sizeof(cmd_list[0]); i++)
        {
            {
                //打印cmd
                printf("%s",cmd_list[i].cmd);
                int cmd_pad_len=(int)cmd_max_len-strlen(cmd_list[i].cmd);
                if(cmd_pad_len > 0)
                {
                    for(size_t i=0; i<cmd_pad_len; i++)
                    {
                        printf(" ");
                    }
                }
            }
            printf("\t");
            {
                //打印usage
                printf("%s",cmd_list[i].usage);
                int usage_pad_len=(int)usage_max_len-strlen(cmd_list[i].usage);
                if(usage_pad_len > 0)
                {
                    for(size_t i=0; i<usage_pad_len; i++)
                    {
                        printf(" ");
                    }
                }
            }
            printf("\t");
            {
                printf("%s",cmd_list[i].help);
            }
            printf("\r\n");
        }
    }
    //退出程序
    exit(0);
    return 0;
}

#ifdef WIN32
static std::string get_system_root()
{
    std::string ret("C:/Windows");
    {
        const char *systemroot=getenv("SystemRoot");
        if(systemroot!=NULL)
        {
            ret=systemroot;
            std::replace(ret.begin(),ret.end(),'\\','/');
        }
    }
    return ret;
}
std::string font_file_path(get_system_root()+"/Fonts/simsun.ttc");
#else
//默认使用 文泉驿 字体,debian系统(最新版)使用sudo apt-get install fonts-wqy-zenhei安装后即可使用
std::string font_file_path("/usr/share/fonts/truetype/wqy/wqy-zenhei.ttc");
#endif
static int cmd_font(int argc,const char *argv[])
{
    for(int i=0; i<argc; i++)
    {
        {
            char temp[4096]= {0};
            const char *para=NULL;
            strcat(temp,argv[i]);
            for(size_t k=0; k<strlen(temp); k++)
            {
                if(temp[k]=='=')
                {
                    temp[k]='\0';
                    para=&temp[k+1];
                    break;
                }
            }
            if(strcmp("--font",temp)==0)
            {
                if(para!=NULL)
                {
                    font_file_path=para;
                    break;
                }
            }
            if(strcmp("-f",argv[i])==0)
            {
                if((i+1)<argc)
                {
                    font_file_path=argv[i+1];
                    break;
                }
            }
        }
    }

    return 0;
}

static std::string input_file_path("font.map");
static int cmd_input_file(int argc,const char *argv[])
{
    for(int i=0; i<argc; i++)
    {
        {
            char temp[4096]= {0};
            const char *para=NULL;
            strcat(temp,argv[i]);
            for(size_t k=0; k<strlen(temp); k++)
            {
                if(temp[k]=='=')
                {
                    temp[k]='\0';
                    para=&temp[k+1];
                    break;
                }
            }
            if(strcmp("--input",temp)==0)
            {
                if(para!=NULL)
                {
                    input_file_path=para;
                    break;
                }
            }
            if(strcmp("-i",argv[i])==0)
            {
                if((i+1)<argc)
                {
                    input_file_path=argv[i+1];
                    break;
                }
            }
        }
    }
    return 0;
}

static std::string output_file_path("hdotfont.c");
static int cmd_output_file(int argc,const char *argv[])
{
    for(int i=0; i<argc; i++)
    {
        {
            char temp[4096]= {0};
            const char *para=NULL;
            strcat(temp,argv[i]);
            for(size_t k=0; k<strlen(temp); k++)
            {
                if(temp[k]=='=')
                {
                    temp[k]='\0';
                    para=&temp[k+1];
                    break;
                }
            }
            if(strcmp("--output",temp)==0)
            {
                if(para!=NULL)
                {
                    output_file_path=para;
                    break;
                }
            }
            if(strcmp("-o",argv[i])==0)
            {
                if((i+1)<argc)
                {
                    output_file_path=argv[i+1];
                    break;
                }
            }
        }
    }
    return 0;
}

static void arg_parse(int argc,const char *argv[])
{
    {
        for(int i=0; i<argc; i++)
        {
            const char *cmd=argv[i];
            for(size_t j=0; j<sizeof(cmd_list)/sizeof(cmd_list[0]); j++)
            {
                if(cmd_list[j].cmd_short==NULL)
                {
                    cmd_list[j].cmd_short="";
                }
                if(cmd_list[j].cmd==NULL)
                {
                    cmd_list[j].cmd="";
                }
                if(strcmp(cmd,cmd_list[j].cmd_short)==0)
                {
                    if(cmd_list[j].cmd_entry!=NULL)
                    {
                        cmd_list[j].cmd_entry(argc,argv);
                    }
                }
                {
                    char temp[256]= {0};
                    strcat(temp,cmd);
                    for(size_t k=0; k<strlen(temp); k++)
                    {
                        if(temp[k]=='=')
                        {
                            temp[k]='\0';
                        }
                    }
                    if(strcmp(temp,cmd_list[j].cmd)==0)
                    {
                        if(cmd_list[j].cmd_entry!=NULL)
                        {
                            cmd_list[j].cmd_entry(argc,argv);
                        }
                    }
                }
            }
        }
    }
}


static std::wstring string_to_wstring(std::string original)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> code_convert;
    return code_convert.from_bytes(original);
}

//需要转换的字符集合
static std::set<wchar_t> char_set;
static void process_input_file()
{
    std::fstream file(input_file_path.c_str(),std::ios::in);
    if(file.is_open())
    {
        std::string buffer;
        while(std::getline(file,buffer))
        {
            std::wstring str=string_to_wstring(buffer);
            buffer.clear();
            for(size_t i=0; i<str.length(); i++)
            {
                char_set.insert(str.at(i));
            }
        }
        file.close();
    }
    else
    {
        printf("can't open %s\r\n",input_file_path.c_str());
    }

    {
        //把常用的ascii字符也添加至字符列表
        for(size_t i=' '; i<0x80; i++)
        {
            char_set.insert(i);
        }
    }
    {
        //打印字符数
        printf("char count=%d,min=%08X,max=%08X\r\n",(int)char_set.size(),(int)*char_set.begin(),(int)*char_set.rbegin());
    }
}

static void checkout_fontfile()
{
    FT_Face face=NULL;
    if(FT_New_Face(g_ft_lib, font_file_path.c_str(), 0, &face))
    {
        printf("FT_New_Face (%s) error!\r\n",font_file_path.c_str());
        exit(-1);
    }

    FT_Done_Face(face);
}

static void font_bitmap_get(void (*on_get)(FT_Bitmap bmp,int x,int y,wchar_t _char,int font_size),int font_size=24)
{
    FT_Face face=NULL;

    if(FT_New_Face(g_ft_lib, font_file_path.c_str(), 0, &face))
    {
        return;
    }
    //设置为unicode
    FT_Select_Charmap(face, FT_ENCODING_UNICODE);
    {
        FT_Set_Pixel_Sizes(face,font_size,font_size);
        for(auto it=char_set.begin(); it!=char_set.end(); it++)
        {
            if(0==FT_Load_Glyph(face,FT_Get_Char_Index(face,*it),FT_LOAD_DEFAULT))
            {
                if(0==FT_Render_Glyph(face->glyph,FT_RENDER_MODE_NORMAL))
                {
                    FT_Bitmap bmp=face->glyph->bitmap;
                    size_t w=bmp.width;
                    size_t h=bmp.rows;
                    int left=face->glyph->bitmap_left;
                    int top=font_size-face->glyph->bitmap_top;//转换为屏幕坐标的偏移
                    {
                        //调整偏移
                        if(h+top > font_size)
                        {
                            top-=(h+top-font_size);
                        }
                        if(left < 0)
                        {
                            //不允许负偏移
                            left=0;
                        }
                    }
                    if(on_get!=NULL)
                    {
                        on_get(bmp,left,top,(*it),font_size);
                    }
                }
            }
        }
    }

    FT_Done_Face(face);
}

static std::fstream outfile;
static void generate_c_file()
{
    outfile.open(output_file_path.c_str(),std::ios::out);
    if(outfile.is_open())
    {
        //包含文件
        outfile << "#include \"stdint.h\""<<std::endl;
        outfile << "#include \"stdlib.h\""<<std::endl;
        //生成12到32的点阵
        for(size_t i=12; i<=32; i++)
        {
            //字体注释信息
            outfile<<"/* Font Size "<<i<<" */"<<std::endl<<std::endl;

            font_bitmap_get([](FT_Bitmap bmp,int x,int y,wchar_t _char,int font_size)
            {
                size_t w=bmp.width;
                size_t h=bmp.rows;
                {
                    //单个字体的注释
                    outfile << "/*"<<std::endl;
                    {
                        char buff[512]= {0};
                        sprintf(buff,"char=%08X,size=%d,x=%d,y=%d,width=%d,height=%d",(int)_char,(int)font_size,(int)x,(int)y,(int)w,(int)h);
                        outfile << buff<<std::endl;
                    }
                    for(size_t i=0; i<h; i++)
                    {
                        for(size_t j=0; j<w; j++)
                        {
                            if(bmp.buffer[i*w+j]<0x80)
                            {
                                outfile<<" ";
                            }
                            else
                            {
                                outfile<<"#";
                            }
                        }
                        outfile << std::endl;
                    }
                    outfile << "*/"<<std::endl;
                }
                {
                    {
                        //输出变量名
                        char buff[512]= {0};
                        sprintf(buff,"const uint8_t hdotfont_char_%08X_%d[]=",(int)_char,(int)font_size);
                        outfile << buff<<std::endl;
                    }
                    outfile << "{"<<std::endl;

                    outfile << "/* wchar_t */"<<std::endl;
                    {
                        //输出wchar_t的值
                        char buff[512]= {0};
                        sprintf(buff,"0x%02X,0x%02X,0x%02X,0x%02X,",(int)_char&0xFF,(int)(_char>>8)&0xFF,(int)(_char>>16)&0xFF,(int)(_char>>24)&0xFF);
                        outfile << buff<<std::endl;
                    }

                    outfile << "/* x,y,w,h */"<<std::endl;
                    {
                        //输出x,y,w,h
                        char buff[512]= {0};
                        sprintf(buff,"%d,%d,%d,%d,",(int)x,(int)y,(int)w,(int)h);
                        outfile << buff<<std::endl;
                    }

                    outfile << "/* data */"<<std::endl;
                    {
                        uint8_t temp=0;
                        for(size_t i=0; i<h; i++)
                        {
                            for(size_t j=0; j<w; j++)
                            {
                                size_t pixel_index=i*w+j;
                                if(bmp.buffer[i*w+j]>=0x80)
                                {
                                    temp |= (0x1<<(pixel_index%8));
                                }
                                if((pixel_index%8)==7)
                                {
                                    //存满8个位
                                    {
                                        char buff[512]= {0};
                                        sprintf(buff,"0x%02X,",temp);
                                        outfile << buff;
                                    }
                                    if((pixel_index/8)%8==7)
                                    {
                                        outfile<<std::endl;
                                    }
                                    temp=0;
                                }
                                else if(i==(h-1) && j==(w-1))
                                {
                                    //最后一位
                                    {
                                        char buff[512]= {0};
                                        sprintf(buff,"0x%02X,",temp);
                                        outfile << buff;
                                    }
                                    temp=0;
                                }
                            }
                        }
                    }

                    //末尾添0
                    outfile << "0x00" <<std::endl;
                    outfile << "};"<<std::endl;
                }
                outfile << std::endl;
            },i);
        }
        outfile.close();
    }
    else
    {
        printf("open output file(%s) error!\r\n",output_file_path.c_str());
    }
}

int main(int argc,const char *argv[])
{
    arg_parse(argc,argv);

    process_input_file();

    checkout_fontfile();

    generate_c_file();

    return 0;
}



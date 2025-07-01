#include "HCPPBox.h"
#include "hrc.h"
#include H3RDPARTY_ARGTABLE3_HEADER
#include "string"

static void show_banner()
{
    const uint8_t * banner=RCGetHandle("banner");
    if(banner!=NULL)
    {
        hprintf("%s",(const char *)banner);
    }
}

static std::string input_file_name="input.png";
static int height=-1;
static int width=-1;
static void check_args(int argc,char *argv[])
{
    struct arg_file * input=NULL;
    struct arg_int *  h=NULL;
    struct arg_int *  w=NULL;
    struct arg_lit  * help=NULL;
    void *argtable[]=
    {
        input=arg_file0("i","input","image",                "input image file name"),
        h=arg_int0("h","height","image height",             "output image height"),
        w=arg_int0("w","width","image width",               "output image width"),
        help=arg_lit0("H","help",                           "print this help and exit"),
        arg_end(20)
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
        hfputs("Usage:\r\n",stdout);
        arg_print_glossary(stdout,argtable,"  %-25s %s\n");
        hexit(-1);
    }

    if(input->count > 0)
    {
        hprintf("input file name is %s .\r\n",input->filename[0]);
        input_file_name=input->filename[0];
    }

    if(h->count > 0)
    {
        if(h->ival[0] > 0)
        {
            hprintf("output image height is %d .\r\n",h->ival[0]);
            height=h->ival[0];
        }
    }

    if(w->count > 0)
    {
        if(w->ival[0] > 0)
        {
            hprintf("output image width is %d .\r\n",w->ival[0]);
            width=w->ival[0];
        }
    }

    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
}

static std::string file_basename(std::string path)
{
    return path.substr(0,path.rfind("."));
}

static std::string file_extname(std::string path)
{
    return path.substr(path.rfind("."));
}


#include<opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/utils/logger.hpp>
cv::Mat image;
cv::Mat grayimage;
static void load_image()
{
    //关闭日志显示
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);

    //读取图像(BGR格式)
    image=cv::imread(input_file_name,cv::IMREAD_COLOR);
    if(image.empty())
    {
        std::string err_string=input_file_name+" load error!\r\n";
        hfputs(err_string.c_str(),stderr);
        hexit(-1);
    }

    if(height <= 0)
    {
        height=image.rows;
    }

    if(width <= 0)
    {
        width=image.cols;
    }

    if(height!=image.rows || width!=image.cols)
    {
        cv::resize(image.clone(),image,cv::Size(width,height));
    }

    hprintf("image size:%d x %d \r\n",image.cols,image.rows);

    //输出彩色文件
    std::string color_image_output_name=file_basename(input_file_name)+"_"+std::to_string(image.cols)+"_"+std::to_string(image.rows)+file_extname(input_file_name);
    cv::imwrite(color_image_output_name,image);


    cv::cvtColor(image, grayimage, cv::COLOR_BGR2GRAY);

    //输出灰度文件
    std::string gray_image_output_name=file_basename(input_file_name)+"_gray_"+std::to_string(image.cols)+"_"+std::to_string(image.rows)+file_extname(input_file_name);
    cv::imwrite(gray_image_output_name,grayimage);


}

#include <fstream>
typedef cv::Vec3b BGR888_Pixel;
void generate_c_source(void)
{
    if(image.empty()||grayimage.empty())
    {
        return;
    }

    std::string output_file_path=file_basename(input_file_name)+"_hrawimage.c";
    std::string output_var_base=file_basename(input_file_name);
    if(output_var_base.find("/") !=std::string::npos)
    {
        output_var_base=output_var_base.substr(output_var_base.find("/")+1);
    }
    if(output_var_base.find("\\") !=std::string::npos)
    {
        output_var_base=output_var_base.substr(output_var_base.find("\\")+1);
    }
    for(auto it=output_var_base.begin(); it!=output_var_base.end(); it++)
    {
        if(!(('0' <= (*it) && '9' >= (*it)) || ('a' <= (*it) && 'z' >= (*it)) || ('A' <= (*it) && 'Z' >= (*it))))
        {
            (*it)='_';
        }
    }
    hprintf("rawimage path=%s,var=%s\r\n",output_file_path.c_str(),output_var_base.c_str());
    std::fstream outfile;
    outfile.open(output_file_path.c_str(),std::ios::out);
    if(outfile.is_open())
    {
        outfile << "#include \"hgui.h\""<<std::endl;


        {
            outfile << "const uint8_t " << "hrawimage_" << output_var_base << "_data[]=" << std::endl;
            outfile << "{" << std::endl;
            uint8_t *data=new uint8_t[image.cols *image.rows*3];
            image.forEach<BGR888_Pixel>([&](BGR888_Pixel &p,const int *pos)
            {
                data[0+(pos[1]+pos[0]*image.cols)*3]=p[2];
                data[1+(pos[1]+pos[0]*image.cols)*3]=p[1];
                data[2+(pos[1]+pos[0]*image.cols)*3]=p[0];
            });
            for(size_t i=0; i<image.cols *image.rows*3; i++)
            {
                outfile << std::to_string(data[i]) << ",";
                if(i>0 && i%40==0)
                {
                    outfile<< std::endl;
                }
            }
            delete []data;
            outfile << "0" << std::endl;
            outfile << "};" << std::endl;

            outfile << "const hgui_gui_rawimage_t " << "hrawimage_" << output_var_base << "=" << std::endl;
            outfile << "{" << std::endl;
            outfile << std::to_string(image.cols) << "," << std::to_string(image.rows) << "," << "3" << "," << "hrawimage_" << output_var_base << "_data" << std::endl;
            outfile << "};" << std::endl;
        }

        {
            outfile << "const uint8_t " << "hrawimage_" << output_var_base << "_gray" << "_data[]=" << std::endl;
            outfile << "{" << std::endl;
            uint8_t *data=new uint8_t[grayimage.cols *grayimage.rows];
            grayimage.forEach<uint8_t>([&](uint8_t &p,const int *pos)
            {
                data[0+pos[1]+pos[0]*grayimage.cols]=p;
            });
            for(size_t i=0; i<grayimage.cols *grayimage.rows; i++)
            {
                outfile << std::to_string(data[i]) << ",";
                if(i>0 && i%40==0)
                {
                    outfile<< std::endl;
                }
            }
            delete []data;
            outfile << "0" << std::endl;
            outfile << "};" << std::endl;

            outfile << "const hgui_gui_rawimage_t " << "hrawimage_" << output_var_base << "_gray" << "=" << std::endl;
            outfile << "{" << std::endl;
            outfile << std::to_string(grayimage.cols) << "," << std::to_string(grayimage.rows) << "," << "1" << "," << "hrawimage_" << output_var_base  << "_gray" << "_data" << std::endl;
            outfile << "};" << std::endl;
        }

        outfile.close();
    }

    {
        //整理代码
        std::string astyle_cmd=std::string("astyle -n ")+output_file_path;
        system(astyle_cmd.c_str());
    }

}


int main(int argc,char *argv[])
{
    show_banner();

    check_args(argc,argv);

    load_image();

    generate_c_source();

    return 0;
}

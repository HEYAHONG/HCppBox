/***************************************************************
 * Name:      hguitoolsMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    HYH (2229388563@qq.com)
 * Created:   2025-07-24
 * Copyright: HYH (https://hyhsystem.cn)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#include "hguitoolsMain.h"
#include "common_wxwidgets.h"
#include "Version.h"
#include <wx/fontutil.h>
#include <wx/log.h>
#include <wx/filedlg.h>
#include <wx/statbmp.h>
#include <wx/file.h>
#include <wx/msgdlg.h>
#include <wx/mstream.h>
#include <wx/dcmemory.h>
#include <wx/rawbmp.h>
#include "hguitools.xpm"
#include <set>
#include <sstream>
#include <string>

hguitoolsFrame::hguitoolsFrame(wxFrame *frame)
    : mainframe(frame)
{
    SetIcon(wxIcon(hguitools_xpm));
    wxLog::EnableLogging(true);
    {
        //设置日志窗口
        wxFont font=m_textCtrl_log->GetFont();
        m_textCtrl_log->SetFont(*wxTheFontList->FindOrCreateFont(font.GetNativeFontInfo()->GetPointSize(),wxFONTFAMILY_TELETYPE,font.GetStyle(),font.GetWeight()));
        wxLogTextCtrl *logger=new wxLogTextCtrl(m_textCtrl_log);
        wxLog *oldlog=wxLog::SetActiveTarget(logger);
        if(oldlog!=NULL)
        {
            delete oldlog;
        }
    }
    {
        //设定dotfontscan窗口
        wxFont font=dotfontscan_scintilla_c_source->GetFont();
        dotfontscan_scintilla_c_source->SetFont(*wxTheFontList->FindOrCreateFont(font.GetNativeFontInfo()->GetPointSize(),wxFONTFAMILY_TELETYPE,font.GetStyle(),font.GetWeight()));
        dotfontscan_scintilla_c_source->StyleSetFont(0,*wxTheFontList->FindOrCreateFont(font.GetNativeFontInfo()->GetPointSize(),wxFONTFAMILY_TELETYPE,font.GetStyle(),font.GetWeight()));
    }
    hcppbox_init();
    {
        const char *banner=(const char *)RCGetHandle("banner");
        if(banner!=NULL)
        {
            wxLogMessage(wxT("\r\n%s\r\n初始化完成!"),banner);
        }
    }
}

hguitoolsFrame::~hguitoolsFrame()
{
    hcppbox_deinit();
}

void hguitoolsFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void hguitoolsFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void hguitoolsFrame::OnAbout(wxCommandEvent &event)
{
    wxMessageBox(wxString(wxT("hguitools ")+wxString(PROJECT_VERSION_STR)+wxT(" BY HEYAHONG")),wxT(" 关于"));
}

void hguitoolsFrame::dotfontscan_OnFontChanged( wxFontPickerEvent& event )
{
    auto font=event.GetFont();
    wxLogMessage(wxT("dotfontscan字体已更改为%s,高度=%d,宽度=%d(%s)"),font.GetFaceName(),(int)font.GetPixelSize().GetHeight(),(int)font.GetPixelSize().GetWidth(),font.IsFixedWidth()?wxT("固定"):wxT("不固定"));
}
void hguitoolsFrame::dotdontscan_start_OnButtonClick( wxCommandEvent& event )
{
    wxFont font=m_fontPicker_dotfontscan->GetSelectedFont();
    if(!font.IsOk())
    {
        wxLogMessage(wxT("选择的字体无效！"));
        return;
    }
    wxLogMessage(wxT("字体 %s,高度=%d,宽度=%d(%s)"),font.GetFaceName(),(int)font.GetPixelSize().GetHeight(),(int)font.GetPixelSize().GetWidth(),font.IsFixedWidth()?wxT("固定"):wxT("不固定"));
    wxFileDialog SourceFileDialog(this, wxT("选择待扫描的文件"),wxT(""), wxT(""),wxT("*"), wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if(SourceFileDialog.ShowModal() == wxID_CANCEL)
    {
        wxLogMessage(wxT("已取消操作！"));
        return;
    }
    wxLogMessage(wxT("准备打开文件 %s"),SourceFileDialog.GetPath());
    wxFile SourceFile(SourceFileDialog.GetPath());
    if(!SourceFile.IsOpened())
    {
        wxLogMessage(wxT("打开文件 %s 失败！"),SourceFileDialog.GetPath());
    }
    std::set<uint32_t> char_code_set;
    {
        wxString data;
        SourceFile.ReadAll(&data);
        SourceFile.Close();
        for(auto it=data.begin(); it!=data.end(); it++)
        {
            char_code_set.insert(*it);
        }
    }
    wxLogMessage(wxT("扫描完成，总字符数:%d"),(int)char_code_set.size());
    if(char_code_set.size()==0)
    {
        return;
    }
    {
        auto minval=char_code_set.begin();
        auto maxval=char_code_set.begin();
        std::advance(maxval,char_code_set.size()-1);
        wxLogMessage(wxT("字符编码最大值:%08X,字符编码最小值：%08X"),(int)(*maxval),(int)(*minval));
    }
    std::string outputdata;
    {
        std::stringstream stream;
        stream << "#include \"stdint.h\""<<std::endl;
        stream << "#include \"stdlib.h\""<<std::endl<<std::endl;
        stream<<"/* Font Size "<<font.GetPixelSize().GetHeight()<<" */"<<std::endl<<std::endl;
        int font_size=font.GetPixelSize().GetHeight();
        for(auto it=char_code_set.begin(); it!=char_code_set.end(); it++)
        {
            uint32_t char_code=*it;
            wxCoord w=0;
            wxCoord h=0;
            wchar_t str[]= {(wchar_t)char_code,0};
            {
                wxBitmap bmp(1000,1000);
                wxMemoryDC dc(bmp);
                dc.SetFont(font);
                dc.GetTextExtent(wxString(str),&w,&h,NULL,NULL,&font);
            }
            wxCoord x=(font_size-w)/2;
            wxCoord y=(font_size-h)/2;
            if(y<0)
            {
                y=0;
            }
            if(x<0)
            {
                x=0;
            }
            wxImage image;
            {
                wxBitmap bmp(1000,1000);
                {
                    wxMemoryDC dc(bmp);
                    dc.SetFont(font);
                    dc.SetTextBackground(*wxBLACK);
                    dc.SetTextForeground(*wxWHITE);
                    dc.DrawText(wxString(str),0,0);
                    dc.SelectObject(wxNullBitmap);
                }
                image=bmp.ConvertToImage();
            }
            stream << "/*"<<std::endl;
            {
                char buff[512]= {0};
                sprintf(buff,"char=%08X,size=%d,x=%d,y=%d,width=%d,height=%d",(int)char_code,(int)font_size,(int)x,(int)y,(int)w,(int)h);
                stream << buff<<std::endl;
            }
            for(size_t i=0; i<h; i++)
            {
                for(size_t j=0; j<w; j++)
                {
                    if(image.GetRed(j,i)==0 && image.GetBlue(j,i)==0 && image.GetGreen(j,i)==0)
                    {
                        stream<<" ";
                    }
                    else
                    {
                        stream<<"*";
                    }
                }
                stream << std::endl;
            }
            stream << "*/"<<std::endl;
            {
                //输出变量名
                char buff[512]= {0};
                sprintf(buff,"const uint8_t hdotfont_char_%08X_%d[]=",(int)char_code,(int)font_size);
                stream << buff<<std::endl;
            }
            stream << "{"<<std::endl;
            stream << "/* wchar_t */"<<std::endl;
            {
                //输出wchar_t的值
                char buff[512]= {0};
                sprintf(buff,"0x%02X,0x%02X,0x%02X,0x%02X,",(int)char_code&0xFF,(int)(char_code>>8)&0xFF,(int)(char_code>>16)&0xFF,(int)(char_code>>24)&0xFF);
                stream << buff<<std::endl;
            }

            stream << "/* x,y,w,h */"<<std::endl;
            {
                //输出x,y,w,h
                char buff[512]= {0};
                sprintf(buff,"%d,%d,%d,%d,",(int)x,(int)y,(int)w,(int)h);
                stream << buff<<std::endl;
            }

            stream << "/* data */"<<std::endl;
            {
                uint8_t temp=0;
                for(size_t i=0; i<h; i++)
                {
                    for(size_t j=0; j<w; j++)
                    {
                        size_t pixel_index=i*w+j;
                        if(!(image.GetRed(j,i)==0 && image.GetBlue(j,i)==0 && image.GetGreen(j,i)==0))
                        {
                            temp |= (0x1<<(pixel_index%8));
                        }
                        if((pixel_index%8)==7)
                        {
                            //存满8个位
                            {
                                char buff[512]= {0};
                                sprintf(buff,"0x%02X,",temp);
                                stream << buff;
                            }
                            if((pixel_index/8)%8==7)
                            {
                                stream<<std::endl;
                            }
                            temp=0;
                        }
                        else if(i==(h-1) && j==(w-1))
                        {
                            //最后一位
                            {
                                char buff[512]= {0};
                                sprintf(buff,"0x%02X,",temp);
                                stream << buff;
                            }
                            temp=0;
                        }
                    }
                }
            }

            //末尾添0
            stream << "0x00" <<std::endl;
            stream << "};"<<std::endl;
        }
        stream << std::endl;
        outputdata=stream.str();
    }
    dotfontscan_scintilla_c_source->SetText(outputdata);
}


void hguitoolsFrame::dotdontscan_save_OnButtonClick( wxCommandEvent& event )
{
    wxString data=dotfontscan_scintilla_c_source->GetText();
    if(data.IsEmpty())
    {
        wxLogMessage(_T("无内容！"));
        return;
    }
    wxFileDialog DestFileDialog(this, wxT("选择待扫描的文件"),wxT(""), wxT(""),wxT("*.c"), wxFD_SAVE);
    if(DestFileDialog.ShowModal()==wxID_CANCEL)
    {
        wxLogMessage(_T("取消操作！"));
        return;
    }
    wxLogMessage(_T("准备保存文件: %s"),DestFileDialog.GetPath());
    wxFile file;
    file.Open(DestFileDialog.GetPath(),wxFile::write);
    if(!file.IsOpened())
    {
        wxLogMessage(_T("打开文件失败！"));
        return;
    }
    file.Write(data);
    file.Close();
}

class bitmap_preview:public imageresourcegenerate_dialog_preview
{
private:
    static bitmap_preview *oldptr;
public:
    bitmap_preview( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxMINIMIZE_BOX ):imageresourcegenerate_dialog_preview(parent,id,title,pos,size,style)
    {
        if(oldptr!=NULL)
        {
            oldptr->Destroy();
        }
        oldptr=this;
    }
    virtual ~bitmap_preview()
    {
        if(oldptr==this)
        {
            oldptr=NULL;
        }
    }
};
bitmap_preview *bitmap_preview::oldptr=NULL;

void hguitoolsFrame::imageresourcegenerate_load_OnButtonClick( wxCommandEvent& event )
{
    wxFileDialog SourceFileDialog(this, wxT("选择加载的图片"),wxT(""), wxT(""),wxT("*"), wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if(SourceFileDialog.ShowModal() == wxID_CANCEL)
    {
        wxLogMessage(wxT("已取消操作！"));
        return;
    }
    wxLogMessage(wxT("准备打开文件 %s"),SourceFileDialog.GetPath());
    wxImage image;
    if(!image.LoadFile(SourceFileDialog.GetPath()))
    {
        wxLogMessage(wxT("图片读取失败！"));
        return;
    }
    int w=image.GetWidth();
    int h=image.GetHeight();
    wxLogMessage(wxT("图片大小：%dX%d "),(int)w,(int)h);
    bitmap_preview *bitmap=new bitmap_preview(this,wxID_ANY,SourceFileDialog.GetPath(),wxDefaultPosition,wxSize((w>300?w:300)+20,(h>300?h:300)+20));
    bitmap->SetSize(wxSize(wxSize((w>300?w:300)+20,(h>300?h:300)+40)));
    bitmap->imageresourcegenerate_bitmap_preview->SetBitmap(image);
    bitmap->Layout();
    bitmap->Show();
    std::string outputdata;
    {
        std::string output_var_base="custom";
        std::stringstream stream;
        stream << "#include \"hgui.h\""<<std::endl;
        {
            stream << "const uint8_t " << "hrawimage_" << output_var_base << "_data[]=" << std::endl;
            stream << "{" << std::endl;
            uint8_t *data=new uint8_t[w*h*3];
            for(size_t i=0; i<h; i++)
            {
                for(size_t j=0; j<w; j++)
                {
                    data[(i*w+j)*3+0]=image.GetRed(j,i);
                    data[(i*w+j)*3+1]=image.GetGreen(j,i);
                    data[(i*w+j)*3+2]=image.GetBlue(j,i);
                }
            }
            for(size_t i=0; i<w *h*3; i++)
            {
                stream << std::to_string(data[i]) << ",";
                if(i>0 && i%40==0)
                {
                    stream<< std::endl;
                }
            }
            delete []data;
            stream << "0" << std::endl;
            stream << "};" << std::endl;

            stream << "const hgui_gui_rawimage_t " << "hrawimage_" << output_var_base << "=" << std::endl;
            stream << "{" << std::endl;
            stream << std::to_string(w) << "," << std::to_string(h) << "," << "3" << "," << "hrawimage_" << output_var_base << "_data" << std::endl;
            stream << "};" << std::endl;
        }

        outputdata+=stream.str();
    }
    {
        std::string output_var_base="custom_gray";
        std::stringstream stream;
        {
            stream << "const uint8_t " << "hrawimage_" << output_var_base << "_data[]=" << std::endl;
            stream << "{" << std::endl;
            uint8_t *data=new uint8_t[w*h];
            for(size_t i=0; i<h; i++)
            {
                for(size_t j=0; j<w; j++)
                {
                    //灰度公式:0.299R + 0.587G + 0.114B
                    data[(i*w+j)+0]=0.299*image.GetRed(j,i)+0.587*image.GetGreen(j,i)+0.114*0.299*image.GetBlue(j,i);
                }
            }
            for(size_t i=0; i<w *h; i++)
            {
                stream << std::to_string(data[i]) << ",";
                if(i>0 && i%40==0)
                {
                    stream<< std::endl;
                }
            }
            delete []data;
            stream << "0" << std::endl;
            stream << "};" << std::endl;

            stream << "const hgui_gui_rawimage_t " << "hrawimage_" << output_var_base << "=" << std::endl;
            stream << "{" << std::endl;
            stream << std::to_string(w) << "," << std::to_string(h) << "," << "1" << "," << "hrawimage_" << output_var_base << "_data" << std::endl;
            stream << "};" << std::endl;
        }

        outputdata+=stream.str();
    }
    imageresourcegenerate_scintilla_c_source->SetText(wxString(outputdata.c_str()));
}

void hguitoolsFrame::imageresourcegenerate_save_OnButtonClick( wxCommandEvent& event )
{
    wxString data=imageresourcegenerate_scintilla_c_source->GetText();
    if(data.IsEmpty())
    {
        wxLogMessage(_T("无内容！"));
        return;
    }
    wxFileDialog DestFileDialog(this, wxT("选择待扫描的文件"),wxT(""), wxT(""),wxT("*.c"), wxFD_SAVE);
    if(DestFileDialog.ShowModal()==wxID_CANCEL)
    {
        wxLogMessage(_T("取消操作！"));
        return;
    }
    wxLogMessage(_T("准备保存文件: %s"),DestFileDialog.GetPath());
    wxFile file;
    file.Open(DestFileDialog.GetPath(),wxFile::write);
    if(!file.IsOpened())
    {
        wxLogMessage(_T("打开文件失败！"));
        return;
    }
    file.Write(data);
    file.Close();
}

void hguitoolsFrame::OnMSTimer( wxTimerEvent& event )
{
    hcppbox_softwaretimer_isr();
}

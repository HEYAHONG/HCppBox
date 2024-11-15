/***************************************************************
 * Name:      HCPPInit.cpp
 * Purpose:   HCPPInit接口实现。
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-11-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "HCPPInit.h"
#include <mutex>
#include <string>
#include <queue>
#include <functional>
#include <map>
namespace HCPPInitGlobal
{
extern std::recursive_mutex m_lock;
extern std::map<std::string,std::queue<std::function<void(void)>>> *m_map;
}
void HCPPInit(void)
{
    std::lock_guard<std::recursive_mutex> lock(HCPPInitGlobal::m_lock);
    if(HCPPInitGlobal::m_map==NULL)
    {
        return;
    }
    for(auto it=HCPPInitGlobal::m_map->begin(); it!=HCPPInitGlobal::m_map->end(); it++)
    {
        std::queue<std::function<void(void)>> &queue=it->second;
        while(!queue.empty())
        {
            auto init_fn=queue.front();
            if(init_fn!=NULL)
            {
                init_fn();
            }
            queue.pop();
        }
    }
    {
        // 删除，节省空间
        delete HCPPInitGlobal::m_map;
        HCPPInitGlobal::m_map=NULL;
    }
}

void HCPPInitRegister(std::string tag,std::function<void (void)> init_fn)
{
    std::lock_guard<std::recursive_mutex> lock(HCPPInitGlobal::m_lock);
    if(HCPPInitGlobal::m_map==NULL)
    {
        HCPPInitGlobal::m_map=new std::map<std::string,std::queue<std::function<void(void)>>>();
    }
    (*HCPPInitGlobal::m_map)[tag].push(init_fn);
};

void HCPPInitRegister(const char * tag,void (*init_fn)(void))
{
    if(tag==NULL)
    {
        tag="";
    }
    if(init_fn!=NULL)
    {
        HCPPInitRegister(std::string(tag),std::function<void (void)>(*init_fn));
    }
}

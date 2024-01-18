#include "HCPPObjectPool.h"
#include "map"
#include <mutex>

static std::mutex HCPPObjectPoolLock;
static std::map<std::string,HCPPObject *> HCPPObjectPool;
static std::map<void *,std::string> HCPPObjectPoolDeleteHelper;

/** \brief 删除对象池的对象(一般用于自动删除)
 *
 * \param ptr void* 待删除的指针
 *
 */
void HCPPObjectPool_ObjectDelete(void *ptr)
{
    std::lock_guard<std::mutex> lock(HCPPObjectPoolLock);
    if(HCPPObjectPoolDeleteHelper.find(ptr)!=HCPPObjectPoolDeleteHelper.end())
    {
        std::string id=HCPPObjectPoolDeleteHelper[ptr];
        if(HCPPObjectPool.find(id)!=HCPPObjectPool.end())
        {
            HCPPObjectPool.erase(HCPPObjectPool.find(id));
        }
        HCPPObjectPoolDeleteHelper.erase(HCPPObjectPoolDeleteHelper.find(ptr));
    }
}
void HCPPObjectPool_Set(std::string id,HCPPObject *obj)
{
    if(id.empty() || obj == NULL)
    {
        //不接受id为空或obj为NULL
        return;
    }
    std::lock_guard<std::mutex> lock(HCPPObjectPoolLock);
    HCPPObjectPool[id]=obj;
    HCPPObjectPoolDeleteHelper[obj->GetVoidPtr()]=id;

}

HCPPObject *HCPPObjectPool_Get(std::string id)
{
    std::lock_guard<std::mutex> lock(HCPPObjectPoolLock);
    if(HCPPObjectPool.find(id)!=HCPPObjectPool.end())
    {
        return HCPPObjectPool[id];
    }
    return NULL;
}

std::string HCPPObjectPool_Id_Helper(std::vector<std::string> id_list)
{
    std::string ret;
    for(size_t i=0; i<id_list.size(); i++)
    {
        if(id_list[i].empty())
        {
            //跳过空子字符串
            continue;
        }
        ret+=id_list[i];
        if((i+1) != id_list.size())
        {
            ret+= ".";
        }
    }
    return ret;
}

std::vector<std::string> HCPPObjectPool_Id_Helper(std::string id)
{
    std::vector<std::string>  ret;
    while(!id.empty())
    {
        std::string::size_type pos=id.find('.');
        if(pos == 0)
        {
            //跳过空子字符串
            id=id.substr(1);
            continue;
        }
        if(pos==std::string::npos)
        {
            ret.push_back(id);
            break;
        }
        else
        {
            ret.push_back(id.substr(0,pos));
            id=id.substr(pos+1);
        }
    }
    return ret;
}

void HCPPObjectPool_Enum(std::function<void(std::string,HCPPObject *)> on_enum)
{
    if(on_enum==NULL)
    {
        return;
    }
    std::lock_guard<std::mutex> lock(HCPPObjectPoolLock);
    for(std::map<std::string,HCPPObject *>::iterator it=HCPPObjectPool.begin(); it!=HCPPObjectPool.end(); it++)
    {
        on_enum(it->first,it->second);
    }
}

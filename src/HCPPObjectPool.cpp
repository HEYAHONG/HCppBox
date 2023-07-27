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

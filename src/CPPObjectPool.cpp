#include "CPPObjectPool.h"
#include "map"
#include <mutex>

static std::mutex CPPObjectPoolLock;
static std::map<std::string,CPPObject::CPPObject *> CPPObjectPool;
static std::map<void *,std::string> CPPObjectPoolDeleteHelper;

/** \brief 删除对象池的对象(一般用于自动删除)
 *
 * \param ptr void* 待删除的指针
 *
 */
void CPPObjectPool_ObjectDelete(void *ptr)
{
    std::lock_guard<std::mutex> lock(CPPObjectPoolLock);
    if(CPPObjectPoolDeleteHelper.find(ptr)!=CPPObjectPoolDeleteHelper.end())
    {
        std::string id=CPPObjectPoolDeleteHelper[ptr];
        if(CPPObjectPool.find(id)!=CPPObjectPool.end())
        {
            CPPObjectPool.erase(CPPObjectPool.find(id));
        }
        CPPObjectPoolDeleteHelper.erase(CPPObjectPoolDeleteHelper.find(ptr));
    }
}
void CPPObjectPool_Set(std::string id,CPPObject::CPPObject *obj)
{
    if(id.empty() || obj == NULL)
    {
        //不接受id为空或obj为NULL
        return;
    }
    std::lock_guard<std::mutex> lock(CPPObjectPoolLock);
    CPPObjectPool[id]=obj;
    CPPObjectPoolDeleteHelper[obj->GetVoidPtr()]=id;

}

CPPObject::CPPObject *CPPObjectPool_Get(std::string id)
{
    std::lock_guard<std::mutex> lock(CPPObjectPoolLock);
    if(CPPObjectPool.find(id)!=CPPObjectPool.end())
    {
        return CPPObjectPool[id];
    }
    return NULL;
}

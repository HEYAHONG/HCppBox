#include "HCPPObject.h"
#include "stdint.h"
#include <thread>
#include <chrono>

class Tree_Item:public HCPPObject
{
    O_HCPPOBJECT
public:
    Tree_Item(Tree_Item *parent=NULL):HCPPObject(parent)
    {
        printf("new item %08X\r\n",(uint32_t)reinterpret_cast<uintptr_t>(this));
    }
    Tree_Item(Tree_Item &other):HCPPObject(other)
    {
        printf("new item %08X\r\n",(uint32_t)reinterpret_cast<uintptr_t>(this));
    }
    Tree_Item(Tree_Item &&other):HCPPObject(std::forward<Tree_Item>(other))
    {
        printf("new item %08X\r\n",(uint32_t)reinterpret_cast<uintptr_t>(this));
    }
    ~Tree_Item()
    {
        printf("delete item %08X\r\n",(uint32_t)reinterpret_cast<uintptr_t>(this));
    }
};

void Parent_main()
{
    printf(__FILE__  " Start\r\n");
    {
        printf("base test\r\n");
        //根节点
        printf("create root node\r\n");
        Tree_Item *root=new Tree_Item;

        printf("add some leaf node - level 1\r\n");
        Tree_Item *leaf_1=new Tree_Item(root);
        new Tree_Item(root);
        new Tree_Item(root);
        new Tree_Item(root);

        printf("add some leaf node - level 2\r\n");
        Tree_Item *leaf_2_1=new Tree_Item(leaf_1);
        Tree_Item *leaf_2_2=new Tree_Item(leaf_1);
        Tree_Item *leaf_2_3=new Tree_Item(leaf_1);
        Tree_Item *leaf_2_4=new Tree_Item(leaf_1);

        printf("remove some leaf\r\n");
        delete leaf_2_4;
        delete leaf_2_3;

        printf("remove root\r\n");
        delete root;
    }

    {
        printf("copy test\r\n");
        //根节点
        printf("create root node\r\n");
        Tree_Item *root=new Tree_Item;

        printf("add some leaf node - level 1\r\n");
        Tree_Item *leaf_1=new Tree_Item(root);
        new Tree_Item(root);
        new Tree_Item(root);
        new Tree_Item(root);

        printf("add some leaf node - level 2\r\n");
        Tree_Item *leaf_2_1=new Tree_Item(leaf_1);
        Tree_Item *leaf_2_2=new Tree_Item(leaf_1);
        Tree_Item *leaf_2_3=new Tree_Item(leaf_1);
        Tree_Item *leaf_2_4=new Tree_Item(leaf_1);

        //调用复制构造,默认不复制子对象
        printf("copy leaf_1\r\n");
        new Tree_Item(*leaf_1);

        printf("remove some leaf\r\n");
        delete leaf_2_4;
        delete leaf_2_3;

        printf("remove root\r\n");
        delete root;
    }

    {
        printf("move test\r\n");
        //根节点
        printf("create root node\r\n");
        Tree_Item *root=new Tree_Item;

        printf("add some leaf node - level 1\r\n");
        Tree_Item *leaf_1=new Tree_Item(root);
        new Tree_Item(root);
        new Tree_Item(root);
        new Tree_Item(root);

        printf("add some leaf node - level 2\r\n");
        Tree_Item *leaf_2_1=new Tree_Item(leaf_1);
        Tree_Item *leaf_2_2=new Tree_Item(leaf_1);
        Tree_Item *leaf_2_3=new Tree_Item(leaf_1);
        Tree_Item *leaf_2_4=new Tree_Item(leaf_1);

        //调用移动构造,默认移动子对象
        printf("move leaf_1\r\n");
        new Tree_Item(std::move(*leaf_1));

        printf("remove some leaf\r\n");
        delete leaf_2_4;
        delete leaf_2_3;

        printf("remove root\r\n");
        delete root;
    }

    printf(__FILE__  " End\r\n");
}

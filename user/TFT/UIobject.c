#include "UIobject.h"
//

UIobject *head;
void headInit()
{
    head = getUIobject();
}

UIobject *getHead()
{
    return head;
}

UIobject *getUIobject()
{
    UIobject *this;
    // warning: if malloc return NULL, make heap size bigger
    do
    {
        this = (UIobject *)malloc(sizeof(UIobject));
    } while (this == NULL);
    this->this = this;
    this->next = NULL;
    this->child = NULL;
    this->childNext = NULL;
    this->x = this->y = 0;
    this->box[0][0] = this->box[0][1] = this->box[1][0] = this->box[1][1] = 0;
    this->eventListener = NULL;
    this->update = NULL;
    this->childUpdate = NULL;
    this->shader = NULL;
    this->selfStruct = NULL;
    this->color = RED;
    // this->priority = 0;
    return this;
}

void insert(UIobject *link, UIobject *node)
{
    node->next = link->next;
    link->next = node;
}

void priorityInsert(UIobject *node)
{
    UIobject *pointer = head;
    while (pointer->next != NULL)
    {
        if (node->priority < pointer->next->priority)
        {
            insert(pointer, node);
            return;
        }
        pointer = pointer->next;
    }
    insert(pointer, node);
}

void delNext(UIobject *node)
{
    node->next = node->next->next;
    free(node->next);
}

short getRelativeAxisX(UIobject *child){
    int x = 0;
    while(child->father!= NULL){
        child = child->father;
        x += child->relativeX;
    }
    return x;
}

short getRelativeAxisY(UIobject *child){
    int y = 0;
    while(child->father!= NULL){
        child = child->father;
        y += child->relativeY;
    }
    return y;
}

void onFatherUpdate(UIobject *this, int deltaT)
{
    this->x = this->father->x + this->relativeX;
    this->y = this->father->y + this->relativeY;
}

void childInsert(UIobject *father, UIobject *child)
{
    child->father = father;
    onFatherUpdate(child, 0);
    if(child->childUpdate == NULL)
        child->childUpdate = onFatherUpdate;
    if (father->child != NULL)
    {
        UIobject *childPointer = father->child;
        while (childPointer->childNext != NULL)
        {
            childPointer = childPointer->childNext;
        }
        childPointer->childNext = child;
    }
    else
        father->child = child;
}
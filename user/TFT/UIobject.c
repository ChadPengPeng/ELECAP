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
	UIobject *result;
    //warning: if malloc return NULL, make heap size bigger
    do{
        result = (UIobject *)malloc(sizeof(UIobject));
    }while(result == NULL);
    result->this = result;
    result->next = NULL;
    result->x = result->y = 0;
    result->box[0][0] = result->box[0][1] = result->box[1][0] = result->box[1][1] = 0;
    result->eventListener = NULL;
    result->update = NULL;
    result->shader = NULL;
    result->selfStruct = NULL;
    // result->priority = 0;
    return result;
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
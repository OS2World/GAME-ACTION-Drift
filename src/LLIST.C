/*                                                                          *\
  Generic linked list functions - node style for OS/2
  Original by Dirk Vandenheuvel 16/09/94
  Rewritten for OpenWatcom / OS/2 PM
\*                                                                          */

#define INCL_WIN
#include <os2.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include "resource.h"
#include "drift.h"

extern HWND hwndClient;

int llist_items = 0;   /* global node count across all lists */

#define ERROR_MALLOC 1
#define ERROR_EOL    2
#define ERROR_TYPE   3

void error_handler(UINT type, char *inFunction, void *extra)
{
char buffer[200];
switch (type)
    {
    case ERROR_MALLOC:
        sprintf(buffer, "Malloc error in function [%s] : object [%s]",
                inFunction, (char *)extra);
        break;
    case ERROR_EOL:
        sprintf(buffer, "End of LList in function [%s] : object ID not found",
                inFunction);
        break;
    case ERROR_TYPE:
        sprintf(buffer, "Wrong llist type for function [%s]", inFunction);
        break;
    default:
        sprintf(buffer, "Undefined error in [%s]!", inFunction);
        break;
    }
WinMessageBox(HWND_DESKTOP, hwndClient, buffer, "LList Error",
              0, MB_ICONEXCLAMATION | MB_OK);
}

/* Prepend a new node holding obj to the list *head points at. */
void add_list(OBJEKT *obj, LLIST **head)
{
LLIST *node;
node = (LLIST *)malloc(sizeof(LLIST));
if (node == NULL)
    {
    error_handler(ERROR_MALLOC, "add_list", "node");
    exit(1);
    }
node->objekt = obj;
node->next   = *head;
*head        = node;
llist_items++;
}

/* Remove the node whose objekt == obj from the list *head points at.
   Does NOT free the OBJEKT itself - caller does that. */
void delete_list(OBJEKT *obj, LLIST **head)
{
LLIST *index = *head;
LLIST *prev  = NULL;
while (index != NULL)
    {
    if (index->objekt == obj)
        {
        if (prev)
            prev->next = index->next;
        else
            *head = index->next;
        free(index);
        llist_items--;
        return;
        }
    prev  = index;
    index = index->next;
    }
error_handler(ERROR_EOL, "delete_list", NULL);
exit(1);
}

/* Free all nodes and their OBJEKTs.  Caller must NULL the head pointer. */
void free_list(LLIST *head)
{
LLIST *tmp;
while (head != NULL)
    {
    tmp = head->next;
    free_objekt(head->objekt);
    free(head);
    llist_items--;
    head = tmp;
    }
}

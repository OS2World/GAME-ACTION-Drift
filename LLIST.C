/*                                                                          *\
  Generic linked list functions
  By Dirk Vandenheuvel 16/09/94

  Revisions:
\*                                                                          */

 #include <stdlib.h>
 #include <stdio.h>
 #include <malloc.h>
 #include <windows.h>
 #include "Drift.h"
 #include "Llist.h"

extern HWND hwndClient;

void free_list ( LLIST *llist)          // frees the list and all objects in llist
{
LLIST_ITEM *tmp;
LLIST_ITEM *index;
index = llist->start_llist;
while (index != NULL)
    {
    free_object (index -> object);
    tmp = index;
    index = index -> next;
    free (tmp);
    }
llist->start_llist = NULL;
llist->llist_items = 0;
}

void add_list (OBJEKT *object, LLIST *llist)
{
 LLIST_ITEM *tmp;
 tmp = llist->start_llist;
 if ((llist->start_llist = ( LLIST_ITEM *) malloc (sizeof(LLIST_ITEM))) == NULL)
     {
     // call errorhandler here
     error_handler(ERROR_MALLOC,"add_list","start_llist");
     exit(1);
     }
 (llist->start_llist)->object = object;
 (llist->start_llist)->next = tmp;
 llist->llist_items++;
}

void delete_list (void *object, LLIST *llist)
{
LLIST_ITEM *index;
LLIST_ITEM *prev;
int count = 1;
index = llist->start_llist;
prev = llist->start_llist;
while (index != NULL)
    {
    if (index -> object == object)  // yep , we found the object to delete
      {
      prev -> next = index -> next;
      if (count == 1)              // the first object was targetted for termination
        llist->start_llist = index -> next;
      free(index);
      llist-> llist_items--;
      return;
      }
    prev = index;
    index = index -> next;
    count++;
    }
// errorhandler: attempt to delete object not in llist
error_handler(ERROR_EOL,"delete_list","object->ID");
exit(1);
}

void error_handler (UINT type, char *inFunction, void* extra)
{
char buffer[100];
switch (type)
  {
  case ERROR_MALLOC:  sprintf(buffer,"Malloc error in function [%s] : object [%s]",inFunction, (char*) extra);
               break;
  case ERROR_EOL:     sprintf(buffer,"End of LList in function [%s] : object [%s]",inFunction, (char*) extra);
               break;
  case ERROR_TYPE:    sprintf(buffer,"Wrong llist type for this function [%s]",inFunction);
               break;
  default:            sprintf(buffer,"Undefined error!!!");
  }
WinMessageBox (HWND_DESKTOP,hwndClient, buffer, "LList",MB_ICONEXCLAMATION | MB_OK);
}
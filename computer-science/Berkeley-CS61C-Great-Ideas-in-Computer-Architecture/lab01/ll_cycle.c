#include <stddef.h>
#include "ll_cycle.h"

int ll_has_cycle(node *head) {
    /* your code here */
	if(head==NULL)
		return 0;

    node *tortoise=head, *hare=head;
	do{
		for(int i=0; i<2;i++){
			hare = hare->next;
			if(hare==NULL)
			    return 0;
		}
		tortoise = tortoise->next;
	}while(tortoise!=hare);

    return 1;
}

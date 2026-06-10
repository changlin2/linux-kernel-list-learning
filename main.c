#include "my_list.h"



struct mynode{
    struct list_head list;
    int id;
};

int main() {
    LIST_HEAD(head);
    struct list_head node1, node2, node3;
    // INIT_LIST_HEAD(&node1);
    // INIT_LIST_HEAD(&node2);

    list_add(&node1, &head);
    list_add(&node2, &head);
    list_add(&node3, &head);

    // 遍历打印地址（简单验证）
    struct list_head *pos;
    for (pos = head.next; pos != &head; pos = pos->next) {
        printf("node at %p\n", pos);
    }

    LIST_HEAD(my_head);
    struct mynode mynode1 = {.id = 1, .list = LIST_HEAD_INIT(mynode1.list)};
    struct mynode mynode2 = {.id = 2, .list = LIST_HEAD_INIT(mynode2.list)};

    list_add(&mynode1.list, &my_head);
    list_add(&mynode2.list, &my_head);
    for(pos = my_head.next; pos != &my_head; pos = pos->next){
        struct mynode *node_ptr = list_entry(pos, struct mynode, list);
        printf("id = %d\n", node_ptr->id);
    }

    struct mynode *node_ptr;
    list_for_each_entry(node_ptr, &my_head, list){
        printf("id = %d\n", node_ptr->id);
    }

    struct mynode *n;
    struct mynode mynode3 = {.id = 3, .list = LIST_HEAD_INIT(mynode3.list)};
    struct mynode mynode4 = {.id = 4, .list = LIST_HEAD_INIT(mynode4.list)};

    list_add(&mynode3.list, &my_head);
    list_add(&mynode4.list, &my_head);

    list_for_each_entry_safe(node_ptr, n, &my_head, list){
        if(node_ptr->id == 3){
            list_del(&node_ptr->list);
            continue;
        }
        printf("id = %d\n", node_ptr->id);
    }
    
    return 0;
}
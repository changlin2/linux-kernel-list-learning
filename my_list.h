#include <stdio.h>
#include <stddef.h>

struct list_head{
    struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }
#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)

// #define _WRITE_ONCE(x, val) \
// do{                                      \
//     *(volatile typeof(x)*)&(x) = (val);  \
// } while(0);

// #define WRITE_ONCE(x, val)   \
// do{
//     compiletime_assert_rwonce_type(x);   \
//     _WRITE_ONCE(x, val);                 \
// }while(0);

static inline void INIT_LIST_HEAD(struct list_head *list)
{
    // WRITE_ONCE(list->next, list);
    // WRITE_ONCE(list->prev, list);
    list->next = list;
    list->prev = list;
}

static inline void __list_add(struct list_head *new_list, struct list_head *prev, struct list_head *next)
{
    next->prev = new_list;
    new_list->next = next;
    new_list->prev = prev;
    prev->next = new_list;
}

static inline void list_add(struct list_head* new_list, struct list_head *head)
{
    __list_add(new_list, head, head->next);
}

static inline void list_add_tail(struct list_head* new_list, struct list_head *head)
{
    __list_add(new_list, head->prev, head);
}

static inline void __list_del(struct list_head* prev, struct list_head *next)
{
    next->prev = prev;
    prev->next = next;
}

static inline void __list_del_entry(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
}

static inline void list_del(struct list_head *entry)
{
    __list_del_entry(entry);
	// entry->next = LIST_POISON1;
	// entry->prev = LIST_POISON2;//poison
}

static inline int list_empty(const struct list_head *head)
{
	return head->next == head;
}

static inline int list_is_head(const struct list_head *list, const struct list_head *head)
{
    return list == head;
}

#define list_entry(ptr, type, member) ({ \
    void *__mptr = (void *)(ptr);   \
    ((type *)(__mptr - offsetof(type, member))); })

#define list_first_entry(ptr, type, member) \
    list_entry((ptr)->next, type, member)

#define list_entry_is_head(pos, head, member) \
    list_is_head(&pos->member, head)

#define list_next_entry(pos, member) \
    list_entry((pos)->member.next, typeof(*(pos)), member)

#define list_for_each_entry(pos, head, member)              \
    for(pos = list_first_entry(head, typeof(*pos), member); \
        !list_entry_is_head(pos, head, member);             \
        pos = list_next_entry(pos, member))
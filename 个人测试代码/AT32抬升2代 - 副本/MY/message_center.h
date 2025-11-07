#ifndef __MESSAGE_CENTER
#define __MESSAGE_CENTER

#include "at32f403a_407.h"

#define MAX_TOPIC_NAME_LEN 32 // 最大的话题名长度,每个话题都由字符串来命名
#define MAX_TOPIC_COUNT 12    // 最多支持的话题数量
#define QUEUE_SIZE 1          // QUENE用来保存数据指针

/**
 * @brief 订阅者类型.每个订阅者拥有订阅者实例 队列用来存储消息，存储的消息数量为QUEUE_SIZE
 */
typedef struct mqt
{
    /* 用数组模拟FIFO队列 */
    void *queue[QUEUE_SIZE];
    uint8_t data_len;
    uint8_t front_idx;
    uint8_t back_idx;
    uint8_t temp_size; // 当前队列长度

    /* 指向下一个订阅了相同的话题的订阅者的指针 */
    struct mqt *next_subs_queue; // 使得发布者可以通过链表访问所有订阅了相同话题的订阅者
} Subscriber_t;


/**
 * @brief 发布者类型.每个发布者拥有发布者实例,并且可以通过链表访问所有订阅了自己发布的话题的订阅者
 *
 */
typedef struct ent
{
    /* 话题名称 */
    char topic_name[MAX_TOPIC_NAME_LEN + 1]; // 1个字节用于存放字符串结束符 '\0'
    uint8_t data_len;                        // 该话题的数据长度
    /* 指向第一个订阅了该话题的订阅者,通过链表访问所有订阅者 */
    Subscriber_t *first_subs;
    /* 指向下一个Publisher的指针 */
    struct ent *next_topic_node;
    uint8_t pub_registered_flag; // 用于标记该发布者是否已经注册
} Publisher_t;




Publisher_t *PubRegister(char *name, uint8_t data_len);
Subscriber_t *SubRegister(char *name, uint8_t data_len);
uint8_t SubGetMessage(Subscriber_t *sub, void *data_ptr);
uint8_t PubPushMessage(Publisher_t *pub, void *data_ptr);

#endif


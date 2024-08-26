#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>

// 文件事件类型定义
#define AE_READABLE 1
#define AE_WRITABLE 2

// 回调函数类型
typedef void aeFileProc(void *clientData, int mask);

// 文件事件结构体
typedef struct aeFileEvent {
    int mask;                // AE_READABLE or AE_WRITABLE
    aeFileProc *rfileProc;   // 读回调
    aeFileProc *wfileProc;   // 写回调
    void *clientData;        // 用户数据
} aeFileEvent;

// 已触发事件结构体
typedef struct aeFiredEvent {
    int fd;
    int mask;
} aeFiredEvent;

// 事件循环结构体
typedef struct aeEventLoop {
    int maxfd;                // 当前最大文件描述符
    int setsize;              // 能够追踪的最大文件描述符数量
    aeFileEvent *events;      // 已注册的事件
    aeFiredEvent *fired;      // 已触发的事件
    int stop;                 // 控制事件循环是否停止
    int epfd;                 // epoll 文件描述符
    struct epoll_event *epollEvents; // epoll 事件列表
} aeEventLoop;

aeEventLoop *aeCreateEventLoop(int setsize) {
    aeEventLoop *eventLoop = malloc(sizeof(*eventLoop));
    eventLoop->maxfd = -1;
    eventLoop->setsize = setsize;
    eventLoop->events = calloc(setsize, sizeof(aeFileEvent));
    eventLoop->fired = calloc(setsize, sizeof(aeFiredEvent));
    eventLoop->stop = 0;
    eventLoop->epfd = epoll_create(1024);
    eventLoop->epollEvents = calloc(setsize, sizeof(struct epoll_event));

    return eventLoop;
}

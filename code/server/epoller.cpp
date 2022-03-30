#include "epoller.h"

// 创建epoll对象 epoll_create(512)，返回值给epoll_Fd成员，初始化检测事件集合
Epoller::Epoller(int maxEvent):epollFd_(epoll_create(512)), events_(maxEvent){
    assert(epollFd_ >= 0 && events_.size() > 0);
}

Epoller::~Epoller() {
    close(epollFd_);
}

// 添加文件描述符到epoll中进行管理
bool Epoller::AddFd(int fd, uint32_t events) {  //events 检测什么事件（读or写...)
    if(fd < 0) return false;
    epoll_event ev = {0};  //创建epoll_event，初始化为0
    ev.data.fd = fd;  //初始化fd为传入的fd
    ev.events = events;  //初始化事件为要检测的事件
    return 0 == epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &ev);
}

// 修改
bool Epoller::ModFd(int fd, uint32_t events) {
    if(fd < 0) return false;
    epoll_event ev = {0};
    ev.data.fd = fd;
    ev.events = events;
    return 0 == epoll_ctl(epollFd_, EPOLL_CTL_MOD, fd, &ev);
}

// 删除
bool Epoller::DelFd(int fd) {
    if(fd < 0) return false;
    epoll_event ev = {0};
    return 0 == epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd, &ev);
}

// 调用epoll_wait()进行事件检测
int Epoller::Wait(int timeoutMs) {
    return epoll_wait(epollFd_, &events_[0], static_cast<int>(events_.size()), timeoutMs);  //检测到有数据到达后，将其放入events_数组中
}

// 获取产生事件的文件描述符
int Epoller::GetEventFd(size_t i) const {
    assert(i < events_.size() && i >= 0);
    return events_[i].data.fd;
}

// 获取事件
uint32_t Epoller::GetEvents(size_t i) const {
    assert(i < events_.size() && i >= 0);
    return events_[i].events;
}
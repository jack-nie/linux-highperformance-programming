#include <stdio.h>
#include <stdlib.h>
#include <event.h>

int lasttime;

static void
timeout_cb(int fd /*超时回调，没有用*/, 
short event /*libevent调用该回调时告知用户发生的事件,此处应该是EV_TIMEOUT*/,
void *arg /*注册的时候设置的参数*/)
{
  struct timeval tv;
  struct event *timeout = arg;
  int newtime = time(NULL);

  printf("%s: called at %d: %d\n", __func__, newtime,
      newtime - lasttime);
  lasttime = newtime;

  evutil_timerclear(&tv);
  tv.tv_sec = 2;
    //调用一次之后再注册该事件，2s之后通知我
    //如果不添加，libevent中就没有event会自动退出
  event_add(timeout, &tv);
}

int
main (int argc, char **argv)
{
    //libevent 的一个event，用于关联handle与callback
  struct event timeout; 
  struct timeval tv;

  /* Initalize the event library 初始化*/
  event_init();

  /* Initalize one event */
  evtimer_set(&timeout/*设置event*/, 
    timeout_cb /*回调函数，看原型*/, 
    &timeout/*调用回调函数时传给回调函数的参数 arg*/);

  evutil_timerclear(&tv); // 初始化事件结构体
  tv.tv_sec = 2;
  event_add(&timeout, &tv); //注册event，设置超时时间为2秒调用一次

  lasttime = time(NULL);

  event_dispatch();//运行libevent，进行事件分发，这里会阻塞

  return (0);
}
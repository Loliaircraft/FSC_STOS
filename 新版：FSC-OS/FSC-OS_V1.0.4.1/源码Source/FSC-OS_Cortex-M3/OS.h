#ifndef _OS_H_
#define _OS_H_
#include "stm32f10x.h"
#include <stdio.h>
#include "conf.h"
#include "type.h"
#include "mem.h"
#include "list.h"
#include "signal.h"
#include "global.h"
#include "method.h"
#include "shell.h"

//重封装printf函数
#define os_printf(fmt, ...) { os_thread_sched_lock(); if(os_shell_cmd.bool_enter_sys==os_false) printf( fmt, ##__VA_ARGS__); os_thread_sched_unlock();}//OS提供的打印输出函数

//timer链表
extern os_timer os_timer_struct_list_head;
extern os_timer os_timer_struct_list_rear;
extern os_timer *os_timer_list_head;
extern os_timer *os_timer_list_rear;
extern os_timer *os_thread_timer_list_head;
extern os_timer *os_thread_timer_list_rear;


//ACB链表
extern os_acb *os_acb_list_head;
extern os_acb *os_acb_list_rear;
extern os_acb *os_acb_cur;  //当前正在运行的应用程序ACB
extern os_acb *os_acb_rdy;  //下个即将运行的应用程序ACB


//TCB链表
extern os_tcb_prio_sort_table *os_tcb_prio_sort_table_list_head;
extern os_tcb_prio_sort_table *os_tcb_prio_sort_table_list_rear;

extern os_tcb *os_tcb_cur;  //当前正在运行的线程TCB
extern os_tcb *os_tcb_rdy;  //下个即将运行的线程TCB

extern os_para os_p;//系统变量
extern os_app_para os_app_p;
extern os_thread_para os_thread_p;
extern os_thread_prio_sort_table_para os_thread_prio_sort_table_p;
extern os_timer_para os_timer_p;

void os_main(void);
void os_core_enter(void);                             //关闭全局中断函数(在汇编中，用户不能使用)           
void os_core_exit(void);                              //打开全局中断函数(在汇编中，用户不能使用)  
void os_psp_reset(void);                              //初使化PSP指针(在汇编中) 
void os_init_and_startup(void);                       //系统初使化并启动函数(在汇编中) 
void os_thread_same_prio_scheduler(void);             //线程调度器(OS核心)
void os_thread_highest_prio_scheduler(void);          //线程调度器(OS核心)
void os_pendsv_pulse(void);                           //触发环境切换
void os_thread_same_prio_sched_and_switch(void);      //线程调度并切换函数  
void os_thread_highest_prio_sched_and_switch(void);   //线程调度并切换函数 
void os_timer_counter_process(void);                  //系统定时器程序
stk32* os_thread_stk_init(void* thread,stk32 *topstkptr); //线程堆栈初使化函数
void os_tcb_ptr_init(void);
os_type_app_id* os_app_new_create(void);
void os_app_new_init(os_type_app_id* app_id);				//新APP初使化
os_type_thread_id* os_thread_new_create(os_type_app_id* app,void* thread,os_str8 *str_name,os_u32 stk_size,os_u32 slice_tick,os_u32 prio,os_thread_state state,void *para);     //任务创建函数                           
os_tcb* os_tcb_same_prio_next_rdy_get(void);//从当前最高优先级组中获取下个顺序运行的线程
os_tcb* os_tcb_highest_prio_next_rdy_get(void);//先从最高优先级组中查找最高优先级分组，再从最高优先级分组中获取下个顺序运行的线程
os_u32 os_clock_counter_get(void);                    //获取系统时钟节拍数函数,clocks

//中断使能控制函数
void os_cpu_interrupt_disable(void);                  //关闭全局中断函数
void os_cpu_interrupt_enable(void);                   //打开全局中断函数

//线程切换锁定函数
void os_thread_sched_lock(void);                      //线程切换锁定函数                         
void os_thread_sched_unlock (void);                   //线程切换解锁函数

//存放用户初使化代码函数
void os_user_init(void);															//用户代码初使化(用户的初使化都放在此函数中)

//软件定时器
os_timer* os_timer_new_create(void (*timerout_callback)(void),os_timer_mode_type tmode,os_u32 time_ticks);//创建系统软定时器
void os_timer_delete(os_timer* timer); //系统软定时器删除
void os_timer_sleep(os_timer* timer); //系统软定时器休眠
void os_timer_relaod_set(os_timer* timer,os_u32 time_ticks); //设置软定时器自动重装值
void os_timer_value_set(os_timer* timer,os_u32 time_ticks); //设置软定时器定时值
os_bool os_timer_state_get(os_timer* timer); //系统软定时器定时状态获取

//APP、线程操作
os_error_code_type os_thread_suspend(os_acb *os_acb_app,os_tcb *os_tcb_thread);//线程挂起，挂起会调度一次
os_error_code_type os_thread_resume(os_acb *os_acb_app,os_tcb *os_tcb_thread);//线程恢复,恢复会调度一次
os_error_code_type os_app_suspend(os_acb *os_acb_app); //app挂起,挂起会调度一次
os_error_code_type os_app_resume(os_acb *os_acb_app); //app恢复,恢复会调度一次

//延时函数
void delay_us(os_u32 us);                            //微秒延时函数(阻塞，不调度)
void delay_ms(os_u32 ms);	                           //毫秒延时函数(阻塞，不调度)
void os_thread_delay(os_u32 time_ticks);             //线程延时函数(阻塞，调度)
#endif

#include "app_config.h"
#include "asm/includes.h"
#include "system/init.h"
#include "system/task.h"
#include "app_core.h"
#include "irq.h"
#include "version.h"
#include "jiffies.h"
#include "system/timer.h"


#include <unistd.h>
long sysconf(int name)
{
    if (name == _SC_NPROCESSORS_CONF) {
        return CPU_CORE_NUM;
    } else {
        printf("[%s, %d][%d]is not supported yet\n", __FUNCTION__, __LINE__, name);
    }
    return -1;
}

int errno;
__attribute__((used)) int *__errno(void)
{
    return &errno;
}

volatile unsigned long jiffies = 10;

#if 0
volatile unsigned long hi_jiffies = 10;
/*
 * 测试cpu0,cpu1 stack用多少空间
 * 使用list_for_stack_info查看stack使用空间前，先在main函数最先调用init_for_stack_info初始化stack
 */
extern unsigned int stack_info_begin[];
extern unsigned int _stack_cpu0[];
extern unsigned int _stack1_cpu0[];
extern unsigned int _stack_cpu1[];
extern unsigned int _stack1_cpu1[];
extern unsigned int stack_info_end[];

#define STACK_MAG	0x87654321

void init_for_stack_info(void)
{
    unsigned int *buf;
    for (buf = stack_info_begin; buf < stack_info_end; buf++) {
        *buf = STACK_MAG;
    }
}
void list_for_stack_info(void)
{
    unsigned int *buf;
    unsigned int all_size;
    unsigned int use_size;
    printf("\r\n\r\n----------------------------------------------------------------------\r\n\r\n");
    for (buf = stack_info_begin; buf < _stack_cpu0; buf++) {
        if (*buf != STACK_MAG) {
            all_size = (unsigned int)(_stack_cpu0 - stack_info_begin) * 4;
            use_size = (unsigned int)(_stack_cpu0 - buf) * 4;
            printf("stack_cpu0  addr : 0x%08x , all size : %08d, used : %08d \r\n", (u32)stack_info_begin, all_size, use_size);
            break;
        }
    }
    for (buf = _stack_cpu0; buf < _stack1_cpu0; buf++) {
        if (*buf != STACK_MAG) {
            all_size = (unsigned int)(_stack1_cpu0 - _stack_cpu0) * 4;
            use_size = (unsigned int)(_stack1_cpu0 - buf) * 4;
            printf("stack1_cpu0 addr : 0x%08x , all size : %08d, used : %08d \r\n", (u32)_stack_cpu0, all_size, use_size);
            break;
        }
    }
    for (buf = _stack1_cpu0; buf < _stack_cpu1; buf++) {
        if (*buf != STACK_MAG) {
            all_size = (unsigned int)(_stack_cpu1 - _stack1_cpu0) * 4;
            use_size = (unsigned int)(_stack_cpu1 - buf) * 4;
            printf("stack_cpu1  addr : 0x%08x , all size : %08d, used : %08d \r\n", (u32)_stack1_cpu0, all_size, use_size);
            break;
        }
    }
    for (buf = _stack_cpu1; buf < stack_info_end; buf++) {
        if (*buf != STACK_MAG) {
            all_size = (unsigned int)(stack_info_end - _stack_cpu1) * 4;
            use_size = (unsigned int)(stack_info_end - buf) * 4;
            printf("stack1_cpu1 addr : 0x%08x , all size : %08d, used : %08d \r\n", (u32)_stack_cpu1, all_size, use_size);
            break;
        }
    }
    printf("----------------------------------------------------------------------\r\n\r\n");
}
#endif

extern volatile char cpu1_run_flag;
#if CPU_CORE_NUM == 1
SEC_USED(.volatile_ram_code)
#endif
void cpu1_main(void)
{
    cpu1_run_flag = 1;//标记CPU1启动

    __local_irq_disable();

    interrupt_init();

    debug_init();

#if CPU_CORE_NUM > 1
    os_start();
#else
    puts("\r\n\n cpu1_run... \r\n\n");
#endif

    __local_irq_enable();

#if defined CONFIG_UCOS_ENABLE ||  (CPU_CORE_NUM == 1)
    //在这句话之后 不可用操作系统接口,printf/puts等打印接口,并且需要替换单核专用的system库,
    while (1) {
        __asm__ volatile("idle");
    }
#endif
}

void cpu_assert_debug()
{
#ifdef CONFIG_DEBUG_ENABLE
    log_flush();
    local_irq_disable();
#endif
    /*while (1);*/
    cpu_reset();
}

#ifdef RTOS_STACK_CHECK_ENABLE
static void rtos_stack_check_func(void *p)
{
#if 0
    char *pWriteBuffer = malloc(2048);
    if (!pWriteBuffer) {
        return;
    }
    extern void vTaskList(char *pcWriteBuffer);
    vTaskList(pWriteBuffer);
    printf(" \n\ntask_name          task_state priority stack task_num\n%s\n", pWriteBuffer);
    free(pWriteBuffer);
#else
    extern void get_task_state(void *parm);
    get_task_state(NULL); //1分钟以内调用一次才准确
#endif

    dump_cpu_irq_usage();
    /* dump_os_sw_cnt(); */
    malloc_stats();
#if configCPU_USAGE_CALCULATE
    extern unsigned char ucGetCpuUsage(unsigned char cpuid);
    printf("cpu0 use: %d%%, cpu1 use: %d%%\n", ucGetCpuUsage(0), ucGetCpuUsage(1));
#endif
    /*cpu_effic_calc();*/
}
#endif

#ifdef MEM_LEAK_CHECK_ENABLE
#include "mem_leak_test.h"
static void malloc_debug_dump(void *p)
{
    malloc_stats();
    malloc_debug_show();
    malloc_dump();
}
#endif

extern void setup_arch(void);
extern int sys_timer_init(void);
extern void app_main(void);
extern void sdfile_ext_mount_init(void);
void __attribute__((weak)) board_init() {}
void __attribute__((weak)) board_early_init() {}

static void app_task_handler(void *p)
{
    sys_timer_init();
    sys_timer_task_init();

#ifdef RTOS_STACK_CHECK_ENABLE
    sys_timer_add(NULL, rtos_stack_check_func, 60 * 1000);
#endif

#ifdef MEM_LEAK_CHECK_ENABLE
    malloc_debug_start();
    sys_timer_add(NULL, malloc_debug_dump, 60 * 1000);
#endif

    board_early_init();
    __do_initcall(early_initcall);
#ifdef CONFIG_SDFILE_EXT_ENABLE
    sdfile_ext_mount_init();
#endif
    __do_initcall(platform_initcall);
    board_init();

#ifdef RF_FCC_TEST_ENABLE
    u8 rf_fcc_test_init(void);
    if (rf_fcc_test_init()) {
#ifndef RF_FCC_IN_NORNAL_WORK
        while (1) {
            os_time_dly(10);
        }
#endif
    }
#endif

    __do_initcall(initcall);
    __do_initcall(module_initcall);
    app_core_init();
    __do_initcall(late_initcall);
    app_main();

#ifdef CONFIG_FINSH_ENABLE
    extern int finsh_system_init(void);
    finsh_system_init();
#endif

    int res;
    int msg[32];

    while (1) {
        res = os_task_pend("taskq", msg, ARRAY_SIZE(msg));
        if (res != OS_TASKQ) {
            continue;
        }
        app_core_msg_handler(msg);
    }
}

#if 0
/*__attribute__((always_inline))*/
//SEC_USED(.volatile_ram_code)
void early_putchar(char a)
{
    if (a == '\n') {
        JL_UART2->CON0 |= BIT(13);
        JL_UART2->BUF = '\r';
        __asm_csync();

        while ((JL_UART2->CON0 & BIT(15)) == 0);
    }
    JL_UART2->CON0 |= BIT(13);
    JL_UART2->BUF = a;
    __asm_csync();

    while ((JL_UART2->CON0 & BIT(15)) == 0);
}

void early_puts(char *s)
{
    do {
        early_putchar(*s);
    } while (*(++s));
}

#endif

int main()
{
    //init_for_stack_info();

    /* early_puts("WL8X SDK RUN !!!\r\n"); */

    __local_irq_disable();

    setup_arch();

#if CPU_CORE_NUM == 1
    EnableOtherCpu();
    //cpu1_run_flag = 0; //如果运行在SFC模式下,不希望写FLASH的时候会挂起CPU1,则打开这句话,但是需要保证cpu1_main调用到的所有函数全部放到内部ram或者sdram,防止写flash过程中引起死机
#endif

    os_init();

    task_create(app_task_handler, NULL, "app_core");

#if CPU_CORE_NUM == 1 //本来改 prvIdleTask
    __local_irq_enable();
#endif

    os_start();

#if CPU_CORE_NUM > 1
    __local_irq_enable();
#endif

#if defined CONFIG_UCOS_ENABLE
    while (1) {
        __asm__ volatile("idle");
    }
#endif

    return 0;
}

int eSystemConfirmStopStatus(void)
{
    /* 系统进入在未来时间里，无任务超时唤醒，可根据用户选择系统停止，或者系统定时唤醒(100ms) */
    //1:Endless Sleep
    //0:100 ms wakeup
    return 0;
}

#define CONFIG_BT_ENABLE_SPINLOCK	1

#if CONFIG_BT_ENABLE_SPINLOCK && CPU_CORE_NUM > 1

static volatile u32 bt_lock_cnt[CPU_CORE_NUM] SEC_USED(.volatile_ram) = {0};
static spinlock_t bt_lock SEC_USED(.volatile_ram) = {0};

__attribute__((always_inline))
void local_irq_disable(void)
{
    __local_irq_disable();
    if (bt_lock_cnt[current_cpu_id()]++ == 0) {
        arch_spin_lock(&bt_lock);
    }
}

__attribute__((always_inline))
void local_irq_enable(void)
{
    if (--bt_lock_cnt[current_cpu_id()] == 0) {
        arch_spin_unlock(&bt_lock);
    }
    __local_irq_enable();
}

__attribute__((used))
void __rt_local_irq_disable(void)
{
    local_irq_disable();
}

__attribute__((used))
void __rt_local_irq_enable(void)
{
    local_irq_enable();
}

#else

__attribute__((always_inline))
void local_irq_disable(void)
{
    sys_local_irq_disable();
}

__attribute__((always_inline))
void local_irq_enable(void)
{
    sys_local_irq_enable();
}

#endif

#ifdef CONFIG_IPMASK_ENABLE

//外部用到宏控制的函数, 库里面不能够内联
SEC_USED(.volatile_ram_code)
__attribute__((noinline))
void __local_irq_disable(void)
{
    __builtin_pi32v2_cli();
    q32DSP(current_cpu_id())->IPMASK = 7;//把中断优先级大于等于7的中断设定为不可屏蔽中断
    irq_lock_cnt[current_cpu_id()]++;
    __asm_csync();
    __builtin_pi32v2_sti();
}

//外部用到宏控制的函数, 库里面不能够内联
SEC_USED(.volatile_ram_code)
__attribute__((noinline))
void __local_irq_enable(void)
{
    if (--irq_lock_cnt[current_cpu_id()] == 0) {
        __builtin_pi32v2_cli();
        q32DSP(current_cpu_id())->IPMASK = 0;
        __asm_csync();
        __builtin_pi32v2_sti();
    }
}

//外部用到宏控制的函数, 库里面不能够内联
SEC_USED(.volatile_ram_code)
__attribute__((noinline))
int cpu_irq_disabled(void)
{
    int flag, flag2;
    __asm__ volatile("%0 = icfg" : "=r"(flag));
    int first = ((flag & 0x300) != 0x300) || (q32DSP(current_cpu_id())->IPMASK == 7);//不可屏蔽中断
    //只读一遍有可能出现实际上没关中断但又条件成立的情况，需要再读一次确保正确，读取前使用ssync也没效果，原因未知
    __asm__ volatile("%0 = icfg" : "=r"(flag2));
    int second = ((flag2 & 0x300) != 0x300) || (q32DSP(current_cpu_id())->IPMASK == 7);//不可屏蔽中断
    return (first && second);
}

#else

//外部用到宏控制的函数, 库里面不能够内联
SEC_USED(.volatile_ram_code)
__attribute__((noinline))
void __local_irq_disable(void)
{
    __builtin_pi32v2_cli();
    irq_lock_cnt[current_cpu_id()]++;
    __asm_csync();
}

//外部用到宏控制的函数, 库里面不能够内联
SEC_USED(.volatile_ram_code)
__attribute__((noinline))
void __local_irq_enable(void)
{
    if (--irq_lock_cnt[current_cpu_id()] == 0) {
        __asm_csync();
        __builtin_pi32v2_sti();
    }
}

//外部用到宏控制的函数, 库里面不能够内联
SEC_USED(.volatile_ram_code)
__attribute__((noinline))
int cpu_irq_disabled(void)
{
    int flag, flag2;
    __asm__ volatile("%0 = icfg" : "=r"(flag));
    int first = ((flag & 0x300) != 0x300);
    //只读一遍有可能出现实际上没关中断但又条件成立的情况，需要再读一次确保正确，读取前使用ssync也没效果，原因未知
    __asm__ volatile("%0 = icfg" : "=r"(flag2));
    int second = ((flag2 & 0x300) != 0x300);
    return (first && second);
}

#endif

__attribute__((always_inline))
int __local_irq_lock_cnt(void)
{
    __asm_csync();
    return irq_lock_cnt[current_cpu_id()];
}

__attribute__((always_inline))
int cpu_in_irq(void)
{
    int flag;
    __asm__ volatile("%0 = icfg" : "=r"(flag));
    return flag & 0xff;
}

__attribute__((always_inline))
void sys_local_irq_disable(void)
{
    __local_irq_disable();
    if (cpu_lock_cnt[current_cpu_id()]++ == 0) {
        asm volatile("lockset;");
    }
    __asm_csync();
}

__attribute__((always_inline))
void sys_local_irq_enable(void)
{
    __asm_csync();
    if (--cpu_lock_cnt[current_cpu_id()] == 0) {
        asm volatile("lockclr;");
    }
    __local_irq_enable();
}

// just fix build&link
#if !defined CONFIG_NET_ENABLE && !defined CONFIG_BT_ENABLE
__attribute__((weak)) void wf_set_phcom_cnt(u32 phcom_cnt)
{
}
#endif

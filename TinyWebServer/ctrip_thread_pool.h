#pragma once
/**
 *  �̳߳ع���, ctrip_thread_pool.h
 *  zhangyl 2018.03.23
 */

#ifndef __CTRIP_THREAD_POOL_H__
#define __CTRIP_THREAD_POOL_H__


// #pragma comment(lib,"pthreadVC2.lib")

#include "pthread.h"

#ifndef NULL
#define NULL 0
#endif

#define PUBLIC 

PUBLIC struct ctrip_task
{
    struct ctrip_task* pNext;
    int                  value;
};

struct ctrip_thread_info
{
    //�߳��˳���־
    int                    thread_running;
    int                    thread_num;
    int                    tasknum;
    struct ctrip_task* tasks;
    pthread_t* threadid;
    pthread_mutex_t        mutex;
    pthread_cond_t         cond;
};

/* ��ʼ���̳߳��߳���Ŀ
 * @param thread_num �߳���Ŀ, Ĭ��Ϊ8��
 */
PUBLIC void  ctrip_init_thread_pool(int thread_num);

/* �����̳߳�
 */
PUBLIC void  ctrip_destroy_thread_pool();

/**�������������һ������
 * @param t ��Ҫ���ӵ�����
 */
PUBLIC void ctrip_thread_pool_add_task(struct ctrip_task* t);

/**���������ȡ��һ������
 * @return ���صõ�������
 */
struct ctrip_task* ctrip_thread_pool_retrieve_task();

/**ִ��������е�����
 * @param t ��Ҫִ�е�����
 */
PUBLIC void ctrip_thread_pool_do_task(struct ctrip_task* t);

/**�̺߳���
 * @param thread_param �̲߳���
 */
void* ctrip_thread_routine(void* thread_param);

#endif //!__CTRIP_THREAD_POOL_H__
/**
 *  �̳߳ع���, ctrip_thread_pool.c
 *  zhangyl 2018.03.23
 */

#include "ctrip_thread_pool.h"
#include <stdio.h>
#include <stdlib.h>

struct ctrip_thread_info g_threadinfo;
int thread_running = 0;

void ctrip_init_thread_pool(int thread_num)
{
    if (thread_num <= 0)
        thread_num = 5;

    pthread_mutex_init(&g_threadinfo.mutex, NULL);
    pthread_cond_init(&g_threadinfo.cond, NULL);

    g_threadinfo.thread_num = thread_num;
    g_threadinfo.thread_running = 1;
    g_threadinfo.tasknum = 0;
    g_threadinfo.tasks = NULL;
    thread_running = 1;

    g_threadinfo.threadid = (pthread_t*)malloc(sizeof(pthread_t) * thread_num);

    int i;
    for (i = 0; i < thread_num; ++i)
    {
        pthread_create(&g_threadinfo.threadid[i], NULL, ctrip_thread_routine, NULL);
    }
}

void ctrip_destroy_thread_pool()
{
    g_threadinfo.thread_running = 0;
    thread_running = 0;
    pthread_cond_broadcast(&g_threadinfo.cond);

    int i;
    for (i = 0; i < g_threadinfo.thread_num; ++i)
    {
        pthread_join(g_threadinfo.threadid[i], NULL);
    }

    free(g_threadinfo.threadid);

    pthread_mutex_destroy(&g_threadinfo.mutex);
    pthread_cond_destroy(&g_threadinfo.cond);
}

void ctrip_thread_pool_add_task(struct ctrip_task* t)
{
    if (t == NULL)
        return;

    pthread_mutex_lock(&g_threadinfo.mutex);

    struct ctrip_task* head = g_threadinfo.tasks;
    if (head == NULL)
        g_threadinfo.tasks = t;
    else
    {
        while (head->pNext != NULL)
        {
            head = head->pNext;
        }

        head->pNext = t;
    }

    ++g_threadinfo.tasknum;
    //���б仯��ʹ��signal֪ͨwait����
    pthread_cond_signal(&g_threadinfo.cond);
    pthread_mutex_unlock(&g_threadinfo.mutex);
}


struct ctrip_task* ctrip_thread_pool_retrieve_task()
{
    struct ctrip_task* head = g_threadinfo.tasks;
    if (head != NULL)
    {
        g_threadinfo.tasks = head->pNext;
        --g_threadinfo.tasknum;
        printf("retrieve a task, task value is [%d]\n", head->value);
        return head;
    }

    printf("no task\n");

    return NULL;
}

void* ctrip_thread_routine(void* thread_param)
{
    printf("thread NO.%d start.\n", (int)pthread_self());

    while (thread_running/*g_threadinfo.thread_running*/)
    {
        struct ctrip_task* current = NULL;

        pthread_mutex_lock(&g_threadinfo.mutex);

        while (g_threadinfo.tasknum <= 0)
        {
            //�������˻��������������������ʵĻ���wait���ͷ�����������ִ�С�
            //���仯���������ʣ���ֱ�ӻ������
            pthread_cond_wait(&g_threadinfo.cond, &g_threadinfo.mutex);

            if (!g_threadinfo.thread_running)
                break;

            current = ctrip_thread_pool_retrieve_task();

            if (current != NULL)
                break;
        }// end inner-while-loop

        pthread_mutex_unlock(&g_threadinfo.mutex);

        ctrip_thread_pool_do_task(current);
    }// end outer-while-loop

    printf("thread NO.%d exit.\n", (int)pthread_self());
}


void ctrip_thread_pool_do_task(struct ctrip_task* t)
{
    if (t == NULL)
        return;


    //TODO: do your work here
    printf("task value is [%d]\n", t->value);
    //TODO�����t��Ҫ�ͷţ��ǵ��������ͷ�
}
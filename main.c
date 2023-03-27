#include <stdio.h>
#include "string.h"

/**
 * @brief 进程控制块
 * 
 */
typedef struct PCB
{
    /* data */
    int id;
    int arrival_time;
    int service_time;
    int start_time;
    int finish_time;
    float turn_around_time;
    float weight_around_time;
    float response_ratio;
}PCB_t;

void vGetData(PCB_t process[], int n);
void vShowData(PCB_t process[], int n);
void vSortByArrivalTime(PCB_t process[], int n);
void vProcessStartScheduler(PCB_t process[], int n);
void vResumeOrderInId(PCB_t process[], int n);

/* 全局变量声明 */
unsigned int current_time = 0;

int main(){
    printf("Highest Response Ratio Next调度算法\r\n");

    int n;

    printf("输入进程个数：");
    scanf("%d", &n);

    PCB_t process[n];

    /* 初始化结构体数组 */
    memset(process, 0, sizeof(process));

    vGetData(process, n);
    vSortByArrivalTime(process, n);
    vProcessStartScheduler(process, n);
    vResumeOrderInId(process, n);
    vShowData(process, n);
    return 0;
}

/**
 * @brief Get the Process Data
 * 
 * @param process 
 * @param n 
 */
void vGetData(PCB_t process[], int n)
{
    for(int i = 0; i < n; i++)
    {
        printf("输入第 %d 个进程的到达时间和服务时间：", i + 1);
        scanf("%d%d", &process[i].arrival_time, &process[i].service_time);
        process[i].id = i + 1;
    }
}

/**
 * @brief Show the Chart of Data
 * 
 * @param process 
 * @param n 
 */
void vShowData(PCB_t process[], int n)
{
    for(int i = 0; i < n; i++)
    {
        printf("Id\t  Arrival time\t   Service time\t   Start time\t   Finish time\t  Turn around time\tWeight around time\r\n");
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t  %.2f\t\t\t%.2f\r\n", process[i].id, process[i].arrival_time, 
        process[i].service_time, process[i].start_time, process[i].finish_time, 
        process[i].turn_around_time, process[i].weight_around_time);
    }
    float avg_turn_around_time = 0;
    float avg_weight_around_time = 0;
    float turn_around_time_sum, weight_around_sum;
    for(int i = 0; i < n; i++)
    {
        turn_around_time_sum += process[i].turn_around_time;
        weight_around_sum += process[i].weight_around_time;
    }

    avg_turn_around_time = turn_around_time_sum / n;
    avg_weight_around_time = weight_around_sum / n;

    printf("Average Turn Around Time = %.2f s\r\n", avg_turn_around_time);

    printf("Average Weight Around Time = %.2f s\r\n", avg_weight_around_time);
}

/**
 * @brief Sort By Service Time
 * 
 * @param process 
 * @param n 
 */
void vSortByArrivalTime(PCB_t process[], int n)
{
    /* 到达时间升序排序 */
    for(int i = 0; i < n - 1; i++){
        for(int j = 0; j < n - i - 1; j++){
            if (process[j].arrival_time > process[j + 1].arrival_time){
                PCB_t temp = process[j];
                process[j] = process[j + 1];
                process[j + 1] = temp;
            }
        }
    }
}

/**
 * @brief Start Schedule
 * 
 * @param process 
 * @param n 
 */
void vProcessStartScheduler(PCB_t process[], int n)
{
    PCB_t Process_TempCompare[n];
    memset(Process_TempCompare, 0, sizeof(Process_TempCompare));
    current_time += process[0].arrival_time;    
    int arrival_time_Compare, Id_compare, count;
    
    for(int i = 0; i < n; i++)
    {
        // /* 记录到达时间相同的结构体元素和个数（只取一个元素） */
        // for(int j = 0; j < n; j++)
        // {
        //     /* 判断该进程是否被执行 */
        //     if(process[j].start_time == 0 && process[j].finish_time == 0)
        //     {
        //         /* 放入比较结构体数组 */
        //         Process_TempCompare[0] = process[j];
        //         /* 记录到达时间 */
        //         arrival_time_Compare = process[j].arrival_time;
        //         /* 记录id */
        //         Id_compare = process[j].id;
        //         break;
        //     }
        // }
        // /* 取出到达时间相等的进程 */
        // int position = 0;
        // for(int j = 0; j < n; j++)
        // {
        //     if((process[j].start_time == 0) && (process[j].finish_time == 0))
        //     {
        //         if((process[j].arrival_time == arrival_time_Compare) && (process[j].id != Id_compare))
        //         {
        //             position ++;
        //             Process_TempCompare[position] = process[j];
        //             count ++;
        //         }
        //     }
        // }
        int position = 0;
        count = 0;
        for(int j = 0; j < n; j++)
        {
            if((process[j].arrival_time <= current_time) && (process[j].turn_around_time == 0))
            {
                Process_TempCompare[position] = process[j];
                position ++;
                count ++;
            }
        }
        if(count > 1)
        {
            for(int j = 0; j < count; j++)
            {
                Process_TempCompare[j].response_ratio = (float)1 + (((float)current_time - (float)Process_TempCompare[j].arrival_time) / (float)Process_TempCompare[j].service_time);
            }
            /* 响应比降序排序 */
            for(int j = 0; j < count - 1; j++)
            {
                for(int k = 0; k < count - j - 1; k++)
                {
                    if(Process_TempCompare[k].response_ratio < Process_TempCompare[k + 1].response_ratio)
                    {
                        PCB_t temp = Process_TempCompare[k];
                        Process_TempCompare[k] = Process_TempCompare[k + 1];
                        Process_TempCompare[k + 1] = temp;
                    }                
                }
            }
        }
        Process_TempCompare[0].start_time = current_time;
        current_time += Process_TempCompare[0].service_time;
        Process_TempCompare[0].finish_time = current_time;
        Process_TempCompare[0].turn_around_time = Process_TempCompare[0].finish_time - Process_TempCompare[0].arrival_time;
        Process_TempCompare[0].weight_around_time = Process_TempCompare[0].turn_around_time / (float)Process_TempCompare[0].service_time;

        /* 在原进程结构体函数中查找对应id */
        for(int j = 0; j < n; j++)
        {
            if(process[j].id == Process_TempCompare[0].id)
            {
                process[j] = Process_TempCompare[0];
                break;
            }
        }
    }
}

void vResumeOrderInId(PCB_t process[], int n)
{
    for(int i = 0; i < n - 1; i++){
        for(int j = 0; j < n - i - 1; j++){
            if (process[j].id > process[j + 1].id){
                PCB_t temp = process[j];
                process[j] = process[j + 1];
                process[j + 1] = temp;
            }
        }
    }
}

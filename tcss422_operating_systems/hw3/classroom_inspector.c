#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include "classroom_inspector.h"
#include "util.h"

typedef enum _days {
    MON, TUE, WED, THU, FRI, DNE
} day;

typedef struct _time_scheule {
    pthread_spinlock_t locks[5];
    char ** days[5];
    unsigned int idx[5];
    const char * building, * room;
} time_schedule;

typedef struct _parse_info {
    char ** URLs, ** html_pages;
    unsigned int bounds[2];
    time_schedule * times;
} parse_info;


int increaseAllocation(char ** array,
                        int * size)
{
    int newsize = *size * 2;
    char ** newArray;
    newArray = realloc(array, newsize*sizeof(char*));
    if (newArray == NULL) {
        printf("Memory reallocation failed\n");
        return -1;
    }
    *size = newsize;
    array = newArray;
    return 1;
}

int strcmpin(char * str1,
             const char * str2,
             const size_t n)
{
    int i;
    char ch1, ch2;
    for (i = 0; i < n; i++) {
        ch1 = tolower(str1[i]);
        ch2 = tolower(str2[i]);
        if (ch1 != ch2)
            return 1;
    }  
    return 0;
}

void initialize_schedule(time_schedule ** schedule,
                         const char * building,
                         const char * room) 
{
    (*schedule) = malloc(sizeof(time_schedule)); 
    (*schedule)->building = building;
    (*schedule)->room = room;
    int i;
    for (i = 0; i < 5; i++) {
        (*schedule)->idx[i] = 0;
        (*schedule)->days[i] = malloc(12 * sizeof(char *));
        pthread_spin_init(&(*schedule)->locks[i], 0);
    }   
}

unsigned int get_start_time(char * time) 
{
    unsigned int hour, minute, digits = 0, len = strlen(time);
    if (len < 7) {
        printf("[ERROR] Invalid time for get_start_time\n");
        return 0;
    }

    while (time[digits] != '-') ++digits;
    if (digits == 3) {
        hour = (time[0] - '0');
        minute = (10*(time[1] - '0') + (time[2] - '0'));
    } else if (digits == 4) {
        hour = (10 + (time[1] - '0'));
        minute = (10*(time[2] - '0') + (time[3] - '0'));
    }
    if (hour < 6)
        hour += 12;
    while (digits < 10) {
        if (time[digits++] == 'P')
            hour += 12;
    }
    return (hour * 60) + minute;    
}

void sort_day(char ** times,
             unsigned int size) 
{
    unsigned int i, j;
    char * temp;

    for (i = 0; i < size; i++) {
        for (j = 0; j < size-1; j++) {
            if (get_start_time(times[j]) > get_start_time(times[j+1])) {
                temp = times[j];
                times[j] = times[j+1];
                times[j+1] = temp;
            }
        }
    }
}

void sort_schedule(time_schedule * schedule) {
    int i;
    for (i = 0; i < 5; i++)
        sort_day(schedule->days[i], schedule->idx[i]);
}

void print_day(char ** day_sched,
               unsigned int size,
               day d)
{
    int i;
    char * abbr;
    switch(d) {
    case MON: abbr = "M"; break;
    case TUE: abbr = "T"; break;
    case WED: abbr = "W"; break;
    case THU: abbr = "Th"; break;
    case FRI: abbr = "F"; break;
    default: perror("Invalid Day"); return;
    }
    if (size)
        printf("%d  %s\n", d, day_sched[0]);
    for (i = 1; i < size; i++)
        printf("   %s\n", day_sched[i]);
}

void print_schedule(time_schedule * schedule) {
    int i;
    for (i = 0; i < 5; i++) {
        print_day(schedule->days[i],
                  schedule->idx[i],
                  i);
    }
}

void add_class(time_schedule * schedule,
               char * webpage,
               unsigned int class_idx,
               unsigned int times_idx,
               unsigned int class_len,
               unsigned int times_len,
               day d) 
{
    char * class;
    unsigned int i, * idx;

    if (d >= DNE) {
        printf("[ERROR] Invalid day\n");
        return;
    }

    class = malloc(24 * sizeof(char));
    sprintf(class, "%-12.*s %.*s", times_len,
                                   webpage+times_idx,
                                   class_len,
                                   webpage+class_idx);

    for (i = 0; i < schedule->idx[d]; i++) {
        if (strcmp(class, schedule->days[d][i]) == 0)
            return;
    }
    //printf("class : %s day = %d\n", class, d);
    pthread_spin_lock(&schedule->locks[d]);
    idx = &schedule->idx[d];
    schedule->days[d][(*idx)++] = class;
    //printf("[%d][%d] %s\n", d, *idx-1, class);
    pthread_spin_unlock(&schedule->locks[d]);
}              

void parse_page(const char * building,
                const char * room,
                char * classpage,
                time_schedule * schedule)
{
    unsigned int i, p_len, b_len, r_len, t_len, n_len, d_len;
    unsigned int t_idx, n_idx, d_idx;
    char * walker;
    day temp_day;
    p_len = strlen(classpage);
    b_len = strlen(building);
    r_len = strlen(room);
    t_len = 0;

    walker = classpage;
    for (i = 0; i < p_len; i++) {
        if (strncmp(walker+i, "A NAME=", 7) == 0) {
             i += 7;
             n_idx = i;
             n_len = 0;
             while(walker[i++] != '>') ++n_len;
        }
        if (strncmp(walker+i, building, b_len) == 0) {
            t_len = 0;
            i += b_len;
            t_len += b_len;
            while(walker[i] == ' ' || walker[i] == '\t'){++i;  ++t_len;}
            if (strncmp(walker+i, room, r_len) == 0) {
                i -= (t_len + 1);
                t_len = 0;
                d_len = 0;
                while(walker[i - 1] == ' ') --i;
                while(walker[i - 1] != ' ') {
                    ++t_len;
                    --i;
                }
                t_idx = i;
                while(walker[i - 1] == ' ') --i;
                while(walker[i - 1] != ' ') {
                    ++d_len;
                    --i;
                }
                //printf("%.5s-len=%d\n", walker+i, d_len);
                d_idx = i;
                while (i < (d_idx+d_len)) {
                    if (strcmpin(walker+i, "F", 1) == 0) temp_day = FRI;
                    else if (strcmpin(walker+i, "Th", 2) == 0) temp_day = THU;
                    else if (strcmpin(walker+i, "W", 1) == 0) temp_day = WED;
                    else if (strcmpin(walker+i, "T", 1) == 0) temp_day = TUE;
                    else if (strcmpin(walker+i, "M", 1) == 0) temp_day = MON;
                    else temp_day = DNE;
                    
                    if (temp_day != DNE)
                    add_class(schedule, 
                              classpage, 
                              n_idx,
                              t_idx,
                              n_len,
                              t_len,
                              temp_day);
                    ++i;
                }
               // while (i < i+d_len) {
                    //if (strcmpin(walker+i, "M", 1) == 0) printf("shit is on mon\n");
               //     ++i;
               // }
                //printf("%-12.*s%.*s\n", t_len, walker+i, n_len, walker+n_idx);
                i += (d_len + t_len + b_len + r_len + 50);
            }
        }
    } //printf("%s\n", classpage);
}

void * inspect_page(void * params)
{
    parse_info* r_info = params;
    int i;
    //printf("bounds: %d to %d\n", r_info->bounds[0], r_info->bounds[1]);
    for (i = r_info->bounds[0]; i < r_info->bounds[1]; i++) {
        parse_page(r_info->times->building,
                   r_info->times->room,
                   r_info->html_pages[i],
                   r_info->times);
        //printf("page %d\n", i);
    }   
}

void * retrieve_page(void * params)
{
    parse_info* r_info = params;
    int i;
    //printf("left = %d, right = %d", r_info->left, r_info->right);
    for (i = r_info->bounds[0]; i < r_info->bounds[1]; i++) {
        r_info->html_pages[i] = getContent(r_info->URLs[i]);
        if (r_info->html_pages[i] == NULL); //pthread error exit
    }
}

void inspect_classroom(const char * building, 
                       const char * room, 
                       const char * time_schedule_url, 
                       int retrieval_threads, 
                       int analysis_threads) 
{
    char buffer[80], * content, * walker;
    unsigned int len, idx, link_idx, k, webpage_size;
    idx = (retrieval_threads > analysis_threads)
                ? retrieval_threads : analysis_threads;
    pthread_t r_threads[idx];
    parse_info r_info[idx];
    time_schedule * schedule;

    webpage_size = 64;
    char ** webpages;
    char ** webpageURLs = malloc((webpage_size + 1) * sizeof(char*));

    if (getMIMEType(time_schedule_url, buffer, 80)) {
        printf("%s\n\n", buffer);
	if ((content = getContent(time_schedule_url)) != NULL) {
            printf("%s\n", content);
            //free(content);
        }
    }

    len = strlen(content);
    walker = content;
    k = 0;
    for (idx = 0; idx < len; idx++) {
        if (strcmpin(walker+idx, "<li><a href=", 12) == 0) {
            if (k == webpage_size) {
                printf("increase! %d %d\n", k, webpage_size);
                increaseAllocation(webpageURLs, &webpage_size);
            }

            idx += 12;
            link_idx = strlen(time_schedule_url);

            char * tempLink = malloc((link_idx + 12) * sizeof(char));

            strcpy(tempLink, time_schedule_url);
          
            while (walker[idx] != '>')
                tempLink[link_idx++] = walker[idx++];
            tempLink[link_idx] = '\0';

            webpageURLs[k++] = tempLink;
        }
    }
 
    webpages = malloc(k * sizeof(char *));
    initialize_schedule(&schedule, building, room);
///*
    for (idx = 0; idx < retrieval_threads; idx++) {
        r_info[idx].URLs = webpageURLs;
        r_info[idx].html_pages = webpages;
        r_info[idx].bounds[0] = ((k/retrieval_threads)*idx);
        r_info[idx].bounds[1] = ((k/retrieval_threads)*(idx+1));
        pthread_create(&r_threads[idx], NULL, &retrieve_page, &r_info[idx]);
        //printf("%d %s\n", idx, webpageURLs[idx]);
        //printf("%s\n", webpages[idx]);
    }
    for (idx = 0; idx < retrieval_threads; idx++)
        pthread_join(r_threads[idx], NULL);

    for (idx = 0; idx < analysis_threads; idx++) {
        r_info[idx].URLs = webpageURLs;
        r_info[idx].html_pages = webpages;
        r_info[idx].bounds[0] = ((k/analysis_threads)*idx);
        r_info[idx].bounds[1] = ((k/analysis_threads)*(idx+1));
        r_info[idx].times = schedule;
        pthread_create(&r_threads[idx], NULL, &inspect_page, &r_info[idx]);
    }

    for (idx = 0; idx < analysis_threads; idx++)
        pthread_join(r_threads[idx], NULL);

    sort_schedule(schedule);
    print_schedule(schedule);
//*/

}


/*
===============================================================================
*                                                                             *
* Author:      Jesse Bannon                                                   *
* Date:        04/25/15                                                       *
* Class:       TCSS 342: Data Structures                                      *
* School:      University of Washington Tacoma                                *
* Desc:        Parses UW Tacoma class pages and displays all classes          *
*              in a particular classroom.                                     *
* Copyright:   Use for educational purposes only.                             *
*                                                                             *
===============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include "classroom_inspector.h"
#include "util.h"

/*
===============================================================================
= Structures and Enums  =======================================================
===============================================================================
*/

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
    int rv;
} parse_info;

/*
===============================================================================
= Function Prototypes =========================================================
===============================================================================
*/


/* Essential Function */
void inspect_classroom(const char * building, 
                       const char * room, 
                       const char * time_schedule_url, 
                       int retrieval_threads, 
                       int analysis_threads);


/* Parsing Functions */
int getURLS(const char * building,
            const char * room,
            const char * time_schedule_url,
            char * ** webpageURLs);

void * retrieve_page(void * params);

void * inspect_page(void * params);

int parse_page(const char * building,
               const char * room,
               char * classpage,
               time_schedule * schedule);

int add_class(time_schedule * schedule,
              char * webpage,
              unsigned int class_idx,
              unsigned int times_idx,
              unsigned int class_len,
              unsigned int times_len,
              day d);


/* Printing Functions */
void sort_schedule(time_schedule * schedule);

void sort_day(char ** times,
              unsigned int size);

void print_day(char ** day_sched,
               unsigned int size,
               day d);

void print_schedule(time_schedule * schedule);


/* Utility Functions */
int initialize_schedule(time_schedule ** schedule,
                         const char * building,
                         const char * room);

unsigned int get_start_time(char * time);

int increaseAllocation(char * ** array,
                       unsigned int * size);

int strcmpin(char * str1,
             const char * str2,
             const size_t n);


/*
===============================================================================
= Functions ===================================================================
===============================================================================
*/


/* ==================================================================== *
 * inspect_classroom:                                                   *
 * Searches the entire UW Tacoma class listing for classes within the   *
 * specified building and room. Prints findings when complete.          *
 * ==================================================================== */
void inspect_classroom(const char * building, 
                       const char * room, 
                       const char * time_schedule_url, 
                       int retrieval_threads, 
                       int analysis_threads) 
{
    char * content, * walker, ** webpages, ** webpageURLs;
    unsigned int len, idx, link_idx, k, webpage_size, url_size;
    int rv;
    time_schedule * schedule;

    idx = (retrieval_threads > analysis_threads)
                ? retrieval_threads : analysis_threads;
    pthread_t r_threads[idx];
    parse_info r_info[idx];
    
    url_size = getURLS(building, room, time_schedule_url, &webpageURLs);
    if (url_size < 0) {
        printf("an error occured.\n");
        return;
    }

    webpages = malloc(url_size * sizeof(char *));

    if (webpages == NULL) {
        perror("malloc error: ");
        return;
    }

    rv = initialize_schedule(&schedule, building, room);
    if (rv != 0) {
        perror("initialize_schedule error: ");
        return;
    }

    for (idx = 0; idx < retrieval_threads; idx++) {
        r_info[idx].URLs = webpageURLs;
        r_info[idx].html_pages = webpages;
        r_info[idx].bounds[0] = ((url_size/retrieval_threads)*idx);
        r_info[idx].bounds[1] = ((url_size/retrieval_threads)*(idx+1));
        r_info[idx].rv = 0;
        rv = pthread_create(&r_threads[idx], NULL, &retrieve_page, &r_info[idx]);
        if (rv != 0) {
            perror("pthread_create error: ");
            return;
        }
    }

    for (idx = 0; idx < retrieval_threads; idx++) {
        pthread_join(r_threads[idx], NULL);
        if (r_info[idx].rv < 0) {
            printf("an error occured.\n");
            return;
        }
    }

    for (idx = 0; idx < analysis_threads; idx++) {
        r_info[idx].URLs = webpageURLs;
        r_info[idx].html_pages = webpages;
        r_info[idx].bounds[0] = ((url_size/analysis_threads)*idx);
        r_info[idx].bounds[1] = ((url_size/analysis_threads)*(idx+1));
        r_info[idx].times = schedule;
        r_info[idx].rv = 0;
        rv = pthread_create(&r_threads[idx], NULL, &inspect_page, &r_info[idx]);
        if (rv != 0) {
            perror("pthread_create error: ");
            return;
        }
    }

    for (idx = 0; idx < analysis_threads; idx++) {
        pthread_join(r_threads[idx], NULL);
        if (r_info[idx].rv < 0) {
            printf("an error occured.\n");
            return;
        }
    }

    sort_schedule(schedule);
    print_schedule(schedule);
}


/* ==================================================================== *
 * increaseAllocation:                                                  *
 * Doubles char pointer array allocation. Used for increasing class     *
 * string pointers.                                                     *
 * ==================================================================== */
int increaseAllocation(char * ** array,
                       unsigned int * size)
{
    *array = realloc(*array, *size * 2 * sizeof(char*));
    if (array == NULL) {
        perror("realloc error: ");
        return -1;
    }
    *size *= 2;
    return 0;
}

/* ==================================================================== *
 * strcmpin:                                                            *
 * Compares strings of length n, case insensitive.                      *
 * ==================================================================== */
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

/* ==================================================================== *
 * initialize_schedule:                                                 *
 * Initializes a schedule pointer. Allocates 12 pointers to a single    *
 * day.                                                                 *
 * ==================================================================== */
int initialize_schedule(time_schedule ** schedule,
                         const char * building,
                         const char * room)
{
    int rv;
    (*schedule) = malloc(sizeof(time_schedule));
    if (*schedule == NULL) {
        perror("malloc error: ");
        return 1;
    }

    (*schedule)->building = building;
    (*schedule)->room = room;
    int i;
    for (i = 0; i < 5; i++) {
        (*schedule)->idx[i] = 0;
        (*schedule)->days[i] = malloc(12 * sizeof(char *));
        rv = pthread_spin_init(&(*schedule)->locks[i], 0);
        if (rv != 0) {
            perror("pthread_spin_init error: ");
            return 1;
        }
    }
    return 0;   
}

/* ==================================================================== *
 * get_start_time:                                                      *
 * Returns military time value of a start time within a class string.   *
 * ==================================================================== */
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
    if (hour < 5)
        hour += 12;
    while (digits < 10) {
        if (time[digits++] == 'P')
            hour += 12;
    }
    return (hour * 60) + minute;    
}

/* ==================================================================== *
 * sort_day:                                                            *
 * Sorts a day's char array by start time.                              *
 * ==================================================================== */
void sort_day(char ** times,
             unsigned int size) 
{
    unsigned int i, j;
    char * temp, swap;

    for (i = 0; i < size; i++) {
        swap = 0;
        for (j = 0; j < size-1; j++) {
            if (get_start_time(times[j]) > get_start_time(times[j+1])) {
                temp = times[j];
                times[j] = times[j+1];
                times[j+1] = temp;
                swap = 1;
            }
        }
        if (!swap)
            return;
    }
}

/* ==================================================================== *
 * sort_schedule:                                                       *
 * Sorts the entire week schedule by start time of the class.           *
 * ==================================================================== */
void sort_schedule(time_schedule * schedule) {
    int i;
    for (i = 0; i < 5; i++)
        sort_day(schedule->days[i], schedule->idx[i]);
}

/* ==================================================================== *
 * print_day:                                                           *
 * Prints a particular day's schedule.                                  *
 * ==================================================================== */
void print_day(char ** day_sched,
               unsigned int size,
               day d)
{
    int i;
    char * abbr;
    switch(d) {
    case MON: abbr = "M \0"; break;
    case TUE: abbr = "T \0"; break;
    case WED: abbr = "W \0"; break;
    case THU: abbr = "Th\0"; break;
    case FRI: abbr = "F \0"; break;
    default: perror("Invalid Day"); return;
    }
    if (size)
        printf("%s %s\n", abbr, day_sched[0]);
    for (i = 1; i < size; i++)
        printf("   %s\n", day_sched[i]);
}

/* ==================================================================== *
 * print_schedule:                                                      *
 * Prints the entire classroom schedule for the week.                   *
 * ==================================================================== */
void print_schedule(time_schedule * schedule) {
    int i;
    for (i = 0; i < 5; i++) {
        print_day(schedule->days[i],
                  schedule->idx[i],
                  i);
    }
}

/* ==================================================================== *
 * add_class:                                                           *
 * Adds a class from the webpage into the time_schedule.                *
 * ==================================================================== */
int add_class(time_schedule * schedule,
               char * webpage,
               unsigned int class_idx,
               unsigned int times_idx,
               unsigned int class_len,
               unsigned int times_len,
               day d) 
{
    char * class;
    unsigned int i, * idx;
    int rv;

    if (d >= DNE) {
        printf("[ERROR] Invalid day\n");
        return 1;
    }

    class = malloc(24 * sizeof(char));
    if (class == NULL) {
        perror("malloc error :");
        return 1;
    }

    rv = sprintf(class, "%-12.*s %.*s", times_len,
                                        webpage+times_idx,
                                        class_len,
                                        webpage+class_idx);
    if (rv < 0) {
        perror("sprintf error: ");
        return 1;
    }

    for (i = 0; i < schedule->idx[d]; i++) {
        if (strcmp(class, schedule->days[d][i]) == 0)
            return 0;
    }

    /* Critical Section */
    pthread_spin_lock(&schedule->locks[d]);
    idx = &schedule->idx[d];
    schedule->days[d][(*idx)++] = class;
    pthread_spin_unlock(&schedule->locks[d]);
    /* End of Critical Section */
    return 0;
}              

/* ==================================================================== *
 * parse_page:                                                          *
 * Parses a major's class list and adds any times for the particular    *
 * building and room to the time_schedule.                              *
 * ==================================================================== */
int parse_page(const char * building,
               const char * room,
               char * classpage,
               time_schedule * schedule)
{
    unsigned int i, p_len, b_len, r_len, t_len, n_len, d_len;
    unsigned int t_idx, n_idx, d_idx;
    int rv;
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

                d_idx = i;
                while (i < (d_idx+d_len)) {
                    if (strcmpin(walker+i, "F", 1) == 0) temp_day = FRI;
                    else if (strcmpin(walker+i, "Th", 2) == 0) temp_day = THU;
                    else if (strcmpin(walker+i, "W", 1) == 0) temp_day = WED;
                    else if (strcmpin(walker+i, "T", 1) == 0) temp_day = TUE;
                    else if (strcmpin(walker+i, "M", 1) == 0) temp_day = MON;
                    else temp_day = DNE;
                    
                    if (temp_day != DNE)
                    rv = add_class(schedule, 
                                   classpage, 
                                   n_idx,
                                   t_idx,
                                   n_len,
                                   t_len,
                                   temp_day);
                    if (rv != 0) {
                        perror("add_class error: ");
                        return -1;
                    }
                    ++i;
                }
                i += (d_len + t_len + b_len + r_len + 20);
            }
        }
    }
    return 0;
}

/* ==================================================================== *
 * inspect_page:                                                        *
 * Threaded function to parse a set of class pages. Input parse_info    *
 * struct.                                                              *
 * ==================================================================== */
void * inspect_page(void * params)
{
    parse_info* r_info = params;
    int i, rv;
    for (i = r_info->bounds[0]; i < r_info->bounds[1]; i++) {
        rv = parse_page(r_info->times->building,
                        r_info->times->room,
                        r_info->html_pages[i],
                        r_info->times);
        if (rv < 0) {
            printf("an error occured.\n");
            r_info->rv = -1;
            return;
        }
    }   
}

/* ==================================================================== *
 * retrieve_page:                                                       *
 * Threaded function to retrieve webpages to major's class list. Input  *
 * parse_info struct.                                                   *
 * ==================================================================== */
void * retrieve_page(void * params)
{
    parse_info* r_info = params;
    int i;

    for (i = r_info->bounds[0]; i < r_info->bounds[1]; i++) {
        r_info->html_pages[i] = getContent(r_info->URLs[i]);
        if (r_info->html_pages[i] == NULL) {
            printf("error: could not load webpage.\n");
            r_info->rv = -1;
            return;
        }
    }
}


int getURLS(const char * building,
            const char * room,
            const char * time_schedule_url,
            char * ** webpageURLs)
{
    unsigned int len, idx, link_idx, webpage_count, url_len, url_size;
    char *webpage;
    int rv;

    if ((webpage = getContent(time_schedule_url)) == NULL) {
        printf("error: could not load webpage.\n");
        return -1;
    }

    url_size = 64;
    url_len = strlen(time_schedule_url);
    *webpageURLs = malloc(url_size * sizeof(char*));
    if (*webpageURLs == NULL) {
        perror("malloc error: ");
        return -1;
    }
    len = strlen(webpage);

    webpage_count = 0;

    for (idx = 0; idx < len; idx++) {
        if (strcmpin(webpage+idx, "<li><a href=", 12) == 0) {
            if (webpage_count == url_size) {
                rv = increaseAllocation(webpageURLs, &url_size);
                if (rv < 0) {
                    printf("an error occured.\n");
                    return -1;
                }
            }

            idx += 12;
            link_idx = url_len;

            char * tempLink = malloc((url_len + 12) * sizeof(char));
            if (tempLink == NULL) {
                perror("malloc error: ");
                return -1;
            }

            strcpy(tempLink, time_schedule_url);
          
            while (webpage[idx] != '>')
                tempLink[link_idx++] = webpage[idx++];

            tempLink[link_idx] = '\0';
            (*webpageURLs)[webpage_count++] = tempLink;
        }
    }
    return webpage_count;
}



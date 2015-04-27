#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include "classroom_inspector.h"
#include "util.h"

typedef struct _time_scheule {
    char ** m, ** t, ** w, ** th, ** f;
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
    (*schedule)->m = malloc(8 * sizeof(char *));
    (*schedule)->t = malloc(8 * sizeof(char *));
    (*schedule)->w = malloc(8 * sizeof(char *));
    (*schedule)->th = malloc(8 * sizeof(char *));
    (*schedule)->f = malloc(8 * sizeof(char *));
    (*schedule)->building = building;
    (*schedule)->room = room;
    int i;
    for (i = 0; i < 5; i++)
        (*schedule)->idx[i] = 0;    
}

void * inspect_page(void * params)
{
    parse_info* r_info = params;
    int i;
    //printf("bounds: %d to %d\n", r_info->bounds[0], r_info->bounds[1]);
    for (i = r_info->bounds[0]; i < r_info->bounds[1]; i++) {
     
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
//*/

}


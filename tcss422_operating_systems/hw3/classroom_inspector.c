#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include "classroom_inspector.h"
#include "util.h"

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

void inspect_page(const char * building,
                  const char * room,
                  const char * time_schedule_url,
                  const char * class_page_html)
{

}

void * retrieve_page(void * params)
{

}

void inspect_classroom(const char * building, 
                       const char * room, 
                       const char * time_schedule_url, 
                       int retrieval_threads, 
                       int analysis_threads) 
{
    char buffer[80], * content, * walker, ** webpages;
    unsigned int len, idx, link_idx, k, webpage_size;
    pthread_t r_threads[retrieval_threads];

    webpage_size = 64;
    webpages = malloc((webpage_size + 1) * sizeof(char*));

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
                increaseAllocation(webpages, &webpage_size);
            }

            idx += 12;
            link_idx = strlen(time_schedule_url);

            char * tempLink = malloc((link_idx + 12) * sizeof(char));

            strcpy(tempLink, time_schedule_url);
          
            while (walker[idx] != '>')
                tempLink[link_idx++] = walker[idx++];
            tempLink[link_idx] = '\0';

            webpages[k++] = tempLink;
        }
    }

///*
    for (idx = 0; idx < k; idx++) {
        printf("%d %s\n", idx, webpages[idx]);
    }
//*/

}


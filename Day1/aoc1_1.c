#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define LINE_LEN 14
#define N_LINES 1000

int   is_sorted(int  *n_arr, size_t len)
{
    for (int i = 1; i < len; i++)
    {
        if (n_arr[i - 1] > n_arr[i])
            return (0);
    }
    return (1);
}

void   sort_n_arr(int *n_arr, size_t len)
{
    int tmp;

    while (!is_sorted(n_arr, len))
    {
        for (int i = 1; i < len; i++)
        {
            if (n_arr[i - 1] > n_arr[i])
            {
                tmp = n_arr[i];
                n_arr[i] = n_arr[i - 1];
                n_arr[i - 1] = tmp;
            }
        }
    }
}

int   get_n1_from_line(char *str)
{
    size_t  len;
    char    *substr;
    int     n;
    
    len = strchr(str, ' ') - str;
    substr = strndup(str, len);
    n = atoi(substr);
    free(substr);
    return (n);

}

int   get_n2_from_line(char *str)
{
    size_t  len;
    char    *substr;
    
    str = strrchr(str, ' ') + 1;
    return (atoi(str));

}

int main(int argc, char **argv)
{
    int     fd;
    char    buffer[LINE_LEN];
    int    arr1[N_LINES];
    int    arr2[N_LINES];
    int    distance;

    if (argc != 2)
    {
        printf("Usage: %s input_file\n", argv[0]);
        return (1);
    }
    fd = open(argv[1], O_RDONLY);
    if (!fd)
        return (1);
    for(int i = 0; read(fd, buffer, 14) && i < N_LINES; i++)
    {
        buffer[LINE_LEN] = '\0';
        arr1[i] = get_n1_from_line(buffer);
        arr2[i] = get_n2_from_line(buffer);
    }
    close(fd);
    sort_n_arr(arr1, N_LINES);
    sort_n_arr(arr2, N_LINES);
    for (int i = 0; i < N_LINES; i++)
        printf("[%d] list1: %d list2: %d\n", i, arr1[i], arr2[i]);
    distance = 0;
    for (int i = 0; i < N_LINES; i++)
        distance += abs(arr1[i] - arr2[i]);
    printf("Total distance: %d\n", distance);
    return (0);
}
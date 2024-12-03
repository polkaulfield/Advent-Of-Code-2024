#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define N_ITEMS_LINE 8

long get_filesize(int fd)
{
	long fsize;

	fsize = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	return (fsize);
}

int read_file(int fd, char **buffer)
{
	long fsize;

	fsize = get_filesize(fd);
	printf("FSIZE: %d", fsize);
	*buffer = (char *)calloc(sizeof(char), fsize + 1);
	if (!*buffer || read(fd, *buffer, fsize) == -1)
		return (0);
}

int get_n_lines(char *buffer)
{
	int i;

	i = 0;
	while (*buffer)
	{
		if (*buffer == '\n')
			i++;
		buffer++;
	}
	return (i);
}

void fill_matrix(int **matrix, int n_lines, char *buffer)
{
	char *token;
	int *arr;
	char *iter;
	char *line;

	iter = NULL;
	for (int i = 0; i < n_lines; i++)
	{
		matrix[i] = (int *)malloc(sizeof(int) * N_ITEMS_LINE);
		line = strndup(buffer, strchr(buffer, '\n') - buffer);
		iter = line;
		for (int j = 0; j < N_ITEMS_LINE; j++)
		{
			token = strtok(iter, " ");
			if (!token)
				break;
			iter = NULL;
			matrix[i][j] = atoi(token);
		}
		buffer = strchr(buffer, '\n') + 1;
		free(line);
	}
}

int is_sorted_up(int *n_arr, size_t len)
{
	for (int i = 1; i < len; i++)
	{
		if (n_arr[i] == 0)
			return (1);
		if (n_arr[i - 1] >= n_arr[i] || n_arr[i - 1] < n_arr[i] - 3)
			return (0);
	}
	return (1);
}

int is_sorted_down(int *n_arr, size_t len)
{
	for (int i = 1; i < len; i++)
	{
		if (n_arr[i] == 0)
			return (1);
		if (n_arr[i - 1] <= n_arr[i] || n_arr[i - 1] > n_arr[i] + 3)
			return (0);
	}
	return (1);
}

int check_safe(int **matrix, int n_lines)
{
	int n_safe;

	n_safe = 0;
	for (int i = 0; i < n_lines; i++)
	{
		if (is_sorted_up(matrix[i], N_ITEMS_LINE) || is_sorted_down(matrix[i], N_ITEMS_LINE))
			n_safe++;
	}
	return (n_safe);
}

void print_matrix(int **matrix, int n_lines)
{
	for (int i = 0; i < n_lines; i++)
	{
		printf("[%d] ", i);
		for (int j = 0; j < N_ITEMS_LINE; j++)
		{
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
}

int main(int argc, char **argv)
{
	int fd;
	int fsize;
	char *buffer;
	int n_lines;
	int **matrix;

	if (argc != 2)
		return (1);
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		return (1);
	if (!read_file(fd, &buffer))
		return (1);
	n_lines = get_n_lines(buffer);
	matrix = (int **)calloc(sizeof(int *), n_lines + 1);
	fill_matrix(matrix, n_lines, buffer);
	print_matrix(matrix, n_lines);
	printf("N of safe lines: %d\n", check_safe(matrix, n_lines));
	return (0);
}
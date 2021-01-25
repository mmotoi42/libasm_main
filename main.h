#ifndef MAIN_H
# define MAIN_H

# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <sys/stat.h>
# include "libasm.h"
# include "libasm_bonus.h"

typedef struct s_option
{
    int fd;
    int ft_fd;
    int input_mode;
    int output_mode;
    int crash_mode;
    char **strings;
    char *file_path;
}   t_option;

size_t	ft_intlen(int n);
char	*ft_strrev(char *str);
char   *ft_itoa(int n);
t_list *ft_lstnew(void *data);
void	ft_lstadd_back(t_list **lst, t_list *new);
void free_strings(char **strings);
void get_input(t_option *option);
char **input_set();
char **input_stdin();
char *input_file_path();
void check_args(int check_list[100], char **av, t_option *option);
void check_all(t_option *option);
void check_ft_strlen(t_option *option);
void check_ft_strcpy(t_option *option);
void check_ft_strcmp(t_option *option);
void check_ft_write(t_option *option);
void check_ft_read(t_option *option);
void check_ft_strdup(t_option *option);
void check_ft_atoi_base(t_option *option);
void check_ft_list_push_front(t_option *option);
void check_ft_list_size(t_option *option);
void check_ft_list_sort(t_option *option);
void check_ft_list_remove_if(t_option *option);

#endif
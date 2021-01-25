//=====================================================
// 引数で受け取った各関数名で実行
// ex) ./a.out ft_strlen ft_strdup ft_strcpy
//     > DONE ft_strlen 結果
//     > DONE ft_strcpy 結果
//     > DONE ft_strdup 結果
//
// 入力形式を指定
// input=0 : 予め準備された文字列(default)
// input=1 : 標準入力で受け付けた文字列
// input=2 : パスで指定されたファイルから受け付けた文字列 (未実装)
// ex1) ./a.out ft_strlen input=1
//      > 入力受付中 : test_input_free_words
// 
// ex2) ./a.out ft_strlen input=2
//      > 入力受付中 : input_file_path 
//
// 出力形式を指定
// output=0 : 標準出力
// output=1 : エラー出力
// output=2 : ファイル出力
// ex) ./a.out ft_strlen output=2
//     > ls
//     > own.txt original.txt
//
// 構成
// 20 main();
// 39 check_args();
// check_all();
// check_ft_strlen();
// check_ft_strcpy();
// check_ft_strcmp();
// check_ft_write();
// check_ft_read();
// check_ft_strdup();
// check_ft_atoi_base();
// check_ft_list_push_front();
// check_ft_list_size();
// check_ft_list_sort();
// check_ft_list_remove_if();
// 
//=====================================================

#include "../includes/main.h"

#define ACCEPT_STRING_SIZE 100000

#define ALL_DEBUG 96
#define CRASH_MODE 97
#define INPUT_MODE 98
#define OUTPUT_MODE 99
#define COMMAND_NUM 100

int main(int ac, char **av)
{
    if (ac > 1000) return (0);
    int check_list[COMMAND_NUM] = {0};
    t_option *option = (t_option *)malloc(sizeof(t_option));
    check_args(check_list, av, option);
    get_input(option);
    if (check_list[ALL_DEBUG] == 1) check_all(option);
    if (check_list[0]  == 1)  check_ft_strlen(option);
    if (check_list[1]  == 1)  check_ft_strcpy(option);
    if (check_list[2]  == 1)  check_ft_strcmp(option);
    if (check_list[3]  == 1)  check_ft_write(option);
    if (check_list[4]  == 1)  check_ft_read(option);
    if (check_list[5]  == 1)  check_ft_strdup(option);
    if (check_list[6]  == 1)  check_ft_atoi_base(option);
    if (check_list[7]  == 1)  check_ft_list_push_front(option);
    if (check_list[8]  == 1)  check_ft_list_size(option);
    if (check_list[9]  == 1)  check_ft_list_sort(option);
    if (check_list[10] == 1)  check_ft_list_remove_if(option);
    free_strings(option->strings);
    free(option);//input_path free
    return (0);
}

// 文字列群を格納していたメモリをfree
// まず中身をfreeしてから外側をfreeしないと
// 経路を失って中身を参照できなくなる
void free_strings(char **strings)
{
    int i = 0;
    while (strings[i])
    {
        free(strings[i]);
        i++;
    }
    free(strings);
}

// ・コード内で準備した文字列
// ・標準入力
// ・外部参照ファイルパス
// それぞれの入力方式で分岐
// どれでもない場合はエラーとして終了(エラー時の出力未実装)
void get_input(t_option *option)
{
    if (option->input_mode == 0)
        option->strings = input_set();
    else if (option->input_mode == 1) 
        option->strings = input_stdin();
    // else if (option->input_mode == 2) 
    //     option->file_path = input_file_path();
    else
        exit(1);
}

// 予め準備した文字列群をheapに保持して返すだけ
char **input_set()
{
    char **strings = (char **)malloc(sizeof(char *) * 15);
    strings[0] = strdup("dst_short");
    strings[1] = strdup("dst_short___");
    strings[2] = strdup("dst_long____");
    strings[3] = strdup("dst_long");
    strings[4] = strdup("dstlen_equal_srclen");
    strings[5] = strdup("srclen_equal_dstlen");
    strings[6] = strdup("");
    strings[7] = strdup("dst_empty");
    strings[8] = strdup("src_empty");
    strings[9] = strdup("");
    strings[10] = strdup("");
    strings[11] = strdup("");
    strings[12] = strdup("\t\t\t\t\t\t\t\t\t");
    strings[13] = strdup("\t\t\t\t\t\t\t\t\t");
    strings[14] = NULL;
    return (strings);
}

// 標準入力を受け取る関数
// 実行するとreadにより標準入力待機状態になる
// 
// readの仕様
//     \EOF(Ctrl+D)で受取終了、bufに文字列格納して文字数を返す
//     ターミナルの仕様でEnterを押すと改行と\EOFが与えられる
//         この場合は一文字だけ入力されたと判定される
//     エラーで-1を返す
//
// 操作
// ターミナル入力でのEnterが一文字に加算されることが直感的な挙動でないため
// ヌル文字前のEnterをカウントしない仕様とした
// 次の文字列入力への移行 : (Ctrl+D) or Enter
// 入力終了  : Enter押して(Ctrl+D) or (Ctrl+D)を二回
// 
// 設計概要
// 受け取った文字列ごとにリストに入れてそのまま二重配列にコピーして返す
// 
// 詳細
// mallocで文字列格納用のメモリ確保 -> buf
// それをdataとしたlist作成する
// bufにreadで受け取った文字列を入力　(この時点では&buf=&dataなのでlistのdataに文字列を入力したことになる)
// bufに新しく確保したメモリのアドレスを上書き(この時listのdataは以前のアドレスのまま残る)
//    ※listのheadを記録していて、後でfreeできるのでリークにならない
// 新しく作ったbufをdataとしたlistを新しく作成
// 次の文字列入力を待つ
// 
// 入力終了後、list_lenで二重配列stringsを作成
// 二重配列stringsの各要素に対応したstr_len_listの長さのメモリを確保、最後の要素はNULL
// str_listをheadからlist->next == nullまで参照し、対応したstringsに格納
// この時、最後の文字列以外では改行が終端文字になっているのでヌル文字に変換
// 
char **input_stdin()
{
    int ret;
    t_list *str_list;
    t_list *str_head;
    t_list *tmp_list;
    t_list *len_head;
    char *buf;
    char **strings;
    long *len;
    t_list *str_len_list;
    size_t list_len = 0;

    // 文字列長を格納するリスト
    len = (long *)malloc(sizeof(long));
    str_len_list = ft_lstnew(len);
    len_head = str_len_list;

    // 文字列を格納するリスト
    buf = (char *)malloc(sizeof(char) * 1024);
    buf[1023] = 0;
    str_list = ft_lstnew(buf);

    str_head = str_list;
    printf("奇数行 : dst\n");
    printf("偶数行 : src\n");
    printf("========入力受付========\n");
    while ((ret = read(0, buf, 1023)) > 0)
    {
        buf[ret] = 0;

        // 文字列長格納後、新規メモリ確保してリスト作成、次はそこに新しいlenを入れる
        *len = ret;
        len = (long *)malloc(sizeof(long));
        ft_lstadd_back(&str_len_list, ft_lstnew(len));

        // 文字列格納
        buf = (char *)malloc(sizeof(char) * 1024);
        ft_lstadd_back(&str_list, ft_lstnew(buf));

        list_len++;
    }
    *len = 0;
    if (ret < 0) exit(1);

    // 入力がなければ、空文字とNULLのみの二重配列を返す
    if (list_len == 0)
    {
        free(str_len_list->data);
        free(str_len_list);
        free(str_list->data);
        free(str_list);
        if (!(strings = (char **)malloc(sizeof(char *) * 2))) exit(1);
        if (!(strings[0] = strdup(""))) exit(1);
        strings[1] = NULL;
        return (strings);
    }

    // 連結リスト＋NULL終端分の長さの二次元配列を作成
    if (!(strings = (char **)malloc(sizeof(char *) * (list_len + 1)))) exit(1);
    str_len_list = len_head;
    int i = 0;
    while (str_len_list && (*(long *)str_len_list->data))
    {
        if (!(strings[i++] = (char *)malloc(sizeof(char) * (*(long *)str_len_list->data + 1))))
            return (NULL);
        str_len_list = str_len_list->next;
    }
    strings[i] = NULL;
    // whileの条件式に(*(long *)str_len_list->data)を加えなかった場合
    // list1 moji 4 strings[0] <- malloc(4 + 1)
    // list2 aa   2 strings[1] <- malloc(2 + 1)
    // list3      0 strings[2] <- malloc(0 + 1)
    // となる。でも、stringsの終端はNULLにしたい
    // strings[2] = NULL <- メモリーリーク
    // なので、list3でdata = 0のときにループから抜ける

    // 文字列格納
    i = 0;
    int j = 0;
    str_list = str_head;
    str_len_list = len_head;
    while (str_list && j < (*(long *)str_len_list->data))
    {
        strings[i][j] = ((char *)(str_list->data))[j];
        j++;
        if (((char *)(str_list->data))[j] == '\0')
        {
            tmp_list = str_list;
            str_list = str_list->next;
            free(tmp_list->data);
            free(tmp_list);

            tmp_list = str_len_list;
            str_len_list = str_len_list->next;
            free(tmp_list->data);
            free(tmp_list);
            
            strings[i][j] = '\0';
            if (strings[i][j - 1] == '\n')
                strings[i][j - 1] = '\0';
            j = 0;
            i++;
        }
    }
    if (str_list) 
        free(str_list->data);
    free(str_list);
    if (str_len_list->data)
        free(str_len_list->data);
    free(str_len_list);
    return (strings);
}

// 標準入力からファイルパスを受け取ってoptionのファイルパスに格納
// それとoptionのfdに登録　する予定(未実装)
char *input_file_path()
{
    return (NULL);
}

// コマンドライン引数からコマンド判定、対応したチェックリストに記録していく
// ここではあくまで記録だけ
void check_args(int check_list[COMMAND_NUM], char **av, t_option *option)
{
    int i = 1; // av[0] is executable_file (a.out)
    while (i < 100 && av[i])
    {
        if (!strncmp(av[i], "strlen", 30))               check_list[0]  = 1;
        else if (!strncmp(av[i], "strcpy", 30))          check_list[1]  = 1;
        else if (!strncmp(av[i], "strcmp", 30))          check_list[2]  = 1;
        else if (!strncmp(av[i], "write", 30))           check_list[3]  = 1;
        else if (!strncmp(av[i], "read", 30))            check_list[4]  = 1;
        else if (!strncmp(av[i], "strdup", 30))          check_list[5]  = 1;
        else if (!strncmp(av[i], "atoi_base", 30))       check_list[6]  = 1;
        else if (!strncmp(av[i], "list_push_front", 30)) check_list[7]  = 1;
        else if (!strncmp(av[i], "list_size", 30))       check_list[8]  = 1;
        else if (!strncmp(av[i], "list_sort", 30))       check_list[9]  = 1;
        else if (!strncmp(av[i], "list_remove_if", 30))  check_list[10] = 1;
        else if (!strncmp(av[i], "ft_strlen", 30))               check_list[0]  = 1;
        else if (!strncmp(av[i], "ft_strcpy", 30))          check_list[1]  = 1;
        else if (!strncmp(av[i], "ft_strcmp", 30))          check_list[2]  = 1;
        else if (!strncmp(av[i], "ft_write", 30))           check_list[3]  = 1;
        else if (!strncmp(av[i], "ft_read", 30))            check_list[4]  = 1;
        else if (!strncmp(av[i], "ft_strdup", 30))          check_list[5]  = 1;
        else if (!strncmp(av[i], "ft_atoi_base", 30))       check_list[6]  = 1;
        else if (!strncmp(av[i], "ft_list_push_front", 30)) check_list[7]  = 1;
        else if (!strncmp(av[i], "ft_list_size", 30))       check_list[8]  = 1;
        else if (!strncmp(av[i], "ft_list_sort", 30))       check_list[9]  = 1;
        else if (!strncmp(av[i], "ft_list_remove_if", 30))  check_list[10] = 1;
        else if (!strncmp(av[i], "all", 30))                check_list[ALL_DEBUG] = 1;
        else if (!strncmp(av[i], "crash=0", 30))            check_list[CRASH_MODE] = 0;
        else if (!strncmp(av[i], "crash=1", 30))            check_list[CRASH_MODE] = 1;
        else if (!strncmp(av[i], "input=0", 30))            check_list[INPUT_MODE] = 0;
        else if (!strncmp(av[i], "input=1", 30))            check_list[INPUT_MODE] = 1;
        //else if (!strncmp(av[i], "input=2", 30))            check_list[INPUT_MODE] = 2;
        else if (!strncmp(av[i], "output=0", 30))           check_list[OUTPUT_MODE] = 0;
        else if (!strncmp(av[i], "output=1", 30))           check_list[OUTPUT_MODE] = 1;
        else if (!strncmp(av[i], "output=2", 30))           check_list[OUTPUT_MODE] = 2;
        i++;
    }
    option->crash_mode = check_list[CRASH_MODE];
    option->input_mode = check_list[INPUT_MODE];
    option->output_mode = check_list[OUTPUT_MODE];
    option->fd = 1;
    if (option->output_mode == 2)
    {
        creat("output/00_output.txt", 0xffffffff);
        creat("output/01_ft_output.txt", 0xffffffff);
        option->fd = open("output/00_output.txt", O_RDWR, O_CREAT);
        option->ft_fd = open("output/01_ft_output.txt", O_RDWR, O_CREAT);
    }
    printf("---クラッシュパターン---\n");
    printf("%lld文字以上\n", (long long)ACCEPT_STRING_SIZE);
    if (option->crash_mode == 1)
        printf("srcの長さがdstの長さを超える\n");
    printf("------------------------\n");
}

// allコマンドを受けとったら全てのテスト実行してexit
void check_all(t_option *option)
{
    check_ft_strlen(option);
    check_ft_strcpy(option);
    check_ft_strcmp(option);
    check_ft_write(option);
    check_ft_read(option);
    check_ft_strdup(option);
    check_ft_atoi_base(option);
    check_ft_list_push_front(option);
    check_ft_list_size(option);
    check_ft_list_sort(option);
    check_ft_list_remove_if(option);
    exit(0);
}

void check_ft_strlen(t_option *option)
{
    printf("\n__________ft_strlen__________\n");
    int i = 0;
    while (option->strings[i])
    {
        int len = strlen(option->strings[i]);
        int ft_len = ft_strlen(option->strings[i]);
        printf("strlen : %d  ft_strlen : %d\n", len, ft_len);
        if (option->output_mode == 2)
        {
            char *len_str = ft_itoa(len);
            char *ft_len_str = ft_itoa(ft_len);
            write(option->fd, len_str, strlen(len_str));
            write(option->ft_fd, ft_len_str, strlen(ft_len_str));
            free(len_str);
            free(ft_len_str);
            
        }
        printf("_____________________________\n");
        i++;
    }
    printf("\n");
}

void check_ft_strcpy(t_option *option)
{
    printf("__________ft_strcpy__________\n");
    char *ft_tmp_src;
    char *ft_tmp_dst;
    char *tmp_src;
    char *tmp_dst;
    int i = 1;
    while (option->strings[i])
    {
        if (option->crash_mode == 0) //default
        {
            //if strings > ACCEPT_STRING_SIZE crush strcpy
            ft_tmp_src = (char *)malloc(sizeof(char) * (ACCEPT_STRING_SIZE + 1));
            ft_tmp_dst = (char *)malloc(sizeof(char) * (ACCEPT_STRING_SIZE + 1));
            tmp_src = (char *)malloc(sizeof(char) *    (ACCEPT_STRING_SIZE + 1));
            tmp_dst = (char *)malloc(sizeof(char) *    (ACCEPT_STRING_SIZE + 1));
            ft_tmp_src[ACCEPT_STRING_SIZE] = 0;
            ft_tmp_dst[ACCEPT_STRING_SIZE] = 0;
            tmp_src[ACCEPT_STRING_SIZE] = 0;
            tmp_dst[ACCEPT_STRING_SIZE] = 0;

            strcpy(ft_tmp_dst, option->strings[i-1]);
            strcpy(ft_tmp_src, option->strings[i]);
            strcpy(tmp_dst, option->strings[i-1]);
            strcpy(tmp_src, option->strings[i]);
        }
        else if (option->crash_mode == 1)
        {
            // when src > dest crash strcpy
            ft_tmp_src = strdup(option->strings[i-1]);
            ft_tmp_dst = strdup(option->strings[i]);
            tmp_dst = strdup(option->strings[i-1]);
            tmp_src = strdup(option->strings[i]);
        }

        strcpy(tmp_dst, tmp_src);
        ft_strcpy(ft_tmp_dst, ft_tmp_src); //ft_strcpy
        printf("default   : dst[%s] src:[%s]\n",option->strings[i - 1], option->strings[i]);
        printf("strcpy    : dst[%s] src:[%s]\n", tmp_dst, tmp_src);
        printf("ft_strcpy : dst[%s] src:[%s]\n",ft_tmp_dst, ft_tmp_src);
        if (option->output_mode == 2)
        {
            write(option->fd, tmp_dst, strlen(tmp_dst));
            write(option->ft_fd, ft_tmp_dst, strlen(ft_tmp_dst));
        }
        free(ft_tmp_src);
        free(ft_tmp_dst);
        free(tmp_src);
        free(tmp_dst);
        printf("_____________________________\n");
        i++;
    }
    printf("\n");
}

void check_ft_strcmp(t_option *option)
{
    printf("\n__________ft_strcmp__________\n");
    int i = 1;
    while (option->strings[i])
    {
        printf("default   : src[%s] dst:[%s]\n",option->strings[i - 1], option->strings[i]);
        int or = strcmp(option->strings[i-1],option->strings[i]);
        int ft = ft_strcmp(option->strings[i-1],option->strings[i]);
        printf("strcmp : %d  ft_strcmp : %d\n", or, ft);
        printf("_____________________________\n");
        if (option->output_mode == 2)
        {
            char *or_str = ft_itoa(or);
            char *ft_str = ft_itoa(ft);
            write(option->fd, or_str, strlen(or_str));
            write(option->ft_fd, ft_str, strlen(ft_str));
            free(or_str);
            free(ft_str);
        }
        i++;
    }
    printf("\n");
}

void check_ft_write(t_option *option)
{
    int write_fd = 1;
    int ft_write_fd = 1;
    write(1, "\n__________ft_write__________\n", strlen("\n__________ft_write__________\n"));
    if (option->output_mode == 1)
    {
         write_fd = 2;
         ft_write_fd = 2;
    }
    else if (option->output_mode == 2)
    {
        creat("output/02_write_log.txt", 0xffffffff);
        creat("output/03_ft_write_log.txt", 0xffffffff);
        write_fd = open("output/02_write_log.txt", O_RDWR, O_CREAT);
        perror("error");
        ft_write_fd = open("output/03_ft_write_log.txt", O_RDWR, O_CREAT);
        write(1, "\n-----create these file-----", strlen("\n-----create these file-----"));
        write(1, "\nwrite_log.txt\n", strlen("\nwrite_log.txt\n"));
        write(1, "ft_write_log.txt\n", strlen("ft_write_log.txt\n"));
        write(1, "------------------------------\n", strlen("------------------------------\n"));
    }
    int i = 0;
    while (option->strings[i])
    {
        write(1,"write    : ", strlen("write    : "));
        write(write_fd, option->strings[i], strlen(option->strings[i]));
        perror("\nError_write");
        write(1, "ft_write : ", strlen("ft_write : "));
        ft_write(ft_write_fd, option->strings[i], strlen(option->strings[i]));
        perror("\nError_ft_write");
        write(1 , "-----------------------------\n", strlen("_____________________________\n"));
        i++;
    }
    if (option->output_mode > 2)
        write(1,"----------------------------", strlen("____________________________"));
    write(1, "\n", 1);

}

void check_ft_read(t_option *option)
{
    write(1, "\n__________ft_read__________\n", strlen("\n__________ft_read__________\n"));

    char *read_buf = (char *)malloc(sizeof(char) * ACCEPT_STRING_SIZE);
    char *ft_read_buf = (char *)malloc(sizeof(char) * ACCEPT_STRING_SIZE);
    int read_ret;
    int ft_read_ret;
    int read_fd;
    int ft_read_fd;
    if (option->input_mode == 1)
    {
        read_fd = 1;
        ft_read_fd = 1;
    }
    else if (option->input_mode == 2)
    {
        creat("output/04_read_log.txt", 0xffffffff);
        creat("output/05_ft_read_log.txt", 0xffffffff);
        read_fd = open("output/04_read_log.txt", O_RDWR, O_CREAT);
        ft_read_fd = open("output/05_ft_read_log.txt", O_RDWR, O_CREAT);
        int i = 0;
        while (option->strings[i])
        {
            write(read_fd, option->strings[i], strlen(option->strings[i]));
            ft_write(ft_read_fd, option->strings[i], strlen(option->strings[i]));
            i++;
        }
        // write が file を握りっぱなしにしてしまうので、一旦closeする
        close(read_fd);
        close(ft_read_fd);
        read_fd = open("output/04_read_log.txt", O_RDWR, O_CREAT);
        ft_read_fd = open("output/05_ft_read_log.txt", O_RDWR, O_CREAT);
    }
    printf("read test\n");
    read_ret = read(read_fd, read_buf, ACCEPT_STRING_SIZE);
    perror("Error_read");
    if (read_ret >= 0)
        read_buf[read_ret] = 0;
    else
        exit(1);
    printf("ft_read test\n");
    ft_read_ret = ft_read(ft_read_fd, ft_read_buf, ACCEPT_STRING_SIZE);
    perror("Error_ft_read");
    if (ft_read_ret >= 0)
        ft_read_buf[ft_read_ret] = 0;
    else
        exit(1);
    printf("---check return---\n");
    printf("read_buf_len : %ld\nft_read_buf_len : %ld\n", strlen(read_buf), strlen(ft_read_buf));
    printf("read_ret   : %d\nft_read_ret: %d\n",read_ret,ft_read_ret);
    printf("read_buf    : %s\n" ,read_buf);
    printf("ft_read_buf : %s\n", ft_read_buf);
    write(option->fd, read_buf, strlen(read_buf));
    write(option->ft_fd, ft_read_buf, strlen(ft_read_buf));
	free(read_buf);
    free(ft_read_buf);
}

void check_ft_strdup(t_option *option)
{
    printf("__________ft_strdup__________\n");
    char *ft_tmp;
    char *tmp;
    int i = 0;
    while (option->strings[i])
    {
        ///*
        tmp = strdup(option->strings[i]);
        ft_tmp = ft_strdup(option->strings[i]);
        printf("default   : [%s]\n",option->strings[i]);
        printf("strdup    : [%s]\n",tmp);
        printf("ft_strdup : [%s]\n",ft_tmp);
        if (option->output_mode == 2)
        {
            write(option->fd, tmp, strlen(tmp));
            write(option->ft_fd, ft_tmp, strlen(ft_tmp));
        }
        free(ft_tmp);
        free(tmp);
        printf("_____________________________\n");
        i++;
    }
    printf("\n");
}

void check_ft_atoi_base(t_option *option)
{
    if (option->output_mode < 2)
        write(option->fd,"DONE ft_atoi_base\n", strlen("DONE ft_atoi_base\n"));
    write(option->fd,"DONE ft_atoi_base\n", strlen("DONE ft_atoi_base\n"));
    write(option->ft_fd,"DONE ft_atoi_base\n", strlen("DONE ft_atoi_base\n"));
}

void check_ft_list_push_front(t_option *option)
{
    if (option->output_mode < 2)
        write(option->fd,"DONE ft_list_push_front\n", strlen("DONE ft_list_push_front\n"));
    write(option->fd,"DONE ft_list_push_front\n", strlen("DONE ft_list_push_front\n"));
    write(option->ft_fd,"DONE ft_list_push_front\n", strlen("DONE ft_list_push_front\n"));
}

void check_ft_list_size(t_option *option)
{
    if (option->output_mode < 2)
        write(option->fd,"DONE ft_list_size\n", strlen("DONE ft_list_size\n"));
    write(option->fd,"DONE ft_list_size\n", strlen("DONE ft_list_size\n"));
    write(option->ft_fd,"DONE ft_list_size\n", strlen("DONE ft_list_size\n"));
}

void check_ft_list_sort(t_option *option)
{
    if (option->output_mode < 2)
        write(option->fd,"DONE ft_list_sort\n", strlen("DONE ft_list_sort\n"));
    write(option->fd,"DONE ft_list_sort\n", strlen("DONE ft_list_sort\n"));
    write(option->ft_fd,"DONE ft_list_sort\n", strlen("DONE ft_list_sort\n"));
}

void check_ft_list_remove_if(t_option *option)
{
    if (option->output_mode < 2)
        write(option->fd,"DONE ft_remove_if\n", strlen("DONE ft_remove_if\n"));
    write(option->fd,"DONE ft_remove_if\n", strlen("DONE ft_remove_if\n"));
    write(option->ft_fd,"DONE ft_remove_if\n", strlen("DONE ft_remove_if\n"));
}

// input_stdinで使用する
// libftからコピペ。libft.aはft_strlenとかが
// 今回の課題と干渉するので不使用
t_list *ft_lstnew(void *data)
{
	t_list	*new;

	new = (t_list*)malloc(sizeof(t_list));
	if (new == NULL)
		return (NULL);
	new->data = data;
	new->next = NULL;
	return (new);
}

// lstnew同様 input_stdinで使用
void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*p;

	if (new != NULL && lst != NULL)
	{
		if (*lst != NULL)
		{
			p = *lst;
			while (p->next != NULL)
				p = p->next;
			p->next = new;
		}
		else
			*lst = new;
	}
}

size_t	ft_intlen(int n)
{
	int		len;

	len = 0;
	if (n == 0)
		return (1);
	else if (n == -2147483648)
		return (11);
	else if (n < 0)
		len++;
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char		*ft_strrev(char *str)
{
	char		c;
	size_t		len;
	size_t		i;

	if (!str)
		return (NULL);
	len = strlen(str);
	i = 0;
	while (i < len / 2)
	{
		c = str[i];
		str[i] = str[(len - 1) - i];
		str[(len - 1) - i] = c;
		i++;
	}
	return (str);
}

char			*ft_itoa(int n)
{
	char	*r;
	size_t	len;
	char	*r_p;

	len = ft_intlen(n);
	if (n == -2147483648)
		return (strdup("-2147483648"));
	if (n == 0)
		return (strdup("0"));
	if (!(r = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	*(r + len) = 0;
	if (n < 0)
		*(r + len - 1) = '-';
	r_p = r;
	while (n != 0)
	{
		*r_p = (n >= 0) ? (n % 10) + '0' : (n * -1) % 10 + '0';
		n /= 10;
		r_p++;
	}
	ft_strrev(r);
	return (r);
}

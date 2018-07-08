#include <stdio.h>
#include <mysql.h>
#include <stdlib.h>

/* setp1 */
MYSQL mysql;
MYSQL_RES *result;
MYSQL_ROW row;
unsigned int num_fields;
unsigned int i;
const char host_name[] = "localhost";
const char user_name[] = "root";
const char password[] = "888888";
const char db_name[] = "my_db";
unsigned int port_num = 0;
const char socket_name[] = "";
unsigned long CLIENT_MULTI_STATMENTS = 0;

int main(void)
{
    /* int mysql_library_init(int argc, char **argv, char **groups) */
    if (mysql_library_init(0, NULL, NULL))
    {
        fprintf(stderr, "could not initialize MySQL client library\n");
        exit(1);
    }

    /* step2 MYSQL *mysql_init(MYSQL *mysql) */
    mysql_init(&mysql);
    /* mysql_options(&mysql,MYSQL_OPT_COMPRESS,0); */
    /* mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"odbc"); */
    /* mysql_options(&mysql,MYSQL_INIT_COMMAND,"SET autocommit=0"); */

    /* step3 MYSQL *mysql_real_connect(MYSQL *mysql, const char *host, const char *user, const char *passwd, const char *db, unsigned int port, const char *unix_socket, unsigned long client_flag) */
    if (mysql_real_connect(&mysql, host_name, user_name, password,
                           db_name, port_num, NULL, CLIENT_MULTI_STATEMENTS) == NULL)
    {
        printf("mysql_real_connect() failed\n");
        fprintf(stderr, "Failed to connect to database: Error: %s\n", mysql_error(&mysql));
        mysql_close(&mysql);
        exit(1);
    }

    /* step4 mysql_query() */
    /* excute multiple ststements */
    int status = mysql_query(&mysql, "SELECT * FROM Account"); /* int mysql_query(MYSQL *mysql, const char *stmt_str) */

    if (status)
    {
        printf("Could not execute statement(s)");
        mysql_close(&mysql);
        exit(0);
    }

    /* process each statement result */
    do
    {
        /* did current statement return data? */
        /* MYSQL_RES *mysql_store_result(MYSQL *mysql) */
        result = mysql_store_result(&mysql);
        if (result)
        {
            /* unsigned int mysql_num_fields(MYSQL_RES *result) */
            num_fields = mysql_num_fields(result);
            /* MYSQL_ROW mysql_fetch_row(MYSQL_RES *result) */
            while ((row = mysql_fetch_row(result)))
            {
                unsigned long *lengths;
                lengths = mysql_fetch_lengths(result);
                for (i = 0; i < num_fields; i++)
                {
                    printf("[%.*s] ", (int)lengths[i],
                           row[i] ? row[i] : "NULL");
                }
                printf("\n");
            }
            /* yes; process rows and free the result set */
            /* process_result_set(mysql, result); */
            mysql_free_result(result) /* void mysql_free_result(MYSQL_RES *result) */;
        }
        else /* no result set or error*/
        {
            /* unsigned int mysql_field_count(MYSQL *mysql) */
            if (mysql_field_count(&mysql) == 0)
            {
                printf("lld rows affected\n");
                break;
            }
        }
        /* more results? -1 = no, >0 = error, 0 = yes (keep looping) */
        if ((status = mysql_next_result(&mysql)) > 0) /* int mysql_next_result(MYSQL *mysql) */
        {
            printf("Counld not execute statement\n");
        }
    } while (status == 0);

    mysql_close(&mysql);

    mysql_library_end();

    return EXIT_SUCCESS;
}

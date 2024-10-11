#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <string.h>


#define MAX_LEN 128
#define OUTPUTFILE ("output.csv")

typedef struct list_t
{
    char *subject;
    char *statistic;
    struct list_t *next;
} list_t;


/**
 * adapted from template code
 */
void *emalloc(size_t n)
{
    void *p;

    p = malloc(n);
    if (p == NULL)
    {
        fprintf(stderr, "malloc of %zu bytes failed", n);
        exit(1);
    }

    return p;
}

/**
 * adapted from template code
 */
list_t *add_end(list_t *list, list_t *new)
{
    list_t *curr;

    if (list == NULL)
    {
        new->next = NULL;
        return new;
    }

    for (curr = list; curr->next != NULL; curr = curr->next);
    curr->next = new;
    new->next = NULL;
    return list;
}

/**
 * a function to sort the list
 * question 1 & 3: sort by statistic in ascending order, then by subject in ascending order
 * question 2: sort by statistic in descending order
 */
list_t *sort_list(list_t *list, int question) {
    list_t *sorted_list = NULL;
    list_t *current, *prev, *max_node, *max_node_prev;

    while (list != NULL) {
        max_node = list;
        max_node_prev = NULL;
        prev = list;
        current = list->next;
        while (current != NULL) {
            if (question == 1 || question == 3){
            if (atoi(current->statistic) < atoi(max_node->statistic)) {
                max_node = current;
                max_node_prev = prev;
            }
            else if (atoi(current->statistic) == atoi(max_node->statistic)) {
                if (strcmp(current->subject, max_node->subject) > 0) {
                    max_node = current;
                    max_node_prev = prev;
                }
            }
            }
        
            else if (question == 2){
                if (atoi(current->statistic) > atoi(max_node->statistic)) {
                max_node = current;
                max_node_prev = prev;
                }
                else if (atoi(current->statistic) == atoi(max_node->statistic)) {
                    if (strcmp(current->subject, max_node->subject) > 0) {
                        max_node = current;
                        max_node_prev = prev;
                    }
                }
            }
            
            prev = current;
            current = current->next;
        }

        if (max_node_prev == NULL) {
            list = list->next;
        } else {
            max_node_prev->next = max_node->next;
        }

        max_node->next = sorted_list;
        sorted_list = max_node;
    }

    return sorted_list;
}


struct Route{
    char airline_name[MAX_LEN];
    char airline_icao_unique_code[MAX_LEN];
    char airline_country[MAX_LEN];
    char from_airport_name[MAX_LEN];
    char from_airport_city[MAX_LEN];
    char from_airport_country[MAX_LEN];
    char from_airport_icao_unique_code[MAX_LEN];
    float from_airport_altitude;
    char to_airport_name[MAX_LEN];
    char to_airport_city[MAX_LEN];
    char to_airport_country[MAX_LEN];
    char to_airport_icao_unique_code[MAX_LEN];
    float to_airport_altitude;
};
struct Route route;

/**
 * a function to find if the node is in the list
 */
struct list_t *find_in_list(struct list_t *list, char *searched){
    struct list_t *p;
    for (p = list; p != NULL; p = p->next){
        if (strncmp(p->subject, searched, MAX_LEN) == 0){
            return p;
        }
    }
    return NULL;
}

/**
 * a function to print the list to the output file
 */
void write_output(struct list_t *list, int n) {
    struct list_t *current = list;
    int count = 0;
    FILE *fp = fopen(OUTPUTFILE, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot open file");
        exit(1);
    }

    current = list;
    fprintf(fp, "subject,statistic\n");
    while (current != NULL) {
        if (count < n) {
            fprintf(fp, "%s,%s\n", current->subject, current->statistic);
            count++;
        }
        current = current->next;
    }

    fclose(fp);
}


void question1(struct list_t **head, struct Route route) {
    struct list_t *new_node;
    struct list_t *p;

    new_node = (struct list_t *)emalloc(sizeof(struct list_t));

    new_node->subject = emalloc(MAX_LEN * sizeof(char)*2);
    new_node->statistic = emalloc(MAX_LEN * sizeof(char));
    sprintf(new_node->subject, "%s (%s)", route.airline_name, route.airline_icao_unique_code);
    p = find_in_list(*head, new_node->subject);

    if (p == NULL) {
        sprintf(new_node->statistic, "%d", 1);
        *head = add_end(*head, new_node);
    } else {
        int count = atoi(p->statistic);
        count++;
        sprintf(p->statistic, "%d", count);
    }

    *head = sort_list(*head, 1);

}

void question2(struct list_t **head, struct Route route) {
    struct list_t *new_node;
    struct list_t *p;

    new_node = (struct list_t *)emalloc(sizeof(struct list_t));

    new_node->subject = emalloc(MAX_LEN * sizeof(char));
    new_node->statistic = emalloc(MAX_LEN * sizeof(char));
    sprintf(new_node->subject, "%s", route.to_airport_country);
    p = find_in_list(*head, new_node->subject);

    if (p == NULL) {
        sprintf(new_node->statistic, "%d", 1);
        *head = add_end(*head, new_node);
    } else {
        int count = atoi(p->statistic);
        count++;
        sprintf(p->statistic, "%d", count);
    }
    *head = sort_list(*head, 2);
}

void question3(struct list_t **head, struct Route route) {
    struct list_t *new_node;
    struct list_t *p;

    new_node = (struct list_t *)emalloc(sizeof(struct list_t));

    new_node->subject = emalloc(MAX_LEN * sizeof(char)*4);
    new_node->statistic = emalloc(MAX_LEN * sizeof(char));
    sprintf(new_node->subject, "\"%s (%s), %s, %s\"", route.to_airport_name, route.to_airport_icao_unique_code, route.to_airport_city, route.to_airport_country);
    p = find_in_list(*head, new_node->subject);

    if (p == NULL) {
        sprintf(new_node->statistic, "%d", 1);
        *head = add_end(*head, new_node);
    } else {
        int count = atoi(p->statistic);
        count++;
        sprintf(p->statistic, "%d", count);
    }
    *head = sort_list(*head, 3);
}

/**
 * a function to remove the trailing spaces and quotes from the input string
 */
void remove_trailing_quotes_and_spaces(char *str) {
    int len = strlen(str);
    int i;

    for (i = len - 1; i >= 0; i--) {
        if (str[i] != ' ' && str[i] != '\'') {
            break;
        }
    }

    // Null-terminate the string after the last non-space, non-quote character
    str[i + 1] = '\0';
}

void data_process(char *data_file, int question, int n) {
    FILE *fp;
    struct list_t *head = NULL;
    char *line = emalloc(MAX_LEN * sizeof(char));

    fp = fopen(data_file, "r");
    
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot open file %s", data_file);
        exit(1);
    }
    // skip the first line
    fgets(line, MAX_LEN, fp);

    while (fgets(line, MAX_LEN, fp) != NULL) {
        for (int i = 0; i < 13; i++) {
            // avoid extra reading
            if (i > 0) {
                fgets(line, MAX_LEN, fp);
            }
            switch (i) {
                case 0:
                    sscanf(line, "- airline_name:%*[ ']%[^\n]", route.airline_name); //remove the leading spaces and quotes
                    remove_trailing_quotes_and_spaces(route.airline_name);
                    break;
                case 1:
                    sscanf(line, "  airline_icao_unique_code:%*[ ']%[^\n]", route.airline_icao_unique_code);
                    remove_trailing_quotes_and_spaces(route.airline_icao_unique_code);
                    break;
                case 8:
                    sscanf(line, "  to_airport_name:%*[ ']%[^\n]", route.to_airport_name);
                    remove_trailing_quotes_and_spaces(route.to_airport_name);
                    break;
                case 9:
                    sscanf(line, "  to_airport_city:%*[ ']%[^\n]", route.to_airport_city);
                    remove_trailing_quotes_and_spaces(route.to_airport_city);
                    break;
                case 10:
                    sscanf(line, "  to_airport_country:%*[ ']%[^\n]", route.to_airport_country);
                    remove_trailing_quotes_and_spaces(route.to_airport_country);
                    break;
                case 11:
                    sscanf(line, "  to_airport_icao_unique_code:%*[ ']%[^\n]", route.to_airport_icao_unique_code);
                    remove_trailing_quotes_and_spaces(route.to_airport_icao_unique_code);
                    break;
                default:
                    continue;
            }
        }
        if (question == 1) {
            if (strcmp(route.to_airport_country, "Canada") == 0){
            question1(&head, route);
            }
        }
        else if (question == 2) {
            question2(&head, route);
        }
        else if (question == 3) {
            question3(&head, route);
        }
    }

    write_output(head, n);
    fclose(fp);

    //free memory
    
    struct list_t *list = head;
    while (list != NULL) {
        struct list_t *temp = list;
        list = list->next;
        free(temp->subject);
        free(temp->statistic);
        free(temp);
    }
    free(line);
    
}


int main(int argc, char *argv[]){
    char *data_file;
    int question;
    int n;

    //read command line arguments
    //copy and modify from my assignment 1
    if (argc != 4) {
        fprintf(stderr, "Error: Wrong number of arguments.\n");
        exit(1);
    }
    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "--DATA=", 7)==0) {
            data_file = &argv[i][7];
        }
        else if (strncmp(argv[i], "--QUESTION=", 11)==0) {
            question = atoi(&argv[i][11]);
        }
        else if (strncmp(argv[i], "--N=", 4)==0) {
            n = atoi(&argv[i][4]);
        }
        else {
            fprintf(stderr, "Usage: %s <data_file> <question> <n>\n", argv[0]);
            exit(1);
        }
    }

    //call the data_process function which is the main part of the program
    data_process(data_file, question, n);

    exit(0);
}
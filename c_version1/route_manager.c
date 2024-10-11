#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_LINE_LEN 1024
#define MAX_LEN 128
#define OUTPUTFILE ("output.txt")


struct Route {
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

struct UserRoute {
    char airline[MAX_LEN];
    char src_city[MAX_LEN];
    char src_country[MAX_LEN];
    char dest_city[MAX_LEN];
    char dest_country[MAX_LEN];
    int case_num;
};
struct UserRoute user_route;



/*
 * process data file
 * write matched lines to OUTPUTFILE
 * compare route(input data file)
 * with user_route(command line input)
 */

void get_line(char *data_file, struct UserRoute *user_route){
    FILE *fp;
    FILE *ofp;
    char line[MAX_LINE_LEN];
    int founded = 0;

    fp = fopen(data_file, "r");
    
    if (fp == NULL){
        fprintf(stderr, "Error: Cannot open file %s", data_file);
        exit(1);
    }
    
    ofp = fopen(OUTPUTFILE, "w");
    
    if (ofp == NULL){
        fprintf(stderr, "File open error: %s", OUTPUTFILE);
        exit(1);
    }
    
    while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f,%[^,],%[^,],%[^,],%[^,],%f",route.airline_name, route.airline_icao_unique_code, route.airline_country, route.from_airport_name, route.from_airport_city, route.from_airport_country, route.from_airport_icao_unique_code, &route.from_airport_altitude, route.to_airport_name, route.to_airport_city, route.to_airport_country, route.to_airport_icao_unique_code, &route.to_airport_altitude);
        
    	if (user_route->case_num == 1 && strcmp(route.airline_icao_unique_code, user_route->airline) == 0 && strcmp(route.to_airport_country, user_route->dest_country) == 0) {
            if (founded == 0) {
                fprintf(ofp, "FLIGHTS TO %s BY %s (%s):\n", route.to_airport_country, route.airline_name, route.airline_icao_unique_code);
            }
            fprintf(ofp, "FROM: %s, %s, %s TO: %s (%s), %s\n", route.from_airport_icao_unique_code, route.from_airport_city, route.from_airport_country, route.to_airport_name, route.to_airport_icao_unique_code, route.to_airport_city);
            founded += 1;
        }

        if (user_route->case_num == 2 && strcmp(route.from_airport_country, user_route->src_country) == 0 && strcmp(route.to_airport_country, user_route->dest_country) == 0 && strcmp(route.to_airport_city, user_route->dest_city) == 0) {
            if (founded == 0) {
                fprintf(ofp, "FLIGHTS FROM %s TO %s, %s:\n", route.from_airport_country, route.to_airport_city, route.to_airport_country);
            }
            fprintf(ofp, "AIRLINE: %s (%s) ORIGIN: %s (%s), %s\n",route.airline_name, route.airline_icao_unique_code, route.from_airport_name, route.from_airport_icao_unique_code,route.from_airport_city);
            founded += 1;
        }

        if (user_route->case_num == 3 && strcmp(route.from_airport_country, user_route->src_country) == 0 && strcmp(route.from_airport_city, user_route->src_city) == 0 && strcmp(route.to_airport_country, user_route->dest_country) == 0 && strcmp(route.to_airport_city, user_route->dest_city) == 0) {
            if(founded == 0){
                fprintf(ofp, "FLIGHTS FROM %s, %s TO %s, %s:\n", route.from_airport_city, route.from_airport_country, route.to_airport_city, route.to_airport_country);
            }
            fprintf(ofp, "AIRLINE: %s (%s) ROUTE: %s-%s\n", route.airline_name, route.airline_icao_unique_code, route.from_airport_icao_unique_code, route.to_airport_icao_unique_code);
            founded += 1;
        }
        
    } 

    if (founded == 0) {
        fprintf(ofp,"NO RESULTS FOUND.\n");
    }

    fclose(fp);
    fclose(ofp);
}



/*
 * read command line
 * store matching keywords
 * verify input format/case
 */

struct UserRoute* read_command(int argc, char* argv[], struct UserRoute *user_route){
    int found_data = 0, found_airline = 0, found_dest_country = 0;
    int found_dest_city = 0, found_src_country = 0, found_src_city = 0;

    for(int i = 1; i < argc; i++){
        if (strncmp(argv[i], "--DATA=", 7) == 0){
            found_data = 1;
        }
        else if (strncmp(argv[i], "--AIRLINE=", 10) == 0){
            strcpy(user_route->airline, &argv[i][10]);
            found_airline = 1;
        }
        else if (strncmp(argv[i], "--DEST_COUNTRY=", 15) == 0){
            strcpy(user_route->dest_country, &argv[i][15]);
            found_dest_country = 1;
        }
        else if (strncmp(argv[i], "--DEST_CITY=", 12) == 0){
            strcpy(user_route->dest_city, &argv[i][12]);
            found_dest_city = 1;
        }
        else if (strncmp(argv[i], "--SRC_COUNTRY=", 14) == 0){
            strcpy(user_route->src_country, &argv[i][14]);
            found_src_country = 1;
        }
        else if (strncmp(argv[i], "--SRC_CITY=", 11) == 0){
            strcpy(user_route->src_city, &argv[i][11]);
            found_src_city = 1;
        }
        else{
            fprintf(stderr, "Error: Invalid argument %s\n", argv[i]);
            exit(1);
        }
    }

    if (!found_data){
        fprintf(stderr, "Error: Missing data file.\n");
        exit(1);
    }
    
    if (found_airline && found_dest_country && !found_dest_city && !found_src_country && !found_src_city){
        user_route->case_num = 1;
        return user_route;
    }
        
    if (found_src_country && found_dest_city && found_dest_country && !found_airline && !found_src_city){
        user_route->case_num = 2;
        return user_route;
    }
    
    if (found_src_city && found_src_country && found_dest_city && found_dest_country && !found_airline){
        user_route->case_num = 3;
        return user_route;
    }
    
    else {
        fprintf(stderr, "Error: Missing required arguments.\n");
        exit(1);
    }
}

/*
 * Check argument number
 * get data_file
 * call help functions
 */

int main(int argc, char *argv[]){
    if (argc < 2){
        fprintf(stderr, "Error: Missing required arguments.\n");
        exit(1);
    }

    char *data_file;
    
    for(int i = 1; i < argc; i++){
        if (strncmp(argv[i], "--DATA=", 7) == 0){
            data_file = &argv[i][7];
        }
    }
    
    read_command(argc, argv, &user_route);
    
    get_line(data_file, &user_route);
    
    return 0;
}

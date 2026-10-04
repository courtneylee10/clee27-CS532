#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SIZE 2048

struct listing {
    int id;
    int host_id;
    char *host_name;
    char *neighbourhood_group;
    char *neighbourhood;
    float latitude;
    float longitude;
    char *room_type;
    float price;
    int minimum_nights;
    int number_of_reviews;
    int calculated_host_listings_count;
    int availability_365;
};

/* Makes a copy of a string */
char *copyString(char *str)
{
    char *copy;

    copy = malloc(strlen(str) + 1);

    if (copy != NULL)
    {
        strcpy(copy, str);
    }

    return copy;
}

/* Parses one line from the CSV file */
struct listing getFields(char *line)
{
    struct listing item;
    char *token;

    token = strtok(line, ",");
    item.id = atoi(token);

    token = strtok(NULL, ",");
    item.host_id = atoi(token);

    token = strtok(NULL, ",");
    item.host_name = copyString(token);

    token = strtok(NULL, ",");
    item.neighbourhood_group = copyString(token);

    token = strtok(NULL, ",");
    item.neighbourhood = copyString(token);

    token = strtok(NULL, ",");
    item.latitude = atof(token);

    token = strtok(NULL, ",");
    item.longitude = atof(token);

    token = strtok(NULL, ",");
    item.room_type = copyString(token);

    token = strtok(NULL, ",");
    item.price = atof(token);

    token = strtok(NULL, ",");
    item.minimum_nights = atoi(token);

    token = strtok(NULL, ",");
    item.number_of_reviews = atoi(token);

    token = strtok(NULL, ",");
    item.calculated_host_listings_count = atoi(token);

    token = strtok(NULL, ",");
    item.availability_365 = atoi(token);

    return item;
}

/* Sorts by host name */
int compareHostName(const void *a, const void *b)
{
    struct listing *item1 = (struct listing *)a;
    struct listing *item2 = (struct listing *)b;

    return strcmp(item1->host_name, item2->host_name);
}

/* Sorts by price */
int comparePrice(const void *a, const void *b)
{
    struct listing *item1 = (struct listing *)a;
    struct listing *item2 = (struct listing *)b;

    if (item1->price < item2->price)
    {
        return -1;
    }
    else if (item1->price > item2->price)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* Writes one listing to a file */
void writeListing(FILE *file, struct listing item)
{
    fprintf(file,
            "%d,%d,%s,%s,%s,%.6f,%.6f,%s,%.2f,%d,%d,%d,%d\n",
            item.id,
            item.host_id,
            item.host_name,
            item.neighbourhood_group,
            item.neighbourhood,
            item.latitude,
            item.longitude,
            item.room_type,
            item.price,
            item.minimum_nights,
            item.number_of_reviews,
            item.calculated_host_listings_count,
            item.availability_365);
}

/* Frees memory used by the listings */
void freeListings(struct listing list[], int count)
{
    int i;

    for (i = 0; i < count; i++)
    {
        free(list[i].host_name);
        free(list[i].neighbourhood_group);
        free(list[i].neighbourhood);
        free(list[i].room_type);
    }

    free(list);
}

int main(void)
{
    FILE *inputFile;
    FILE *hostFile;
    FILE *priceFile;

    char line[LINE_SIZE];

    struct listing *list;

    int count = 0;
    int capacity = 100;

    list = malloc(capacity * sizeof(struct listing));

    if (list == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    /* Open input file */
    inputFile = fopen("listings.csv", "r");

    if (inputFile == NULL)
    {
        printf("Could not open listings.csv\n");
        free(list);
        return 1;
    }

    /* Skip the header line */
    fgets(line, LINE_SIZE, inputFile);

    /* Read the listings */
    while (fgets(line, LINE_SIZE, inputFile) != NULL)
    {
        if (count >= capacity)
        {
            capacity = capacity * 2;

            list = realloc(list, capacity * sizeof(struct listing));

            if (list == NULL)
            {
                printf("Memory allocation failed.\n");
                fclose(inputFile);
                return 1;
            }
        }

        list[count] = getFields(line);
        count++;
    }

    fclose(inputFile);

    /* Sort by host name */
    qsort(list, count, sizeof(struct listing), compareHostName);

    /* Open host name output file */
    hostFile = fopen("host_name_sorted.csv", "w");

    if (hostFile == NULL)
    {
        printf("Could not create host_name_sorted.csv\n");
        freeListings(list, count);
        return 1;
    }

    /* Write header */
    fprintf(hostFile,
            "id,host_id,host_name,neighbourhood_group,neighbourhood,latitude,longitude,room_type,price,minimum_nights,number_of_reviews,calculated_host_listings_count,availability_365\n");

    /* Write host name sorted data */
    {
        int i;

        for (i = 0; i < count; i++)
        {
            writeListing(hostFile, list[i]);
        }
    }

    fclose(hostFile);

    /* Sort by price */
    qsort(list, count, sizeof(struct listing), comparePrice);

    /* Open price output file */
    priceFile = fopen("price_sorted.csv", "w");

    if (priceFile == NULL)
    {
        printf("Could not create price_sorted.csv\n");
        freeListings(list, count);
        return 1;
    }

    /* Write header */
    fprintf(priceFile,
            "id,host_id,host_name,neighbourhood_group,neighbourhood,latitude,longitude,room_type,price,minimum_nights,number_of_reviews,calculated_host_listings_count,availability_365\n");

    /* Write price sorted data */
    {
        int i;

        for (i = 0; i < count; i++)
        {
            writeListing(priceFile, list[i]);
        }
    }

    fclose(priceFile);

    printf("Successfully read and sorted %d listings.\n", count);
    printf("Created host_name_sorted.csv\n");
    printf("Created price_sorted.csv\n");

    freeListings(list, count);

    return 0;
}
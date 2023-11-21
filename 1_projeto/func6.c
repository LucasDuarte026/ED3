#include "structs.h"
#include "func3.h"
#include "func4.h" 
#include "func5.h" 
#include "B-Tree.h"

void functionality_6(const char dataFileName[], const char indexFileName[], int n) {
    // Open the binary data file and B-tree index file
    FILE *dataFile = fopen(dataFileName, "rb");
    FILE *indexFile = fopen(indexFileName, "rb");
    if (dataFile == NULL || indexFile == NULL) {
        printf("File processing failed.\n");
        if(dataFile) fclose(dataFile);
        if(indexFile) fclose(indexFile);
        return;
    }

    // Code to check the status of the data and index files should go here

    for (int i = 0; i < n; i++) {
        char fieldName[MAX_STRING_LENGTH];
        char fieldValue[MAX_STRING_LENGTH];

        scanf("%s", fieldName);
        scan_quote_string(fieldValue); // Works for both quoted and non-quoted strings

        // Check if the field is the search key
        if (strcmp(fieldName, "nomeTecnologiaOrigemDestino") == 0) {
            // Search using B-tree
            int recordRRN = search_btree(indexFile, fieldValue); // Assuming search_btree returns the RRN or -1 if not found
            if(recordRRN != -1) {
                Data recordData;
                if (retrieve_record(dataFile, recordRRN, &recordData)) { // Assuming retrieve_record returns 1 on success
                    printa_registro(&recordData); 
                } else {
                    printf("Record not found.\n");
                }
            } else {
                printf("Record not found.\n");
            }
        } else {
           
        }
    }

    fclose(dataFile);
    fclose(indexFile);
}

#include <stdio.h>
#include <stdlib.h>

#define MAX_HASH_SIZE 100

struct RecordType {
    int id;
    char name;
    int order;
};

struct HashType {
    struct RecordType* records[MAX_HASH_SIZE];
    int count[MAX_HASH_SIZE];
};

int hash(int x) {
    return x % MAX_HASH_SIZE;
}

int parseData(char* inputFileName, struct RecordType** ppData) {
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile) {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL) {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i) {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, " %c ", &c);  // Added space to correctly read the char
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

void printRecords(struct RecordType pData[], int dataSz) {
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i) {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

void displayRecordsInHash(struct HashType* pHashArray) {
    for (int i = 0; i < MAX_HASH_SIZE; ++i) {
        if (pHashArray->count[i] > 0) {
            printf("index %d -> ", i);
            for (int j = 0; j < pHashArray->count[i]; ++j) {
                struct RecordType* record = &pHashArray->records[i][j];
                printf("%d, %c, %d -> ", record->id, record->name, record->order);
            }
            printf("\n");
        }
    }
}

int main(int argc, char* argv[]) {
    struct RecordType *pRecords;
    struct HashType hashTable = {0};
    int recordSz = 0;

    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    recordSz = parseData(argv[1], &pRecords);
    if (recordSz < 0) {
        fprintf(stderr, "Failed to parse data from the file\n");
        return 1;
    }

    printRecords(pRecords, recordSz);

    // Initialize hash table
    for (int i = 0; i < MAX_HASH_SIZE; ++i) {
        hashTable.records[i] = NULL;
        hashTable.count[i] = 0;
    }

    // Insert records into the hash table
    for (int i = 0; i < recordSz; ++i) {
        int index = hash(pRecords[i].id);
        hashTable.records[index] = &pRecords[i];
        hashTable.count[index]++;
    }

    displayRecordsInHash(&hashTable);

    free(pRecords);
    return 0;
}

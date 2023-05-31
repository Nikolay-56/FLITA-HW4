#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Задание: Найти все рёбра, смежные к вершинам с максимальной степенью вершины


typedef struct Graph {
    int** matrix;
    int verticesNum;
} Graph_t;



char* getData(FILE* filePtr) {
    fseek(filePtr, 0, SEEK_END);
    int fileLen = ftell(filePtr) - 1;
    fseek(filePtr, 0, SEEK_SET);

    char* data;
    data = (char *) malloc(sizeof(char) * fileLen);
    fread(data, sizeof(char), fileLen, filePtr);
    data[fileLen] = '\0';

    return data;
}




int getVerticesNum(const char* data) {
    int count = 0;
    int i = 0;

    while (data[i] != '\n') {
        if (data[i] != ' ')
            count++;
        i++;
    }

    return count;
}




Graph_t* initGraph(int verticesNum) {
    Graph_t* graph = (Graph_t*) malloc(sizeof(Graph_t));
    graph->verticesNum = verticesNum;
    graph->matrix = (int**) malloc(verticesNum * sizeof(int*));

    for (int i = 0; i < verticesNum; i++) {
        graph->matrix[i] = (int*) malloc(verticesNum * sizeof(int));
    }

    return graph;
}


void strToGraph(Graph_t* graph, char* data) {
    char* token;

    for (int i = 0; i < graph->verticesNum; i++) {
        for (int j = 0; j < graph->verticesNum; j++) {
            token = strtok_r(data, "\n ", &data);
            graph->matrix[i][j] = atoi(token);
        }
    }
}




int maxVerticesDegree(Graph_t* graph) {
    int maxDegree = 0;

    for (int i = 0; i < graph->verticesNum; i++) {
        int count = 0;

        for (int j = 0; j < graph->verticesNum; j++) {
            count += graph->matrix[i][j];
        }

        if (count > maxDegree)
            maxDegree = count;
    }
    return maxDegree;
}




int* returnListOfVerticesWithMaxDegree(Graph_t* graph) {
    int maxDegree = maxVerticesDegree(graph);
    int* listOfMaxDegrees = (int*) malloc(graph->verticesNum * sizeof(int));

    for (int i = 0; i < graph->verticesNum; i++) {
        int count = 0;

        for (int j = 0; j < graph->verticesNum; j++) {
            count += graph->matrix[i][j];
        }

        if (count == maxDegree){
            listOfMaxDegrees[i] = i;
        }
    }
    return listOfMaxDegrees;
}




void printEdges(Graph_t* graph) {
    int* listOfMaxDegrees = returnListOfVerticesWithMaxDegree(graph);

    for (int i = 0; i < graph->verticesNum; i++) {
        if (i == listOfMaxDegrees[i])
            for (int j = 0; j < graph->verticesNum; j++) {
                if (graph->matrix[i][j] != 0)
                    printf("%d - %d\n", i + 1, j + 1) ;
            }
    }
}




void freeGraph(Graph_t* graph) {
    for (int i = 0; i < graph->verticesNum; i++) {
        free(graph->matrix[i]);
    }

    free( graph->matrix);
    free(graph);
}




int main(int argc, char** argv) {
    if (argc != 2) {
        puts("Wrong number of arguments!");
        exit(EXIT_FAILURE);
    }

    const char* filePath = argv[1];
    FILE* filePtr = fopen(filePath, "r");

    if (filePtr == NULL) {
        puts("Error opening file");
        exit(EXIT_FAILURE);
    }

    char* data = getData(filePtr);
    fclose(filePtr);

    int vertices = getVerticesNum(data);
    Graph_t* graph = initGraph(vertices);

    strToGraph(graph, data);
    free(data);

    printEdges(graph);
    freeGraph(graph);
    return EXIT_SUCCESS;
}
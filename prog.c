#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

typedef struct {
    int x1, y1, x2, y2; // Координаты прямоугольника: верхний левый угол (x1, y1) и нижний правый угол (x2, y2)
} Rectangle;

typedef struct {
    int x1, y1, x2, y2; //  Координаты отрезка единичной длины: начало (x1, y1) и конец (x2, y2)
    int length;
    int is_perimiter;
} Segment;

Rectangle rectangles[] = {
    {0, 0, 3, 3},
    {1, 1, 5, 7},
    {2, 1, 4, 9},
    {4, 4, 8, 6},
    {7, 3, 10, 11},
    {4, 8, 10, 10},
    {9, 8, 13, 13},
    {13, 12, 17, 15},
    {13, 10, 15, 13},
    {14, 16, 19, 18},
    {7, 12, 20, 13},
    {5, 13, 17, 15},
    {16, 9, 18, 11},
    {22, 11, 25, 17},
    {16, 7, 17, 9},
    {13, 1, 20, 6},
    {20, 18, 21, 22},
    {19, 3, 24, 10},
};

int isPerrimeter(Rectangle *, int, Segment *, int, int, int);
int power(int, int);


void printSubset(Rectangle* subset, int size) {
    printf("{ ");
    for (int i = 0; i < size; i++) {
        printf("(%d, %d, %d, %d) ", subset[i].x1, subset[i].y1, subset[i].x2, subset[i].y2);
    }
    printf("}\n");
}

void saveSubset(Rectangle* subset, int size, Rectangle** subsets, int* count, int subsetSize[]) {
    for (int i = 0; i < size; i++) {
        subsets[*count][i] = subset[i];
    }
    subsetSize[*count] = size; // Сохранение размера подмножества
    (*count)++;
}

void generateAllSubsets(Rectangle* rectangles, int n, Rectangle* subset, int subsetSize, int index, Rectangle** subsets, int* count, int subsetSizeArray[]) {
    if (index == n) {
        //printSubset(subset, subsetSize);
        saveSubset(subset, subsetSize, subsets, count, subsetSizeArray);
        return;
    }
    
    subset[subsetSize] = rectangles[index];
    generateAllSubsets(rectangles, n, subset, subsetSize + 1, index + 1, subsets, count, subsetSizeArray);
    generateAllSubsets(rectangles, n, subset, subsetSize, index + 1, subsets, count, subsetSizeArray);
}

int printSubsetWithArea(Rectangle* subset, int size) {
    int totalArea = 0;
    printf("{ ");
    for (int i = 0; i < size; i++) {
        printf("(%d, %d, %d, %d) ", subset[i].x1, subset[i].y1, subset[i].x2, subset[i].y2);
        totalArea += (subset[i].x2 - subset[i].x1) * (subset[i].y2 - subset[i].y1);
    }
    printf("} Площадь подмножества: %d\n", totalArea);
    return totalArea;
}

int subsetWithArea(Rectangle* subset, int size) {
    int totalArea = 0;
    for (int i = 0; i < size; i++) {
        totalArea += (subset[i].x2 - subset[i].x1) * (subset[i].y2 - subset[i].y1);
    }
    return totalArea;
}

void process_subset(Rectangle* rectangles, int index_subset, int subsetSizeArray[], Rectangle** subsets, double * min_relation, int * min_relation_subset) {
    int s = index_subset;
    //printf("\n---------------------------------------------------\n");
    //printf("Начинаем обработку subset %d (Size %d): ", s + 1, subsetSizeArray[s]);
    if (subsetSizeArray[s] == 0) {
        //printf("Прерываем обработку subset %d (Size %d): ", s + 1, subsetSizeArray[s]);
        return;
    }
    //printSubset(subsets[s], subsetSizeArray[s]);
    int area = subsetWithArea(subsets[s], subsetSizeArray[s]);
    //printf("Площадь subset %d: %d", s + 1, area);
   //printf("\n---------------------------------------------------\n");
    int numRectangles = subsetSizeArray[s];
    Segment segments[numRectangles][4]; // массив отрезков для каждого прямоугольника
    int common_length = 0;
    for (int i = 0; i < numRectangles; ++i) {
        segments[i][0].x1 = subsets[s][i].x1;//верхнее ребро слева направо
        segments[i][0].y1 = subsets[s][i].y1;//верхнее ребро слева направо
        segments[i][0].x2 = subsets[s][i].x2;//верхнее ребро слева направо
        segments[i][0].y2 = subsets[s][i].y1;//верхнее ребро слева направо
        segments[i][0].length = subsets[s][i].x2 - subsets[s][i].x1;
        common_length += segments[i][0].length;
        segments[i][1].x1 = subsets[s][i].x1;//нижнее ребро слева направо
        segments[i][1].y1 = subsets[s][i].y2;//нижнее ребро слева направо
        segments[i][1].x2 = subsets[s][i].x2;//нижнее ребро слева направо
        segments[i][1].y2 = subsets[s][i].y2;//нижнее ребро слева направо
        segments[i][1].length = subsets[s][i].x2 - subsets[s][i].x1;
        common_length += segments[i][1].length;
        segments[i][2].x1 = subsets[s][i].x1;//левое ребро сверху вниз
        segments[i][2].y1 = subsets[s][i].y1;//левое ребро сверху вниз
        segments[i][2].x2 = subsets[s][i].x1;//левое ребро сверху вниз
        segments[i][2].y2 = subsets[s][i].y2;//левое ребро сверху вниз
        segments[i][2].length = subsets[s][i].y2 - subsets[s][i].y1;
        common_length += segments[i][2].length;
        segments[i][3].x1 = subsets[s][i].x2;//правое ребро сверху вниз
        segments[i][3].y1 = subsets[s][i].y1;//правое ребро сверху вниз
        segments[i][3].x2 = subsets[s][i].x2;//правое ребро сверху вниз
        segments[i][3].y2 = subsets[s][i].y2;//правое ребро сверху вниз
        segments[i][3].length = subsets[s][i].y2 - subsets[s][i].y1;
        common_length += segments[i][3].length;

        //printf("Прямоугольник %d:\n", i+1);
        for (int j = 0; j < 4; ++j) {
            //printf("Отрезок %d: x1=%d, y1=%d x2=%d, y2=%d\n", j+1, segments[i][j].x1, segments[i][j].y1, segments[i][j].x2, segments[i][j].y2);
        }
    }
    int counter = 0;
    Segment * segments_ed = NULL;
    int segments_ed_size = 0;
    for (int i = 0; i < numRectangles; ++i) {
        //printf("Начинаем исследование прямоугольника %d:\n", i+1);
        for (int j = 0; j < 4; ++j) {
            if (j == 0) {
                //printf("Начинаем исследование верхнего ребра прямоугольника %d: ", i+1);
                int len = segments[i][j].length;
                //printf("длина=%d x1=%d, y1=%d x2=%d, y2=%d\n", len, segments[i][j].x1, segments[i][j].y1, segments[i][j].x2, segments[i][j].y2);
                for (int k = 0; k < len; ++k) {
                    Segment segment = {segments[i][j].x1 + k, segments[i][j].y1, segments[i][j].x1 + k + 1, segments[i][j].y2, 0};
                    ++segments_ed_size;
                    segments_ed = (Segment *)realloc(segments_ed, segments_ed_size * sizeof(Segment));
                    int target_segment = segments_ed_size - 1;
                    segments_ed[target_segment] = segment;
                    counter += isPerrimeter(rectangles, numRectangles, segments_ed, target_segment, i, j);
                }
            }
            if (j == 1) {
                //printf("Начинаем исследование нижнего ребра прямоугольника %d: ", i+1);
                int len = segments[i][j].length;
                //printf("длина=%d x1=%d, y1=%d x2=%d, y2=%d\n", len, segments[i][j].x1, segments[i][j].y1, segments[i][j].x2, segments[i][j].y2);
                for (int k = 0; k < len; ++k) {
                    Segment segment = {segments[i][j].x1 + k, segments[i][j].y2, segments[i][j].x1 + k + 1, segments[i][j].y2, 0};
                    ++segments_ed_size;
                    segments_ed = (Segment *)realloc(segments_ed, segments_ed_size * sizeof(Segment));
                    int target_segment = segments_ed_size - 1;
                    segments_ed[target_segment] = segment;
                    counter += isPerrimeter(rectangles, numRectangles, segments_ed, target_segment, i, j);
                }
            }
            if (j == 2) {
                //printf("Начинаем исследование левого ребра прямоугольника %d: ", i+1);
                int len = segments[i][j].length;
                //printf("длина=%d x1=%d, y1=%d x2=%d, y2=%d\n", len, segments[i][j].x1, segments[i][j].y1, segments[i][j].x2, segments[i][j].y2);
                for (int k = 0; k < len; ++k) {
                    Segment segment = {segments[i][j].x1, segments[i][j].y1 + k, segments[i][j].x1, segments[i][j].y1 + k + 1, 0};
                    ++segments_ed_size;
                    segments_ed = (Segment *)realloc(segments_ed, segments_ed_size * sizeof(Segment));
                    int target_segment = segments_ed_size - 1;
                    segments_ed[target_segment] = segment;
                    counter += isPerrimeter(rectangles, numRectangles, segments_ed, target_segment, i, j);
                }
            }
            if (j == 3) {
                //printf("Начинаем исследование правого ребра прямоугольника %d: ", i+1);
                int len = segments[i][j].length;
                //printf("длина=%d x1=%d, y1=%d x2=%d, y2=%d\n", len, segments[i][j].x1, segments[i][j].y1, segments[i][j].x2, segments[i][j].y2);
                for (int k = 0; k < len; ++k) {
                    Segment segment = {segments[i][j].x2, segments[i][j].y1 + k, segments[i][j].x2, segments[i][j].y1 + k + 1, 0};
                    ++segments_ed_size;
                    segments_ed = (Segment *)realloc(segments_ed, segments_ed_size * sizeof(Segment));
                    int target_segment = segments_ed_size - 1;
                    segments_ed[target_segment] = segment;
                    counter += isPerrimeter(rectangles, numRectangles, segments_ed, target_segment, i, j);
                }
            }
        }
    }
    int cout_segments_checked = 0;
    for (int i = 0; i < segments_ed_size; ++i) {
        if (segments_ed[i].is_perimiter != 0) {
            ++cout_segments_checked;
            for (int j = 0; j < segments_ed_size; ++j) {
                if (i != j) {
                    if (segments_ed[i].x1 == segments_ed[j].x1 && segments_ed[i].x2 == segments_ed[j].x2 && segments_ed[i].y1 == segments_ed[j].y1 &&segments_ed[i].y2 == segments_ed[j].y2) {
                        segments_ed[j].is_perimiter = 0;
                        //printf("Встретили дубликат и сборосили на нуль его претензию быть периметром\n");
                    }
                }
            }
        }
    }
    //printf("Насчитали отмеченых единичных сегментов: %d\n", cout_segments_checked);
    //printf("Итоговая длинна периметра: %d\n", cout_segments_checked);
    double relation = ((double)area)/((double)cout_segments_checked);
    //printf("Отношение периметра к площади: %f\n", relation);
    if (*(min_relation) == 0) {
        *(min_relation) = relation;
        *(min_relation_subset) = s;
    } else {
        if (relation < *(min_relation)) {
            *(min_relation) = relation;
            *(min_relation_subset) = s;
        }
    }
    //printf("Насчитали: %d\n", counter);
    //printf("Общая длина всех периметров: %d\n", common_length);
    //printf("Создано единичных сегментов: %d\n", segments_ed_size);
    free(segments_ed);
    segments_ed_size = 0;
}
void processing(int * min_relation_subset, double * min_relation, int begin, int end, int rank) {
    int n = sizeof(rectangles) / sizeof(rectangles[0]);
    int numSubsets = power(2, n);
    Rectangle subset[n];
    Rectangle** subsets = (Rectangle**)malloc(numSubsets * sizeof(Rectangle*));
    for (int i = 0; i < numSubsets; ++i) {
        subsets[i] = (Rectangle*)malloc(n * sizeof(Rectangle));
    }
    int *subsetSizeArray = (int*)malloc(numSubsets * sizeof(int)); // Создание массива для хранения размеров подмножеств
    int count = 0;
    generateAllSubsets(rectangles, n, subset, 0, 0, subsets, &count, subsetSizeArray);
    //for (int i = 0; i < numSubsets; ++i) {
        //printf("Subset %d (Size %d): ", i + 1, subsetSizeArray[i]);
        //printSubset(subsets[i], subsetSizeArray[i]);
        //printSubsetWithArea(subsets[i], subsetSizeArray[i]);
    //}
    //printf("\n------------------Предобработка данных закончена--------------------------------\n");
    for (int s = begin; s < end; ++s) {
        process_subset(rectangles, s, subsetSizeArray, subsets, min_relation, min_relation_subset);
        //printf("Периметр на процессоре %d минимальное отношение периметра к площади у subset %d у подмножества %d равное %f\n", rank, s, *(min_relation_subset), *(min_relation));
    }
    for (int i = 0; i < numSubsets; i++) {
        free(subsets[i]);
    }
    free(subsetSizeArray);
    free(subsets);
}


int main(int argc, char* argv[]) {

    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (rank == 0) {
        if (size >= 2) {
            double start_time, end_time;
            start_time = MPI_Wtime();
            int n = sizeof(rectangles) / sizeof(rectangles[0]);
            printf("Начата обработка масссива из %d пряиоугольникова %d с процессорами\n", n, size);
            int numSubsets = power(2, n);
            double min_relation[5];
            int min_relation_subset[5];
            double relation = 0;
            int relation_subset = 0;
            int begin[10];
            int end[10];


            int subproc = size - 1;
            int subsetSize = (numSubsets / subproc);
            for (int i = 0; i < subproc; ++i) {
                begin[i] = i * subsetSize;
                printf("begin[%d]=%d\n", i, begin[i]);
                if (i != subproc - 1) {
                    end[i] = (i + 1) * subsetSize;
                    printf("end[%d]=%d\n", i, end[i]);
                } else {
                    end[subproc - 1] = numSubsets;
                    printf("end[%d]=%d\n", subproc - 1, end[subproc - 1]);
                }
            }
            
            /*for (int i = 0; i < size-1; ++i) {
                MPI_Send(&begin[i], 1, MPI_INT, i+1, 0, MPI_COMM_WORLD);
                MPI_Send(&end[i], 1, MPI_INT, i+1, 0, MPI_COMM_WORLD);
            }
            for (int i = 0; i < size-1; ++i) {
                MPI_Recv(&min_relation_subset[i], 1, MPI_INT, i+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(&min_relation[i], 1, MPI_DOUBLE, i+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }*/
            MPI_Request request[2*(size-1)];
            for (int i = 0; i < size-1; ++i) {
                MPI_Isend(&begin[i], 1, MPI_INT, i+1, 0, MPI_COMM_WORLD, &request[i]);
                MPI_Isend(&end[i], 1, MPI_INT, i+1, 0, MPI_COMM_WORLD, &request[size-1+i]);
            }
            MPI_Waitall(2*(size-1), request, MPI_STATUSES_IGNORE);

            for (int i = 0; i < size-1; ++i) {
                MPI_Irecv(&min_relation_subset[i], 1, MPI_INT, i+1, 0, MPI_COMM_WORLD, &request[i]);
                MPI_Irecv(&min_relation[i], 1, MPI_DOUBLE, i+1, 0, MPI_COMM_WORLD, &request[size-1+i]);
            }
            MPI_Waitall(2*(size-1), request, MPI_STATUSES_IGNORE);
            for (int i = 0; i < size-1; ++i) {
                printf("Получено от процессора %d минимальное подмножество %d с минимальным отношением: %f\n", i+1,  min_relation_subset[i], min_relation[i]);
                if (relation == 0 || (min_relation[i] < relation)) {
                    relation = min_relation[i];
                    relation_subset = min_relation_subset[i];
                }
            }
            end_time = MPI_Wtime();
            printf("Итоговое минимальное подмножество %d с минимальным отношением: %f\n", relation_subset, relation);
            printf("Общее время выполнения: %f секунд\n", end_time - start_time);
        }
    } else {
        int begin;
        int end;
        MPI_Recv(&begin, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&end, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("На процессоре %d считаем с %d по %d\n", rank, begin, end);
        double min_relation = 0;
        int min_relation_subset = 0;
        processing(&min_relation_subset, &min_relation, begin, end, rank);
        printf("Минимальное отношение периметра к площади у subset: %d\n", min_relation_subset);
        printf("Минимальное отношение периметра к площади: %f\n", min_relation);
        //MPI_Send(&min_relation_subset, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        //MPI_Send(&min_relation, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        MPI_Request request[2];
        MPI_Isend(&min_relation_subset, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request[0]);
        MPI_Isend(&min_relation, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &request[1]);
        MPI_Waitall(2, request, MPI_STATUSES_IGNORE);
    }
    
    MPI_Finalize();

    return 0;
}

int isPerrimeter(Rectangle * rectangles, int numRectangles, Segment * segments, int target_segment, int rect, int position) {
    for (int i = 0; i < numRectangles; ++i) {
        if (i != rect) {
            if (segments[target_segment].x1 >= rectangles[i].x1 && segments[target_segment].x2 <= rectangles[i].x2 && segments[target_segment].y1 >= rectangles[i].y1 && segments[target_segment].y2 <= rectangles[i].y2) {
                //printf("\tЕдиничный сегмент прямоугольника %d: x1=%d, y1=%d x2=%d, y2=%d принадлежит также прямоугольнику %d\n", rect+1, segments[target_segment].x1, segments[target_segment].y1, segments[target_segment].x2, segments[target_segment].y2, i+1);
                int j = i;
                if (position == 0) {
                    if (segments[target_segment].x1 >= rectangles[j].x1 && segments[target_segment].x2 <= rectangles[j].x2 && segments[target_segment].y1 == rectangles[j].y1 && segments[target_segment].y2 == rectangles[j].y1) {
                        //printf("\t\tЕдиничный сегмент прямоугольника %d: x1=%d, y1=%d x2=%d, y2=%d принадлежит также периметру прямоугольника %d\n", rect+1, segments[target_segment].x1, segments[target_segment].y1, segments[target_segment].x2, segments[target_segment].y2, j+1);
                        if (segments[target_segment].y1 - 1 >= rectangles[j].y1 && segments[target_segment].y2 - 1 >= rectangles[j].y1) {
                            //printf("\t\t\tПрямоугольники смыкаются\n");
                            return 0;
                        } else {
                            //printf("\t\t\tПрямоугольники не смыкаются\n");
                            ++segments[target_segment].is_perimiter;
                            return 1;
                        }
                    } else {
                        //printf("\t\tЕдиничный сегмент прямоугольника %d: x1=%d, y1=%d x2=%d, y2=%d не принадлежит периметру ни одного из других прямоугольников\n", rect+1, segments[target_segment].x1, segments[target_segment].y1, segments[target_segment].x2, segments[target_segment].y2);
                        return 0;
                    }
                }
                if (position == 1) {
                    if (segments[target_segment].x1 >= rectangles[j].x1 && segments[target_segment].x2 <= rectangles[j].x2 && segments[target_segment].y1 == rectangles[j].y2 && segments[target_segment].y2 == rectangles[j].y2) {
                        //printf("\t\tЕдиничный сегмент прямоугольника %d: x1=%d, y1=%d x2=%d, y2=%d принадлежит также периметру прямоугольника %d\n", rect+1, segments[target_segment].x1, segments[target_segment].y1, segments[target_segment].x2, segments[target_segment].y2, j+1);
                        if (segments[target_segment].y1 + 1 <= rectangles[j].y2 && segments[target_segment].y2 + 1 <= rectangles[j].y2) {
                            //printf("\t\t\tПрямоугольники смыкаются\n");
                            return 0;
                        } else {
                            //printf("\t\t\tПрямоугольники не смыкаются\n");
                            ++segments[target_segment].is_perimiter;
                            return 1;
                        }
                    } else {
                        //printf("\t\tЕдиничный сегмент прямоугольника %d: x1=%d, y1=%d x2=%d, y2=%d не принадлежит периметру ни одного из других прямоугольников\n", rect+1, segments[target_segment].x1, segments[target_segment].y1, segments[target_segment].x2, segments[target_segment].y2);
                        return 0;
                    }
                }
                if (position == 2) {
                    if (segments[target_segment].x1 == rectangles[j].x1 && segments[target_segment].x2 == rectangles[j].x1 && segments[target_segment].y1 >= rectangles[j].y1 && segments[target_segment].y2 <= rectangles[j].y2) {
                        //printf("\t\tЕдиничный сегмент прямоугольника %d: x1=%d, y1=%d x2=%d, y2=%d принадлежит также периметру прямоугольника %d\n", rect+1, segments[target_segment].x1, segments[target_segment].y1, segments[target_segment].x2, segments[target_segment].y2, j+1);
                        if (segments[target_segment].x1 - 1 <= rectangles[j].x2 && segments[target_segment].x2 - 1 <= rectangles[j].x2) {
                            //printf("\t\t\tПрямоугольники смыкаются\n");
                            return 0;
                        } else {
                            //printf("\t\t\tПрямоугольники не смыкаются\n");
                            ++segments[target_segment].is_perimiter;
                            return 1;
                        }
                    } else {
                         //printf("\t\tЕдиничный сегмент прямоугольника %d: x1=%d, y1=%d x2=%d, y2=%d не принадлежит периметру ни одного из других прямоугольников\n", rect+1, segments[target_segment].x1, segments[target_segment].y1, segments[target_segment].x2, segments[target_segment].y2);
                        return 0;
                    }
                }
                if (position == 3) {
                    if (segments[target_segment].x1 == rectangles[j].x2 && segments[target_segment].x2 == rectangles[j].x2 && segments[target_segment].y1 >= rectangles[j].y1 && segments[target_segment].y2 <= rectangles[j].y2) {
                        //printf("\t\tЕдиничный сегмент прямоугольника %d: x1=%d, y1=%d x2=%d, y2=%d принадлежит также периметру прямоугольника %d\n", rect+1, segments[target_segment].x1, segments[target_segment].y1, segments[target_segment].x2, segments[target_segment].y2, j+1);
                        if (segments[target_segment].x1 + 1 <= rectangles[j].x1 && segments[target_segment].x2 + 1 <= rectangles[j].x1) {
                            //printf("\t\t\tПрямоугольники смыкаются\n");
                            return 0;
                        } else {
                            //printf("\t\t\tПрямоугольники не смыкаются\n");
                            ++segments[target_segment].is_perimiter;
                            return 1;
                        }
                    } else {
                        //printf("\t\tЕдиничный сегмент прямоугольника %d: x1=%d, y1=%d x2=%d, y2=%d не принадлежит периметру ни одного из других прямоугольников\n", rect+1, segments[target_segment].x1, segments[target_segment].y1, segments[target_segment].x2, segments[target_segment].y2);
                        return 0;
                    }
                }
            }
        }
    }
    segments[target_segment].is_perimiter = 1;
    //printf("\tЕдиничный сегмент прямоугольника %d: x1=%d, y1=%d x2=%d, y2=%d явялется периметром\n", rect+1, segments[target_segment].x1, segments[target_segment].y1, segments[target_segment].x2, segments[target_segment].y2);
    return 1;
}

int power(int base, int exponent) {
    int result = 1;
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}

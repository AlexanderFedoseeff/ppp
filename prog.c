#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

typedef struct {
    int x1, y1, x2, y2; // Координаты прямоугольника: верхний левый угол (x1, y1) и нижний правый угол (x2, y2)
} Rectangle;

typedef struct {
    int x1, y1, x2, y2; //  Координаты отрезка единичной длины: начало (x1, y1) и конец (x2, y2)
    int length;
} Segment;

int isPerrimeter(Rectangle *, int, Segment *, int, int);//Прямоугольники, их число, еденичный сегмент, прямоугольник из которого этот сегмент 

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);                                                                                                                     

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Создаем структуру прямоугольника
    Rectangle rectangles[] = {
        {0, 0, 3, 3},
        {1, 1, 5, 7},
        {2, 1, 4, 9},
        {4, 4, 8, 6},
        {7, 3, 10, 11},
        {4, 8, 10, 10}
    };
    int numRectangles = 6;
    int min_x = 0;
    int max_x = 11;
    int min_y = 0;
    int max_y = 11;
    Segment segments[6][4]; // массив отрезков для каждого прямоугольника
    if (rank == 0) {
        int common_length = 0;
        for (int i = 0; i < numRectangles; ++i) {
            segments[i][0].x1 = rectangles[i].x1;//верхнее ребро слева направо
            segments[i][0].y1 = rectangles[i].y1;//верхнее ребро слева направо
            segments[i][0].x2 = rectangles[i].x2;//верхнее ребро слева направо
            segments[i][0].y2 = rectangles[i].y1;//верхнее ребро слева направо
            segments[i][0].length = rectangles[i].x2 - rectangles[i].x1;
            common_length += segments[i][0].length;
            segments[i][1].x1 = rectangles[i].x1;//нижнее ребро слева направо
            segments[i][1].y1 = rectangles[i].y2;//нижнее ребро слева направо
            segments[i][1].x2 = rectangles[i].x2;//нижнее ребро слева направо
            segments[i][1].y2 = rectangles[i].y2;//нижнее ребро слева направо
            segments[i][1].length = rectangles[i].x2 - rectangles[i].x1;
            common_length += segments[i][1].length;
            segments[i][2].x1 = rectangles[i].x1;//левое ребро сверху вниз
            segments[i][2].y1 = rectangles[i].y1;//левое ребро сверху вниз
            segments[i][2].x2 = rectangles[i].x1;//левое ребро сверху вниз
            segments[i][2].y2 = rectangles[i].y2;//левое ребро сверху вниз
            segments[i][2].length = rectangles[i].y2 - rectangles[i].y1;
            common_length += segments[i][2].length;
            segments[i][3].x1 = rectangles[i].x2;//правое ребро сверху вниз
            segments[i][3].y1 = rectangles[i].y1;//правое ребро сверху вниз
            segments[i][3].x2 = rectangles[i].x2;//правое ребро сверху вниз
            segments[i][3].y2 = rectangles[i].y2;//правое ребро сверху вниз
            segments[i][3].length = rectangles[i].y2 - rectangles[i].y1;
            common_length += segments[i][3].length;

            printf("Прямоугольник %d:\n", i+1);
            for (int j = 0; j < 4; ++j) {
                printf("Отрезок %d: x1=%d, y1=%d x2=%d, y2=%d\n", j+1, segments[i][j].x1, segments[i][j].y1, segments[i][j].x2, segments[i][j].y2); 
            }
        }
        int counter = 0;
        Segment * segments_ed = NULL;
        int segments_ed_size = 0;
        for (int i = 0; i < numRectangles; ++i) {
            printf("Начинаем исследование прямоугольника %d:\n", i+1);
            for (int j = 0; j < 4; ++j) {
                if (j == 0) { 
                    printf("Начинаем исследование верхнего ребра прямоугольника %d: ", i+1);
                    int len = segments[i][j].length;
                    printf("длина=%d x1=%d, y1=%d x2=%d, y2=%d\n", len, segments[i][j].x1, segments[i][j].y1, segments[i][j].x2, segments[i][j].y2);
                    for (int k = 0; k < len; ++k) {
                        Segment segment = {segments[i][j].x1 + k, segments[i][j].y1, segments[i][j].x1 + k + 1, segments[i][j].y2};
                        ++segments_ed_size;
                        segments_ed = (Segment *)realloc(segments_ed, segments_ed_size * sizeof(Segment));
                        segments_ed[segments_ed_size - 1] = segment;
                        //counter += isPerrimeter(rectangles, numRectangles, &segments_ed[segments_ed_size - 1], i, j);продолжу когда оживет сервак
                        counter += isPerrimeter(rectangles, numRectangles, &segment, i, j);
                    }
                }
                if (j == 1) { 
                    printf("Начинаем исследование нижнего ребра прямоугольника %d: ", i+1);
                    int len = segments[i][j].length;
                    printf("длина=%d x1=%d, y1=%d x2=%d, y2=%d\n", len, segments[i][j].x1, segments[i][j].y1, segments[i][j].x2, segments[i][j].y2);
                    for (int k = 0; k < len; ++k) {
                        Segment segment = {segments[i][j].x1 + k, segments[i][j].y2, segments[i][j].x1 + k + 1, segments[i][j].y2};
                        ++segments_ed_size;
                        segments_ed = (Segment *)realloc(segments_ed, segments_ed_size * sizeof(Segment));
                        segments_ed[segments_ed_size - 1] = segment;
                        counter += isPerrimeter(rectangles, numRectangles, &segment, i, j);
                    }
                }
                if (j == 2) { 
                    printf("Начинаем исследование левого ребра прямоугольника %d: ", i+1);
                    int len = segments[i][j].length;
                    printf("длина=%d x1=%d, y1=%d x2=%d, y2=%d\n", len, segments[i][j].x1, segments[i][j].y1, segments[i][j].x2, segments[i][j].y2);
                    for (int k = 0; k < len; ++k) {
                        Segment segment = {segments[i][j].x1, segments[i][j].y1 + k, segments[i][j].x1, segments[i][j].y1 + k + 1};
                        ++segments_ed_size;
                        segments_ed = (Segment *)realloc(segments_ed, segments_ed_size * sizeof(Segment));
                        segments_ed[segments_ed_size - 1] = segment;
                        counter += isPerrimeter(rectangles, numRectangles, &segment, i, j);
                    }
                }
                if (j == 3) { 
                    printf("Начинаем исследование правого ребра прямоугольника %d: ", i+1);
                    int len = segments[i][j].length;
                    printf("длина=%d x1=%d, y1=%d x2=%d, y2=%d\n", len, segments[i][j].x1, segments[i][j].y1, segments[i][j].x2, segments[i][j].y2);
                    for (int k = 0; k < len; ++k) {
                        Segment segment = {segments[i][j].x2, segments[i][j].y1 + k, segments[i][j].x2, segments[i][j].y1 + k + 1};
                        ++segments_ed_size;
                        segments_ed = (Segment *)realloc(segments_ed, segments_ed_size * sizeof(Segment));
                        segments_ed[segments_ed_size - 1] = segment;
                        counter += isPerrimeter(rectangles, numRectangles, &segment, i, j);
                    }
                }
            }
        }
        
        printf("Насчитали: %d\n", counter);
        printf("Общая длина всех периметров: %d\n", common_length);
        free(segments_ed);
        printf("Создано единичных сегментов: %d\n", segments_ed_size);
        segments_ed_size = 0;
    }
    
    int numCells = 0;
    int searchAreas = 0;

    // Собираем на процессе с рангом 0
    MPI_Reduce(&numCells, &searchAreas, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("хз что: %d\n", searchAreas);
    }
    MPI_Finalize();

    return 0;
}

int isPerrimeter(Rectangle * rectangles, int numRectangles, Segment * segment, int rect, int position) {
    for (int i = 0; i < numRectangles; ++i) {
        if (i != rect) {
            if (segment->x1 >= rectangles[i].x1 && segment->x2 <= rectangles[i].x2 && segment->y1 >= rectangles[i].y1 && segment->y2 <= rectangles[i].y2) {
                printf("\tЕдиничный сегмент прямоугольника %d: x1=%d, y1=%d x2=%d, y2=%d принадлежит также прямоугольнику %d\n", rect+1, segment->x1, segment->y1, segment->x2, segment->y2, i+1);
                int j = i;
                if (position == 0) {
                    if (segment->x1 >= rectangles[j].x1 && segment->x2 <= rectangles[j].x2 && segment->y1 == rectangles[j].y1 && segment->y2 == rectangles[j].y1) {
                        printf("\t\tЕдиничный сегмент прямоугольника %d: x1=%d, y1=%d x2=%d, y2=%d принадлежит также периметру прямоугольника %d\n", rect+1, segment->x1, segment->y1, segment->x2, segment->y2, j+1);
                        if (segment->y1 - 1 >= rectangles[j].y1 && segment->y2 - 1 >= rectangles[j].y1) {
                            printf("\t\t\tПрямоугольники смыкаются\n");
                            return 0;
                        } else {
                            printf("\t\t\tПрямоугольники не смыкаются\n");
                            return 1;
                        }
                    } else {
                        //printf("\t\tЕдиничный сегмент прямоугольника %d: x1=%d, y1=%d x2=%d, y2=%d не принадлежит периметру ни одного из других прямоугольников\n", rect+1, segment->x1, segment->y1, segment->x2, segment->y2);
                        return 0;
                    }
                }
                if (position == 1) {
                    if (segment->x1 >= rectangles[j].x1 && segment->x2 <= rectangles[j].x2 && segment->y1 == rectangles[j].y2 && segment->y2 == rectangles[j].y2) {
                        printf("\t\tЕдиничный сегмент прямоугольника %d: x1=%d, y1=%d x2=%d, y2=%d принадлежит также периметру прямоугольника %d\n", rect+1, segment->x1, segment->y1, segment->x2, segment->y2, j+1);
                        if (segment->y1 + 1 <= rectangles[j].y2 && segment->y2 + 1 <= rectangles[j].y2) {
                            printf("\t\t\tПрямоугольники смыкаются\n");
                            return 0;
                        } else {
                            printf("\t\t\tПрямоугольники не смыкаются\n");
                            return 1;
                        }
                    } else {
                        //printf("\t\tЕдиничный сегмент прямоугольника %d: x1=%d, y1=%d x2=%d, y2=%d не принадлежит периметру ни одного из других прямоугольников\n", rect+1, segment->x1, segment->y1, segment->x2, segment->y2);
                        return 0;
                    }
                }
                if (position == 2) {
                    if (segment->x1 == rectangles[j].x1 && segment->x2 == rectangles[j].x1 && segment->y1 >= rectangles[j].y1 && segment->y2 <= rectangles[j].y2) {
                        printf("\t\tЕдиничный сегмент прямоугольника %d: x1=%d, y1=%d x2=%d, y2=%d принадлежит также периметру прямоугольника %d\n", rect+1, segment->x1, segment->y1, segment->x2, segment->y2, j+1);
                        if (segment->x1 - 1 <= rectangles[j].x2 && segment->x2 - 1 <= rectangles[j].x2) {
                            printf("\t\t\tПрямоугольники смыкаются\n");
                            return 0;
                        } else {
                            printf("\t\t\tПрямоугольники не смыкаются\n");
                            return 1;
                        }
                    } else {
                         //printf("\t\tЕдиничный сегмент прямоугольника %d: x1=%d, y1=%d x2=%d, y2=%d не принадлежит периметру ни одного из других прямоугольников\n", rect+1, segment->x1, segment->y1, segment->x2, segment->y2);
                        return 0;
                    }
                }
                if (position == 3) {
                    if (segment->x1 == rectangles[j].x2 && segment->x2 == rectangles[j].x2 && segment->y1 >= rectangles[j].y1 && segment->y2 <= rectangles[j].y2) {
                        printf("\t\tЕдиничный сегмент прямоугольника %d: x1=%d, y1=%d x2=%d, y2=%d принадлежит также периметру прямоугольника %d\n", rect+1, segment->x1, segment->y1, segment->x2, segment->y2, j+1);
                        if (segment->x1 + 1 <= rectangles[j].x1 && segment->x2 + 1 <= rectangles[j].x1) {
                            printf("\t\t\tПрямоугольники смыкаются\n");
                            return 0;
                        } else {
                            printf("\t\t\tПрямоугольники не смыкаются\n");
                            return 1;
                        }
                    } else {
                        //printf("\t\tЕдиничный сегмент прямоугольника %d: x1=%d, y1=%d x2=%d, y2=%d не принадлежит периметру ни одного из других прямоугольников\n", rect+1, segment->x1, segment->y1, segment->x2, segment->y2);
                        return 0;
                    }
                }
            }
        }
    }
    printf("\tЕдиничный сегмент прямоугольника %d: x1=%d, y1=%d x2=%d, y2=%d явялется периметром\n", rect+1, segment->x1, segment->y1, segment->x2, segment->y2);
    return 1;
}
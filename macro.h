#define COMPARISON(FigurdEnter, FigurComparison, FigurLength, result) \
    {                                                                 \
        char* TempString = malloc((FigurLength + 1) * sizeof(char));  \
        strcpy(TempString, FigurdEnter);                              \
        TempString[FigurLength] = '\0';                               \
        result = !strcmp(TempString, FigurComparison);                \
        free(TempString);                                             \
    }

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define _GNU_SOURCE

double eucDist(double *pointA, double *pointB, int vecSize)
{
    double dist = 0;
    for (int i = 0; i < vecSize; i++)
    {
        dist = dist + ((pointA[i] - pointB[i]) * (pointA[i] - pointB[i]));
    }
    return sqrt(dist);
}

int main(int argc, char *argv[])
{
    /***** DATA FETCH *****/

    // arg size should be 5 for full, 4 for missing iteration
    char *err = NULL;
    double EPSILON = 0.001;
    int iter = 100;
    int k = strtol(argv[1], err, 10); // K should be int
    int pointIndex = 0;
    if (argc == 6 || argc == 7)
    {
        if (argc == 6)
        {
            pointIndex = 5;
        }
        else
        {
            pointIndex = 6;
        }
    }
    else
    {
        printf("An Error Has Occurred");
        return 1;
    }

    if (err == argv[1] || *err != '\0')
    {
        printf("Invalid number of clusters!");
        return 1;
    }
    else
    {
        // k is integer
        if (k <= 0)
        {
            printf("Invalid number of clusters!");
            return 1;
        }
    }
    int n = strtol(argv[2], err, 10); // N should be int
    if (err == argv[2] || *err != '\0')
    {
        printf("Invalid number of points!");
        return 1;
    }
    else
    {
        // k is integer
        if (n <= 0)
        {
            printf("Invalid number of points!");
            return 1;
        }
        if (k >= n)
        {
            printf("Invalid number of clusters!");
            return 1;
        }
    }
    int d = strtol(argv[3], err, 10); // d should be int
    if (err == argv[3] || *err != '\0')
    {
        printf("Invalid dimension of point!");
        return 1;
    }
    else
    {
        // d is integer
        if (d <= 0)
        {
            printf("Invalid dimension of point!");
            return 1;
        }
    }
    if (argc == 5)
    {
        iter = strtol(argv[4], err, 10); // if iter present it should be int
        if (err == argv[4] || *err != '\0')
        {
            printf("Invalid maximum iteration!");
            return 1;
        }
        else
        {
            if (iter >= 1000 || iter <= 0)
            {
                printf("Invalid maximum iteration!");
                return 1;
            }
        }
    }
    /***** FETCH POINTS *****/
    double **points = (double **)malloc(n * sizeof(double *)); // the actual points
    int *direct = malloc(n * sizeof(int));                     // points to in what cluster each point belongs
    int *clusterSize = malloc(k * sizeof(int));                // saves cluster size
    for (int i = 0; i < n; i++)
    {
        direct[i] = -1;
    }
    for (int i = 0; i < n; i++)
    {
        points[i] = (double *)malloc(d * sizeof(double));
    }
    int read;
    char *line = NULL;
    size_t len = 0;
    int g = 0;
    const char *delim = " ,\n";
    for (int i = 0; i < n; i++)
    {
        // read the entire line
        read = getline(&line, &len, stdin);
        if (read == -1)
        {
            free(line);
            free(direct);
            free(clusterSize);
            for (int i = 0; i < n; i++)
            {
                free(points[i]);
            }
            free(points);
            printf("An Error Has Occurred");
            return 1;
        }
        g = 0;
        char *token = strtok(line, delim);
        while (token != NULL)
        {
            points[i][g] = strtod(token, NULL);
            token = strtok(NULL, delim);
            g++;
        }
    }
    free(line);
    /***** INIT CENTER *****/
    double **centers = (double **)malloc(k * sizeof(double *));
    for (int i = 0; i < k; i++)
    {
        centers[i] = (double *)malloc(d * sizeof(double));
    }

    double **prevCenters = (double **)malloc(k * sizeof(double *));
    for (int i = 0; i < k; i++)
    {
        prevCenters[i] = (double *)malloc(d * sizeof(double));
    }
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < d; j++)
        {
            centers[i][j] = points[i][j];
        }
    }
    /***** ITERATION LOOP *****/
    int flag = 1;
    int bestIndex = 0;
    double bestDist = 0.0;
    double currentDist = 0.0;
    while (flag != 0 && iter > 0)
    {
        // prepare prev centers
        for (int i = 0; i < k; i++)
        {
            for (int j = 0; j < d; j++)
            {
                prevCenters[i][j] = centers[i][j];
            }
        }
        for (int i = 0; i < n; i++)
        {
            clusterSize[i] = 0;
        }
        flag = 0;
        // direct points to clusters
        for (int i = 0; i < n; i++)
        {
            bestDist = -1;
            bestIndex = 0;
            for (int j = 0; j < k; j++)
            {
                currentDist = eucDist(points[i], centers[j], d);
                if (bestDist == -1 || bestDist > currentDist)
                {
                    bestDist = currentDist;
                    bestIndex = j;
                }
            }
            direct[i] = bestIndex;
            clusterSize[bestIndex] = clusterSize[bestIndex] + 1;
        }
        // make new centers
        // step 1 clean centers
        for (int i = 0; i < k; i++)
        {
            for (int j = 0; j < d; j++)
            {
                centers[i][j] = 0.0;
            }
        }
        // step 2 adjust new center raw values
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < d; j++)
            {
                centers[direct[i]][j] = centers[direct[i]][j] + points[i][j]; // direct directs each point to a relevant center so direct[i] will give point[i] its cluster index
            }
        }
        // step3 divide each coord by the size
        for (int i = 0; i < k; i++)
        {
            for (int j = 0; j < d; j++)
            {
                centers[i][j] = centers[i][j] / ((double)clusterSize[i]);
            }
        }
        // test new centers against prev centers
        for (int i = 0; i < k; i++)
        {
            if (eucDist(centers[i], prevCenters[i], d) >= EPSILON)
            {
                flag = 1;
            }
        }
    }
    /***** PRINT CENTERS *****/
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < d; j++)
        {
            printf("%.4f", centers[i][j]);
            if (j < d - 1)
            {
                printf(",");
            }
        }
        printf("\n");
    }
    /***** FREE MEMORY *****/
    free(direct);
    free(clusterSize);
    for (int i = 0; i < n; i++)
    {
        free(points[i]);
    }
    free(points);
    for (int i = 0; i < k; i++)
    {
        free(centers[i]);
        free(prevCenters[i]);
    }
    free(centers);
    free(prevCenters);
    return 0;
}

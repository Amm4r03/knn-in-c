// simple implementation of the K-nearest-neighbors algorithm in C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// allow a datapoint to have not more than 10 dimensions
#define MAX_COORDINATES 10

typedef struct DataPoint {
    float coordinates[MAX_COORDINATES];
    int numDimensions;
    char* label;
} DataPoint;

typedef struct DistanceIndex {
    float distance;
    int index;
} DistanceIndex;

DataPoint* createDataPoint(float *coordinates, int numCoordinates, const char* label) {
    if (numCoordinates > MAX_COORDINATES) {
        fprintf(stderr, "ERROR : num_coord for DataPoint is greater than MAX_COORDINATES (10)\n");
        return (DataPoint*) NULL;
    }

    DataPoint* dp = (DataPoint*) malloc(sizeof(DataPoint));
    for (int i = 0; i < numCoordinates; i++) {
        dp->coordinates[i] = coordinates[i];
    }
    for (int i = numCoordinates; i < MAX_COORDINATES; i++) {
        dp->coordinates[i] = 0;
    }

    dp->numDimensions = numCoordinates;

    dp->label = (char*) malloc(strlen(label) + 1);
    strcpy(dp->label, label);
    return dp;
}

void freeDataPoint(DataPoint* dp) {
    free(dp->label);
    free(dp);
    printf("deleted Datapoint\n");
}

// compute euclidean distance between two datapoints of equal dimensions
float calculate_euclidean_distance(DataPoint p1, DataPoint p2) {
    if (p1.numDimensions != p2.numDimensions) {
        fprintf(stderr, "ERROR : given datapoints are of unequal dimensions (%d and %d)\n", p1.numDimensions, p2.numDimensions);
        return NAN;
    }

    float distance = 0;

    for (int i = 0; i < p1.numDimensions; i++) {
        float diff = (float) (p1.coordinates[i] - p2.coordinates[i]);
        distance += (diff * diff);
    }
    return sqrtf(distance);
}

float compute_manhattan_distance(DataPoint p1, DataPoint p2) {
    if (p1.numDimensions != p2.numDimensions) {
        fprintf(stderr, "ERROR : given datapoints are of unequal dimensions (%d and %d)\n", p1.numDimensions, p2.numDimensions);
        return NAN;
    }

    float distance = 0;

    for (int i = 0; i < p1.numDimensions; i++) {
        float diff = (float) (p1.coordinates[i] - p2.coordinates[i]);
        distance += abs(diff);
    }
    return distance;
}

int compare(const void* a, const void* b) {
    float diff = ((DistanceIndex*)a)->distance - ((DistanceIndex*)b)->distance;
    return (diff > 0) - (diff < 0);
}

// KNN implementation
const char* k_nearest_neighbors(DataPoint currentPoints[], int currentPointCount, DataPoint testPoint, int k) {
    // float *distances = (float*) malloc(currentPointCount * sizeof(float));
    DistanceIndex* distances = (DistanceIndex*) malloc(currentPointCount * sizeof(DistanceIndex));

    for (int i = 0; i < currentPointCount; i++) {
        distances[i].distance = calculate_euclidean_distance(currentPoints[i], testPoint);
        distances[i].index = i;
    }

    qsort(distances, currentPointCount, sizeof(DistanceIndex), compare);

    printf("\n\n%d nearest neighbors for our testpoint\n", k);
    for (int i = 0; i < k; i++) {
        printf("point %d | distance : %f | label : %s\n", distances[i].index, distances[i].distance, currentPoints[distances[i].index].label);
    }

    int labelCounts[MAX_COORDINATES] = {0};
    const char* labels[MAX_COORDINATES];
    int labelSize = 0;

    for (int i = 0; i < k; i++) {
            const char* label = currentPoints[distances[i].index].label;
            int found = 0;

            for (int j = 0; j < labelSize; j++) {
                if (strcmp(labels[j], label) == 0) {
                    labelCounts[j]++;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                labels[labelSize] = label;
                labelCounts[labelSize] = 1;
                labelSize++;
            }
        }
    int maxCount = 0;
    const char* mostFreqLabel = NULL;
    for (int i = 0; i < labelSize; i++) {
        if (labelCounts[i] > maxCount) {
            maxCount = labelCounts[i];
            mostFreqLabel = labels[i];
        }
    }

    free(distances);
    return mostFreqLabel;
}

int main() {
    int num_dimensions;
    printf("enter the number of dimensions for all points : ");
    scanf("%d", &num_dimensions);

    int num_datapoints;
    printf("enter the number of points to add in the dataset : ");
    scanf("%d", &num_datapoints);

    DataPoint train_data[num_datapoints];

    // take data for all datapoints
    for (int i = 0; i < num_datapoints; i++) {
        float coordinates[num_dimensions];
        printf("enter the dimensions for point %d : ", i + 1);
        for (int j = 0; j < num_dimensions; j++) {
            scanf("%f", &coordinates[j]);
        }

        char label[100];
        printf("enter the label for point %d : ", i + 1);
        scanf("%s", label);

        train_data[i] = *createDataPoint(coordinates, num_dimensions, label);
    }

    // take data for the testpoint
    DataPoint testPoint;
    printf("enter coordinates for test point : ");
    float testCoordinates[num_dimensions];
    for (int i = 0; i < num_dimensions; i++) {
        scanf("%f", &testCoordinates[i]);
    }
    testPoint = *createDataPoint(testCoordinates, num_dimensions, "test");
    // free(testCoordinates);

    int k = 5;
    const char* predicted_label = k_nearest_neighbors(train_data, num_datapoints, testPoint, k);
    printf("\n\npredicted label : %s\n", predicted_label);

    // freeDataPoint(&testPoint);
    // for (int i = 0; i < num_datapoints; i++) {
    //     freeDataPoint(&train_data[i]);
    // }
    // free(train_data);
    printf("freed all memory - exiting\n");

    return 0;
}
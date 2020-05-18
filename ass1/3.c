#include <stdio.h>

#define MAXVAL 999
#define MIN_CONCEN 52.5
#define MIN_TONNES 375

int stage_one(double A[], double B[]);
void stage_two(double A[], double B[], int n);
void stage_three();

int
main (int argc, char *argv[]) {
    double A[MAXVAL];
    double B[MAXVAL];
    int n;

    n = stage_one(A, B);
    printf("\n");
    stage_two(A, B, n);
    printf("\n");
    stage_three(A, B, n);
}

int
stage_one (double A[], double B[]) {
    char str1[6], str2[7];
    int n = 0;
    double tonnes, percent;
    double sum_tonnes = 0, concentration = 0;   

    // scanf part
    scanf("%s %s", str1, str2);
    while (scanf("%lf %lf", &tonnes, &percent) == 2) {
        A[n] = tonnes;
        B[n] = percent;
        n++;
        sum_tonnes += tonnes;
    }

    // calculate concentration
    for (int i=0; i<n; i++) {
        concentration += A[i] * B[i];
    }
    concentration /= sum_tonnes;

    // print result
    for (int i=0; i<n; i+=n-1) {
        printf("S1,    wagon %4d, tonnes= %5.1lf, %%= %5.1lf\n", i+1, A[i], B[i]);
    }
    printf("S1,   whole train, tonnes= %5.1lf, %%= %5.1lf\n", sum_tonnes, concentration);

    return n;
}

void 
stage_two(double A[], double B[], int n) {
    double progress = 0, concentration = 0;
    int flag = 0, consignment_num = 0;
    int fail_test[MAXVAL]={0}, failarray_count = 0;

    for (int i=0; i<=n; i++) {
        if (progress > MIN_TONNES) {
            concentration /=  progress;

            if (concentration > MIN_CONCEN) {
                consignment_num++;
                printf("S2,   consignment %d, tonnes= %.1lf, %%= %.1lf\n", consignment_num, progress, concentration);
                printf("S2,                  wagons= ");
                for (int j=flag; j<i; j++) {
                    printf("  %d", j+1);
                }
                printf("\n");
            }
            else {
                for (int j=flag; j<i; j++) {
                    fail_test[failarray_count] = j+1;
                    failarray_count++;
                }
            }

            progress = 0;
            concentration = 0;
            flag = i;
            i--;
        }
        else {
            progress += A[i];
            concentration += A[i] * B[i];
            if (i == n) {
                for (int j=flag; j<i; j++) {
                    fail_test[failarray_count] = j+1;
                    failarray_count++;
                }
            }
        }
    }

    flag = 0;
    
    progress = 0;
    concentration = 0;
    failarray_count = 0;
    while(fail_test[failarray_count] != 0) {
        progress += A[fail_test[failarray_count]-1];
        concentration += A[fail_test[failarray_count]-1] * B[fail_test[failarray_count]-1];
        failarray_count++;
    }
    concentration /= progress;

    printf("S2,    total dumped, tonnes= %.1lf, %%= %.1lf\n", progress, concentration);
    printf("S2,                  wagons= ");
    for(int j=0; j<failarray_count; j++) {
        printf("  %d", fail_test[j]);
    }
    printf("\n");
}

void
stage_three(double A[], double B[], int n) {
    double progress = 0, concentration = 0;
    int flag = 0, consignment_num = 0;
    int fail_test[MAXVAL]={0}, failarray_count = 0;

    for (int i=0; i<=n; i++) {
        if (progress > MIN_TONNES) {
            concentration /=  progress;

            if (concentration > MIN_CONCEN) {
                consignment_num++;
                printf("S3,   consignment %d, tonnes= %.1lf, %%= %.1lf\n", consignment_num, progress, concentration);
                printf("S3,                  wagons= ");
                for (int j=flag; j<i; j++) {
                    printf("  %d", j+1);
                }
                printf("\n");
            }
            else {
                int min_concen = B[flag], min = 0;
                for(int j=flag; j<i; j++) {
                    if (min_concen > B[j]) {
                        min_concen = B[j];
                        min = j;
                    }
                }
                fail_test[failarray_count] = min+1;
                failarray_count++;
                concentration = 0;
                progress -= A[min];
                while (progress < MIN_TONNES) {
                    progress += A[i++];
                }
                for(int j=flag; j<i; j++) {
                    if(j!=min) concentration += A[j] * B[j];
                }
                concentration /= progress;

                if (concentration > MIN_CONCEN) {
                    consignment_num++;
                    printf("S3,   consignment %d, tonnes= %.1lf, %%= %.1lf\n", consignment_num, progress, concentration);
                    printf("S3,                  wagons= ");
                    for (int j=flag; j<i; j++) {
                        if(j!=min) printf("  %d", j+1);
                    }
                    printf("\n");
                }
                else {
                    for (int j=flag; j<i; j++) {
                    fail_test[failarray_count] = j+1;
                    failarray_count++;
                    }
                }
            }
            progress = 0;
            concentration = 0;
            flag = i;
            i--;
        }
        else {
            progress += A[i];
            concentration += A[i] * B[i];
            if (i == n) {
                for (int j=flag; j<i; j++) {
                    fail_test[failarray_count] = j+1;
                    failarray_count++;
                }
            }
        }
    }

    flag = 0;
    
    progress = 0;
    concentration = 0;
    failarray_count = 0;
    while(fail_test[failarray_count] != 0) {
        progress += A[fail_test[failarray_count]-1];
        concentration += A[fail_test[failarray_count]-1] * B[fail_test[failarray_count]-1];
        failarray_count++;
    }
    concentration /= progress;

    printf("S3,    total dumped, tonnes= %.1lf, %%= %.1lf\n", progress, concentration);
    printf("S3,                  wagons= ");
    for(int j=0; j<failarray_count; j++) {
        printf("  %d", fail_test[j]);
    }
    printf("\n");

}

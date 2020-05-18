#include <stdio.h>

#define DEBUG 1
#if DEBUG
#define DUMP_DBL(x) printf("line %d: %s = %d\n", __LINE__, #x, x)
#else
#define DUMP_DBL(x)
#endif

#define MAX_WAGONS 999
#define MIN_CONCEN 52.5
#define MIN_TONNES 375

struct wagon_t {
    double tonnes;
    double percent;
};

int stage_one(struct wagon_t *wagons);
void stage_two(struct wagon_t *wagons, int numb_wagon, int *target);
void stage_three(struct wagon_t *wagons, int numb_wagon, int *target);
void print_stage(int stage, int numb_wagon, struct wagon_t *wagons, int consign_num, int *target, int size);
double sum_wagons (int *target, struct wagon_t *wagons, int wagon_num);
void sort_num (int *target, int target_cnt);

int
main (int argc, char *argv[]) {
    struct wagon_t wagons[MAX_WAGONS];
    int numb_wagon;
    int target[MAX_WAGONS] = {0};

    numb_wagon = stage_one(wagons);
    printf("\n");
    stage_two(wagons, numb_wagon, target);
    printf("\n");
    stage_three(wagons, numb_wagon, target);
}

int
stage_one (struct wagon_t *wagons) {
    char tonnes_str[6], percent_str[7];
    int numb_wagon = 0, i;
    double tonnes, percent;
    double sum_tonnes = 0, concentration = 0;

    // scanf part
    scanf("%s %s", tonnes_str, percent_str);
    while (scanf("%lf %lf", &tonnes, &percent) == 2) {
        wagons[numb_wagon+1].tonnes = tonnes;
        wagons[numb_wagon+1].percent = percent;
        numb_wagon++;
        sum_tonnes += tonnes;
    }

    // calculate concentration
    for (i=1; i<=numb_wagon; i++) {
        concentration += wagons[i].tonnes * wagons[i].percent;
    }
    concentration = concentration / sum_tonnes;

    wagons[0].tonnes = sum_tonnes;
    wagons[0].percent = concentration;

    print_stage(1, numb_wagon, wagons, 0, 0, 0);
    return numb_wagon;
}

void
stage_two(struct wagon_t *wagons, int numb_wagon, int *target) {
    int flag = 1, i, j, consign_num = 0;
    int dump[MAX_WAGONS], dump_cnt=0, target_cnt=0;
    wagons[0].percent = 0;
    wagons[0].tonnes = 0;

    for(i=1; i<=numb_wagon; i++) {
        target[i-flag] = i;
        target_cnt++;
        if( sum_wagons(target, wagons, i-flag) > MIN_TONNES ) {
            if( wagons[0].percent > MIN_CONCEN ) {
                consign_num++;
                print_stage(2, numb_wagon, wagons, consign_num, target, i-flag);
            }
            else {
                for (j=flag; j<i; j++) {
                    dump[dump_cnt] = j;
                    dump_cnt++;
                }
            }

            for(j=0;j<target_cnt;j++) {
                target[i] = 0;
            }
            wagons[0].tonnes = 0;
            wagons[0].percent = 0;
            flag = i;
            i--;
            target_cnt = 0;
        }
        else {
            if ( i == numb_wagon ) {
                for (j=flag; j<=i; j++) {
                    dump[dump_cnt] = j;
                    dump_cnt++;
                }
            }
        }
    }
    for(j=0;j<target_cnt;j++) {
        target[i] = 0;
    }
    wagons[0].tonnes = 0;
    wagons[0].percent = 0;
    sum_wagons(dump, wagons, dump_cnt);
    print_stage(2, numb_wagon, wagons, 0, dump, dump_cnt);
}

void
stage_three(struct wagon_t *wagons, int numb_wagon, int *target) {
    int flag = 1, i, j, consign_num = 0, true_flag = 0;
    int dump[MAX_WAGONS], dump_cnt=0, target_cnt=0;
    int min_wagon;
	double min_percent;
    wagons[0].percent = 0;
    wagons[0].tonnes = 0;

    for(i=1; i<=numb_wagon+1; i++) {
        if(true_flag == 1) {
            i++;
            flag++;
            true_flag = 0;
        }
        target[i-flag] = i;
        target_cnt++;
        printf("%d %lf %lf sum_wagons test\n", i, wagons[0].tonnes, wagons[0].percent);
        if( sum_wagons(target, wagons, i-flag) >= MIN_TONNES ) {
            printf("%lf %lf sum_wagons success\n", wagons[0].tonnes, wagons[0].percent);
            if( wagons[0].percent >= MIN_CONCEN ) {
                consign_num++;
                print_stage(3, numb_wagon, wagons, consign_num, target, i-flag);
				
				for(j=0;j<target_cnt;j++) {
                    target[i] = 0;
            	}
                target_cnt = 0;
                flag = i;
            }
            else {
				min_percent = wagons[target[0]].percent;
                min_wagon = 0;
                for(j=0;j<target_cnt;j++) {
                    printf("%d  ", target[j]);
                }
                printf(" minimum\n%lf min_percent %d\n",min_percent, target_cnt);
                for(j=0;j<target_cnt-1;j++) {
					if (wagons[target[j]].percent < min_percent ) {
						min_percent = wagons[target[j]].percent;
                        min_wagon = j;
					}
				}
                for(j=0;j<target_cnt;j++) {
                    printf("%d  ", target[j]);
                }
                printf(" minimum2\n%lf min_percent %d %d\n",min_percent, target_cnt, min_wagon);
                printf("%lf changed wagons percent\n", wagons[target[min_wagon]].percent);
                dump[dump_cnt] = target[min_wagon];
                printf("%d dump wagon\n", target[min_wagon]);
                dump_cnt++;
                target[min_wagon] = i;
                target_cnt--;
                true_flag = 1;
            }

            wagons[0].tonnes = 0;
            wagons[0].percent = 0;
            i--;
        }
        else {
            if ( i == numb_wagon+1 ) {
                for (j=flag; j<=i-1; j++) {
                    dump[dump_cnt] = j;
                    dump_cnt++;
                    printf("%d dump wagon last\n", j);
                }
            }
        }
    }
	
    for(j=0;j<target_cnt;j++) {
        target[i] = 0;
    }
    wagons[0].tonnes = 0;
    wagons[0].percent = 0;
    sum_wagons(dump, wagons, dump_cnt);
    print_stage(3, numb_wagon, wagons, 0, dump, dump_cnt);
}

void
print_stage(int stage, int numb_wagon, struct wagon_t *wagons, int consign_num, int *target, int target_cnt) {
   	int j;
	
	if (stage == 1) {
		printf("S1,    wagon %4d, tonnes= %5.1lf, %%= %5.1lf\n", 
			1, wagons[1].tonnes, wagons[1].percent);
		printf("S1,    wagon %4d, tonnes= %5.1lf, %%= %5.1lf\n", 
			numb_wagon, wagons[numb_wagon].tonnes, wagons[numb_wagon].percent);
		printf("S1,   whole train, tonnes= %5.1lf, %%= %5.1lf\n", 
			wagons[0].tonnes, wagons[0].percent);
	}
   	else {
		if (consign_num > 0) {
			printf("S%d, consignment %d, tonnes= %.1lf, %%= %.1lf\n", 
					stage, consign_num, wagons[0].tonnes, wagons[0].percent);
		}
		else {
			printf("S%d,  total dumped, tonnes= %.1lf, %%= %.1lf\n", 
					stage, wagons[0].tonnes, wagons[0].percent);
		}
		printf("S%d,                wagons= ", stage);
        sort_num(target, target_cnt);
		for(j=0; j<target_cnt; j++) {
			printf("  %d", target[j]);
		}
		printf("\n");	
	}
}

double 
sum_wagons (int *target, struct wagon_t *wagons, int wagon_num) {
    double sum_ton=0, sum_per=0;
    int i = 0;

    for(i=0; i<wagon_num; i++) {
        printf("%d  ", target[i]);
        sum_ton += wagons[target[i]].tonnes;
        sum_per += wagons[target[i]].tonnes * wagons[target[i]].percent;
    }
    printf("\n");
    sum_per /= sum_ton;

    wagons[0].tonnes = sum_ton;
    wagons[0].percent = sum_per;

    return sum_ton;
}

void
sort_num (int *target, int target_cnt) {
    int i, j, tmp;

    for(i=0;i<target_cnt;i++) {
        for(j=0; j<target_cnt;j++) {
            if (target[i]<target[j]) {
                tmp = target[i];
                target[i] = target[j];
                target[j] = tmp;
            }
        }
    }
}

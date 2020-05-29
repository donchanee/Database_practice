#include <stdio.h>
#include <math.h>

#define MAX_LIGHTING_TOWER 99
#define HEIGHT 8.25
#define PI 3.14159

struct light_n {
       double x;
       double y;
       double lm;
};

void stage2(struct light_n light[], int numb_light, double grid[][100]);
void stage3(struct light_n light[], int numb_light);

int
main(int argc, char *argv[]){
      struct light_n light[MAX_LIGHTING_TOWER];
      int numb_light = 0, total_output = 0;
      char x_str[5], y_str[5], lm_str[5];
      double corner_x, corner_y;
      double x, y, lm;
      double grid[100][100] = {0};

      scanf("%s %s", x_str, y_str);
      scanf("%lf %lf", &light[0].x,&light[0].y);
      scanf("%s %s %s", x_str, y_str, lm_str);
      while (scanf("%lf %lf %lf", &x,&y,&lm)==3) {
          numb_light++;
          light[numb_light].x = x;
          light[numb_light].y = y;
          light[numb_light].lm = lm;
          total_output += lm;
      }
      corner_x = light[0].x;
      corner_y = light[0].y;

      printf("S1, north-east corner at x=%6.1lfm, y=%6.1fm\n", corner_x, corner_y);
      printf("S1, number of lights =  %d\n", numb_light);
      printf("S1, total light output =  %dlm\n", total_output);
      printf("\n");
      stage2(light, numb_light, grid);
      printf("\n");
      stage3(light, numb_light);

      return 0;
}

void stage2(struct light_n light[], int numb_light, double grid[][100]) {
      int i, j, k, inadequate=0;
      double x_grid,  y_grid, percent_inadequate;
      x_grid = light[0].x / 100.0;
      y_grid = light[0].y / 100.0;
      for(k=1; k<numb_light+1; k++) {
        for(i=0; i<100; i++){
          for(j=0; j<100; j++){
            grid[j][i] += light[k].lm / (4 * PI * ((HEIGHT*HEIGHT)+(((i*x_grid+0.5*x_grid) - light[k].x)*((i*x_grid+0.5*x_grid) - light[k].x) + ((j*y_grid+0.5*y_grid) -light[k].y)*((j*y_grid+0.5*y_grid)-light[k].y))));
          }
        }
      }
      for(i=0;i<100; i++){
        for(j=0;j<100;j++){
          if(grid[i][j] < 1.0){
            inadequate++;
          }
        }
      }
      percent_inadequate = inadequate / 100.0;
      printf("S2, 100 x 100 grid, %.2lfm x %.2lfm cells\n", x_grid, y_grid);
      printf("S2, fraction of cell centroids below 1.0lx requirement = %4.1lf%%\n",percent_inadequate);
}

void stage3(struct light_n light[], int numb_light){
      double cell_width, target_height, cell_height;
      double ratio;
      int k, i, j;
      cell_width = light[0].x / 72.0;
      target_height = 1.8 * cell_width;
      ratio = round(light[0].y / target_height);
      cell_height = light[0].y / ratio;
      double grid_s3[200][72] = {0};

      for(k=1; k<numb_light+1; k++) {
        for(i=0; i<72; i++){
          for(j=0; j<ratio; j++){
         grid_s3[j][i] += light[k].lm / (4 * PI * ((HEIGHT*HEIGHT)+(((i*cell_width+0.5*cell_width) - light[k].x)*((i*cell_width+0.5*cell_width) - light[k].x) + ((j*cell_height+0.5*cell_height) -light[k].y)*((j*cell_height+0.5*cell_height)-light[k].y))));
          }
        }
      }
      printf("S3, 72 x %.lf grid, %.2lfm x %.2lfm cells\n", ratio, cell_width, cell_height);

      for(k=ratio-1; k>=0; k--){
      	printf("S3, ");
        for(i=0;i<72;i++){
          if(grid_s3[k][i]<1.0)
            printf("-");
          else if(2.0<grid_s3[k][i] && grid_s3[k][i]<3.0)
            printf("2");
          else if(4.0<grid_s3[k][i] && grid_s3[k][i]<5.0)
            printf("4");
          else if(6.0<grid_s3[k][i] && grid_s3[k][i]<7.0)
            printf("6");
          else if(8.0<grid_s3[k][i] && grid_s3[k][i]<9.0)
            printf("8");
          else if(10.0<=grid_s3[k][i])
            printf("+");
          else
            printf(" ");
        }
        printf("\n");
      }
}

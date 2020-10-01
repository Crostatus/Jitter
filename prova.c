#include <stdio.h>
#include <stdlib.h>
#include "gnuplot_i.h"

void draw_stream();

int main(int argc, char const *argv[]) {
    draw_stream();

    return 0;
}

void draw_stream(){
    printf("Insert ip source and ip destination, Example: 'Ip-10.0.2.15-99.86.47.11\n");
    char file_name[40];
    int i=0, a[10],b[10];
    FILE *f;
    char aux[121], *token;
    double d[40], index[40];
    gnuplot_ctrl * h;
    h = gnuplot_init();

    //scanf("%s", file_name);
    // search this stream into the Main data structure
    //strcat(file_name, ".txt");
    /*f = fopen (file_name,"w");
    if(f == NULL) {
        perror("Error opening file");
        return;
    }*/
    gnuplot_setstyle(h, "lines");
    gnuplot_cmd(h, "set term x11 persist");
    for(i=0; i<10; i++){
        index[i] = i;
        d[i] = i*i;
    }
    //gnuplot_cmd(h, "plot sample [i=0:9] '+' (a[i]):(b[i]) with linespoints");

    /*while(fgets(aux, 120, f) != NULL) {
        token = strtok(aux, " ");
        d[i] = i;

        token = strtok(NULL, " ");
        index[i] = atof(token);
        printf("time: %le(double) or %d(int), jiter: %le(double) or %d(int)\n", d[i], (int)d[i], index[i], (int)index[i]);
        i++;
    }*/



    //strcat(file_name, ": Jitter Communication");
    gnuplot_set_xlabel(h, "# packets");
    gnuplot_set_ylabel(h, "Jitter");
    gnuplot_plot_x(h, d, 9,"Jitter Variation");

    gnuplot_close(h);
}

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define POPTYPE int
#define GENEMIN 0
#define GENEMAX 1

typedef struct population {
    POPTYPE ** organism;
    int genotypeLength;
    int size;
} Population;

POPTYPE randgeneval(){
    return (rand() % (GENEMAX-GENEMIN)) - GENEMIN;
}

Population * createPopulation(int sz, int geneLen){
    Population * out = malloc(sizeof(Population));
    out->organism = malloc(sizeof(* POPTYPE)*sz);
    int i;
    for (i=0;i<sz;i++)
        out->organism[i] = malloc(sizeof(POPTYPE)*geneLen);
    out->size = sz;
    out->genotypeLength = geneLen;
    return out;
}

void destroyPopulation(Population * pop){
    int i;
    for (i=0;i<pop->size;i++)
        free(pop->organism[i]);
    free(pop->organism);
    free(pop);
}

void randomizePopulation(Population * pop){
    int i,j;
    for (i=0;i<pop->size;i++){
        for (j=0; j<pop->genotypeLength;j++){
            pop->organism[i][j] = randgeneval()
        }
    }
}

double * evalobjective(double (*f)(int,* POPTYPE), Population * pop){
    double * out = malloc(sizeof(double)*pop->size);
    int i;
    for (i=0; i<pop->size; i++)
        out[i] = (*f)(pop->genotypeLength,pop->organism[i]);
    return out;
}

void fracionOfTotal(int sz, double * objective){
    double total = 0.0;
    int i;
    for (i=0; i<sz,i++)
        total = total+objective[i];
    for (i=0; i<sz,i++)
        total = objective[i]/total;
}

// Assumes weights sum to 1.0
int weightedRandom(int sz, double * weights){
    int i = 0;
    double r = ((double) rand())/RAND_MAX;
    while (r > weights[i]){
        r = r-weights[i];
        i++;
    }
    return i;
}

Population * reproduction(Population * pop,double * weights){
    Population * newpop = createPopulation(pop->size, pop->genotypeLength);
    int i;
    for (i=0;i<pop->size;i++){
        int n = weightedRandom(pop->size,weights);
        for (j=0;j<pop->genotypeLength;j++){
            newpop->organism[i][j] = pop->organism[n][j];
        }
    }
    destroyPopulation(pop);
    return newpop;
}

Population * crossover(Population * pop, double rate){
    double realrate = 0.5*rate;
    int * userates = malloc(sizeof(double)*pop->size);
    int i,j,k,numdone;
    double d = 1.0/pop->size;
    for (i=0;i<pop->size;i++)
        userates[i] = d;
    numdone = 0;
    while ((pop->size > (numdone+2))&&((((double) rand())/RAND_MAX) < realrate)){
        i = weightedRandom(pop->size, userates);
        userates[i] = 0.0;
        fracionOfTotal(pop->size, userates);
        j = weightedRandom(pop->size, userates);
        userates[j] = 0.0;
        fracionOfTotal(pop->size, userates);
        int copyto = rand() % pop->genotypeLength;
        POPTYPE tmp;
        for (k=0;k<copyto;k++){
            tmp = pop->organism[i][k];
            pop->organism[i][k] = pop->organism[j][k];
            pop->organism[j][k] = tmp;
        }
        numdone = numdone+2;
    }
    
}

void mutation(Population * pop, double rate){
    int i,j;
    for (i=0;i<pop->size;i++){
        for (j=0;j<pop->genotypeLength;j++){
            if ((((double) rand())/RAND_MAX) < rate){
                pop->organism[i][j] = randgeneval();
            }
        }
    }
}

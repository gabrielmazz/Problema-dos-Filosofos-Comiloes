//O sistema é composto por N filósofos sentados em uma mesa circular. 
//Cada filósofo tem um prato (com espaguete) e um garfo do lado direito e outro do lado esquerdo (Veja Figura 1). 
//Os filósofos seguem o ciclo: comer e pensar. 
//Para comer o filósofo precisa de dois garfos (o espaguete é muito liso!). 
//Caso os garfos não estejam disponíveis, o filósofo deverá esperar. 
//Para pensar, o filósofo deve soltar os garfos na mesa novamente.

//O filósofo 0 deve usar o garfo 0 e o garfo N. O filósofo 1 deve usar o garfo 1 e o garfo 0.
//O filósofo N deve usar o garfo N e o garfo N-1.
//Filósofos terão um tempo para comer, e então irão liberar seus garfos, para pensar.
//Cada filósofo vai ter uma quantidade específica de comida no prato, e come X quantidade dessa comida cada vez que pega os garfos.
//Quando termina de comer, ele libera os grafos e não mais os pega, mas espera seus amigos comerem, afinal, são educados.

//Traduzindo: num vetor dos filósofos, o filósofo N deve utilizar o garfo N e o garfo N-1.
//Se for o filósofo na posição 0, ele utilizará o garfo 0 e o garfo M, onde M é o número de filósofos na mesa.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int n;
int *buffer;

sem_t mutex;
sem_t libera;
sem_t entra;

void *thread_function(void* a){
    int pos = *((int *) a);
    pos--;
    int fome = 3;
    int ant;
    int valor;

    if (pos > 0){
        ant = pos - 1;
    }
    else {
        ant = n - 1;
    }
    printf("%d está indo para a mesa\n", pos);
    usleep(100);

    while(fome != 0){
        //entrando na Seção Crítica
        sem_wait(&mutex);
        while ((buffer[pos] == 1) || (buffer[ant] == 1)){
            sem_post(&mutex);
        }

        //sem_getvalue(&mutex, &valor);
        //printf("%d é o valor de mutex no momento\n", valor);

        /*if((buffer[pos] == 1) || (buffer[ant] == 1)){
            sem_post(&mutex);
            while ((buffer[pos] == 1) || (buffer[ant] == 1));
            sem_wait(&mutex);
            printf("%d pegou o mutex dentro\n", pos);
        }*/
        sem_wait(&entra);

        buffer[pos] = 1;
        buffer[ant] = 1;
        sem_post(&mutex);

        printf("   %d vai pegar seu garfo %d e o anterior %d\n", pos, pos, ant);
        //acaba seção critica

        printf("%d comendo...\n", pos);
        usleep(100);
        fome--;
        printf("%d comeu e vai pensar\n", pos);
        printf("   fome de %d agora é %d\n", pos, fome);
        

        printf("   %d largou seu garfo %d e o anterior %d\n", pos, pos, ant);
        sem_wait(&mutex);
        buffer[pos] = 0;
        buffer[ant] = 0;
        sem_post(&mutex);
        sem_post(&entra);

        printf("%d pensando...\n", pos);
        usleep(300);
    }

    printf("\n%d terminou de pensar e vai para o campo das Ideias\n\n", pos);
}

int main(int argc, char const *argv[])
{
    pthread_t *threads;
    
    n = atoi(argv[1]);

    buffer = (int *) malloc(n*sizeof(int));
    for (int i = 0; i < n; i++)
        buffer[i] = 0;

    /*float metadef = n*0.5;
    int metade = (int)(metadef + 0.5);*/
    
    int metade = (int)n*0.5;

    threads = (pthread_t *) malloc(n * sizeof(pthread_t));

    sem_init(&mutex, 0, 1);
    sem_init(&entra, 0, metade);
    sem_init(&libera, 0, 1);

    //criando N filósofos
    for (int i = 0; i < n; i++){
        void* a = &i;
        pthread_create(&(threads[i]), NULL, &thread_function, a);
    }
    
    //esperando os N filósofos
    for (int i = 0; i < n; i++)
        pthread_join(threads[i], NULL);

    sem_destroy(&mutex);
    sem_destroy(&entra);

    printf("\nTODOS OS FILÓSOFOS COMERAM TUDO!!\n");

    free(buffer);

    return 0;
}

# Multithreads - Problema dos Filósofos Comilões

### Trabalho 02 - Sistemas Operacionais
### Professor: Marcion Oyamada

>Colaboradores: *Gabriel Mazzuco* ([Github Profile](https://github.com/gabrielmazz)), *Rodrigo Rocha* ([Github Profile](https://github.com/Rodrigo2603)) e *Gabriel Norato* ([Github Profile](https://github.com/iMaGiNaTrOn))
---

### Objetivo:
- Como objetivo deste trabalho, é desenvolver uma solução para o problema dos filósofos comilões, e também uma solução que possibilite um deadlock

### Problema:
- O problema dos filosofos comilões tem como característica uma mesa circular aonde N filosofos terão um prato com espaguete, além de um garfo do seu lado esquerdo e outro do seu lado direito
- Esses filósofos terão um ciclo de ações, sendo eles comer e pensar
- Para realizar a ação de comer, cada filósofo precisa ter os dois garfos em suas mãos, caso contrário ele estará no estado de espera
- Após comer, os filósofos devem soltar os seus garfos na mesa e assim poderão começar a pensar
- Deve-se levar em conta que cada filósofo tera uma quantidade específica de comida no prato e ao realizar o ato de comer, será retirada uma unidade de comida do prato
- ![](filosofos.jpg)

### Código

- Para a criação dos filósofos estamos utilizando a biblioteca **<pthread.h>**, junto com um for que determina quantas threads serão criadas

	```c
	for (int i = 0; i < n; i++){
		void* a = &i;
		pthread_create(&(threads[i]), NULL, &thread_function, a);
	}
	```

- Para sincronização de todos os filósofos, é utilizado semáfaros *mutex*, mais especificamente três deles, sendo o próprio **mutex, libera e entra**. Para isso é utilizado a biblioteca **<semaphore.h>**

	```c
	sem_t mutex;
	sem_t libera;
	sem_t entra;
	```

- A função thread ficará responsável pelas ações de comer e pensar dos filósofos, passando e printando a cada passo que é realizado no código

	```c
	void *thread_function(void* a){
	    int pos = *((int *) a);
	    pos--;
	    int fome = 3;
	    int ant;
	    int valor;
	
	    if (pos > 0)
	        ant = pos - 1;
	    else
	        ant = n - 1;
	        
	    printf("%d está indo para a mesa\n", pos);
	    usleep(100);
	
	    while(fome != 0){
	        //entrando na Seção Crítica
	        sem_wait(&mutex);
	        while ((buffer[pos] == 1) || (buffer[ant] == 1)){
	            sem_post(&mutex);
	        }
	        
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
	```

- Depois de todo o código está executando, aqui vemos que ocorrerá uma espera para que todos os filósofos terminem de comer e pensar, assim o programa poderá *finalizar com sucesso*

	```c
	for (int i = 0; i < n; i++)
	    pthread_join(threads[i], NULL);
	```

- Para determinarmos quantas threas serão criadas, é utilizado o *argv[] do próprio C*, passando então pelo terminal na hora da execução

	```c
	int main(int argc, char const *argv[])
	...
	n = atoi(argv[1]);
	```

- Para determinar o número de filósofos

	```bash
	./main <filosofos/threads>
	```

### Compilação/Execução

- Para executar compilar o código e posteriormente executar, devemos fazer os seguintes passos
	- Lembrando que para executar, precisamos estár no **Linux** ou se preferir, utilizar o **WSL2** no windows


- Para compilar o código, devemos colocar a tag -pthread, para podermos utilizar de forma correta a biblioteca e em si mesmo, o código

	```bash
	gcc filosofos.c -pthread -o main
	./main <filosofos/threads>
	```



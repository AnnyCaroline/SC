# Memória compartilhada, variáveis compartilhadas

Este programa usa como base o programa da etapa 1. As seções relativas a esta etapa (etapa 2) e a anterir estão sinalizadas por meio de comentários.

- O processo pai:
    - Cria uma área de memória compartilhada(*shmem*) suficiente para conter duas variáveis inteiras (*int*);
    > Para armazenar as duas variáveis foi utilizado um array de duas posições.
    - Inicializada a primeira variável compartilhada com 0 e a segunda com 300;
    - Antes de terminar, exibe o valor final das duas variáveis compartilhadas e libera todos os recursos;

- Cada processo:
    - Exibe, junto às demais informações, o valor das variáveis compartilhadas;
    - Executa em sua rotina (lembrando, que, segundo a etapa 1, cada rotina é executada 100 vezes por processo):
        - Lê a primeira variável compartilhada;
        - Copia para uma variável local;
        - Dorme um tempo;
        - Copia o valor da variável local para a primeira variável compartilhada;
        - > Apesar de não estar no enunciado, acrescentei mais uma "dormida"
        - Decrementa a variável compartilhada (diretamente na memória compartilhada);
        - Dorme mais um tempo;

        > Ao executar uma rotina, isto é, ao executar uma iteração do loop de 100 paços, um número entre 5 e 100 é sorteado (isso foi tratado na etapa 1). Esse número foi utilizado nessa etapa como o tempo, em ms, para as dormidas citadas acima.

## Compilação
Make


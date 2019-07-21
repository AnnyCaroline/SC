# Projeto 2 - Implementação

## Compilação (para todas as etapas)

```
 $ make
```

## Etapa 1 - Criação de processos, fork/wait, geração de números aleatórios, tempo, dormir

- O processo pai:

  - Cria 3 processos-filho através da _system call_ `fork()`. Sugestão: armazenar o pid de cada processo em uma posição de um _array_ de _int_ no pai;
  - Em seguida, aguarda o término de cada um dos filhos (`wait()`);
  - A cada filho que termina o processo pai, imprime na tela uma mensagem do tipo, "Filho X, pid Y, terminou";
  - Depois que os três filhos terminarem o pai imprime na tela, "Todos os filhos terminaram. Processamento teve duração de Z ms";
  - No caso dos processos criados através de `fork()`, vamos utilizar sinais (`signal`) como mecanismo do Unix de comunicação entre processos filhos com o pai. Ou seja, o processo filho vai enviar um `signal` para o processo pai - o processo pai já deve estar "aguardando" (bloqueado) este evento através de um `wait()`, como já definido anteriormente.

- Cada processo:
  - Executa a sua rotina 100 vezes;
  - Após cada paço da rotina o filho deve exibir uma mensagem na tela, com as informações de sue número (0, 1 ou 2), pid, paço onde se encontra (e outras informações que se entenda por relevantes); e aguarda um tempo aleatório;
  - A cada "dormida" (variando entre 5 e 100ms) o tempo esperado aleatório em milissegundos também deve ser apresentado na tela;
  - Cada processo deve cronometrar/calcular o tempo gasto médio de **execução** da rotina, e o desvio padrão. Exibir este vlaor na tela antes de terminar;
  - Quando a rotina terminar, cada processo "avisa" ao processo "pai" que já terminou. **O pai deve avisar que está ciente de que o filho X terminou**.

**Lembre-se:** os processos devem ser programados de forma que se **imponha o "intercalamento" aleatório dos mesmos** (caso contrário eles podem executar em sequência). Ou seja, o "dormir" de cada processo deve ser aleatório (usar o _random_ e _srand_ corretamente).

## Etapa 2 - Memória compartilhada, variáveis compartilhadas

Este programa usa como base o programa da etapa 1. As seções relativas a esta etapa (etapa 2) e a anterir estão sinalizadas por meio de comentários.

- O processo pai:

  - Cria uma área de memória compartilhada(_shmem_) suficiente para conter duas variáveis inteiras (_int_);
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

## Etapa 3 - # Semáforos

Na etapa 2 foi identificado um problema de inconsistência. Ao final da execução, a primeira variável compartilhada é diferente de 300 (o resultado esperado). Além disso, as variáveis compartilhadas exibidas dentro da rotina dos processos filhos mostram valores também incosistentes.

Duas soluções utilizandos semáforos _mutex_ foram propostas. Em ambas os semáforos são inicializados no processo pai, antes da criação dos processos filhos.

A primeira utiliza um único semáforo mutex para envolver toda a rotina dos processos filhos, enquanto que a segunda utiliza dois semáforos, um para envolver a rotina de incremento da primeira variável, e o segundo para a rotina de decremento da segunda variável. Como esperado, a segunda solução, por permitir um maior grau de concorrência, possui um menor tempo de execução.

|                    | consistente | média   | tempo 1  | tempo 2 | tempo 3 |
| ------------------ | ----------- | ------- | -------- | ------- | ------- |
| etapa 2            | não         | 16,890s | 16.464s  | 17.585s | 16.622s |
| etapa 3 - 1 mutex  | sim         | 49,817s | 49.824s  | 49.590s | 50.036s |
| etapa 3 - 2 mutexs | sim         | 19,456s | 19.561 s | 19.085s | 19.722s |

## Etapa 4 - Troca de mensagens

Nesta etapa, o mecanismos de _wait/signal_ do Unix usados para garantir a sincronização entre processos filhos e pai foi substituído pela troca de mensagens.

Agora, cada processo filho, ao final de sua atividade, envia uma mensagem para o pai através de uma fila de mensagens. O pai, ao invés de aguardar 3 signals, um de cada filho, aguarda ler 3 mensagens da fila.

### Links

- https://gist.github.com/Mark-htmlgogogo/e024c36541646373581472348657304d

## Etapa 5 - Produzindo _starvation_ alterando a prioridade dos processos

Nesta etapa a chamada _setpriority_ foi utilizada para alterar a prioridade dos processos filhos a fim de "forçar" uma situação de _starvation_.

O processo 0 foi mantido com a prioridade 0, enquanto os processos 1 e 2 receberam a prioridade mínima de 19.

|                    | consistente | média   | tempo 1  | tempo 2 | tempo 3 |
| ------------------ | ----------- | ------- | -------- | ------- | ------- |
| etapa 2            | não         | 16,890s | 16.464s  | 17.585s | 16.622s |
| etapa 3 - 1 mutex  | sim         | 49,817s | 49.824s  | 49.590s | 50.036s |
| etapa 3 - 2 mutex  | sim         | 19,456s | 19.561 s | 19.085s | 19.722s |
| etapa 5 - 2 mutexs | sim         | 19,546s | 20.277 s | 19.669s | 18.692s |

## Etapa 6 - Resolvendo o problema do starvation

> Verifique que tipo de semáforo é provido no Linux (FIFO ou não). Os semáforos usado até o momento deveriam resolver este problema?

Segundo a [documentação](http://pubs.opengroup.org/onlinepubs/007908799/xsh/sem_post.html) e uma pergunta do [StackOverflow](https://stackoverflow.com/questions/16163932/multiple-threads-can-wait-on-a-semaphore-at-same-time), se `_POSIX_PRIORITY_SCHEDULING` estiver definido, a tarefa a ser desbloqueada no semáforo será escolhida segundo as políticas de escalonamento do sistema. Para implementar um semáforo FIFO, portanto, seria necessário definir que a aplicação utilizará um escalonador `SCHED_FIFO`, por meio da função `sched_setscheduler` da biblioteca `sched.h` ([documentação](http://man7.org/linux/man-pages/man2/sched_setscheduler.2.html)). Entretanto, essa chamada exige permissões de super usuário ([stackoverflow](https://stackoverflow.com/questions/27694250/why-does-sched-setscheduler-require-root-priveledges)).

A solução proposta neste trabalho é limitar o número de execuções de cada processo para um inteiro `EXEC`, por exemplo, igual a 3. Para isso, um contador para cada processo foi adicionado na memória compartilhada. Todos foram inicializados com o valor de `EXEC`, e a cada execução de um processo o contador correspondente é decrementado. Quando todos atingem o valor 0, são reiniciados, isto é, recebem novamente o valor `EXEC`. Essa solução é semelhante a proposta para o _starvation_ do problema dos filósofos ([link](https://github.com/AnnyCaroline/SC/tree/master/lista2)).

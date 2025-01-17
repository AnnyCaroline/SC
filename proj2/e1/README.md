# Criação de processos, fork/wait, geração de números aleatórios, tempo, dormir

- O processo pai:
    - Cria 3 processos-filho através da *system call* ```fork()```. Sugestão: armazenar o pid de cada processo em uma posição de um *array* de *int* no pai;
    - Em seguida, aguarda o término de cada um dos filhos (```wait()```);
    - A cada filho que termina o processo pai, imprime na tela uma mensagem do tipo, "Filho X, pid Y, terminou";
    - Depois que os três filhos terminarem o pai imprime na tela, "Todos os filhos terminaram. Processamento teve duração de Z ms";
    - No caso dos processos criados através de ```fork()```, vamos utilizar sinais (```signal```) como mecanismo do Unix de comunicação entre processos filhos com o pai. Ou seja, o processo filho vai enviar um ```signal``` para o processo pai - o processo pai já deve estar "aguardando" (bloqueado) este evento através de um ```wait()```, como já definido anteriormente.

- Cada processo: 
    - Executa a sua rotina 100 vezes;
    - Após cada paço da rotina o filho deve exibir uma mensagem na tela, com as informações de sue número (0, 1 ou 2), pid, paço onde se encontra (e outras informações que se entenda por relevantes); e aguarda um tempo aleatório;
    - A cada "dormida" (variando entre 5 e 100ms) o tempo esperado aleatório em milissegundos também deve ser apresentado na tela;
    - Cada processo deve cronometrar/calcular o tempo gasto médio de **execução** da rotina, e o desvio padrão. Exibir este vlaor na tela antes de terminar;
    - Quando a rotina terminar, cada processo "avisa" ao processo "pai" que já terminou. **O pai deve avisar que está ciente de que o filho X terminou**.

**Lembre-se:** os processos devem ser programados de forma que se **imponha o "intercalamento" aleatório dos mesmos (caso contrário eles podem executar em sequência). Ou seja, o "dormir" de cada processo deve ser aleatório (sar o *random* e *srand* corretamente).

## Compilação
```
 $ make
```



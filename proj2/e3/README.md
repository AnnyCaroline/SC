# Semáforos
Na etapa 2 foi identificado um problema de inconsistência. Ao final da execução, a primeira variável compartilhada é diferente de 300 (o resultado esperado). Além disso, as variáveis compartilhadas exibidas dentro da rotina dos processos filhos mostram valores também incosistentes.

Duas soluções utilizandos semáforos *mutex* foram propostas. Em ambas os semáforos são inicializados no processo pai, antes da criação dos processos filhos.

A primeira utiliza um único semáforo mutex para envolver toda a rotina dos processos filhos, enquanto que a segunda utiliza dois semáforos, um para envolver a rotina de incremento da primeira variável, e o segundo para a rotina de decremento da segunda variável. Como esperado, a segunda solução, por permitir um maior grau de concorrência, possui um menor tempo de execução.

|                   | consistente | média   | tempo 1  | tempo 2 | tempo 3 |
|-------------------|-------------|---------|----------|---------|---------|
| etapa 2           | não         | 16,890s | 16.464s  | 17.585s | 16.622s |
| etapa 3 - 1 mutex | sim         | 49,817s | 49.824s  | 49.590s | 50.036s |
| etapa 3 - 2 mutex | sim         | 19,456s | 19.561 s | 19.085s | 19.722s |

## Compilação
```
 $ make
```


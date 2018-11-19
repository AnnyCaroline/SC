# Troca de mensagens

Nesta etapa, o mecanismos de *wait/signal* do Unix usados para garantir a sincronização entre processos filhos e pai foi substituído pela troca de mensagens.

Agora, cada processo filho, ao final de sua atividade, envia uma mensagem para o pai através de uma fila de mensagens. O pai, ao invés de aguardar 3 signals, um de cada filho, aguarda ler 3 mensagens da fila.

## Compilação
```
 $ make
```

## Links
- https://gist.github.com/Mark-htmlgogogo/e024c36541646373581472348657304d
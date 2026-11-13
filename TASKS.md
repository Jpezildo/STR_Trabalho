# Tarefas FreeRTOS

Os valores seguem a convenção do FreeRTOS: número maior significa prioridade maior. A tarefa T1 não realiza I/O de rede; ela registra a decisão e mantém a regra de negócio pendente, evitando bloqueio pelo cliente HTTP.

| Tarefa | Prioridade | Ativação | Responsabilidade | Comunicação |
|---|---:|---|---|---|
| T1 — Gatilho (`task_gatilho`) | 3 | Bloqueia esperando mensagem em `Q_PRICE`. | Validar preço e identificar cruzamento ascendente/descendente e rearme. Não submete ordem enquanto o mapeamento não for aprovado. | Consome `Q_PRICE`; envia eventos a `Q_LOG`. |
| T2 — Consulta (`task_consulta`) | 2 | Periódica, 5000 ms, por `vTaskDelayUntil`. | Usar sequência simulada sem Wi-Fi configurado ou consultar ticker Testnet via HTTPS quando conectado; validar e publicar preço. | Publica em `Q_PRICE`; registra em `Q_LOG`. |
| T3 — Registro (`task_registro`) | 1 | Bloqueia na fila de logs com timeout para relatar perdas. | Consumir e imprimir logs; informar descartes e pico de ocupação. | Consome `Q_LOG`. |

## Recursos compartilhados

- `Q_PRICE`: fila de comprimento 1; preserva o dado mais recente e pode sobrescrever amostras não consumidas. O número sequencial permite detectar lacunas em T1.
- `Q_LOG`: fila de 16 mensagens; produtores usam envio sem espera. Descartes e pico de ocupação são contados.
- Event Group: registra conexão Wi-Fi; sincronização de horário não é usada enquanto a ordem assinada estiver desabilitada.
- Não há `M_NET`: cada consulta cria seu próprio cliente HTTP e T1 não faz acesso de rede. Não existe cliente HTTP global compartilhado para proteger.

## Limites temporais

`published_at_us` usa relógio monotônico para estimar idade/atraso de fila. Não é a métrica RT01. Para medir RT01 será necessário decidir uma ação de ordem, instrumentar o instante observável de início do envio e apresentar testes com placa e condições definidos. A latência HTTP deve permanecer separada.

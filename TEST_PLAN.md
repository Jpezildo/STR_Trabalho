# Plano de testes — PC3

Este documento mistura casos planejados e um teste host implementado. Não é relatório de execução: resultados só podem ser registrados após rodar os comandos e coletar evidências.

## Teste automatizado disponível

`test/native/test_trigger.cpp` valida primeira amostra, cruzamento para cima/baixo, ausência de cruzamento sem transição, preços inválidos, rearme e prevenção de disparo repetido. Ele compila apenas a máquina de estados, sem hardware ou ESP-IDF. Executado com `g++ -std=c++17 -Wall -Wextra -Werror`; compilação e asserções passaram.

## Casos planejados

| ID | Cenário | Esperado | Estado |
|---|---|---|---|
| UT01 | Executar teste host de `TriggerStateMachine`. | Asserções passam. | Passou em host; não cobre ESP-IDF. |
| TF01 | Resposta válida do ticker ou mock. | Preço finito e positivo publicado com sequência. | Pendente de build/integr. |
| TF02 | HTTP não-200, JSON inválido, preço ausente/inválido. | Erro explícito; não publicar preço inválido. | Implementação parcial; testes pendentes. |
| TC01 | T2 publica preço e T1 consome. | Sequência e valor coerentes; lacunas observáveis. | Pendente em ESP-IDF/placa. |
| TC02 | Fila de logs cheia. | Produtor não bloqueia; descarte contado e reportado por T3. | Implementado em código; teste pendente. |
| TC03 | Wi-Fi desconecta e reconecta. | Bit de conectividade atualizado e reconexão solicitada. | Implementado em código; placa pendente. |
| TLS01 | Certificado inválido/CA não confiável. | HTTPS falha; validação TLS não é desativada. | Pendente em ambiente ESP-IDF. |
| INT01 | Preço simulado → Q_PRICE → T1 → Q_LOG → T3. | Cruzamento detectado e log assíncrono; sem ordem real. | Pendente em placa; não há saída presumida. |

## Experimento controlado de carga PC3

`test/native/load_experiment.cpp` foi compilado e executado em host. Para o modelo virtual de 60 s, o resultado observado foi:

| Condição | Produzidos | Consumidos | Sobrescritos | Logs descartados | Pico fila de log | Pior atraso virtual |
|---|---:|---:|---:|---:|---:|---:|
| 5000 ms | 12 | 12 | 0 | 0 | 1/16 | 0 ms |
| 500 ms | 120 | 80 | 39 | 4 | 16/16 | 250 ms |

O modelo fixa serviço da tarefa em 750 ms, serviço de log em 1000 ms e fila de log de 16. Esses valores são resultados do modelo de simulação, não medições do ESP32; o PC3 ainda requer execução controlada em hardware, sem usar API externa como gerador de carga.

## Medição temporal

Registrar, quando houver uma ordem de teste aprovada:

- `t_detect`: instante monotônico em que T1 reconhece o gatilho;
- instante de aquisição de recurso, caso exista;
- início observável da transmissão HTTP;
- resposta HTTP e estado remoto em campos separados.

A métrica preliminar é `t_send_begin - t_detect`. O atual `published_at_us` permite estimar atraso de fila, mas não mede RT01. Não afirmar requisito atendido sem medição reproduzível.

## Registro

Para cada execução, anotar versão do firmware, ferramenta/framework, placa, configuração, duração, passos, esperado, observado, logs/CSV e conclusão (aprovado/reprovado/inconclusivo). Não preencher resultados com estimativas.

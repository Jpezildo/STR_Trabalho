# Plano de testes — PC3

Este arquivo é um plano proposto, não um relatório de resultados. Nenhum teste abaixo deve ser marcado como aprovado sem execução e registro de evidências.

## 1. Princípios

O PC3 pede implementação e integração com ESP-IDF e FreeRTOS, além de evidências de concorrência, comunicação, sincronização, funcionamento sob carga e integração. O relatório deve apresentar condição de execução, comportamento esperado, resultado observado e evidência. Logs isolados mostrando que uma tarefa executou não bastam.

## 2. Verificação funcional

| ID do teste | Requisitos | Condição | Resultado esperado | Evidência sugerida |
|---|---|---|---|---|
| TF01 — Consulta | RF01, RF02 | Wi-Fi disponível; endpoint de ticker acessível. | O sistema obtém resposta válida dentro da rotina de consulta e registra a cotação. | Log com início/fim da consulta, status HTTP e preço parseado. |
| TF02 — Parsing inválido | RF03, RF08 | Resposta JSON malformada, campo ausente ou valor não numérico via mock. | O sistema rejeita a resposta inválida sem publicar preço incorreto. | Log de erro e contagem de respostas rejeitadas. |
| TF03 — Não disparo | RF05 | Cotação que não satisfaz a regra de gatilho. | Nenhuma ordem é solicitada. | Entrada usada, estado do gatilho e ausência de tentativa de ordem. |
| TF04 — Disparo | RF05, RF06 | Cotação que satisfaz a regra acordada. | T1 identifica o evento e inicia uma solicitação conforme a regra. | Registro de preço anterior/atual, regra avaliada e identificador da tentativa. |
| TF05 — Rearme/duplicidade | RF07 | Sequência de preços que dispara, permanece na região de disparo e depois retorna à condição de rearme. | O sistema segue exatamente a política de rearme definida e não envia duplicatas indevidas. | Sequência completa de entradas e contagem de tentativas. |
| TF06 — Resposta da ordem | RF06, RF09 | Resposta simulada ou endpoint de teste da corretora. | O sistema distingue aceitação, rejeição, falha de autenticação e timeout. | Status HTTP, corpo sanitizado e estado interno registrado. |
| TF07 — Logging | RF08 | Produção de eventos por T1 e T2. | T3 consome e imprime mensagens sem bloquear as tarefas produtoras. | IDs de sequência, contadores de mensagens produzidas/consumidas/perdidas. |

## 3. Comunicação e concorrência

| ID | Cenário | O que observar | Evidência |
|---|---|---|---|
| TC01 | T2 produz cotações; T1 aguarda e consome. | Correspondência entre sequência/valor publicado e recebido; efeito da fila de tamanho 1. | Contadores ou logs estruturados de produtor e consumidor. |
| TC02 | T1 e T2 solicitam uso do recurso de rede. | Exclusão mútua, tempo de espera e duração da seção protegida. | Timestamps de entrada/saída do mutex e de início/fim HTTP. |
| TC03 | T1 produz evento enquanto T3 está imprimindo. | Se a tarefa crítica consegue enfileirar o log sem aguardar pela Serial. | Tempo de execução de `enqueue`, ocupação da fila e mensagens descartadas. |
| TC04 | Wi-Fi/NTP indisponíveis ou recuperados. | Bloqueio, desbloqueio e transição dos bits de estado. | Logs de estado e sequência de transições. |

## 4. Experimento obrigatório sob carga — pelo menos duas condições

O PC3 solicita pelo menos um experimento controlado com duas ou mais condições e ao menos um indicador observável, por exemplo quantidade de eventos, ocupação de fila, bloqueios ou perdas.

### Experimento sugerido

**Objetivo:** observar se o aumento da frequência de cotações ou do processamento concorrente afeta a comunicação entre T2, T1 e T3.

| Condição | Parâmetro | Configuração exemplo | Métricas |
|---|---|---|---|
| Carga baixa | Frequência de produção de cotação/evento | Simulação a cada 5 s | Eventos produzidos/consumidos, fila, atraso de T1. |
| Carga alta | Frequência de produção de cotação/evento | Simulação a cada 500 ms ou carga adicional controlada | Mesmas métricas da condição baixa. |

Os valores são sugestões para um teste local controlado, não parâmetros definitivos do sistema nem uma instrução para ultrapassar limites da API externa. Preferencialmente, use uma fonte simulada/mock para variar a frequência sem sobrecarregar a Binance Testnet.

Registre constantes entre condições: placa, firmware, prioridade das tarefas, duração do experimento, tamanho das filas, configuração de rede e valor de limiar.

### Tabela de resultados

| Condição | Eventos gerados | Eventos consumidos por T1 | Eventos sobrescritos/perdidos | Pico da fila de logs | Atraso médio T1 | Pior atraso T1 |
|---|---:|---:|---:|---:|---:|---:|
| Baixa | preencher | preencher | preencher | preencher | preencher | preencher |
| Alta | preencher | preencher | preencher | preencher | preencher | preencher |

Não preencha com valores estimados. Use somente dados medidos.

## 5. Teste integrado

Executar um cenário de ponta a ponta em modo de teste:

1. iniciar o sistema;
2. estabelecer conectividade e estado necessário;
3. fornecer uma sequência de cotações controlada;
4. fazer a sequência satisfazer uma regra de gatilho previamente definida;
5. observar T1 receber e avaliar a cotação;
6. observar a solicitação simulada/de teste ser gerada;
7. observar o resultado ser registrado por T3.

A evidência deve mostrar a ligação entre a entrada inicial, o evento de gatilho, a tentativa de ordem e o resultado. Não usar conta com fundos reais.

## 6. Medições para PC4

Embora o PC3 foque testes funcionais, instrumente o código para permitir análise temporal posterior. Registre ao menos:

- instante de detecção do gatilho;
- instante de aquisição do mutex/recurso de rede, se aplicável;
- instante de início do envio HTTP;
- instante de término da chamada HTTP;
- resultado/estado da tentativa.

Métrica de referência de RT01: `t_send_begin - t_detect`. Mantenha separada a latência de ida e volta HTTP. No PC4, compare medições reais ao requisito temporal e analise variação/jitter conforme o roteiro da disciplina.

## 7. Uso da Binance Testnet

A documentação oficial informa que `POST /api/v3/order` é endpoint de negociação assinado, com parâmetros `timestamp` e `signature`. Para testes de validação de ordem, existe `POST /api/v3/order/test`, que valida a criação mas não envia a ordem ao mecanismo de matching. O endpoint de teste ainda exige assinatura/autenticação. Consulte: https://developers.binance.me/en/docs/products/spot/testnet/rest-api

Se a integração assinada ainda não estiver pronta, use um mock local para demonstrar o fluxo do sistema. Não declare o teste integrado da corretora concluído com base apenas em um POST não autenticado.

## 8. Registro dos resultados

Para cada teste, registrar:

- ID e requisito(s) relacionados;
- data e versão/commit local do firmware testado;
- placa, framework e configuração de build;
- setup e parâmetros;
- passos executados;
- resultado esperado;
- resultado observado;
- evidência (log, captura, CSV ou gráfico);
- conclusão: aprovado, reprovado ou inconclusivo;
- limitações encontradas.

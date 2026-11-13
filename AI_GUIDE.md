# Guia para analisar o projeto com uma IA

Este guia pode ser enviado junto ao código para que Gemini, ChatGPT, Codex ou outra ferramenta entenda o contexto sem assumir que a implementação está completa.

## 1. Arquivos que devem acompanhar o pedido

Para uma leitura geral:

- `README.md`
- `ARCHITECTURE.md`
- `AGENTS.md`
- `docs/PROJECT_SPEC.md`
- `docs/REQUIREMENTS.md`
- `docs/TASKS.md`
- `docs/CURRENT_STATUS.md`
- `docs/DECISIONS_PENDING.md`
- `platformio.ini`
- `src/main.cpp`
- arquivos de cabeçalho necessários, sem segredos

Para investigar uma função específica, enviar também somente os módulos relacionados e seus headers.

**Nunca enviar:** arquivo com senha Wi-Fi, API Key, Secret Key, token ou outro segredo. Substitua valores sensíveis por marcadores como `REDACTED`.

## 2. Prompt de contexto

Copie e adapte este prompt:

```text
You are reviewing an academic real-time embedded systems project named "Gatilho de Negociação de Ativos".

The project targets an ESP32. Its intended architecture has three FreeRTOS tasks:
- T1: event-driven price trigger and order-request initiation (highest relative priority).
- T2: periodic quote polling from an exchange API (initial PC1 period: 5 seconds; medium relative priority).
- T3: best-effort serial logging (lowest relative priority).

The initial PC1 proposal defines a preliminary target below 500 ms from trigger detection to the beginning of network transmission. This is NOT a guarantee that the exchange server will respond or execute the order within 500 ms. Keep local dispatch time separate from HTTP round-trip time and remote order status.

Important discrepancies found in the repository snapshot:
1. The current PlatformIO configuration selects the Arduino framework, while the PC3 assignment explicitly asks for ESP-IDF and FreeRTOS. Do not assume this is already compliant.
2. The current source polls every 20 ms, while PC1 specifies an initial 5-second period.
3. The current trigger code sends BUY when price is above the threshold; the proposal describes both upward/downward crossings and buy/sell behavior.
4. The current order POST does not include the signed-request timestamp and signature required by Binance private trading endpoints.
5. The current trigger task may wait up to 2 seconds for the network mutex, which conflicts with the preliminary 500 ms target.
6. Credentials were found in a tracked configuration file. Do not reproduce, request, or use any credentials. Treat exposed credentials as compromised and recommend rotation.

Read README.md, ARCHITECTURE.md, AGENTS.md and the docs files before drawing conclusions. Distinguish:
- facts observed in source code;
- requirements stated in the PC1 proposal;
- proposed architecture;
- unresolved decisions.

Do not silently resolve the decisions listed in docs/DECISIONS_PENDING.md. Do not invent test results, measurements, files, or successful builds. Do not claim a trading order was accepted merely because an HTTP request returned a positive status code.

Unless I explicitly authorize it, do not modify files, GitHub branches, commits, pull requests, issues, or remote repository settings. Start by explaining the current code flow and listing discrepancies. Then propose the smallest safe next step without implementing it.
```

## 3. Como pedir uma explicação do código

Exemplo de primeiro pedido:

> Leia a documentação e os arquivos de código enviados. Explique o fluxo completo desde a inicialização do ESP32 até a publicação da cotação na fila, o recebimento por T1 e o envio do log por T3. Ainda não altere nenhum arquivo. Separe o comportamento real do código dos requisitos descritos na documentação.

## 4. Como pedir revisão técnica

> Analise se a arquitetura atual consegue atender RT01 (< 500 ms entre detecção do gatilho e início do envio). Considere espera pelo mutex, chamadas HTTP bloqueantes, prioridades e sincronização. Não faça alterações. Aponte evidências no código e proponha testes para confirmar cada hipótese.

## 5. Como pedir revisão do PC3

> Compare a implementação com os itens do roteiro PC3: ESP-IDF/FreeRTOS, organização da aplicação, implementação das tarefas, comunicação/sincronização, temporização, testes funcionais, concorrência, pelo menos duas condições de carga, teste integrado e limitações. Gere uma matriz de conformidade com status "atendido", "parcial", "não atendido" ou "não verificado", sempre citando o arquivo/trecho que sustenta a classificação. Não invente evidências.

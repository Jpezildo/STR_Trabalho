# Guia de análise assistida por IA

Leia [README.md](README.md), [PROJECT_SPEC.md](PROJECT_SPEC.md), [REQUIREMENTS.md](REQUIREMENTS.md), [ARCHITECTURE.md](ARCHITECTURE.md), [TASKS.md](TASKS.md), [CURRENT_STATUS.md](CURRENT_STATUS.md) e [DECISIONS_PENDING.md](DECISIONS_PENDING.md) antes de analisar o firmware.

## Contexto sanitizado

O projeto é uma aplicação acadêmica ESP32 em ESP-IDF/FreeRTOS. T1 processa cruzamentos, T2 consulta periodicamente (5 s), T3 consome logs. Quando não há credenciais locais, T2 usa valores simulados. A estratégia BUY/SELL permanece pendente e nenhuma ordem é submetida. A API privada de ordem ainda não está implementada.

## Segurança e evidências

- Nunca envie `include/secrets.h`, chaves, senhas, tokens ou logs sensíveis a uma IA.
- Não use Mainnet e não afirme execução de ordem com base em status HTTP.
- Distinga código implementado, requisito acadêmico, decisão pendente e validação executada.
- Não declare build, teste, medição ou validação de hardware sem evidência.

## Prompt de referência

> Analise o projeto local ESP32 com ESP-IDF/FreeRTOS. Leia a documentação de contexto antes do código. Não escolha regra BUY/SELL nem parâmetros de ordem. Identifique evidências concretas, explique limitações de build e concorrência, e não declare testes aprovados sem execução. Não solicite nem reproduza credenciais. Use apenas Binance Spot Testnet ou mocks; nunca Mainnet.

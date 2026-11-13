# Gatilho de Negociação de Ativos — ESP32 e FreeRTOS

Documentação de apoio ao projeto acadêmico **Gatilho de Negociação de Ativos**, desenvolvido na disciplina de Sistemas de Tempo Real.

> **Importante:** esta pasta contém documentação e orientações para revisão local. Ela não altera o repositório GitHub e não inclui uma implementação de firmware pronta para substituir o código atual.

## Estado desta documentação

O conteúdo foi preparado a partir de três fontes:

1. proposta inicial do grupo (PC1), enviada em texto;
2. roteiros acadêmicos `STR_PC2.pdf` e `STR_PC3.pdf`;
3. leitura dos arquivos `platformio.ini`, `src/main.cpp` e `include/config.h` da branch `Develop` do repositório `Jpezildo/STR_Trabalho` em 13/11/2026.

A documentação distingue explicitamente:

- **implementação observada:** o que está presente no código consultado;
- **proposta do grupo:** requisitos e decisões registradas no documento do PC1;
- **proposta arquitetural:** organização recomendada para discussão e implementação;
- **pendência:** informação que precisa ser decidida ou validada pela equipe/professor.

## Documentos

| Arquivo | Finalidade |
|---|---|
| [`ARCHITECTURE.md`](ARCHITECTURE.md) | Arquitetura atual observada e arquitetura conceitual pretendida. |
| [`AGENTS.md`](AGENTS.md) | Regras para uso de Gemini, Codex e outras IAs no projeto. |
| [`APLICACAO_LOCAL.md`](APLICACAO_LOCAL.md) | Como revisar e aplicar a documentação localmente, sem alterar o GitHub. |
| [`PLANO_ACAO.md`](PLANO_ACAO.md) | Plano de ação para consolidação do projeto, com decisões, prioridades e critérios de conclusão. |
| [`docs/PROJECT_SPEC.md`](docs/PROJECT_SPEC.md) | Síntese fiel da proposta apresentada pelo grupo. |
| [`docs/REQUIREMENTS.md`](docs/REQUIREMENTS.md) | Requisitos funcionais e temporais organizados para PC2/PC3. |
| [`docs/TASKS.md`](docs/TASKS.md) | Responsabilidades, ativação, prioridades e comunicação das tarefas. |
| [`docs/TEST_PLAN.md`](docs/TEST_PLAN.md) | Plano de testes e evidências exigidas no PC3. |
| [`docs/CURRENT_STATUS.md`](docs/CURRENT_STATUS.md) | Diagnóstico do código consultado, divergências e riscos. |
| [`docs/DECISIONS_PENDING.md`](docs/DECISIONS_PENDING.md) | Decisões que precisam ser resolvidas antes da consolidação da arquitetura. |
| [`docs/AI_GUIDE.md`](docs/AI_GUIDE.md) | Contexto e prompt de referência para pedir análises a uma IA. |

## Resumo do projeto

O ESP32 consulta a cotação de um ativo por Wi-Fi, avalia condições de preço e, quando o gatilho é satisfeito, solicita uma ordem à API da corretora. Uma terceira tarefa registra eventos pela interface serial. A proposta inicial define três tarefas concorrentes:

- **T1 — Gatilho de Operação:** tarefa orientada a evento, de maior criticidade temporal;
- **T2 — Consulta de Cotação:** tarefa periódica, com período preliminar de 5 segundos;
- **T3 — Registro de Operações:** tarefa esporádica, com comportamento best-effort.

O requisito temporal preliminar de T1 é inferior a 500 ms entre a detecção do gatilho e o início do envio da requisição pela rede. Esse valor ainda precisa ser validado experimentalmente; não representa uma garantia de que a corretora processará ou executará a ordem dentro de 500 ms.

## Divergência técnica prioritária

O `platformio.ini` atualmente consultado declara `framework = arduino`. O roteiro do PC3 pede explicitamente uma aplicação utilizando **ESP-IDF e FreeRTOS**. PlatformIO pode ser utilizado com ESP-IDF, mas a configuração atual está em Arduino. A equipe precisa decidir e alinhar a migração antes de tratar o projeto como aderente ao PC3.

## Como começar

0. Leia [`PLANO_ACAO.md`](PLANO_ACAO.md) para seguir a sequência de decisões e ações prioritárias do projeto.
1. Leia [`docs/CURRENT_STATUS.md`](docs/CURRENT_STATUS.md) para entender o estado observado do código.
2. Leia [`docs/DECISIONS_PENDING.md`](docs/DECISIONS_PENDING.md) e resolva as divergências com o grupo/professor.
3. Leia [`ARCHITECTURE.md`](ARCHITECTURE.md) e [`docs/TASKS.md`](docs/TASKS.md) para consolidar a arquitetura.
4. Use [`docs/REQUIREMENTS.md`](docs/REQUIREMENTS.md) e [`docs/TEST_PLAN.md`](docs/TEST_PLAN.md) para orientar implementação e evidências.

## Segurança

O arquivo `include/config.h` da branch consultada continha credenciais de Wi-Fi e da API. Os valores não foram reproduzidos nesta documentação. Caso ainda sejam válidos, trate-os como comprometidos: altere a senha Wi-Fi e revogue/rotacione as credenciais da API. Não compartilhe `config.h` com segredos, arquivos locais de credenciais ou logs que possam revelar tokens.

O sistema deve permanecer restrito à Binance Testnet ou a simulações. Não conecte uma implementação não validada a uma conta com fundos reais.

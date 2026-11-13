# Gatilho de Negociação de Ativos — ESP32

Projeto acadêmico de Sistemas de Tempo Real. O firmware está organizado como aplicação **ESP-IDF + FreeRTOS**, usando PlatformIO como configuração de build. A implementação atual é uma base modular em evolução; ainda não foi validada em placa física.

## Comece por aqui

1. Leia [CURRENT_STATUS.md](CURRENT_STATUS.md) para ver o que existe e o que ainda não foi validado.
2. Leia [PROJECT_SPEC.md](PROJECT_SPEC.md), [REQUIREMENTS.md](REQUIREMENTS.md) e [DECISIONS_PENDING.md](DECISIONS_PENDING.md).
3. Consulte [ARCHITECTURE.md](ARCHITECTURE.md) e [TASKS.md](TASKS.md) antes de alterar tarefas ou sincronização.
4. Siga [APLICACAO_LOCAL.md](APLICACAO_LOCAL.md) para configurar o ambiente e executar as verificações disponíveis.
5. Registre resultados reais conforme [TEST_PLAN.md](TEST_PLAN.md).

## Documentação

| Arquivo | Conteúdo |
|---|---|
| [PROJECT_SPEC.md](PROJECT_SPEC.md) | Objetivos acadêmicos e limites herdados de PC1, PC2 e PC3. |
| [REQUIREMENTS.md](REQUIREMENTS.md) | Requisitos funcionais, temporais e de segurança. |
| [TASKS.md](TASKS.md) | Ativação, responsabilidades, prioridades e comunicação das tarefas. |
| [ARCHITECTURE.md](ARCHITECTURE.md) | Arquitetura implementada localmente e fluxo dos módulos. |
| [DECISIONS_PENDING.md](DECISIONS_PENDING.md) | Decisões de estratégia e validação que continuam bloqueadas. |
| [TEST_PLAN.md](TEST_PLAN.md) | Casos planejados e evidências; não é um relatório de testes aprovados. |
| [PLANO_ACAO.md](PLANO_ACAO.md) | Ações, estado atual e critérios para concluir o projeto. |
| [AGENTS.md](AGENTS.md) | Regras de contribuição e segurança para agentes de IA. |
| [AI_GUIDE.md](AI_GUIDE.md) | Contexto sanitizado para consultas técnicas a uma IA. |
| [APLICACAO_LOCAL.md](APLICACAO_LOCAL.md) | Configuração local, build e testes disponíveis. |

## Estrutura do firmware

- `src/main.cpp`: inicialização ESP-IDF, Wi-Fi e criação das tarefas.
- `src/tasks/`: T1 (gatilho), T2 (consulta) e T3 (registro).
- `src/services/`: consulta pública à cotação e logging assíncrono.
- `src/system/`: filas, Event Group e máquina de estados do gatilho.
- `include/`: interfaces e configuração.
- `test/native/`: testes host da máquina de estados, independentes do hardware.

Sem `include/secrets.h` local, o firmware usa uma sequência simulada e não envia ordens. A estratégia de BUY/SELL ainda não foi aprovada; por isso, o envio de ordens permanece desabilitado em qualquer modo.

## Build e segurança

O build ESP-IDF pode ser iniciado com `pio run` após instalar e configurar PlatformIO com a plataforma ESP32. A máquina usada nesta revisão não tinha `pio` nem `idf.py`, então ainda não há resultado de compilação ESP-IDF.

Não use Binance Mainnet nem contas com fundos reais. O arquivo [include/secrets.example.h](include/secrets.example.h) contém somente valores vazios de referência. Segredos devem ficar em `include/secrets.h`, ignorado pelo Git, e não devem ser incluídos em logs, documentação ou consultas a IA.

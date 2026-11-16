# Especificação do projeto

## Objetivo acadêmico

O **Gatilho de Negociação de Ativos** é um sistema embarcado em ESP32 para demonstrar decomposição de tarefas, comunicação, sincronização e análise temporal em Sistemas de Tempo Real. A consulta de cotação e qualquer ação de negociação são atividades distintas; o serviço externo não está sob controle do escalonador local.

## Escopo proposto no PC1

- T1 — Gatilho de Operação: ativação por evento de preço e maior prioridade relativa.
- T2 — Consulta de Cotação: tarefa periódica; período inicial proposto de 5 s.
- T3 — Registro de Operações: tarefa de baixa prioridade, orientada a eventos e best-effort.
- RT01 preliminar: menos de 500 ms entre detecção do gatilho e início do envio pela rede. Esse alvo não é garantia de resposta nem de execução remota.
- A proposta menciona cruzamentos em sentidos diferentes e operações de compra/venda, mas não define inequivocamente qual ação corresponde a cada cruzamento.

## PC2

O trabalho requer arquitetura geral, decomposição em tarefas FreeRTOS, prioridades e ativações, comunicação entre tarefas, sincronização/recursos compartilhados, estratégia temporal, diagrama consolidado e recursos de hardware/software.

## PC3

O trabalho exige ESP-IDF e FreeRTOS, integração funcional, implementação das tarefas, comunicação e sincronização, temporização, testes funcionais, evidência de concorrência, experimento controlado com pelo menos duas condições de carga, teste integrado e limitações documentadas.

## Limites e decisões

- A implementação local usa ESP-IDF como framework, com PlatformIO configurado como build. A compilação ainda precisa ser executada em ambiente com toolchain.
- O fluxo atual detecta cruzamentos e simula cotações quando não há credenciais Wi-Fi.
- Nenhum lado/quantidade de ordem foi adotado. Submissão externa está desabilitada até a decisão da equipe.
- A histerese de 100 unidades é herdada do código existente, não é requisito aprovado do PC1.
- Toda integração deve permanecer em mock local ou Binance Spot Testnet; Mainnet está fora do escopo.

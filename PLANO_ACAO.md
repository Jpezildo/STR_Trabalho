# Plano de ação do projeto STR_Trabalho

Este documento organiza a consolidação do firmware, sem cronograma. A implementação atual evita decisões de estratégia financeira que não foram aprovadas.

## Concluído localmente (ainda sem validação de build)

- Alterar framework para ESP-IDF/FreeRTOS e criar arquivos CMake de projeto/componente.
- Separar inicialização, T1, T2, T3, serviços, filas, sincronização e máquina de estados.
- Configurar consulta periódica para 5 s e fallback determinístico simulado quando não houver credenciais Wi-Fi locais.
- Implementar máquina de estados que identifica cruzamentos e rearmamento herdado, sem escolher BUY/SELL.
- Substituir credenciais no arquivo rastreado por configuração local opcional e ignorar `include/secrets.h`.
- Corrigir documentação para refletir arquivos e módulos existentes; adicionar especificação, requisitos e descrição de tarefas.
- Compilar e executar o teste host da máquina de estados e o modelo virtual de carga; os resultados não substituem medições em ESP32.

## Ações em aberto

1. **Resolver a regra do gatilho.** A equipe/professor deve aprovar cruzamento → ação, ativo, quantidade, rearme e prevenção de duplicidade. Até lá, manter submissão de ordem desabilitada.
2. **Validar a integração de rede.** Instalar a toolchain ESP-IDF/PlatformIO, compilar, tratar erros e verificar TLS, Wi-Fi, reconexão e cliente HTTP na placa.
3. **Definir RT01 mensurável.** Separar detecção, preparação, envio e resposta. Não declarar menos de 500 ms atendido antes de medir em condições documentadas.
4. **Implementar ordem de teste após a decisão.** Usar mock local ou Testnet com timestamp/assinatura HMAC e credenciais locais. Nunca Mainnet.
5. **Executar testes do PC3.** Testar comunicação, erros de rede, fila, logs, concorrência, carga em pelo menos duas condições e fluxo integrado com mock.
6. **Atualizar evidências.** Registrar resultados reais em [TEST_PLAN.md](TEST_PLAN.md) e atualizar [CURRENT_STATUS.md](CURRENT_STATUS.md).
7. **Ação manual de segurança.** O proprietário deve rotacionar segredos que possam ter sido publicados e revisar o histórico remoto. Não foi feita reescrita do histórico.

## Critérios de conclusão

- Build ESP-IDF reproduzível e erros/avisos revisados.
- Regra de operação aprovada e implementada sem duplicidade não intencional.
- Segredos fora do Git e credenciais antigas rotacionadas.
- RT01 mensurado com ponto de início explicitamente definido.
- Testes funcionais, concorrência, carga e integração registrados com evidências.
- Documentação descreve o que foi realmente executado e mantém limitações visíveis.

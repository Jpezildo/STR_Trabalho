# Instruções para agentes de IA

Leia [README.md](README.md), [ARCHITECTURE.md](ARCHITECTURE.md), [REQUIREMENTS.md](REQUIREMENTS.md) e [DECISIONS_PENDING.md](DECISIONS_PENDING.md) antes de alterar o projeto.

## Estado real

- Firmware C++ com ESP-IDF e FreeRTOS; build configurado para PlatformIO em `platformio.ini`.
- Entrada do firmware: `src/main.cpp` (`app_main`).
- T1/T2/T3: `src/tasks/`.
- Filas e sincronização: `src/system/`; serviço HTTP público e logging: `src/services/`.
- `include/secrets.h` é local/ignorado; `include/secrets.example.h` contém apenas placeholders.
- O firmware não envia ordens. A regra BUY/SELL não está aprovada.

## Regras

1. Inspecione `git status`, branch e arquivos atuais antes de editar. Preserve mudanças de terceiros.
2. Não invente regras de compra/venda, ativo, limiar, quantidade ou tipo de ordem. A falta de mapeamento de cruzamento para ação mantém ordens desabilitadas.
3. Não use Mainnet, não realize transações reais e não afirme execução com base apenas em HTTP positivo.
4. Nunca revele ou copie credenciais. Não leia `include/secrets.h`; não o sobrescreva. Não altere histórico Git. A rotação de credenciais expostas cabe ao proprietário.
5. Não desative validação TLS. A consulta HTTPS usa o bundle de certificados do ESP-IDF.
6. Preserve o caráter não bloqueante de T1/T2 para logging e registre perdas de fila.
7. Verifique retornos de criação de recursos/tarefas e mostre erros reais.
8. Não declare builds, testes, métricas ou hardware validados sem executar e registrar evidências.
9. Atualize os documentos relacionados quando mudar arquitetura, comportamento, build ou resultados.
10. Não faça commits, pushes ou alterações remotas sem solicitação explícita.

## Verificação local

- Firmware: `pio run` (requer PlatformIO e toolchain ESP-IDF instalados).
- Teste host da máquina de estados: comando e escopo descritos em [APLICACAO_LOCAL.md](APLICACAO_LOCAL.md).
- Upload e comportamento em placa exigem ESP32 físico; não inferir validação de hardware a partir de compilação.

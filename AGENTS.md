# Instruções para agentes de IA

Leia `README.md` e `ARCHITECTURE.md` antes de propor ou realizar alterações neste projeto.

## Contexto do projeto

- Firmware em C++ para ESP32.
- Plataforma de build: PlatformIO.
- Framework: Arduino.
- Concorrência: FreeRTOS.
- Dependência JSON: ArduinoJson 7.
- Integração de rede: Binance Testnet.

## Regras obrigatórias

1. Não invente arquivos, funções, testes ou resultados de compilação. Inspecione os arquivos disponíveis antes de concluir.
2. Preserve a separação entre inicialização (`main.cpp`), recursos compartilhados (`AppContext`), tarefas (`src/tasks/`), logging (`logger.cpp`) e integração HTTP (`src/services/`).
3. Não mova lógica entre módulos sem explicar a mudança e atualizar `ARCHITECTURE.md`.
4. Não altere o limiar, intervalo de consulta, quantidade, lado ou tipo de ordem sem solicitação explícita.
5. Não afirme que uma ordem foi executada apenas porque a requisição retornou um código HTTP positivo.
6. Não remova o mutex de rede nem substitua filas/Event Groups por variáveis globais compartilhadas sem justificar e analisar a concorrência.
7. Verifique os retornos de criação de filas, mutexes, Event Groups e tarefas.
8. Não introduza credenciais reais no código, documentação, exemplos, logs ou saída. Nunca leia ou solicite `include/secrets.h` sem necessidade explícita.
9. Use somente `include/secrets.example.h` como referência de nomes de configuração. O arquivo real `include/secrets.h` é local e não deve ser versionado.
10. Não faça alterações no GitHub, commits, branches, pull requests ou issues. Só faça isso quando o usuário pedir explicitamente.
11. Não declare testes aprovados sem executá-los. Informe exatamente quais comandos foram executados e quais limitações permaneceram.
12. Prefira mudanças pequenas e revisáveis. Separe refatoração estrutural de correções funcionais.

## Verificação

O comando principal de compilação é:

```bash
pio run
```

Quando houver hardware disponível, upload e monitor serial podem ser verificados com:

```bash
pio run --target upload
pio device monitor
```

Se PlatformIO ou uma placa ESP32 não estiverem disponíveis, diga isso claramente. Não simule resultados.

## Ao responder

- Explique primeiro o problema e a proposta de alteração.
- Liste os arquivos que serão criados, modificados ou removidos.
- Destaque mudanças de comportamento, riscos de concorrência e impacto em tempo real.
- Diferencie fatos observados no repositório de hipóteses.
- Atualize `README.md` e/ou `ARCHITECTURE.md` quando a mudança afetar a documentação.

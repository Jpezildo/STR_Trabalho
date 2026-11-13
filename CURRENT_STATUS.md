# Estado atual do projeto

## Alterações implementadas localmente

- A configuração foi alterada para ESP-IDF/FreeRTOS, com PlatformIO e arquivos CMake de projeto/componente.
- A lógica concentrada em `src/main.cpp` foi dividida em T1, T2, T3, serviços, filas, sincronização e máquina de estados.
- T2 usa período inicial de 5000 ms, fila de preço de tamanho 1 e serviço de ticker HTTPS com bundle TLS.
- Sem credenciais locais, T2 gera preços simulados. A configuração rastreada foi substituída por valores vazios; `include/secrets.h` está excluído pelo Git.
- T1 detecta cruzamentos a partir de preços válidos anteriores/atuais. Não há estratégia aprovada para mapear cruzamentos para ordens; nenhuma ordem é enviada.
- Há teste host para a máquina de estados em `test/native/test_trigger.cpp`.
- Documentação reorganizada em arquivos Markdown na raiz: [PROJECT_SPEC.md](PROJECT_SPEC.md), [REQUIREMENTS.md](REQUIREMENTS.md), [TASKS.md](TASKS.md) e demais guias.

## Verificação executada nesta revisão

- `git status`, branch e histórico inspecionados antes das alterações; branch de trabalho era `estrutura`.
- Teste host executado com `g++ -std=c++17 -Wall -Wextra -Werror`: `test_trigger.cpp` compilou e passou.
- Modelo host `load_experiment.cpp` compilou e passou suas verificações. Resultado determinístico: baixa (5000 ms) = 12 produzidos, 12 consumidos, 0 sobrescritos, 0 logs descartados, pico 1, pior atraso 0 ms; alta (500 ms) = 120 produzidos, 80 consumidos, 39 sobrescritos, 4 logs descartados, pico 16, pior atraso 250 ms. São dados do modelo virtual, não medições do ESP32.
- `pio run` foi tentado e não executou: `pio` não é reconhecido no PATH.
- `idf.py build` foi tentado e não executou: `idf.py` não é reconhecido no PATH.
- A verificação automatizada de links locais Markdown passou: todos os destinos locais existem.
- Build ESP-IDF, execução em placa e validação de rede/TLS permanecem não verificados.

## Limitações e riscos abertos

1. A API privada de ordem não está implementada, nem habilitada; faltam decisão sobre BUY/SELL, assinatura, parâmetros e política de duplicidade.
2. A medição RT01 (detecção até início de envio) não pode ser declarada atendida sem ordem habilitada e instrumento adequado. O timestamp atual mede processamento/atraso da fila.
3. Wi-Fi, TLS, NVS e cliente HTTP precisam de build com ESP-IDF e teste em placa.
4. Ainda não há experimento de carga integrado em ESP32 nem teste completo de ponta a ponta.
5. As credenciais anteriormente expostas podem permanecer no histórico Git. O proprietário deve revogá-las/rotacioná-las; não foi feita alteração do histórico.

## Próximas verificações

Consulte [TEST_PLAN.md](TEST_PLAN.md) e [DECISIONS_PENDING.md](DECISIONS_PENDING.md). Não apresentar testes, build ou medições como aprovados antes de executar e guardar evidência.

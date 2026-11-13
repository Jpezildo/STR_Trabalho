# Diagnóstico do estado atual

**Data do snapshot analisado:** 13/11/2026  
**Repositório:** `Jpezildo/STR_Trabalho`  
**Branch consultada:** `Develop`

Este diagnóstico descreve somente os arquivos efetivamente consultados nesta revisão. Ele não substitui a compilação local nem uma inspeção completa de todos os arquivos do repositório.

O plano de ação para tratar os pontos levantados está em [`PLANO_ACAO.md`](PLANO_ACAO.md). Use-o como referência para a sequência de trabalho; as pendências técnicas abaixo continuam sem resolução até que sejam decididas e validadas.

## 1. Arquivos consultados

| Caminho | Observação |
|---|---|
| `platformio.ini` | PlatformIO, plataforma `espressif32`, placa `esp32dev`, framework `arduino`, monitor serial 115200, ArduinoJson 7. |
| `src/main.cpp` | Contém criação dos recursos FreeRTOS, inicialização Wi-Fi/NTP e as três funções de tarefa. |
| `include/config.h` | Contém limiar, URLs e credenciais locais/publicadas. Valores secretos foram deliberadamente omitidos deste diagnóstico. |
| `README.md` | Não foi encontrado na raiz da branch `Develop` durante a consulta. |

## 2. Resumo do código observado

### T1 — `vTaskGatilho`

- Aguarda preço em `Q_PRICE` com `portMAX_DELAY`.
- Usa `sistema_armado` para impedir tentativas contínuas após o disparo.
- Dispara quando `preco_atual > LIMIAR_GATILHO`.
- Envia uma requisição POST com `side=BUY`, `type=MARKET` e quantidade fixa no corpo.
- Espera `M_NET` por até 2000 ms.
- Após concluir a tentativa dentro da seção protegida, desarma o sistema; rearma quando o preço fica abaixo do limiar menos 100.

**Diferença em relação ao PC1:** a proposta fala em cruzamento para cima ou para baixo e compra/venda; o código observado cobre somente a condição acima do limiar e tentativa de compra. Além disso, a comparação é com o valor do limiar, não com a comparação explícita entre preço anterior e atual para comprovar um cruzamento.

### T2 — `vTaskConsulta`

- Usa `vTaskDelayUntil()` com frequência programada de 20 ms.
- Exige bits de Wi-Fi e NTP ativos antes de consultar.
- Espera `M_NET` por até 1000 ms.
- Faz GET do ticker, interpreta o campo `price` do JSON e publica o valor usando `xQueueOverwrite(Q_PRICE, ...)`.

**Diferença em relação ao PC1:** o período preliminar documentado pelo grupo é 5 s, não 20 ms.

### T3 — `vTaskRegistro`

- Aguarda mensagens na fila `Q_LOG`.
- Imprime cada mensagem na Serial.
- Sua prioridade numérica é 1, abaixo de T2 (2) e T1 (3).

### Recursos compartilhados

- `Q_PRICE`: fila com uma posição para um `float`.
- `Q_LOG`: fila com 16 estruturas `LogMessage`.
- `M_NET`: mutex para serializar chamadas HTTP de T1 e T2.
- `EG_SYS`: Event Group para bits de Wi-Fi e sincronização NTP.

## 3. Divergência Arduino versus ESP-IDF

O arquivo `platformio.ini` declara `framework = arduino`. O roteiro do PC3 pede explicitamente que a aplicação seja implementada utilizando ESP-IDF e FreeRTOS. PlatformIO pode ser usado com ESP-IDF, mas a configuração atualmente consultada não seleciona esse framework.

**Consequência:** não tratar a versão atual como aderente ao requisito de framework do PC3 sem confirmar com o professor e, se necessário, migrar e validar o projeto.

## 4. Problemas que afetam o deadline de 500 ms

### 4.1 Espera pelo mutex

T1 pode esperar até 2000 ms por `M_NET`. Esse valor, sozinho, já ultrapassa o deadline preliminar de 500 ms. Enquanto T2 mantém o mutex durante uma operação HTTP, T1 pode ficar impedida de iniciar sua requisição.

O mutex é útil para proteção do recurso compartilhado, mas sua política e duração de posse precisam ser revistas. Não basta dar prioridade maior à T1 se ela continuar bloqueada esperando uma operação de rede iniciada por outra tarefa.

### 4.2 Medição atual não mede o início do envio

O código registra `t0` antes de obter o mutex e `t1` depois de `http.POST()` retornar. Portanto, `t1 - t0` inclui espera pelo mutex e a duração da chamada HTTP até a resposta/falha. Não é a métrica isolada do requisito descrito no PC1 (detecção até início do envio pela rede).

Para avaliar RT01, registrar pelo menos `t_detect` e `t_send_begin` separadamente. Registrar o fim da chamada HTTP como métrica auxiliar.

## 5. Integração com a Binance

O código consultado adiciona o header `X-MBX-APIKEY` à requisição de ordem, mas não monta os parâmetros `timestamp` e `signature` nem utiliza o segredo para assinar o payload.

A documentação oficial da Binance Spot Testnet especifica que endpoints privados/assinados precisam de `signature` e `timestamp`; a documentação inclui exemplos HMAC-SHA-256. Assim, o POST observado não está completo para autenticação de uma ordem privada. Além disso, um código HTTP positivo, isoladamente, não comprova que a ordem foi aceita ou executada.

Referência: https://developers.binance.me/en/docs/products/spot/testnet/rest-api

## 6. Segurança de credenciais

O `include/config.h` consultado continha credenciais de Wi-Fi e Binance Testnet diretamente no código. Não reproduzimos nenhum valor neste pacote.

Ações recomendadas:

1. trocar a senha Wi-Fi caso ainda esteja válida;
2. revogar/rotacionar a API Key e Secret Key expostas;
3. remover segredos de arquivos versionados e armazená-los somente em configuração local ignorada pelo Git ou mecanismo de segredo apropriado;
4. revisar o histórico Git, pois apagar o valor do arquivo atual não o remove de commits antigos;
5. não compartilhar o arquivo de configuração real com IA ou em issues/capturas.

## 7. Outras limitações observadas

- `xQueueCreate`, `xSemaphoreCreateMutex`, `xEventGroupCreate` e `xTaskCreatePinnedToCore` não têm seus retornos verificados no trecho consultado.
- A conexão Wi-Fi espera indefinidamente por `WL_CONNECTED`; não há timeout/reconexão nessa rotina.
- A sincronização NTP é tentada uma vez, sem repetição explícita.
- `enviarLog()` tenta inserir mensagem sem espera; se `Q_LOG` estiver cheia, o log pode ser perdido.
- O intervalo de 20 ms pode gerar chamadas e logs em alta frequência; revisar limites da API e usar mock para experimentos de carga.
- O código não apresenta evidências de testes automatizados ou medições reproduzíveis nesta análise.
- A validação explícita de certificado TLS não foi identificada no trecho consultado; revisar como o cliente HTTPS está configurado na versão final.

## 8. O que ainda não foi verificado

- compilação da branch `Develop` em ambiente local;
- upload e execução em uma placa física;
- funcionamento real do endpoint de ordem;
- autenticação e assinatura da requisição;
- atendimento do deadline inferior a 500 ms;
- comportamento sob carga;
- aderência do projeto ao ESP-IDF exigido no PC3.

Não apresentar esses itens como aprovados até que existam logs e resultados de testes reproduzíveis.

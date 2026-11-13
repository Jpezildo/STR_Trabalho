# Decisões pendentes do projeto

Use este arquivo como pauta de alinhamento do grupo e, quando necessário, de consulta ao professor. Os itens abaixo não devem ser resolvidos silenciosamente por uma IA.

O plano de ação em [`PLANO_ACAO.md`](PLANO_ACAO.md) organiza as decisões e ações necessárias para consolidar o projeto. Consulte-o para a sequência recomendada de trabalho; este arquivo continua sendo o registro das decisões técnicas pendentes e adotadas.

## D01 — Framework exigido no PC3

**Situação:** `platformio.ini` da branch `Develop` usa `framework = arduino`; o roteiro do PC3 pede ESP-IDF e FreeRTOS.

**Decisão necessária:** confirmar com o professor se o PC3 exige ESP-IDF nativo ou se a camada Arduino é aceita para a atividade. Se ESP-IDF for obrigatório, definir migração e estratégia de build. PlatformIO pode ser mantido como ferramenta se configurado para o framework correto.

**Prioridade:** crítica.

## D02 — Significado mensurável do deadline de 500 ms

**Situação:** o PC1 define menos de 500 ms entre detecção do gatilho e geração/início do envio via Wi-Fi. A implementação atual mede até o retorno da chamada POST.

**Decisão necessária:** fixar o ponto exato de início e fim da métrica principal. Recomendação de interpretação alinhada ao PC1: `t_send_begin - t_detect`. Medir o tempo da resposta HTTP e a confirmação remota separadamente.

**Prioridade:** crítica.

## D03 — Polling de 5 s versus 20 ms

**Situação:** PC1 registra 5 s; código atual usa 20 ms.

**Decisão necessária:** definir o período de projeto a ser levado ao PC2. Justificar tecnicamente a escolha considerando atualidade da cotação, taxa de requisições, latência e experimento de carga. Não assumir que 20 ms é uma necessidade do sistema.

**Prioridade:** alta.

## D04 — Regra de compra e venda

**Situação:** proposta menciona cruzamentos para cima/baixo e compra/venda. Código atual tenta apenas BUY quando preço > limiar.

**Decisão necessária:** estabelecer tabela de decisão, por exemplo qual cruzamento gera BUY, qual gera SELL, que ativo e parâmetros entram na ordem e se haverá uma ou mais estratégias.

**Prioridade:** alta.

## D05 — Definição formal de “cruzamento”

**Situação:** o código verifica se o preço está acima do limiar, o que não prova por si só que houve cruzamento naquele instante.

**Decisão necessária:** definir se o gatilho compara o preço anterior e o atual (transição entre lados do limiar), usa uma condição por faixa ou outra regra. Definir tratamento para primeira cotação após inicialização.

**Prioridade:** alta.

## D06 — Rearme, histerese e duplicidade

**Situação:** código atual desarma depois da tentativa e rearma quando o preço fica abaixo do limiar menos 100. A regra não está claramente especificada no PC1.

**Decisão necessária:** aprovar uma política explícita: condição de desarme, condição de rearme, comportamento em erro HTTP, timeout ou falha de autenticação, e prevenção contra ordem duplicada.

**Prioridade:** alta.

## D07 — Recurso de rede compartilhado

**Situação:** T1 e T2 usam `M_NET`; T1 pode aguardar o mutex por até 2 s, enquanto RT01 é menor que 500 ms.

**Decisão necessária:** escolher estratégia para coordenar o uso HTTP sem permitir que T1 espere mais que o deadline. Avaliar duração máxima de operações, timeout, client independente, fila de solicitações de ordem ou outra estratégia compatível com a plataforma. Testar a decisão.

**Prioridade:** crítica.

## D08 — NTP e dependência de horário

**Situação:** T2 só consulta quando Wi-Fi e NTP estão prontos. Consulta pública do ticker e envio de ordem assinada têm necessidades diferentes; o endpoint privado exige timestamp.

**Decisão necessária:** decidir se T2 pode consultar após Wi-Fi, deixando NTP como pré-condição apenas para requisições assinadas, ou se há outro motivo para bloquear a consulta.

**Prioridade:** média.

## D09 — Estratégia de teste de ordem

**Situação:** a integração privada do código atual não inclui assinatura. O PC1 admite API com envio de ordens ou simulação/mock.

**Decisão necessária:** escolher entre mock local, endpoint de teste de criação de ordem da Binance Testnet (`POST /api/v3/order/test`) ou uma integração autenticada com a Testnet. O endpoint de teste valida a ordem sem enviá-la ao mecanismo de matching, mas exige a assinatura correspondente.

**Prioridade:** alta.

## D10 — Configuração de limiar, ativo e parâmetros da ordem

**Situação:** o código consultado define ativo/endpoint, limiar e dados da operação no código.

**Decisão necessária:** decidir quais parâmetros serão configuráveis, quais serão constantes de compilação e como serão validados. Não alterar quantidade, ativo ou estratégia sem autorização do grupo.

**Prioridade:** média.

## D11 — Semântica das prioridades

**Situação:** PC1 usa rótulos “Prioridade 1/2/3” para alta/média/baixa; código usa valores numéricos 3/2/1.

**Decisão necessária:** documentar as prioridades como alta/média/baixa e registrar os valores reais adotados na implementação. No FreeRTOS, número maior representa prioridade maior.

**Prioridade:** média.

## D12 — Política de perda de mensagens

**Situação:** fila de preço tem tamanho 1 e sobrescreve valores; fila de logs pode descartar mensagens se estiver cheia.

**Decisão necessária:** decidir se é aceitável descartar cotações intermediárias e logs, e quais contadores/indicadores serão medidos no experimento de carga do PC3.

**Prioridade:** média.

## D13 — Credenciais publicadas

**Situação:** o `include/config.h` consultado contém credenciais em texto.

**Decisão necessária:** rotacionar/revogar credenciais expostas, revisar histórico Git e estabelecer padrão local de segredos ignorados pelo controle de versão.

**Prioridade:** crítica e imediata.

## Registro de decisões

Quando uma decisão for tomada, registrar aqui:

| ID | Decisão adotada | Responsável/validação | Data | Arquivos afetados |
|---|---|---|---|---|
| D01 | Pendente | — | — | `platformio.ini`, `README.md`, arquitetura |
| D02 | Pendente | — | — | `REQUIREMENTS.md`, instrumentação, testes |
| D03 | Pendente | — | — | `config`, T2, `REQUIREMENTS.md` |
| D04–D06 | Pendente | — | — | T1, requisitos, testes |
| D07–D12 | Pendente | — | — | arquitetura, tarefas, testes |
| D13 | Pendente | — | — | configuração local, histórico Git |

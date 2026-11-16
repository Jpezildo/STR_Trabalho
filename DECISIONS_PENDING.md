# Decisões pendentes

Este arquivo separa decisões acadêmicas ainda necessárias dos comportamentos técnicos que já foram implementados. As escolhas abaixo não devem ser feitas silenciosamente por uma IA.

## D01 — Ação associada a cada cruzamento

- **Pergunta:** cruzamento ascendente e descendente geram BUY, SELL ou apenas sinalização?
- **Importância:** não existe implementação correta de ordens sem a regra aprovada.
- **Bloqueio:** nenhuma ordem, nem mesmo uma requisição de teste que exija lado/quantidade, é enviada.
- **Decisão:** equipe/professor devem fornecer a tabela de decisão e confirmar parâmetros.

## D02 — Limiar, ativo e quantidade

- **Pergunta:** manter valores atuais ou escolher valores de demonstração?
- **Importância:** afeta a estratégia e a validade acadêmica.
- **Bloqueio:** ativo, limiar atual de 61000 e parâmetros de operação permanecem sem nova aprovação.
- **Observação:** o valor de histerese 100 é herdado do código anterior e configurável no firmware; não é requisito aprovado.

## D03 — Definição e evidência de RT01

- **Pergunta:** que evento observável representa exatamente o início da transmissão?
- **Importância:** o PC1 define menos de 500 ms entre detecção e início do envio, não até resposta/execução remota.
- **Bloqueio:** sem ordem habilitada, não há `t_send_begin`; o requisito não pode ser declarado atendido.
- **Decisão:** validar a métrica com professor e instrumentar evento disponível na versão ESP-IDF adotada; rotular aproximação quando não for medição do primeiro byte na rede.

## D04 — Teste da integração privada

- **Pergunta:** usar mock local ou endpoint `/api/v3/order/test` da Binance Spot Testnet depois de aprovada a estratégia?
- **Importância:** ordem privada precisa timestamp, assinatura HMAC e credenciais.
- **Bloqueio:** somente consulta pública do ticker está implementada; autenticação de ordem e SNTP seguem desabilitados.

## D05 — Ambiente e versão ESP-IDF

- **Situação:** `platformio.ini` seleciona framework `espidf`, mas o ambiente analisado não tem `pio` nem `idf.py` disponíveis no PATH.
- **Ação necessária:** instalar/configurar toolchain e registrar versões usadas; executar build e resolver incompatibilidades.

## D06 — Experimento PC3

- **Pergunta:** duração e condições de carga a executar em placa, sem sobrecarregar API externa?
- **Proposta:** usar eventos simulados de frequência baixa e alta, mantendo firmware, duração, tarefas e filas comparáveis.
- **Bloqueio:** números e evidências físicos ainda não existem.

## D07 — Credenciais anteriormente expostas

- **Ação:** proprietário deve rotacionar senha Wi-Fi e chaves API que tenham sido publicadas e verificar o histórico remoto.
- **Limite:** não foram copiadas credenciais e não foi reescrito histórico. Remover do arquivo atual não invalida valores já expostos.

## Registro

| ID | Estado | Evidência/decisão |
|---|---|---|
| D01 | Pendente | Mapeamento de cruzamentos ainda não aprovado. |
| D02 | Pendente | 61000 e histerese 100 foram mantidos como valores herdados. |
| D03 | Pendente | Não existe envio para medir. |
| D04 | Pendente | Serviço de ordem não implementado. |
| D05 | Bloqueado | Toolchain ausente no PATH na inspeção local. |
| D06 | Pendente | Testes no hardware ainda não executados. |
| D07 | Ação manual necessária | Rotação e revisão de histórico cabem ao proprietário. |

# Requisitos do sistema

Os requisitos descrevem o escopo acadêmico e a implementação local. “Implementado” não significa “validado em hardware”; evidências ficam em [TEST_PLAN.md](TEST_PLAN.md).

| ID | Requisito | Estado |
|---|---|---|
| RF01 | Consultar cotação por HTTPS em período configurado de 5 s. | Parcial: serviço e período implementados; build/integração não verificados. |
| RF02 | Rejeitar HTTP inválido, JSON inválido, preço não numérico, não finito ou não positivo. | Implementado no serviço; testes de integração pendentes. |
| RF03 | Publicar uma cotação recente para T1 por fila FreeRTOS. | Implementado com fila de um elemento e identificação sequencial; sobrescrita é possível. |
| RF04 | Detectar cruzamentos reais usando amostra anterior e atual; primeira amostra não dispara. | Implementado na máquina de estados; teste host disponível. |
| RF05 | Disparar BUY/SELL conforme estratégia aprovada. | Pendente; não enviar ordens até a equipe decidir o mapeamento. |
| RF06 | Rearmar após histerese configurada, evitando disparos repetidos. | Parcial: rearme abaixo do limiar menos 100 herdado do código anterior. |
| RF07 | Registrar eventos de forma assíncrona sem bloquear produtores. | Implementado com fila não bloqueante; perdas são contadas e reportadas. |
| RF08 | Expor condição de conectividade Wi-Fi e tentar reconectar após desconexão. | Implementado; integração em dispositivo não verificada. |
| RF09 | Usar TLS com verificação de certificados para HTTPS. | Configurado por bundle de certificados; build/dispositivo não verificados. |
| RT01 | Meta preliminar inferior a 500 ms entre detecção e início do envio da ordem. | Não verificável: não há envio habilitado nem medição de início de transmissão. |
| RT02 | T2 consulta periodicamente a cada 5 s por padrão. | Implementado na configuração; comportamento não medido em hardware. |
| SEC01 | Não manter segredos em arquivos rastreados; permitir simulação sem credenciais. | Segredos removidos da configuração atual; segredos anteriores no histórico ainda exigem rotação manual. |
| PC3-01 | Implementar em ESP-IDF e FreeRTOS. | Código e build configurados; build pendente por toolchain ausente no ambiente inspecionado. |
| PC3-02 | Demonstrar carga controlada em duas ou mais condições e teste integrado. | Plano e teste unitário preparados; experimento integrado em hardware ainda pendente. |

# Plano de ação do projeto STR_Trabalho

## Objetivo

Consolidar o projeto para atender ao escopo acadêmico do PC3, com arquitetura coerente, regra de negócio definida, integração de rede segura e evidências de validação. O objetivo principal é garantir que o sistema funcione de forma previsível em tempo real, com separação clara entre tarefas, controle de concorrência e medição de desempenho.

## Contexto

A pasta contém documentação de revisão e análise do projeto, e o código atual observado em `src/main.cpp` apresenta uma implementação preliminar, mas com divergências importantes em relação à proposta acadêmica e ao requisito do PC3.

Os principais pontos observados são:

- uso de `framework = arduino` em `platformio.ini`;
- uso de FreeRTOS em tarefas concorrentes;
- regra de gatilho simplificada e incompleta;
- compartilhamento do mutex de rede entre tarefas críticas;
- falta de definição formal do deadline de 500 ms;
- ausência de autenticação completa da ordem privada da Binance;
- segredos expostos em `include/config.h`.

## Diretriz de trabalho

Antes de alterar qualquer código, a equipe deve resolver as pendências críticas de arquitetura e regra de negócio, documentar as decisões e só então implementar as mudanças de forma incremental.

## Plano de ação

### 1. Resolver a decisão crítica de plataforma

Definir se o projeto será implementado com ESP-IDF nativo ou se o uso de Arduino será aceito pelo professor e pelo roteiro do PC3.

Ações:
- confirmar com o professor/grupo a exigência do PC3;
- decidir se será mantido PlatformIO como ferramenta de build;
- registrar a decisão formalmente;
- ajustar a estrutura do projeto conforme a plataforma escolhida.

Entregáveis:
- decisão documentada sobre o framework;
- ajuste do arquivo de configuração de build, se necessário;
- atualização da arquitetura e da documentação oficial.

### 2. Definir a regra de negócio do gatilho

Formalizar como o sistema decide comprar, vender ou ignorar uma operação.

Ações:
- definir se o comportamento será somente compra, somente venda ou ambos;
- decidir se a regra usa cruzamento de limiar, comparação de valor atual versus anterior ou outra lógica;
- definir a condição de disparo e a condição de rearme;
- estabelecer a política de histerese para evitar oscilações e disparos repetidos;
- definir tratamento para primeira medição, preço inválido e falha de comunicação.

Entregáveis:
- tabela de regras de decisão;
- especificação da lógica de armado/desarmado;
- documentação da regra aplicada no código.

### 3. Definir a métrica temporal correta

Estabelecer claramente o que significa o deadline de 500 ms descrito no PC1.

Ações:
- definir a métrica principal como intervalo entre detecção do gatilho e início do envio da requisição;
- separar também as medições auxiliares de:
  - tempo de aquisição de recursos;
  - tempo de resposta HTTP;
  - tempo total de confirmação da ordem;
- instrumentar o código para registrar timestamps relevantes.

Entregáveis:
- métrica principal formalizada;
- logs com timestamps de detecção, aquisição, envio e resposta;
- explicação clara do que foi medido e do que não foi medido.

### 4. Reavaliar a concorrência e o uso do recurso de rede

O código atual usa um mutex compartilhado entre T1 e T2. Isso pode bloquear a tarefa de gatilho e comprometer o deadline.

Ações:
- avaliar se o mutex deve continuar existindo;
- definir política de acesso ao cliente HTTP e ao endpoint de ordem;
- decidir se será necessário separar clientes ou introduzir fila de requisições;
- garantir que a tarefa crítica não fique bloqueada por operação de outra tarefa;
- testar a solução antes de considerar a arquitetura final.

Entregáveis:
- estratégia de coordenação da rede documentada;
- código com acesso controlado ao recurso compartilhado;
- evidência de que o deadline foi respeitado em teste.

### 5. Corrigir a integração da API e a autenticação da ordem

A integração atual não apresenta a autenticação correta para ordens privadas da Binance Testnet.

Ações:
- revisar endpoint de ordem e parâmetros exigidos;
- incluir timestamp e assinatura, quando necessário;
- garantir que a autenticação respeite a API da corretora utilizada;
- separar consulta pública de ordem privada;
- evitar afirmar que a ordem foi executada somente porque houve resposta HTTP positiva.

Entregáveis:
- fluxo de autenticação correto;
- estrutura de integração de rede adequada;
- documentação de como a ordem é composta e validada.

### 6. Remover segredos e ajustar a configuração

A configuração atual em `include/config.h` contém dados sensíveis expostos em texto.

Ações:
- rotacionar ou revogar credenciais expostas;
- remover segredos do repositório;
- usar arquivo local de configuração ignorado pelo Git;
- manter referências genéricas em documentação e código;
- revisar histórico do Git para detectar vazamento antigo.

Entregáveis:
- configuração segura;
- documentação do padrão de uso local;
- credenciais revogadas/rotacionadas, quando aplicável.

### 7. Reorganizar a estrutura modular do projeto

Separar responsabilidades para melhorar clareza, manutenção e análise de concorrência.

Ações:
- manter configuração centralizada;
- separar módulos para:
  - gatilho;
  - consulta de cotação;
  - registro/log;
  - sincronização;
  - integração HTTP;
  - métricas;
- evitar mistura de responsabilidades em `main.cpp`;
- manter a arquitetura consistente com a proposta do projeto.

Entregáveis:
- módulos organizados por responsabilidade;
- estrutura clara e revisável;
- documentação de módulos e dependências.

### 8. Validar o comportamento do sistema

Antes de declarar o projeto concluído, validar as propriedades fundamentais.

Ações:
- testar Wi‑Fi e reconexão;
- testar sincronização NTP;
- testar consulta de cotação;
- testar gatilho em condições de preço;
- testar falha de rede;
- testar o critério de tempo do gatilho;
- validar logs e estado de cada tarefa.

Entregáveis:
- resultados de testes documentados;
- evidências de comportamento real;
- limitação clara quando o hardware ou a rede não estiverem disponíveis.

### 9. Atualizar a documentação oficial

A documentação precisa refletir a solução final e não apenas a análise inicial.

Ações:
- atualizar `README.md`;
- atualizar `ARCHITECTURE.md`;
- revisar `TEST_PLAN.md`;
- registrar decisões pendentes resolvidas;
- explicar o que foi observado, o que foi decidido e o que ficou como hipótese.

Entregáveis:
- documentação coerente com a implementação final;
- descrição clara de arquitetura, tarefas e regras;
- testes e evidências em formato revisável.

## Critérios de conclusão

O projeto deverá ser considerado pronto quando:

- a plataforma foi definida e aprovada;
- o gatilho foi formalizado e documentado;
- o deadline de 500 ms foi definido e mensurado corretamente;
- a concorrência e o acesso à rede foram analisados e validados;
- a integração da API está correta e segura;
- segredos foram removidos e a configuração ficou segura;
- o código está modularizado e documentado;
- há evidências de testes e medição.

## Observações finais

Este plano é prioritário e orienta a execução gradual da correção e consolidação do projeto. Ele não depende de cronograma, mas define a sequência adequada das decisões e implementações necessárias para transformar a análise atual em um projeto coerente e apresentável.

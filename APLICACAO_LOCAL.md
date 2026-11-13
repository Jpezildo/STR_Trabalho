# Como aplicar esta proposta ao repositório existente

Esta pasta é uma proposta de reorganização para revisão local. Ela não altera o GitHub e não substitui automaticamente os arquivos do seu repositório.

1. Faça uma cópia local/backup do repositório atual.
2. Compare os arquivos desta proposta com os arquivos existentes.
3. Copie os novos diretórios e arquivos para a raiz local do projeto.
4. Adicione `/include/secrets.h` ao `.gitignore` existente. Preserve as regras que já estão lá (`.pio` e configurações do VS Code).
5. Crie `include/secrets.h` localmente a partir de `include/secrets.example.h` e preencha seus dados. Não compartilhe esse arquivo.
6. Remova do `src/main.cpp` original a lógica que foi movida para os módulos novos; use a versão proposta como referência integral.
7. Mantenha `src/tasks/task_registro.cpp` como o módulo que implementa T3; `src/logger.cpp` contém apenas a função que enfileira mensagens.
8. Execute `pio run` no ambiente PlatformIO.
9. Só depois de revisar a compilação e testar no ESP32, considere incorporar a reorganização ao seu fluxo normal de Git.

## Arquivos que permanecem do projeto atual

- `platformio.ini`: mantenha a configuração existente; ela já define ESP32, Arduino e ArduinoJson.
- `.gitignore`: mantenha as regras existentes e acrescente a regra para `include/secrets.h`.
- `lib/` e `test/`: podem permanecer como estão. Os READMEs padrão do PlatformIO podem ser substituídos futuramente quando houver conteúdo real nesses diretórios.

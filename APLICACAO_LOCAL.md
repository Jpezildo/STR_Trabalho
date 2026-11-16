# Configuração e validação local

## Segredos locais

1. Copie `include/secrets.example.h` para `include/secrets.h`.
2. Preencha apenas credenciais locais de Wi-Fi se for testar consulta real à Testnet. O firmware não usa as chaves de ordem e não envia ordens.
3. `include/secrets.h` está no `.gitignore`; confirme `git check-ignore include/secrets.h`.
4. Nunca compartilhe esse arquivo. Rotacione credenciais que tenham sido publicadas anteriormente; apagar o arquivo atual não remove o histórico.

Com SSID e senha preenchidos, o firmware conecta ao Wi-Fi e consulta o ticker público da Spot Testnet por HTTPS. Se ambos estiverem vazios ou `secrets.h` não existir, o firmware entra em simulação. Se apenas um deles estiver preenchido, a inicialização falha explicitamente em vez de usar dados simulados. Nenhuma ordem é enviada.

## Firmware ESP-IDF com PlatformIO

O projeto usa `platformio.ini` (`framework = espidf`), `CMakeLists.txt` na raiz e `src/CMakeLists.txt` como componente. Instale PlatformIO e o toolchain ESP-IDF compatível antes de executar:

```powershell
pio run
```

Upload requer uma placa conectada e configuração local da porta:

```powershell
pio run --target upload
pio device monitor
```

Não foi possível executar esses comandos no ambiente desta revisão porque `pio` e `idf.py` não estavam disponíveis no PATH.

## Teste host da máquina de estados

Com `g++` disponível, no PowerShell a partir da raiz:

```powershell
g++ -std=c++17 -Wall -Wextra -Werror -I include src\system\trigger.cpp test\native\test_trigger.cpp -o "$env:TEMP\str_trigger_test.exe"
if ($LASTEXITCODE -eq 0) { & "$env:TEMP\str_trigger_test.exe" }
```

O binário de teste é criado em `%TEMP%`; esse teste não valida ESP-IDF, rede ou hardware.

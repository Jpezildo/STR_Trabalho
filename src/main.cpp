#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include "config.h" 


QueueHandle_t Q_PRICE;
QueueHandle_t Q_LOG;
SemaphoreHandle_t M_NET;
EventGroupHandle_t EG_SYS;

#define WIFI_UP_BIT (1 << 0)
#define TIME_OK_BIT (1 << 1)

struct LogMessage {
    char texto[128];
};

void enviarLog(const char* formato, ...) {
    LogMessage msg;
    va_list args;
    va_start(args, formato);
    vsnprintf(msg.texto, sizeof(msg.texto), formato, args);
    va_end(args);
 
    xQueueSend(Q_LOG, &msg, 0); 
}


void vTaskRegistro(void *pvParameters) {
    LogMessage msg_recebida;
    while(true) {
        if (xQueueReceive(Q_LOG, &msg_recebida, portMAX_DELAY) == pdTRUE) {
            Serial.println(msg_recebida.texto);
        }
    }
}


void vTaskGatilho(void *pvParameters) {
    float preco_atual;
    bool sistema_armado = true;

    while(true) {
        if (xQueueReceive(Q_PRICE, &preco_atual, portMAX_DELAY) == pdTRUE) {
            if (sistema_armado && preco_atual > LIMIAR_GATILHO) {
                unsigned long t0 = millis(); 
                enviarLog("[T1-GATILHO] Limiar cruzado! Preço: %.2f. Iniciando envio...", preco_atual);
                if (xSemaphoreTake(M_NET, pdMS_TO_TICKS(2000)) == pdTRUE) {
                    HTTPClient http;
                    http.begin(URL_ORDEM);
                    http.addHeader("X-MBX-APIKEY", API_KEY);
                    int httpResponseCode = http.POST("symbol=BTCUSDT&side=BUY&type=MARKET&quantity=0.01");
                    
                    unsigned long t1 = millis(); 
                    
                    if (httpResponseCode > 0) {
                        enviarLog("[T1-GATILHO] Ordem ENVIADA! Tempo de Resposta (t1-t0): %lu ms", (t1 - t0));
                    } else {
                        enviarLog("[T1-ERRO] Falha no POST da ordem. Código: %d", httpResponseCode);
                    }
                    
                    http.end();
                    xSemaphoreGive(M_NET); 
                    sistema_armado = false; 
                    enviarLog("[T1-GATILHO] Sistema bloqueado/aguardando rearme.");
                }
            }
            else if (!sistema_armado && preco_atual < (LIMIAR_GATILHO - 100)) {
                sistema_armado = true;
                enviarLog("[T1-GATILHO] Preço recuou. Gatilho REARMADO.");
            }
        }
    }
}
void vTaskConsulta(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(20); 

    while(true) {
        vTaskDelayUntil(&xLastWakeTime, xFrequency);

        EventBits_t status = xEventGroupGetBits(EG_SYS);
        if ((status & (WIFI_UP_BIT | TIME_OK_BIT)) == (WIFI_UP_BIT | TIME_OK_BIT)) {
            
            if (xSemaphoreTake(M_NET, pdMS_TO_TICKS(1000)) == pdTRUE) {
                
                HTTPClient http;
                http.begin(URL_COTACAO);
                int httpCode = http.GET();
                
                if (httpCode == HTTP_CODE_OK) {
                    String payload = http.getString();
   
                    JsonDocument doc;
                    DeserializationError error = deserializeJson(doc, payload);
                    
                    if (!error) {
                        float preco = doc["price"].as<float>();
                        enviarLog("[T2-CONSULTA] Preço atualizado: %.2f", preco);
                        
                        xQueueOverwrite(Q_PRICE, &preco);
                    }
                } else {
                    enviarLog("[T2-ERRO] Erro na consulta HTTP: %d", httpCode);
                }
                
                http.end();
                xSemaphoreGive(M_NET);
            }
        } else {
             enviarLog("[T2-CONSULTA] Aguardando Wi-Fi e NTP...");
        }
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n--- INICIANDO SISTEMA DE TEMPO REAL (ESP32) ---");

    Q_PRICE = xQueueCreate(1, sizeof(float)); 
    Q_LOG = xQueueCreate(16, sizeof(LogMessage));
    M_NET = xSemaphoreCreateMutex();
    EG_SYS = xEventGroupCreate();

    xTaskCreatePinnedToCore(vTaskRegistro, "T3_Log", 2048, NULL, 1, NULL, 1);

    enviarLog("[SISTEMA] Conectando ao Wi-Fi %s...", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    xEventGroupSetBits(EG_SYS, WIFI_UP_BIT);
    enviarLog("[SISTEMA] Wi-Fi Conectado!");

    enviarLog("[SISTEMA] Sincronizando hora NTP...");
    configTime(0, 0, "pool.ntp.org");
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
        xEventGroupSetBits(EG_SYS, TIME_OK_BIT);
        enviarLog("[SISTEMA] Hora NTP Sincronizada!");
    }

    xTaskCreatePinnedToCore(vTaskConsulta, "T2_Consulta", 8192, NULL, 2, NULL, 1);
    xTaskCreatePinnedToCore(vTaskGatilho,  "T1_Gatilho",  8192, NULL, 3, NULL, 1);
}

void loop() {

    vTaskDelete(NULL);
}
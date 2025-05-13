#include "human_face_detect.hpp"
#include "who_detect_app.hpp"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
using namespace who::cam;
using namespace who::lcd;
using namespace who::app;

#define WITH_LCD 1
#define DELAY_MS 1000

extern "C" void app_main(void)
{
#if CONFIG_HUMAN_FACE_DETECT_MODEL_IN_SDCARD
    ESP_ERROR_CHECK(bsp_sdcard_mount());
#endif

    // set log level
    esp_log_level_set("*", ESP_LOG_VERBOSE);
    ESP_LOGI("app_main", "Starting app_main");
    vTaskDelay(DELAY_MS);

    // **和串口冲突，能正确设置和使用，但会导致串口没有输出**
#if 0
#if CONFIG_IDF_TARGET_ESP32S3
    ESP_LOGI("app_main", "Initializing leds");
    vTaskDelay(DELAY_MS);
    ESP_ERROR_CHECK(bsp_leds_init());
    ESP_LOGI("app_main", "Led initialized");
    vTaskDelay(DELAY_MS);
    ESP_LOGI("app_main", "Setting led");
    vTaskDelay(DELAY_MS);
    ESP_ERROR_CHECK(bsp_led_set(BSP_LED_GREEN, true));
    ESP_LOGI("app_main", "Led set");
    vTaskDelay(DELAY_MS);
#endif
#endif

#if 1
    ESP_LOGI("app_main", "Creating cam");
    vTaskDelay(DELAY_MS);
#if CONFIG_IDF_TARGET_ESP32P4
    auto cam = new WhoP4Cam(VIDEO_PIX_FMT_RGB565, 3, V4L2_MEMORY_USERPTR, true);
    // auto cam = new WhoP4PPACam(VIDEO_PIX_FMT_RGB565, 4, V4L2_MEMORY_USERPTR, 160, 120, true);
#elif CONFIG_IDF_TARGET_ESP32S3
    auto cam = new WhoS3Cam(PIXFORMAT_RGB565, FRAMESIZE_240X240, 2, true);
#endif
    ESP_LOGI("app_main", "cam created");
    vTaskDelay(DELAY_MS);
    ESP_LOGI("app_main", "Creating lcd");
    vTaskDelay(DELAY_MS);
    auto lcd = new WhoLCD();
    ESP_LOGI("app_main", "lcd created");
    vTaskDelay(DELAY_MS);

#if WITH_LCD
    ESP_LOGI("app_main", "Creating model");
    vTaskDelay(DELAY_MS);
    auto model = new HumanFaceDetect();
    ESP_LOGI("app_main", "model created");
    vTaskDelay(DELAY_MS);
    ESP_LOGI("app_main", "Creating detect");
    vTaskDelay(DELAY_MS);
    auto detect = new WhoDetectAppLCD({{255, 0, 0}});
    ESP_LOGI("app_main", "detect created");
    vTaskDelay(DELAY_MS);
    ESP_LOGI("app_main", "Setting cam");
    vTaskDelay(DELAY_MS);
    detect->set_cam(cam);
    ESP_LOGI("app_main", "Setting model");
    vTaskDelay(DELAY_MS);
    detect->set_model(model);
    ESP_LOGI("app_main", "Setting lcd");
    vTaskDelay(DELAY_MS);
    detect->set_lcd(lcd);
    ESP_LOGI("app_main", "Setting fps");
    vTaskDelay(DELAY_MS);
    detect->set_fps(5);
    ESP_LOGI("app_main", "Running");
    vTaskDelay(DELAY_MS);
    detect->run();
    ESP_LOGI("app_main", "Running done");
    vTaskDelay(DELAY_MS);
#else
    ESP_LOGI("app_main", "Creating model");
    vTaskDelay(DELAY_MS);
    auto model = new HumanFaceDetect();
    ESP_LOGI("app_main", "model created");
    vTaskDelay(DELAY_MS);
    ESP_LOGI("app_main", "Creating detect");
    vTaskDelay(DELAY_MS);
    auto detect = new WhoDetectAppTerm();
    ESP_LOGI("app_main", "detect created");
    vTaskDelay(DELAY_MS);
    ESP_LOGI("app_main", "Setting cam");
    vTaskDelay(DELAY_MS);
    detect->set_cam(cam);
    ESP_LOGI("app_main", "Setting model");
    vTaskDelay(DELAY_MS);
    detect->set_model(model);
    ESP_LOGI("app_main", "Setting fps");
    vTaskDelay(DELAY_MS);
    detect->set_fps(5);
    ESP_LOGI("app_main", "Running");
    vTaskDelay(DELAY_MS);
    detect->run();
    ESP_LOGI("app_main", "Running done");
    vTaskDelay(DELAY_MS);
#endif
#endif

    for(;;) {
        static bool led_on = false;
        led_on = !led_on;
        bsp_led_set(BSP_LED_GREEN, led_on);
        ESP_LOGI("app_main", "Looping...");
        vTaskDelay(DELAY_MS);
    }
}

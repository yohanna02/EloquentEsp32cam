// 15_Motion_Localization_Live_Feed.ino

#define MAX_RESOLUTION_VGA

#include "esp32cam.h"
#include "esp32cam/JpegDecoder.h"
#include "esp32cam/motion/Detector.h"
#include "esp32cam/motion/SimpleChange.h"
#include "esp32cam/motion/localization/SlidingWindow.h"
#include "esp32cam/http/MotionLiveFeed.h"

// Replace with your WiFi credentials
#define WIFI_SSID "Abc"
#define WIFI_PASS "12345678"


Eloquent::Esp32cam::Cam cam;
Eloquent::Esp32cam::JpegDecoder decoder;
Eloquent::Esp32cam::Motion::SimpleChange algorithm;
Eloquent::Esp32cam::Motion::Detector detector(algorithm);
Eloquent::Esp32cam::Motion::Localization::SlidingWindow localizer;
Eloquent::Esp32cam::Http::MotionLiveFeed feed(cam, detector, 80);


void setup() {
    Serial.begin(115200);
    delay(3000);
    Serial.println("Init");

    cam.aithinker();
    cam.highQuality();
    cam.vga();
    cam.highestSaturation();
    cam.disableAutomaticWhiteBalance();
    cam.disableAutomaticExposureControl();
    cam.disableGainControl();

    // See 10_Image_Wise_Motion_Detection for details
    detector.trainFor(30);
    detector.triggerAbove(0.2);
    detector.retrainAfter(33ULL * 600);
    detector.denoise();

    algorithm.differBy(20);
    algorithm.smooth(0.9);
    algorithm.onlyUpdateBackground();

    while (!cam.begin())
        Serial.println(cam.getErrorMessage());

    while (!cam.connect(WIFI_SSID, WIFI_PASS))
        Serial.println(cam.getErrorMessage());

    while (!feed.begin())
        Serial.println(feed.getErrorMessage());

    /**
     * Display localization info on top of stream
     */
    localizer.setMinCoverage(0.7);
    localizer.earlyStop();
    feed.localizeWith(localizer);

    cam.viewAt("esp32cam");
    Serial.println(feed.getWelcomeMessage());
}


void loop() {
    feed.handle();
}
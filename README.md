# Arduino-Smart-Security-System
IR-controlled autonomous vehicle security and lighting project using Arduino.
# 🚨 Arduino Akıllı Güvenlik ve Otonom Aydınlatma Sistemi

Bu proje; kızılötesi kumanda, ultrasonik radar ve ışık sensörleri entegrasyonu ile çalışan, `millis()` tabanlı eşzamanlı görev yönetim sistemine sahip bir akıllı güvenlik projesidir.

## 🛠️ Donanım Bileşenleri
* Arduino Uno / Nano
* HC-SR04 Ultrasonik Mesafe Sensörü (Radar)
* IR Alıcı ve Kumanda (Kızılötesi Kontrol)
* LDR (Işık Sensörü)
* Kırmızı/Mavi LED (Polis Çakarı Modülü) & Buzzer

## 🚀 Öne Çıkan Yazılımsal Özellikler
* **Non-Blocking Architecture:** `delay()` kullanılmadan `millis()` ve `micros()` ile zamanlayıcı tabanlı multi-tasking mimarisi kurulmuştur.
* **Otonom Far Modu:** LDR verisine göre ışık seviyesi düştüğünde farları otomatik açar.
* **Otonom Nöbetçi Modu:** Sistem kilitliyken 15 cm yakınına bir engel girdiğinde otomatik olarak polis sireni ve çakarlı alarmı tetikler.
* **Öncelik Algoritması:** Modlar arası çakışmayı önleyen durum yönetim mantığı (State Machine) uygulanmıştır.

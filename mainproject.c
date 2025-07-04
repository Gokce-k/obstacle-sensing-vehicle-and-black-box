// ? PIC16F877A + LCD + DS3231 RTC + HC-SR04 + PIR + Motor + Kaza Durumu LCD Yazdýrma
// ? Derleyici: CCS C Compiler


#include <16F877A.h>
#fuses XT, NOWDT, NOLVP, PUT
#use delay(clock=4000000)
#use i2c(master, sda=PIN_C4, scl=PIN_C3, slow=100000)
#include <lcd.c>

// ------------------- BCD Dönüþümleri ----------------------
int8 bcd_to_dec(int8 bcd) { return ((bcd >> 4) * 10 + (bcd & 0x0F)); }
int8 dec_to_bcd(int8 dec) { return (((dec / 10) << 4) + (dec % 10)); }

// ------------------- DS3231 Fonksiyonlarý ----------------------
void rtc_init() {
   i2c_start(); i2c_write(0xD0); i2c_write(0x0E); i2c_write(0x00); i2c_stop();
}

void rtc_get_time(int8 *h, int8 *m, int8 *s) {
   i2c_start(); i2c_write(0xD0); i2c_write(0x00); i2c_start(); i2c_write(0xD1);
   *s = bcd_to_dec(i2c_read(1));
   *m = bcd_to_dec(i2c_read(1));
   *h = bcd_to_dec(i2c_read(0));
   i2c_stop();
}

// ------------------- HC-SR04 Ultrasonik ----------------------
long measure_distance_cm() {
   int16 duration;
   output_low(PIN_E0);
   delay_us(2);
   output_high(PIN_E0);  // Trigger
   delay_us(10);
   output_low(PIN_E0);

   while(!input(PIN_E1));  // Echo bekle
   set_timer1(0);
   while(input(PIN_E1));   // Yükseklik devam ederken saya
   duration = get_timer1();

   return duration / 58;  // cm cinsinden mesafe
}

// ------------------- PIR Hareket ----------------------
int1 hareket_var_mi() {
   return input(PIN_B1);
}

// ------------------- Motor Kontrol (L293D) ----------------------
void ileri_git() {
   output_high(PIN_D0); output_low(PIN_D1); // Motor1
   output_high(PIN_D2); output_low(PIN_D3); // Motor2
}

void dur() {
   output_low(PIN_D0); output_low(PIN_D1);
   output_low(PIN_D2); output_low(PIN_D3);
}

// ------------------- Ana Kod ----------------------
void main() {
   lcd_init();
   rtc_init();
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_2);

   output_low(PIN_D0); output_low(PIN_D1);
   output_low(PIN_D2); output_low(PIN_D3);

   lcd_putc("\fArac Sistemi\nBaslatiliyor...");
   delay_ms(1500);

   while(TRUE) {
      long mesafe = measure_distance_cm();
      int1 hareket = hareket_var_mi();
      int8 h, m, s;
      rtc_get_time(&h, &m, &s);

      if(mesafe < 15) {
         dur();
         lcd_putc("\fENGEL ALGILANDI!");
         printf(lcd_putc, "\n%02u:%02u:%02u MES:%lu", h, m, s, mesafe);
         delay_ms(3000);
      }
      else if(hareket) {
         ileri_git();
         lcd_putc("\fHAREKET VAR\nIleri Gidiyor...");
      }
      else {
         dur();
         lcd_putc("\fHAREKET YOK\nBeklemede...");
      }

      delay_ms(200);
   }
}


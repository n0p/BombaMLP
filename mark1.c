#include "mark1.h"


#define LCD_TYPE 2;
#include <lcd.c>
#include <string.h>

#define LED_VERDE PIN_D3
#define LED_ROJO PIN_C3
#define PIEZO PIN_C4

long cuentas;

char cuenta[18];

void armar();
void detona();

void main()
{

   lcd_init();

   setup_adc_ports(NO_ANALOGS);
   setup_adc(ADC_OFF);
   setup_psp(PSP_DISABLED);
   setup_spi(SPI_SS_DISABLED);
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_256);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);

   output_low(PIEZO);
   output_low(LED_ROJO);
   output_high(LED_VERDE);
   delay_ms(100);
   
   lcd_putc('\f');
   lcd_putc("    SISTEMA\n    ACTIVADO");
   
   while (1)
   {
      delay_ms(100);
      output_toggle(LED_ROJO);
      if( !input(PIN_A0) ) {
         delay_ms(500);
         if (!input(PIN_A0))
            break;
      }
   }
   
   output_low(LED_ROJO);
   lcd_putc('\f');
   lcd_putc("    DETONADOR\n     ARMADO");
                                                     
   while (1)
   {
      delay_ms(200);
      output_toggle(LED_VERDE);
      if( input(PIN_A0) ) {
         delay_ms(200); // para falsos positivos
         if (input(PIN_A0))
            armar();
      }
   }

}

int segundos;

void armar()
{
   output_high(LED_VERDE);
   output_high(LED_ROJO);
   lcd_putc("\f  CUENTA ATRAS:\n");
   for (cuentas=9900;cuentas>0;cuentas--)
   {
      lcd_gotoxy(4,2);
      //lcd_putc(48+(cuentas/10000)%10);
      lcd_putc(48+(cuentas/1000)%10);
      lcd_putc(48+(cuentas/100)%10);
      lcd_putc(".");
      lcd_putc(48+(cuentas/10)%10);
      lcd_putc(48+cuentas%10);
      delay_ms(9);
      if (!(cuentas%(cuentas>1000?100:(cuentas>300?50:25)))) {
         output_toggle(LED_ROJO);
         output_toggle(PIEZO);
         delay_ms(100);
         output_toggle(PIEZO);
         cuentas-=10;
      }
   }
   detona();
}

void detona()
{
   lcd_putc("\f");
   lcd_putc("  WTF BOOOOOM!  \n");
   lcd_putc("  (has muerto)  ");
   while (1) {
      delay_ms(50);
      output_toggle(LED_VERDE);
      output_toggle(LED_ROJO);
      output_toggle(PIEZO);
   }
}


#include "ch.h"
#include "hal.h"

// UART routines and definitions.

static VirtualTimer vt1, vt2;

static void restart(void *p) {

  (void)p;
  uartStartSendI(&UARTD2, 14, "Hello World!\r\n");
}

static void ledoff(void *p) {

  (void)p;
  palSetPad(IOPORT3, GPIOC_LED);
}

/*
 * This callback is invoked when a transmission buffer has been completely
 * read by the driver.
 */
static void txend1(UARTDriver *uartp) {

  (void)uartp;
  palClearPad(IOPORT3, GPIOC_LED);
}

/*
 * This callback is invoked when a transmission has physically completed.
 */
static void txend2(UARTDriver *uartp) {

  (void)uartp;
  palSetPad(IOPORT3, GPIOC_LED);
  chSysLockFromIsr();
  if (chVTIsArmedI(&vt1))
    chVTResetI(&vt1);
  chVTSetI(&vt1, MS2ST(5000), restart, NULL);
  chSysUnlockFromIsr();
}

/*
 * This callback is invoked on a receive error, the errors mask is passed
 * as parameter.
 */
static void rxerr(UARTDriver *uartp, uartflags_t e) {

  (void)uartp;
  (void)e;
}

/*
 * This callback is invoked when a character is received but the application
 * was not ready to receive it, the character is passed as parameter.
 */
static void rxchar(UARTDriver *uartp, uint16_t c) {

  (void)uartp;
  (void)c;
  /* Flashing the LED each time a character is received.*/
  palClearPad(IOPORT3, GPIOC_LED);
  chSysLockFromIsr();
  if (chVTIsArmedI(&vt2))
    chVTResetI(&vt2);
  chVTSetI(&vt2, MS2ST(200), ledoff, NULL);
  chSysUnlockFromIsr();
}

/*
 * This callback is invoked when a receive buffer has been completely written.
 */
static void rxend(UARTDriver *uartp) {

  (void)uartp;
}

/*
 * UART driver configuration structure.
 */
static UARTConfig uart_cfg_1 = {
  txend1,
  txend2,
  rxend,
  rxchar,
  rxerr,
  38400,
  0,
  USART_CR2_LINEN,
  0
};

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Activates the serial driver 2 using the driver default configuration.
   */
  uartStart(&UARTD2, &uart_cfg_1);

  /*
   * Starts the transmission, it will be handled entirely in background.
   */
  uartStartSend(&UARTD2, 13, "Starting...\r\n");

  /*
   * Normal main() thread activity, in this demo it does nothing.
   */
  while ( TRUE ) {
    chThdSleepMilliseconds(500);
  }
  return 0;
}






static WORKING_AREA( waGps, 512 );
static msg_t gps( void *arg )
{
    (void)arg;
    chRegSetThreadName( "ua" );
    while (TRUE)
    {
        static const uint32_t BUFFER_SZ = 128;
        static inBuffer[ BUFFER_SZ ], 
               outBuffer[ BUFFER_SZ ];
        static uint16_t inIndex;
        inIndex = 0;
        // IO with USB serial driver.
        while ( TRUE ) 
        {
	    

        }
    }
    return 0;
}

void gpsInit( void )
{
    uartStart( &UARTD1, &uart_cfg_1 );
    chThdCreateStatic( waGps, sizeof(waGps), NORMALPRIO, gps, NULL );
}



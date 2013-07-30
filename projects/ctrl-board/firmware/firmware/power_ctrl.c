
#include "power_ctrl.h"
#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "led_ctrl.h"
#include "light_ctrl.h"
#include "moto_ctrl.h"
#include "i2c_slave_ctrl.h"
#include "conv_ctrl.h"

// #include <stdlib.h>

#define JUST_PS_PORT       GPIOB
#define JUST_PS_PIN        15

static Mutex mutex;
static int idleTime      = ( 3 * 60 * 60 );
static int activeTime    = ( 3 * 60 );
static int timeout       = ( 3 * 60 );

inline uint8_t justPs( void );
static void    powerOn( void );
static void    powerOff( void );

void powerConfig( int aactiveTime, int aidleTime )
{
    chMtxLock( &mutex );
        idleTime   = aidleTime;
        activeTime = aactiveTime;
    chMtxUnlock();
}

void powerOffReset( void )
{
    chMtxLock( &mutex );
        timeout = activeTime;
    chMtxUnlock();
}

static void setPower( bool_t en )
{
   if ( !en )
   {
       setLight( 0 );
       motoSet( 0 );
       motoSetEn( 0 );
       setLed( 0 );
   }
   if ( !en )
       stopI2cSlave();
   convSetBuckEn( en ? 1 : 0 );
   if ( en )
   {
       // Wait for some time and turn I2C on after that.
       chThdSleepMilliseconds( 15000 );
       startI2cSlave();
   }
}

void powerHandler( void )
{
    chRegSetThreadName( "p" );
    // First wait for power on.
    if ( justPs() )
        powerOn();
    while ( 1 )
    {
        powerOff();
        powerOn();
    }
}

void initPower( void )
{
    // Starting power converters.
    convStart();

    // Just power source mode check pin.
    palSetPadMode( JUST_PS_PORT, JUST_PS_PIN,  PAL_MODE_INPUT );

    chMtxInit( &mutex );
    setPower( 0 );
}

inline uint8_t justPs( void )
{
    if ( palReadPad( JUST_PS_PORT, JUST_PS_PIN ) )
        return 0;
    return 1;
}

static void powerOn( void )
{
    // The very first power on hangs Raspberry Pi up if I2C cable is connected.
    // Trying to overcome this problem
    chThdSleepMilliseconds( 5000 );

    // Power on.
    setPower( 1 );
    // Wait for next power off.
    chMtxLock( &mutex );
        timeout = activeTime;
    chMtxUnlock();
    while ( 1 )
    {
        chThdSleepSeconds( 1 );
        if ( !justPs() )
        {
            chMtxLock( &mutex );
                int t = timeout;
            chMtxUnlock();
            t--;
            chMtxLock( &mutex );
                timeout = t;
            chMtxUnlock();
            if ( t<=0 )
                break;
        }
    }
}

static void powerOff( void )
{
    // Power off.
    setPower( 0 );
    // Wait for next power off.
    chMtxLock( &mutex );
        timeout = idleTime;
    chMtxUnlock();
    while ( 1 )
    {
        chThdSleepSeconds( 1 );
        chMtxLock( &mutex );
            int t = timeout;
        chMtxUnlock();
        t--;
        chMtxLock( &mutex );
            timeout = t;
        chMtxUnlock();
        if ( t<=0 )
            break;
    }
}







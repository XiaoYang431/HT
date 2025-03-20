#include "delay.h"
 
void Delay_ms(u32 ms)
{
    u32 i;
    
    /* SYSTICK configuration */
    SYSTICK_ClockSourceConfig(SYSTICK_SRC_STCLK);       // ????????????
    SYSTICK_SetReloadValue(SystemCoreClock / 8 / 1000); // (CK_SYS/8/1000) = 1ms 
    SYSTICK_IntConfig(DISABLE);                         // ?????
 
    /* ??SysTick??? */
    SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);
    SYSTICK_CounterCmd(SYSTICK_COUNTER_ENABLE);
    
    for( i = 0;i < ms;i++ )
    {
        while( !( (SysTick->CTRL) & (1<<16) ) ); 
    }
 
    /* ??SysTick??? */
    SYSTICK_CounterCmd(SYSTICK_COUNTER_DISABLE);
    /* ??SysTick??? */
    SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);
}
 
void Delay_us(u32 us)
{
    u32 i;
    
    /* SYSTICK configuration */
    SYSTICK_ClockSourceConfig(SYSTICK_SRC_STCLK);       // ????????????
    SYSTICK_SetReloadValue(SystemCoreClock / 8 / 1000000); // (CK_SYS/8/1000000) = 1us 
    SYSTICK_IntConfig(DISABLE);                         // ?????
 
    /* ??SysTick??? */
    SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);
    SYSTICK_CounterCmd(SYSTICK_COUNTER_ENABLE);
 
    for( i = 0;i < us;i++ )
    {
        while( !( (SysTick->CTRL) & (1<<16) ) ); 
    }
 
    /* ??SysTick??? */
    SYSTICK_CounterCmd(SYSTICK_COUNTER_DISABLE);
    /* ??SysTick??? */
    SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);
}

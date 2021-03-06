#include <stdbool.h>
#include <mk20dx128.h>
#include "hal_ftm.h"
#include "stm32hal.h"

#define HAL_MAX_DELAY 0xffffffff

#define MP_HAL_UNIQUE_ID_ADDRESS (0x40048058)

#ifdef  USE_FULL_ASSERT
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

//#define HAL_NVIC_EnableIRQ(irq)    NVIC_ENABLE_IRQ(irq)

#define GPIOA   ((GPIO_TypeDef *)&GPIOA_PDOR)
#define GPIOB   ((GPIO_TypeDef *)&GPIOB_PDOR)
#define GPIOC   ((GPIO_TypeDef *)&GPIOC_PDOR)
#define GPIOD   ((GPIO_TypeDef *)&GPIOD_PDOR)
#define GPIOE   ((GPIO_TypeDef *)&GPIOE_PDOR)
#define GPIOZ   ((GPIO_TypeDef *)NULL)

#define I2C0    ((I2C_TypeDef *)&KINETIS_I2C0)
#define I2C1    ((I2C_TypeDef *)&KINETIS_I2C1)
#define I2C2    ((I2C_TypeDef *)&KINETIS_I2C2)
#define I2C3    ((I2C_TypeDef *)&KINETIS_I2C3)

#undef  SPI0
#define SPI0    ((SPI_TypeDef *)&KINETISK_SPI0)
#define SPI1    ((SPI_TypeDef *)&KINETISK_SPI1)
#define SPI2    ((SPI_TypeDef *)&KINETISK_SPI2)

#define UART0   ((UART_TypeDef *)&UART0_BDH)
#define UART1   ((UART_TypeDef *)&UART1_BDH)
#define UART2   ((UART_TypeDef *)&UART2_BDH)
#define UART3   ((UART_TypeDef *)&UART3_BDH)
#define UART4   ((UART_TypeDef *)&UART4_BDH)
#define UART5   ((UART_TypeDef *)&UART5_BDH)

typedef struct {
    volatile    uint32_t    SC1A;   // 00
    volatile    uint32_t    SC1B;   // 04
    volatile    uint32_t    CFG1;   // 08
    volatile    uint32_t    CFG2;   // 0c
    volatile    uint32_t    RA;     // 10
    volatile    uint32_t    RB;     // 14
    volatile    uint32_t    CV1;    // 18
    volatile    uint32_t    CV2;    // 1c
    volatile    uint32_t    SC2;    // 20
    volatile    uint32_t    SC3;    // 24
    volatile    uint32_t    OFS;    // 28
    volatile    uint32_t    PG;     // 2c
    volatile    uint32_t    MG;     // 30
    volatile    uint32_t    CLPD;   // 34
    volatile    uint32_t    CLPS;   // 38
    volatile    uint32_t    CLP4;   // 3c
    volatile    uint32_t    CLP3;   // 40
    volatile    uint32_t    CLP2;   // 44
    volatile    uint32_t    CLP1;   // 48
    volatile    uint32_t    CLP0;   // 4c
    volatile    uint32_t    PGA;    // 50
    volatile    uint32_t    CLMD;   // 54
    volatile    uint32_t    CLMS;   // 58
    volatile    uint32_t    CLM4;   // 5c
    volatile    uint32_t    CLM3;   // 60
    volatile    uint32_t    CLM2;   // 64
    volatile    uint32_t    CLM1;   // 68
    volatile    uint32_t    CLM0;   // 6c

} ADC_TypeDef;

#define ADC0    ((ADC_TypeDef *)&ADC0_SC1A)
#define ADC1    ((ADC_TypeDef *)&ADC1_SC1A)

typedef struct {
    uint32_t dummy;
} I2C_TypeDef;

typedef struct {
    uint32_t dummy;
} UART_TypeDef;

typedef struct {
    uint32_t dummy;
} SPI_TypeDef;

typedef struct {
    volatile    uint32_t    PDOR;   // Output register
    volatile    uint32_t    PSOR;   // Set output register
    volatile    uint32_t    PCOR;   // Clear output register
    volatile    uint32_t    PTOR;   // Toggle output register
    volatile    uint32_t    PDIR;   // Data Input register
    volatile    uint32_t    PDDR;   // Data Direction register
} GPIO_TypeDef;

#define GPIO_OUTPUT_TYPE    ((uint32_t)0x00000010)  // Indicates OD

#define GPIO_MODE_INPUT     ((uint32_t)0x00000000)
#define GPIO_MODE_OUTPUT_PP ((uint32_t)0x00000001)
#define GPIO_MODE_OUTPUT_OD ((uint32_t)0x00000011)
#define GPIO_MODE_AF_PP     ((uint32_t)0x00000002)
#define GPIO_MODE_AF_OD     ((uint32_t)0x00000012)
#define GPIO_MODE_ANALOG    ((uint32_t)0x00000003)

#define IS_GPIO_MODE(MODE) (((MODE) == GPIO_MODE_INPUT)              ||\
                            ((MODE) == GPIO_MODE_OUTPUT_PP)          ||\
                            ((MODE) == GPIO_MODE_OUTPUT_OD)          ||\
                            ((MODE) == GPIO_MODE_AF_PP)              ||\
                            ((MODE) == GPIO_MODE_AF_OD)              ||\
                            ((MODE) == GPIO_MODE_ANALOG))

#define GPIO_NOPULL         ((uint32_t)0)
#define GPIO_PULLUP         ((uint32_t)1)
#define GPIO_PULLDOWN       ((uint32_t)2)

#define IS_GPIO_PULL(PULL) (((PULL) == GPIO_NOPULL) || ((PULL) == GPIO_PULLUP) || \
                            ((PULL) == GPIO_PULLDOWN))

#define  GPIO_SPEED_LOW         ((uint32_t)0)
#define  GPIO_SPEED_MEDIUM      ((uint32_t)1)
#define  GPIO_SPEED_FAST        ((uint32_t)2)
#define  GPIO_SPEED_HIGH        ((uint32_t)3)

#define IS_GPIO_AF(af)      ((af) >= 0 && (af) <= 7)

typedef struct {
    uint32_t    Pin;
    uint32_t    Mode;
    uint32_t    Pull;
    uint32_t    Speed;
    uint32_t    Alternate;
} GPIO_InitTypeDef;

#define GPIO_PORT_TO_PORT_NUM(GPIOx) \
    ((&GPIOx->PDOR - &GPIOA_PDOR) / (&GPIOB_PDOR - &GPIOA_PDOR))

#define GPIO_PIN_TO_PORT_PCR(GPIOx, pin) \
    (&PORTA_PCR0 + (GPIO_PORT_TO_PORT_NUM(GPIOx) * 0x400) + (pin))

#define GPIO_AF0_ADC0   0
#define GPIO_AF0_ADC1   0
#define GPIO_AF2_I2C0   2
#define GPIO_AF2_I2C1   2
#define GPIO_AF2_I2C3   2
#define GPIO_AF2_SPI0   2
#define GPIO_AF2_SPI1   2
#define GPIO_AF2_SPI2   2
#define GPIO_AF3_FTM0   3
#define GPIO_AF3_FTM1   3
#define GPIO_AF3_FTM2   3
#define GPIO_AF3_FTM3   3
#define GPIO_AF3_UART0  3
#define GPIO_AF3_UART1  3
#define GPIO_AF3_UART2  3
#define GPIO_AF3_UART3  3
#define GPIO_AF3_UART4  3
#define GPIO_AF3_UART5  3
#define GPIO_AF4_FTM0   4
#define GPIO_AF4_FTM3   4
#define GPIO_AF5_I2C0   5
#define GPIO_AF5_I2C2   5
#define GPIO_AF6_FTM1   6
#define GPIO_AF6_FTM2   6
#define GPIO_AF6_FTM3   6
#define GPIO_AF6_I2C1   6
#define GPIO_AF7_FTM0   7
#define GPIO_AF7_FTM1   7
#define GPIO_AF7_I2C0   7
#define GPIO_AF7_SPI1   7


__attribute__(( always_inline )) static inline void __WFI(void) {
  __asm volatile ("wfi");
}

void mp_hal_set_interrupt_char(int c);

#if 1
// Currently, the ResetHandler function in mk20dx128.c enables all of the
// clocks for all of the PORTs so we don't need to do anything here.
#define mp_hal_gpio_clock_enable(gpio)
#endif

struct pin_obj_s;
bool mp_hal_gpio_set_af(const struct pin_obj_s *pin, GPIO_InitTypeDef *init, uint8_t fn, uint8_t unit);

void HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *init);

struct _pin_obj_t;
#define mp_hal_pin_obj_t const struct _pin_obj_t*
#define mp_hal_pin_high(p) (((p)->gpio->PSOR) = (p)->pin_mask)
#define mp_hal_pin_low(p)  (((p)->gpio->PCOR) = (p)->pin_mask)
#define mp_hal_pin_read(p) (((p)->gpio->PDIR >> (p)->pin) & 1)
#define mp_hal_pin_write(p, v)  do { if (v) { mp_hal_pin_high(p); } else { mp_hal_pin_low(p); } } while (0)

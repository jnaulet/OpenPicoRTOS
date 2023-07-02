#include "clock-stm32h7xx.h"
#include "picoRTOS.h"

#include <stdint.h>

struct RCC {
    volatile uint32_t CR;
    volatile uint32_t ICSR_HSICFGR; /* rev Y or V */
    volatile uint32_t CRRCR;
    volatile uint32_t CSICFGR;
    volatile uint32_t CFGR;
    uint32_t RESERVED0;
    volatile uint32_t D1CFGR;
    volatile uint32_t D2CFGR;
    volatile uint32_t D3CFGR;
    uint32_t RESERVED1;
    volatile uint32_t PLLCKSELR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t PLL1DIVR;
    volatile uint32_t PLL1FRACR;
    volatile uint32_t PLL2DIVR;
    volatile uint32_t PLL2FRACR;
    volatile uint32_t PLL3DIVR;
    volatile uint32_t PLL3FRACR;
    uint32_t RESERVED2;
    volatile uint32_t D1CCIPR;
    volatile uint32_t D2CCIP1R;
    volatile uint32_t D2CCIP2R;
    volatile uint32_t D3CCIPR;
    uint32_t RESERVED3;
    volatile uint32_t CIER;
    volatile uint32_t CIFR;
    volatile uint32_t CICR;
    uint32_t RESERVED4;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
    uint32_t RESERVED5;
    volatile uint32_t AHB3RSTR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB4RSTR;
    volatile uint32_t APB3RSTR;
    volatile uint32_t APB1LRSTR;
    volatile uint32_t APB1HRSTR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t APB4RSTR;
    volatile uint32_t GCR;
    uint32_t RESERVED6;
    volatile uint32_t D3AMR;
    uint32_t RESERVED7[9];
    volatile uint32_t RSR;
    volatile uint32_t AHB3ENR;
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    volatile uint32_t AHB4ENR;
    volatile uint32_t APB3ENR;
    volatile uint32_t APB1LENR;
    volatile uint32_t APB1HENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t APB4ENR;
    uint32_t RESERVED8;
    volatile uint32_t AHB3LPENR;
    volatile uint32_t AHB1LPENR;
    volatile uint32_t AHB2LPENR;
    volatile uint32_t AHB4LPENR;
    volatile uint32_t APB3LPENR;
    volatile uint32_t APB1LLPENR;
    volatile uint32_t APB1HLPENR;
    volatile uint32_t APB2LPENR;
    volatile uint32_t APB4LPENR;
};

#define CR_PLL3RDY   (1 << 29)
#define CR_PLL3ON    (1 << 28)
#define CR_PLL2RDY   (1 << 27)
#define CR_PLL2ON    (1 << 26)
#define CR_PLL1RDY   (1 << 25)
#define CR_PLL1ON    (1 << 24)
#define CR_HSECSSON  (1 << 19)
#define CR_HSEBYP    (1 << 18)
#define CR_HSERDY    (1 << 17)
#define CR_HSEON     (1 << 16)
#define CR_D2CKRDY   (1 << 15)
#define CR_D1CKRDY   (1 << 14)
#define CR_HSI48RDY  (1 << 13)
#define CR_HSI48ON   (1 << 12)
#define CR_CSIKERON  (1 << 9)
#define CR_CSIRDY    (1 << 8)
#define CR_CSION     (1 << 7)
#define CR_HSIDIVF   (1 << 5)
#define CR_HSIDIV_M  0x3u
#define CR_HSIDIV(x) (((x) & CR_HSIDIV_M) << 3)
#define CR_HSIRDY    (1 << 2)
#define CR_HSIKERON  (1 << 1)
#define CR_HSION     (1 << 0)

#define ICSR_CSITRIM_M  0x1fu
#define ICSR_CSITRIM(x) (((x) & ICSR_CSITRIM_M) << 26)
#define ICSR_CSICAL_M   0xffu
#define ICSR_CSICAL(x)  (((x) & ICSR_CSICAL_M) << 18)
#define ICSR_HSITRIM_M  0x3fu
#define ICSR_HSITRIM(x) (((x) & ICSR_HSITRIM_M) << 12)
#define ICSR_HSICAL_M   0xfffu
#define ICSR_HSICAL(x)  ((x) & ICSR_HSICAL_M)

#define HSICFGR_HSITRIM_M  0x3fu
#define HSICFGR_HSITRIM(x) (((x) & HSICFGR_HSITRIM_M) << 24)
#define HSICFGR_HSICAL_M   0xfffu
#define HSICFGR_HSICAL(x)  ((x) & HSICFGR_HSICAL_M)

#define CRRCR_HSI48CAL_M  0x3ffu
#define CRRCR_HSI48CAL(x) ((x) & CRRCR_HSI48CAL_M)

#define CSICFGR_CSITRIM_M  0x3fu
#define CSICFGR_CSITRIM(x) (((x) & CSICFGR_CSITRIM_M) << 24)
#define CSICFGR_CSICAL_M   0x3ffu
#define CSICFGR_CSICAL(x)  ((x) & CSICFGR_CSICAL_M)

#define CFGR_MCO2_M      0x7u
#define CFGR_MCO2(x)     (((x) & CFGR_MCO2_M) << 29)
#define CFGR_MCO2PRE_M   0xfu
#define CFGR_MCO2PRE(x)  (((x) & CFGR_MCO2PRE_M) << 25)
#define CFGR_MCO1_M      0x7u
#define CFGR_MCO1(x)     (((x) & CFGR_MCO1_M) << 22)
#define CFGR_MCO1PRE_M   0xfu
#define CFGR_MCO1PRE(x)  (((x) & CFGR_MCO1PRE_M) << 18)
#define CFGR_TIMPRE      (1 << 15)
#define CFGR_HRTIMSEL    (1 << 14)
#define CFGR_RTCPRE_M    0x3fu
#define CFGR_RTCPRE(x)   (((x) & CFGR_RTCPRE_M) << 8)
#define CFGR_STOPKERWUCK (1 << 7)
#define CFGR_STOPWUCK    (1 << 6)
#define CFGR_SWS_M       0x7u
#define CFGR_SWS(x)      (((x) & CFGR_SWS_M) << 3)
#define CFGR_SW_M        0x7u
#define CFGR_SW(x)       ((x) & CFGR_SW_M)

#define D1CFGR_D1CPRE_M  0xfu
#define D1CFGR_D1CPRE(x) (((x) & D1CFGR_D1CPRE_M) << 8)
#define D1CFGR_D1PPRE_M  0x7u
#define D1CFGR_D1PPRE(x) (((x) & D1CFGR_D1PPRE_M) << 4)
#define D1CFGR_HPRE_M    0xfu
#define D1CFGR_HPRE(x)   ((x) & D1CFGR_HPRE_M)

#define D2CFGR_D2PPRE2_M  0x7u
#define D2CFGR_D2PPRE2(x) (((x) & D2CFGR_D2PPRE2_M) << 8)
#define D2CFGR_D2PPRE1_M  0x7u
#define D2CFGR_D2PPRE1(x) (((x) & D2CFGR_D2PPRE1_M) << 4)

#define D2CFGR_D3PPRE_M  0x7u
#define D2CFGR_D3PPRE(x) (((x) & D2CFGR_D3PPRE_M) << 4)

#define PLLCKSELR_DIVM3_M   0x3fu
#define PLLCKSELR_DIVM3(x)  (((x) & PLLCKSELR_DIVM3_M) << 20)
#define PLLCKSELR_DIVM2_M   0x3fu
#define PLLCKSELR_DIVM2(x)  (((x) & PLLCKSELR_DIVM2_M) << 12)
#define PLLCKSELR_DIVM1_M   0x3fu
#define PLLCKSELR_DIVM1(x)  (((x) & PLLCKSELR_DIVM1_M) << 4)
#define PLLCKSELR_PLLSRC_M  0x3u
#define PLLCKSELR_PLLSRC(x) ((x) & PLLCKSELR_PLLSRC_M)

#define PLLCFGR_DIVR3EN    (1 << 24)
#define PLLCFGR_DIVQ3EN    (1 << 23)
#define PLLCFGR_DIVP3EN    (1 << 22)
#define PLLCFGR_DIVR2EN    (1 << 21)
#define PLLCFGR_DIVQ2EN    (1 << 20)
#define PLLCFGR_DIVP2EN    (1 << 19)
#define PLLCFGR_DIVR1EN    (1 << 18)
#define PLLCFGR_DIVQ1EN    (1 << 17)
#define PLLCFGR_DIVP1EN    (1 << 16)
#define PLLCFGR_PLL3RGE_M  0x3u
#define PLLCFGR_PLL3RGE(x) (((x) & PLLCFGR_PLL3RGE_M) << 10)
#define PLLCFGR_PLL3VCOSEL (1 << 9)
#define PLLCFGR_PLL3FRACEN (1 << 8)
#define PLLCFGR_PLL2RGE_M  0x3u
#define PLLCFGR_PLL2RGE(x) (((x) & PLLCFGR_PLL2RGE_M) << 6)
#define PLLCFGR_PLL2VCOSEL (1 << 5)
#define PLLCFGR_PLL2FRACEN (1 << 4)
#define PLLCFGR_PLL1RGE_M  0x3u
#define PLLCFGR_PLL1RGE(x) (((x) & PLLCFGR_PLL1RGE_M) << 2)
#define PLLCFGR_PLL1VCOSEL (1 << 1)
#define PLLCFGR_PLL1FRACEN (1 << 0)

#define PLLnDIVR_DIVRn_M  0x3fu
#define PLLnDIVR_DIVRn(x) (((x) & PLLnDIVR_DIVRn_M) << 24)
#define PLLnDIVR_DIVQn_M  0x3fu
#define PLLnDIVR_DIVQn(x) (((x) & PLLnDIVR_DIVQn_M) << 16)
#define PLLnDIVR_DIVPn_M  0x3fu
#define PLLnDIVR_DIVPn(x) (((x) & PLLnDIVR_DIVPn_M) << 9)
#define PLLnDIVR_DIVNn_M  0x1ffu
#define PLLnDIVR_DIVNn(x) ((x) & PLLnDIVR_DIVNn_M)

#define PLLnFRACR_FRACNn_M  0x1fffu
#define PLLnFRACR_FRACNn(x) (((x) & PLL1FRACR_FRACNn_M) << 3)

#define D1CCIPR_CKPERSEL_M  0x3u
#define D1CCIPR_CKPERSEL(x) (((x) & D1CCIPR_CKPERSEL_M) << 28)
#define D1CCIPR_SDMMCSEL    (1 << 16)
#define D1CCIPR_QSPISEL_M   0x3u
#define D1CCIPR_QSPISEL(x)  (((x) & D1CCIPR_QSPISEL_M) << 4)
#define D1CCIPR_FMCSEL_M    0x3u
#define D1CCIPR_FMCSEL(x)   ((x) & D1CCIPR_FMCSEL_M)

#define D2CCIP1R_SWPSEL       (1u << 31)
#define D2CCIP1R_FDCANSEL_M   0x3u
#define D2CCIP1R_FDCANSEL(x)  (((x) & D2CCIP1R_FDCANSEL_M) << 28)
#define D2CCIP1R_DFSDM1SEL    (1 << 24)
#define D2CCIP1R_SPDIFSEL_M   0x3u
#define D2CCIP1R_SPDISEL(x)   (((x) & D2CCIP1R_SPDIFSEL_M) << 20)
#define D2CCIP1R_SPI45SEL_M   0x7u
#define D2CCIP1R_SPI45SEL(x)  (((x) & D2CCIP1R_SPI45SEL_M) << 16)
#define D2CCIP1R_SPI123SEL_M  0x7u
#define D2CCIP1R_SPI123SEL(x) (((x) & D2CCIP1R_SPI123SEL_M) << 12)
#define D2CCIP1R_SAI23SEL_M   0x7u
#define D2CCIP1R_SAI23SEL(x)  (((x) & D2CCIP1R_SAI23SEL_M) << 6)
#define D2CCIP1R_SAI1SEL_M    0x7u
#define D2CCIP1R_SAISEL(x)    ((x) & D2CCIP1R_SAI1SEL_M)

#define D2CCIP2R_LPTIM1SEL_M       0x7u
#define D2CCIP2R_LPTIM1SEL(x)      (((x) & D2CCIP2R_LPTIM1SEL_M) << 28)
#define D2CCIP2R_CECSEL_M          0x3u
#define D2CCIP2R_CECSEL(x)         (((x) & D2CCIP2R_CECSEL_M) << 22)
#define D2CCIP2R_USBSEL_M          0x3u
#define D2CCIP2R_USBSEL(x)         (((x) & D2CCIP2R_USBSEL_M) << 20)
#define D2CCIP2R_I2C123SEL_M       0x3u
#define D2CCIP2R_I2C123SEL(x)      (((x) & D2CCIP2R_I2C123SEL_M) << 12)
#define D2CCIP2R_RNGSEL_M          0x3u
#define D2CCIP2R_RNGSEL(x)         (((x) & D2CCIP2R_RNGSEL_M) << 8)
#define D2CCIP2R_USART16SEL_M      0x7u
#define D2CCIP2R_USART16SEL(x)     (((x) & D2CCIP2R_USART16SEL_M) << 3)
#define D2CCIP2R_USART234578SEL_M  0x7u
#define D2CCIP2R_USART234578SEL(x) ((x) & D2CCIP2R_USART234578SEL_M)

#define D3CCIPR_SPI6SEL_M      0x7u
#define D3CCIPR_SPI6SEL(x)     (((x) & D3CCIPR_SPI6SEL_M) << 28)
#define D3CCIPR_SAI4BSEL_M     0x7u
#define D3CCIPR_SAI4BSEL(x)    (((x) & D3CCIPR_SAI4BSEL_M) << 24)
#define D3CCIPR_SAI4ASEL_M     0x7u
#define D3CCIPR_SAI4ASEL(x)    (((x) & D3CCIPR_SAI4ASEL_M) << 21)
#define D3CCIPR_ADCSEL_M       0x3u
#define D3CCIPR_ADCSEL(x)      (((x) & D3CCIPR_ADCSEL_M) << 16)
#define D3CCIPR_LPTIM345SEL_M  0x7u
#define D3CCIPR_LPTIM345SEL(x) (((x) & D3CCIPR_LPTIM345SEL_M) << 13)
#define D3CCIPR_LPTIM2SEL_M    0x7u
#define D3CCIPR_LPTIM2SEL(x)   (((x) & D3CCIPR_LPTIM2SEL_M) << 10)
#define D3CCIPR_I2C4SEL_M      0x3u
#define D3CCIPR_I2C4SEL(x)     (((x) & D3CCIPR_I2C4SEL_M) << 8)
#define D3CCIPR_LPUART1SEL_M   0x7u
#define D3CCIPR_LPUART1SEL(x)  ((x) & D3CCIPR_LPUART1SEL_M)

#define CIFR_HSECSSF   (1 << 10)
#define CIFR_LSECSSF   (1 << 9)
#define CIFR_PLL3RDYF  (1 << 8)
#define CIFR_PLL2RDYF  (1 << 7)
#define CIFR_PLL1RDYF  (1 << 6)
#define CIFR_HSI48RDYF (1 << 5)
#define CIFR_CSIRDYF   (1 << 4)
#define CIFR_HSERDYF   (1 << 3)
#define CIFR_HSIRDYF   (1 << 2)
#define CIFR_LSERDYF   (1 << 1)
#define CIFR_LSIRDYF   (1 << 0)

#define BDCR_BDRST     (1 << 16)
#define BDCR_RTCEN     (1 << 15)
#define BDCR_RTCSEL_M  0x3u
#define BDCR_RTCSEL(x) (((x) & BDCR_RTCSEL_M) << 8)
#define BDCR_LSECCSD   (1 << 6)
#define BDCR_LSECSSON  (1 << 5)
#define BDCR_LSEDRV_M  0x3u
#define BDCR_LSEDRV(x) (((x) & BDCR_LSEDRV_M) << 3)
#define BDCR_LSEBYP    (1 << 2)
#define BDCR_LSERDY    (1 << 1)
#define BDCR_LSEON     (1 << 0)

#define CSR_LSIRDY (1 << 1)
#define CSR_LSION  (1 << 0)

#define AHB3RSTR_SDMMC1RST (1 << 16)
#define AHB3RSTR_QSPIRST   (1 << 14)
#define AHB3RSTR_FMCRST    (1 << 12)
#define AHB3RSTR_JPGDECRST (1 << 5)
#define AHB3RSTR_DMA2DRST  (1 << 4)
#define AHB3RSTR_MDMARST   (1 << 0)

#define AHB1RSTR_USB2OTGRST (1 << 27)
#define AHB1RSTR_USB1OTGRST (1 << 25)
#define AHB1RSTR_ETH1MACRST (1 << 15)
#define AHB1RSTR_ADC12RST   (1 << 5)
#define AHB1RSTR_DMA2RST    (1 << 1)
#define AHB1RSTR_DMA1RST    (1 << 0)

#define AHB2RSTR_SDMMC2RST (1 << 9)
#define AHB2RSTR_RNGRST    (1 << 6)
#define AHB2RSTR_HASHRST   (1 << 5)
#define AHB2RSTR_CRYPTRST  (1 << 4)
#define AHB2RSTR_CAMITFRST (1 << 0)

#define AHB4RSTR_HSEMRST  (1 << 25)
#define AHB4RSTR_ADC3RST  (1 << 24)
#define AHB4RSTR_BDMARST  (1 << 21)
#define AHB4RSTR_CRCRST   (1 << 19)
#define AHB4RSTR_GPIOKRST (1 << 10)
#define AHB4RSTR_GPIOJRST (1 << 9)
#define AHB4RSTR_GPIOIRST (1 << 8)
#define AHB4RSTR_GPIOHRST (1 << 7)
#define AHB4RSTR_GPIOGRST (1 << 6)
#define AHB4RSTR_GPIOFRST (1 << 5)
#define AHB4RSTR_GPIOERST (1 << 4)
#define AHB4RSTR_GPIODRST (1 << 3)
#define AHB4RSTR_GPIOCRST (1 << 2)
#define AHB4RSTR_GPIOBRST (1 << 1)
#define AHB4RSTR_GPIOARST (1 << 0)

#define APB3RSTR_LTDCRST (1 << 3)

#define APB1LRSTR_UART8RST   (1u << 31)
#define APB1LRSTR_UART7RST   (1 << 30)
#define APB1LRSTR_DAC12RST   (1 << 29)
#define APB1LRSTR_CECRST     (1 << 27)
#define APB1LRSTR_I2C3RST    (1 << 23)
#define APB1LRSTR_I2C2RST    (1 << 22)
#define APB1LRSTR_I2C1RST    (1 << 21)
#define APB1LRSTR_UART5RST   (1 << 20)
#define APB1LRSTR_UART4RST   (1 << 19)
#define APB1LRSTR_USART3RST  (1 << 18)
#define APB1LRSTR_USART2RST  (1 << 17)
#define APB1LRSTR_SPDIFRXRST (1 << 16)
#define APB1LRSTR_SPI3RST    (1 << 15)
#define APB1LRSTR_SPI2RST    (1 << 14)
#define APB1LRSTR_LPTIM1RST  (1 << 9)
#define APB1LRSTR_TIM14RST   (1 << 8)
#define APB1LRSTR_TIM13RST   (1 << 7)
#define APB1LRSTR_TIM12RST   (1 << 6)
#define APB1LRSTR_TIM7RST    (1 << 5)
#define APB1LRSTR_TIM6RST    (1 << 4)
#define APB1LRSTR_TIM5RST    (1 << 3)
#define APB1LRSTR_TIM4RST    (1 << 2)
#define APB1LRSTR_TIM3RST    (1 << 1)
#define APB1LRSTR_TIM2RST    (1 << 0)

#define APB1HRSTR_FDCANRST (1 << 8)
#define APB1HRSTR_MDIOSRST (1 << 5)
#define APB1HRSTR_OPAMPRST (1 << 4)
#define APB1HRSTR_SWPRST   (1 << 2)
#define APB1HRSTR_CRSRST   (1 << 1)

#define APB2RSTR_HRTIMRST  (1 << 29)
#define APB2RSTR_DFSDM1RST (1 << 28)
#define APB2RSTR_SAI3RST   (1 << 24)
#define APB2RSTR_SAI2RST   (1 << 23)
#define APB2RSTR_SAI1RST   (1 << 22)
#define APB2RSTR_SPI5RST   (1 << 20)
#define APB2RSTR_TIM17RST  (1 << 18)
#define APB2RSTR_TIM16RST  (1 << 17)
#define APB2RSTR_TIM15RST  (1 << 16)
#define APB2RSTR_SPI4RST   (1 << 13)
#define APB2RSTR_SPI1RST   (1 << 12)
#define APB2RSTR_USART6RST (1 << 5)
#define APB2RSTR_USART1RST (1 << 4)
#define APB2RSTR_TIM8RST   (1 << 1)
#define APB2RSTR_TIM1RST   (1 << 0)

#define APB4RSTR_SAI4RST    (1 << 21)
#define APB4RSTR_VREFRST    (1 << 15)
#define APB4RSTR_COMP12RST  (1 << 14)
#define APB4RSTR_LPTIM5RST  (1 << 12)
#define APB4RSTR_LPTIM4RST  (1 << 11)
#define APB4RSTR_LPTIM3RST  (1 << 10)
#define APB4RSTR_LPTIM2RST  (1 << 9)
#define APB4RSTR_I2C4RST    (1 << 7)
#define APB4RSTR_SPI6RST    (1 << 5)
#define APB4RSTR_LPUART1RST (1 << 3)
#define APB4RSTR_SYSCFGRST  (1 << 1)

#define GCR_WW1RSC (1 << 0)

#define D3AMR_SRAM4AMEN   (1 << 29)
#define D3AMR_BKPRAMAMEN  (1 << 28)
#define D3AMR_ADC3AMEN    (1 << 24)
#define D3AMR_SAI4AMEN    (1 << 21)
#define D3AMR_CRCAMEN     (1 << 19)
#define D3AMR_RTCAMEN     (1 << 16)
#define D3AMR_VREFAMEN    (1 << 15)
#define D3AMR_COMP12AMEN  (1 << 14)
#define D3AMR_LPTIM5AMEN  (1 << 12)
#define D3AMR_LPTIM4AMEN  (1 << 11)
#define D3AMR_LPTIM3AMEN  (1 << 10)
#define D3AMR_LPTIM2AMEN  (1 << 9)
#define D3AMR_I2C4AMEN    (1 << 7)
#define D3AMR_SPI6AMEN    (1 << 5)
#define D3AMR_LPUART1AMEN (1 << 3)
#define D3AMR_BDMAAMEN    (1 << 0)

struct PWR {
    volatile uint32_t CR1;
    volatile uint32_t CSR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t CPU_CR;
    uint32_t RESERVED0;
    volatile uint32_t D3CR;
    uint32_t RESERVED1;
    volatile uint32_t WKUPCR;
    volatile uint32_t WKUPFR;
    volatile uint32_t WKUPEPR;
};

#define CSR1_ACTVOSRDY (1 << 13)

#define CR3_LDOEN (1 << 1)

#define D3CR_VOS_M  0x3u
#define D3CR_VOS(x) (((x) & D3CR_VOS_M) << 14)
#define D3CR_VOSRDY (1 << 13)

/* direct access to rcc */
static struct RCC *RCC = (struct RCC*)ADDR_RCC;
static struct PWR *PWR = (struct PWR*)ADDR_PWR;

/* TODO: offer better support */
#define SYSCFG_PWRCR ((volatile uint32_t*)(ADDR_SYSCFG + 0x2c))

#define SYSCFG_PWRCR_ODEN (1 << 0)

/* fixed frequencies */
#define CLOCK_STM32H7XX_LSI_HZ         32000
#define CLOCK_STM32H7XX_CSI_HZ         4000000
#define CLOCK_STM32H7XX_HSI48_HZ       48000000
#define CLOCK_STM32H7XX_PLLSRC_MAX_HZ  16000000
#define CLOCK_STM32H7XX_SYS_SCALING_HZ 200000000

static struct {
    clock_freq_t hsi;
    clock_freq_t hse;
    clock_freq_t pllsrc; /* this doesn't exist in the tree but we need it */
    /* plls */
    struct {
        clock_freq_t ref_ck;
        clock_freq_t p;
        clock_freq_t q;
        clock_freq_t r;
    } pll[CLOCK_STM32H7XX_PLL_COUNT];
    /* sys */
    clock_freq_t sys;
} clocks;

static int hse_setup(unsigned long hz)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    /* disabled */
    if (hz == 0) {
        RCC->CR &= ~CR_HSEON;
        return 0;
    }

    /* otherwise */
    RCC->CR |= CR_HSEON;

    while (deadlock-- != 0)
        if ((RCC->CR & CR_HSERDY) != 0)
            break;

    if (!picoRTOS_assert(deadlock != -1))
        return -EBUSY;

    clocks.hse = (clock_freq_t)hz;
    return 0;
}

static int hsi_setup(clock_stm32h7xx_hsi_t hsi)
{
    if (!picoRTOS_assert(hsi < CLOCK_STM32H7XX_HSI_COUNT)) return -EINVAL;

    int deadlock = CONFIG_DEADLOCK_COUNT;

    switch (hsi) {
    case CLOCK_STM32H7XX_HSI_64MHZ: clocks.hsi = (clock_freq_t)64000000; break;
    case CLOCK_STM32H7XX_HSI_32MHZ: clocks.hsi = (clock_freq_t)32000000; break;
    case CLOCK_STM32H7XX_HSI_16MHZ: clocks.hsi = (clock_freq_t)16000000; break;
    case CLOCK_STM32H7XX_HSI_8MHZ: clocks.hsi = (clock_freq_t)8000000; break;

    case CLOCK_STM32H7XX_HSI_OFF:
        clocks.hsi = (clock_freq_t)0;
        RCC->CR &= ~(CR_HSION | CR_HSIKERON);
        return 0;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    /* success */
    RCC->CR &= ~(CR_HSIDIV(CR_HSIDIV_M) | CR_HSION | CR_HSIKERON);
    RCC->CR |= (CR_HSIDIV(hsi) | CR_HSION | CR_HSIKERON);

    /* wait */
    while (deadlock-- != 0)
        if ((RCC->CR & CR_HSIRDY) != 0 &&
            (RCC->CR & CR_HSIDIVF) != 0)
            break;

    if (!picoRTOS_assert(deadlock != -1))
        return -EBUSY;

    return 0;
}

static int csi_setup(clock_stm32h7xx_csi_t csi)
{
    if (!picoRTOS_assert(csi < CLOCK_STM32H7XX_CSI_COUNT)) return -EINVAL;

    if (csi == CLOCK_STM32H7XX_CSI_ON) {
        int deadlock = CONFIG_DEADLOCK_COUNT;

        RCC->CR |= (CR_CSION | CR_CSIKERON);
        while (deadlock-- != 0)
            if ((RCC->CR & CR_CSIRDY) != 0)
                break;

        if (!picoRTOS_assert(deadlock != -1))
            return -EBUSY;
    }else
        RCC->CR &= ~(CR_CSION | CR_CSIKERON);

    return 0;
}

static int hsi48_setup(clock_stm32h7xx_hsi48_t hsi48)
{
    if (!picoRTOS_assert(hsi48 < CLOCK_STM32H7XX_HSI48_COUNT)) return -EINVAL;

    if (hsi48 == CLOCK_STM32H7XX_HSI48_ON) {
        int deadlock = CONFIG_DEADLOCK_COUNT;

        RCC->CR |= CR_HSI48ON;
        while (deadlock-- != 0)
            if ((RCC->CR & CR_HSI48RDY) != 0)
                break;

        if (!picoRTOS_assert(deadlock != -1))
            return -EBUSY;
    }else
        RCC->CR &= ~CR_HSI48ON;

    return 0;
}

static int pllsrc_setup(clock_stm32h7xx_pllsrc_t pllsrc)
{
    if (!picoRTOS_assert(pllsrc < CLOCK_STM32H7XX_PLLSRC_COUNT)) return -EINVAL;

    switch (pllsrc) {
    case CLOCK_STM32H7XX_PLLSRC_HSI_CK: clocks.pllsrc = clocks.hsi; break;
    case CLOCK_STM32H7XX_PLLSRC_CSI_CK: clocks.pllsrc = (clock_freq_t)CLOCK_STM32H7XX_CSI_HZ; break;
    case CLOCK_STM32H7XX_PLLSRC_HSE_CK: clocks.pllsrc = clocks.hse; break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    /* turn all plls off */
    RCC->CR &= ~(CR_PLL3ON | CR_PLL2ON | CR_PLL1ON);

    /* set source */
    RCC->PLLCKSELR &= ~PLLCKSELR_PLLSRC(PLLCKSELR_PLLSRC_M);
    RCC->PLLCKSELR |= PLLCKSELR_PLLSRC(pllsrc);

    return 0;
}

static int pll_enable(size_t index, bool on)
{
    if (!picoRTOS_assert(index < (size_t)CLOCK_STM32H7XX_PLL_COUNT)) return -EINVAL;

    uint32_t pllxon;
    uint32_t pllxrdy;

    switch (index) {
    case 0:
        pllxon = (uint32_t)CR_PLL1ON;
        pllxrdy = (uint32_t)CR_PLL1RDY;
        break;

    case 1:
        pllxon = (uint32_t)CR_PLL2ON;
        pllxrdy = (uint32_t)CR_PLL2RDY;
        break;

    case 2:
        pllxon = (uint32_t)CR_PLL3ON;
        pllxrdy = (uint32_t)CR_PLL3RDY;
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    if (on) {

        int deadlock = CONFIG_DEADLOCK_COUNT;

        RCC->CR |= pllxon;

        while (deadlock-- != 0)
            if ((RCC->CR & pllxrdy) != 0)
                break;

        if (!picoRTOS_assert(deadlock != -1))
            return -EBUSY;

    }else
        RCC->CR &= ~pllxon;

    return 0;
}

static int set_divm(size_t index, unsigned long divm)
{
    if (!picoRTOS_assert(index < (size_t)CLOCK_STM32H7XX_PLL_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(divm <= (unsigned long)PLLCKSELR_DIVM1_M)) return -EINVAL;

    switch (index) {
    case 0:
        RCC->PLLCKSELR &= ~PLLCKSELR_DIVM1(PLLCKSELR_DIVM1_M);
        RCC->PLLCKSELR |= PLLCKSELR_DIVM1(divm);
        break;

    case 1:
        RCC->PLLCKSELR &= ~PLLCKSELR_DIVM2(PLLCKSELR_DIVM2_M);
        RCC->PLLCKSELR |= PLLCKSELR_DIVM2(divm);
        break;

    case 2:
        RCC->PLLCKSELR &= ~PLLCKSELR_DIVM3(PLLCKSELR_DIVM3_M);
        RCC->PLLCKSELR |= PLLCKSELR_DIVM3(divm);
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return 0;
}

static int divx_enable(size_t index, bool p, bool q, bool r)
{
    if (!picoRTOS_assert(index < (size_t)CLOCK_STM32H7XX_PLL_COUNT)) return -EINVAL;

    uint32_t pllcfgr_divpxen;
    uint32_t pllcfgr_divqxen;
    uint32_t pllcfgr_divrxen;

    switch (index) {
    case 0:
        pllcfgr_divpxen = (uint32_t)PLLCFGR_DIVP1EN;
        pllcfgr_divqxen = (uint32_t)PLLCFGR_DIVQ1EN;
        pllcfgr_divrxen = (uint32_t)PLLCFGR_DIVR1EN;
        break;

    case 1:
        pllcfgr_divpxen = (uint32_t)PLLCFGR_DIVP2EN;
        pllcfgr_divqxen = (uint32_t)PLLCFGR_DIVQ2EN;
        pllcfgr_divrxen = (uint32_t)PLLCFGR_DIVR2EN;
        break;

    case 2:
        pllcfgr_divpxen = (uint32_t)PLLCFGR_DIVP3EN;
        pllcfgr_divqxen = (uint32_t)PLLCFGR_DIVQ3EN;
        pllcfgr_divrxen = (uint32_t)PLLCFGR_DIVR3EN;
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    /* p */
    if (p) RCC->PLLCFGR |= pllcfgr_divpxen;
    else RCC->PLLCFGR &= ~pllcfgr_divpxen;
    /* q */
    if (q) RCC->PLLCFGR |= pllcfgr_divqxen;
    else RCC->PLLCFGR &= ~pllcfgr_divqxen;
    /* r */
    if (r) RCC->PLLCFGR |= pllcfgr_divrxen;
    else RCC->PLLCFGR &= ~pllcfgr_divrxen;

    return 0;
}

static int pll_range_setup(size_t index, unsigned long vco_hz)
{
    if (!picoRTOS_assert(index < (size_t)CLOCK_STM32H7XX_PLL_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(vco_hz <= 960000000ul)) return -EINVAL;

    uint32_t refx_ck;
    bool pllxvcosel = (vco_hz <= 420000000ul);

    if (!picoRTOS_assert(clocks.pllsrc >= (clock_freq_t)1000000)) return -EIO;

    if (clocks.pll[index].ref_ck <= (clock_freq_t)2000000) refx_ck = (uint32_t)0x0;
    else if (clocks.pll[index].ref_ck <= (clock_freq_t)4000000) refx_ck = (uint32_t)0x1;
    else if (clocks.pll[index].ref_ck <= (clock_freq_t)8000000) refx_ck = (uint32_t)0x2;
    else refx_ck = (uint32_t)0x3;

    switch (index) {
    case 0:
        /* range */
        RCC->PLLCFGR &= ~PLLCFGR_PLL1RGE(PLLCFGR_PLL1RGE_M);
        RCC->PLLCFGR |= PLLCFGR_PLL1RGE(refx_ck);
        /* vco */
        if (pllxvcosel) RCC->PLLCFGR |= PLLCFGR_PLL1VCOSEL;
        else RCC->PLLCFGR &= ~PLLCFGR_PLL1VCOSEL;
        break;

    case 1:
        /* range */
        RCC->PLLCFGR &= ~PLLCFGR_PLL2RGE(PLLCFGR_PLL2RGE_M);
        RCC->PLLCFGR |= PLLCFGR_PLL2RGE(refx_ck);
        /* vco */
        if (pllxvcosel) RCC->PLLCFGR |= PLLCFGR_PLL2VCOSEL;
        else RCC->PLLCFGR &= ~PLLCFGR_PLL2VCOSEL;
        break;

    case 2:
        /* range */
        RCC->PLLCFGR &= ~PLLCFGR_PLL3RGE(PLLCFGR_PLL3RGE_M);
        RCC->PLLCFGR |= PLLCFGR_PLL3RGE(refx_ck);
        /* vco */
        if (pllxvcosel) RCC->PLLCFGR |= PLLCFGR_PLL3VCOSEL;
        else RCC->PLLCFGR &= ~PLLCFGR_PLL3VCOSEL;
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return 0;
}

static int divr_setup(size_t index, unsigned long divp, unsigned long divq,
                      unsigned long divr, unsigned long divn)
{
    if (!picoRTOS_assert(index < (size_t)CLOCK_STM32H7XX_PLL_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(divp <= (unsigned long)PLLnDIVR_DIVPn_M)) return -EINVAL;
    if (!picoRTOS_assert(divq <= (unsigned long)PLLnDIVR_DIVQn_M)) return -EINVAL;
    if (!picoRTOS_assert(divr <= (unsigned long)PLLnDIVR_DIVRn_M)) return -EINVAL;
    if (!picoRTOS_assert(divn <= (unsigned long)PLLnDIVR_DIVNn_M)) return -EINVAL;

    switch (index) {
    case 0:
        RCC->PLL1DIVR = (uint32_t)(PLLnDIVR_DIVRn(divr - 1ul) | PLLnDIVR_DIVQn(divq - 1ul) |
                                   PLLnDIVR_DIVPn(divp - 1ul) | PLLnDIVR_DIVNn(divn - 1ul));
        break;

    case 1:
        RCC->PLL2DIVR = (uint32_t)(PLLnDIVR_DIVRn(divr - 1ul) | PLLnDIVR_DIVQn(divq - 1ul) |
                                   PLLnDIVR_DIVPn(divp - 1ul) | PLLnDIVR_DIVNn(divn - 1ul));
        break;

    case 2:
        RCC->PLL3DIVR = (uint32_t)(PLLnDIVR_DIVRn(divr - 1ul) | PLLnDIVR_DIVQn(divq - 1ul) |
                                   PLLnDIVR_DIVPn(divp - 1ul) | PLLnDIVR_DIVNn(divn - 1ul));
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return 0;
}

static int pll_setup(size_t index, const struct pll_settings *settings)
{
#define div_ceil(x, y) (((x) + ((y) - 1)) / (y))

    if (!picoRTOS_assert(index < (size_t)CLOCK_STM32H7XX_PLL_COUNT)) return -EINVAL;

    int res;
    unsigned long divn;
    unsigned long divm = 1ul;
    unsigned long divm_hz = (unsigned long)clocks.pllsrc;

    /* step1: ensure DIVM1 <= 16Mhz */
    if (clocks.pllsrc > (clock_freq_t)CLOCK_STM32H7XX_PLLSRC_MAX_HZ) {
        divm = div_ceil(divm_hz, (unsigned long)CLOCK_STM32H7XX_PLLSRC_MAX_HZ);
        divm_hz = divm_hz / divm;
    }

    /* set refx_ck */
    clocks.pll[index].ref_ck = (clock_freq_t)divm_hz;

    if ((res = set_divm(index, divm)) < 0)
        return res;

    /* step2: compute DIVN */
    divn = settings->vco_hz / divm_hz;

    /* turn off if needed */
    if ((res = divx_enable(index, settings->divp != 0, settings->divq != 0, settings->divr != 0)) < 0 ||
        (res = divr_setup(index, settings->divp, settings->divq, settings->divr, divn)) < 0 ||
        (res = pll_range_setup(index, settings->vco_hz)) < 0)
        return res;

    /* remember p */
    if (settings->divp != 0) clocks.pll[index].p = (clock_freq_t)(divm_hz * divn / settings->divp);
    else clocks.pll[index].p = (clock_freq_t)0;
    /* q */
    if (settings->divq != 0) clocks.pll[index].q = (clock_freq_t)(divm_hz * divn / settings->divq);
    else clocks.pll[index].q = (clock_freq_t)0;
    /* r */
    if (settings->divr != 0) clocks.pll[index].r = (clock_freq_t)(divm_hz * divn / settings->divr);
    else clocks.pll[index].r = (clock_freq_t)0;

    return pll_enable(index, settings->vco_hz != 0);
}

static int set_voltage_scale1(void)
{
    int timeout0 = CONFIG_DEADLOCK_COUNT;
    int timeout1 = CONFIG_DEADLOCK_COUNT;

    /* supply if LDO */
    PWR->CR3 |= CR3_LDOEN;

    while (timeout0-- != 0)
        if ((PWR->CSR1 & CSR1_ACTVOSRDY) != 0)
            break;

    if (!picoRTOS_assert(timeout0 != -1))
        return -EBUSY;

    /* disable overdrive */
    *SYSCFG_PWRCR &= ~SYSCFG_PWRCR_ODEN;
    /*@i@*/ (void)*SYSCFG_PWRCR; /* delay */

    /* voltage scaling */
    PWR->D3CR = (uint32_t)D3CR_VOS(0x3);

    while (timeout1-- != 0)
        if ((PWR->D3CR & D3CR_VOSRDY) != 0)
            break;

    if (!picoRTOS_assert(timeout1 != -1))
        return -EBUSY;

    return 0;
}

static int sw_setup(clock_stm32h7xx_sw_t sw)
{
    if (!picoRTOS_assert(sw < CLOCK_STM32H7XX_SW_COUNT)) return -EINVAL;

    int res;
    uint32_t cfgr = RCC->CFGR;
    int deadlock = CONFIG_DEADLOCK_COUNT;

    switch (sw) {
    case CLOCK_STM32H7XX_SW_HSI_CK: clocks.sys = clocks.hsi; break;
    case CLOCK_STM32H7XX_SW_CSI_CK: clocks.sys = (clock_freq_t)CLOCK_STM32H7XX_CSI_HZ; break;
    case CLOCK_STM32H7XX_SW_HSE_CK: clocks.sys = clocks.hse; break;
    case CLOCK_STM32H7XX_SW_PLL1_P_CK: clocks.sys = clocks.pll[0].p; break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    /* scaling */
    if (clocks.sys > (clock_freq_t)CLOCK_STM32H7XX_SYS_SCALING_HZ &&
        (res = set_voltage_scale1()) < 0)
        return res;

    /* switch */
    cfgr &= ~CFGR_SW(CFGR_SW_M);
    RCC->CFGR = cfgr | (uint32_t)CFGR_SW(sw);

    while (deadlock-- != 0)
        if ((RCC->CFGR & CFGR_SWS(CFGR_SWS_M)) == CFGR_SWS(sw))
            break;

    if (!picoRTOS_assert(deadlock != -1))
        return -EBUSY;

    return 0;
}

int clock_stm32h7xx_init(struct clock_settings *settings)
{
    int res;

    /* HSE, HSI, CSI & HSI48 */
    if ((res = hse_setup(settings->hse_hz)) < 0 ||
        (res = hsi_setup(settings->hsi)) < 0 ||
        (res = csi_setup(settings->csi)) < 0 ||
        (res = hsi48_setup(settings->hsi48)) < 0)
        return res;

    /* PLLs */
    if ((res = pllsrc_setup(settings->pllsrc)) < 0 ||
        (res = pll_setup((size_t)0, &settings->pll[0])) < 0 ||
        (res = pll_setup((size_t)1, &settings->pll[1])) < 0 ||
        (res = pll_setup((size_t)2, &settings->pll[2])) < 0)
        return res;

    /* SW mux */
    if ((res = sw_setup(settings->sw)) < 0)
        return res;

    return 0;
}

int clock_stm32h7xx_enable(clock_stm32h7xx_axb_t clk)
{
    if (!picoRTOS_assert(clk < CLOCK_STM32H7XX_AXB_COUNT)) return -EINVAL;

    size_t index = (size_t)clk >> 5;
    size_t bit = (size_t)(0x1fu & clk);
    /* FIXME: debatable */
    volatile uint32_t *reg = &RCC->AHB3ENR;

    reg[index] |= (1 << bit);
    return 0;
}

int clock_stm32h7xx_disable(clock_stm32h7xx_axb_t clk)
{
    if (!picoRTOS_assert(clk < CLOCK_STM32H7XX_AXB_COUNT)) return -EINVAL;

    size_t index = (size_t)clk >> 5;
    size_t bit = (size_t)(0x1fu & clk);
    /* FIXME: debatable */
    volatile uint32_t *reg = &RCC->AHB3ENR;

    reg[index] &= ~(1 << bit);
    return 0;
}

int clock_stm32h7xx_ker_sel(clock_stm32h7xx_ker_t ker, unsigned int value)
{
    size_t index = (size_t)(0x3 & (ker >> 8));
    unsigned int mask = (0xffu & ker) >> 5;
    size_t shift = (size_t)(0x1f & ker);

    if (!picoRTOS_assert(value <= mask)) return -EINVAL;
    if (!picoRTOS_assert(index < (size_t)4)) return -EINVAL; /* FIXME */

    volatile uint32_t *DxCCIPxR = &RCC->D1CCIPR;

    DxCCIPxR[index] &= ~(mask << shift);
    DxCCIPxR[index] |= (value << shift);

    return 0;
}

clock_freq_t clock_get_freq(clock_id_t clkid)
{
    if (!picoRTOS_assert(clkid <= CLOCK_STM32H7XX_HSI48_CK)) return -EINVAL;

    switch (clkid) {
    case CLOCK_STM32H7XX_PLL1_P_CK: return clocks.pll[0].p;
    case CLOCK_STM32H7XX_PLL1_Q_CK: return clocks.pll[0].q;
    case CLOCK_STM32H7XX_PLL1_R_CK: return clocks.pll[0].r;
    case CLOCK_STM32H7XX_PLL2_P_CK: return clocks.pll[1].p;
    case CLOCK_STM32H7XX_PLL2_Q_CK: return clocks.pll[1].q;
    case CLOCK_STM32H7XX_PLL2_R_CK: return clocks.pll[1].r;
    case CLOCK_STM32H7XX_PLL3_P_CK: return clocks.pll[2].p;
    case CLOCK_STM32H7XX_PLL3_Q_CK: return clocks.pll[2].q;
    case CLOCK_STM32H7XX_PLL3_R_CK: return clocks.pll[2].r;
    case CLOCK_STM32H7XX_SYS_CK: return clocks.sys;
    case CLOCK_STM32H7XX_HSE_CK: return clocks.hse;
    case CLOCK_STM32H7XX_HSI_KER_CK: return clocks.hsi;
    case CLOCK_STM32H7XX_CSI_KER_CK: return (clock_freq_t)CLOCK_STM32H7XX_CSI_HZ;
    case CLOCK_STM32H7XX_LSI_CK: return (clock_freq_t)CLOCK_STM32H7XX_LSI_HZ;
    // case CLOCK_STM32H7XX_LSE_CK:
    case CLOCK_STM32H7XX_HSI48_CK: return (clock_freq_t)CLOCK_STM32H7XX_HSI48_HZ;
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return (clock_freq_t)-EIO;
}

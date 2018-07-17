/***************************************************************************************************
*  TheaBoot                                                                                        *
*  Copyright (C) 2018 YiQiChuang(ShanXi) Electronic Technology CO,LTD.                             *
*  Copyright (c) 2012-2014 PX4 Development Team.                                                   *
*                                                                                                  *
*  This file is part of TheaBoot project.                                                          *
*                                                                                                  *
*  @file     bl.h                                                                                  *
*  @brief                                                                                          *
*  @author   Arthur Zheng                                                                          *
*  @email    15034186698@163.com                                                                   *
*  @version  0.2.0.0                                                                               *
*  @date     2018/07/15                                                                            *
*                                                                                                  *
*--------------------------------------------------------------------------------------------------*
*  Remark         :                                                                                *
*--------------------------------------------------------------------------------------------------*
*  Change History :                                                                                *
*  <Date>     | <Version> | <Author>       | <Description>                                         *
*--------------------------------------------------------------------------------------------------*
*  2015/05/17 | 0.1.0.0   | david_s5       | Create file                                           *
*  2018/07/15 | 0.2.0.0   | Arthur Zheng   | Restyle the structure of project                      *
*--------------------------------------------------------------------------------------------------*
* Lisense       : BSD 3-Clause                                                                     *
*                                                                                                  *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR   *
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND *
* FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR       *
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR              *
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR         *
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY     *
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR     *
* OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE            *
* POSSIBILITY OF SUCH DAMAGE.                                                                      *
*--------------------------------------------------------------------------------------------------*
*                                                                                                  *
***************************************************************************************************/

#ifndef HW_CONFIG_H_
#define HW_CONFIG_H_

/***************************************************************************************************
*  TARGET_HW_CGR                                                                                   *
***************************************************************************************************/
#ifdef TARGET_HW_CGR

/* 设备文字信息 */
#define DEVICE_ID                      "CGR"                              /*!< 设备型号 */
#define DEVICE_REV                     "3.0"                              /*!< 设备版本 */
#define DEVICE_SN                      "9ax1fde1vna1cn15"                 /*!< 设备序列号 */
#define DEVICE_FLASH_STRC              "@internal 0x08000000 12x64a"      /*!< 设备内存结构描述 */
#define DEVICE_DES                     "单片机:GD32F103C8T6 设计电流:2Amax"   /*!< 设备描述 */                

/* 接口使能 */
#define INTERFACE_USB                  0             /*!< USB接口使能 */
#define INTERFACE_USART                1             /*!< USART接口使能 */

/* Bootloader参数 */
#define BOOTLOADER_DELAY               2000          /*!< 引导启动延时 */  
#define BOOTLOADER_RESERVATION_SIZE    (8 * 1024)    /*!< Bootloader区大小 */
#define APP_LOAD_ADDRESS               0x08002000    /*!< APP加载地址 */

/* USB接口配置 */                
#define USBDEVICESTRING                "CGR V3.0"
#define USBPRODUCTID                   0x0001
#define USBMFGSTRING                   "YiQiChuang(ShanXi) Electronic Technology CO,LTD"

/* FLASH结构 */
#define BOARD_FLASH_SECTORS            64            /*!< 共64页,如果没有SECTOR则按照PAGE填写 */
#define BOARD_FLASH_SIZE               (64 * 2)
#define FLASH_SECTOR_SIZE              0x400

/* 外设配置 */
#define OSC_FREQ                       8             /*!< 外部晶振频率 */

#define BOARD_PORT_USART               GPIOA               /*!< USART接口 */
#define BOARD_PIN_TX                   GPIO9
#define BOARD_PIN_RX                   GPIO10
#define BOARD_USART_PIN_CLOCK_REGISTER RCC_APB2ENR
#define BOARD_USART_PIN_CLOCK_BIT      RCC_APB2ENR_IOPAEN
#define BOARD_USART                    USART1
#define BOARD_USART_CLOCK_REGISTER     RCC_APB1ENR
#define BOARD_USART_CLOCK_BIT          RCC_APB1ENR_USART2EN

#define BOARD_PIN_LED_ACTIVITY         GPIO14              /*!< 指示灯接口 */
#define BOARD_PIN_LED_BOOTLOADER       GPIO15
#define BOARD_PORT_LEDS                GPIOB
#define BOARD_CLOCK_LEDS_REGISTER      RCC_APB2ENR
#define BOARD_CLOCK_LEDS               RCC_APB2ENR_IOPBEN
#define BOARD_LED_ON                   gpio_clear
#define BOARD_LED_OFF                  gpio_set

#define BOARD_FORCE_BL_PIN             GPIO8               /*!< 强制进入Bootloader引脚接口 */
#define BOARD_FORCE_BL_PORT            GPIOA
#define BOARD_FORCE_BL_CLOCK_REGISTER  RCC_APB2ENR
#define BOARD_FORCE_BL_CLOCK_BIT       RCC_APB2ENR_IOPBEN
#define BOARD_FORCE_BL_PULL            GPIO_CNF_INPUT_FLOAT
#define BOARD_FORCE_BL_STATE           0                   /*!< 触发电平 */

#endif /* CGR-D-032 */

#endif /* HW_CONFIG_H_ */

/********* Copyright (C) 2018 YiQiChuang(ShanXi) Electronic Technology CO,LTD  *****END OF FILE****/


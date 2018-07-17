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

/* �豸������Ϣ */
#define DEVICE_ID                      "CGR"                              /*!< �豸�ͺ� */
#define DEVICE_REV                     "3.0"                              /*!< �豸�汾 */
#define DEVICE_SN                      "9ax1fde1vna1cn15"                 /*!< �豸���к� */
#define DEVICE_FLASH_STRC              "@internal 0x08000000 12x64a"      /*!< �豸�ڴ�ṹ���� */
#define DEVICE_DES                     "��Ƭ��:GD32F103C8T6 ��Ƶ���:2Amax"   /*!< �豸���� */                

/* �ӿ�ʹ�� */
#define INTERFACE_USB                  0             /*!< USB�ӿ�ʹ�� */
#define INTERFACE_USART                1             /*!< USART�ӿ�ʹ�� */

/* Bootloader���� */
#define BOOTLOADER_DELAY               2000          /*!< ����������ʱ */  
#define BOOTLOADER_RESERVATION_SIZE    (8 * 1024)    /*!< Bootloader����С */
#define APP_LOAD_ADDRESS               0x08002000    /*!< APP���ص�ַ */

/* USB�ӿ����� */                
#define USBDEVICESTRING                "CGR V3.0"
#define USBPRODUCTID                   0x0001
#define USBMFGSTRING                   "YiQiChuang(ShanXi) Electronic Technology CO,LTD"

/* FLASH�ṹ */
#define BOARD_FLASH_SECTORS            64            /*!< ��64ҳ,���û��SECTOR����PAGE��д */
#define BOARD_FLASH_SIZE               (64 * 2)
#define FLASH_SECTOR_SIZE              0x400

/* �������� */
#define OSC_FREQ                       8             /*!< �ⲿ����Ƶ�� */

#define BOARD_PORT_USART               GPIOA               /*!< USART�ӿ� */
#define BOARD_PIN_TX                   GPIO9
#define BOARD_PIN_RX                   GPIO10
#define BOARD_USART_PIN_CLOCK_REGISTER RCC_APB2ENR
#define BOARD_USART_PIN_CLOCK_BIT      RCC_APB2ENR_IOPAEN
#define BOARD_USART                    USART1
#define BOARD_USART_CLOCK_REGISTER     RCC_APB1ENR
#define BOARD_USART_CLOCK_BIT          RCC_APB1ENR_USART2EN

#define BOARD_PIN_LED_ACTIVITY         GPIO14              /*!< ָʾ�ƽӿ� */
#define BOARD_PIN_LED_BOOTLOADER       GPIO15
#define BOARD_PORT_LEDS                GPIOB
#define BOARD_CLOCK_LEDS_REGISTER      RCC_APB2ENR
#define BOARD_CLOCK_LEDS               RCC_APB2ENR_IOPBEN
#define BOARD_LED_ON                   gpio_clear
#define BOARD_LED_OFF                  gpio_set

#define BOARD_FORCE_BL_PIN             GPIO8               /*!< ǿ�ƽ���Bootloader���Žӿ� */
#define BOARD_FORCE_BL_PORT            GPIOA
#define BOARD_FORCE_BL_CLOCK_REGISTER  RCC_APB2ENR
#define BOARD_FORCE_BL_CLOCK_BIT       RCC_APB2ENR_IOPBEN
#define BOARD_FORCE_BL_PULL            GPIO_CNF_INPUT_FLOAT
#define BOARD_FORCE_BL_STATE           0                   /*!< ������ƽ */

#endif /* CGR-D-032 */

#endif /* HW_CONFIG_H_ */

/********* Copyright (C) 2018 YiQiChuang(ShanXi) Electronic Technology CO,LTD  *****END OF FILE****/


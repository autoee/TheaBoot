/***************************************************************************************************
*  TheaBoot                                                                                        *
*  Copyright (C) 2018 YiQiChuang(ShanXi) Electronic Technology CO,LTD.                             *
*  Copyright (c) 2012-2014 PX4 Development Team.                                                   *
*                                                                                                  *
*  This file is part of TheaBoot project.                                                          *
*                                                                                                  *
*  @file     bl.c                                                                                  *
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
*  2018/04/19 | 0.1.0.0   | Arthur Zheng   | Create file                                           *
*  2018/07/15 | 0.2.0.0   | Arthur Zheng   | Restyle the structure of project                      *
*  2018/08/06 | 0.2.0.1   | Arthur Zheng   | ȡ��last_input���ƣ����޸����˿�ͬʱ����ʱ�����������ݻ�*
*                                            ���µ�Ӳ������                                         *
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

/* Includes --------------------------------------------------------------------------------------*/
#include "hw_config.h"

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/flash.h>

#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/systick.h>

#include "bl.h"
#include "cdcacm.h"
#include "usart.h"

/* Defination ------------------------------------------------------------------------------------*/
/** 
  * @breif �������� 
  **/
#define BL_PROTOCOL_VERSION 		"0.1.1.0"     	/*!< ��ǰЭ��汾 */

#define PROTO_INSYNC				0x11            /*!< The month of her birthday */
#define PROTO_EOC					0x25            /*!< The day of her birthday */
#define PROTO_PROG_MULTI_MAX        256	            /*!< ��󵥴���д���ݳ���,��λ:byte */
#define PROTO_REPLY_MAX             256	            /*!< ��󷵻����ݳ���,��λ:byte */

/** 
  * @breif ����״̬ 
  **/
#define PROTO_OK					0x10            /*!< �����ɹ� */
#define PROTO_FAILED				0x11            /*!< ����ʧ�� */
#define PROTO_INVALID				0x13	        /*!< ָ����Ч */

/** 
  * @breif ����ָ�� 
  **/
#define PROTO_GET_SYNC				0x21            /*!< ����ͬ�� */

#define PROTO_GET_UDID				0x31            /*!< ��ȡоƬָ����ַ�ϵ� UDID 12�ֽڵ�ֵ */
#define PROTO_GET_FW_SIZE           0x32            /*!< ��ȡ�̼�����С */

#define PROTO_GET_BL_REV            0x41            /*!< ��ȡBootloader�汾 */
#define PROTO_GET_ID                0x42            /*!< ��ȡ��·���ͺ�,�����汾 */
#define PROTO_GET_SN                0x43            /*!< ��ȡ��·�����к� */
#define PROTO_GET_REV               0x44            /*!< ��ȡ��·��汾 */
#define PROTO_GET_FLASH_STRC        0x45            /*!< ��ȡFLASH�ṹ���� */
#define PROTO_GET_DES               0x46            /*!< ��ȡ�� ASCII ��ʽ��ȡ�豸���� */

#define PROTO_CHIP_ERASE			0x51            /*!< �����豸 Flash ����λ���ָ�� */
#define PROTO_PROG_MULTI			0x52            /*!< �ڵ�ǰ���ָ��λ��д��ָ���ֽڵ����ݣ���ʹ���ָ������ƶ�����һ�ε�λ�� */
#define PROTO_GET_CRC				0x53	        /*!< ���㲢����CRCУ��ֵ */
#define PROTO_BOOT					0x54            /*!< ���� APP ���� */

/* Private variable  -----------------------------------------------------------------------------*/
static uint8_t bl_type;
static const uint8_t bl_proto_rev[] = BL_PROTOCOL_VERSION; /*!<  */ // value returned by PROTO_DEVICE_BL_REV
volatile unsigned timer[NTIMERS];
static enum led_state {LED_BLINK, LED_ON, LED_OFF} _led_state;  /*<! ��¼LED״̬ */
static unsigned head, tail;
static uint8_t rx_buf[256];

/** 
  * @brief  ���ݽӿڳ�ʼ��
  * @param  none
  * @return none
  */
inline void cinit(void *config, uint8_t interface)
{
#if INTERFACE_USB

    if (interface == USB)
    {
        return usb_cinit();
    }

#endif
#if INTERFACE_USART

    if (interface == USART)
    {
        return uart_cinit(config);
    }

#endif
}

/** 
  * @brief  ���ݽӿڷ���ʼ��
  * @param  none
  * @return none
  */
inline void cfini(void)
{
#if INTERFACE_USB
    usb_cfini();
#endif
#if INTERFACE_USART
    uart_cfini();
#endif
}

/** 
  * @brief  ���ݽ���
  * @param  none
  * @return -1,ʧ��.����,���յ�������
  */
inline int cin(void)
{
#if INTERFACE_USB

    if (bl_type == NONE || bl_type == USB)
    {
        int usb_in = usb_cin();

        if (usb_in >= 0)
        {
            bl_type = USB;
            return usb_in;
        }
    }

#endif

#if INTERFACE_USART

    if (bl_type == NONE || bl_type == USART)
    {
        int uart_in = uart_cin();

        if (uart_in >= 0)
        {
            bl_type = USART;
            return uart_in;
        }
    }

#endif

    return -1;
}

/** 
  * @brief  ��������
  * @param  buf,����ָ��
  *         len,���ݳ���
  * @return none
  */
inline void cout(uint8_t *buf, unsigned len)
{
#if INTERFACE_USB

    if (bl_type == NONE || bl_type == USB)
    {
        usb_cout(buf, len);
    }

#endif
#if INTERFACE_USART

    if (bl_type == NONE || bl_type == USART)
    {
        uart_cout(buf, len);
    }

#endif
}

void buf_put(uint8_t b)
{
	unsigned next = (head + 1) % sizeof(rx_buf);

	if (next != tail) {
		rx_buf[head] = b;
		head = next;
	}
}

int buf_get(void)
{
	int	ret = -1;

	if (tail != head) {
		ret = rx_buf[tail];
		tail = (tail + 1) % sizeof(rx_buf);
	}

	return ret;
}

/** 
  * @brief  ��ת��APP��ڻ�����
  * @param  stacktop,ջ����ַ
  *         entrypoint,��ڵ�ַ
  * @return none
  */
static void do_jump(uint32_t stacktop, uint32_t entrypoint)
{
    asm volatile(
        "msr msp, %0	\n"
        "bx	%1	\n"
        :
        : "r"(stacktop), "r"(entrypoint)
        :);

    // �˴����򲻻ᱻִ��
    for (;;)
        ;
}

/** 
  * @brief  ��ת��APP���
  * @param  none
  * @return none
  */
void jump_to_app()
{

    const uint32_t *app_base = (const uint32_t *)APP_LOAD_ADDRESS;

    /*
     * APP ����λĬ����û�����ݵģ�ֱ������λ��ˢ��̼��������λ�� 0xffffffff �������ǻ᲻����
     */
    if (app_base[0] == 0xffffffff)
    {
        return;
    }

    /*
     * �ڶ������� APP ����ڵ�ַ������Ҫָ�� APP flash ��ַ
     */
    if (app_base[1] < APP_LOAD_ADDRESS) // С��APP��ַ��Χ
    {
        return;
    }

    if (app_base[1] >= (APP_LOAD_ADDRESS + board_info.fw_size)) // ����APP��ַ��Χ
    {
        return;
    }

    /* ����FLASH */
    flash_lock();

    /* �ر� systick �������ж� */
    systick_interrupt_disable();
	systick_counter_disable();

    /* ����ʼ���ӿ� */
    cfini();

    /* ��λʱ�� */
    clock_deinit();

    /* ����ʼ��������Դ */
    board_deinit();

    /* �����ж�������λ��ƫ�Ƶ�APP��ʼλ�� */
    SCB_VTOR = APP_LOAD_ADDRESS;

    /* ����ַ����Ĵ�����Ȼ����ת */
    do_jump(app_base[0], app_base[1]);
}

/** 
  * @brief  SysTick �жϴ�����
  * @param  none
  * @return none
  */
void sys_tick_handler(void)
{
    unsigned i;

	for (i = 0; i < NTIMERS; i++)
		if (timer[i] > 0) {
			timer[i]--;
		}

	if ((_led_state == LED_BLINK) && (timer[TIMER_LED] == 0)) {
		led_toggle(LED_BOOTLOADER);
		timer[TIMER_LED] = 50;
	}
}

/** 
  * @brief  ��ʱms����
  * @param  msec,ʱ��
  * @return none
  */
void delay(unsigned msec)
{

    timer[TIMER_DELAY] = msec;

    while (timer[TIMER_DELAY] > 0)
        ;

}

/** 
  * @brief  LED״̬���ú���
  * @param  state,LED״̬
  * @return none
  */
static void led_set(enum led_state state)
{
	_led_state = state;

	switch (state) {
	case LED_OFF:
		led_off(LED_BOOTLOADER);
		break;

	case LED_ON:
		led_on(LED_BOOTLOADER);
		break;

	case LED_BLINK:
		/* restart the blink state machine ASAP */
		timer[TIMER_LED] = 0;
		break;
	}
}

/** 
  * @brief  �����ɹ��ظ�
  * @param  none
  * @return none
  */
static void sync_response(void)
{

    uint8_t data[] =
        {
            PROTO_INSYNC, // "in sync"
            PROTO_OK      // "OK"
        };

    cout(data, sizeof(data));

}

/** 
  * @brief  ������Ч�ظ�
  * @param  none
  * @return none
  */
static void invalid_response(void)
{

    uint8_t data[] =
        {
            PROTO_INSYNC, // "in sync"
            PROTO_INVALID // "invalid command"
        };

    cout(data, sizeof(data));

}

/** 
  * @brief  ����ʧ�ܻظ�
  * @param  none
  * @return none
  */
static void failure_response(void)
{

    uint8_t data[] =
        {
            PROTO_INSYNC, // "in sync"
            PROTO_FAILED  // "command failed"
        };

    cout(data, sizeof(data));
}

static volatile unsigned cin_count;

/** 
  * @brief  �ȴ���������
  * @param  timeout,��ʱʱ��
  * @return -1,ʧ��.����,���յ�������
  */
static int cin_wait(unsigned timeout)
{

    int c = -1;

    /* start the timeout */
    timer[TIMER_CIN] = timeout;

    do
    {
        c = cin();

        if (c >= 0)
        {
            cin_count++;
            break;
        }

    } while (timer[TIMER_CIN] > 0);

    return c;
}

/** 
  * @brief      �ȴ�����EOC����
  * @param[in]  timeout,��ʱʱ��
  * @return     -1,ʧ��.����,���յ�������
  */
inline static uint8_t wait_for_eoc(unsigned timeout)
{
    return cin_wait(timeout) == PROTO_EOC;
}

/** 
  * @brief      ���4���ֽڵ�����
  * @param[in]  val,����
  * @return     none
  */
static void cout_word(uint32_t val)
{
    cout((uint8_t *)&val, 4);
}

/** 
  * @brief      ����4���ֽڵ�����
  * @param[out] wp,���ݽ���λ�õ�ָ��
  * @param[in]  timeout,��ʱʱ��
  * @return     1,ʧ��.����,���յ�������
  */
static int __attribute__((unused)) cin_word(uint32_t *wp, unsigned timeout)
{
    union {
        uint32_t w;
        uint8_t b[4];
    } u;

    for (unsigned i = 0; i < 4; i++)
    {
        int c = cin_wait(timeout);

        if (c < 0)
        {
            return c;
        }

        u.b[i] = c & 0xff;
    }

    *wp = u.w;
    return 0;
}

/** 
  * @brief      ����CRC32У��ֵ
  * @param[in]  src,����ָ��
  * @param[in]  len,���ݳ���
  * @param[in]  state,֮ǰCRCУ���ֵ
  * @return     CRC������
  */
static uint32_t crc32(const uint8_t *src, unsigned len, unsigned state)
{
    static uint32_t crctab[256];

    /* check whether we have generated the CRC table yet */
    /* this is much smaller than a static table */
    if (crctab[1] == 0)
    {
        for (unsigned i = 0; i < 256; i++)
        {
            uint32_t c = i;

            for (unsigned j = 0; j < 8; j++)
            {
                if (c & 1)
                {
                    c = 0xedb88320U ^ (c >> 1);
                }
                else
                {
                    c = c >> 1;
                }
            }

            crctab[i] = c;
        }
    }

    for (unsigned i = 0; i < len; i++)
    {
        state = crctab[(state ^ src[i]) & 0xff] ^ (state >> 8);
    }

    return state;
}

/** 
  * @brief      �������������ܺ���
  * @param[in]  timeout,�˳���������ĳ�ʱʱ��
  * @return     none
  */
void bootloader(unsigned timeout)
{

    uint32_t address = board_info.fw_size; /* Ĭ�Ͻ����ָ�����õ��̼���ĩβ */
    uint32_t first_word = 0xffffffff;

    bl_type = NONE;                        /* �����յ����ݵĽӿڽ���ȷ��Ϊ������Դ */

    /* ���� SysTick ���趨Ϊ 1ms �жϼ��*/
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	systick_set_reload(board_info.systick_mhz * 1000);
	systick_interrupt_enable();
	systick_counter_enable();

    /* ��� timeout �������㣬�����ڼ�ʱģʽ�£����� timeout ��ʱ */
    if (timeout)
    {
        timer[TIMER_BL_WAIT] = timeout;
    }

	/* Ĭ��״̬����˸LED */
	led_set(LED_BLINK);

    while (1) // Сѭ��
    {
        volatile int c;
        int arg;

        static union {
            uint8_t c[256];
            uint32_t w[64];
        } flash_buffer; // ���ջ���

		led_off(LED_ACTIVITY);

        do
        {
            if (timeout && !timer[TIMER_BL_WAIT]) // ��� timeout �������������
            {
                return;
            }
            c = cin_wait(0); // ���Դ���λ����ȡһ���ֽ�

        } while (c < 0); // �ڽ��յ�����ǰ��ѭ��

		led_on(LED_ACTIVITY);

        // ������յ�������
        switch (c)
        {
        // ����ͬ��
        case PROTO_GET_SYNC:
            if (!wait_for_eoc(2)) // �ȴ� EOC ��β
            {
                goto cmd_bad;
            }

            break;

        // ��ȡоƬ���к�
        case PROTO_GET_UDID:
        {
            uint32_t udid[3] = {0};
            char tmp[12] = {0};
            uint8_t i;

            if (!wait_for_eoc(2))
            {
                goto cmd_bad;
            }

            udid[0] = flash_func_read_udid(0);
            udid[1] = flash_func_read_udid(1);
            udid[2] = flash_func_read_udid(2);

            for (i = 0; i < 12; i++)
            {
                tmp[i] = (udid[i / 4] >> (4 * (i % 4)));
            }

            cout((uint8_t *)tmp, 24);
        }
        break;

        // ��ȡ�豸�̼�����С
        case PROTO_GET_FW_SIZE:
            if (!wait_for_eoc(2))
            {
                goto cmd_bad;
            }

            cout((uint8_t *)&board_info.fw_size, sizeof(board_info.fw_size));
            break; 

        // ��ȡBootloader�汾    
        case PROTO_GET_BL_REV:
            if (!wait_for_eoc(2))
            {
                goto cmd_bad;
            }

            cout((uint8_t *)&bl_proto_rev, sizeof(bl_proto_rev));
            break;

        // ��ȡ�豸ID
        case PROTO_GET_ID:
            if (!wait_for_eoc(2))
            {
                goto cmd_bad;
            }

            cout((uint8_t *)&board_info.id, sizeof(board_info.id));
            break;

        // ��ȡ�豸���к�
        case PROTO_GET_SN:
            if (!wait_for_eoc(2))
            {
                goto cmd_bad;
            }

            cout((uint8_t *)&board_info.sn, sizeof(board_info.sn)); 
            break;

        // ��ȡ�豸�汾
        case PROTO_GET_REV:
            if (!wait_for_eoc(2))
            {
                goto cmd_bad;
            }

            cout((uint8_t *)&board_info.rev, sizeof(board_info.rev)); 
            break; 

        // ��ȡ�豸FLASH�ṹ����
        case PROTO_GET_FLASH_STRC:
            if (!wait_for_eoc(2))
            {
                goto cmd_bad;
            }

            cout((uint8_t *)&board_info.flash_strc, sizeof(board_info.flash_strc));
            break;  

        // ��ȡ�豸����
        case PROTO_GET_DES:
            if (!wait_for_eoc(2))
            {
                goto cmd_bad;
            }

            cout((uint8_t *)&board_info.device_des, sizeof(board_info.device_des));
            break; 

        // ��д�̼���
        case PROTO_CHIP_ERASE:

            if (!wait_for_eoc(2))
            {
                goto cmd_bad;
            }

			led_set(LED_ON);

            // ����FLASH,��������APP����
            flash_unlock();

            for (int i = 0; flash_func_sector_size(i) != 0; i++)
            {
                flash_func_erase_sector(i);
            }

			led_set(LED_OFF);

            // verify the erase
            for (address = 0; address < board_info.fw_size; address += 4)
                if (flash_func_read_word(address) != 0xffffffff)
                {
                    goto cmd_fail;
                }

            address = 0; // ��λ��ַ

			led_set(LED_BLINK);

            break;

        // ��д
        case PROTO_PROG_MULTI: // program bytes

            arg = cin_wait(50);

            if (arg < 0)
            {
                goto cmd_bad;
            }

            // ������
            if (arg % 4) // ���Ȳ���4����������
            {
                goto cmd_bad;
            }

            if ((address + arg) > board_info.fw_size) // ��ǰ��ַ+���ȴ��ڹ̼�����������
            {
                goto cmd_bad;
            }

            if (arg > sizeof(flash_buffer.c)) // ���ȴ��ڻ����С����
            {
                goto cmd_bad;
            }

            for (int i = 0; i < arg; i++) // ��������
            {
                c = cin_wait(1000);

                if (c < 0) // ����Ϊ������
                {
                    goto cmd_bad;
                }

                flash_buffer.c[i] = c;
            }

            if (!wait_for_eoc(200))
            {
                goto cmd_bad;
            }

            if (address == 0)
            {
                // ���̼���һ�����ݱ��棬ֱ�����в��������д�룬�Է�ֹ�����г��ִ���
                first_word = flash_buffer.w[0];
                // �滻�̼���һ������
                flash_buffer.w[0] = 0xffffffff;
            }

            arg /= 4;

            for (int i = 0; i < arg; i++) // ��д����
            {
                // ��д����
                flash_func_write_word(address, flash_buffer.w[i]);

                // ������ȡ����һȷ��д����������
                if (flash_func_read_word(address) != flash_buffer.w[i])
                {
                    goto cmd_fail;
                }

                address += 4;
            }

            break;

        // ����̼���CRC32У��ֵ
        case PROTO_GET_CRC:
        {

            uint32_t sum = 0;

            if (!wait_for_eoc(2))
            {
                goto cmd_bad;
            }

            for (unsigned p = 0; p < board_info.fw_size; p += 4)
            {
                uint32_t bytes;

                // �滻��һλ����Ϊʵ������
                if ((p == 0) && (first_word != 0xffffffff))
                {
                    bytes = first_word;
                }
                else
                {
                    bytes = flash_func_read_word(p);
                }

                sum = crc32((uint8_t *)&bytes, sizeof(bytes), sum);
            }

            cout_word(sum);
        }
        break;

        // ȷ�������д������
        case PROTO_BOOT:

            if (!wait_for_eoc(1000))
            {
                goto cmd_bad;
            }

            // д���滻�ĵ�һ�ֽڵ�����
            if (first_word != 0xffffffff)
            {
                flash_func_write_word(0, first_word);

                if (flash_func_read_word(0) != first_word)
                {
                    goto cmd_fail;
                }

                // �ظ��������ݷ�ֹflashд�����
                first_word = 0xffffffff;
            }

            sync_response();
            delay(100);

            // ֹͣ����ת��Ӧ�ó���
            return;

        default:
 			goto cmd_bad;
        }

        // �����յ�һ����Чָ������Ѿ����ӵ�����������ֹͣtimeout��ʱ
        timeout = 0;

        // ���Ͳ����ɹ��ź�
        sync_response();
        continue;

    cmd_bad:
        // ���Ͳ�����Ч�ź�
        invalid_response();
        continue;

    cmd_fail:
        // ���Ͳ���ʧ���ź�
        failure_response();
        continue;
    }
}

/********* Copyright (C) 2018 YiQiChuang(ShanXi) Electronic Technology CO,LTD  *****END OF FILE****/

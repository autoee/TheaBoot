#--------------------------------------------------------------------------------------------------#
# TheaBoot                                                                                         #
# Copyright (c) 2018 YiQiChuang(ShanXi) Electronic Technology CO,LTD.                              #
# Copyright (c) 2012-2014 PX4 Development Team.                                                    #
#                                                                                                  #
# This file is part of TheaBoot project.                                                           #
#                                                                                                  #
# File    : makeifle                                                                               #
# Brief   :                                                                                        #
# Author  : Arthur Zheng                                                                           #
# Email   : 15034186698@163.com                                                                    #
# Version : 1.1.0.0                                                                                #
# Date    : 2018/07/15                                                                             #
#                                                                                                  #
#--------------------------------------------------------------------------------------------------#
# Remark         :                                                                                 #
#--------------------------------------------------------------------------------------------------#
# Change History :                                                                                 #
# <Date>     | <Version> | <Author>       | <Description>                                          #
#--------------------------------------------------------------------------------------------------#
# 2017/01/27 | 1.0.0.0   | Arthur Zheng   | Create file                                            #
# 2018/07/15 | 1.1.0.0   | Arthur Zheng   | Restyle the structure of project                       #
#--------------------------------------------------------------------------------------------------#
# Lisense       : BSD 3-Clause                                                                     #
#                                                                                                  #
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR   #
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND #
# FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR       #
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR              #
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR         #
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY     #
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR     #
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE            #
# POSSIBILITY OF SUCH DAMAGE.                                                                      #
#--------------------------------------------------------------------------------------------------#

#---------------------------------------------------------------------------------------------------
# 路径设定
#---------------------------------------------------------------------------------------------------
export BL_BASE		:= $(wildcard .)
export LIBOPENCM3	:= $(wildcard libopencm3)

#---------------------------------------------------------------------------------------------------
# 工具链设定
#---------------------------------------------------------------------------------------------------
export CC	 	 	= arm-none-eabi-gcc
export OBJCOPY		= arm-none-eabi-objcopy

#---------------------------------------------------------------------------------------------------
# 公共配置
#---------------------------------------------------------------------------------------------------
export FLAGS		= -std=gnu99 -Os -g -Wundef -Wall -fno-builtin -fgnu89-inline\
                      -I$(LIBOPENCM3)/include -ffunction-sections -nostartfiles\
                      -lnosys -Wl,-gc-sections -Wl,-g \
					  -Werror

export COMMON_SRCS	= bl.c cdcacm.c usart.c

#---------------------------------------------------------------------------------------------------
# 构建对象
#---------------------------------------------------------------------------------------------------
TARGETS	= \
		CGR

all:	$(TARGETS)

clean:
	cd libopencm3 && make --no-print-directory clean && cd ..
	rm -f *.elf *.bin

#---------------------------------------------------------------------------------------------------
# 对象构建指令
#---------------------------------------------------------------------------------------------------
CGR: $(MAKEFILE_LIST) $(LIBOPENCM3)
	make -f Makefile.f1 TARGET_HW=CGR LINKER_FILE=stm32f1.ld TARGET_FILE_NAME=$@

#---------------------------------------------------------------------------------------------------
# libopencm3构建
#---------------------------------------------------------------------------------------------------
.PHONY: $(LIBOPENCM3)
$(LIBOPENCM3): 
	make -C $(LIBOPENCM3) lib

#********* Copyright (C) 2018 YiQiChuang(ShanXi) Electronic Technology CO,LTD  *****END OF FILE****#

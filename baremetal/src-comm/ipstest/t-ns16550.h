/*
 * Header for NS16550 UART test code.
 *
 * Author: Jianheng.Zhang <zhang.jianheng@verisilicon.com>
 *         Zhigang.Kang <kang.zhigang@verisilicon.com>
 *
 * Copyright (C) 2021 VeriSilicon Microelectronics (Shanghai) Co., Ltd.
 *
 */

#ifndef __T_NS16550_H__
#define __T_NS16550_H__

void ns16550_test(void);

void ns16550_test_loopback(struct ns16550 *ns_uart);

#endif /* __T_NS16550_H__ */

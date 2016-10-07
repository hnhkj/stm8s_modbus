---
title: "FreeModbus移植到stm8s芯片"
author: "hnhkj@163.com"
date: "2016年10月7日"
output: html_document
---


## FreeModbus

是一款开源的modbus实现代码，在嵌入式领域有比较广泛的应用。但是，在FreeModbus众多的支持当中，缺少了
对STM8S003低端MCU的支持。为了降低产品成本，考虑移植FreeModbus到STM8S003当中。
对于移植的文档，在网络中有不少说明，但是很少有实际的代码。本人通过对FreeModbus的移植之后，觉得有必要
将给代码公开，方便人人使用，减少不必要的工作。


## FreeModbus使用注意：

modbus系统默认开始地址为0x01,而大多数系统的开始地址为0x00.因而，modbus系统默认是不能读取mcu内部的0x00数据的。因而，在使用当中要注意这一点。


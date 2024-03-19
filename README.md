# USART参考文档
## Serial库函数介绍
***适配型号STM32F103C8T6***

### 一、宏定义
#### 1.SERIAL_FALS|SERIAL_TRUE
用来定义Serial库中所有只有两种数据情况的规定值:
SERIAL_FALS 0
SERIAL_TRUE 1
#### 2.TXT|NUM
两个宏定义分别用来设置包的发送和接收的内容是以十六进制数的形式还是字符形式
#### 3.PACKLENGTH|TXTPACKLENGTH
PCAKLENGTH 用来规定一个数据包的长度，如PACKLENGTH 4表示一个数据包是4个字节大小
TXTPACKLENGTH 用来规定存储接收数据的最大字节数
### 二、变量
#### 1.Serial_TxPacket
用来存放发送数据的数组,发送数据时，要确保初始化时变量Send_Pack_Mode为SNUM,再调用Serial_Sendpack函数
#### 2.Serial_RxPacket
接收中断触发后，接收的数据存放在该数组中,要确保初始化时变量Recieve_Pack_Mode为RNUM
#### 3.Serial_txtRxPacket
用来存放接收文本形式的数据，要确保初始化时变量Recieve_Pack_Mode为RTXT
#### 4.Serial_Handling_Flag
***主进程是否完成标志位***
SERIAL_TRUE 表示主程序中正在处理的程序已经完成可以进入中断
SERIAL_FALS 表示主程序中正在处理的程序未完成可以进入中断
#### 5.NVIC_GroupConfigFlag
NVIC_PriorityGroupConfig()函数是否调用过的标志位
#### 6.Send_Pack_Mode
设置发送包数据的形式
STXT 发送形式为文本
SNUM 发送形式为16进制数据
#### 7.Recieve_Pack_Mode
设置接收包数据的形式
RTXT 接收形式为文本
RNUM 接收形式为16进制数据
#### 8.Serial_DataException
标识数据异常的变量
NO_EXCEPTION 无异常
SERIAL_TXPACKET_EXCEPTION 发送异常
SERIAL_RXPACKET_EXCEPTION 接收异常
#### 9.Recive_Area_State
标识接受区的异常
NOT_FULL 接受区未满
FULL 接收区满
#### 10.Serial_InitStructTypeDef
包含初始化USART1部分信息的结构体
### 三、函数
#### 1.void USART1_RxITMethod(void)
***不可被外部调用***
内部方法,USART1的USART_IT_RXNE标志位置位时自动调用
#### 2.void Serial_Init_1(Serial_InitStructTypeDef* Serial_InitStruct)
***可以被外部调用***
使用***Serial_InitStructTypeDef***类型结构体对USART1进行初始化
#### 3.void Serial_SendByte(USART_TypeDef *USARTx,uint8_t Byte)
***可以被外部调用***
发送一个字节的数据
#### 4.void Serial_SendArray(USART_TypeDef *USARTx,uint8_t* Array)
***可以被外部调用***
发送一个数组，数组的每个元素类型为uint8_t
#### 5.void Serial_SendString(USART_TypeDef* USARTx, char* String)
***可以被外部调用***
发送字符串
#### 6.void Serial_SendNumber(USART_TypeDef* USARTx,uint32_t Number)
***可以被外部调用***
将数字转为字符发送
#### 7.void Serial_SendPacket(USART_TypeDef* USARTx)
***可以被外部调用***
发送数据包
#### 8.void USART1_IRQHandler(void)
***USART1中断函数***

### 四、数据包的格式
#### |以十六进制收发数据时,每个包的结构如下
##### 1.
0xFF X X X X 0xFE
包头为0xFF,包尾为0xFE中间有若干字节的数据,默认为4字节长,在.h文件中修改**PACKLENGTH**宏可更改包长,使用Serial_SendPacket函数时无需添加包头包尾，将由软件自动完成添加

##### 2.
@ text '\r' '\n'
包头以'@'起始,包尾以'\r' '\n'结束包长最大长度固定由宏***TXTPACKLENGTH***定义,默认为100
### 运行方式
硬件连线参看STM32F103C8T6引脚定义图

开启APB2上USART1和GPIOA的时钟**-->**
初始化USART1的对应的IO引脚,输入默认设置为上拉输入,输出为复用功能推挽输出**-->**
初始化USART1寄存器,开启USART1中断**-->**
开启USART1**-->**
配置NVIC中断分组**-->**
配置开启NVIC上的USART1的中断
***以上配置完成后串口通信的硬件配置就已经完成***



​													**@written by Alvari**
​											  **Email: 496199270@qq.com**	

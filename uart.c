#include "uart.h"
#include "delay.h"
#include "stdio.h"
#if 0
u8 USART1_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 USART2_RX_BUF[USART_REC_LEN];
u8 USART3_RX_BUF[USART_REC_LEN];
u8 USART4_RX_BUF[USART_REC_LEN];
u8 USART5_RX_BUF[USART_REC_LEN];

//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART1_RX_STA=0;       //����״̬���	  
u16 USART2_RX_STA=0;
u16 USART3_RX_STA=0;
u16 USART4_RX_STA=0;
u16 USART5_RX_STA=0;


void USART1_IRQHandler(void)
{
	u8 Res;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		Res =USART_ReceiveData(USART1);	//��ȡ���յ�������	
		if((USART1_RX_STA&0x8000)==0)//����δ���
		{
			if(USART1_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART1_RX_STA=0;//���մ���,���¿�ʼ
				else USART1_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART1_RX_STA|=0x4000;
				else
				{
					USART1_RX_BUF[USART1_RX_STA&0X3FFF]=Res ;
					USART1_RX_STA++;
					if(USART1_RX_STA>(USART_REC_LEN-1))USART1_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
     } 
}	


void USART2_IRQHandler(void)
{
	u8 recv_date;
	
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		recv_date = USART_ReceiveData(USART2);	//(USART_ReceiveData(USART1) & 0x7f)
		USART_SendData(USART2,recv_date);	//��������
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET){};	//�ȴ����ͽ���	
	}  
}

void USART3_IRQHandler(void)
{
	u8 recv_date;

	if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		recv_date = USART_ReceiveData(USART3);	//(USART_ReceiveData(USART1) & 0x7f)
		USART_SendData(USART3,recv_date);	//��������
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET){};	//�ȴ����ͽ���	
	}
}	

void UART4_IRQHandler(void)
{
	u8 recv_date;
	
	if(USART_GetITStatus(UART4,USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(UART4,USART_IT_RXNE);
		recv_date = USART_ReceiveData(UART4);	//(USART_ReceiveData(USART1) & 0x7f)
		USART_SendData(UART4,recv_date);	//��������
		while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET){};	//�ȴ����ͽ���	
	}
}

void UART5_IRQHandler(void)
{
	u8 recv_date;
	
	if(USART_GetITStatus(UART5,USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(UART5,USART_IT_RXNE);
		recv_date = USART_ReceiveData(UART5);	//(USART_ReceiveData(USART1) & 0x7f)
		USART_SendData(UART5,recv_date);	//��������
		while(USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET){};	//�ȴ����ͽ���	
	}
}

void UART_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//1��GPIO�˿�ʱ�ӡ�����ʱ������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_USART1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB1Periph_USART3|RCC_APB1Periph_UART4|RCC_APB1Periph_UART5,ENABLE);
	
	//2��USART�˿ڸ�λ
	USART_DeInit(USART1);
	USART_DeInit(USART2);
	USART_DeInit(USART3);
	USART_DeInit(UART4);
	USART_DeInit(UART5);
	
	//3��GPIO�˿�ģʽ��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_9;  	//USART2��USART1 TX���� GPIO����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  	//USART3  TX���� GPIO����
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_12;	//UART4��UART5 TX���� GPIO����
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_10;		//USART2��USART1  RX���� GPIO����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	//USART3  RX���� GPIO����
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	//UART4  RX���� GPIO����
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	//UART5  RX���� GPIO����
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	//4��USART1-UART5���ڹ��ܳ�ʼ��
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStructure);
	USART_Init(USART2,&USART_InitStructure);
	USART_Init(USART3,&USART_InitStructure);
	USART_Init(UART4,&USART_InitStructure);
	USART_Init(UART5,&USART_InitStructure);
	
	//5�������жϳ�ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_Init(&NVIC_InitStructure);	
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_Init(&NVIC_InitStructure);	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
	
	USART_Cmd(USART1,ENABLE);
	USART_Cmd(USART2,ENABLE);
	USART_Cmd(USART3,ENABLE);
	USART_Cmd(UART4,ENABLE);
	USART_Cmd(UART5,ENABLE);
	
//	printf("USART ��ʼ���ɹ���\n");
}
#endif
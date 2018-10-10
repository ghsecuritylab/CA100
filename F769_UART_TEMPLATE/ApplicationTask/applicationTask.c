#include "applicationTask.h"

#define TCP_SERVER_RX_BUFSIZE 100
char tcp_server_recvbuf[TCP_SERVER_RX_BUFSIZE];

const char *taskSocket = "taskSocket";
osThreadId taskSocketHandle;

void creatOsTask(void)
{
//	osThreadDef(taskSocket,vTaskSocket,osPriorityAboveNormal,0,configMINIMAL_STACK_SIZE);
//	taskSocketHandle = osThreadCreate(osThread(taskSocket),NULL);

	sys_thread_new(taskSocket,tcpecho_thread,NULL,2*configMINIMAL_STACK_SIZE,osPriorityAboveNormal);
//	osThreadDef(taskSocket,tcpecho_thread,osPriorityAboveNormal,0,configMINIMAL_STACK_SIZE);
//	taskSocketHandle = osThreadCreate(osThread(taskSocket),NULL);
}

void tcpecho_thread(void *arg)
{
	//------parameter define-----//
  ip_addr_t clientIpAddr;
  struct netconn *conn,*newconn;
  struct netbuf *buf;
	struct pbuf *q;
  void *data;
  u16_t len,clientPort;
  err_t err,recv_err;
	struct netbuf *recvbuf;
	uint32_t data_len = 0;
	//------IPv4 address defined--//
  //192.168.0.10 is defined as sever IP
  LWIP_UNUSED_ARG(arg);
	osDelay(10);
	
	//------Create a new netconn--//
	conn = netconn_new(NETCONN_TCP);
	
	//------Bind IP address and port---//
	conn->recv_timeout = 10;
	err = netconn_bind(conn,IP_ADDR_ANY,50070);  
	if(err == ERR_OK)
		printf("Bind OK!\r\n");
	
	//------Listen the netconn----//
	err = netconn_listen(conn);
	printf("Listen OK!\r\n");  
	for( ;; )
	{
		//-----Wait for the client connect--//
		err = netconn_accept(conn,&newconn);
		newconn->recv_timeout = 10;
		if(err == ERR_OK)
		{
			printf("Connect OK!\r\n");
			
			netconn_getaddr(newconn,&clientIpAddr,&clientPort,0); 
			uint8_t remot_addr[4];
			remot_addr[3] = (uint8_t)(clientIpAddr.addr >> 24); 
			remot_addr[2] = (uint8_t)(clientIpAddr.addr >> 16);
			remot_addr[1] = (uint8_t)(clientIpAddr.addr >> 8);
			remot_addr[0] = (uint8_t)(clientIpAddr.addr);
			printf("IP:%d.%d.%d.%d,Port:%d\r\n",\
			remot_addr[0], remot_addr[1],remot_addr[2],remot_addr[3],clientPort);
				
			//NOTING! This osDelay();must be here
			osDelay(5);
			/* receive data until the other host closes the connection */
			for(;;)
			{
				if((recv_err = netconn_recv(newconn,&recvbuf)) == ERR_OK)
				{
					taskENTER_CRITICAL();//close interrupt
					memset(tcp_server_recvbuf,0,TCP_SERVER_RX_BUFSIZE);
					for(q=recvbuf->p;q!=NULL;q=q->next)
					{
							//??????TCP_SERVER_RX_BUFSIZE????????TCP_SERVER_RX_BUFSIZE?????,????
							//??????TCP_SERVER_RX_BUFSIZE????????,????????????
							if(q->len > (TCP_SERVER_RX_BUFSIZE-data_len)) 
									memcpy(tcp_server_recvbuf+data_len,q->payload,(TCP_SERVER_RX_BUFSIZE-data_len));//????
							else 
									memcpy(tcp_server_recvbuf+data_len,q->payload,q->len);
							data_len += q->len;      
							if(data_len > TCP_SERVER_RX_BUFSIZE) 
								break;  
					}
					taskEXIT_CRITICAL();  //open interrupt
					data_len=0;  //?????data_len????    
					printf("%s\r\n",tcp_server_recvbuf);  //????????????
					netbuf_delete(recvbuf);
				}
//				else if(recv_err == ERR_CLSD)  //????
//				{
//						netconn_close(newconn);
//						netconn_delete(newconn);
//						printf("master:%d.%d.%d.%d  is breaked\r\n",remot_addr[0], remot_addr[1],remot_addr[2],remot_addr[3]);
//						break;
//				}
			}
			
		}				
		osDelay(10);
	}
}


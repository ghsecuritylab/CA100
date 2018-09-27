#include "applicationTask.h"

int a;

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
  ip_addr_t serverIpAddr,clientIpAddr;
  struct netconn *conn,*newconn;
  struct netbuf *buf;
  void *data;
  u16_t len,clientPort;
  err_t err;

	//------IPv4 address defined--//
  IP4_ADDR(&serverIpAddr,192,168,0,2);//
  LWIP_UNUSED_ARG(arg);
	osDelay(10);
	
	//------Create a new netconn--//
	conn = netconn_new(NETCONN_TCP);
	
	//------Bind IP address and port---//
	conn->recv_timeout = 10;
	err = netconn_bind(conn,IP_ADDR_ANY,50070);  
	if(err == ERR_OK)
		printf("BindOK\r\n");
	
	//------Listen the netconn----//
	err = netconn_listen(conn);
	printf("ListenOK\r\n");  
	for( ;; )
	{
		//-----Wait for the client connect--//
		err = netconn_accept(conn,&newconn);
		newconn->recv_timeout = 10;
		if(err == ERR_OK)
		{
			printf("connectOK\r\n");
			for(;;)
			{
				netconn_getaddr(newconn,&clientIpAddr,&clientPort,0); 
				uint8_t remot_addr[4];
				remot_addr[3] = (uint8_t)(clientIpAddr.addr >> 24); 
				remot_addr[2] = (uint8_t)(clientIpAddr.addr >> 16);
				remot_addr[1] = (uint8_t)(clientIpAddr.addr >> 8);
				remot_addr[0] = (uint8_t)(clientIpAddr.addr);
				printf("IP:%d.%d.%d.%d,Port:%d\r\n",\
				remot_addr[0], remot_addr[1],remot_addr[2],remot_addr[3],clientPort);
				osDelay(5);
				for(;;);
				
			}

			netconn_close(conn);
			netconn_delete(conn);
		}
		osDelay(10);
	}
}


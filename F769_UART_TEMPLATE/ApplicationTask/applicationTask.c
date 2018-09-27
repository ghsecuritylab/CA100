#include "applicationTask.h"


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

void vTaskSocket(void const *argument)
{
	struct netconn *conn,*newconn;
	
	
	
//	conn = netconn_new(NETCONN_TCP);
//	netconn_bind(conn,IP_ADDR_ANY,50070);
//	netconn_listen(conn);
//	conn->recv_timeout = 10;

//	int sock,size,newconn;
//	struct sockaddr_in address,remotehost;
//	char recv_buffer[10];
//	/* create a TCP socket */
//	sock = lwip_socket(AF_INET,SOCK_STREAM,0);
//	if(sock<0)
//	{
//		printf("socket Create Failed\n");
//		return;
//	}
//	
//	/* bind to port 80 at any interface */
//	address.sin_family = AF_INET;
//	address.sin_port = htons(50007);
//	address.sin_addr.s_addr = INADDR_ANY;
//	if (bind(sock, (struct sockaddr *)&address, sizeof (address)) < 0)
//	{
//		printf("Bind Failed\n");
//		close(sock);
//		return;
//	}
//	
//	/* listen for connections (TCP listen backlog = 1) */
//  lwip_listen(sock, 1); 
//  size = sizeof(remotehost);

	
	for(;;)
	{
//		newconn = lwip_accept(sock, (struct sockaddr *)&remotehost, (socklen_t *)&size);
//		read(newconn, recv_buffer, sizeof(recv_buffer));
//		printf("%s",recv_buffer);
		printf("OK");
		osDelay(500);
	}
}
  err_t err;

void tcpecho_thread(void *arg)
{
  ip_addr_t serverIpAddr;
  struct netconn *conn,*newconn;
  struct netbuf *buf;
  void *data;
  u16_t len;

  IP4_ADDR(&serverIpAddr,192,168,0,2);// ???IP??
      
  LWIP_UNUSED_ARG(arg);
	
	printf(":192,168,0,2 :65000...\r\n");
	osDelay(10);
	
	/* Create a new connection identifier. */
	conn = netconn_new(NETCONN_TCP);
	conn->recv_timeout = 10;
	err = netconn_bind(conn,IP_ADDR_ANY,50070);  //???? 8088???
	if(err == ERR_OK)
		printf("BindOK\r\n");
	
	err = netconn_listen(conn);
	printf("ListenOK\r\n");  
	for( ;; )
	{
		err = netconn_accept(conn,&newconn);
		newconn->recv_timeout = 10;
		if(err == ERR_OK)
		{
			printf("connectOK\r\n");
			for(;;)
			{
				osDelay(10);
			}

			netconn_close(conn);
			netconn_delete(conn);
		}
		osDelay(10);
	}
}


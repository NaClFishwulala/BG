#define WhiteList "WhiteList.txt"
#define FlowData "FlowData.txt"

struct file *filep = NULL;//变成局部变量就不行了？
struct file *filep_flow = NULL;
char buf[256];
char src_ip[16];

#define BUF_SIZE 256 

int judgment_ip(u32 ina)
{
	loff_t pos = 0;
	int ret;
	int i;
	int head = 0;
	int end = 0;
	int flag = 0;
	if(filep == NULL) {
		filep = filp_open(WhiteList, O_RDONLY, 0);
		printk("open file\n");
	}
	
	if (IS_ERR(filep)) {
		printk("Open file %s error, code:%ld \n", WhiteList, PTR_ERR(filep));
		return -1;
	}

	sprintf(src_ip,"%d.%d.%d.%d\n", 
					(ina & 0xff000000) >> (6 * 4),
                    (ina & 0x00ff0000) >> (4 * 4),
                    (ina & 0x0000ff00) >> (2 * 4),
                    ina & 0x000000ff);
	printk("src_ip:%s len:%d\n", src_ip,strlen(src_ip));
	memset(buf, 0, sizeof(buf));
	pos = 0;
	ret = kernel_read(filep, buf, BUF_SIZE, &pos);
	printk("ret:%d buf:%s \n",ret,buf);
	for(i=0;i<ret;i++)
	{
	  if(buf[i] == '\n'){
	    end = i-2;
	    if(strncmp(buf+head , src_ip, end-head+1) == 0){
	      printk("Equall\n");
	      flag = 1;
	      break;
	    }
	    else{
	      printk("InEquall\n");
	    }
	    head = i + 1;
	  }
	}

	// if(filep != NULL) {
	// 	printk("before file close\n");
 //    	int t = filp_close(filep, NULL);
 //    	printk("file close code:%d",t);
	// }

	return flag;
}

void flow_count(char* buf1, char*buf2){
	loff_t pos = 0;	

	if(filep_flow == NULL) {
		filep_flow = filp_open(FlowData, O_RDWR | O_APPEND, 0644);
		printk("open file\n");
	}
	if (IS_ERR(filep_flow)) {
		printk("Open file %s error, code:%ld \n", FlowData, PTR_ERR(filep_flow));
		return -1;
	}
	kernel_write(filep_flow, buf1, strlen(buf1), &pos);
	kernel_write(filep_flow, buf2, strlen(buf2), &pos);

	return;
}
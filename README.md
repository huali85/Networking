# Networking
## TCP基础
### 建立Socket连接（三次握手）
A发起同步：[SYN] SEQ: 1000, ACK:  
B确认并返回同步：[SYN+ACK] SEQ: 2000, ACK: 1001  
A确认，连接建立：[ACK] SEQ: 1001, ACK: 2001  
### 数据交换
A发送：SEQ: 1200 100字节的数据  
B返回：ACK: 1301  
ACK：SEQ号+传递的字节数+1  
超时：A发送SEQ后未收到B返回的ACK，则A再次发送该SEQ
### 断开Socket连接（四次握手）
A发送FIN消息：[FIN]SEQ: 5000, ACK  
B确认：SEQ: 7500, ACK: 5001  
B发送FIN消息：[FIN]SEQ: 7501, ACK: 5001  
A确认，连接关闭：SEQ: 5001, ACK: 7502
### SEQ与ACK的含义
SEQ：XXXX 现在传递序号为XXXX的数据包，如果接收无误，请通知我向您传递第XXXX+1号数据包  
ACK：XXXX 刚才传递的XXXX-1号数据包接收无误，现在请传递第XXXX号数据包


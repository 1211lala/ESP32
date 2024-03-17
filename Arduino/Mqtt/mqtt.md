### mqtt流程简介

    MQTT 通信流程主要涉及到客户端（Client）和 MQTT 代理（Broker）之间的交互。通信流程如下:

    1. 建立连接
    -   客户端向 MQTT 代理发起连接请求。
    -   MQTT 代理响应连接请求，并建立连接。

    2.订阅主题：

    - 客户端可以选择订阅一个或多个主题。
    - 订阅表示客户端对感兴趣的特定主题的消息感兴趣。
  
    3.发布消息：

    - 客户端可以发布消息到一个或多个主题。
    - 消息包含一个主题和相应的有效载荷（Payload）。
  
    4.消息传递
    - MQTT 代理接收到发布的消息。
    - 代理将消息传送给所有订阅了相关主题的客户端。
  
    5.处理确认（可选）
    - 客户端可以选择要求确认的消息发布。
    - MQTT 代理将确认消息返回给发布者。

    6.取消订阅（可选）
    - 客户端可以选择取消对特定主题的订阅。
    - 取消订阅表示客户端不再对该主题的消息感兴趣。

    7.断开连接：
    - 客户端或 MQTT 代理可以选择断开连接。
    - 断开连接前，客户端通常会发送一个断开连接的请求。
    - 在 MQTT 中，客户端和代理之间的通信是异步的。这意味着客户端可以在没有等待响应的情况下继续执行其他操作。此外，MQTT 还提供了不同级别的服务质量（QoS），以确保消息传递的可靠性。


### Mqtt 版本 
    1. Mqtt3.1.1    
    2. Mqtt5


### Mqtt数据格式
1. `connect` 报文
   1. **clientid**          相对于服务端客户端的ID, 唯一.
   2. **cleanSession**      true:对于重要的话题在服务端发送服务后,会等待客户端回复(Qos > 1). false:不会等待回复     
   3. username
   4. password
   5. lastWillTopic
   6. lastWillQos
   7. lastWillMessage
   8. lastWillRetain
   9. **keepLive**          心跳时间间隔时间(单位:s)

2. `connack`报文
    1. sessionPresent       当 `cleanSession` 为 false 时如果有上次没有发送来的数据则返回 `true`
    2. returnCode           连接返回码(0: 成功  /其他失败具体查看错误码)



3. `publish`报文
   1. packetId              数据包的编号
   2. topicName             发布信息的主题名称
   3. qos                   服务质量
   4. retainFlag            如果设置为`true`,服务端会把这次上传的数据保存,只有客户端订阅,服务端就马上发送,不同等待下一次的发送
   5. payload               发布信息的内容
   6. dupFlag               重发标志



4. `subscribe`报文(可以一次订阅多个主题)

5. `suback`报文
   1. returnCode            订阅返回码(与`subscribe`报文的订阅等级有关.)

6. `unsubscribe`报文
   1. packetId  
   2. topic1               取消订阅的主题1
   3. topic2               liuao/setLed
   4. ....


### 通贝符
1. `+`   
      home/sensor/+/temp   = home/sensor/bedroom/temp = home/sensor/kitchen/temp

1. `#`
       home/sensor/#   = home/sensor/bedroom  = home/sensor/kitchen/temp




### Qos服务质量等级
- Qos == 0  最多发送一次
- Qos == 1  最少发送一次   
- Qos == 2  保证收一次
#### 设置Qos等级
   1. 在`mqttClient.connect(clientId)`连接服务端时设置 `cleanSession`参数为`false`
   2. 在发布时设置Qos
   3. 在订阅时设置Qos
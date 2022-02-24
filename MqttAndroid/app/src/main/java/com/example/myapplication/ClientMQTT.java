package com.example.myapplication;

import java.util.concurrent.ScheduledExecutorService;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttTopic;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;
public class ClientMQTT {

    public static final String HOST = "tcp://broker.emqx.io:1883";
    public static final String TOPIC1 = "qwerwlw";
    private static final String clientid = "client12";
    private MqttClient client;
    private MqttConnectOptions options;
    private long time=0;
    private boolean isIn = false;
    @SuppressWarnings("unused")
    private ScheduledExecutorService scheduler;

    public void start() {
        try {
            // host为主机名，clientid即连接MQTT的客户端ID，一般以唯一标识符表示，MemoryPersistence设置clientid的保存形式，默认为以内存保存  
            client = new MqttClient(HOST, clientid, new MemoryPersistence());
            // MQTT的连接设置  
            options = new MqttConnectOptions();
            options.setCleanSession(false);// 设置是否清空session
            options.setConnectionTimeout(10);             // 设置超时时间 单位为秒
            // 设置会话心跳时间 单位为秒 服务器会每隔1.5*20秒的时间向客户端发送个消息判断客户端是否在线，但这个方法并没有重连的机制  
            options.setKeepAliveInterval(20);             // 设置回调
            client.setCallback(new PushCallback());
            MqttTopic topic = client.getTopic(TOPIC1);
            client.connect(options);
            //订阅消息
            int[] Qos  = {1};
            String[] topic1 = {TOPIC1};
            client.subscribe(topic1, Qos);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}  

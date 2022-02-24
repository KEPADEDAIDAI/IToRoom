import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

public class SentMqttTmp {
    public static ServerMQTT server;
    public static void init() throws MqttException {
        server = new ServerMQTT();
        server.message = new MqttMessage();
        server.message.setQos(1);  //保证消息能到达一次
        server.message.setRetained(true);
        server.message.setPayload("g初始化成功".getBytes());
        server.publish(server.topic11 , server.message);
    }
    public static void pushMsg(String msg) throws MqttException {
        server.message.setPayload(msg.getBytes());
        server.publish(server.topic11 , server.message);
    }
}

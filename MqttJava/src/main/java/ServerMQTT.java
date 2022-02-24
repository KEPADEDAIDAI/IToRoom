import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.MqttPersistenceException;
import org.eclipse.paho.client.mqttv3.MqttTopic;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;
public class ServerMQTT {

    public static final String HOST = "tcp://broker.emqx.io:1883";
    public static final String TOPIC = "qwerwlw";
    private static final String clientid = "server11";

    private MqttClient client;
    MqttTopic topic11;

    MqttMessage message;
    public ServerMQTT() throws MqttException {
        // MemoryPersistence设置clientid的保存形式，默认为以内存保存
        client = new MqttClient(HOST, clientid, new MemoryPersistence());
        connect();
    }

    private void connect() {
        MqttConnectOptions options = new MqttConnectOptions();
        options.setCleanSession(false);
        // 设置超时时间
        options.setConnectionTimeout(10);
        // 设置会话心跳时间
        options.setKeepAliveInterval(20);
        try {
            client.setCallback(new PushCallback());
            client.connect(options);
            topic11 = client.getTopic(TOPIC);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    public void publish(MqttTopic topic , MqttMessage message) throws MqttPersistenceException,
            MqttException {
        MqttDeliveryToken token = topic.publish(message);
        token.waitForCompletion();
        System.out.println("message is published completely! "
                + token.isComplete());
    }

//    public static void main(String[] args) throws MqttException {
//        ServerMQTT server = new ServerMQTT();
//
//        server.message = new MqttMessage();
//        server.message.setQos(1);  //保证消息能到达一次
//        server.message.setRetained(true);
//        server.message.setPayload("这是推送消息的内容".getBytes());
//        server.publish(server.topic11 , server.message);
//        System.out.println(server.message.isRetained() + "------ratained状态");
//    }
}

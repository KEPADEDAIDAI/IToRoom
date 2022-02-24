import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import java.io.UnsupportedEncodingException;
import java.util.*;

public class Main {
    public static void main(String[] args) throws MqttException {
        SentMqttTmp.init();
        ClientMQTT client = new ClientMQTT();
        client.start();
        replayCenter.init();
    }

}
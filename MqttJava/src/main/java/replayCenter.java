import org.eclipse.paho.client.mqttv3.MqttException;

import java.util.Date;

public class replayCenter {
    public static void get(String str) throws MqttException {
        if (str.charAt(0) == 'o') {//开门信息
            int p = str.charAt(3) - '0';
            if (Chang.state[p]) {
                long now = new Date().getTime();
                Chang.allTime[p] = now - Chang.las[p];
                Chang.state[p] = false;
                Chang.nowNum--;
            } else {
                Chang.las[p] = new Date().getTime();
                Chang.state[p] = true;
                Chang.nowNum++;
            }
            SentMqttTmp.pushMsg("n"+Chang.nowNum);
        }
        if (str.charAt(0) == 'f') {//发送所有座位信息
            replayCenter.sentSit();
        }
        if (str.charAt(0) == 'e') //发送所有信息关于x
        {
            int x = str.charAt(3) - '0';
            replayCenter.sentInfo(x);
        }
        if (str.charAt(0) == 'c') {//更改座位信息
            int x = str.charAt(1) - '0';
            Chang.sit[x] = str.charAt(2) == '1';
            replayCenter.sentSit();
        }
    }

    public static void sentInfo(int x) throws MqttException {
        String str1 = "s00" + x;
        if (Chang.state[x]) str1 += '1';
        else str1 += '0';
        SentMqttTmp.pushMsg(str1);//发送x在馆状态
        SentMqttTmp.pushMsg("a00" + x + Chang.allTime[x]);//发送x在馆总时间
        SentMqttTmp.pushMsg("b00" + x + Chang.las[x]);//发送上次到馆时间
        SentMqttTmp.pushMsg("n" + Chang.nowNum);//发生当前在馆人数
    }

    public static void sentSit() throws MqttException {
        StringBuilder sit = new StringBuilder("d");
        for (int i = 0; i < 10; i++) {
            if (Chang.sit[i]) sit.append('1');
            else sit.append('0');
        }
        SentMqttTmp.pushMsg(sit.toString());
    }
    public static void init() throws MqttException {
        SentMqttTmp.pushMsg("n"+Chang.nowNum);
        replayCenter.sentSit();
    }
}

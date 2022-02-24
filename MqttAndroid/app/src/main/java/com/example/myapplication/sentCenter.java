package com.example.myapplication;

import org.eclipse.paho.client.mqttv3.MqttException;

public class sentCenter {//发送中心
    public static void ask() throws MqttException {//发送请求
        SentMqttTmp.pushMsg("e000");
    }
    public static void si() throws MqttException {//查看座位情况
        SentMqttTmp.pushMsg("ff");
    }
    public static void choseSit(int x) throws MqttException {
        SentMqttTmp.pushMsg("c"+x+"1");
    }
    public static void openDoor() throws MqttException {
        SentMqttTmp.pushMsg("o000");//开门
    }
}

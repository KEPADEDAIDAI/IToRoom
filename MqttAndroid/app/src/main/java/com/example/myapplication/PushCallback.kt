package com.example.myapplication

import org.eclipse.paho.client.mqttv3.MqttCallback
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken
import kotlin.Throws
import org.eclipse.paho.client.mqttv3.MqttMessage
import com.example.myapplication.SentMqttTmp
import java.lang.Exception
import android.widget.EditText
class PushCallback : MqttCallback {
    override fun connectionLost(cause: Throwable) {
        // 连接丢失后，一般在这里面进行重连
        println("连接断开，可以做重连")
    }

    override fun deliveryComplete(token: IMqttDeliveryToken) {
        println("deliveryComplete---------" + token.isComplete)
    }

    @Throws(Exception::class)
    override fun messageArrived(topic: String, message: MqttMessage) {
        // subscribe后得到的消息会执行到这里面
        println("接收消息主题 : $topic")
        println("接收消息Qos : " + message.qos)
        println("接收消息内容 : " + String(message.payload))
        val getStr = String(message.payload)
        if (getStr[0] == 'n') {
            val str: String = getStr.substring(1,getStr.length)
            Chang.nowNum = str.toInt();
        }
        else if(getStr[0]=='s'){
            Chang.state = getStr[4]=='1';
        }
        else if(getStr[0]=='a'){
            val str:String = getStr.substring(4,getStr.length)
            Chang.allTime = str.toLong()
        }
        else if(getStr[0]=='b'){
            val str:String = getStr.substring(4,getStr.length)
            Chang.laTime = str.toLong()
        }
        else if(getStr[0]=='d')
        {
            for(i in 1..10)
            {
                Chang.sit[i-1] = getStr[i]=='1';
            }
        }
    }
}
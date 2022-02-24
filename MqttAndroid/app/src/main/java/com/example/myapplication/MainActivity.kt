package com.example.myapplication

import android.annotation.SuppressLint
import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.Button
import android.widget.TextView
import java.util.*

class MainActivity : AppCompatActivity() {

    @SuppressLint("SetTextI18n")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        SentMqttTmp.init()
        val client:ClientMQTT = ClientMQTT()
        client.start()
        val flush: Button = findViewById(R.id.flush)
        val open: Button = findViewById(R.id.openTheDoor)
        val ask: Button = findViewById(R.id.ask)
        val reserve: Button = findViewById(R.id.reserve)
        val personNum: TextView = findViewById(R.id.personNum)
        val state: TextView = findViewById(R.id.state)
        val allTime : TextView = findViewById(R.id.allTime)
        val oneTime: TextView = findViewById(R.id.thisTime)
        flush.setOnClickListener {
            personNum.setText(Chang.nowNum.toString())//在馆人数
            if(Chang.state) {
                state.setText("在馆")
                val now = Date().time
                oneTime.setText(((now-Chang.laTime)/1000).toString()+"秒")
            }
            else {
                oneTime.setText("---")
                state.setText("未在馆")
            }
            allTime.setText((Chang.allTime/1000).toString()+"秒")
        }
        ask.setOnClickListener {
            sentCenter.ask()//同步
        }
        open.setOnClickListener {
            sentCenter.openDoor()
        }
        reserve.setOnClickListener {
            val intent = Intent(this,bookActivity::class.java)
            startActivity(intent)
        }
    }
}

package com.example.myapplication

import android.annotation.SuppressLint
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.text.Editable
import android.widget.Button
import android.widget.EditText
import android.widget.TextView
import android.widget.Toast

class bookActivity : AppCompatActivity() {
    val array = arrayListOf<TextView>()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_book)
        val button2: Button = findViewById(R.id.button2) //同步按键
        val button3: Button = findViewById(R.id.button3) //刷新按键
        val button: Button = findViewById(R.id.button)//座位预约
        val editTextNumber: EditText = findViewById(R.id.editTextNumber)
        init()
        show()
        button2.setOnClickListener {//同步
            sentCenter.si()
        }
        button3.setOnClickListener { // 界面刷新
            show()
        }
        button.setOnClickListener { //座位预约
            val text: String = editTextNumber.text.toString()
            var x: Int = text.toInt()
            x--;
            if(x<0||x>9)
            {
                Toast.makeText(this,"输入座位不存在",Toast.LENGTH_SHORT).show()
            }
            else if(Chang.sit[x])
                Toast.makeText(this,"该座位已有人",Toast.LENGTH_SHORT).show()
            else {
                sentCenter.choseSit(x)
                Toast.makeText(this,"预约成功",Toast.LENGTH_SHORT).show()
            }
        }
    }
    private fun init(){
        array.add(findViewById(R.id.textView3))
        array.add(findViewById(R.id.textView5))
        array.add(findViewById(R.id.textView7))
        array.add(findViewById(R.id.textView8))
        array.add(findViewById(R.id.textView9))
        array.add(findViewById(R.id.textView14))
        array.add(findViewById(R.id.textView11))
        array.add(findViewById(R.id.textView10))
        array.add(findViewById(R.id.textView13))
        array.add(findViewById(R.id.textView12))
    }
    @SuppressLint("SetTextI18n")
    fun show()
    {
        for(i in 0..9)
        {
            if(!Chang.sit[i]){
                array[i].setText("座位"+(i+1)+"：空")
            }
            else {
                array[i].setText("座位"+(i+1)+"：有人")
            }
        }
    }
}
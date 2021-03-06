# 基于物联网的智能自习室系统  
## 系统描述
本系统是一个基于物联网的自习室，有两种方式进入教室：刷卡和手机端APP开门。在手机的APP上，还可以预约座位、查看累计学习时长和本次学习时长、查看教室座位情况。在教室门口的屏幕上，实时显示教室里的座位情况和馆内人数。当你刷卡或者使用APP开门时门口的屏幕会显示欢迎词，当你离开的时候，屏幕也会显示欢迎再次光临的字样，系统会自动判断是一个用户出门还是另外的用户进门。

## 通讯协议
本系统使用MQTT进行通讯
服务器地址：broker.emqx.io
端口：1883
topic：qwerwlw

## 通讯信息格式控制

| 信息 | payload[0] |其余部分|实例|
| --- | --- | --- | --- |
| 开门 | o | payload[1]-payload[3]为用户编号，长度为3位，不够前面补0 | o001|
| 所有座位状态 | d | payload[1] - payload[10]10个数字为10个座位的使用情况，若为1则有人，0为无人使用 | d0100000000 |
| 教室人数变化 | n | payload[1]为教室人数 | n2 |
| 单个座位状态变化 | c | payload[1]为座位编号payload[2]为座位状态，1为有人，0为无人使用 | c01 |
| 请求座位信息 | f | 当发送f时表示请求当前座位信息，用于初始化座位信息使用 | f |
| 在教室总时间 | a | payload[1]-payload[3]用户编号，长度为3位，不够前面补0。payload[3]之后的所有数字表示该用户的在馆总时长的毫秒数 | a00010000（表示用户000在馆总时长为10秒） |
| 进入教室时间 | b | payload[1]-payload[3]用户编号。payload[3]之后的所有数字表示该用户上次到馆的时间，以距1970年1月1日的毫秒数计。 | b0001640880210061 |
|用户当前是否在教室| s | payload[1]-payload[3]用户编号。Payload[4]表示状态若在教室，则为1，不在为0 | s0001|
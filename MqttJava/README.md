# MqttJava
这是智能自习室的服务端

服务端作为硬件端和用户端之间的桥梁，用于传输和处理用户的进出自习教室数据、教室情况信息传输等数据。

当用户成功刷卡或者用APP成功进入教室之后，Nodemcu会识别用户信息并使用MQTT协议上传信息，而这些信息会被服务端读取和保存。

同时服务端负责对其他端的请求进行回应。提供当前的人数，状态和每个人的时长等信息。
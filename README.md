This is an extension of mBlock to be used in MeArm.Joystick Industry 4.0  
website : MeArm.weebly.com (chinese)

=============
Description : 
=============
1. MeArm angle : action based on angles of the servo motors
2. MeArm IK : action based on coordinate of x,y,z
  (0,0,0) is at the axis of base servo motor 
3. RFID : TK4100, 
4. LCD : 1602/1604 I2C LCD
5. Stepper motor : 42H48 Stepper motor
6. LedStrip : WS2812

developer : Ted Lien at ted99.tw@gmail.com

【中文】
這個擴展是給「機械手臂工業4.0」版使用的，內容如下：

1. MeArm 正運動 : 基於馬達角度的運動，每次運動軌跡是：出發點－目標點－出發點

2. MeArm 逆運動 : 基於座標點的運動，給三度空間座標x,y,z（mm），每次運動軌跡是：出發點－目標點，不會自動回到出發點，原點(0,0,0)是基座馬達軸心
（注意）調整欄位輸入後，必需呈現垂直狀態，更詳細請參考：
https://www.instructables.com/id/MeArmJoystick-%E9%80%86%E9%81%8B%E5%8B%95%E5%AD%B8/

3. RFID 晶片感測 : 125KHz，TK4100晶片

4. LCD 驅動 : 1602/1604 I2C LCD

5. 步進馬達 : 42H48 兩相混合式步進馬達

6. LED燈條 : 基於WS2812晶片，實現全彩效果

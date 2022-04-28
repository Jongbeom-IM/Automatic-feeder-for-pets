# Automatic-feeder-for-pets
This product is for pets that are designed to receive food without their owners.

I thought the fundamental requirment of this product is that It should be available wherever the owner is.

So I applied the MQTT protocol to this product to enable TCP/IP communication.

### Block diagram and Flow chart
![image](https://user-images.githubusercontent.com/80473250/165743268-c6b16450-fc37-4b83-a66c-12fa3102889d.png)

Enter the value by setting the meal time and amount of meal on the **USER's** mobile phone. After that according to each communication method, MQTT communication, Wireless transmission to the LOLIN board connected to the Wi-Fi via the broker The data is transmitted to the Arduino Uno R3 via USART communication. 

when the set meal time matches the current time, control the servo motor 1 to drop feed into the bowl and delay the amount of the payday entered

Adjust servo motor 1 to stop feed drop. Afterwards, the PWM is adjusted to play the melody with the buzzer to alert the pet that the meal is ready, and if the pet finishes eating for a certain period of time, it operates the servo motor 2 to discharge the feed, and the feed is collected by the load cell. Send the pay check message and the remaining feed to the mobile phone.

<img src="https://user-images.githubusercontent.com/80473250/165743257-5e8e02b4-c2be-4eba-a99c-9f992cc3b95c.png" width="600" height="800"/>

Allows **USER** to schedule the amount of food they serve and meal times they eat. At this time, additional meals can be provided by measuring the amount of exercise of pets through toys. 

If the time **USER** booked matches the current time, Feed will be dropped. 

When the meal is completed, the melody will ring once. After a certain period of time, the bowl is recovered and the remaining feed amount is measured and notified to the **USER**.

### Solid Works Modeling
|![image](https://user-images.githubusercontent.com/80473250/165741807-ce06d3be-81e2-45a2-b0b3-6a4036ad5dce.png)|![image](https://user-images.githubusercontent.com/80473250/165741818-fdc891cd-abcb-46bf-9f45-12a606281ef4.png)|![image](https://user-images.githubusercontent.com/80473250/165741837-e77d94a7-1812-4284-b5c0-0a6d70d0bacd.png)|
|:---:|:---:|:---:|
|Round veiw|inner veiw||


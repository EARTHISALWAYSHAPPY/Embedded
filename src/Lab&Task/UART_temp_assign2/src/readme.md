1. เริ่มต้นแสดง "Read Temperature in Celsius (1) or Fahrenheit (2):"
2. หากมีการรับค่าจาก Serial ให้แสดงค่าที่รับบน Serial และขึ้นบรรทัดใหม่ เช่น
    "Read Temperature in Celsius (1) or Fahrenheit (2):1"
3. หากกด 1 ให้แสดงอุณหภูมิในรูปแบบองศาเซสเซียส โดยมีทศนิยม 2 ตำแหน่งเช่น
    "Temperature is 27.34 C"
4. หากกด 2 ให้แสดงอุณหภูมิในรูปแบบองศาฟาเรนไฮต์ โดยมีทศนิยม 2 ตำแหน่งเช่น
    "Temperature is 81.22 F"
5. หากกดค่าอื่น ๆ ให้ขึ้นว่า "Please type only 1 or 2"
หลังจากอ่านค่าเรียบร้อยวนไปขึ้นประโยคที่ข้อ 1 อีกครั้ง
    "Read Temperature in Celsius (1) or Fahrenheit (2):"
6. สูตรการแปลงแรงดันอนาล็อกเป็นอุณหภูมิของ LM35 คือ -> Temp = Voltage*100
# ![Image](https://github.com/user-attachments/assets/7d7233ca-be4c-49b1-8409-13e3496bd51b)
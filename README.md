# AL5D Robot Arm Example Code
Arduino code for controlling the AL5D Robot Arm by Lynxmotion. Code here can be used for reference or for just playing around with.

# Useful Resouces
- [SSC-32U Documentation](https://www.robotshop.com/media/files/pdf2/lynxmotion_ssc-32u_usb_user_guide.pdf)

# Technical Details
The arduino connects too the SSC-32U through RX and TX pins over Serial. 

Instructions are sent in the format of `#<servo_num>P<pulse>S<speed>T<time>\r`, the S and T are optional.

This project is liscensed under the GPL-3.0 license.

> ~ <i>I like bread. But only in the winter.</i> ~
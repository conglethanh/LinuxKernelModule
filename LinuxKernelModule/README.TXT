######################################################
#                                                    #      
#               LOADABLE KERNEL MODULE               #
#            FOR CREATING A RANDOM NUMBER            #
#                                                    #
######################################################

README   
         1. Đây là một linux kernel module có nhiệm vụ đó là dùng để tạo ra một số nguyên ngẫu nhiên có kiểu int. 
         2. Module này tạo một character device cho phép tiến trình ở user space có thể open và read các số ngẫu nhiên đó.
         3. File random_number.c là file code C của module cần thực hiện random số.
         4. File test_random_number.c sẽ build ra file test để kiểm tra khả năng đọc và in ra số ngẫu nhiên đó thông qua chrdev.
         5. Mỗi lần chạy file test sẽ đọc được số ngẫu nhiên khác nhau từ module.

CONTACT
         Lê Thành Công
         MSSV: 1612842
         Lớp: 16CNTN 
         Khoa CNTT Trường ĐH KHTN

THANKS
         Derek Molly
         https://derekmolloy.ie

INSTALLATION & OPERATING INSTRUCTIONS
         1. Mở terminal
         2. Dùng lệnh cd [đường dẫn] để đường dẫn đến thư mục chứa file random_number.c, test_random_number.c và Makefile
         3. Gõ make để build file random_number.ko, file test và các file kèm theo
         4. Gõ sudo insmod random_number.ko để cài đặt module vào hệ thống
         5. Gõ sudo ./test để chạy file test đọc số random từ kernel module 

REMOVING LOADABLE KERNEL MODULE
         *. Dùng lệnh sudo rmmod random_number 

CLEANING EXECUTABLE FILES
         *. Dùng lệnh make clean

CONFIGURATION INSTRUCTIONS
         *. Cài đặt để chạy ./test mà không cần quyền root (sudo ./test) 
         1. Mở terminal
         2. Dùng lệnh cd [đường dẫn] để đường dẫn đến thư mục chứa file 99-random_number.rules
         3. Gõ: sudo cp 99-random_number.rules /etc/udev/rules.d (copy file rule đến thư mục rules.d hệ thống)
         4. Chạy ./test thành công mà không cần quyền root (sudo ./test)

MUTEX ADDED
         *. LKM này có sử dụng mutex locks để đảm bảo tại một thời điểm chỉ có một chương trình từ user space 
truy cập và đọc số ngẫu nhiên từ module, nếu có chương trình khác muốn đọc sẽ được thông báo busy, khi chương trình
kia kết thúc thì chương trình này mới đọc được số ngẫu nhiên. 
	

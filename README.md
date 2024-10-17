# Project_socket_15.23TNT1
This is test socket, **mail test**: mmt15.23tnt1@gmail.com
Để test project này, trước tiên hãy đọc 2 link sau:
https://www.emailarchitect.net/easendmail/ex/vc/20.aspx#installation
https://www.emailarchitect.net/eagetmail/ex/vc/18.aspx#installation
Ở đó sẽ có 2 link để tải, hãy tải về và để ở thư mục C:\\Program Files (x86) (Nếu thư mục khác, chỉnh nó trong phần Include ở 2 file: getM.cpp và sendM.cpp)
Để chạy, hãy làm các bước sau:
- Dùng cmd, nhập ipconfig, lướt xuống IPv4 adress (hoặc IPv6) để copy chính xác địa chỉ ip; lướt xuống cuối file ClientSock.cpp (chứa hàm main) và điều chỉnh địa chỉ ip cho đúng (có 2 chỗ để điều chỉnh)
- Chạy 2 project ClientSock & ServerSock cùng lúc.
- Khi nhấn Enter ở ClientSock, yêu cầu đăng nhập và xác thực trên **mail test**
- Sau khi đăng nhập thành công, client sẽ quét mail liên tục. Lúc này, hãy gửi mail (từ đâu cũng được) đến **mail test**, trong phần nội dung  mail đảm bảo có chuỗi kí tự password: _12345_
- Sau khi nhận được mail chứa đúng pass, client sẽ kết nối với  server. Nếu hiện dòng chữ: _what mess u want to send:_ tức là đã kết nối thành công. Nhập nội dung bất kì gửi đến server
- Server sẽ phản hồi kèm theo 1 file png; lúc này nếu thành công file sẽ được tạo trên thư mục của ClientSock; Client sẽ bắt đăng nhập và xác thực lần nữa trên **mail test**
- Sau khi đăng nhập thành công, mail sẽ được gửi (từ **mail test**) tới chính mail mà bạn đã dùng để gửi ban đầu kèm theo 1 file png.
- Có thể test các dạng file khác (trừ file pdf) bằng việc điều chỉnh trên cả ClientSock & ServerSock
Happy Coding 🫵

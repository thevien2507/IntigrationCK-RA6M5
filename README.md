<h1 align="center"><span style="color:#33cccc">Hệ thống tưới tiêu cho cây mini sử dụng Board CK-RA6M5 🎍</span></h1>

<p align="center">
  <img align="center" src="https://badgen.net/badge/icon/github?icon=github&label">
  <img align="center" src="https://badgen.net/badge/icon/git?icon=git&label">
  <img align="center" src="https://badgen.net/badge/icon/terminal?icon=terminal&label">
</p>

<p align="center">
  <strong><code style="color: name_color; font-size: 18px;">Cảm biến ánh sáng, độ ẩm, nhiệt độ và độ ẩm đất 🎋</code>
</strong></p>

<br>

## Nội dung 📂

- [Giới thiệu](#giới-thiệu)
- [Cài đặt](#cài-đặt)
- [Tính năng](#tính-năng)
- [Schematic](#schematic)
- [Video demo sản phẩm](#video-demo-sản-phẩm)
- [Feedback về Board CK-RA6M5](#đóng-góp)
- [Liên hệ](#liên-hệ)

<br>

## Giới thiệu

Hệ thống tưới tiêu thông minh cho chậu cây mini sử dụng cảm biến nhiệt độ, độ ẩm, ánh sáng có sẵn trên board CK-RA6M5 cùng với cảm biến độ ẩm đất để theo dõi tình trạng hiện tại của cây. Khi cây cần nước, hệ thống sẽ phát ra âm thanh vui nhộn để báo hiệu cây đang cần nước, sau đó tự động tưới nước, âm thanh sẽ tiếp tục phát khi trong bình chứa không có nước để bơm. Tính năng này không chỉ đảm bảo cung cấp nước mà còn tạo sự gắn kết giữa cây và người chăm sóc thông qua âm thanh độc đáo.

<br>

## Cài đặt

Để bắt đầu sử dụng dự án, làm theo các bước sau:

<span style="color:#33cccc">1.</span> Clone repository về máy của bạn và đảm bảo bạn đang có sẵn E2 Stduio IDE.

<span style="color:#33cccc">2.</span> Mở thư mục source code bằng E2 Stduio IDE.

<span style="color:#33cccc">3.</span> Click phải vào phần Project gần thanh Project Explorer và chọn Build Project.

<span style="color:#33cccc">4.</span> Sau khi Build Project xong, click phải vào phần Project và chọn Debug As -> Renesas GDB Hardware Debugging.

<br>

## Tính năng

- <span style="color:#ff9933">**Cảm biến nhiệt độ và độ ẩm**</span>

  - Cảm biến nhiệt độ và độ ẩm: Đo nhiệt độ và độ ẩm không khí xung quanh cây.
  - Giúp điều chỉnh thời gian tưới phù hợp với môi trường cây trồng.

- <span style="color:#ff9933">**Cảm biến ánh sáng:**</span>

  - Đo lượng ánh sáng nhận được bởi cây.
  - Giúp điều chỉnh thời gian tưới phù hợp với nhu cầu của cây dựa trên lượng ánh sáng.

- <span style="color:#ff9933">**Cảm biến độ ẩm đất**</span>

  - Đo độ ẩm của đất trong chậu. Là yếu tố quan trọng để quyết định cần phải tưới hay không.

- <span style="color:#ff9933">**Phát âm thanh**</span>

  - Nếu trong bình chứa nước đang thiếu nước, âm thanh sẽ được phát ra liên tục để báo hiệu cho người dùng biết cần cung cấp thêm nước vào bình chứa nước, khi đã đủ lượng nước, hệ thống sẽ bắt đầu quá trình tưới cây.

<br>

## Schematic

<div style="text-align: center;">
  <a href="https://drive.google.com/uc?id=1McouYJx6DSRdSDsLtI-h4RQH-V1U_Uc3&export=download target="_blank">
    <img src="https://drive.google.com/uc?id=1McouYJx6DSRdSDsLtI-h4RQH-V1U_Uc3&export=download" alt="Video thumbnail" width="1920" height="555">
  </a>
</div>

<br>

<!-- ## Video demo sản phẩm -->

## Video demo sản phẩm

[![Video thumbnail](https://drive.google.com/uc?id=1Ty28f8SqwLyGBMBXKwb2h6q4sl1AMCYN&export=download)](https://youtu.be/6IFA8qZ6_Kg)

<br>

## Đóng góp

- <span style="color:#ff9933">**Các điểm tích cực**</span>
  - Board tích hợp sẵn khá nhiều cảm biến như cảm biến khoảng cách, nhiệt độ, độ ẩm, ánh sáng hay nhịp tim nên rất tiện khi sử dụng.
  - Giao diện kết nối tương thích với Arduino UNO R3 khá dễ sử dụng.
  - Board hỗ trợ nhiều giao thức kết nối Ethernet, CAN, UART, I2C, SPI, cho phép kết nối với các thiết bị và cảm biến khác nhau.
  - Có nhiều cổng GPIO, ADC, Timer, và các giao diện khác, cho phép kết nối và điều khiển nhiều tính năng mở rộng.
- <span style="color:#ff9933">**Các điểm cần cải thiện**</span>
  - Bộ nhớ RAM hạn chế: Board chỉ có 512KB RAM, có thể hạn chế đối với một số ứng dụng yêu cầu lượng bộ nhớ lớn hơn.
  - Tài liệu hạn chế: Mặc dù có tài liệu hướng dẫn, nhưng một số phần có thể thiếu chi tiết, gây khó khăn cho người mới bắt đầu.
  - Cộng đồng người dùng và nhà phát triển chưa đông đảo: Mặc dù là một board mạnh mẽ, nhưng cộng đồng sử dụng CK-RA6M5 vẫn chưa phổ biến và lớn mạnh như một số board phổ thông khác. Điều này có thể dẫn đến việc thiếu các tài nguyên và hỗ trợ từ cộng đồng, khiến việc học hỏi, tìm kiếm thông tin và giải quyết vấn đề trở nên khó khăn hơn.
  - Trình soạn thảo IDE (Integrated Development Environment) phức tạp cho người mới bắt đầu: Mặc dù Renesas đã cung cấp E2 Studio - một trình soạn thảo IDE để phát triển phần mềm cho CK-RA6M5, nhưng nó có thể khá rắc rối và phức tạp đối với những người mới bắt đầu làm quen với board này.

## Liên hệ

Để biết thêm thông tin hoặc câu hỏi, xin vui lòng liên hệ người duy trì dự án qua email: [thevien2507@gmail.com](mailto:thevien2507@gmail.com).

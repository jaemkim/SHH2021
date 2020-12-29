e제이코어(eJCore)
=================
* 서울 하드웨어 해커톤 e제이코어(eJCore)팀
## 팀원
* 이종형대표 jhl@jcore.co.kr
* 차유정이사 cha@jcore.co.kr
## 개발 필요성
![LTE용 IP-PTT 단말기(DJ-CP100)](image/DJ-CP100.png)
* Quectel EC25J 칩 기반 Quectel QuecOpen development platform 활용
* **최신 음성 인식 기술**을 추가하여 **NTT도코모의 아성**에 도전
## 구현 관련 분석
### STT (Speech-to-Text) 기능 구현의 한계
![구성도](image/before.png)
___
<!-- 테이블 -->
|플랫폼|단점|비고|
|:--:|:--|:--|
|QuecOpen|python환경 없음|ALSA 대신 Quec Solution 사용|
|라즈베리파이|<ul><li>마이크 없음</li><li>python 활용</li><ul>|USB 마이크 사용|
|MCU|<ul><li>python환경 없음</li></ul>|<ul><li>WIFI</li><li>Mic/Speaker</li><li></li>STT_API 구현 필요</ul>|
|
### STT (Speech-to-Text) 기능 한계 극복 방안
![구성도](image/after.png)
* 개인 음성에 특화된 End-to-End Machine Learning 환경 제공
    + 개인 특정 음성 파일 입력
    + 음성 인식 모델 생성
## To Do List
- [x] STM32CubeMX/IDE 익숙해 지기
    + scanf 구현 : [4-5강 UART - TrueStudio에서 scanf 사용하기(ARM 기초 강좌 - Cortex-M3/STM32F1xx)](https://blog.naver.com/rhoblack/221449235824)
- [x] LSM6DSL 6축 센서 (3D accelerometer and 3D gyroscope)
    + [Getting started with STM32Cube.AI and motion sensing on the STM32L4 IoT node](https://wiki.st.com/stm32mcu/wiki/Getting_started_with_STM32Cube.AI_and_motion_sensing_on_the_STM32L4_IoT_node)
- [ ] AI 기능 구현 하기
- [ ] AWS IoT Core 사용하기
- [ ] Thingspark 사용하기
- [ ] 프로젝트 필요 기능 구현하기
    + WiFi
    + Audio
    + 모델 만들기
    + MCU에 넣기

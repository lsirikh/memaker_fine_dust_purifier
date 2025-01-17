## 팀명 및 팀원
* 팀명  : MEMAKER
* 팀원 1.  이기호 (프로그램작성 /디자인/개발/디버깅/ 발표 등)
* 팀원  2.  이준수 (기획 / 하드웨어 /3D출력관리 /프로젝트관리)


## 프로젝트 제목
* “iot기반 가정용 미세먼지측정기 제작 및 앱 관리, 측정데이터시스템 구축” 
<img src="https://github.com/lsirikh/memaker_fine_dust_purifier/blob/master/img%20resources/%EC%A0%95%EB%A9%B4%EC%83%B7.png">
<img src="https://github.com/lsirikh/memaker_fine_dust_purifier/blob/master/img%20resources/main%20case%20v16.png">

## 프로젝트 배경 혹은 목적
- 배 경 :  최근 미세먼지의 증가로 인해 가정에서 미세먼지 공기 질에 대한 관심 및 측정데이터의 관리가 국가적 정책과제*
  (iot 기반스마트융합 활성화 방안 정책과제 설정-산업통상자원부2016.12)
- 목 적 : 향 후 많은 IOT 기기의 접속에 따른 스마트폰 작동 과 모니터링 ,  측정 데이터의 집중 관리(빅데이터)  및 데이터 활용의 구체적 실현 

## 프로젝트 제품의 특징
1) 3가지 화면 전환을 통한 제품 컨트롤(미세먼지 농도 측정, 팬 속도 조절, 오토/수동 모두 설정)
<img src ="https://github.com/lsirikh/memaker_fine_dust_purifier/blob/master/img%20resources/OLED.png">

2) PM2.5 미세먼지 필터 활용가능
<img src ="https://github.com/lsirikh/memaker_fine_dust_purifier/blob/master/img%20resources/후면샷_필터개방.png">

3) 타이젠 OS 클라우드 플랫폼 Nubison 활용을 통한 원격 컨트롤 및 모니터링
<img src ="https://github.com/lsirikh/memaker_fine_dust_purifier/blob/master/img%20resources/nubison_memaker.png">

4) 타이젠 OS 클라우드 플랫폼 Thingspark 데이터 수집 
<img src ="https://github.com/lsirikh/memaker_fine_dust_purifier/blob/master/img%20resources/thingspark.png">


## Wiring 정보

* 1) Entire Wiring Diagram
<img src ="https://github.com/lsirikh/memaker_fine_dust_purifier/blob/master/img%20resources/전체연결도(수정).png">

* 2) Fine Dust Sensor Wiring Diagram
<img src ="https://github.com/lsirikh/memaker_fine_dust_purifier/blob/master/img%20resources/아두이노&pms7003.png">

* 3) Motor & Motor Driver Wiring Diagram
<img src ="https://github.com/lsirikh/memaker_fine_dust_purifier/blob/master/img%20resources/아두이노&모터드라이버.png">

* 4) Arduino & Raspberri Pi Wiring Diagram
<img src ="https://github.com/lsirikh/memaker_fine_dust_purifier/blob/master/img%20resources/타이젠&아두이노&레벨시프트.png">

* 5) Tactile Switch Wiring Diagram
<img src ="https://github.com/lsirikh/memaker_fine_dust_purifier/blob/master/img%20resources/아두이노&택트스위치.png">

* 6) OLED Display Wiring Diagram
<img src ="https://github.com/lsirikh/memaker_fine_dust_purifier/blob/master/img%20resources/oled_128x64(수정).png">

## 타이젠 오픈소스에 컨트리뷰션한 내역
없음

## 파일 리스트
* src/ai2c.cpp
* src/resource_AI2C.c
* arduinoi2c_slave_dust_PMS7003_final.ino (아두이노 코드)

## 코드 기여자
* 이기호 src/ai2c.cpp
* 이기호 src/resource_AI2C.c
* 이기호 arduinoi2c_slave_dust_PMS7003_final.ino


## 보드
* 아두이노 보드
-	공기 센서의 측정 값 처리 ( PMS7003 센서) 
-	데이터 디스플레이 (OLED) 
-	공기청정기 작동 ( 팬 작동(pwm)LMD18500 모듈)  
* 라즈베리파이3   ( 타이젠o/s  탑재 )
-	아두이노 연결 I2C 연결
-	크라우드 서버 구동 ( 스마트폰 연결 / 데이터 구축 및 모니터링)
- Thingspark, Nubison활용


* RPI3 : 아두이노 연동, thingspark,누비센 연동, github.com/lsirikh/tizen_hackerton01
* 아두이노 보드: 먼지센서, 모터콘트롤러, OLED 연동, RPi3 연동 , github.com/lsirikh/tizen_hackerton02

## 구현사항

* 타이젠 I2C, 아두이노 UART, I2C, 
* 누비슨 클라우드 / 싱스파크 클라우드 둘 다활용


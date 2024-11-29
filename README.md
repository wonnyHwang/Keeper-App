# Keeper-App

## 배경
* 졸업프로젝트 (전공 과목명 : 종합설계과제및운영)
* 유독 가스의 경우 무색/무취 특성 -> 장시간 노출 시 위험도 증가
* 기존 사례 : 농도 검출 후 경보를 울리는 경우만 존재
* 다양한 사랑들이 기능의 혜택을 얻을 수 있는 제품 필요

## 설명
* 센서 이용 -> 가스 농도 파악 및 대처
* 양방향 연결 : Client-Server / 양방향 Full-Duplex TCP 통신
* App -> RaspberryPI : 사용자 경험(UX) 제공

## 역할
#### 2인 진행
* 황종원 : HW 설계(RaspberryPI 회로 구성) / Web Socket 구현(리눅스 환경)
* 한종헌 : App 설계 / Web Socket 구현(PC 네트워크 설계)

## 프로젝트 목표
### 1. 핵심 기능 및 성능 목표
|핵심 필수 기능|성능 목표|고려 사항|
|:---:|:---:|:---:|
|Client-Server / 양방향 Full-Duplex TCP 통신|50번 전송 시, 최소한 49번 성공 / 지연: 1초 이내|MultiThread TCP 소켓 통신 이용 & Client-Server를 통해 2개의 양방향 Full-Duplex 통신으로
정보 전달|
|MQ-7B, MQ-4 센서
유독가스 검출|최소 2초 이내 감|측정된 아날로그 값 -> 디지털 값 변환, 측 : ADC 모듈|
|LED, 부저 등
상황 전달|최소 2초 이내 작동|가스 수치에 따른 LED 색 출력 변화 & 기준 값 이상 -> 부저 작동|
|App → RaspberryPi
사용자 경험(UX) 제공|최소 2초이내 작동
지연: 1초 이내|Raspberry Pi에서 시스템 기능 전달 시, 메시지가 섞이지 않도록 MultiThread 구현|

### 2. 최종 결과물
* 기능 블록도
![image.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/f3126d18-5831-4067-9d49-ba1cbc36a5a9/92baeb96-168b-4225-aa1b-a457ca78fa83/image.png)



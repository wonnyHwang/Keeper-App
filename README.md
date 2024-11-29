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

## 진행
![image](https://github.com/user-attachments/assets/1b50157a-7eab-4b24-85f6-261deec42e2a)


## 프로젝트 목표
### 1. 핵심 기능 및 성능 목표
|핵심 필수 기능|성능 목표|고려 사항|
|:---:|:---:|:---:|
|Client-Server / 양방향 Full-Duplex TCP 통신|50번 전송 시, 최소한 49번 성공 & 지연: 1초 이내|MultiThread TCP 소켓 통신 이용 & Client-Server를 통해 2개의 양방향 Full-Duplex 통신으로 정보 전달|
|MQ-7B, MQ-4 센서 유독가스 검출|최소 2초 이내 감지|측정된 아날로그 값 -> 디지털 값 변환, 측 : ADC 모듈|
|LED, 부저 -> 상황 전달|최소 2초 이내 작동|가스 수치에 따른 LED 색 출력 변화 & 기준 값 이상 -> 부저 작동|
|App → RaspberryPi 사용자 경험(UX) 제공|최소 2초이내 작동 & 지연: 1초 이내|Raspberry Pi에서 시스템 기능 전달 시, 메시지가 섞이지 않도록 MultiThread 구현|



### 2. 최종 결과물
* 기능 블록도

![image](https://github.com/user-attachments/assets/52e299ef-2502-4c7d-95f7-cfc99910131d)



* 안드로이드 App

![image](https://github.com/user-attachments/assets/22602806-9781-4def-9f16-7ea552cd6ad0)



* 기능 실행

![image](https://github.com/user-attachments/assets/193b1e28-3f2e-4e10-ae58-49315276f5cc)



### 3. 종합설계 프로젝트 구성 요소 및 제한 요소, 구현 사양
|요소 설계 기술|관련 교과목|제한 요소(구현, 가격 등)|구현 사양 (SPEC)(필수 기능 / 성능)|비고 (참고 사항)|
|:---:|:---:|:---:|:---:|:---:|
|Client-Server / 양방향 Full-Duplex TCP 통신|컴퓨터 네트워크|Application 및 MFC 이용|RaspberryPi – ServerApplication – ClientMFC(119) - Server|양방향 Socket 통신 / App 이용|
|MQ-7B, MQ-4 센서 유독가스 검출|임베디드|Raspberry Pi / MQ-7B, MQ-4 이용|가스 감지 및 상황 전달|Raspberry Pi / ADC 변환 사용|
|LED, 부저 -> 상황 전달|임베디드|Raspberry Pi / LED, 부저 사용|LED, 부저|상황에 따른 3단계 LED 및 부저|




### 4. 결과물 성능 분석
|핵심 필수 기능|최초 성능 목표|구현 성능|달성 여부 분석|
|:---:|:---:|:---:|:---:|
|Client-Server / 양방향 Full-Duplex TCP 통신|50번 전송 시, 최소한 49번 성공 & 지연: 1초 이내|MultiThread TCP 소켓 통신 이용 & Client-Server 간 2개의 양방향 Full-Duplex 통신 정보 전달|다수 MultiThread 이용 & 양방향 Socket 통신 구현 완료|
|MQ-7B, MQ-4 센서 유독가스 검출|최소 2초 이내 감지| 센서 측정 아날로그 값 → ADC 모듈 → Digital 값 변환, 출력|구현 완료|
|LED, 부저 -> 상황 전달|최소 2초 이내 작동|가스 수치 → RGB LED 변화 & 기준 초과 → 부저 작동 & 상황 개선 팬 작동|구현 완료|
|App → RaspberryPi 사용자 경험(UX) 제공|최소 2초이내 작동 & 지연: 1초 이내|Raspberry Pi에서 시스템 기능 전달 시, 메시지가 섞이지 않도록 MultiThread 구현|APP 이용 & RaspberryPi 제어 완료|


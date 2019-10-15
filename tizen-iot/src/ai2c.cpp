/*****************************************************************
 *
 * Copyright (c) 2019 TEAM MEMAKER. All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

//기존의 Nubison 예제 파일을 활용하여 구성함.
//미메이커 미세먼지 공기청정기 프로젝트를 위해 수정 및 추가 작업을 진행함.

#include <tizen.h>
#include <service_app.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Ecore.h>
#include <unistd.h>

//Nubison 예제파일은 C++기반으로 작성되어 C로 작성된 헤더파일 extern 변환하여 include
extern "C" {
	//Thingspark 헤더파일
	#include <thingspark.h>
	#include <tizen_2_thingspark.h>

	//아두이노 I2C 통신관련 헤더파일
	#include "resource.h"
}



// Nubison IoT 연계 모듈 헤더파일
#include "nubisonif.hpp"

#include "log.h"


#define BUFSIZE 32

// Nubison 클라우드에서 발급하는 인식키
// 연결장치의 제품 식별키를 입력하기
char mytoken[32] = "####-####-####-####";//이번 해커톤에 활용가능한 API KEY : team17@tizen.org계정
										 //해당 계정은 해커톤 이후에 삭제 될 예정

// Nubison IoT 클라우드 서비스 주소(테스트서버 주소)
char cloudifaddress[64] = "nubisoniot.com";

// 클라우드 접속 포트 번호
int  cloudifport = 1883;

//Nubison IoT 연계 모듈 클래스 인스턴스
static NubisonIF *cloudif = NULL;


////////////////// 아두이노 I2C 통신 버스 설정//////////////
//////////////라즈베리파이에는 버스가 하나 존재/////////////
#define I2C_BUS_NUMBER (1)
////////////////// 아두이노와 I2C 통신 인터벌 설정//////////
#define ARDUINO_GATHER_INTERVAL (0.7f)

///////////////// 아두이노 수신데이터 구조체///////////////
typedef struct app_data_s {
	Ecore_Timer *getter_arduino;
} app_data;

//Ecore 타이머를 통해 타이머 Thread 설정
Ecore_Timer *timer = NULL;


// app_data를 활용하여 app간 호출 시 참조 변수 활용
static app_data *g_ad = NULL;

//*이번 프로젝트에 가장 중요한 값들이 들어오는 전역 변수*
//각각 변수는 다음과 같은 의미를 갖게 된다.
//value[0] PM1.0의 미세먼지 센서 값
//value[1] PM2.5의 미세먼지 센서 값
//value[2] PM2.5+의 미세먼지 센서 값
//value[3] Fan Speed의 단계
//value[4] AUTO 모드 여부
uint16_t value[] = {0,0,0,0,0};//variables for data from arduino with I2C




// Nubison Cloud과 통신하는 콜백 함수
//1.Device 의 상태를 조회하는 콜백
//미메이커 공기청정기 프로젝트에서 활용하지 않았음
void NubisonCB_Query(char* rdata, char* api, char* uniqkey)
{
	//DBG("QueryCB : %s %s %s", rdata, api, uniqkey);

	// sendData를 String 자료형 변수로 저장하여 보내주세요.
	//char tmp[BUFSIZE] = {0, };
	//sprintf(tmp, "%d:%d", led0, led1);

	// 클라우드에서 조회 요청이 왔을때 관련된내용을 담아서 전달 함
	// 관련해서 정확히 DB에 Unit 별로 들어 게 하는 것은 클라우드 서버에서 Driver로 셋팅함
	//cloudif->SendtoCloud(tmp, TYPE_STRING, api, uniqkey);
}

//2.Device 의 제어 하는 콜백
//누비슨 앱을 활용하여 명령 데이터를 보내기 위해서 활용된 함수
void NubisonCB_Invoke(char* rdata, char* api, char* uniqkey)
{
	DBG("InvokeCB : %s %s %s", rdata, api, uniqkey);
	//기존 함수에 활용되던 변수 현재 활용 하지 않음
	char tmp[BUFSIZE] = {0, };
	char *word = NULL;

	int ret = 0;

	int sensor_number = 0; //sensor_number는 명령이 들어오는 값에 따라 get_value를 구분하게 된다.
	int get_value = 0;//get_value는 누비슨 앱에서 보내는 데이터의 값이 개별적으로 달라진다.
	//sensor_number == 3 인 경우 ,팬 속도 정보
	//해당 경우 get_value ==0 은 OFF(1단계)
	//해당 경우 get_value ==1 은 50(PWM)
	//해당 경우 get_value ==2 은 100(PWM)
	//해당 경우 get_value ==3 은 150(PWM)
	//해당 경우 get_value ==4 은 200(PWM)
	//해당 경우 get_value ==5 은 250(PWM)

	//sensor_number == 4 인 경우 ,AUTO 모드 정보
	//해당 경우 get_value ==0 은 MANUAL(수동)
	//해당 경우 get_value ==1 은 AUTO(자동)

	//누비슨으로 부터 받은 데이터를 valueSet 배열에 저장 한 후, resource_write_arduino로 송신할 떄 활용
	uint8_t valueSet[2]={0,0};

	strncpy(tmp, rdata, BUFSIZE);

	word = strtok(tmp, ":");
	sensor_number = atoi(word);

	word = strtok(NULL, ":");
	get_value = atoi(word);

	//명령 데이터 상황에 따른 프로세싱
	//automode on이면 팬속도 입력값 무시
	if(sensor_number == 4){
		//위의 설명에 참고
		valueSet[0]=sensor_number;
		if(get_value){
			_D("===========AUTO MODE ON===========");
			valueSet[1]=get_value;
		}else{
			_D("===========AUTO MODE OFF===========");
			valueSet[1]=get_value;
		}
	}else if(sensor_number == 3){
		valueSet[0]=sensor_number;
		_D("===========FAN SPEED INPUT===========");
		if(value[4]==0){
			valueSet[1]=get_value;
			_D("===========FAN SPEED : %d===========", get_value);
		}else{
			_D("Auto Mode is ON");
		}
	}
	ret = resource_write_arduino(I2C_BUS_NUMBER, valueSet);
	_D("2, ret = %d",ret);
	//retv_if(ret != 0, -1);


	// 클라우드에서 조회 요청이 왔을때 관련된내용을 담아서 전달 함
    // 제어가 성공적으로 되었는지확인해서 값을 전달함//
	//cloudif->SendtoCloud(tmp, TYPE_STRING, api, uniqkey);
	//API테스트 용으로 return용 확인 메세지 사용
	cloudif->SendtoCloud((char *)"ok", TYPE_STRING, api, uniqkey);
}

//3.Device 의 하드웨어 설정 하는 콜백
//수정하지 않음
void NubisonCB_Setting(char* rdata, char* api, char* uniqkey)
{
	DBG("SettingCB : %s %s %s", rdata, api, uniqkey);

	// 클라우드에서 조회 요청이 왔을때 관련된내용을 담아서 전달 함
	// 설정 성공적으로 되었는지확인해서 값을 전달함//
	cloudif->SendtoCloud((char*)"ok", TYPE_STRING, api, uniqkey);
}

//4.Device 의 하드웨어 상태를 체크 하는 콜백
//수정하지 않음
void NubisonCB_Check(char* rdata, char* api, char* uniqkey)
{
	DBG("CheckCB : %s %s %s", rdata, api, uniqkey);

	// 클라우드에서 조회 요청이 왔을때 관련된내용을 담아서 전달 함
	// 상태체크 성공적으로 되었는지확인해서 값을 전달함//
	cloudif->SendtoCloud((char*)"Success.", TYPE_STRING, (char*)"check", uniqkey);
}

//5.Device 의 클라우드와의 인증 관련 콜백
//수정하지 않음
void NubisonCB_AUTHO(int authocode)
{
	DBG("AUTHOCB : %d", authocode);

	if (authocode == NUBISONIOT_AUTHO_OK) {
		//1.인증에 정상적으로
		DBG("NUBISONIOT_AUTHO_OK");
	} else if (authocode == NUBISONIOT_AUTHO_ERROR_TESTTIME_EXPIRE) {
		//2.테스트 계정기간이 만료
		DBG("NUBISONIOT_AUTHO_ERROR_TESTTIME_EXPIRE");
	} else if (authocode == NUBISONIOT_AUTHO_ERROR_UNAUTHORIZED_USE) {
		//3.인증되지 않은 계정
		DBG("NUBISONIOT_AUTHO_ERROR_UNAUTHORIZED_USE");
	}
}

//본 프로젝트를 위해 사용자에 의해 편집됨
Eina_Bool app_idler(void *data)
{
	NubisonIF *nubif = (NubisonIF * )data;
	char tmp[BUFSIZE] = {0, };

	if(nubif != NULL) {
		nubif->Loop();
	}
	//value 배열에 저장된 Arduino로 부터 수신된 센서와 기타 데이터 값을 누비슨 클라우드로 송신함
	//송신 프로토콜은 순서대로 값:값:값:값:값 의 형태를 취함
	sprintf(tmp, "%d:%d:%d:%d:%d", value[0], value[1], value[2], value[3], value[4]);
	nubif->NotitoCloud(tmp, TYPE_STRING, mytoken, 1);


    return ECORE_CALLBACK_RENEW;
}

//thingspark에서 활용되는 함수 본 프로젝트 활용을 위해 편집됨
Eina_Bool _get_sensor_value(void *data) {

	int ret = 0;
	tp_handle_h handle = NULL;

	// TODO API_KEY 변경
	// thingspark.kr 에서 채널별로 부여되는 api-key를 입력합니다.
	ret = tp_initialize("#############", &handle);
	retv_if(ret != 0, -1);


	//thingspark에서 활용되는 부분으로 각 변수는 센서의 값을 저장하도록 함
	//char s1[10];               // 변환한 문자열을 저장할 배열
	char PM10[5];               // 변환한 문자열을 저장할 배열
	char PM25[5];               // 변환한 문자열을 저장할 배열
	char PM25_[5];               // 변환한 문자열을 저장할 배열
	char FanSpeed[5];               // 변환한 문자열을 저장할 배열

	//thingspark에서는 char값을 활용하므로 변환 저장
	sprintf(PM10, "%d", value[0]);   // %f를 지정하여 실수를 문자열로 저장
	sprintf(PM25, "%d", value[1]);   // %f를 지정하여 실수를 문자열로 저장
	sprintf(PM25_, "%d", value[2]);   // %f를 지정하여 실수를 문자열로 저장
	sprintf(FanSpeed, "%d", value[3]);   // %f를 지정하여 실수를 문자열로 저장

	//각 데이터를 핸들을 활용하여 각 입력 순서 지정후 ret확인
	ret = tp_set_field_value(handle, 1, PM10);
	retv_if(ret != 0, -1);
	ret = tp_set_field_value(handle, 2, PM25);
	retv_if(ret != 0, -1);
	ret = tp_set_field_value(handle, 3, PM25_);
	retv_if(ret != 0, -1);
	ret = tp_set_field_value(handle, 4, FanSpeed);
	retv_if(ret != 0, -1);


	//thingspark 데이터 송신
	ret = tp_send_data(handle);
	retv_if(ret != 0, -1);

	//송신 마무리
	ret = tp_finalize(handle);
	retv_if(ret != 0, -1);

	return ECORE_CALLBACK_RENEW;
}

/////////////////////////아두이노 I2C통신 관련 함수부 시작//////////////////////////////
//I2C로 통신하는 아두이노 호출하는 callback함수
static Eina_Bool __get_arduino_cb(void *data)
{
	int ret = 0;

	//resource_read_arduino를 통해 아두이노 값 수신
	ret = resource_read_arduino(I2C_BUS_NUMBER, value);
	retv_if(ret != 0, ECORE_CALLBACK_RENEW);
	//_D("Check address value : %d(%d)", value, &value);
	_D("Data from Arduino through I2C communication : %d(over PM1.0), %d(under PM2.5), %d(over PM2.5), %d(FAN SPEED), %d(AUTO:1, MANUAL:0)", value[0], value[1], value[2], value[3], value[4]);

	return ECORE_CALLBACK_RENEW;
}

void gathering_stop(void *data)
{
	app_data *ad = (app_data *)data;
	_D("1, ad = %d", *ad);
	ret_if(!ad);

	if (ad->getter_arduino) {
		ecore_timer_del(ad->getter_arduino);
		ad->getter_arduino = NULL;
	}
}


void gathering_start(void *data)
{
	app_data *ad = (app_data *)data;
	_D("1, ad = %d", *ad);
	ret_if(!ad);

	gathering_stop(ad);

	//ecore_timer를 이용한 주기적인 정보획득 API등록
	ad->getter_arduino = ecore_timer_add(ARDUINO_GATHER_INTERVAL, __get_arduino_cb, NULL);
	_D("1, ad->getter_arduino = %p", ad->getter_arduino);
	if (!ad->getter_arduino)
		_E("Failed to add getter_arduino");
}

/////////////////////////아두이노 I2C통신 관련 함수부 종료////////////////////////////////



static bool service_app_create(void *user_data)
{
	DBG(">>>>>>>>>>> App create");

	//1. Nubison IoT 연계 모듈 생
	cloudif = new NubisonIF();

	//2.콜백 함수를 설정
	cloudif->SetCBFuntion(NubisonCB_Query, NubisonCB_Invoke, NubisonCB_Setting, NubisonCB_Check, NubisonCB_AUTHO);

	//3. 클라우드 접속 정보를 설정해서 초기화
	int ret = cloudif->Init(cloudifaddress, cloudifport, mytoken);
	if(ret!=0) {
		ERR("Failed to connect with Nubison IoT");
	}

	ecore_idler_add(app_idler, cloudif);

	//ecore timer 등록으로 thingspark 연결 가능할 경우 통신
	timer = ecore_timer_add(30.0f, _get_sensor_value, NULL);
	if (!timer){
		_E("cannot add a timer(%s)", "Cannot register ecore timer for thingspark data transmission");
	}

	return true;
}

static void service_app_terminate(void *user_data)
{
	_D("terminate");
	// terminate ecore timer before program ends
	if (timer) {
		ecore_timer_del(timer);
		timer = NULL;
	}
	resource_close_arduino();
	gathering_stop(user_data);
}

static void service_app_control(app_control_h app_control, void *user_data)
{
	_D("control");
	gathering_start(user_data);
}

int main(int argc, char *argv[])
{
	//C++맞게 app_data casting
	app_data *ad = NULL;
	service_app_lifecycle_callback_s event_callback;
	ad = (app_data *)calloc(1, sizeof(app_data));

	g_ad = ad;
	event_callback.create = service_app_create;
	event_callback.terminate = service_app_terminate;
	event_callback.app_control = service_app_control;

	return service_app_main(argc, argv, &event_callback, ad);
}

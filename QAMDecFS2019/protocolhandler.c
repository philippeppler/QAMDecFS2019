/*
 * protocolhandler.c
 *
 * Created: 25.05.2019 13:28:40
 *  Author: PhilippAdmin
 */ 

#include "avr_compiler.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "protocolhandler.h"
#include "string.h"

// KONSTANTEN
#define ANZSENDQUEUE					32
#define	PROTOCOLBUFFERSIZE				32
// xQuelle
#define PAKET_TYPE_ALDP					0x01
#define ALDP_SRC_UART					0x00
#define ALDP_SRC_I2C					0x01
#define ALDP_SRC_TEST					0x02
#define ALDP_SRC_ERROR					0xFF

// xSettings
#define Settings_QAM_Order				1<<0
#define Settings_Source_Bit1			1<<1
#define Settings_Source_Bit2			1<<2
#define Settings_Frequency				1<<3
// xStatus
#define Status_Error					1<<1
#define Status_Daten_ready				1<<2
#define Status_Daten_sending			1<<3
// xProtocolBufferStatus
#define BUFFER_A_freetouse				1<<0
#define BUFFER_B_freetouse				1<<1

EventGroupHandle_t xSettings;								// Settings vom GUI von Cedi
EventGroupHandle_t xStatus;									// auch irgendwas von Cedi

EventGroupHandle_t xProtocolBufferStatus;					// Eventbits for Buffer-Status

xQueueHandle xALDPQueue;									// Queue from Protocolhandler to Main-Task

//globale Variablen
uint8_t ucglobalProtocolBuffer_A[PROTOCOLBUFFERSIZE] = {};	// Buffer_A from Demodulator to ProtocolTask
uint8_t ucglobalProtocolBuffer_B[PROTOCOLBUFFERSIZE] = {};	// Buffer_B from Demodulator to ProtocolTask


void vProtokollHandlerTask(void *pvParameters) {
	(void) pvParameters;

	struct ALDP_t_class *xALDP_Paket;
	struct SLDP_t_class xSLDP_Paket;
	uint8_t ucBuffer_A_Position = 0;								// position inside the used buffer (A or B)
	uint8_t ucBuffer_B_Position = 0;								// position inside the used buffer (A or B)
	
	uint8_t ucBufferSLDPpayloadInput[]= {};
	uint8_t ucBufferSLDPpayloadInputCounter;
	
	xALDPQueue = xQueueCreate(ANZSENDQUEUE, sizeof(uint8_t));

	for(;;) {
		
		if (xEventGroupGetBits(xProtocolBufferStatus) & BUFFER_A_freetouse) {
			xEventGroupClearBits(xProtocolBufferStatus, BUFFER_A_freetouse );
			xSLDP_Paket.sldp_size = ucglobalProtocolBuffer_A[ucBuffer_A_Position];
			
			for (ucBufferSLDPpayloadInputCounter = 0; ucBufferSLDPpayloadInputCounter < xSLDP_Paket.sldp_size; ucBufferSLDPpayloadInputCounter++) {
			
				for (ucBuffer_A_Position = xSLDP_Paket.sldp_size; ucBuffer_A_Position < PROTOCOLBUFFERSIZE; ucBuffer_A_Position++) {
					ucBufferSLDPpayloadInput[ucBufferSLDPpayloadInputCounter] = ucglobalProtocolBuffer_A[ucBuffer_A_Position];
			
				}
			
			
			
			}

			
			
			
			/*
			xSLDP_Paket.sldp_crc8 = ucglobalProtocolBuffer_A[ucBufferPosition+1+xSLDP_Paket.sldp_size];
			xSLDP_Paket.sldp_payload = &ucglobalProtocolBuffer_A[ucBufferPosition+1];
			xALDP_Paket = (struct ALDP_t_class *)xSLDP_Paket.sldp_payload;*/
			
		}
		













		vTaskDelay(50 / portTICK_RATE_MS);				// Delay 50ms
	}
}
#include <cmath>

#include "UdpMsg.h"
#include "DataUDP.h"

	UdpMsg::UdpMsg() {
		memset(&(this->topic), 0, BUFLEN);
		pSize = 0;
	}

	/*
	*	Before using this constructor the cp param
	* !!must!! have '\0' at the end of the payload.
	*/
	UdpMsg::UdpMsg(const UdpMsg& cp, int size) {
		memset(&(this->topic), 0, BUFLEN);
		strcpy(this->topic, cp.topic);
		this->data_type = cp.data_type;
		memcpy(this->payload, cp.payload, size);
		pSize = size;
	}

	/*
	*	Before using this constructor the cp param
	* !!must!! have '\0' at the end of the payload.
	*/
	UdpMsg::UdpMsg(const char* topic, const DataUDP& data, int size) {

		memset(&(this->topic), 0, BUFLEN);
		
		strcpy(this->topic, topic);
		this->data_type = data.data_type;
		memcpy(this->payload, data.payload, size);
		pSize = size;
	}

	uint8_t UdpMsg::getDataType() {
		return uint8_t(data_type);
	}

	uint32_t UdpMsg::getInt() {
			DIE(data_type != (int)DataType::INT,
				"tried to get int from nonIntUDPMsg");
			uint32_t* intP;  // for DataType::INT and DataType::FLOAT value
			uint32_t resInt;
			resInt = (payload[0] == 0 ? 1 : -1);  // sign decode
			intP = (uint32_t* )(payload + 1);
			resInt *= htonl(*intP);
			return resInt;
	}

	double UdpMsg::getShortReal() {
		DIE(data_type != (int)DataType::SHORT_REAL,
			"tried to get shortReal from nonshortRealUDPMsg");
		uint16_t* shortP;  // for DataType::SHORT_REAL
		double resShort;  // result get from short, BUT type is double
		shortP = (uint16_t* )(payload);
		resShort = htons(*shortP) / 100.;
		return resShort;
	}

	double UdpMsg::getFloat() {
		DIE(data_type != (int)DataType::STRING,
			"tried to get String from nonStringUDPMsg");
		uint32_t resInt;
		uint32_t* intP;
		uint8_t* decP;  // for DataType::FLOAT
		uint8_t decPow;
		double resFloat;

		resInt = (payload[0] == 0 ? 1 : -1);
		intP = (uint32_t* )(payload + 1);
		resInt *= htonl(*intP);
		decP = (uint8_t* )(payload + 5);
		decPow = *decP;
		// int cast ensures that the value will be converted as result
		// to double not the bytes.
		resFloat = (double)((int)resInt / (double)pow(10, decPow));
		return resFloat;
	}

	const char* UdpMsg::getString() {
		return payload;
	}

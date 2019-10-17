#include <cmath>

#include "DataUDP.h"

	DataUDP::DataUDP() {
		memset(&data_type, 0, BUFLEN - TOPIC_SIZE);
		pSize = 0;
	}

	DataUDP::DataUDP(const UdpMsg& cp, int size, struct sockaddr_in& addr1) {
		memset(&data_type, 0, BUFLEN - TOPIC_SIZE);
		this->data_type = cp.data_type;
		memcpy(this->payload, cp.payload, size);
		pSize = size;
		memcpy(&(this->addr), &addr1, sizeof(addr1));
	}

	DataUDP::DataUDP(const DataUDP& cp, int size, struct sockaddr_in& addr1) {
		memset(&data_type, 0, BUFLEN - TOPIC_SIZE);
		this->data_type = cp.data_type;
		memcpy(this->payload, cp.payload, size);
		pSize = size;
		memcpy(&(this->addr), &addr1, sizeof(addr1));
	}

	uint8_t DataUDP::getDataType() {
		return uint8_t(data_type);
	}

	uint32_t DataUDP::getInt() {
			DIE(data_type != (int)DataType::INT,
				"tried to get int from nonIntUDPMsg");
			uint32_t* intP;  // for DataType::INT and DataType::FLOAT value
			int resInt;
			resInt = (payload[0] == 0 ? 1 : -1);  // sign decode
			intP = (uint32_t* )(payload + 1);
			resInt *= htonl(*intP);
			return resInt;
	}

	double DataUDP::getShortReal() {
		DIE(data_type != (int)DataType::SHORT_REAL,
			"tried to get shortReal from nonshortRealUDPMsg");
		uint16_t* shortP;  // for DataType::SHORT_REAL
		double resShort;  // result get from short, BUT type is double
		shortP = (uint16_t* )(payload);
		resShort = htons(*shortP) / 100.;
		return resShort;
	}

	double DataUDP::getFloat() {
		DIE(data_type != (int)DataType::FLOAT,
			"tried to get FLOAT from nonFloatUDPMsg");
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

	const char* DataUDP::getString() {
		DIE(data_type != (int)DataType::STRING,
			"tried to get STRING om nonStringUDPMsg");
		return payload;
	}


  /*DataUDP& DataUDP::operator=(const DataUDP& cp) {
    memset(&data_type, 0, BUFLEN - TOPIC_SIZE);
		this->data_type = cp.data_type;
		strcpy(this->payload, cp.payload);
		pSize = size;  // cum dau size ca param
    return *this;
  }*/
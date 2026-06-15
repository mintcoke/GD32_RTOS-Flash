#ifndef __ECAT_PDO_TYPES_H
#define __ECAT_PDO_TYPES_H

#include <stdint.h>

#pragma pack(push, 1)

struct TxData {
    uint16_t DI_bits;
    uint16_t sensor_value;
    uint16_t temperature;
    uint32_t status_code;
    uint32_t uptime_counter;
};

struct RxData {
    uint16_t DO_bits;
    uint8_t  control_mode;
    uint16_t target_speed;
    uint32_t config_param;
};

#pragma pack(pop)

#endif /* __ECAT_PDO_TYPES_H */

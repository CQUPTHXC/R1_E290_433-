#ifndef  _PAN_PARAM_H_
#define  _PAN_PARAM_H_

#include "pan_rf.h"

extern const uint8_t reg_agc_freq400[40];
extern const uint8_t reg_agc_freq800[40];
extern const power_ramp_t power_ramp[RF_MAX_RAMP][4];
extern const power_ramp_cfg_t power_ramp_cfg[RF_MAX_RAMP + 1];
extern const pan_reg_cfg_t g_reg_cfg[];
extern const uint32_t g_reg_cfg_len;
extern const uint32_t freq_table[16][2];
extern const uint8_t freq_param_table[16];

#endif // ! _PAN_PARAM_H_

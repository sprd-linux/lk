#ifndef __REGULATOR_CONSUMER_H_
#define __REGULATOR_CONSUMER_H_

int regulator_disable(const char con_id[]);

int regulator_enable(const char con_id[]);

int regulator_set_voltage(const char con_id[], int to_vol);

int regulator_pmic_init(void);

#endif //__REGULATOR_CONSUMER_H_

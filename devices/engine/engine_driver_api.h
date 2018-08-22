#ifndef ENGINE_DRIVER_API_H
#define ENGINE_DRIVER_API_H
#include <stdint.h>
#include "../../main.h"


void ENGINE_init_driver(void);
void ENGINE_run_forvard_sm(float speed);
void ENGINE_run_forvard(void);
void ENGINE_run_backvard_sm(float speed);
void ENGINE_run_backvard(void);
void ENGINE_stop(void);
void ENGINE_rotate_clockvice_ang(uint32_t angle);
void ENGINE_rotate_clockvice(void);
void ENGINE_rotate_unticlockvice_ang(uint32_t angle);
void ENGINE_rotate_unticlockvice(void);
#endif /*ENGINE_DRIVER_API_H*/

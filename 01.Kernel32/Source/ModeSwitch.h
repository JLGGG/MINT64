/*
 * ModeSwitch.h
 *
 *  Created on: Mar 12, 2018
 *      Author: root
 */

#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "Types.h"

void kReadCPUID(DWORD dwEAX, DWORD* pdwEAX, DWORD* pdwEBX, DWORD* pdwECX, DWORD* pdwEDX);
void kSwitchAndExecute64bitKernel(void);


#endif

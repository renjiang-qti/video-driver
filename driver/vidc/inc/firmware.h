/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2022, The Linux Foundation. All rights reserved.
 * Copyright (c) 2023 Qualcomm Innovation Center, Inc. All rights reserved.
 */

#ifndef _MSM_VIDC_FIRMWARE_H_
#define _MSM_VIDC_FIRMWARE_H_

#include <linux/version.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(6, 3, 0))
#include <linux/firmware/qcom/qcom_scm.h>
#else
#include <linux/qcom_scm.h>
#endif

struct msm_vidc_core;

int fw_load(struct msm_vidc_core *core);
int fw_unload(struct msm_vidc_core *core);
int fw_suspend(struct msm_vidc_core *core);
int fw_resume(struct msm_vidc_core *core);
void fw_coredump(struct msm_vidc_core *core);

#endif

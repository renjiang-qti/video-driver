/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#ifndef _MSM_VIDC_KODIAK_H_
#define _MSM_VIDC_KODIAK_H_

struct msm_vidc_core;

#if defined(CONFIG_MSM_VIDC_QLI)
int msm_vidc_get_platform_data_kodiak(struct msm_vidc_core *core);
int msm_vidc_init_platform_kodiak(struct msm_vidc_core *core);
int msm_vidc_adjust_ir_period_kodiak(void *instance, struct v4l2_ctrl *ctrl);
int msm_vidc_set_ir_period_kodiak(void *instance,
				  enum msm_vidc_inst_capability_type cap_id);
#else
int msm_vidc_get_platform_data_kodiak(struct msm_vidc_core *core)
{
	return -EINVAL;
}

int msm_vidc_init_platform_kodiak(struct msm_vidc_core *core)
{
	return -EINVAL;
}

int msm_vidc_adjust_ir_period_kodiak(void *instance, struct v4l2_ctrl *ctrl)
{
	return -EINVAL;
}

int msm_vidc_set_ir_period_kodiak(void *instance,
				  enum msm_vidc_inst_capability_type cap_id)
{
	return -EINVAL;
}

#endif
#endif // _MSM_VIDC_KODIAK_H_

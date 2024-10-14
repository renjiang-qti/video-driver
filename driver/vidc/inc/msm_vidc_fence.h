/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2022-2024 Qualcomm Innovation Center, Inc. All rights reserved.
 */

#ifndef __H_MSM_VIDC_FENCE_H__
#define __H_MSM_VIDC_FENCE_H__

struct msm_vidc_core;
struct msm_vidc_inst;
struct msm_vidc_fence;
enum msm_vidc_buffer_type;

void populate_fence_name(struct msm_vidc_inst *inst,
	struct msm_vidc_fence *f, bool override_tl);
int msm_vidc_fence_init(struct msm_vidc_inst *inst);
void msm_vidc_fence_deinit(struct msm_vidc_inst *inst);
int msm_vidc_put_sw_fence(struct msm_vidc_inst *inst,
	struct msm_vidc_fence *fence, bool is_error);
struct msm_vidc_fence *msm_vidc_get_sw_fence(
	struct msm_vidc_inst *inst, struct list_head *fence_list,
	enum msm_vidc_buffer_type buf_type, bool is_imported);
int msm_vidc_get_sw_fence_fd(struct msm_vidc_inst *inst,
	struct msm_vidc_fence *fence);

#define call_fence_op(c, op, ...)                  \
	(((c) && (c)->fence_ops && (c)->fence_ops->op) ? \
	((c)->fence_ops->op(__VA_ARGS__)) : 0)

struct msm_vidc_fence_ops {
	int (*fence_register)(struct msm_vidc_core *core);
	int (*fence_deregister)(struct msm_vidc_core *core);
	struct msm_vidc_fence *(*fence_create)(
		struct msm_vidc_inst *inst, struct list_head *fence_list,
		enum msm_vidc_buffer_type buf_type);
	struct msm_vidc_fence *(*fence_import)(
		struct msm_vidc_inst *inst, struct list_head *fence_list,
		enum msm_vidc_buffer_type buf_type);
	int (*fence_create_fd)(struct msm_vidc_inst *inst, struct msm_vidc_fence *fence);
	int (*fence_signal)(struct msm_vidc_inst *inst, struct msm_vidc_fence *fence);
	int (*fence_destroy)(struct msm_vidc_inst *inst,
		struct msm_vidc_fence *fence, bool is_error);
	int (*fence_release)(struct msm_vidc_inst *inst,
		struct msm_vidc_fence *fence, bool is_error);
	void (*fence_recover)(struct msm_vidc_core *core);
	int (*fence_enable_resources)(struct msm_vidc_core *core, bool state);
};

const struct msm_vidc_fence_ops *get_dma_fence_ops(void);

#endif // __H_MSM_VIDC_FENCE_H__

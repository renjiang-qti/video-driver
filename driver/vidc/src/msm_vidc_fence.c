// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2022-2024 Qualcomm Innovation Center, Inc. All rights reserved.
 */

#include <linux/sync_file.h>
#include <linux/file.h>

#include "msm_vidc_fence.h"
#include "msm_vidc_driver.h"
#include "msm_vidc_debug.h"

static const char *fence_dir_name(enum msm_vidc_fence_direction dir)
{
	switch (dir) {
	case MSM_VIDC_FENCE_DIR_TX:  return "tx";
	case MSM_VIDC_FENCE_DIR_RX:  return "rx";
	default:                     return "none";
	}

	return "none";
}

static const char *msm_vidc_dma_fence_get_driver_name(struct dma_fence *df)
{
	return "msm_vidc_fence";
}

static const char *msm_vidc_dma_fence_get_timeline_name(struct dma_fence *df)
{
	struct msm_vidc_fence *fence = container_of(df, struct msm_vidc_fence, dma_fence);

	return fence->name;
}

static void msm_vidc_dma_fence_release(struct dma_fence *df)
{
	struct msm_vidc_fence *fence = container_of(df, struct msm_vidc_fence, dma_fence);

	d_vpr_l("%s: name %s\n", __func__, fence->name);
	vfree(fence);
}

static const struct dma_fence_ops msm_vidc_dma_fence_ops = {
	.get_driver_name = msm_vidc_dma_fence_get_driver_name,
	.get_timeline_name = msm_vidc_dma_fence_get_timeline_name,
	.release = msm_vidc_dma_fence_release,
};

void populate_fence_name(struct msm_vidc_inst *inst,
	struct msm_vidc_fence *f, bool override_tl)
{
	snprintf(f->name, sizeof(f->name),
		"%sfence: %s: %s: %s: fd %3d id %10llu mid %5llu f.no %5llu",
		override_tl ? inst->fence_context.name : "",
		f->imp_fence ? "input" : "output",
		fence_dir_name(f->direction),
		f->session ? "hw" : "sw",
		f->fd,
		f->fence_id,
		f->fence_id & 0x7fffffff,
		f->seqno);
}

int msm_vidc_fence_init(struct msm_vidc_inst *inst)
{
	int rc = 0;

	inst->fence_context.ctx_num = dma_fence_context_alloc(1);
	inst->fence_context.input_seq_num = 0;
	inst->fence_context.output_seq_num = 0;
	snprintf(inst->fence_context.name, sizeof(inst->fence_context.name),
		"%s: ", inst->debug_str);
	i_vpr_h(inst, "%s: %s\n", __func__, inst->fence_context.name);

	return rc;
}

void msm_vidc_fence_deinit(struct msm_vidc_inst *inst)
{
	i_vpr_h(inst, "%s: %s\n", __func__, inst->fence_context.name);
	inst->fence_context.ctx_num = 0;
	inst->fence_context.input_seq_num = 0;
	inst->fence_context.output_seq_num = 0;
	snprintf(inst->fence_context.name, sizeof(inst->fence_context.name),
		"%s", "");
}

int msm_vidc_put_sw_fence(struct msm_vidc_inst *inst,
	struct msm_vidc_fence *fence, bool is_error)
{
	/* remove fence entry from list */
	list_del_init(&fence->list);

	if (fence->imp_fence) {
		dma_fence_put(fence->imp_fence);
		vfree(fence);
	} else {
		/* send error signal incase of error */
		if (is_error) {
			dma_fence_set_error(&fence->dma_fence, -EINVAL);
			dma_fence_signal(&fence->dma_fence);
		}
		/* override fence name with timeline for raw fence */
		populate_fence_name(inst, fence, true);
		dma_fence_put(&fence->dma_fence);
	}

	return 0;
}

struct msm_vidc_fence *msm_vidc_get_sw_fence(
	struct msm_vidc_inst *inst, struct list_head *fence_list,
	enum msm_vidc_buffer_type buf_type, bool is_imported)
{
	enum msm_vidc_fence_type f_type = get_fence_type(inst, buf_type);
	enum msm_vidc_fence_direction f_dir = get_fence_direction(inst, buf_type);
	struct msm_vidc_fence *fence = NULL;
	struct dma_fence *imp_dma_fence = NULL;
	u64 fence_seqno = 0, fence_id = 0;
	int fence_fd = INVALID_FD;

	if (f_type == MSM_VIDC_FENCE_NONE) {
		i_vpr_e(inst, "%s: invalid fence type\n", __func__);
		return NULL;
	}
	if (f_dir == MSM_VIDC_FENCE_DIR_NONE) {
		i_vpr_e(inst, "%s: invalid fence direction\n", __func__);
		return NULL;
	}

	fence = vzalloc(sizeof(struct msm_vidc_fence));
	if (!fence) {
		i_vpr_e(inst, "%s: allocation failed\n", __func__);
		return NULL;
	}

	if (is_imported) {
		fence_fd = inst->capabilities[INPBUF_FENCE_FD].value;
		if (fence_fd == INVALID_FD) {
			i_vpr_e(inst, "%s: Invalid dma fence fd!\n", __func__);
			goto error;
		}
		inst->capabilities[INPBUF_FENCE_FD].value = INVALID_FD;

		imp_dma_fence = sync_file_get_fence(fence_fd);
		if (!imp_dma_fence) {
			i_vpr_e(inst, "%s: getting dma fence failed\n", __func__);
			goto error;
		}
		fence_seqno = ++inst->fence_context.input_seq_num;
		/* reset seqno to avoid going beyond INT_MAX */
		if (fence_seqno >= INT_MAX)
			inst->fence_context.input_seq_num = 0;
		fence_id = imp_dma_fence->seqno;
	} else {
		fence_seqno = ++inst->fence_context.output_seq_num;
		/* reset seqno to avoid going beyond INT_MAX */
		if (fence_seqno >= INT_MAX)
			inst->fence_context.output_seq_num = 0;
		spin_lock_init(&fence->lock);
		dma_fence_init(&fence->dma_fence, &msm_vidc_dma_fence_ops,
			&fence->lock, inst->fence_context.ctx_num, fence_seqno);
		fence_id = fence->dma_fence.seqno;
	}

	fence->fd = fence_fd;
	fence->imp_fence = imp_dma_fence;
	fence->sync_file = NULL;
	fence->session = NULL;
	fence->type = f_type;
	fence->direction = f_dir;
	fence->fence_id = fence_id;
	fence->seqno = fence_seqno;

	/* prepare sw fence name */
	populate_fence_name(inst, fence, false);

	/* insert into fence_list */
	INIT_LIST_HEAD(&fence->list);
	list_add_tail(&fence->list, fence_list);

	return fence;

error:
	vfree(fence);
	return NULL;
}

int msm_vidc_get_sw_fence_fd(struct msm_vidc_inst *inst,
	struct msm_vidc_fence *fence)
{
	int rc = 0;

	fence->fd = get_unused_fd_flags(0);
	if (fence->fd < 0) {
		i_vpr_e(inst, "%s: getting fd (%d) failed\n", __func__,
			fence->fd);
		rc = -EINVAL;
		goto err_fd;
	}
	fence->sync_file = sync_file_create(&fence->dma_fence);
	if (!fence->sync_file) {
		i_vpr_e(inst, "%s: sync_file_create failed\n", __func__);
		rc = -EINVAL;
		goto err_sync_file;
	}
	fd_install(fence->fd, fence->sync_file->file);
	/* prepare fence name with fd */
	populate_fence_name(inst, fence, false);

	return 0;

err_sync_file:
	put_unused_fd(fence->fd);
err_fd:
	return rc;
}

static struct msm_vidc_fence *msm_vidc_fence_create(
	struct msm_vidc_inst *inst, struct list_head *fence_list,
	enum msm_vidc_buffer_type buf_type)
{
	struct msm_vidc_fence *fence = NULL;

	/* create dma fence */
	fence = msm_vidc_get_sw_fence(inst, fence_list, buf_type, false);
	if (!fence) {
		i_vpr_e(inst, "%s: failed to create sw fence\n", __func__);
		return NULL;
	}

	return fence;
}

static struct msm_vidc_fence *msm_vidc_fence_import(
	struct msm_vidc_inst *inst, struct list_head *fence_list,
	enum msm_vidc_buffer_type buf_type)
{
	struct msm_vidc_fence *fence = NULL;

	/* import dma fence */
	fence = msm_vidc_get_sw_fence(inst, fence_list, buf_type, true);
	if (!fence) {
		i_vpr_e(inst, "%s: failed to import sw fence\n", __func__);
		return NULL;
	}

	return fence;
}

int msm_vidc_fence_create_fd(struct msm_vidc_inst *inst,
	struct msm_vidc_fence *fence)
{
	int rc = 0;

	rc = msm_vidc_get_sw_fence_fd(inst, fence);
	if (rc) {
		i_vpr_e(inst, "%s: failed. %s\n", __func__, fence->name);
		return rc;
	}

	return rc;
}

static int msm_vidc_fence_signal(struct msm_vidc_inst *inst, struct msm_vidc_fence *fence)
{
	int rc = 0;

	/* sanity - only sw output fence is allowed to signal */
	if (fence->imp_fence) {
		i_vpr_e(inst, "%s: unexpected. name %s\n", __func__, fence->name);
		return -EINVAL;
	}

	/* signal sw fence */
	dma_fence_signal(&fence->dma_fence);

	/* remove entry from fence_list */
	rc = msm_vidc_put_sw_fence(inst, fence, false);
	if (rc)
		return rc;

	return rc;
}

static int msm_vidc_fence_destroy(struct msm_vidc_inst *inst,
	struct msm_vidc_fence *fence, bool is_error)
{
	int rc = 0;

	/* sanity - calling fence_destroy for imp fence not expected */
	if (fence->imp_fence) {
		i_vpr_e(inst, "%s: unexpected. name %s\n", __func__, fence->name);
		return -EINVAL;
	}

	/* destroy sw fence */
	rc = msm_vidc_put_sw_fence(inst, fence, is_error);
	if (rc)
		return rc;

	return rc;
}

static int msm_vidc_fence_release(struct msm_vidc_inst *inst,
	struct msm_vidc_fence *fence, bool is_error)
{
	int rc = 0;

	/* sanity - calling fence_release for raw fence not expected */
	if (!fence->imp_fence) {
		i_vpr_e(inst, "%s: unexpected. name %s\n", __func__, fence->name);
		return -EINVAL;
	}

	/* destroy sw fence */
	rc = msm_vidc_put_sw_fence(inst, fence, is_error);
	if (rc)
		return rc;

	return rc;
}

static const struct msm_vidc_fence_ops msm_dma_fence_ops = {
	.fence_create             = msm_vidc_fence_create,
	.fence_import             = msm_vidc_fence_import,
	.fence_create_fd          = msm_vidc_fence_create_fd,
	.fence_signal             = msm_vidc_fence_signal,
	.fence_destroy            = msm_vidc_fence_destroy,
	.fence_release            = msm_vidc_fence_release,
};

const struct msm_vidc_fence_ops *get_dma_fence_ops(void)
{
	return &msm_dma_fence_ops;
}

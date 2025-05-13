// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2020-2021, The Linux Foundation. All rights reserved.
 * Copyright (c) 2023-2025 Qualcomm Innovation Center, Inc. All rights reserved.
 */

#include <media/v4l2-mem2mem.h>
#include <media/videobuf2-v4l2.h>

#include "msm_vidc_v4l2.h"
#include "msm_vidc_internal.h"
#include "msm_vidc_core.h"
#include "msm_vidc_inst.h"
#include "msm_vidc_driver.h"
#include "msm_vidc_debug.h"
#include "msm_vidc.h"
#include "msm_vidc_events.h"

extern struct msm_vidc_core *g_core;

struct video_device *get_video_device(struct msm_vidc_inst *inst)
{
	struct msm_vidc_core *core = inst->core;
	struct video_device *vdev = NULL;

	if (is_decode_session(inst))
		vdev = &core->vdev[0].vdev;
	else if (is_encode_session(inst))
		vdev = &core->vdev[1].vdev;

	return vdev;
}

static struct msm_vidc_inst *get_vidc_inst(struct file *filp, void *fh)
{
	if (!filp || !filp->private_data)
		return NULL;
	return container_of(filp->private_data,
					struct msm_vidc_inst, fh);
}

static int __msm_v4l2_try_fmt(struct msm_vidc_inst *inst, void *data)
{
	return inst->event_handle(inst, MSM_VIDC_TRY_FMT, data);
}

static int __msm_v4l2_s_fmt(struct msm_vidc_inst *inst, void *data)
{
	return inst->event_handle(inst, MSM_VIDC_S_FMT, data);
}

static int __msm_v4l2_reqbufs(struct msm_vidc_inst *inst, void *data)
{
	return inst->event_handle(inst, MSM_VIDC_REQBUFS, data);
}

static int __msm_v4l2_decoder_cmd(struct msm_vidc_inst *inst, void *data)
{
	struct v4l2_decoder_cmd *dec = data;
	enum msm_vidc_event event;

	if (dec->cmd != V4L2_DEC_CMD_START && dec->cmd != V4L2_DEC_CMD_STOP) {
		i_vpr_e(inst, "%s: invalid cmd %#x\n", __func__, dec->cmd);
		return -EINVAL;
	}
	event = (dec->cmd == V4L2_DEC_CMD_START ? MSM_VIDC_CMD_START : MSM_VIDC_CMD_STOP);

	return inst->event_handle(inst, event, NULL);
}

static int __msm_v4l2_encoder_cmd(struct msm_vidc_inst *inst, void *data)
{
	struct v4l2_encoder_cmd *enc = data;
	enum msm_vidc_event event;

	if (enc->cmd != V4L2_ENC_CMD_START && enc->cmd != V4L2_ENC_CMD_STOP) {
		i_vpr_e(inst, "%s: invalid cmd %#x\n", __func__, enc->cmd);
		return -EINVAL;
	}
	event = (enc->cmd == V4L2_ENC_CMD_START ? MSM_VIDC_CMD_START : MSM_VIDC_CMD_STOP);

	return inst->event_handle(inst, event, NULL);
}

unsigned int msm_v4l2_poll(struct file *filp, struct poll_table_struct *pt)
{
	int poll = 0;
	struct msm_vidc_inst *inst = get_vidc_inst(filp, NULL);

	inst = get_inst_ref(g_core, inst);
	if (!inst) {
		d_vpr_e("%s: invalid instance\n", __func__);
		return POLLERR;
	}
	if (is_session_error(inst)) {
		i_vpr_e(inst, "%s: inst in error state\n", __func__);
		poll = POLLERR;
		goto exit;
	}
	poll = msm_vidc_poll((void *)inst, filp, pt);
	if (poll)
		goto exit;

exit:
	put_inst(inst);
	return poll;
}

int msm_v4l2_open(struct file *filp)
{
	struct video_device *vdev = video_devdata(filp);
	struct msm_video_device *vid_dev =
		container_of(vdev, struct msm_video_device, vdev);
	struct msm_vidc_core *core = video_drvdata(filp);
	struct msm_vidc_inst *inst;

	trace_msm_v4l2_vidc_open("START", NULL);
	inst = msm_vidc_open(core, vid_dev->type);
	if (!inst) {
		d_vpr_e("Failed to create instance, type = %d\n",
			vid_dev->type);
		trace_msm_v4l2_vidc_open("END", NULL);
		return -ENOMEM;
	}
	filp->private_data = &(inst->fh);
	trace_msm_v4l2_vidc_open("END", inst);
	return 0;
}

int msm_v4l2_close(struct file *filp)
{
	struct msm_vidc_inst *inst;
	int rc = 0;

	inst = get_vidc_inst(filp, NULL);
	if (!inst) {
		d_vpr_e("%s: invalid instance\n", __func__);
		return -EINVAL;
	}

	trace_msm_v4l2_vidc_close("START", inst);

	rc = msm_vidc_close(inst);
	filp->private_data = NULL;
	trace_msm_v4l2_vidc_close("END", NULL);
	return rc;
}

int msm_v4l2_querycap(struct file *filp, void *fh,
			struct v4l2_capability *cap)
{
	void *instance = get_vidc_inst(filp, fh);

	return msm_vidc_session(instance, msm_vidc_querycap, cap, false, __func__);
}

int msm_v4l2_enum_fmt(struct file *filp, void *fh,
					struct v4l2_fmtdesc *fmtdesc)
{
	void *instance = get_vidc_inst(filp, fh);

	return msm_vidc_session(instance, msm_vidc_enum_fmt, fmtdesc, false, __func__);
}

int msm_v4l2_try_fmt(struct file *filp, void *fh, struct v4l2_format *data)
{
	void *instance = get_vidc_inst(filp, fh);

	return msm_vidc_session(instance, __msm_v4l2_try_fmt, data, false, __func__);
}

int msm_v4l2_s_fmt(struct file *filp, void *fh, struct v4l2_format *fmt)
{
	void *instance = get_vidc_inst(filp, fh);

	return msm_vidc_session(instance, __msm_v4l2_s_fmt, fmt, false, __func__);
}

int msm_v4l2_g_fmt(struct file *filp, void *fh,
					struct v4l2_format *f)
{
	void *instance = get_vidc_inst(filp, fh);

	return msm_vidc_session(instance, msm_vidc_g_fmt, f, false, __func__);
}

int msm_v4l2_s_selection(struct file *filp, void *fh,
					struct v4l2_selection *sel)
{
	void *instance = get_vidc_inst(filp, fh);

	return msm_vidc_session(instance, msm_vidc_s_selection, sel, false, __func__);
}

int msm_v4l2_g_selection(struct file *filp, void *fh,
					struct v4l2_selection *sel)
{
	void *instance = get_vidc_inst(filp, fh);

	return msm_vidc_session(instance, msm_vidc_g_selection, sel, false, __func__);
}

int msm_v4l2_s_parm(struct file *filp, void *fh,
					struct v4l2_streamparm *p)
{
	void *instance = get_vidc_inst(filp, fh);

	return msm_vidc_session(instance, msm_vidc_s_param, p, false, __func__);
}

int msm_v4l2_g_parm(struct file *filp, void *fh,
					struct v4l2_streamparm *p)
{
	void *instance = get_vidc_inst(filp, fh);

	return msm_vidc_session(instance, msm_vidc_g_param, p, false, __func__);
}

int msm_v4l2_reqbufs(struct file *filp, void *fh,
				struct v4l2_requestbuffers *data)
{
	void *instance = get_vidc_inst(filp, fh);

	return msm_vidc_session(instance, __msm_v4l2_reqbufs, data, false, __func__);
}

int msm_v4l2_querybuf(struct file *filp, void *fh,
				struct v4l2_buffer *data)
{
	void *instance = get_vidc_inst(filp, fh);

	return msm_vidc_session(instance, msm_vidc_querybuf, data, false, __func__);
}

int msm_v4l2_create_bufs(struct file *filp, void *fh,
				struct v4l2_create_buffers *data)
{
	void *instance = get_vidc_inst(filp, fh);

	return msm_vidc_session(instance, msm_vidc_create_bufs, data, false, __func__);
}

int msm_v4l2_prepare_buf(struct file *filp, void *fh,
				struct v4l2_buffer *data)
{
	void *instance = get_vidc_inst(filp, fh);

	return msm_vidc_session(instance, msm_vidc_prepare_buf, data, false, __func__);
}

int msm_v4l2_qbuf(struct file *filp, void *fh,
				struct v4l2_buffer *data)
{
	void *instance = get_vidc_inst(filp, fh);

	/*
	 * [1] If request_fd enabled, msm_vb2_buf_queue() is not called from here.
	 *   instead it's called as part of msm_v4l2_request_queue().
	 *   Hence inst lock should be acquired in common function i.e
	 *   msm_vb2_buf_queue, to handle both requests and non-request
	 *   scenarios.
	 * [2] If request_fd is disabled, inst_lock can be acquired here.
	 *   Acquiring inst_lock from here will ensure RO list insertion
	 *   and deletion i.e. attach/map will happen under lock.
	 * Currently, request_fd is disabled. Therefore, acquire inst_lock
	 * from this function to ensure RO list insertion/updation is under
	 * lock to avoid stability usecase(msm_vidc_session() will acquire
	 * inst_lock).
	 */
	return msm_vidc_session(instance, msm_vidc_qbuf, data, false, __func__);
}

int msm_v4l2_dqbuf(struct file *filp, void *fh,
				struct v4l2_buffer *data)
{
	void *instance = get_vidc_inst(filp, fh);

	return msm_vidc_session(instance, msm_vidc_dqbuf, data, true, __func__);
}

int msm_v4l2_streamon(struct file *filp, void *fh,
				enum v4l2_buf_type data)
{
	void *instance = get_vidc_inst(filp, fh);

	return msm_vidc_session(instance, msm_vidc_streamon, &data, false, __func__);
}

int msm_v4l2_streamoff(struct file *filp, void *fh,
				enum v4l2_buf_type data)
{
	void *instance = get_vidc_inst(filp, fh);

	return msm_vidc_session(instance, msm_vidc_streamoff, &data, true, __func__);
}

int msm_v4l2_subscribe_event(struct v4l2_fh *fh,
				const struct v4l2_event_subscription *data)
{
	void *instance = container_of(fh, struct msm_vidc_inst, fh);

	return msm_vidc_session(instance, msm_vidc_subscribe_event,
			(void *)data, false, __func__);
}

int msm_v4l2_unsubscribe_event(struct v4l2_fh *fh,
				const struct v4l2_event_subscription *data)
{
	void *instance = container_of(fh, struct msm_vidc_inst, fh);

	return msm_vidc_session(instance, msm_vidc_unsubscribe_event,
			(void *)data, true, __func__);
}

int msm_v4l2_try_decoder_cmd(struct file *filp, void *fh,
			     struct v4l2_decoder_cmd *data)
{
	void *instance = get_vidc_inst(filp, fh);

	return msm_vidc_session(instance, msm_vidc_try_cmd,
			(union msm_v4l2_cmd *)data, false, __func__);
}

int msm_v4l2_decoder_cmd(struct file *filp, void *fh,
				struct v4l2_decoder_cmd *dec)
{
	void *instance = get_vidc_inst(filp, fh);

	return msm_vidc_session(instance, __msm_v4l2_decoder_cmd, dec, false, __func__);
}

int msm_v4l2_try_encoder_cmd(struct file *filp, void *fh,
			     struct v4l2_encoder_cmd *enc)
{
	void *instance = get_vidc_inst(filp, fh);

	return msm_vidc_session(instance, msm_vidc_try_cmd,
			(union msm_v4l2_cmd *)enc, false, __func__);
}

int msm_v4l2_encoder_cmd(struct file *filp, void *fh,
	struct v4l2_encoder_cmd *enc)
{
	void *instance = get_vidc_inst(filp, fh);

	return msm_vidc_session(instance, __msm_v4l2_encoder_cmd, enc, false, __func__);
}

int msm_v4l2_enum_framesizes(struct file *filp, void *fh,
				struct v4l2_frmsizeenum *data)
{
	void *instance = get_vidc_inst(filp, fh);

	return msm_vidc_session(instance, msm_vidc_enum_framesizes, data, false, __func__);
}

int msm_v4l2_enum_frameintervals(struct file *filp, void *fh,
				struct v4l2_frmivalenum *data)
{
	void *instance = get_vidc_inst(filp, fh);

	return msm_vidc_session(instance, msm_vidc_enum_frameintervals, data, false, __func__);
}

int msm_v4l2_queryctrl(struct file *filp, void *fh,
	struct v4l2_queryctrl *data)
{
	void *instance = get_vidc_inst(filp, fh);

	return msm_vidc_session(instance, msm_vidc_query_ctrl, data, false, __func__);
}

int msm_v4l2_querymenu(struct file *filp, void *fh,
	struct v4l2_querymenu *data)
{
	void *instance = get_vidc_inst(filp, fh);

	return msm_vidc_session(instance, msm_vidc_query_menu, data, false, __func__);
}

int msm_v4l2_request_validate(struct media_request *req)
{
	d_vpr_l("%s()\n", __func__);
	return vb2_request_validate(req);
}

void msm_v4l2_request_queue(struct media_request *req)
{
	d_vpr_l("%s()\n", __func__);
	v4l2_m2m_request_queue(req);
}

void msm_v4l2_m2m_device_run(void *priv)
{
	d_vpr_l("%s()\n", __func__);
}

void msm_v4l2_m2m_job_abort(void *priv)
{
	struct v4l2_m2m_dev *m2m_dev = NULL;
	struct v4l2_m2m_ctx *m2m_ctx = NULL;
	struct msm_vidc_inst *inst = priv;

	if (!inst || !inst->fh.m2m_ctx) {
		d_vpr_e("%s: invalid params\n", __func__);
		return;
	}
	m2m_ctx = inst->fh.m2m_ctx;
	m2m_dev = m2m_ctx->m2m_dev;

	i_vpr_h(inst, "%s: m2m job aborted\n", __func__);
	v4l2_m2m_job_finish(m2m_dev, m2m_ctx);
}

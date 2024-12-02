/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2022-2024 Qualcomm Innovation Center, Inc. All rights reserved.
 */

#ifndef _MSM_VIDC_PLATFORM_EXT_H_
#define _MSM_VIDC_PLATFORM_EXT_H_

struct v4l2_ctrl;
enum msm_vidc_inst_capability_type;

/* Control to enable output buffer TX fence (video device signal) feature */
#define V4L2_CID_MPEG_VIDC_METADATA_OUTPUT_TX_FENCE                          \
	(V4L2_CID_MPEG_VIDC_BASE + 0x38)
/* Control to set fence id to driver in order to get corresponding fence fd */
#define V4L2_CID_MPEG_VIDC_OUTPUT_TX_FENCE_ID                                \
	(V4L2_CID_MPEG_VIDC_BASE + 0x39)
/* Control to get fence fd from driver for the fence id */
#define V4L2_CID_MPEG_VIDC_OUTPUT_TX_FENCE_FD                                \
	(V4L2_CID_MPEG_VIDC_BASE + 0x3A)
#define V4L2_CID_MPEG_VIDC_METADATA_PICTURE_TYPE                             \
	(V4L2_CID_MPEG_VIDC_BASE + 0x3B)

/* Encoder Slice Delivery Mode
 * set format has a dependency on this control
 * and gets invoked when this control is updated.
 */
#define V4L2_CID_MPEG_VIDC_HEVC_ENCODE_DELIVERY_MODE                         \
	(V4L2_CID_MPEG_VIDC_BASE + 0x3C)

#define V4L2_CID_MPEG_VIDC_H264_ENCODE_DELIVERY_MODE                         \
	(V4L2_CID_MPEG_VIDC_BASE + 0x3D)

#define V4L2_CID_MPEG_VIDC_CRITICAL_PRIORITY                                 \
	(V4L2_CID_MPEG_VIDC_BASE + 0x3E)
#define V4L2_CID_MPEG_VIDC_RESERVE_DURATION                                  \
	(V4L2_CID_MPEG_VIDC_BASE + 0x3F)

/* Control to enable early notify feature */
#define V4L2_CID_MPEG_VIDC_EARLY_NOTIFY_ENABLE                               \
	(V4L2_CID_MPEG_VIDC_BASE + 0x44)

/* Control to configure line count to get partial decode completion notification */
#define V4L2_CID_MPEG_VIDC_EARLY_NOTIFY_LINE_COUNT                           \
	(V4L2_CID_MPEG_VIDC_BASE + 0x45)

/* Control to enable input buffer RX fence (video device wait) feature */
#define V4L2_CID_MPEG_VIDC_INPUT_RX_FENCE_ENABLE                             \
	(V4L2_CID_MPEG_VIDC_BASE + 0x4F)

/*
 * Control to set fence fd to the driver for each I/P buf
 * set via V4L2_CID_MPEG_VIDC_INPUT_FENCE_FD
 */
#define V4L2_CID_MPEG_VIDC_INPUT_RX_FENCE_FD                                 \
	(V4L2_CID_MPEG_VIDC_BASE + 0x50)

/* Control to set input buffer RX fence (video device wait) type */
#define V4L2_CID_MPEG_VIDC_INPUT_RX_FENCE_TYPE                               \
	(V4L2_CID_MPEG_VIDC_BASE + 0x51)
/* Control to set output buffer TX fence (video device signal) type */
#define V4L2_CID_MPEG_VIDC_OUTPUT_TX_FENCE_TYPE                              \
	(V4L2_CID_MPEG_VIDC_BASE + 0x52)
enum v4l2_mpeg_vidc_fence_type {
	V4L2_MPEG_VIDC_FENCE_NONE       = 0,
	V4L2_MPEG_VIDC_FENCE_SW         = 1,
	V4L2_MPEG_VIDC_FENCE_SYNX_V2    = 2,
};

/* Control to set offset in metadata buffer where extra data can be present */
#define V4L2_CID_MPEG_VIDC_INPUT_EXTRA_METADATA_OFFSET                       \
	(V4L2_CID_MPEG_VIDC_BASE + 0x53)

int msm_vidc_adjust_ir_period(void *instance, struct v4l2_ctrl *ctrl);
int msm_vidc_adjust_dec_frame_rate(void *instance, struct v4l2_ctrl *ctrl);
int msm_vidc_adjust_dec_operating_rate(void *instance, struct v4l2_ctrl *ctrl);
int msm_vidc_adjust_delivery_mode(void *instance, struct v4l2_ctrl *ctrl);
int msm_vidc_set_ir_period(void *instance,
			   enum msm_vidc_inst_capability_type cap_id);
int msm_vidc_set_signal_color_info(void *instance,
				   enum msm_vidc_inst_capability_type cap_id);
int msm_vidc_adjust_csc(void *instance, struct v4l2_ctrl *ctrl);
int msm_vidc_adjust_csc_custom_matrix(void *instance, struct v4l2_ctrl *ctrl);
int msm_vidc_adjust_fence_info(void *instance, struct v4l2_ctrl *ctrl);

#endif

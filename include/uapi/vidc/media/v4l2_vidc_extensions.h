/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
/*
 * Copyright (c) 2020-2021, The Linux Foundation. All rights reserved.
 * Copyright (c) 2023-2024 Qualcomm Innovation Center, Inc. All rights reserved.
 */

#ifndef __V4l2_VIDC_EXTENSIONS_H__
#define __V4l2_VIDC_EXTENSIONS_H__

#include <linux/types.h>
#include <linux/v4l2-controls.h>

/* AV1 */
#ifndef V4L2_PIX_FMT_AV1
#define V4L2_PIX_FMT_AV1                        v4l2_fourcc('A', 'V', '1', '0')
#endif

/* HEIC encoder and decoder */
#define V4L2_PIX_FMT_VIDC_HEIC                  v4l2_fourcc('H', 'E', 'I', 'C')

#define V4L2_META_FMT_VIDC                      v4l2_fourcc('Q', 'M', 'E', 'T')

#ifndef V4L2_MPEG_VIDEO_HEVC_PROFILE_MAIN_10_STILL_PICTURE
#define V4L2_MPEG_VIDEO_HEVC_PROFILE_MAIN_10_STILL_PICTURE    (3)
#endif

/* vendor controls start */
#ifdef V4L2_CTRL_CLASS_CODEC
#define V4L2_CID_MPEG_VIDC_BASE (V4L2_CTRL_CLASS_CODEC | 0x2000)
#else
#define V4L2_CID_MPEG_VIDC_BASE (V4L2_CTRL_CLASS_MPEG | 0x2000)
#endif

#define V4L2_MPEG_MSM_VIDC_DISABLE 0

#define V4L2_MPEG_MSM_VIDC_ENABLE 1

#define VIDC_BASE (V4L2_CID_MPEG_VIDC_BASE)

#define V4L2_CID_MPEG_VIDC_SECURE                             (VIDC_BASE + 0x1)

#define V4L2_CID_MPEG_VIDC_LOWLATENCY_REQUEST                 (VIDC_BASE + 0x3)

#define V4L2_CID_MPEG_VIDC_CODEC_CONFIG                       (VIDC_BASE + 0x4)

#define V4L2_CID_MPEG_VIDC_FRAME_RATE                         (VIDC_BASE + 0x5)

#define V4L2_CID_MPEG_VIDC_OPERATING_RATE                     (VIDC_BASE + 0x6)

#define V4L2_CID_MPEG_VIDC_TIME_DELTA_BASED_RC                (VIDC_BASE + 0xD)

/* Encoder quality controls */
#define V4L2_CID_MPEG_VIDC_CONTENT_ADAPTIVE_CODING            (VIDC_BASE + 0xE)

#define V4L2_CID_MPEG_VIDC_QUALITY_BITRATE_BOOST              (VIDC_BASE + 0xF)

#define V4L2_CID_MPEG_VIDC_BLUR_TYPES                        (VIDC_BASE + 0x10)
enum v4l2_mpeg_vidc_blur_types {
	VIDC_BLUR_NONE               = 0x0,
	VIDC_BLUR_EXTERNAL           = 0x1,
	VIDC_BLUR_ADAPTIVE           = 0x2,
};
/* (blur width) << 16 | (blur height) */
#define V4L2_CID_MPEG_VIDC_BLUR_RESOLUTION                   (VIDC_BASE + 0x11)

#define V4L2_CID_MPEG_VIDC_CSC_CUSTOM_MATRIX                 (VIDC_BASE + 0x12)

#define V4L2_CID_MPEG_VIDC_METADATA_LTR_MARK_USE_DETAILS     (VIDC_BASE + 0x13)

#define V4L2_CID_MPEG_VIDC_METADATA_SEQ_HEADER_NAL           (VIDC_BASE + 0x14)

#define V4L2_CID_MPEG_VIDC_METADATA_DPB_LUMA_CHROMA_MISR     (VIDC_BASE + 0x15)

#define V4L2_CID_MPEG_VIDC_METADATA_OPB_LUMA_CHROMA_MISR     (VIDC_BASE + 0x16)

#define V4L2_CID_MPEG_VIDC_METADATA_INTERLACE                (VIDC_BASE + 0x17)

#define V4L2_CID_MPEG_VIDC_METADATA_CONCEALED_MB_COUNT       (VIDC_BASE + 0x18)

#define V4L2_CID_MPEG_VIDC_METADATA_HISTOGRAM_INFO           (VIDC_BASE + 0x19)

#define V4L2_CID_MPEG_VIDC_METADATA_SEI_MDCV                 (VIDC_BASE + 0x1A)

#define V4L2_CID_MPEG_VIDC_METADATA_SEI_CLL                  (VIDC_BASE + 0x1B)

#define V4L2_CID_MPEG_VIDC_METADATA_HDR10PLUS                (VIDC_BASE + 0x1C)

#define V4L2_CID_MPEG_VIDC_METADATA_EVA_STATS                (VIDC_BASE + 0x1D)

#define V4L2_CID_MPEG_VIDC_METADATA_BUFFER_TAG               (VIDC_BASE + 0x1E)

#define V4L2_CID_MPEG_VIDC_METADATA_SUBFRAME_OUTPUT          (VIDC_BASE + 0x1F)

#define V4L2_CID_MPEG_VIDC_METADATA_ROI_INFO                 (VIDC_BASE + 0x20)

#define V4L2_CID_MPEG_VIDC_METADATA_TIMESTAMP                (VIDC_BASE + 0x21)

#define V4L2_CID_MPEG_VIDC_METADATA_ENC_QP_METADATA          (VIDC_BASE + 0x22)

#define V4L2_CID_MPEG_VIDC_MIN_BITSTREAM_SIZE_OVERWRITE      (VIDC_BASE + 0x23)

#define V4L2_CID_MPEG_VIDC_METADATA_BITSTREAM_RESOLUTION     (VIDC_BASE + 0x24)

#define V4L2_CID_MPEG_VIDC_METADATA_CROP_OFFSETS             (VIDC_BASE + 0x25)

#define V4L2_CID_MPEG_VIDC_METADATA_SALIENCY_INFO            (VIDC_BASE + 0x26)

#define V4L2_CID_MPEG_VIDC_METADATA_TRANSCODE_STAT_INFO      (VIDC_BASE + 0x27)

/* Encoder Super frame control */
#define V4L2_CID_MPEG_VIDC_SUPERFRAME                        (VIDC_BASE + 0x28)

/* Thumbnail Mode control */
#define V4L2_CID_MPEG_VIDC_THUMBNAIL_MODE                    (VIDC_BASE + 0x29)

/* Priority control */
#define V4L2_CID_MPEG_VIDC_PRIORITY                          (VIDC_BASE + 0x2A)

/* Metadata DPB Tag List*/
#define V4L2_CID_MPEG_VIDC_METADATA_DPB_TAG_LIST             (VIDC_BASE + 0x2B)

/* Encoder Input Compression Ratio control */
#define V4L2_CID_MPEG_VIDC_COMPRESSION_RATIO                 (VIDC_BASE + 0x2C)

#define V4L2_CID_MPEG_VIDC_METADATA_QP                       (VIDC_BASE + 0x2E)

/* Encoder Complexity control */
#define V4L2_CID_MPEG_VIDC_COMPLEXITY                        (VIDC_BASE + 0x2F)

/* Decoder Max Number of Reorder Frames */
#define V4L2_CID_MPEG_VIDC_METADATA_MAX_NUM_REORDER_FRAMES   (VIDC_BASE + 0x30)

/* Control IDs for AV1 */
#define V4L2_CID_MPEG_VIDC_AV1_PROFILE                       (VIDC_BASE + 0x31)
enum v4l2_mpeg_vidc_av1_profile {
	V4L2_MPEG_VIDC_AV1_PROFILE_MAIN            = 0,
	V4L2_MPEG_VIDC_AV1_PROFILE_HIGH            = 1,
	V4L2_MPEG_VIDC_AV1_PROFILE_PROFESSIONAL    = 2,
};

#define V4L2_CID_MPEG_VIDC_AV1_LEVEL                         (VIDC_BASE + 0x32)
enum v4l2_mpeg_vidc_av1_level {
	V4L2_MPEG_VIDC_AV1_LEVEL_2_0  = 0,
	V4L2_MPEG_VIDC_AV1_LEVEL_2_1  = 1,
	V4L2_MPEG_VIDC_AV1_LEVEL_2_2  = 2,
	V4L2_MPEG_VIDC_AV1_LEVEL_2_3  = 3,
	V4L2_MPEG_VIDC_AV1_LEVEL_3_0  = 4,
	V4L2_MPEG_VIDC_AV1_LEVEL_3_1  = 5,
	V4L2_MPEG_VIDC_AV1_LEVEL_3_2  = 6,
	V4L2_MPEG_VIDC_AV1_LEVEL_3_3  = 7,
	V4L2_MPEG_VIDC_AV1_LEVEL_4_0  = 8,
	V4L2_MPEG_VIDC_AV1_LEVEL_4_1  = 9,
	V4L2_MPEG_VIDC_AV1_LEVEL_4_2  = 10,
	V4L2_MPEG_VIDC_AV1_LEVEL_4_3  = 11,
	V4L2_MPEG_VIDC_AV1_LEVEL_5_0  = 12,
	V4L2_MPEG_VIDC_AV1_LEVEL_5_1  = 13,
	V4L2_MPEG_VIDC_AV1_LEVEL_5_2  = 14,
	V4L2_MPEG_VIDC_AV1_LEVEL_5_3  = 15,
	V4L2_MPEG_VIDC_AV1_LEVEL_6_0  = 16,
	V4L2_MPEG_VIDC_AV1_LEVEL_6_1  = 17,
	V4L2_MPEG_VIDC_AV1_LEVEL_6_2  = 18,
	V4L2_MPEG_VIDC_AV1_LEVEL_6_3  = 19,
	V4L2_MPEG_VIDC_AV1_LEVEL_7_0  = 20,
	V4L2_MPEG_VIDC_AV1_LEVEL_7_1  = 21,
	V4L2_MPEG_VIDC_AV1_LEVEL_7_2  = 22,
	V4L2_MPEG_VIDC_AV1_LEVEL_7_3  = 23,
};

#define V4L2_CID_MPEG_VIDC_AV1_TIER                          (VIDC_BASE + 0x33)
enum v4l2_mpeg_vidc_av1_tier {
	V4L2_MPEG_VIDC_AV1_TIER_MAIN  = 0,
	V4L2_MPEG_VIDC_AV1_TIER_HIGH  = 1,
};
/* Decoder Timestamp Reorder control */
#define V4L2_CID_MPEG_VIDC_TS_REORDER                        (VIDC_BASE + 0x34)

/* AV1 Decoder Film Grain */
#define V4L2_CID_MPEG_VIDC_FILM_GRAIN_PRESENT                (VIDC_BASE + 0x35)

#define V4L2_CID_MPEG_VIDC_METADATA_DOLBY_RPU                (VIDC_BASE + 0x40)

#define V4L2_CID_MPEG_VIDC_CLIENT_ID                         (VIDC_BASE + 0x41)

#define V4L2_CID_MPEG_VIDC_LAST_FLAG_EVENT_ENABLE            (VIDC_BASE + 0x42)

#define V4L2_CID_MPEG_VIDC_VUI_TIMING_INFO                   (VIDC_BASE + 0x43)

/*
 * This control is introduced to overcome v4l2 limitation
 * of allowing only standard colorspace info via s_fmt.
 * v4l_sanitize_colorspace() is introduced in s_fmt ioctl
 * to reject private colorspace. Through this control, client
 * can set private colorspace info and/or use this control
 * to set colorspace dynamically.
 * The control value is 32 bits packed as:
 *      [ 0 -  7] : matrix coefficients
 *      [ 8 - 15] : transfer characteristics
 *      [16 - 23] : colour primaries
 *      [24 - 31] : range
 * This control is only for encoder.
 * Currently g_fmt in v4l2 does not santize colorspace,
 * hence this control is not introduced for decoder.
 */
#define V4L2_CID_MPEG_VIDC_SIGNAL_COLOR_INFO                 (VIDC_BASE + 0x46)

/* control to enable csc */
#define V4L2_CID_MPEG_VIDC_CSC                               (VIDC_BASE + 0x47)

#define V4L2_CID_MPEG_VIDC_DRIVER_VERSION                    (VIDC_BASE + 0x48)

#define V4L2_CID_MPEG_VIDC_GRID_WIDTH                        (VIDC_BASE + 0x49)

#define V4L2_CID_MPEG_VIDC_MAX_NUM_REORDER_FRAMES            (VIDC_BASE + 0x4A)

#define V4L2_CID_MPEG_VIDC_INTERLACE                         (VIDC_BASE + 0x4B)

#define V4L2_CID_MPEG_VIDC_OPEN_GOP_ENABLE                   (VIDC_BASE + 0x4C)

#define V4L2_CID_MPEG_VIDC_METADATA_HDR10_MAX_RGB_INFO       (VIDC_BASE + 0x4D)

#define V4L2_CID_MPEG_VIDC_CAPTURE_DATA_OFFSET               (VIDC_BASE + 0x4E)

#endif

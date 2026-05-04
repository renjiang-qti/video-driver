// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#include <linux/err.h>

#include "msm_vidc_platform.h"
#include "msm_vidc_debug.h"

#if MSM_VIDC_HAS_QCOM_UBWC_HEADER
int msm_vidc_update_ubwc_config(struct msm_vidc_core *core)
{
	const struct qcom_ubwc_cfg_data *ubwc_cfg;

	ubwc_cfg = qcom_ubwc_config_get_data();
	if (IS_ERR(ubwc_cfg)) {
		long rc = PTR_ERR(ubwc_cfg);

		if (rc == -EINVAL) {
			/*
			 * Platform UBWC data is not yet present in the kernel
			 * database.  Fall back to the per-platform defaults
			 * already set in platform data.
			 */
			d_vpr_h("%s: no global UBWC config, using platform defaults\n",
				__func__);
			return 0;
		}

		d_vpr_e("%s: failed to get global UBWC config: %ld\n",
			__func__, rc);
		return rc;
	}

	core->platform->data.ubwc_config = ubwc_cfg;

	d_vpr_h("%s: global UBWC config applied\n", __func__);

	return 0;
}
#else
int msm_vidc_update_ubwc_config(struct msm_vidc_core *core)
{
	return 0;
}
#endif

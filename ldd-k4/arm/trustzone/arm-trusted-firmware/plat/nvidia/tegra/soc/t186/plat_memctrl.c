/*
 * Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <bl_common.h>
#include <memctrl_v2.h>

/*******************************************************************************
 * Array to hold stream_id override config register offsets
 ******************************************************************************/
const static uint32_t tegra186_streamid_override_regs[] = {
	MC_STREAMID_OVERRIDE_CFG_PTCR,
	MC_STREAMID_OVERRIDE_CFG_AFIR,
	MC_STREAMID_OVERRIDE_CFG_HDAR,
	MC_STREAMID_OVERRIDE_CFG_HOST1XDMAR,
	MC_STREAMID_OVERRIDE_CFG_NVENCSRD,
	MC_STREAMID_OVERRIDE_CFG_SATAR,
	MC_STREAMID_OVERRIDE_CFG_MPCORER,
	MC_STREAMID_OVERRIDE_CFG_NVENCSWR,
	MC_STREAMID_OVERRIDE_CFG_AFIW,
	MC_STREAMID_OVERRIDE_CFG_HDAW,
	MC_STREAMID_OVERRIDE_CFG_MPCOREW,
	MC_STREAMID_OVERRIDE_CFG_SATAW,
	MC_STREAMID_OVERRIDE_CFG_ISPRA,
	MC_STREAMID_OVERRIDE_CFG_ISPWA,
	MC_STREAMID_OVERRIDE_CFG_ISPWB,
	MC_STREAMID_OVERRIDE_CFG_XUSB_HOSTR,
	MC_STREAMID_OVERRIDE_CFG_XUSB_HOSTW,
	MC_STREAMID_OVERRIDE_CFG_XUSB_DEVR,
	MC_STREAMID_OVERRIDE_CFG_XUSB_DEVW,
	MC_STREAMID_OVERRIDE_CFG_TSECSRD,
	MC_STREAMID_OVERRIDE_CFG_TSECSWR,
	MC_STREAMID_OVERRIDE_CFG_GPUSRD,
	MC_STREAMID_OVERRIDE_CFG_GPUSWR,
	MC_STREAMID_OVERRIDE_CFG_SDMMCRA,
	MC_STREAMID_OVERRIDE_CFG_SDMMCRAA,
	MC_STREAMID_OVERRIDE_CFG_SDMMCR,
	MC_STREAMID_OVERRIDE_CFG_SDMMCRAB,
	MC_STREAMID_OVERRIDE_CFG_SDMMCWA,
	MC_STREAMID_OVERRIDE_CFG_SDMMCWAA,
	MC_STREAMID_OVERRIDE_CFG_SDMMCW,
	MC_STREAMID_OVERRIDE_CFG_SDMMCWAB,
	MC_STREAMID_OVERRIDE_CFG_VICSRD,
	MC_STREAMID_OVERRIDE_CFG_VICSWR,
	MC_STREAMID_OVERRIDE_CFG_VIW,
	MC_STREAMID_OVERRIDE_CFG_NVDECSRD,
	MC_STREAMID_OVERRIDE_CFG_NVDECSWR,
	MC_STREAMID_OVERRIDE_CFG_APER,
	MC_STREAMID_OVERRIDE_CFG_APEW,
	MC_STREAMID_OVERRIDE_CFG_NVJPGSRD,
	MC_STREAMID_OVERRIDE_CFG_NVJPGSWR,
	MC_STREAMID_OVERRIDE_CFG_SESRD,
	MC_STREAMID_OVERRIDE_CFG_SESWR,
	MC_STREAMID_OVERRIDE_CFG_ETRR,
	MC_STREAMID_OVERRIDE_CFG_ETRW,
	MC_STREAMID_OVERRIDE_CFG_TSECSRDB,
	MC_STREAMID_OVERRIDE_CFG_TSECSWRB,
	MC_STREAMID_OVERRIDE_CFG_GPUSRD2,
	MC_STREAMID_OVERRIDE_CFG_GPUSWR2,
	MC_STREAMID_OVERRIDE_CFG_AXISR,
	MC_STREAMID_OVERRIDE_CFG_AXISW,
	MC_STREAMID_OVERRIDE_CFG_EQOSR,
	MC_STREAMID_OVERRIDE_CFG_EQOSW,
	MC_STREAMID_OVERRIDE_CFG_UFSHCR,
	MC_STREAMID_OVERRIDE_CFG_UFSHCW,
	MC_STREAMID_OVERRIDE_CFG_NVDISPLAYR,
	MC_STREAMID_OVERRIDE_CFG_BPMPR,
	MC_STREAMID_OVERRIDE_CFG_BPMPW,
	MC_STREAMID_OVERRIDE_CFG_BPMPDMAR,
	MC_STREAMID_OVERRIDE_CFG_BPMPDMAW,
	MC_STREAMID_OVERRIDE_CFG_AONR,
	MC_STREAMID_OVERRIDE_CFG_AONW,
	MC_STREAMID_OVERRIDE_CFG_AONDMAR,
	MC_STREAMID_OVERRIDE_CFG_AONDMAW,
	MC_STREAMID_OVERRIDE_CFG_SCER,
	MC_STREAMID_OVERRIDE_CFG_SCEW,
	MC_STREAMID_OVERRIDE_CFG_SCEDMAR,
	MC_STREAMID_OVERRIDE_CFG_SCEDMAW,
	MC_STREAMID_OVERRIDE_CFG_APEDMAR,
	MC_STREAMID_OVERRIDE_CFG_APEDMAW,
	MC_STREAMID_OVERRIDE_CFG_NVDISPLAYR1,
	MC_STREAMID_OVERRIDE_CFG_VICSRD1,
	MC_STREAMID_OVERRIDE_CFG_NVDECSRD1
};

/*******************************************************************************
 * Array to hold the security configs for stream IDs
 ******************************************************************************/
const static mc_streamid_security_cfg_t tegra186_streamid_sec_cfgs[] = {
	mc_make_sec_cfg(SCEW, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(AFIR, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(AFIW, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(NVDISPLAYR1, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(XUSB_DEVR, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(VICSRD1, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(NVENCSWR, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(TSECSRDB, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(AXISW, SECURE, NO_OVERRIDE, DISABLE),
	mc_make_sec_cfg(SDMMCWAB, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(AONDMAW, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(GPUSWR2, SECURE, NO_OVERRIDE, DISABLE),
	mc_make_sec_cfg(SATAW, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(UFSHCW, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(SDMMCR, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(SCEDMAW, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(UFSHCR, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(SDMMCWAA, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(SESWR, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(MPCORER, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(PTCR, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(BPMPW, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(ETRW, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(GPUSRD, SECURE, NO_OVERRIDE, DISABLE),
	mc_make_sec_cfg(VICSWR, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(SCEDMAR, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(HDAW, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(ISPWA, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(EQOSW, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(XUSB_HOSTW, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(TSECSWR, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(SDMMCRAA, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(VIW, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(AXISR, SECURE, NO_OVERRIDE, DISABLE),
	mc_make_sec_cfg(SDMMCW, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(BPMPDMAW, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(ISPRA, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(NVDECSWR, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(XUSB_DEVW, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(NVDECSRD, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(MPCOREW, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(NVDISPLAYR, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(BPMPDMAR, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(NVJPGSWR, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(NVDECSRD1, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(TSECSRD, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(NVJPGSRD, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(SDMMCWA, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(SCER, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(XUSB_HOSTR, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(VICSRD, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(AONDMAR, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(AONW, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(SDMMCRA, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(HOST1XDMAR, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(EQOSR, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(SATAR, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(BPMPR, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(HDAR, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(SDMMCRAB, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(ETRR, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(AONR, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(SESRD, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(NVENCSRD, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(GPUSWR, SECURE, NO_OVERRIDE, DISABLE),
	mc_make_sec_cfg(TSECSWRB, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(ISPWB, NON_SECURE, OVERRIDE, ENABLE),
	mc_make_sec_cfg(GPUSRD2, SECURE, NO_OVERRIDE, DISABLE),
	mc_make_sec_cfg(APEDMAW, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(APER, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(APEW, NON_SECURE, NO_OVERRIDE, ENABLE),
	mc_make_sec_cfg(APEDMAR, NON_SECURE, NO_OVERRIDE, ENABLE),
};

/*******************************************************************************
 * Array to hold the transaction override configs
 ******************************************************************************/
const static mc_txn_override_cfg_t tegra186_txn_override_cfgs[] = {
	mc_make_txn_override_cfg(BPMPW, CGID_TAG_ADR),
	mc_make_txn_override_cfg(EQOSW, CGID_TAG_ADR),
	mc_make_txn_override_cfg(NVJPGSWR, CGID_TAG_ADR),
	mc_make_txn_override_cfg(SDMMCWAA, CGID_TAG_ADR),
	mc_make_txn_override_cfg(MPCOREW, CGID_TAG_ADR),
	mc_make_txn_override_cfg(SCEDMAW, CGID_TAG_ADR),
	mc_make_txn_override_cfg(SDMMCW, CGID_TAG_ADR),
	mc_make_txn_override_cfg(AXISW, CGID_TAG_ADR),
	mc_make_txn_override_cfg(TSECSWR, CGID_TAG_ADR),
	mc_make_txn_override_cfg(GPUSWR, CGID_TAG_ADR),
	mc_make_txn_override_cfg(XUSB_HOSTW, CGID_TAG_ADR),
	mc_make_txn_override_cfg(TSECSWRB, CGID_TAG_ADR),
	mc_make_txn_override_cfg(GPUSWR2, CGID_TAG_ADR),
	mc_make_txn_override_cfg(AONDMAW, CGID_TAG_ADR),
	mc_make_txn_override_cfg(AONW, CGID_TAG_ADR),
	mc_make_txn_override_cfg(SESWR, CGID_TAG_ADR),
	mc_make_txn_override_cfg(BPMPDMAW, CGID_TAG_ADR),
	mc_make_txn_override_cfg(SDMMCWA, CGID_TAG_ADR),
	mc_make_txn_override_cfg(HDAW, CGID_TAG_ADR),
	mc_make_txn_override_cfg(NVDECSWR, CGID_TAG_ADR),
	mc_make_txn_override_cfg(UFSHCW, CGID_TAG_ADR),
	mc_make_txn_override_cfg(SATAW, CGID_TAG_ADR),
	mc_make_txn_override_cfg(ETRW, CGID_TAG_ADR),
	mc_make_txn_override_cfg(VICSWR, CGID_TAG_ADR),
	mc_make_txn_override_cfg(NVENCSWR, CGID_TAG_ADR),
	mc_make_txn_override_cfg(SDMMCWAB, CGID_TAG_ADR),
	mc_make_txn_override_cfg(ISPWB, CGID_TAG_ADR),
	mc_make_txn_override_cfg(APEW, CGID_TAG_ADR),
	mc_make_txn_override_cfg(XUSB_DEVW, CGID_TAG_ADR),
	mc_make_txn_override_cfg(AFIW, CGID_TAG_ADR),
	mc_make_txn_override_cfg(SCEW, CGID_TAG_ADR),
};

/*******************************************************************************
 * Struct to hold the memory controller settings
 ******************************************************************************/
static tegra_mc_settings_t tegra186_mc_settings = {
	.streamid_override_cfg = tegra186_streamid_override_regs,
	.num_streamid_override_cfgs = ARRAY_SIZE(tegra186_streamid_override_regs),
	.streamid_security_cfg = tegra186_streamid_sec_cfgs,
	.num_streamid_security_cfgs = ARRAY_SIZE(tegra186_streamid_sec_cfgs),
	.txn_override_cfg = tegra186_txn_override_cfgs,
	.num_txn_override_cfgs = ARRAY_SIZE(tegra186_txn_override_cfgs)
};

/*******************************************************************************
 * Handler to return the pointer to the memory controller's settings struct
 ******************************************************************************/
tegra_mc_settings_t *tegra_get_mc_settings(void)
{
	return &tegra186_mc_settings;
}
